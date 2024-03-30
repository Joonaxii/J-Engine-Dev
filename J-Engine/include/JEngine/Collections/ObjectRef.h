#pragma once
#include <JEngine/Collections/IReferenceProvider.h>
#include <JEngine/Utility/Flags.h>
#include <functional>
#include <cstdint>

namespace JEngine {
    static constexpr uint32_t MAX_UINT_24 = 0xFFFFFFU;
    static constexpr uint32_t UINT_20_MAX = 0x1FFFFFU;
    struct RefID {
        RefID() {};
        RefID(uint32_t i, uint32_t v) {};

        void setVersion(uint32_t v) const { return; }
        void setId(uint32_t v) const { return; }

        uint32_t getVersion() const { return 0; }
        uint32_t getId() const { return 0; }
    };

    namespace priv {
        struct IndexData {
            uint32_t data;

            IndexData(const uint32_t index, const UI8Flags flags) {
                data = index & MAX_UINT_24;
                data |= uint32_t(flags) << 24;
            }

            IndexData(const uint32_t index, const uint8_t flags) {
                data = index & MAX_UINT_24;
                data |= uint32_t(flags) << 24;
            }

            void setIndex(const uint32_t index) {
                data &= ~MAX_UINT_24;
                data |= index & MAX_UINT_24;
            }

            uint32_t getIndex() const {
                return data & MAX_UINT_24;
            }

            UI8Flags& getFlags() {
                return *(reinterpret_cast<UI8Flags*>(&data) + 3);
            }
        };

        struct RefPtr {
            mutable void* ptr;

            RefPtr(void* ptr) : ptr(ptr) {}

            operator bool() { return bool(ptr); }

            RefPtr& operator=(void* other) {
                ptr = other;
                return *this;
            }

            template<typename T>
            T* getAs() { return reinterpret_cast<T*>(ptr); }

            template<typename T>
            const T* getAs()const { return reinterpret_cast<const T*>(ptr); }
        };
    }

    template<typename T, bool isArray = false>
    class ObjectRef {
    public:
        static constexpr uint32_t NULL_ID = 0xFFFFFFF;

        ObjectRef() : _data(MAX_UINT_24, 0), _idData(NULL_ID, 0), _obj(nullptr) {}
        ObjectRef(const int32_t id) : ObjectRef(id, false) {}
        ObjectRef(const int32_t id, const bool ownedByThis) :
            _data(MAX_UINT_24, (ownedByThis ? OWNS_PTR_FLAG : 0)), _idData(NULL_ID, 0), _obj(nullptr) {
            setPtr(id);
        }

        ObjectRef(T* ptr) : ObjectRef(ptr, false) { }
        ObjectRef(T* ptr, const bool ownedByThis) :
            _data(MAX_UINT_24, (ownedByThis ? OWNS_PTR_FLAG : 0)), _idData(NULL_ID, 0), _obj(ptr) { }

        ObjectRef(const ObjectRef<T>& other) : 
            _data(MAX_UINT_24, uint8_t(other._data.getFlags()) & ~OWNS_PTR_FLAG), _idData(other._idData), _obj(other._obj) { }

        ~ObjectRef() {
            T* ptr = getPtr();
            if (ownsPointer() && isValid()) {
          

                if (isArray) {
                    delete[] ptr;
                }
                else {
                    delete ptr;
                }
            }
        }

        void setOwnsPointerFlag(const bool owns) {
            _data.getFlags().setBit(OWNS_PTR_FLAG, owns);
        }

        bool ownsPointer() const { 
            return bool(_data.getFlags() & OWNS_PTR_FLAG); 
        }

        ObjectRef<T>& operator=(const ObjectRef<T>& other) {
            _data = other._data;
            _data.getFlags().setBit(OWNS_PTR_FLAG, false);
            _obj = other._obj;
            _idData = other._idData;
            return *this;
        }

        bool operator==(const ObjectRef<T, isArray>& other) const {
            const bool isFromProvT = (_data.getFlags() & IS_FROM_PROVIDER);
            const bool isFromProvO = (other._data.getFlags() & IS_FROM_PROVIDER);
            if (isFromProvT && isFromProvO) {
                return _obj.ptr == other._obj.ptr && (_idData.getId() == other._idData.getId());
            }
            return isFromProvT == isFromProvO && _obj == other._obj.ptr;
        }

        bool operator!=(const ObjectRef<T, isArray>& other) const { return !(*this == other); }

        //bool operator==(const T* other) const {
        //    return getPtr() == other;
        //}

        //bool operator!=(const T* other) const { return !(*this == other); }

        operator T* () { return getPtr(); }
        operator const T* () const { return getPtr(); }

        bool isValid() const { 
            return  _obj && (_idData.getId() != NULL_ID || !(_data.getFlags() & IS_FROM_PROVIDER));
        }

        T* getPtr() {
            return nullptr;
        }

        const T* getPtr() const {
            return nullptr;
        }

        void setPtr(T* refe) {
            _idData.setId(NULL_ID);
            _idData.setVersion(0);
            _data.setIndex(MAX_UINT_24);
            _obj.ptr = refe;
            _data.getFlags().setBit(IS_FROM_PROVIDER, false);
        }

        void setPtr(const uint32_t id) {
            if (_idData.getId() != (id & UINT_20_MAX)) {
                _data.setIndex(MAX_UINT_24);
            }

            _idData.setId(id);
            if (!(_data.getFlags() & IS_FROM_PROVIDER)) {
                _obj = _idData.getId() == NULL_ID ? nullptr : _obj.ptr;
                return;
            }
        }

        const uint32_t getVersion() const { return _idData.getVersion(); }
        const uint32_t getID() const { return _idData.getId(); }

        // friend bool operator==(const T* ptr, const ObjectRef<T, isArray>& other);
        // friend bool operator!=(const T* ptr, const ObjectRef<T, isArray>& other);

    private:
        static constexpr uint8_t OWNS_PTR_FLAG = 0x1;
        static constexpr uint8_t IS_FROM_PROVIDER = 0x2;

        mutable priv::RefPtr _obj;
        mutable priv::IndexData _data;
        mutable RefID _idData;
    };

    //template<typename T, bool isArray>
    //inline bool operator==(const T* ptr, const ObjectRef<T, isArray>& other) {
    //    return other.getPtr() == ptr;
    //}

    //template<typename T, bool isArray>
    //inline bool operator!=(const T* ptr, const ObjectRef<T, isArray>& other) { return !(*ptr == other); }
}