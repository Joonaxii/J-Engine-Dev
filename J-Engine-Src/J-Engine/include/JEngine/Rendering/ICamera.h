#pragma once
#include <cstdint>
#include <GL/glew.h>
#include <JEngine/Helpers/EnumMacros.h>
#include <JEngine/Math/Graphics/JColor32.h>
#include <JEngine/Math/Graphics/JVertex.h>
#include <JEngine/Core/LayerMask.h>
#include <JEngine/Utility/Flags.h>
#include <JEngine/Math/Units/JRect.h>
#include <JEngine/Math/Units/JMatrix.h>
#include <JEngine/Global/UUIDFactory.h>
#include <JEngine/Collections/ObjectRef.h>
#include <JEngine/Assets/Graphics/Material/Material.h>
#include <JEngine/Rendering/FrameBuffer.h>
#include <JEngine/Rendering/DynamicBatch.h>
#include <JEngine/Rendering/FrameBuffer.h>

namespace JEngine {
    class Renderer;
    class ICamera : public ISerializable<ICamera, sizeof(UI8Flags) + sizeof(float) + sizeof(uint32_t) + sizeof(JColor32) + sizeof(LayerMask) + sizeof(JRectf) + sizeof(JRectf) + sizeof(UUID)> {
    public:
        static const JColor32 DEFAULT_CLEAR_COLOR;

        struct CameraRenderData {
            JMatrix4f projection;
            JRectf worldRect;
            JRectf viewRect;
            JRectf screenRect;
            JVector2i reso;
            FrameBuffer* frameBuffer;
        };

        typedef void (*RegistrationEvent)(ICamera* camera);
        typedef void (*PrepareRender)(const JColor32& clearColor, const uint32_t clearFlags, CameraRenderData& renderInfo);
        typedef void (*RenderObjects)(ICamera* camera, const uint32_t mask, CameraRenderData& renderInfo);
        typedef void (*WriteToBuffer)(Material* blendMaterial, const JColor32& uniformTint, CameraRenderData& renderInfo);
 
     
        enum ClearFlags : uint32_t {
            Clear_None = 0x0,
            Clear_Depth = GL_DEPTH_BUFFER_BIT,
            Clear_Color = GL_COLOR_BUFFER_BIT,
        };

        ICamera();
        ~ICamera();

        Material* getBlendMaterial() { return _blendMaterial.getPtr(); }
        const Material* getBlendMaterial() const { return _blendMaterial.getPtr(); }

        void setViewRect(const JRectf& viewRect);
        const JRectf& getViewRect() const;

        void setScreenRect(const JRectf& screenRect);
        const JRectf& getScreenRect() const;
     
        const int32_t compareTo(const ICamera& other) const;

        void setManualRenderMode(const bool state);
        const bool getManualRenderModeState() const;

        void setCameraDisabled(const bool state);
        const bool isCameraDisabled() const;

        void setCameraDepth(const float depth);
        const float getCameraDepth() const;

        void setClearFlags(const ClearFlags flags);
        const ClearFlags getClearFlags() const;

        void setClearColor(const JColor32& color);
        const JColor32& getClearColor() const;
        
        void setCameraTint(const JColor32& color);
        const JColor32& getCameraTint() const;

        void setLayerMask(const LayerMask mask);
        const LayerMask getLayerMask() const; 

        virtual const bool shouldRender() const;

        const bool render();

        virtual void setTransformMatrix(const JMatrix&) = 0;
        virtual const JMatrix& getTransformMatrix() const = 0;
        virtual void transformProjection(JMatrix& projection) const;

        virtual const bool serializeJson(json& jsonF) const;
        virtual const bool deserializeJson(json& jsonF);

        virtual const bool serializeBinary(std::ostream& stream) const;
        virtual const bool deserializeBinary(std::istream& stream);

        static void setRegistrationMethods(RegistrationEvent reg, RegistrationEvent unreg) {
            REGISTER_CAMERA = reg;
            UNREGISTER_CAMERA = unreg;
        }

        static void setCameraRenderMethods(PrepareRender prepare, RenderObjects render, WriteToBuffer write) {
            PREPARE_RENDER = prepare;
            RENDER_OBJECTS = render;
            WRITE_TO_BUFFER = write;
        }

    protected:
        friend class Renderer;
        virtual const bool renderInternal(const bool isAuto = true);

    protected:
        static constexpr uint8_t AUTO_RENDER_CAMERA = 0x1;
        static constexpr uint8_t DISABLE_CAMERA = 0x2;

        static inline RegistrationEvent REGISTER_CAMERA = nullptr;
        static inline RegistrationEvent UNREGISTER_CAMERA = nullptr;

        static inline PrepareRender PREPARE_RENDER = nullptr;
        static inline RenderObjects RENDER_OBJECTS = nullptr;
        static inline WriteToBuffer WRITE_TO_BUFFER = nullptr;

        UI8Flags _flags;
        float _depth;
        ClearFlags _clearFlags;
        JColor32 _clearColor;
        JColor32 _tintColor;
        LayerMask _layerMask;
        JRectf _viewRect;
        JRectf _screenRect;
        ObjectRef<Material> _blendMaterial;

        static void registerCamera(ICamera* cam) {
            if (REGISTER_CAMERA) {
                REGISTER_CAMERA(cam);
            }
        }
        static void unregisterCamera(ICamera* cam) {
            if (UNREGISTER_CAMERA) {
                UNREGISTER_CAMERA(cam);
            }
        }
    };
}
CREATE_ENUM_OPERATORS_SELF(JEngine::ICamera::ClearFlags);