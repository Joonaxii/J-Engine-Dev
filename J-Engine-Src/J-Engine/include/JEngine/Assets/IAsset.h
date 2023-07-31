#pragma once
#include <JEngine/Cryptography/UUIDFactory.h>
#include <JEngine/Core/IObject.h>
#include <cstdint>

namespace JEngine {
    class IAsset : public INamedObject {
    public:
        virtual ~IAsset() {};
        virtual bool unload() {
            UI16Flags& flags = getFlags();
            if (flags & FLAG_IS_LOADED) {
                flags &= ~FLAG_IS_LOADED;
                return true;
            }
            return false;
        }
    protected:
        friend class AssetDB;

        virtual void init(const char* name, UUID8 uuid, uint16_t flags);

        /// <summary>
        /// This method is meant for deallocating/freeing this asset's pointer so it can be reused.
        /// <para>Deallocation is type specific, hence the abstract method.</para>
        /// </summary>
        virtual void destroyAsset() {}
    private:
    };

}
REGISTER_UUID_FACTORY(JEngine::IAsset)
