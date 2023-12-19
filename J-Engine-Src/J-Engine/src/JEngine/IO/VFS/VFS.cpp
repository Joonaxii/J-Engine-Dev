#include <JEngine/IO/VFS/VFS.h>
#include <JEngine/Utility/StringHelpers.h>

namespace JEngine {
    VFS::VFS() : _rootPath(), _root(), _entries{}, _sources{}  {}

    VFS::VFS(ConstSpan<char> root, uint32_t flags) : _flags(flags), _rootPath(root.get(), root.get() + root.length()), _root(), _entries{}, _sources{} {
        FileEntry* rootEnt{ nullptr };
        
        _root = FileID(_entries.getNext(&rootEnt));

        IO::fixPath(_rootPath);
        ConstSpan<char> temp(_rootPath.c_str(), _rootPath.length());
        _rootSpan = temp.trim('/');
        rootEnt->setup(_root, FileID(), _rootSpan.slice(_rootSpan.indexOfLast('/') + 1), SIZE_MAX, FOLDER_EINF);
    }

    VFS::~VFS() { }

    void VFS::changeRoot(ConstSpan<char> root, uint32_t flags) {
        if(Helpers::strIEquals(root, _rootPath)) {
            JENGINE_CORE_WARN("[VFS] Warning: Given root path is already setup! Nothing changed.");
            return;
        }

        _flags = flags;

        char* tempBuf = reinterpret_cast<char*>(_malloca(root.length() + 1));
        if (!tempBuf) { return; }
        tempBuf[root.length()] = 0;
        memcpy(tempBuf, root.get(), root.length());
        IO::fixPath(tempBuf);
        ConstSpan<char> newRoot(tempBuf, root.length());
        ConstSpan<char> oldRoot(_rootPath.c_str(), _rootPath.length());

        newRoot = newRoot.trim('/');
        oldRoot = oldRoot.trim('/');

        std::string temp{};

        //By default we assume that the root has changed and all files are present under the new root
        for (uint32_t i = 0; i < _sources.size(); i++) {
            FileStream* file = _sources.getAt(i);
            if (!file) { continue; }
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

        FileEntry* rootE = getEntryByRef(_root);

        int32_t lastSlash = newRoot.indexOfLast('/');
        ConstSpan<char> newRootName = newRoot.slice(lastSlash + 1);
        rootE->name = std::string(newRootName.get(), newRootName.length());

        _rootPath = std::string(newRoot.get(), newRoot.length());
        _rootSpan = ConstSpan<char>(_rootPath.c_str(), _rootPath.length());

        _freea(tempBuf);
    }

    bool VFS::buildFromRoot() {
        if (_rootSpan.length() < 1 || !IO::exists(_rootSpan)) {
            JENGINE_CORE_ERROR("[VFS] Error: Failed to build VFS from root path '{0}' (Either the given directory doesn't exist or path string is empty)", _rootPath.c_str());
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
                addEntry(-1, temp, isFile ? FILE_EINF : FOLDER_EINF);
            }
        }
        return true;
    }

    FileID VFS::addEntry(uint32_t source, ConstSpan<char> path, EntryInfo dataInfo, bool* addedNew) {
        if (addedNew) {
            *addedNew = false;
        }
        
        size_t len = path.length();
        char* tempBuf = reinterpret_cast<char*>(_malloca(len + 1));
        if (!tempBuf) { return false; }
        tempBuf[len] = 0;
        memcpy(tempBuf, path.get(), len);
        Span<char> pathSpn(tempBuf, len);
        IO::fixPath(pathSpn);

        if (source < 0 && dataInfo == FILE_EINF) {
            source = indexOfSourceFile(pathSpn);
            if (source < 0) {
                FileStream* strm{ nullptr };
                source = _sources.getNext(&strm);
                if (strm) {
                    strm->setFilepath(pathSpn.get(), pathSpn.length());
                }
            }
        }

        if (source < 0 && dataInfo == FILE_EINF) {
            JENGINE_CORE_WARN("[VFS] Warning: Given path '{0}' couldn't be added! (File source is invalid, if part of a JPAK add the file beforehand)", tempBuf);
            goto failure;
        }

        size_t rootP = Helpers::indexOf(pathSpn, _rootSpan);
        if (rootP != SIZE_MAX) {
            pathSpn = pathSpn.slice(rootP + _rootSpan.length());
        }

        FileEntry* root = getRoot();

        FileEntry* current = root;
        size_t slashCount = pathSpn.getOccurenceCount('/') + 1;
        size_t partCount = 0;
        Span<char>* pathParts = reinterpret_cast<Span<char>*>(_malloca(slashCount * sizeof(Span<char>)));

        FileID lastInd{};
        if (pathParts && (partCount = pathSpn.split(pathParts, slashCount, '/')) && current) {
            if (!current->isSame(pathParts[0])) {
                current = getEntryByRef(current->findInChild(*this, pathParts[0]));
            }
            bool isLast = partCount == 1;

            if (current == nullptr) {
                lastInd = _entries.getNext(&current);
                root->children.emplace_back(lastInd);
                current->setup(lastInd, _root, pathParts[0], source, isLast ? dataInfo : FOLDER_EINF);

                if (isLast && addedNew) {
                    *addedNew = true;
                }
            }

            if (current) {
                for (size_t i = 1; i < partCount; i++) {
                    auto& part = pathParts[i];
                    isLast = i >= partCount - 1;

                    auto find = current->findInChild(*this, part);
                    if (find.isValid()) {
                        FileEntry* findPtr{ nullptr };
                        lastInd = _entries.getNext(&findPtr);
       
                        current->children.emplace_back(lastInd);
                        findPtr->setup(lastInd, current->id, part, source, isLast ? dataInfo : FOLDER_EINF);

                        if (isLast && addedNew) {
                            *addedNew = true;
                        }
                    }
                    else if (isLast) {
                        lastInd = find;
                    }
                    current = getEntryByRef(find);
                }
            }
            else {
                JENGINE_CORE_WARN("[VFS] Warning: Given path '{0}' isn't a valid path!", tempBuf);
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

    void VFS::openSourceFile(uint32_t index, const char* mode) {
        if (index < 0 || index >= _sources.size()) { return; }
        _sources[index].open(mode);
    }

    void VFS::closeSourceFile(uint32_t index) {
        if (auto strm = _sources.getAt(index)) {
            strm->close();
        }
    }

    bool VFS::removeSourceFile(uint32_t index) {
        return _sources.markFree(index);
    }

    bool VFS::removeSourceFile(ConstSpan<char> path) {
        return removeSourceFile(indexOfSourceFile(path));
    }

    FileID VFS::indexOfFileEntry(ConstSpan<char> path) const {
        char* temp = reinterpret_cast<char*>(_malloca(path.length()));
        if (!temp) { return FileID(); }

        memcpy(temp, path.get(), path.length());
        IO::fixPath({ temp, path.length() });
        path = ConstSpan<char>(temp, path.length());

        size_t slashCount = path.getOccurenceCount('/') + 1;
        size_t partCount = 0;
        ConstSpan<char>* pathParts = reinterpret_cast<ConstSpan<char>*>(_malloca(slashCount * sizeof(ConstSpan<char>)));
        const FileEntry* current = getEntryByRef(_root);

        FileID id{};
        if (current && pathParts && (partCount = path.split(pathParts, slashCount, '/'))) {
            size_t start = 0;
            if (current->isSame(pathParts[0])) {
                start = 1;
            }
            for (size_t i = start; i < partCount; i++) {
                id = current->findInChild(*this, pathParts[i]);
                current = getEntryByRef(id);
                if (current == nullptr) { 
                    _freea(temp);
                    return FileID();
                }
            }
        }
        _freea(temp);
        return id;
    }

    uint32_t VFS::indexOfSourceFile(ConstSpan<char> path) const {
        if (path.length() < 1) { return SIZE_MAX; }
        uint32_t len = uint32_t(_sources.size());
        for (uint32_t i = 0; i < len; i++) {
            if (IO::pathsAreEqual(path, _sources[i].getFilePath())) { return i; }
        }
        return SIZE_MAX;
    }

    bool VFS::pathExists(ConstSpan<char> path) const {
        return indexOfFileEntry(path).isValid();
    }

    void VFS::removeMissing() {
        auto root = getRoot();
        if (!root) { return; }

        size_t longest = root->findLongestPath(*this, false, 0);
        char* buffer = reinterpret_cast<char*>(_malloca(longest + 1 + _rootSpan.length()));
        if (!buffer) { return; }

        buffer[longest] = 0;
        memcpy(buffer, _rootSpan.get(), _rootSpan.length());
        char* fileStart = buffer + _rootSpan.length();

        FileEntry* fEnt{nullptr};
        for (uint32_t i = 0; i < _entries.size(); i++) {
            if ((fEnt = _entries.getAt(i)) && !fEnt->isSubFile(*this) && !fEnt->parent.isValid()) {
                size_t len = fEnt->buildPath(*this, false, fileStart);
                if (len > 0) {
                    len += _rootSpan.length();
                    if (IO::exists(ConstSpan<char>( buffer, len))) { continue; }
                    removeEntry(fEnt->id);
                    JENGINE_CORE_TRACE("[VFS] Removed missing path '{0}'", fileStart);
                }
            }
        }
        _freea(buffer);
    }

    FileEntry* VFS::getEntryByRef(FileID ref) {
        return _entries.getAt(ref.getID());
    }

    const FileEntry* VFS::getEntryByRef(FileID ref) const {
        return _entries.getAt(ref.getID());
    }

    bool VFS::removeEntry(ConstSpan<char> path, uint8_t sourceAction)  {
        return removeEntry(indexOfFileEntry(path), sourceAction);
    }

    bool VFS::removeEntry(FileID entry, uint8_t sourceAction) {
        if (auto ePtr = this->getEntryByRef(entry)) {
            switch (sourceAction) {
                case F_ACT_CLOSE:
                    closeSourceFile(ePtr->source);
                    break;
                case F_ACT_REMOVE:
                    removeSourceFile(ePtr->source);
                    break;
            }
        }
        return _entries.markFree(entry.getID());
    }

    uint32_t VFS::addSourceFile(ConstSpan<char> path) {
        if (path.length() < 1) { return UINT32_MAX; }
        uint32_t ind = indexOfSourceFile(path);
        if (ind != UINT32_MAX) { return ind; }

        FileStream* strm{ nullptr };
        ind = _sources.getNext(&strm);
        if (strm) {
            strm->setFilepath(path.get(), path.length());
        }
        return ind;
    }

    FileID FileEntry::findInChild(const VFS& vfs, ConstSpan<char> span) const {
        for (FileID fld : children) {
            const FileEntry* ent = vfs.getEntryByRef(fld);
            if (ent && ent->isSame(span)) {
                return id.getID();
            }
        }
        return FileID();
    }

    size_t FileEntry::getPathLength(const VFS& vfs, bool includeRoot) const {
        size_t len = 0;
        const FileEntry* entry = this;
        while (entry) {
            if (!includeRoot && !entry->parent.isValid()) { break; }
            len += entry->name.length() + 1;
            entry = vfs.getEntryByRef(entry->parent);
        }
        return len;
    }

    bool FileEntry::isSubFile(const VFS& vfs) const {
        const FileEntry* entry = vfs.getEntryByRef(parent);
        while (entry) {
            if (entry->isFile()) { return true; }
            entry = vfs.getEntryByRef(entry->parent);
        }
        return false;
    }

    void FileEntry::remove(VFS& vfs) {
        if (FileEntry* parRef = vfs.getEntryByRef(parent)) {
            parRef->children.erase(std::find(parRef->children.begin(), parRef->children.end(), id));
        }

        for (FileID id : children) {
            if (FileEntry* item = vfs.getEntryByRef(id)) {
                item->remove(vfs);
            }
        }
        vfs.removeEntry(id);
    }
    size_t FileEntry::getDepth(const VFS& vfs, bool includeRoot) const {
        size_t depth = 0;
        const FileEntry* entry = this;
        while (entry) {
            if (!includeRoot && !entry->parent.isValid() && depth > 0) { break; }
            depth++;
            entry = vfs.getEntryByRef(entry->parent);
        }
        return depth < 1 ? 0 : depth - 1;
    }

    size_t FileEntry::buildPath(const VFS& vfs, bool includeRoot, char* buffer) const {
        size_t depth = getDepth(vfs, includeRoot);
        ConstSpan<char>* eBuf = reinterpret_cast<ConstSpan<char>*>(_malloca((depth + 1) * sizeof(ConstSpan<char>)));
        if (!eBuf) { return 0; }

        size_t pos = depth;
        const FileEntry* entry = this;
        while (entry) {
            if (!includeRoot && !entry->parent.isValid()) { break; }
            eBuf[pos--] = entry->name;
            entry = vfs.getEntryByRef(entry->parent);
        }

        pos = 0;
        for (size_t i = 0; i <= depth; i++) {
            *buffer++ = '/';
            auto& name = eBuf[i];
            memcpy(buffer, name.get(), name.length());
            buffer += name.length();
            pos += name.length() + 1;
        }
        *buffer = 0;
        _freea(eBuf);
        return pos;
    }

    size_t FileEntry::findLongestPath(const VFS& vfs, bool includeRoot, size_t initSize) const {
        initSize = std::max(initSize, getPathLength(vfs, includeRoot));
        for (FileID ch : children) {
            if (auto entPtr = vfs.getEntryByRef(ch)) {
                initSize = entPtr->findLongestPath(vfs, includeRoot, initSize);
            }
        }
        return initSize;
    }

    bool FileEntry::deleteIO(VFS& vfs) {
        if (dataInfo == RUNTIME_EINF) { return false; }

        bool removed = false;
        FileStream* stream{ nullptr };
        if (dataInfo.isFile() && (stream = vfs.getSourceFile(source))) {
            vfs.removeSourceFile(stream->getFilePath());
            removed = IO::delFile(stream->getFilePath());
        }
        else {
            char path[260]{ 0 };
            ConstSpan<char> rootP = vfs.getRootPath();
            memcpy(path, rootP.get(), rootP.length());

            size_t len = buildPath(vfs, false, path + rootP.length());
            ConstSpan<char> pathSP(path, len + rootP.length());
            vfs.removeSourceFile(pathSP);
            removed =  IO::delDirRecursively(pathSP);
        }
        remove(vfs);
        return removed;
    }

    void FileEntry::getOrderedChildren(std::vector<FileID>& output) const {
        FileID* files = reinterpret_cast<FileID*>(_malloca(children.size() * sizeof(FileID)));
        if(files) {
            size_t fileCount = 0;
            for (FileID ent : children) {
                if (ent.isFolder()) {
                    output.push_back(ent);
                }
                else {
                    files[fileCount++] = ent;
                }
            }
            if (fileCount > 0) {
                output.insert(output.end(), files, files + fileCount);
            }
            _freea(files);
        }
    }
}