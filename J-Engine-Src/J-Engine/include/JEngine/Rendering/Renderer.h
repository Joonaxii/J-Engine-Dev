#pragma once 
#include <JEngine/Rendering/Window.h>

#include <JEngine/Rendering/IRenderer.h>
#include <JEngine/Math/Graphics/JVertex.h>

#include <JEngine/Rendering/Buffers/VertexBuffer.h>
#include <JEngine/Rendering/Buffers/VertexArray.h>
#include <JEngine/Rendering/Buffers/IndexBuffer.h>

#include <JEngine/Rendering/RenderingConsts.h>
#include <JEngine/Assets/Graphics/Shader/Shader.h>
#include <JEngine/Rendering/ImGui/IImGuiDrawable.h>
#include <JEngine/Rendering/DynamicBatch.h>
#include <vector>
#include <set>

namespace JEngine {
    class Renderer {
    public:
        typedef DynamicBatch<JVertex2f> DynamicBatch2D;

        struct RenderInfo {
            uint32_t drawCalls;
            uint32_t activeRenderers;
            uint64_t vertexCount;
            uint64_t triCount;

            RenderInfo() : drawCalls(), activeRenderers(), vertexCount(), triCount() {}

            RenderInfo& operator +=(const RenderInfo& other) {
                drawCalls += other.drawCalls;
                activeRenderers += other.activeRenderers;
                vertexCount += other.vertexCount;
                triCount += other.triCount;
                return *this;
            }

            void clear() {
                drawCalls = 0;
                activeRenderers = 0;
                vertexCount = 0;
                triCount = 0;
            }
        };

        Renderer();
        ~Renderer();

        bool init(const char* title, const int32_t width = RenderingConsts::DEF_WINDOW_WIDTH, const int32_t height = RenderingConsts::DEF_WINDOW_HEIGHT);
        void terminate();

        const RenderInfo& getRenderInfo() const;

        bool doRender();

        void setCameraBlendMaterial(const ObjectRef<Material>& blendMaterial);

        static void draw(const VertexArray& va, const IndexBuffer& ib, Material& shader);

        const Window& getWindow() const;
        Window& getWindow();

    private:
        static Renderer* _instance;

        struct RenderInfoGroup {
            RenderInfo renderInfo;
            RenderInfo uiRenderInfo;

            RenderInfoGroup& operator +=(const RenderInfoGroup& other) {
                renderInfo += other.renderInfo;
                uiRenderInfo += other.uiRenderInfo;
                return *this;
            }

            void clear() {
                renderInfo.clear();
                uiRenderInfo.clear();
            }
        };

        bool _initialized;

        BufferLayout _defaultBufferLayout;
        BufferLayout _gizmoBufferLayout;

        uint64_t _tick;
        RenderInfoGroup _renderStats;

        Window _window;

        std::set<IRenderer<JVertex2f>*> _renderers;
        std::vector<IRenderer<JVertex2f>*> _activeRenderers[33];

        std::set<IImGuiDrawable*> _imGuiDrawables;
        std::vector<IImGuiDrawable*> _activeImGuiDrawables;

        ICamera* _firstCam;
        std::map<ICamera*, RenderInfoGroup> _camerasToDraw;

        ObjectRef<Material> _blendMaterial;
        DynamicBatch2D _batch;
        DynamicBatch2D _batchGizmos;
  
        void renderNull(RenderInfo& renderInfo, RenderInfo& uiRenderInfo);

        void prepareCameraRender(const JColor32& clearColor, const uint32_t clearFlags, ICamera::CameraRenderData& renderInfo);
        void renderObjectsToCamera(ICamera* camera, const uint32_t mask, ICamera::CameraRenderData& renderInfo);
        void writeCameraToBuffer(Material* blendMaterial, const JColor32& uniformColor, ICamera::CameraRenderData& renderInfo);

        static void prepareCameraRenderStatic(const JColor32& clearColor, const uint32_t clearFlags, ICamera::CameraRenderData& renderInfo);
        static void renderObjectsToCameraStatic(ICamera* camera, const uint32_t mask, ICamera::CameraRenderData& renderInfo);
        static void writeCameraToBufferStatic(Material* blendMaterial, const JColor32& uniformColor, ICamera::CameraRenderData& renderInfo);
    
        static void registerRenderer(IRenderer<JVertex2f>* renderer);
        static void unregisterRenderer(IRenderer<JVertex2f>* renderer);

        static void registerGuiDrawable(IImGuiDrawable* renderer);
        static void unregisterGuiDrawable(IImGuiDrawable* renderer);

        static void registerCamera(ICamera* camera);
        static void unregisterCamera(ICamera* camera);
    };
}