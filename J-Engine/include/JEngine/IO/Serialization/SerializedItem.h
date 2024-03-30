#pragma once
#include <string>
#include <vector>
#include <JEngine/Core.h>
#include <JEngine/Core/Ref.h>
#include <JEngine/Core/String.h>
#include <JEngine/Collections/PoolAllocator.h>
#include <JEngine/Utility/StringHelpers.h>
#include <JEngine/IO/Serialization/Serialize.h>
#include <JEngine/IO/Serialization/Serializable.h>

namespace JEngine {
    struct SerializedItem {
    public:
        enum class IType : uint8_t {
            ITYPE_VALUE = 0x00,
            ITYPE_ARRAY = 0x01,
            ITYPE_FIXED_BUFFER = 0x02,
            ITYPE_OBJECT = 0x03,
        };

        enum TargetType : uint8_t {
            TGT_UNKNOWN = 0x00,
            TGT_GAME_OBJECT = 0x01,
            TGT_COMPONENT = 0x02,
            TGT_ASSET = 0x03,
        };

        JE_BEG_PACK
        struct Target {
            TargetType type;
            union {
                uint64_t raw;
                GORef gRef;
                CompRef cRef;
                AssetRef aRef;
            } ref;

            GORef asGO() const { return ref.gRef; }
            CompRef asComp() const { return ref.cRef; }
            AssetRef asAsset() const { return ref.aRef; }
        };
        JE_END_PACK

        static constexpr size_t DEFAULT_SITEM_POOL_SIZE = 1;
        using SItemPool = PoolAllocator<SerializedItem, DEFAULT_SITEM_POOL_SIZE>;

        ~SerializedItem();

        static SItemPool& getSItemPool() {
            static SItemPool pool{};
            return pool;
        }

        const SerializedItem* operator[](std::string_view key) const;
        SerializedItem* operator[](std::string_view key);

        template<typename T>
        const T& value(const T& defaultVal = T{}) {
            auto ptr = _data.getAs<T>();
            return ptr ? *ptr : defaultVal;
        }

        template<typename T>
        const T& value(std::string_view path, const T& defaultVal = T{}) const {
            const SerializedItem* item = findByPath(path);
            return item ? item->value<T>(defaultVal) : defaultVal;
        }

        template<typename T>
        void value(T& valueOut, const T& defaultVal = T{}) {
            auto ptr = _data.getAs<T>();
            if (ptr) {
                _data.copyTo(valueOut);
                return;
            }
            valueOut = defaultValue;
        }

        template<typename T>
        void value(std::string_view path, T& valueOut, const T& defaultVal = T{}) const {
            const SerializedItem* item = findByPath(path);
            if (item) {
                item->value<T>(valueOut, defaultVal);
                return;
            }
            valueOut = defaultVal;
        }

        template<typename T, typename C>
        void value(C& collection, const T& defaultVal = T{}) {
            if (this->_itemType == IType::ITYPE_ARRAY) {
                size_t len = _data.size
                for (size_t i = 0; i < length; i++)
                {

                }
            }

            auto ptr = _data.getAs<T>();
            if (ptr) {
                _data.copyTo(valueOut);
                return;
            }
            valueOut = defaultValue;
        }

        template<typename T, typename C>
        void value(std::string_view path, C& collection, const T& defaultVal = T{}) const {
            const SerializedItem* item = findByPath(path);
            if (item) {
                item->value<T>(valueOut, defaultVal);
                return;
            }
            valueOut = defaultVal;
        }

        const GUIStyle& getStyle() const {
            const GUIStyle* style = _fieldInfo ? _fieldInfo->style : nullptr;
            return style ? *style : Gui::Styles::Default;
        }

        void copyTo(SerializedItem& other);

        void clear() {
            for (auto& ch : _children) {
                if (ch) {
                    ch->clear();
                    getSItemPool().deallocate(ch);
                }
            }
            _children.clear();
        }

        template<typename T>
        void setData(const T& data) {
            _data.allocate(sizeof(T));
            _data.copyFrom(&data, sizeof(T));
        }

        template<typename T, size_t size>
        void setData(const T data[size]) {
            _data.allocate(sizeof(data));
            _data.copyFrom(&data, sizeof(data));
        }

        template<typename T>
        inline void setData(const std::vector<T>& data) {
            _data.allocate(sizeof(T) * data.size());
            _data.copyFrom(data.data(), sizeof(T) * data.size()));
        }

        template<typename T>
        void copyData(T& target) const {
            _data.copyTo(&target, sizeof(T));
        }

        template<typename T>
        void copyData(T* target, size_t length) const {
            _data.copyTo(target, std::min(sizeof(T) * length, _data.size));
        }

        size_t getElementSize() const {
            return _fieldInfo ? _fieldInfo->size : 0;
        }

        size_t getArraySize() const {
            if (!_fieldInfo || (_itemType != IType::ITYPE_ARRAY && _itemType != IType::ITYPE_FIXED_BUFFER)) { return 0; }
            return _data.size / Math::max(_fieldInfo->size, 1ULL);
        }

        std::string_view getName() const { return _name; }
        const char* getCName() const { return _name.data(); }

        void* getData() {
            return _data.get();
        }

        const void* getData() const {
            return _data.get();
        }

        void* getDataEnd() {
            return _data.getEnd();
        }

        const void* getDataEnd() const {
            return _data.getEnd();
        }

        template<typename T>
        T* getDataAs() {
            return _data.getAs<T>();
        }

        template<typename T>
        const T* getDataAs() const {
            return _data.getAs<T>();
        }

        template<typename T>
        T* getDataEndAs() {
            return _data.getEndAs<T>();
        }

        template<typename T>
        const T* getDataEndAs() const {
            return _data.getEndAs<T>();
        }

        SerializedItem* findByKey(std::string_view key) const;
        SerializedItem* findByPath(std::string_view path);
        const SerializedItem* findByPath(std::string_view path) const;

        void serialize(const Stream& stream) const;
        void deserialize(const Stream& stream);

        void serialize(json& jsonF) const;
        void deserialize(const json& jsonF);

        void serialize(yamlEmit& emit) const;
        void deserialize(const yamlNode& node);

        Target getTarget() const { return _target; }

        static SerializedItem* fromFieldInfo(const Type* type, const FieldInfo* field);
        static SerializedItem* fromType(const Type* type);
        static SerializedItem* fromGameObject(GameObject* go);
        static SerializedItem* fromComponent(uint32_t typeHash);
        static SerializedItem* fromComponent(std::string_view typeName) {
            return fromComponent(Types::calculateNameHash(typeName));
        }

        template<typename T>
        static SerializedItem* fromComponent() {

        }

    private:
        friend class SItemPool;
        friend struct PoolChunk<SerializedItem>;

        String _name{};
        Target _target;

        SerializedItem* _root;

        const Type* _type;
        const FieldInfo* _fieldInfo;

        struct Data {
            bool useExternal{false};

            struct Array {
                uint8_t* ptrSt{};
                uint8_t* ptrEn{};

                void clear() {
                    if (ptrSt) {
                        JE_FREE_ALIGNED(16, ptrSt);
                    }
                    ptrSt = nullptr;
                    ptrEn = nullptr;
                }
                size_t getSize() const { return ptrEn - ptrSt; }

                void allocate(size_t size) {
                    if (size <= getSize()) { return; }
                    if (ptrSt) {
                        void* real = JE_REALLOC_ALIGNED(16, ptrSt, size);
                        if (real) {
                            ptrSt = reinterpret_cast<uint8_t*>(real);
                        }
                    }
                    else {
                        ptrSt = reinterpret_cast<uint8_t*>(JE_ALLOC_ALIGNED(16, size));
                    }
                    ptrEn = ptrSt + size;

                    if (ptrSt) {
                        JE_ZERO_MEM(ptrSt, size);
                    }
                }
            };
            SIMD_ALIGN union {
                uint8_t buffer[64];
                Array arr;
            } data;
            size_t size{};

            ~Data() {
                clear();
            }
            void clear() {
                this->size = 0;
                if (useExternal) {
                    data.arr.clear();
                }
                JE_ZERO_MEM(&data, sizeof(data));
                useExternal = false;
            }

            void copyFrom(const void* ptr, size_t size) {
                if (useExternal) {
                    JE_COPY(data.arr.ptrSt, ptr, size);
                    return;
                }
                JE_COPY(&data, ptr, size);
            }

            void copyTo(void* ptr, size_t size) const {
                if (useExternal) {
                    JE_COPY(ptr, data.arr.ptrSt, size);
                    return;
                }
                JE_COPY(ptr, &data, size);
            }

            void copyTo(Data& other) const {
                other.allocate(size);
                if (useExternal) {
                    JE_COPY(other.data.arr.ptrSt, data.arr.ptrSt, size);
                    return;
                }
                JE_COPY(&other.data, &data, size);
            }

            void allocate(size_t size) {
                uint8_t temp[64]{0};

                size_t prevSize = this->size;
                this->size = size;
                bool wasExt = useExternal;
                useExternal = size > 64;

                if (wasExt != useExternal) {
                    if (wasExt) {
                        JE_COPY(temp, data.arr.ptrSt, std::min(prevSize, 64ULL));
                    }
                    else {
                        JE_COPY(temp, data.buffer, 64);
                    }
                }

                if (useExternal) {
                    data.arr.allocate(size);
                    if (!wasExt && data.arr.ptrSt) {
                        JE_COPY(data.arr.ptrSt, temp, Math::min(prevSize, size));
                    }
                    return;
                }

                if (wasExt) {
                    JE_COPY(&data, temp, Math::min(prevSize, size));
                    if (prevSize < size) {
                        JE_ZERO_MEM(data.buffer + prevSize, size - prevSize);
                    }
                    data.arr.clear();
                }
            }

            void* get() {
                if (useExternal) {
                    return data.arr.ptrSt;
                }
                return &data;
            }

            const void* get() const {
                if (useExternal) {
                    return data.arr.ptrSt;
                }
                return &data;
            }

            void* getEnd() {
                if (useExternal) {
                    return data.arr.ptrEn;
                }
                return reinterpret_cast<uint8_t*>(&data) + size;
            }

            const void* getEnd() const {
                if (useExternal) {
                    return data.arr.ptrEn;
                }
                return reinterpret_cast<const uint8_t*>(&data) + size;
            }

            template<typename T>
            T* getAs() {
                if (useExternal) {
                    return reinterpret_cast<T*>(data.arr.ptrSt);
                }
                return sizeof(T) > sizeof(data) ? nullptr : reinterpret_cast<T*>(&data);
            }  
            
            template<typename T>
            const T* getAs() const {
                if (useExternal) {
                    return reinterpret_cast<const T*>(data.arr.ptrSt);
                }
                return sizeof(T) > sizeof(data) ? nullptr : reinterpret_cast<const T*>(&data);
            }

            template<typename T>
            T* getEndAs() {
                if (useExternal) {
                    return reinterpret_cast<T*>(data.arr.ptrEn);
                }
                return sizeof(T) > sizeof(data) ? nullptr : reinterpret_cast<T*>(&data) + (size / sizeof(T));
            }

            template<typename T>
            const T* getEndAs() const {
                if (useExternal) {
                    return reinterpret_cast<T*>(data.arr.ptrEn);
                }
                return sizeof(T) > sizeof(data) ? nullptr : reinterpret_cast<T*>(&data) + (size / sizeof(T));
            }
        } _data;
        std::vector<SerializedItem*> _children{};

        SerializedItem();

        void* getDataIn(size_t& size);
        void clearData();

    };
}