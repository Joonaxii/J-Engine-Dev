#include <JEngine/Core/Ref.h>
#include <JEngine/Core/Scene.h>
#include <JEngine/Core/GameObject.h>
#include <JEngine/Components/Component.h>
#include <JEngine/Core/Application.h>
#include <JEngine/Assets/IAsset.h>

namespace JEngine {
    GORef::GORef(const GameObject* go) : uuid(go ? go->getUUID() : detail::NULL_GAME_OBJECT) { }
    GameObject* GORef::get() const {
        return Application::get()->getScene().getByUUID(uuid);
    }

    CompRef::CompRef(const Component* comp) : 
        CompRef(comp ? comp->getUUID() : UINT32_MAX) { }

    Component* CompRef::get() const {
        return Application::get()->getScene().findComponent(*this);
    }

    AssetRef::AssetRef(const IAsset* asset) : _uuid(asset ? asset->getUUID() : detail::NULL_ASSET) { }
    IAsset* AssetRef::get() const {
        if (!isValid()) { return nullptr; }
        return Application::get()->getAssetDB().getAssetByUUID(_uuid);
    }
}