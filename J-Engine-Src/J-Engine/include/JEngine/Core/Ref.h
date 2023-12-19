#pragma once
#include <cstdint>

namespace JEngine {
    namespace detail {
        static constexpr uint32_t NULL_GAME_OBJECT = 0x0FFFFFFFU;
        static constexpr uint32_t NULL_COMPONENT = 0xF;
        static constexpr uint32_t NULL_COMPONENT_MASK = ~NULL_GAME_OBJECT;

        static constexpr uint32_t NULL_ASSET = 0x1FFFFFFFU;
        static constexpr uint32_t ASSET_SOURCE_MASK = 0x7U;
    }

    class GameObject;
    struct GORef {
    public:
        uint32_t uuid;

        GORef() : uuid(detail::NULL_GAME_OBJECT) {}
        GORef(uint32_t uuid) :
            uuid((uuid& detail::NULL_GAME_OBJECT)) {}
        GORef(const GameObject* go);

        bool isValid() const { return (uuid & detail::NULL_GAME_OBJECT) != detail::NULL_GAME_OBJECT; }

        GameObject* get() const;
        GameObject* operator->() const {
            return get();
        }
        operator GameObject* () const { return get(); }
    };

    class Component;
    struct CompRef {
    public:
        uint32_t uuid{};

        CompRef() : uuid(detail::NULL_GAME_OBJECT | (detail::NULL_COMPONENT << 28)) {}
        CompRef(uint32_t rawUUID) : uuid(rawUUID) {}
        CompRef(uint32_t gameObject, uint32_t component) :
            uuid((gameObject& detail::NULL_GAME_OBJECT) | ((component & detail::NULL_COMPONENT) << 28)) {}
        CompRef(const Component* comp);

        bool isForObject(uint32_t goUUID) const {
            return (goUUID & detail::NULL_GAME_OBJECT) == (uuid & detail::NULL_GAME_OBJECT);
        }

        bool isValid() const {
            return (uuid & detail::NULL_GAME_OBJECT) != detail::NULL_GAME_OBJECT &&
                (uuid & detail::NULL_COMPONENT_MASK) != detail::NULL_COMPONENT_MASK;
        }

        uint32_t getGOUUID() const {
            return uuid & detail::NULL_GAME_OBJECT;
        }

        uint32_t getIndex() const {
            return (uuid >> 28) & detail::NULL_COMPONENT;
        }

        Component* get() const;
        operator Component* () const {
            return get();
        }

        Component* operator->() const {
            return get();
        }
    };

    template<typename T>
    struct TCompRef : public CompRef {
        TCompRef() : CompRef(detail::NULL_GAME_OBJECT | (detail::NULL_COMPONENT << 28)) {}
        TCompRef(uint32_t rawUUID) : CompRef(rawUUID) {}
        TCompRef(uint32_t goUUID, uint32_t component) :
            CompRef((goUUID & detail::NULL_GAME_OBJECT) | ((component & detail::NULL_COMPONENT) << 28)) {}
        TCompRef(CompRef compRef) : CompRef(compRef.uuid) {}
        TCompRef(const Component* comp) : CompRef(comp) { }

        T* as() const {
            return dynamic_cast<T*>(this->get());
        }

        T* operator->() const {
            return as();
        }

        explicit operator T* () const {
            return as();
        }
    };


    class IAsset;
    struct AssetRef {
    public:
        AssetRef() : _uuid(detail::NULL_ASSET) {}
        AssetRef(uint32_t source, uint32_t uuid) : _uuid((uuid& detail::NULL_ASSET) | ((source & 0x7) << 29)) {}
        AssetRef(const IAsset* asset);

        bool isValid() const { return (_uuid & detail::NULL_ASSET) != detail::NULL_ASSET; }
        IAsset* get() const;

        operator IAsset* () const { return get(); }

        IAsset* operator->() const {
            return get();
        }

        uint32_t getSource() const { return (_uuid >> 29); }
        uint32_t getUUID() const { return _uuid & detail::NULL_ASSET; }

    private:
        uint32_t _uuid;
    };

    template<typename T>
    struct TAssetRef : public AssetRef {
    public:
        TAssetRef() : AssetRef() {}
        TAssetRef(uint32_t source, uint32_t uuid) : AssetRef(uuid) {}
        TAssetRef(AssetRef asset) : AssetRef(asset.uuid) { }
        TAssetRef(const T* asset) : AssetRef(asset) { }

        T* as() const {
            return dynamic_cast<T*>(this->get());
        }

        explicit operator T* () const {
            return as();
        }

        T* operator->() const {
            return as();
        }
    };
}