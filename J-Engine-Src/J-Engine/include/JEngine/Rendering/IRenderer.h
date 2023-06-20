#pragma once
#include <JEngine/IO/Serialization/Serializable.h>

#include <JEngine/Rendering/SortingLayer.h>
#include <JEngine/Assets/Graphics/Material/Material.h>

#include <JEngine/Math/Graphics/JVertex.h>
#include <JEngine/Math/Graphics/JColor32.h>
#include <JEngine/Math/Units/JMatrix.h>
#include <JEngine/Math/Units/JRect.h>

#include <JEngine/Collections/ObjectRef.h>

namespace JEngine {
    template<typename TVert>
    class IRenderer {
    public:
        typedef void (*RegistrationEvent)(IRenderer* renderer);

        IRenderer() : _color(255, 255, 255, 255), _material(nullptr), _layer(0, 0), _flags(REND_IS_VISIBLE), _bounds() {
            registerRenderer(this);
        }
        ~IRenderer() { unregisterRenderer(this); }

        void setMaterial(const ObjectRef<Material>& material) { _material = material; }

        Material* getMaterial() { return _material.getPtr(); }
        const Material* getMaterial() const { return _material.getPtr(); }

        void setSortingOrder(const int32_t order) { _layer.setOrder(order); }

        void setSortingLayer(const std::string& layerName) { _layer.setLayerByName(layerName); }
        void setSortingLayer(const uint16_t layer) { _layer.setLayerByIndex(layer); }
        void setSortingLayer(const SortingLayer& layer) { _layer = layer; }

        SortingLayer getSortingLayer() const { return _layer; }

        virtual bool canRender() const { return bool(_flags & REND_IS_VISIBLE); }

        void setVisible(const bool visible) { _flags.setBit(REND_IS_VISIBLE); }
        bool isVisible() const { return (_flags & uint8_t(REND_IS_VISIBLE | REND_IS_IN_VIEW)) == (REND_IS_VISIBLE | REND_IS_IN_VIEW); }
        
        void markOutOfView() { _flags &= ~REND_IS_IN_VIEW; }
        void markInView() { _flags |= REND_IS_IN_VIEW; }

        virtual uint32_t getObjectLayer() const = 0;

        virtual int32_t getVertexCount() const = 0;
        virtual int32_t getIndexCount() const = 0;

        virtual const TVert* getVertices() const = 0;
        virtual const uint32_t* getIndices() const = 0;

        virtual const JMatrix4f& getWorldMatrix() const = 0;

        void setColor(const JColor32& color) { _color = color; }
        const JColor32& getColor() const { return _color; }

        virtual void updateRenderer() {
            auto& mat = getWorldMatrix();
            _bounds = mat.transformRect(getLocalBounds());
        }
        virtual const JRectf& getBounds() const { return _bounds; }

        virtual bool deserializeJson(json& jsonFile) {
            const UUID8 uuid = _material.getPtr() ? _material.getPtr()->getUUID() : UUID8::Empty;
            Serialization::deserialize(uuid, jsonFile["material"]);
            Serialization::deserialize(_color, jsonFile["color"], JColor32::White);
            Serialization::deserialize(_layer, jsonFile["layer"]);
            return true;
        }

        virtual bool serializeJson(json& jsonFile) const {
            UUID8 uuid{};

            Serialization::serialize(uuid, jsonFile["material"]);
            Serialization::serialize(_color, jsonFile["color"]);
            Serialization::serialize(_layer, jsonFile["layer"]);
            return true;
        }

        virtual bool deserializeBinary(const Stream& stream) {
            UUID8 uuid{};

            Serialization::deserialize(uuid, stream);
            Serialization::deserialize(_color, stream);
            Serialization::deserialize(_layer, stream);
            return true;
        }

        virtual bool serializeBinary(const Stream& stream) const {
            const Material* mat = _material.getPtr();
            const UUID8 uuid = mat ? mat->getUUID() : UUID8::Empty;
     
            Serialization::serialize(uuid, stream);
            Serialization::serialize(_color, stream);
            Serialization::serialize(_layer, stream);
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