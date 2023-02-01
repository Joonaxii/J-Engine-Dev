#pragma once
#include <JEngine/Collections/IReferenceProvider.h>
#include <JEngine/Utility/Flags.h>
#include <functional>
#include <stdint.h>

namespace JEngine {
    template<typename T, bool isArray = false>
    class ObjectRef {
    public:
        ObjectRef() :_flags(0), _id(-1), _version(0), _object(nullptr), _referenceProvider(nullptr) {}
        ObjectRef(const int32_t id) : ObjectRef(id, false) {}
        ObjectRef(const int32_t id, const bool ownedByThis) :
            _flags((ownedByThis ? OWNS_PTR_FLAG : 0)), _id(-1), _version(0), _object(nullptr), _referenceProvider(nullptr) {
            setPtr(id);
        }

        ObjectRef(T* ptr) : ObjectRef(ptr, false) { }
        ObjectRef(T* ptr, const bool ownedByThis) : 
            _flags((ownedByThis ? OWNS_PTR_FLAG : 0)), _id(-1), _version(0), _object(ptr), _referenceProvider(nullptr) { }

        ObjectRef(const int32_t id, IReferenceProvider<T>& referenceProvider) :ObjectRef(id, refrenceProvider, false) {}
        ObjectRef(const int32_t id, IReferenceProvider<T>& referenceProvider, const bool ownedByThis) : 
            _flags((ownedByThis ? OWNS_PTR_FLAG : 0)), _id(-1), _version(0), _object(nullptr), _referenceProvider(&referenceProvider) {
            setPtr(id);
        }

        ObjectRef(const ObjectRef<T>& other) : _flags(other._flags & ~OWNS_PTR_FLAG), _id(other._id), _version(other._version), _object(other._object), _referenceProvider(other._referenceProvider) { }


        ~ObjectRef() {
            getPtr();
            if (ownsPointer() && isValid()) {
                if (_referenceProvider) {
                    _referenceProvider->removeById(_id);
                }

                if (isArray) {
                    delete[] _object;
                }
                else {
                    delete _object;
                }
            }
        }

        void setOwnsPointerFlag(const bool owns) {
            _flags.setBit(OWNS_PTR_FLAG, owns);
        }
        const bool ownsPointer() const {
            return bool(_flags & OWNS_PTR_FLAG);
        }

        ObjectRef<T, isArray>& operator=(ObjectRef<T, isArray>& other) {
            _flags = other._flags & ~OWNS_PTR_FLAG;
            _id = other._id;
            _referenceProvider = other._referenceProvider;
            _version = other._version;
            _object = other._object;
            return *this;
        }

        const bool operator==(const ObjectRef<T, isArray>& other) const {
            if (_referenceProvider && other._referenceProvider) {
                return _referenceProvider == other._referenceProvider && (_id == other._id);
            }
            return _object == other._object;
        }

        const bool operator!=(const ObjectRef<T, isArray>& other) const {
            return !(*this == other);
        }

        operator T* () {
            return getPtr();
        }

        operator T* () const {
            return getPtr();
        }

        const bool isValid() const { return (!_referenceProvider || _id > -1) && _object; }

        T* getPtr() {
            if (_referenceProvider == nullptr) { return _object; }
            if (_referenceProvider->hasToUpdate(_version, _object)) {
                _object = _referenceProvider->getPtr(_id, _version);
            }
            return _object;
        }

        const T* getPtr() const {
            if (_referenceProvider == nullptr) { return _object; }
            if (_referenceProvider->hasToUpdate(_version, _object)) {
                _object = _referenceProvider->getPtr(_id, _version);
            }
            return _object;
        }

        void setPtr(T* refe) {
            _id = -1;
            _version = 0;
            _object = refe;
            _referenceProvider = nullptr;
        }

        void setPtr(const int32_t id) {
            _id = id;
            if (_referenceProvider == nullptr) {
                _object = _id < 0 ? nullptr : _object;
                return;
            }
            if (id < 0) {
                _object = nullptr;
                return;
            }
            _object = _referenceProvider->getPtr(_id, _version);
        }

        void setPtr(const int32_t id, IReferenceProvider<T>& refProvider) {
            _referenceProvider = &refProvider;
            setPtr(id);
        }

        const uint32_t getVersion() const {
            return _version;
        }

        const int32_t getID() const {
            return _id;
        }

    private:
        static constexpr uint8_t OWNS_PTR_FLAG = 0x1;

        mutable T* _object;

        int32_t _id;
        uint8_t _version;
        UI8Flags _flags;

        IReferenceProvider<T>* _referenceProvider;

    };
}