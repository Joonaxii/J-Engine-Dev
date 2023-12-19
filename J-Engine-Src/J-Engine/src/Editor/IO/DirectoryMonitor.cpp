#include <Editor/IO/DirectoryMonitor.h>
#include <JEngine/Core/Log.h>
#include <JEngine/Utility/StringHelpers.h>

namespace JEngine {
    struct TempAction {
        DWORD action{};
        std::string path{};
    };

    DirectoryMonitor::DirectoryMonitor() : _handle{}, _overlapped{}, _flags{}, _changeBuffer{0} { }
    DirectoryMonitor::~DirectoryMonitor() {
        close();
    }

    void DirectoryMonitor::close() {
        if (_handle != INVALID_HANDLE_VALUE) {
            CloseHandle(_handle);
            _handle = INVALID_HANDLE_VALUE;
        }

        if (_overlapped.hEvent != INVALID_HANDLE_VALUE) {
            CloseHandle(_overlapped.hEvent);
            _overlapped.hEvent = INVALID_HANDLE_VALUE;
        }

        _flags &= ~FLAG_OPEN;
    }

    bool DirectoryMonitor::tryOpen(const std::string& path) {
        close();
        _handle = CreateFile(
            path.c_str(),
            FILE_LIST_DIRECTORY,
            FILE_SHARE_READ | FILE_SHARE_WRITE | FILE_SHARE_DELETE,
            NULL,
            OPEN_EXISTING,
            FILE_FLAG_BACKUP_SEMANTICS | FILE_FLAG_OVERLAPPED,
            NULL);

        if (_handle == INVALID_HANDLE_VALUE) {
            JENGINE_CORE_ERROR("[DirectoryMonitor] Error: Failed to create handle to directory {0}!", path);
            return false;
        }
        memset(_changeBuffer, 0, CHANGE_BUFFER_SIZE);
        _overlapped.hEvent = CreateEvent(NULL, FALSE, 0, NULL);

        _flags |= FLAG_OPEN;
        BOOL success = ReadDirectoryChangesW(
            _handle, _changeBuffer, CHANGE_BUFFER_SIZE, TRUE,
            FILE_NOTIFY_CHANGE_FILE_NAME |
            FILE_NOTIFY_CHANGE_DIR_NAME |
            FILE_NOTIFY_CHANGE_LAST_WRITE,
            NULL, &_overlapped, NULL);

        return true;
    }

    bool DirectoryMonitor::poll(std::vector<DirectoryChange>& changes) {
        if (!isOpen()) { return false; }

        changes.clear();
        static std::vector<TempAction> actions{};
        wchar_t nameBuf[MAX_PATH]{0};

        DWORD result = WaitForSingleObject(_overlapped.hEvent, 0);
        if (result == WAIT_OBJECT_0) {
            DWORD bytesTransferred{};
            GetOverlappedResult(_handle, &_overlapped, &bytesTransferred, FALSE);

            FILE_NOTIFY_INFORMATION* evnt = reinterpret_cast<FILE_NOTIFY_INFORMATION*>(_changeBuffer);
            actions.clear();
            while (true) {
                DWORD nameLen = evnt->FileNameLength / sizeof(wchar_t);
                memcpy(nameBuf, evnt->FileName, evnt->FileNameLength);
                nameBuf[nameLen] = 0;

                auto& act = actions.emplace_back();
                act.action = evnt->Action;
                Helpers::wideToUTF8(nameBuf, nameLen, act.path);

                if (evnt->NextEntryOffset == 0) {
                    break;
                }
                *((uint8_t**)&evnt) += evnt->NextEntryOffset;
            }

            BOOL success = ReadDirectoryChangesW(
                _handle, _changeBuffer, CHANGE_BUFFER_SIZE, TRUE,
                FILE_NOTIFY_CHANGE_FILE_NAME |
                FILE_NOTIFY_CHANGE_DIR_NAME |
                FILE_NOTIFY_CHANGE_LAST_WRITE,
                NULL, &_overlapped, NULL);

            for (size_t i = 0; i < actions.size(); i++) {
                auto& act = actions[i];
                switch (act.action)
                {
                    case FILE_ACTION_ADDED:
                        changes.emplace_back(DirectoryChange::TYPE_CREATE, act.path);
                        break;
                    case FILE_ACTION_REMOVED:
                        changes.emplace_back(DirectoryChange::TYPE_DELETE, act.path);
                        break;
                    case FILE_ACTION_MODIFIED:
                        changes.emplace_back(DirectoryChange::TYPE_MODIFIED, act.path);
                        break;
                    case FILE_ACTION_RENAMED_OLD_NAME:
                        i++;
                        if (i < actions.size()) {
                            changes.emplace_back(act.path, actions[i].path);
                        }
                        break;
                }
            }
        }

        return changes.size() > 0;
    }
}