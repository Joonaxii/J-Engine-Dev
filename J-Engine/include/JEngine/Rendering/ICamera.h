#pragma once
#include <cstdint>
#include <GL/glew.h>
#include <JEngine/Utility/EnumUtils.h>
#include <JEngine/Math/Graphics/JColor32.h>
#include <JEngine/Math/Graphics/JVertex.h>
#include <JEngine/Core/LayerMask.h>
#include <JEngine/Utility/Flags.h>
#include <JEngine/Math/Units/JRect.h>
#include <JEngine/Math/Units/JMatrix.h>
#include <JEngine/Cryptography/UUIDFactory.h>
#include <JEngine/Rendering/FrameBuffer.h>
#include <JEngine/Rendering/DynamicBatch.h>
#include <JEngine/Rendering/FrameBuffer.h>

namespace JEngine {
    class Renderer;
    class ICamera : public ISerializable<ICamera> {
    public:
        static const JColor32 DEFAULT_CLEAR_COLOR;

        struct CameraRenderData {
            JMatrix4f projection;
            JRectf worldRect;
            JRectf viewRect;
            JRectf screenRect;
            JVector2i reso;
            FrameBuffer* frameBuffer;
            ICamera* camera;
            bool doBlend;
        };

        typedef void (*RegistrationEvent)(ICamera* camera);
        typedef void (*PrepareRender)(const JColor32& clearColor, const uint32_t clearFlags, CameraRenderData& renderInfo);
        typedef void (*RenderObjects)(ICamera* camera, const uint32_t mask, CameraRenderData& renderInfo);
        typedef void (*WriteToBuffer)(void* blendMaterial, const JColor32& uniformTint, CameraRenderData& renderInfo);

        enum ClearFlags : uint32_t {
            Clear_None = 0x0,
            Clear_Depth = GL_DEPTH_BUFFER_BIT,
            Clear_Color = GL_COLOR_BUFFER_BIT,
        };

        ICamera();
        ICamera(uint8_t flags);
        ~ICamera();

        //Material* getBlendMaterial() { return _blendMaterial.getPtr(); }
        //const Material* getBlendMaterial() const { return _blendMaterial.getPtr(); }

        void setViewRect(const JRectf& viewRect);
        const JRectf& getViewRect() const;

        void setScreenRect(const JRectf& screenRect);
        const JRectf& getScreenRect() const;
     
        int32_t compareTo(const ICamera& other) const;

        void setManualRenderMode(const bool state);
        bool getManualRenderModeState() const;

        void setCameraDisabled(const bool state);
        bool isCameraDisabled() const;

        void setCameraDepth(const float depth);
        float getCameraDepth() const;

        void setClearFlags(const ClearFlags flags);
        ClearFlags getClearFlags() const;

        void setClearColor(const JColor32& color);
        const JColor32& getClearColor() const;
        
        void setCameraTint(const JColor32& color);
        const JColor32& getCameraTint() const;

        void setLayerMask(const LayerMask mask);
        LayerMask getLayerMask() const; 

        virtual bool shouldRender() const;

        bool render();

        virtual void setTransformMatrix(const JMatrix4f&) = 0;
        virtual const JMatrix4f& getTransformMatrix() const = 0;
        virtual void transformProjection(JMatrix4f& projection) const;

        virtual bool serializeJson(json& jsonF) const override;
        virtual bool deserializeJson(json& jsonF) override;

        virtual bool serializeBinary(const Stream& stream) const override;
        virtual bool deserializeBinary(const Stream& stream) override;

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
        virtual bool renderInternal(const bool isAuto = true);

    protected:
        enum : uint8_t {
            CAMERA_FLAGS_NONE = 0x0,
            AUTO_RENDER_CAMERA = 0x1,
            DISABLE_CAMERA = 0x2,


            CAMERA_NO_REGISTRATION = 0x80,
        };

        static inline RegistrationEvent REGISTER_CAMERA = nullptr;
        static inline RegistrationEvent UNREGISTER_CAMERA = nullptr;

        static inline PrepareRender PREPARE_RENDER = nullptr;
        static inline RenderObjects RENDER_OBJECTS = nullptr;
        static inline WriteToBuffer WRITE_TO_BUFFER = nullptr;

        UI8Flags _cameraFlags;
        float _depth;
        ClearFlags _clearFlags;
        JColor32 _clearColor;
        JColor32 _tintColor;
        LayerMask _layerMask;
        JRectf _viewRect;
        JRectf _screenRect;
        //ObjectRef<Material> _blendMaterial;

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

    template<>
    inline bool Serializable<ICamera::ClearFlags>::deserializeJson(ICamera::ClearFlags& itemRef, const json& jsonF, const ICamera::ClearFlags& defaultValue) {
        itemRef = jsonF.is_number_integer() ? ICamera::ClearFlags(jsonF.get<uint32_t>()) : defaultValue;
        return true;
    }

    template<>
    inline bool Serializable<ICamera::ClearFlags>::serializeJson(const ICamera::ClearFlags& itemRef, json& jsonF) {
        jsonF.update(uint32_t(itemRef));
        return true;
    }
}
CREATE_ENUM_OPERATORS_SELF(JEngine::ICamera::ClearFlags);