#include <JEngine/IO/VFS/VFS.h>

namespace JEngine {
    static bool compareStream(FileStream* const& stream, const ConstSpan<char>& span) {
        return stream && IO::pathsAreEqual(span, ConstSpan<char>(stream->getFilePath().c_str(), stream->getFilePath().size()));
    }

    static size_t calculateLongestPath(size_t initLen, FileEntry* entry) {
        std::vector<FileEntry*> fileNodes{};
        if (entry) {

        }
        return initLen;
    }

    VFS::VFS() : _rootPath(), _root(nullptr), _rootSpan{}, _entryAllocator(), _fileAllocator() {}

    VFS::VFS(const std::string& root) : _rootPath(root), _root(nullptr), _entryAllocator(), _fileAllocator() {
        _root = _entryAllocator.allocate();

        IO::fixPath(_rootPath);
        ConstSpan<char> temp(_rootPath.c_str(), _rootPath.length());
        temp = temp.trim('/');
        _rootSpan = temp;

        int32_t lastSlash = temp.indexOfLast('/');
        UUID8 uuid = UUID8::Empty;
        HashState state{};

        temp = temp.slice(lastSlash + 1);
        uuid.updateHash(state, temp.get(), temp.length());
        _root->setup(nullptr, temp, uuid, _entries.push(_root), -1, FOLDER_EINF);
        _fileEntryLUT.insert(std::make_pair(uuid, _root->index));
    }

    VFS::~VFS() { }

    void VFS::changeRoot(const std::string& root) {
        if(root == _rootPath) {
            JENGINE_CORE_WARN("[J-Engine - VFS] Warning: Given root path is already setup! Nothing changed.");
            return;
        }

        char* tempBuf = reinterpret_cast<char*>(_malloca(root.length() + 1));
        if (!tempBuf) { return; }
        tempBuf[root.length()] = 0;
        memcpy(tempBuf, root.c_str(), root.length());
        IO::fixPath(tempBuf);
        ConstSpan<char> newRoot(tempBuf, root.length());
        ConstSpan<char> oldRoot(_rootPath.c_str(), _rootPath.length());

        newRoot = newRoot.trim('/');
        oldRoot = oldRoot.trim('/');

        std::string temp{};

        //By default we assume that the root has changed and all files are present under the new root
        for (size_t i = 0; i < _files.tail(); i++) {
            auto file = _files[i];
            if (file) {
                auto& path = file->getFilePath();
                size_t ind = path.find(oldRoot.get(), oldRoot.length());
                if (ind != std::string::npos) {
                    bool wasOpen = file->isOpen();
                    const char* fMode = file->getCFileMode();

                    file->close();
                    ind += oldRoot.length();
                    size_t endPart = (path.length() - ind);
                    temp.clear();
                    temp.reserve(newRoot.length() + endPart);
                    temp.append(newRoot.get(), newRoot.length());
                    temp.append(path.c_str(), endPart);
                    file->setFilepath(temp);

                    if (wasOpen) {
                        file->open(fMode);
                    }
                }
            }
        }

        int32_t lastSlash = newRoot.indexOfLast('/');
        ConstSpan<char> newRootName = newRoot.slice(lastSlash + 1);
        _root->name = std::string(newRootName.get(), newRootName.length());
        _root->updateHash();

        _fileEntryLUT.clear();
        for (int32_t i = 0; i < _entries.tail(); i++) {
            if (FileEntry* fEnt = _entries[i]) {
                _fileEntryLUT.insert(std::make_pair(fEnt->hash, i));
            }
        }
        _freea(tempBuf);

        _rootPath = std::string(newRoot.get(), newRoot.length());
        _rootSpan = ConstSpan<char>(_rootPath.c_str(), _rootPath.length());
    }

    bool VFS::buildFromRoot() {
        if (_rootSpan.length() < 1 || !IO::exists(_rootSpan.get(), _rootSpan.length())) {
            JENGINE_CORE_ERROR("[J-Engine - VFS] Error: Failed to build VFS from root path '{0}' (Either the given directory doesn't exist or path string is empty)", _rootPath.c_str());
            return false; 
        }

        fs::path rootP(_rootSpan.get(), _rootSpan.get() + _rootSpan.length());
        std::vector<IO::FilePath> paths{};
        std::string temp{};
        temp.reserve(260);
        if (IO::getAll(rootP, IO::F_TYPE_ALL, paths, true)) {
            for (auto& pth : paths) {
                bool isFile = pth.type == IO::F_TYPE_FILE;
                temp = pth.path.string();
                addEntry(-1, isFile, temp, isFile ? FILE_EINF : FOLDER_EINF);
            }
        }
        return true;
    }

    int32_t VFS::addEntry(int32_t source, bool isFile, const char* path, size_t len, EntryInfo dataInfo) {
        char* tempBuf = reinterpret_cast<char*>(_malloca(len + 1));
        if (!tempBuf) { return false; }
        tempBuf[len] = 0;
        memcpy(tempBuf, path, len);
        IO::fixPath(tempBuf);
        Span<char> pathSpn(tempBuf, len);

        if (source < 0 && isFile && dataInfo.size == EntryInfo::NPOS) {
            source = indexOfSourceFile(pathSpn);
            if (source < 0) {
                source = _files.push(_fileAllocator.allocate(pathSpn.get(), pathSpn.length()));
            }
        }

        Span<char>* pathParts = nullptr;
        UUID8 uuid = UUID8::Empty;
        HashState state{};

        if (source < 0 && isFile) {
            JENGINE_CORE_WARN("[J-Engine - VFS] Warning: Given path '{0}' couldn't be added! (File source is invalid, if part of a JPAK add the file beforehand)", tempBuf);
            goto failure;
        }

        int32_t rootP = indexOf(pathSpn, _rootSpan);
        if (rootP > -1) {
            pathSpn = pathSpn.slice(rootP + _rootSpan.length());
        }

        FileEntry* current = _root;
        size_t slashCount = pathSpn.getOccurenceCount('/') + 1;
        size_t partCount = 0;
        pathParts = reinterpret_cast<Span<char>*>(_malloca(slashCount * sizeof(Span<char>)));

        int32_t lastInd = -1;
        if (pathParts && (partCount = pathSpn.split(pathParts, slashCount, '/')) && current) {
            if (!current->isSame(pathParts[0])) {
                uuid.updateHash(state, current->name.c_str(), current->name.length(), FileEntry::PATH_HASH_BLOCK_SIZE);
                current = current->findInChild(pathParts[0]);
            }
            bool isLast = partCount == 1;

            if (current == nullptr) {
                current = _root->children.emplace_back(_entryAllocator.allocate());
                lastInd = _entries.push(current);
                uuid.updateHash(state, pathParts[0].get(), pathParts[0].length(), FileEntry::PATH_HASH_BLOCK_SIZE);
                current->setup(_root, pathParts[0], uuid, lastInd, source, isLast ? dataInfo : FOLDER_EINF);
                _fileEntryLUT.insert(std::make_pair(uuid, lastInd));
            }

            if (current) {
                for (size_t i = 1; i < partCount; i++) {
                    auto& part = pathParts[i];
                    isLast = i >= partCount - 1;

                    auto find = current->findInChild(part);
                    if (find == nullptr) {
                        find = current->children.emplace_back(_entryAllocator.allocate());
                        lastInd = _entries.push(find);
                        uuid.updateHash(state, part.get(), part.length(), FileEntry::PATH_HASH_BLOCK_SIZE);
                        find->setup(current, part, uuid, lastInd, source, isLast ? dataInfo : FOLDER_EINF);
                        _fileEntryLUT.insert(std::make_pair(uuid, lastInd));
                    }
                    else if (isLast) {
                        lastInd = _entries.indexOf(find);
                    }
                    current = find;
                }
            }
            else {
                JENGINE_CORE_WARN("[J-Engine - VFS] Warning: Given path '{0}' isn't a valid path!", tempBuf);
                goto failure;
            }
            _freea(pathParts);
            _freea(tempBuf);
            return lastInd;
        }

    failure:
        if (pathParts) {
            _freea(pathParts);
        }
        _freea(tempBuf);
        return -1;
    }

    int32_t VFS::addEntry(int32_t source, bool isFile, const char* path, EntryInfo dataInfo) {
        return addEntry(source, isFile, path, strlen(path), dataInfo);
    }

    int32_t VFS::addEntry(int32_t source, bool isFile, const std::string& path, EntryInfo dataInfo) {
        return addEntry(source, isFile, path.c_str(), path.size(), dataInfo);
    }

    bool VFS::removeEntry(const char* path, size_t length, uint8_t fileAction) {
        if (path == nullptr || length < 1) { return false; }
        int32_t index = indexOfFileEntry(ConstSpan<char>(path, length));

        if (index < 0 || !_entries[index]) { return false; }
        removeEntry(_entries[index], fileAction);
        return true;
    }

    bool VFS::removeEntry(const char* path, uint8_t fileAction) {
        return removeEntry(path, strlen(path), fileAction);
    }

    bool VFS::removeEntry(const std::string& path, uint8_t fileAction) {
        return removeEntry(path.c_str(), path.size(), fileAction);
    }

    void VFS::openSourceFile(int32_t index, const char* mode) {
        if (index < 0 || index >= _files.tail()) { return; }
        if (auto file = _files[index]) {
            file->open(mode);
        }
    }

    void VFS::closeSourceFile(int32_t index) {
        if (index < 0 || index >= _files.tail()) { return; }
        if (auto file = _files[index]) {
            file->close();
        }
    }

    int32_t VFS::indexOfFileEntry(ConstSpan<char> path) const {
        char* tempBuf = reinterpret_cast<char*>(_malloca(path.length() + 1));
        if (!tempBuf) { return false; }
        tempBuf[path.length()] = 0;
        memcpy(tempBuf, path.get(), path.length());
        IO::fixPath(tempBuf);

        path = ConstSpan<char>(tempBuf, path.length());
        UUID8 pathHash = pathToHash(path);
        _freea(tempBuf);
        auto find = _fileEntryLUT.find(pathHash);
        return find != _fileEntryLUT.end() ? find->second : -1;
    }

    int32_t VFS::indexOfSourceFile(ConstSpan<char> path) const {
        return _files.indexOfType<ConstSpan<char>>(path, compareStream);
    }

    bool VFS::pathExists(ConstSpan<char> path) const {
        return indexOfFileEntry(path) > -1;
    }

    bool VFS::pathExists(const char* path, size_t length) const {
        return indexOfFileEntry(ConstSpan<char>(path, length)) > -1;
    }

    bool VFS::pathExists(const char* path) const {
        return indexOfFileEntry(ConstSpan<char>(path, strlen(path))) > -1;
    }

    bool VFS::pathExists(const std::string& path) const {
        return indexOfFileEntry(ConstSpan<char>(path.c_str(), path.size())) > -1;
    }

    UUID8 VFS::pathToHash(ConstSpan<char> path, bool autoAppendRoot) const {
        size_t slashCount = path.getOccurenceCount('/') + 1;
        size_t partCount = 0;
        ConstSpan<char>* pathParts = reinterpret_cast<ConstSpan<char>*>(_malloca(slashCount * sizeof(ConstSpan<char>)));

        UUID8 uuid = UUID8::Empty;
        HashState state{};

        if (pathParts && (partCount = path.split(pathParts, slashCount, '/'))) {
            if (_root || !autoAppendRoot) {
                if (autoAppendRoot && !_root->isSame(pathParts[0])) {
                    uuid.updateHash(state, _root->name.c_str(), _root->name.length(), FileEntry::PATH_HASH_BLOCK_SIZE);
                }

                for (size_t i = 0; i < partCount; i++) {
                    uuid.updateHash(state, pathParts[0].get(), pathParts[0].length(), FileEntry::PATH_HASH_BLOCK_SIZE);
                }
            }
        }
        return uuid;
    }

    void VFS::removeMissing() {
        if (!_root) { return; }
        size_t longest = _root->findLongestPath(false, 0);
        char* buffer = reinterpret_cast<char*>(_malloca(longest + 1 + _rootSpan.length()));
        if (!buffer) { return; }

        buffer[longest] = 0;
        memcpy(buffer, _rootSpan.get(), _rootSpan.length());
        char* fileStart = buffer + _rootSpan.length();

        FileEntry* fEnt{nullptr};
        for (size_t i = 0; i < _entries.tail(); i++) {
            if ((fEnt = _entries[i]) && !fEnt->isSubFile() && fEnt->parent) {
                size_t len = fEnt->buildPath(false, fileStart);
                if (len > 0) {
                    len += _rootSpan.length();
                    if (IO::exists(buffer, len)) { continue; }
                    removeEntry(fEnt, F_ACT_REMOVE);
                    JENGINE_CORE_TRACE("[J-Engine - VFS] Removed missing path '{0}'", fileStart);
                }
            }
        }
        _freea(buffer);
    }

    void VFS::removeEntry(FileEntry* entry, uint8_t fileAction) {
        if (entry) {
            for (auto ch : entry->children) {
                removeEntry(ch, fileAction);
            }

            _fileEntryLUT.erase(entry->hash);
            if (entry->isFile()) {
                switch (fileAction) {
                    case F_ACT_CLOSE:
                        closeSourceFile(entry->source);
                        break;
                    case F_ACT_REMOVE:
                        removeSourceFile(entry->source);
                        break;
                }
            }
        }
    }

    int32_t VFS::addSourceFile(const char* path, size_t length) {
        if (path == nullptr || length < 1) { return -1; }
        int32_t ind = indexOfSourceFile(ConstSpan<char>(path, length));
        if (ind > -1) { return ind; }
        auto file = _fileAllocator.allocate();
        if (!file) { return -1; }
        file->setFilepath(path, length);
        return _files.push(file);
    }

    int32_t VFS::addSourceFile(const char* path) {
        return addSourceFile(path, strlen(path));
    }

    int32_t VFS::addSourceFile(const std::string& path) {
        return addSourceFile(path.c_str(), path.size());
    }

    bool VFS::removeSourceFile(int32_t index) {
        if (index < 0) { return false; }

        if (FileStream* file = _files.pop(index)) {
            file->close();
            return true;
        }
        return false;
    }

    bool VFS::removeSourceFile(const char* path, size_t length) {
        if (path == nullptr || length < 1) { return false; }
        return removeSourceFile(indexOfSourceFile(ConstSpan<char>(path, length)));
    }

    bool VFS::removeSourceFile(const char* path) {
        return removeSourceFile(path, strlen(path));
    }

    bool VFS::removeSourceFile(const std::string& path) {
        return removeSourceFile(path.c_str(), path.size());
    }
}