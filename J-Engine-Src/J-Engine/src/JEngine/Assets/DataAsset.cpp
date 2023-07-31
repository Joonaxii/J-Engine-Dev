#include <JEngine/Assets/DataAsset.h>

namespace JEngine {
    void DataAsset::resize(size_t newSize) {
        if (_data) {
            if (_length == newSize) { return; }

            void* reloc = realloc(_data, newSize);
            if (reloc) {
                _data = reinterpret_cast<uint8_t*>(reloc);
                if (_length < newSize) {
                    memset(_data + _length, 0, newSize - _length);
                }
                _length = newSize;
            }
            return;
        }
        _data = reinterpret_cast<uint8_t*>(malloc(newSize));
        if (_data) {
            memset(_data, 0, newSize);
        }
        _length = newSize;
    }

    DataAsset::~DataAsset() {
        unload();
    }

    bool DataAsset::unload() {
        if (IAsset::unload()) {
            if (_data) {
                free(_data);
                _data = nullptr;
            }
            _length = 0;
            _type = T_Binary;
            return true;
        }
        return false;
    }

    void DataAsset::setData(const void* data, size_t size, DataType type) {
        resize(size);
        memcpy(_data, data, size);
        _type = type;
    }
}