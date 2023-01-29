#pragma once
#include <JEngine/IO/Serialization/ISerializable.h>

#include <JEngine/Rendering/SortingLayer.h>
#include <JEngine/Assets/Graphics/Material/Material.h>

#include <JEngine/Math/Graphics/JVertex.h>
#include <JEngine/Math/Graphics/JColor32.h>
#include <JEngine/Math/Units/JMatrix.h>
#include <JEngine/Math/Units/JRect.h>

#include <JEngine/IO/Serialization/Serialization.h>
#include <JEngine/Collections/ObjectRef.h>

namespace JEngine {
    template<typename TVert>
    class IRenderer : public ISerializable<IRenderer<TVert>, sizeof(SortingLayer) + sizeof(JColor32) + sizeof(UUID)> {
    public:
        typedef void (*RegistrationEvent)(IRenderer* renderer);

        IRenderer() : _color(255, 255, 255, 255), _material(nullptr), _layer(0, 0), _flags(REND_IS_VISIBLE), _bounds() {
            registerRenderer(this);
        }
        ~IRenderer() {
            unregisterRenderer(this);
        }

        void setMaterial(const ObjectRef<Material>& material) {
            _material = material;
        }

        Material* getMaterial() { return _material.getPtr(); }
        const Material* getMaterial() const { return _material.getPtr(); }

        void setSortingOrder(const int16_t order) {
            _layer.setOrder(order);
        }
        void setSortingLayer(const std::string& layerName) {
            _layer.setLayerByName(layerName);
        }

        void setSortingLayer(const uint16_t layer) {
            _layer.setLayerByIndex(layer);
        }

        void setSortingLayer(const SortingLayer& layer) {
            _layer = layer;
        }

        const SortingLayer getSortingLayer() const { return _layer; }

        virtual const bool canRender() const {
            return bool(_flags & REND_IS_VISIBLE);
        }

        void setVisible(const bool visible) {
            _flags.setBit(REND_IS_VISIBLE)
        }

        const bool isVisible() const { return (_flags & (REND_IS_VISIBLE | REND_IN_VIEW)) == (REND_IS_VISIBLE & REND_IN_VIEW); }
        
        void markOutOfView() {
            _flags &= ~REND_IS_IN_VIEW;
        }
        void markInView() {
            _flags |= REND_IS_IN_VIEW;
        }

        virtual const uint32_t getObjectLayer() const = 0;

        virtual const int32_t getVertexCount() const = 0;
        virtual const int32_t getIndexCount() const = 0;

        virtual const TVert* getVertices() const = 0;
        virtual const uint32_t* getIndices() const = 0;

        virtual const JMatrix4f& getWorldMatrix() const = 0;

        void setColor(const JColor32& color) { _color = color; }
        const JColor32& getColor() const { return _color; }

        virtual void updateRenderer() {
            auto& mat = getWorldMatrix();
            _bounds = mat.transformRect(getLocalBounds());
        }
        virtual const JRectf& getBounds() const {
            return _bounds;
        }

        virtual const bool deserializeJson(json& jsonFile) {
            const UUID matUUID = _material.getPtr() ? _material.getPtr()->getUUID() : UUIDFactory::UUID_EMPTY;
            jsonFile["material"] = UUIDFactory::getUUIDStr(matUUID);

            _color.serializeJson(jsonFile["color"]);
            _layer.serializeJson(jsonFile["layer"]);
            return true;
        }
        virtual const bool serializeJson(json& jsonFile) const {
            UUID uuid;
            UUIDFactory::parseUUID(jsonFile.value("material", ""), uuid);

            _color.serializeJson(jsonFile["color"]);
            _layer.serializeJson(jsonFile["layer"]);
            return true;
        }

        virtual const bool deserializeBinary(std::istream& stream) {
            UUID uuid;
            Serialization::deserializeBinary(stream, uuid);

            _color.deserializeBinary(stream);
            _layer.deserializeBinary(stream);
            return true;
        }

        virtual const bool serializeBinary(std::ostream& stream) const {
            const Material* mat = _material.getPtr();
            const UUID uuid = mat ? mat->getUUID() : UUIDFactory::UUID_EMPTY;
            Serialization::serializeBinary(stream, uuid);

            _color.serializeBinary(stream);
            _layer.serializeBinary(stream);
            return true;
        }

        static void setRegistrationMethods(RegistrationEvent reg, RegistrationEvent unreg) {
            REGISTER_RENDERER = reg;
            UNREGISTER_RENDERER = unreg;
        }

    protected:
        static constexpr uint8_t REND_IS_IN_VIEW = 0x1;
        static constexpr uint8_t REND_IS_VISIBLE = 0x2;

        static inline RegistrationEvent REGISTER_RENDERER = nullptr;
        static inline RegistrationEvent UNREGISTER_RENDERER = nullptr;

        UI8Flags _flags;

        JColor32 _color;
        SortingLayer _layer;
        ObjectRef<Material> _material;

        JRectf _bounds;

        virtual const JRectf& getLocalBounds() const = 0;

        static void registerRenderer(IRenderer<TVert>* rend) {
            if (REGISTER_RENDERER) {
                REGISTER_RENDERER(rend);
            }
        }
        static void unregisterRenderer(IRenderer<TVert>* rend) {
            if (UNREGISTER_RENDERER) {
                UNREGISTER_RENDERER(rend);
            }
        }
    };
}