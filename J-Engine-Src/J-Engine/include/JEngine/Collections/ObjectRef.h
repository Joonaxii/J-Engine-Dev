#pragma once
#include <JEngine/Collections/IReferenceProvider.h>
#include <functional>
#include <stdint.h>

namespace JEngine {
    template<typename T, bool isOwning = false, bool isArray = false>
    class ObjectRef {
    public:
        ObjectRef() : _id(-1), _version(0), _object(nullptr), _referenceProvider(nullptr) {}
        ObjectRef(const int32_t id) : _id(-1), _version(0), _object(nullptr), _referenceProvider(nullptr) {
            setPtr(id);
        }

        ObjectRef(T* ptr) : _id(-1), _version(0), _object(ptr), _referenceProvider(nullptr) { }

        ObjectRef(const int32_t id, IReferenceProvider<T>& referenceProvider) : _id(-1), _version(0), _object(nullptr), _referenceProvider(&referenceProvider) {
            setPtr(id);
        }

        ~ObjectRef() {
            getPtr();
            if (isOwning && isValid()) {
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
        
        template<bool isOwning>
        inline ObjectRef<T, false, isArray>& operator=(ObjectRef<T, isOwning, isArray>& other) {
            _id = other._id;
            _referenceProvider = other._referenceProvider;
            _version = other._version;
            _object = other._object;
            return *this;
        }

        operator T* () {
            return getPtr();
        }

        operator T* () const {
            return getPtr();
        }

        const bool isValid() const { return (!_referenceProvider ||  _id > -1) && _object; }

        T* getPtr() {
            if (_referenceProvider == nullptr) { return _object; }
            if (_referenceProvider->versionChanged(_version)) {
                _object = _referenceProvider->getPtr(_id, _version);
            }
            return _object;
        }

        const T* getPtr() const {
            if (_referenceProvider == nullptr) { return _object; }
            if (_referenceProvider->versionChanged(_version)) {
                return _referenceProvider->getPtr(_id, _version);
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
        T* _object;
        int32_t _id;
        uint32_t _version;

        IReferenceProvider<T>* _referenceProvider;
    };
}