#pragma once
#include <string>
#include <vector>
#include <JEngine/Core/Ref.h>
#include <JEngine/Collections/PoolAllocator.h>
#include <JEngine/Utility/StringHelpers.h>
#include <JEngine/IO/Serialization/Serializable.h>
#include <JEngine/IO/Serialization/SerializationUtils.h>

namespace JEngine {
    struct SerializedNode {
    public:
        enum TargetType : uint8_t {
            TGT_UNKNOWN = 0x00,
            TGT_GAME_OBJECT = 0x01,
            TGT_COMPONENT = 0x02,
            TGT_ASSET = 0x03,
        };

#pragma pack(push, 1)
        struct Target {
            TargetType type;
            union {
                uint32_t raw;
                GORef gRef;
                CompRef cRef;
                AssetRef aRef;
            } ref;

            GORef asGO() const { return ref.gRef; }
            CompRef asComp() const { return ref.cRef; }
            AssetRef asAsset() const { return ref.aRef; }
        };
#pragma pack(pop)

        static constexpr size_t DEFAULT_SNODE_POOL_SIZE = 1;
        using SNodePool = PoolAllocator<SerializedNode, DEFAULT_SNODE_POOL_SIZE>;

        ~SerializedNode();

        template<typename T>
        static void convert(SerializedNode& node, const T& in, const ValueType* overrideType = nullptr, const SubValueType* overrideSubType = nullptr, const GUIStyle* overrideStyle = nullptr) {
            auto desc = Serialization::getDescriptor<T>(
                overrideType ? *overrideType : CustomSerializables::getValueType<T>(),
                overrideSubType ? *overrideSubType : CustomSerializables::getSubValueType<T>(), NodeType::NTYPE_VALUE);
            node.setup(desc, in, overrideStyle ? *overrideStyle : CustomSerializables::getDefaultStyle<T>());
        }

        template<typename T, size_t size>
        static void convert(SerializedNode& node, const T in[size], const ValueType* overrideType = nullptr, const SubValueType* overrideSubType = nullptr, const GUIStyle* overrideStyle = nullptr) {
            auto desc = Serialization::getDescriptor<T>(
                overrideType ? *overrideType : CustomSerializables::getValueType<T>(),
                overrideSubType ? *overrideSubType : CustomSerializables::getSubValueType<T>(), NodeType::NTYPE_FIXED_BUFFER);
            node.setup(desc, in, overrideStyle ? *overrideStyle : CustomSerializables::getDefaultStyle<T>());
        }

        template<typename T>
        static void parse(const SerializedNode& node, T& out) {
            node.copyData(out);
        }

        template<typename T, size_t size>
        static void parse(const SerializedNode& node, T out[size]) {
            node.copyData(out, size);
        }

        template<size_t size>
        inline static void parse(const SerializedNode& node, std::string out[size]) {
            const char* ptr = node.getDataAs<char>();
            auto end = node.getDataEnd();
            for (size_t i = 0; ptr < end && i < size; i++) {
                size_t len = strlen(ptr);
                out[i].clear();
                out[i].append(ptr, len);
                ptr += len + 1;
            }
        }

        template<typename T>
        static void parse(const SerializedNode& node, std::vector<T>& out) {
            size_t count = node.getArraySize();
            out.clear();

            const T* data = node.getDataAs<T>();
            for (size_t i = 0; i < count; i++) {
                out.emplace_back(data[i]);
            }
        }

        template<>
        inline static void parse(const SerializedNode& node, std::vector<std::string>& out) {
            size_t count = node.getArraySize();
            out.clear();

            const std::string* data = node.getDataAs<std::string>();
            for (size_t i = 0; i < count; i++) {
                out.emplace_back(data[i]);
            }
        }

        template<typename T>
        static bool drawSelf(SerializedNode& node) {
            return false;
        }

        static SNodePool& getSNodePool() {
            static SNodePool pool{};
            return pool;
        }

        const SerializedNode* operator[](std::string_view key) const;
        SerializedNode* operator[](std::string_view key);

        SerializedNode* findByKey(std::string_view key) const;

        void copyTo(SerializedNode& other);

        void clear() {
            for (auto& ch : _children) {
                if (ch) {
                    ch->clear();
                    getSNodePool().deallocate(ch);
                }
            }
            _children.clear();
        }

        void setup(const detail::TSDescriptor& desc, const GUIStyle* style = &Gui::Styles::Default) {
            clear();
            _descriptor = desc;
            _style = style;
        }

        void setup(std::string_view name, const detail::TSDescriptor& desc, const GUIStyle* style = &Gui::Styles::Default) {
            _name = name;
            setup(desc, style);
        }

        template<typename T>
        void setData(const T& data) {
            _strings.resize(1);
            _data.allocate(sizeof(T));
            _data.copyFrom(&data, sizeof(T));
        }

        template<>
        inline void setData(const std::string_view& data) {
            _data.clear();
            _strings.resize(1);
            _strings[0] = data;
        }

        template<typename T, size_t size>
        void setData(const T data[size]) {
            _strings.resize(1);
            _data.allocate(sizeof(data));
            _data.copyFrom(&data, sizeof(data));
        }

        template<size_t size>
        inline void setData(const std::string data[size]) {
            _data.clear();

            _strings.resize(size);
            for (size_t i = 0; i < _strings.size(); i++) {
                _strings[i] = data[i];
            }
        }

        template<typename T>
        void setData(const std::vector<T>& data) {
            _data.allocate(sizeof(T) * data.size());
            _data.copyFrom(data.data(), sizeof(T) * data.size()));
        }

        template<>
        inline void setData(const std::vector<std::string>& data) {
            _data.clear();

            _strings.resize(data.size());
            for (size_t i = 0; i < _strings.size(); i++) {
                _strings[i] = data[i];
            }
        }

        template<typename T>
        void setup(const detail::TSDescriptor& desc, const T& data, const GUIStyle* style = &Gui::Styles::Default) {
            clear();
            _descriptor = desc;
            _style = style;
            setData(data);
        }

        template<typename T>
        void setup(std::string_view name, const detail::TSDescriptor& desc, const T& data, const GUIStyle* style = &Gui::Styles::Default) {
            _name = name;
            setup(desc, style);
            setData(data);
        }



        template<typename T, size_t size>
        void setup(const detail::TSDescriptor& desc, const T data[size], const GUIStyle* style = &Gui::Styles::Default) {
            clear();
            _descriptor = desc;
            _style = style;
            setData(data);
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
            return _descriptor.type ? _descriptor.type->size : valueTypeToSize(_descriptor.valueType);
        }

        size_t getArraySize() const {
            if (_descriptor.nodeType != NodeType::NTYPE_ARRAY) { return 0; }
            return _descriptor.valueType == ValueType::VTYPE_PROPERTY ? _children.size() : _descriptor.type ? _data.size / _descriptor.type->size : _data.size / valueTypeToSize(_descriptor.valueType);
        }

        const std::string& getName() const { return _name; }
        const char* getCName() const { return _name.c_str(); }

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

        SerializedNode* findByPath(std::string_view path);

        void serialize(const Stream& stream) const;
        void deserialize(const Stream& stream);

        void serialize(json& jsonF) const;
        void deserialize(const json& jsonF);

        void serialize(yamlEmit& node) const;
        void deserialize(const yamlNode& node);

        Target getTarget() const { return _target; }

    private:
        friend class SNodePool;
        friend struct PoolChunk<SerializedNode>;

        std::string _name{};

        Target _target;
        SerializedNode* _root;
        detail::TSDescriptor _descriptor;
        const GUIStyle* _style;

        struct Data {
            bool useExternal{false};

            struct Array {
                uint8_t* ptrSt{};
                uint8_t* ptrEn{};

                void clear() {
                    if (ptrSt) {
                        free(ptrSt);
                    }
                    ptrSt = nullptr;
                    ptrEn = nullptr;
                }
                size_t getSize() const { return ptrEn - ptrSt; }

                void allocate(size_t size) {
                    if (size <= getSize()) { return; }
                    if (ptrSt) {
                        void* real = realloc(ptrSt, size);
                        if (real) {
                            ptrSt = reinterpret_cast<uint8_t*>(real);
                        }
                    }
                    else {
                        ptrSt = reinterpret_cast<uint8_t*>(malloc(size));
                    }
                    ptrEn = ptrSt + size;

                    if (ptrSt) {
                        memset(ptrSt, 0, size);
                    }
                }
            };
            union {
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
                memset(&data, 0, sizeof(data));
                useExternal = false;
            }

            void copyFrom(const void* ptr, size_t size) {
                if (useExternal) {
                    memcpy(data.arr.ptrSt, ptr, size);
                    return;
                }
                memcpy(&data, ptr, size);
            }

            void copyTo(void* ptr, size_t size) const {
                if (useExternal) {
                    memcpy(ptr, data.arr.ptrSt, size);
                    return;
                }
                memcpy(ptr , &data, size);
            }

            void copyTo(Data& other) const {
                other.allocate(size);
                if (useExternal) {
                    memcpy(other.data.arr.ptrSt, data.arr.ptrSt, size);
                    return;
                }
                memcpy(&other.data, &data, size);
            }

            void allocate(size_t size) {
                uint8_t temp[64]{0};

                size_t prevSize = this->size;
                this->size = size;
                bool wasExt = useExternal;
                useExternal = size > 64;

                if (wasExt != useExternal) {
                    if (wasExt) {
                        memcpy(temp, data.arr.ptrSt, std::min(prevSize, 64ULL));
                    }
                    else {
                        memcpy(temp, data.buffer, 64);
                    }
                }

                if (useExternal) {
                    data.arr.allocate(size);
                    if (!wasExt && data.arr.ptrSt) {
                        memcpy(data.arr.ptrSt, temp, std::min(prevSize, size));
                    }
                    return;
                }

                if (wasExt) {
                    memcpy(&data, temp, std::min(prevSize, size));
                    if (prevSize < size) {
                        memset(data.buffer + prevSize, 0, size - prevSize);
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
                return reinterpret_cast<T*>(&data);
            }  
            
            template<typename T>
            const T* getAs() const {
                if (useExternal) {
                    return reinterpret_cast<const T*>(data.arr.ptrSt);
                }
                return reinterpret_cast<const T*>(&data);
            }

            template<typename T>
            T* getEndAs() {
                if (useExternal) {
                    return reinterpret_cast<T*>(data.arr.ptrEn);
                }
                return reinterpret_cast<T*>(&data) + (size / sizeof(T));
            }

            template<typename T>
            const T* getEndAs() const {
                if (useExternal) {
                    return reinterpret_cast<T*>(data.arr.ptrEn);
                }
                return reinterpret_cast<T*>(&data) + (size / sizeof(T));
            }

        } _data;
        std::vector<std::string> _strings{ {} };
        std::vector<SerializedNode*> _children{};

        SerializedNode();
        bool drawSelf();
        bool drawSelf(const char* label, NodeType type, ValueType valueType, SubValueType subVType, void* data);

        const GUIStyle* ensureStyle();

        void* getDataIn(size_t& size);
    };
}