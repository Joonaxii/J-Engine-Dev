//#include <JEngine/Rendering/IRenderer.h>
//#include <JEngine/rendering/IRenderable.h>
//#include <JEngine/IO/Serialization.h>
//
//namespace JEngine {
//    IRenderer::RegistrationEvent IRenderer::REGISTER_RENDERER = nullptr;
//    IRenderer::RegistrationEvent IRenderer::UNREGISTER_RENDERER = nullptr;
//
//    IRenderer::IRenderer() : _color(JColor32::White), _material(-1) {
//        registerRenderer(this);
//    }
//    IRenderer::~IRenderer() { unregisterRenderer(this); }
//
//    void IRenderer::setColor(const JColor32& color) {
//        _color = color;
//    }
//
//    const JColor32& IRenderer::getColor() const {
//        return _color;
//    }
//
//    void IRenderer::setOrderInLayer(const short order) {
//        _layer.setOrder(order);
//    }
//
//    void IRenderer::setLayer(const uint16_t layer) {
//        _layer.setLayerIndex(layer);
//    }
//
//    void IRenderer::setLayer(const std::string layer) {
//        _layer.setLayerIndex(SortingLayer::nameToLayer(layer, false));
//    }
//
//    const bool IRenderer::deserializeJson(json& jsonFile) {
//        _color.deserializeJson(jsonFile["color"]);
//
//        auto& lr = jsonFile.find("sortingLayer");
//        if (lr != jsonFile.end()) {
//            _layer.deserializeJson(*lr);
//        }
//        else {
//            _layer = SortingLayer(0, 0);
//        }
//        return true;
//    }
//
//    const bool IRenderer::serializeJson(json& jsonFile) const {
//        _color.serializeJson(jsonFile["color"]);
//        _layer.serializeJson(jsonFile["sortingLayer"]);
//        return true;
//    }
//
//    const bool IRenderer::deserializeBinary(std::istream& stream) {
//        _color.deserializeBinary(stream);
//        _layer.deserializeBinary(stream);
//        return true;
//    }
//
//    const bool IRenderer::serializeBinary(std::ostream& stream) const {
//        _color.serializeBinary(stream);
//        _layer.serializeBinary(stream);
//        return true;
//    }
//
//    void IRenderer::setMaterial(const Material& material, const bool makeNew, const bool deletePrevious) {
//        if (&material == _material.getReference()) { return; }
//
//        if (deletePrevious && _material.isValid()) {
//            MaterialManager::deleteMaterial(_material.getID(), true);
//        }
//
//        const Texture2D* tex = getCurrentTexture();
//
//        if (makeNew) {
//            MaterialManager::getNewMaterial(tex, material, _material);
//            return;
//        }
//
//        if (tex == nullptr || material.getTexture() == tex) {
//            _material.setMaterial(material.getMaterialID());
//            return;
//        }
//        MaterialManager::findMaterialByMaterial(tex, material, _material);
//
//        if (!_material.isValid()) {
//            MaterialManager::getNewMaterial(tex, material, _material);
//        }
//    }
//
//    const Material* IRenderer::getMaterial() {
//        return _material.getMaterial();
//    }
//
//    const SortingLayer IRenderer::getLayer() const {
//        return _layer;
//    }
//
//    void IRenderer::setRegistrationMethods(RegistrationEvent reg, RegistrationEvent unreg) {
//        REGISTER_RENDERER = reg;
//        UNREGISTER_RENDERER = unreg;
//    }
//
//    void IRenderer::registerRenderer(IRenderer* rend) {
//        if (REGISTER_RENDERER) {
//            REGISTER_RENDERER(rend);
//        }
//    }
//
//    void IRenderer::unregisterRenderer(IRenderer* rend) {
//        if (UNREGISTER_RENDERER) {
//            UNREGISTER_RENDERER(rend);
//        }
//    }
//}