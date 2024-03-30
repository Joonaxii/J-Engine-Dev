#pragma once
#include <cstdint>

namespace JEngine {
    /*
        ----------------------------------------------------
        UUID Bit Structures:

        Maximum number of GameObjects is 16777215 (2^24 - 1), 0x00FFFFFFU is used as a NULL ref.
        GO/CompRef: 55554444-33333333-33333333-33333333-22221111-00000000-00000000-00000000
           - 0 = Main GameObject Index
           - 1 = GameObject Type (Singleton, Prefab, Normal)
           - 2 = Component Index (Unused in GORef)

           - 3 = Relative GameObject Index (Used for Prefabs)
           - 4 = Relative GameObject Type (Unused)
           - 5 = Relative GO Component Index (Unused)

        AssetRef: 33333333-21111111-00000000-00000000-00000000-00000000-00000000-00000000
            - 0 = Asset Index
            - 1 = Asset Source (Source VFS/PAK)
            - 2 = From DB Flag
            - 3 = Asset Type (Potentially, might be useful to encode the type into the UUID)
    */

    enum GOType : uint64_t {
        GO_Normal = 0x00000000ULL,
        GO_Prefab = 0x01000000ULL,
        GO_Singleton = 0x02000000ULL,
    };

    namespace detail {
        static constexpr uint64_t NULL_GAME_OBJECT  = 0x00FFFFFF00FFFFFFULL;
        static constexpr uint64_t COMPONENT_MASK    = 0x7U;
        static constexpr uint64_t COMPONENT_MASK_FU = 0xFU;
        static constexpr uint64_t COMPONENT_FLAG    = 0x80000000ULL;
        static constexpr uint64_t GO_INFO_MASK      = 0x0FFFFFFF0FFFFFFFULL;
        static constexpr uint64_t GO_INDX_MASK      = 0x00FFFFFF00FFFFFFULL;
        static constexpr uint64_t GO_INFO_MASK_ONE  = 0x0FFFFFFFULL;
        static constexpr uint64_t GO_INDX_MASK_ONE  = 0x00FFFFFFULL;
        static constexpr uint64_t GO_TYPE_MASK      = 0x0F000000ULL;

        static constexpr int32_t GO_MASK_BITS        = 24;
        static constexpr int32_t COMPONENT_SHIFT     = GO_MASK_BITS;
        static constexpr int32_t GO_TYPE_SHIFT       = COMPONENT_SHIFT + 4;

        static constexpr uint64_t NULL_ASSET         = 0x0000FFFFFFFFFFFFULL;
        static constexpr uint64_t ASSET_SOURCE_MASK  = 0x007F000000000000ULL;
        static constexpr uint64_t FROM_DB_MASK       = 0x0080000000000000ULL;
        static constexpr uint64_t ASSET_TYPE_MASK    = 0xFF00000000000000ULL;
        static constexpr int32_t ASSET_MASK_BITS     = 48;
        static constexpr int32_t ASSET_SOURCE_SHIFT  = ASSET_MASK_BITS;
        static constexpr int32_t ASSET_TYPE_SHIFT    = ASSET_SOURCE_SHIFT + 8;
    }

    struct Ref {
        uint64_t uuid;
        constexpr Ref() noexcept : uuid{} {}
        constexpr Ref(uint64_t uuid) : uuid(uuid) {}
    };

    class GameObject;
    struct GORef : Ref {
    public:
        constexpr GORef() : Ref(detail::NULL_GAME_OBJECT) {}
        constexpr GORef(uint64_t uuid) :
            Ref((uuid& detail::NULL_GAME_OBJECT)) {}
        GORef(const GameObject* go);

        GameObject* get() const;
        GameObject* operator->() const {
            return get();
        }
        operator GameObject* () const { return get(); }

        constexpr GOType getType() const { return GOType(uuid & detail::GO_TYPE_MASK); }
        constexpr uint32_t getIndex(bool main) const { 
            return  main ? uint32_t(uuid & detail::GO_INDX_MASK_ONE) : uint32_t((uuid >> 32) & detail::GO_INDX_MASK_ONE);
        }

        constexpr bool GORef::isValid() const {
            switch (uuid & detail::GO_TYPE_MASK)
            {
                default: //Assume normal/singleton
                    return (uuid & detail::GO_INDX_MASK_ONE) != detail::GO_INDX_MASK_ONE;
                case GO_Prefab:
                    return (uuid & detail::GO_INDX_MASK_ONE) != detail::GO_INDX_MASK_ONE &&
                        ((uuid >> 32) & detail::GO_INDX_MASK_ONE) != detail::GO_INDX_MASK_ONE;
            }
        }
    };

    class Component;
    struct CompRef : Ref {
    public:
        constexpr CompRef() : Ref(detail::NULL_GAME_OBJECT | detail::COMPONENT_FLAG) {}
        constexpr CompRef(uint64_t rawUUID) : Ref(rawUUID) {}
        constexpr CompRef(uint64_t gameObject, uint64_t component) :
            Ref((gameObject & detail::GO_INFO_MASK) | ((component & detail::COMPONENT_MASK_FU) << detail::COMPONENT_SHIFT)) {}
        CompRef(const Component* comp);

        bool isForObject(uint64_t goUUID) const {
            return (goUUID & detail::GO_INFO_MASK) == (uuid & detail::GO_INFO_MASK);
        }

        constexpr bool isValid() const {
            return GORef(uuid).isValid() &&
                (uuid & detail::COMPONENT_FLAG) != 0;
        }

        GORef getGORef() const {
            return GORef(uuid);
        }

        uint8_t getIndex() const {
            return uint8_t((uuid >> detail::COMPONENT_SHIFT) & detail::COMPONENT_MASK_FU);
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
    struct AssetRef : Ref {
    public:
        constexpr AssetRef() : Ref(detail::NULL_ASSET) {}
        constexpr AssetRef(bool fromDB, uint64_t type, uint64_t source, uint64_t uuid) : 
            Ref((uuid& detail::NULL_ASSET) | ((source << detail::ASSET_SOURCE_SHIFT) & detail::ASSET_SOURCE_MASK) | (fromDB ? detail::FROM_DB_MASK : 0x00) | ((type << detail::ASSET_TYPE_SHIFT) & detail::ASSET_TYPE_MASK)) {}
        AssetRef(const IAsset* asset);

        constexpr bool isValid() const { return (uuid & detail::NULL_ASSET) != detail::NULL_ASSET; }
        IAsset* get() const;

        operator IAsset* () const { return get(); }

        IAsset* operator->() const {
            return get();
        }

        constexpr bool isFromDB() const { return (uuid & detail::FROM_DB_MASK) != 0; }
        constexpr uint8_t getSource() const { return uint8_t((uuid & detail::ASSET_SOURCE_MASK) >> detail::ASSET_SOURCE_SHIFT); }
        constexpr uint8_t getType() const { return uint8_t(((uuid & detail::ASSET_TYPE_MASK) >> detail::ASSET_TYPE_SHIFT)); }
        constexpr uint64_t getUUID() const { return uuid & detail::NULL_ASSET; }
    };

    template<typename T>
    struct TAssetRef : public AssetRef {
    public:
        constexpr TAssetRef() : AssetRef() {}
        constexpr TAssetRef(bool fromDB, uint64_t type, uint64_t source, uint64_t uuid) : AssetRef(fromDB, type, source, uuid) {}
        constexpr TAssetRef(AssetRef asset) : AssetRef(asset.uuid) { }
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