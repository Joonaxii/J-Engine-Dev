#include <JEngine/Rendering/Renderer.h>
#include <JEngine/Algorithm/BinarySearch.h>
#include <JEngine/Rendering/SortingLayer.h>
#include <imgui.h>
#include <imgui_impl_opengl3.h>
#include <imgui_impl_glfw.h>
#include <algorithm>

namespace JEngine {
    typedef IRenderer<JVertex2f> IRenderer2D;
    Renderer* Renderer::_instance = nullptr;

    namespace priv {

        static const uint32_t QUAD_INDICES[6]{
            0, 1, 2,
            2, 3, 0
        };

        static const JVertex2f QUAD[4]{
            { {0, 0}, {255, 255, 255, 255}, {0, 0}},
            { {1, 0}, {255, 255, 255, 255}, {1, 0}},
            { {1, 1}, {255, 255, 255, 255}, {1, 1}},
            { {0, 1}, {255, 255, 255, 255}, {0, 1}},
        };

        static void calculateFramebufferRects(
            JVector2i& minSrc, JVector2i& sizeSrc,
            JVector2i& minDst, JVector2i& sizeDst,
            JRectf viewRect, JRectf screenRect,
            const size_t viewW, const size_t viewH,
            const size_t screenW, const size_t screenH) {

            JVector2f viewSize = viewRect.getMax();
            viewSize.x *= viewW;
            viewSize.y *= viewH;

            JVector2f screenSize = screenRect.getMax();
            screenSize.x *= screenW;
            screenSize.y *= screenH;

            minSrc.x = int32_t(viewRect.getMin().x * viewW);
            minSrc.y = int32_t(viewRect.getMin().y * viewH);

            sizeSrc.x = int32_t(viewSize.x);
            sizeSrc.y = int32_t(viewSize.y);

            minDst.x = int32_t(screenRect.getMin().x * screenW);
            minDst.y = int32_t(screenRect.getMin().y * screenH);

            sizeDst.x = int32_t(screenSize.x);
            sizeDst.y = int32_t(screenSize.y);
        }

        struct MaterialGroup {
            Material* material;
            uint32_t length[33];

            const bool hasItems() const {
                for (size_t i = 0; i < 33; i++) {
                    if (length[i]) { return true; }
                }
                return false;
            }

            void clearItems() {
                memset(length, 0, sizeof(length));
            }
        };

        const void generateMaterialGroups(const std::vector<uint64_t>& workingSet, const std::vector<IRenderer2D*> activeRenderers[33], std::vector<MaterialGroup>& groups) {
            MaterialGroup curGrp{ nullptr, 0, };
            uint32_t vertCount = 0;
            uint32_t indCount = 0;

            for (size_t i = 0; i < workingSet.size(); i++)
            {
                auto ind = workingSet[i];
                const int32_t bInd = int32_t(ind >> 32);
                const auto rInd = int32_t(ind & 0xFFFFFFFF);

                auto& rends = activeRenderers[bInd];
                IRenderer2D* rend = rends[rInd];

                Material* matR = rend->getMaterial();

                const uint32_t vertC = rend->getVertexCount();
                const uint32_t indC = rend->getIndexCount();
                vertCount += vertC;
                indCount += indC;

                if (((vertCount <= Renderer::DynamicBatch2D::MAX_VERTS) && (indCount <= Renderer::DynamicBatch2D::MAX_INDICES)) &&
                    (!matR && !curGrp.material) || (curGrp.material && *curGrp.material == *matR)) {
                    curGrp.length[bInd]++;
                }
                else {
                    if (curGrp.hasItems()) {
                        groups.push_back(curGrp);
                    }
                    curGrp.clearItems();

                    curGrp.length[bInd] = 1;
                    curGrp.material = matR;

                    vertCount = vertC;
                    indCount = indC;
                }
            }
            if (curGrp.hasItems()) {
                groups.push_back(curGrp);
            }
        }

        const int32_t sortRenderers(IRenderer2D* lhs, IRenderer2D* rhs) {
            const int32_t sortComp = lhs->getSortingLayer().compareTo(rhs->getSortingLayer());

            if (sortComp) {
                return sortComp;
            }

            Material* matA = lhs->getMaterial();
            Material* matB = rhs->getMaterial();

            if (matA && matB) {
                auto& matRA = *matA;
                auto& matRB = *matB;
                if (matRA == matRB) { return 0; }

                if (matRA < matRB) { return -1; }
                return 1;
            }

            if (!matA && !matB) {
                return 0;
            }
            return !matA ? -1 : 1;
        }

        const int32_t sortGuiDrawables(IImGuiDrawable* lhs, IImGuiDrawable* rhs) {
            const int32_t sortComp = lhs->compareTo(*rhs);
            return sortComp;
        }

        const int32_t sortCameras(ICamera* lhs, ICamera* rhs) {
            const int32_t sortComp = lhs->compareTo(*rhs);
            return sortComp;
        }

    }

    Renderer::Renderer() : _batch(), _batchGizmos(), _firstCam(nullptr)
        /*, _window()*/, _tick(), _initialized(false), _blendMaterial(nullptr), _defaultBufferLayout(), _gizmoBufferLayout() {
        if (!_instance) {
            _instance = this;
            IRenderer2D::setRegistrationMethods(registerRenderer, unregisterRenderer);
            IImGuiDrawable::setRegistrationMethods(registerGuiDrawable, unregisterGuiDrawable);
            ICamera::setRegistrationMethods(registerCamera, unregisterCamera);
            ICamera::setCameraRenderMethods(prepareCameraRenderStatic, renderObjectsToCameraStatic, writeCameraToBufferStatic);

            _defaultBufferLayout.push<float>(2);
            _defaultBufferLayout.push<uint8_t>(4);
            _defaultBufferLayout.push<float>(2);

            _gizmoBufferLayout.push<float>(2);
            _gizmoBufferLayout.push<uint8_t>(4);
            _gizmoBufferLayout.push<float>(2);
            _gizmoBufferLayout.push<float>(3);
        }
    }
    Renderer::~Renderer() {
        terminate();
    }

    bool Renderer::init(const char* title, const int32_t width, const int32_t height) {
        if (_initialized) {
            std::cout << "[JEngine - Renderer] Warning: Already initialized!" << std::endl;
            return true;
        }
        const int32_t glfwErr = glfwInit();
        if (glfwErr == GLFW_FALSE) {
            std::cout << "[JEngine - Renderer] Error: Failed to initialize GLFW!" << std::endl;
            terminate();
            return false;
        }

       // if (!_window.init(title, width, height)) {
       //     std::cout << "[JEngine - Renderer] Error: Failed to initialize GLFW window!" << std::endl;
       //     terminate();
       //     return false;
       // }
        _batch.init(_defaultBufferLayout);
        _batchGizmos.init(_gizmoBufferLayout, DynamicBatch2D::MAX_VERTS >> 1, DynamicBatch2D::MAX_INDICES >> 1);

        GLCall(glEnable(GL_BLEND));
        glEnable(GL_SCISSOR_TEST);
        std::cout << "[JEngine - Renderer - OpenGL] Version: " << glGetString(GL_VERSION) << std::endl;

        ImGui::CreateContext();
       //ImGui_ImplGlfw_InitForOpenGL(_window.getWindowPtr(), true);
       //ImGui_ImplOpenGL3_Init();

        ImGui::StyleColorsDark();
        _initialized = true;
        return true;
    }

    void Renderer::terminate() {
        if (!_initialized) { return; }
        if (_instance == this) {
            _instance = nullptr;
            IRenderer2D::setRegistrationMethods(nullptr, nullptr);
        }

        _batch.release();
        _batchGizmos.release();

        ImGui_ImplOpenGL3_Shutdown();
        ImGui_ImplGlfw_Shutdown();
        ImGui::DestroyContext();

       // _window.close();
        glfwTerminate();
        _initialized = false;
    }

    const Renderer::RenderInfo& Renderer::getRenderInfo() const {
        return _renderStats.renderInfo;
    }

    bool Renderer::doRender() {
        //if (!_window.isInitialized()) { return false; }
        //if (_window.isMinimized()) { 
        //    _window.pollEvents();
        //    return true; 
        //}
        //if (!_window.tick()) { return false; }

        for (size_t i = 0; i < 33; i++) {
            _activeRenderers[i].clear();
        }

        //Normal Renderers
        for (auto& rend : _renderers) {
            if (rend->canRender()) {
                rend->markOutOfView();
                rend->updateRenderer();
                const int32_t ind = Math::potToIndex(rend->getObjectLayer()) + 1;
                _activeRenderers[ind].push_back(rend);
            }
        }

        for (size_t i = 0; i < 33; i++) {
            std::sort(_activeRenderers[i].begin(), _activeRenderers[i].end(), priv::sortRenderers);
        }

        //TODO: Add UI Renderers etc
        ////////////////////////////

        std::vector<ICamera*> cameras;
        _firstCam = nullptr;
        for (std::pair<ICamera*, RenderInfoGroup> cam : _camerasToDraw) {
            if (cam.first->getLayerMask() && !cam.first->isCameraDisabled()) {
                cameras.push_back(cam.first);
            }
        }
        std::sort(cameras.begin(), cameras.end(), priv::sortCameras);

        _renderStats.clear();

        //_window.resetViewport();
        GLCall(glBindFramebuffer(GL_FRAMEBUFFER, 0));
        //_window.clear(JColor32::Clear, ICamera::Clear_None);

        for (ICamera* cam : cameras) {
            if (!_firstCam && !cam->getManualRenderModeState()) {
                _firstCam = cam;
            }

            auto& stats = _camerasToDraw[cam];
            stats.clear();
            cam->renderInternal();
            _renderStats += stats;
        }

        renderNull(_renderStats.renderInfo, _renderStats.uiRenderInfo);

        _renderStats.renderInfo.activeRenderers = 0;
        for (size_t i = 0; i < 33; i++) {
            for (auto& rend : _activeRenderers[i]) {
                if (rend->isVisible()) {
                    _renderStats.renderInfo.activeRenderers++;
                }
            }
        }

        //Process GuiElements
        _activeImGuiDrawables.clear();
        for (auto& draw : _imGuiDrawables) {
            if (draw->canRenderGui()) {
                _activeImGuiDrawables.push_back(draw);
            }
        }
        std::sort(_activeImGuiDrawables.begin(), _activeImGuiDrawables.end(), priv::sortGuiDrawables);

        GLCall(glBindFramebuffer(GL_FRAMEBUFFER, 0));
        //_window.resetViewport();
        //Render ImGui
        if (_activeImGuiDrawables.size() > 0) {
          //  _window.resetViewport();
            ImGui_ImplGlfw_NewFrame();
            ImGui_ImplOpenGL3_NewFrame();
            ImGui::NewFrame();

            for (auto& draw : _activeImGuiDrawables) {
                ImGui::PushID(draw);
                draw->onImGui();
                ImGui::PopID();
            }

            ImGui::Render();
            ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
        }
       // _window.finalizeFrame();

        _tick++;
       // _window.pollEvents();
        return true;
    }

    void Renderer::setCameraBlendMaterial(const ObjectRef<Material>& blendMaterial) {
        _blendMaterial = blendMaterial;
    }

    void Renderer::renderNull(RenderInfo& renderInfo, RenderInfo& uiRenderInfo) {
        JMatrix4f projection = /*_window.getWorldProjectionMatrix()*/{};
        JRectf worldRect = /*_window.getWorldRect() */{};
        JRectf viewRect = JRectf(0, 0, 1, 1);
        JRectf screenRect = JRectf(0, 0, 1, 1);
        JVector2i reso = { int32_t(/*_window.getWidth()*/), int32_t(/*_window.getHeight()*/) };

        FrameBuffer fBuffer =/* _window.getScreenBuffer()*/FrameBuffer();

        fBuffer.bind();

        //_window.resetViewport();
        //_window.clear(JColor32::Clear, ICamera::Clear_Color | ICamera::Clear_Depth);

        std::vector<uint64_t> workingSet;
        std::vector<priv::MaterialGroup> groups{};

        auto& rendPool = _activeRenderers[0];
        for (uint64_t k = 0; k < rendPool.size(); k++) {
            auto rend = rendPool[k];
            auto layer = rend->getObjectLayer();

            auto& bounds = rend->getBounds();
            if (bounds.intersects(worldRect)) {
                workingSet.push_back(k);
                rend->markInView();
            }
        }

        generateMaterialGroups(workingSet, _activeRenderers, groups);

        uint32_t pos = 0;
        JVector2i srcMin;
        JVector2i srcSiz;

        JVector2i dstMin;
        JVector2i dstSiz;

        priv::calculateFramebufferRects(
            srcMin, srcSiz,
            dstMin, dstSiz,
            viewRect, screenRect,
            0,0,0,0
            /*_window.getWidth(), _window.getHeight(),
            _window.getWidth(), _window.getHeight()*/);

       // _window.resetViewport();

        std::set<Shader*> shaderSet;
        for (priv::MaterialGroup& mGrp : groups) {
            _batch.setup(mGrp.material);

            Shader* shader = mGrp.material ? mGrp.material->getShaderPtr() : nullptr;
            if (shader && shaderSet.find(shader) == shaderSet.end()) {
                shaderSet.insert(shader);
                shader->bind();
                shader->setUniformMat4f("_MVP", projection);
            }

            IRenderer2D** rends = _activeRenderers[0].data() + pos;
            for (size_t j = 0; j < mGrp.length[0]; j++) {
                auto rend = rends[j];
                _batch.addVerts(rend->getWorldMatrix(), rend->getColor(), rend->getVertices(), rend->getVertexCount(), rend->getIndices(), rend->getIndexCount());
            }
            pos += mGrp.length[0];

            if (_batch.drawBatch()) {
                renderInfo.drawCalls++;
                renderInfo.vertexCount += _batch.getVertexCount();
                renderInfo.triCount += _batch.getIndexCount() / 3;
            }
        }

        auto matBlend = _blendMaterial.getPtr();
        if (matBlend && matBlend->getShaderPtr()) {
            GLCall(glBindFramebuffer(GL_FRAMEBUFFER, 0));

          //  _window.updateViewport(srcMin + dstMin, srcSiz, 0x2);
            _batch.setup(matBlend);

            matBlend->setFrameBuffer(&fBuffer);

            matBlend->getShaderPtr()->bind();
           // matBlend->getShaderPtr()->setUniformMat4f("_MVP", _window.getScreenProjectionMatrix());

            JMatrix4f mat = JMatrix4f(screenRect.getMin(), 0, screenRect.getMax());

            _batch.addVerts(mat, JColor32::White, priv::QUAD, 4, priv::QUAD_INDICES, 6);

            _batch.drawBatch();
            matBlend->setFrameBuffer(nullptr);
        }
        GLCall(glBindFramebuffer(GL_READ_FRAMEBUFFER, 0));
        GLCall(glBindFramebuffer(GL_DRAW_FRAMEBUFFER, 0));
        fBuffer.unbind();
    }

    void Renderer::draw(const VertexArray& va, const IndexBuffer& ib, Material& material) {
        material.bind();
        va.bind();
        ib.bind();
        GLCall(glDrawElements(GL_TRIANGLES, ib.getCount(), GL_UNSIGNED_INT, nullptr));
    }

    //const Window& Renderer::getWindow() const { return Window(); }
    //Window& Renderer::getWindow() { return Window(); }

    void Renderer::prepareCameraRenderStatic(const JColor32& clearColor, const uint32_t clearFlags, ICamera::CameraRenderData& renderInfo) {
        if (!_instance) { return; }
        _instance->prepareCameraRender(clearColor, clearFlags, renderInfo);
    }

    void Renderer::prepareCameraRender(const JColor32& clearColor, const uint32_t clearFlags, ICamera::CameraRenderData& renderInfo) {
       // renderInfo.projection = _window.getWorldProjectionMatrix();
       // renderInfo.worldRect = _window.getWorldRect();
       // renderInfo.reso = { int32_t(_window.getWidth()), int32_t(_window.getHeight()) };
       // renderInfo.frameBuffer = &_window.getScreenBuffer();

        renderInfo.frameBuffer->bind();
        bool noColor = (clearFlags & GL_COLOR_BUFFER_BIT) == 0;
        bool first = (renderInfo.camera == _firstCam);
        renderInfo.doBlend = noColor || first;

        //_window.resetViewport();
       // _window.clear(first || noColor ? clearColor : JColor32::Clear, first ? clearFlags : clearFlags | GL_COLOR_BUFFER_BIT);
    }


    void Renderer::renderObjectsToCameraStatic(ICamera* camera, const uint32_t mask, ICamera::CameraRenderData& renderInfo) {
        if (!_instance) { return; }
        _instance->renderObjectsToCamera(camera, mask, renderInfo);
    }

    void Renderer::renderObjectsToCamera(ICamera* camera, const uint32_t mask, ICamera::CameraRenderData& renderInfo) {
        std::vector<uint64_t> workingSet;
        std::vector<priv::MaterialGroup> groups{};

        auto& camRenderInfo = _camerasToDraw[camera];
        for (uint64_t i = 1, j = 1; i < 33; i++, j <<= 1) {
            if (mask & j) {
                auto& rendPool = _activeRenderers[i];
                for (uint64_t k = 0; k < rendPool.size(); k++) {
                    auto rend = rendPool[k];
                    auto layer = rend->getObjectLayer();

                    if (camera && !(camera->getLayerMask() & layer)) { continue; }
                    else if (!camera && layer != 0) { continue; }

                    auto& bounds = rend->getBounds();
                    if (bounds.intersects(renderInfo.worldRect)) {
                        workingSet.push_back(k | (i << 32));
                        rend->markInView();
                    }
                }
            }
        }
        generateMaterialGroups(workingSet, _activeRenderers, groups);

        uint32_t pos[33];
        memset(pos, 0, sizeof(pos));
        std::set<Shader*> shaderSet;
        for (priv::MaterialGroup& mGrp : groups) {
            _batch.setup(mGrp.material);

            Shader* shader = mGrp.material ? mGrp.material->getShaderPtr() : nullptr;
            if (shader && shaderSet.find(shader) == shaderSet.end()) {
                shaderSet.insert(shader);
                shader->bind();
                shader->setUniformMat4f("_MVP", renderInfo.projection);
            }

            for (size_t i = 1; i < 33; i++) {
                IRenderer2D** rends = _activeRenderers[i].data() + pos[i];
                for (size_t j = 0; j < mGrp.length[i]; j++)
                {
                    auto rend = rends[j];
                    _batch.addVerts(rend->getWorldMatrix(), rend->getColor(), rend->getVertices(), rend->getVertexCount(), rend->getIndices(), rend->getIndexCount());
                }
                pos[i] += mGrp.length[i];
            }

            if (_batch.drawBatch()) {
                camRenderInfo.renderInfo.drawCalls++;
                camRenderInfo.renderInfo.vertexCount += _batch.getVertexCount();
                camRenderInfo.renderInfo.triCount += _batch.getIndexCount() / 3;
            }
        }
    }


    void Renderer::writeCameraToBufferStatic(Material* blendMaterial, const JColor32& uniformColor, ICamera::CameraRenderData& renderInfo) {
        if (!_instance) { return; }
        _instance->writeCameraToBuffer(blendMaterial, uniformColor, renderInfo);
    }

    void Renderer::writeCameraToBuffer(Material* blendMaterial, const JColor32& uniformColor, ICamera::CameraRenderData& renderInfo) {
        JVector2i srcMin;
        JVector2i srcSiz;

        JVector2i dstMin;
        JVector2i dstSiz;

        priv::calculateFramebufferRects(
            srcMin, srcSiz,
            dstMin, dstSiz,
            renderInfo.viewRect, renderInfo.screenRect,
            0,0,0,0
            /*_window.getWidth(), _window.getHeight(),
            _window.getWidth(), _window.getHeight()*/);

        Material* matBlend = blendMaterial;
        bool isValid = matBlend && matBlend->getShaderPtr();

        if (!isValid) {
            matBlend = _blendMaterial.getPtr();
            isValid = matBlend && matBlend->getShaderPtr();
        }

        if (isValid) {
            static bool isBlendEnabled = true;

            GLCall(glBindFramebuffer(GL_FRAMEBUFFER, 0));

           // _window.updateViewport(srcMin + dstMin, srcSiz, 0x2);
 
            matBlend->setFrameBuffer(renderInfo.frameBuffer);

            matBlend->getShaderPtr()->bind();
           // matBlend->getShaderPtr()->setUniformMat4f("_MVP", _window.getScreenProjectionMatrix());

            JMatrix4f mat = JMatrix4f(renderInfo.screenRect.getMin(), 0, renderInfo.screenRect.getMax());
            if (isBlendEnabled != renderInfo.doBlend) {
                isBlendEnabled = renderInfo.doBlend;

                if (isBlendEnabled) {
                    glEnable(GL_BLEND);
                }
                else {
                    glDisable(GL_BLEND);
                }
            }

            _batch.setup(matBlend);
            _batch.addVerts(mat, uniformColor, priv::QUAD, 4, priv::QUAD_INDICES, 6);

            //If Batch fails to draw, do a simple blit.
            _batch.drawBatch();
            matBlend->setFrameBuffer(nullptr);

            if (!isBlendEnabled) {
                isBlendEnabled = true;
                glEnable(GL_BLEND);
            }
        }

        GLCall(glBindFramebuffer(GL_FRAMEBUFFER, 0));
        renderInfo.frameBuffer->unbind();
    }


#pragma region Registration Methods

    //IRenderer Registration Methods
    void Renderer::registerRenderer(IRenderer2D* renderer) {
        if (!_instance) { return; }
        _instance->_renderers.insert(renderer);
    }

    void Renderer::unregisterRenderer(IRenderer2D* renderer) {
        if (!_instance) { return; }

        const auto find = _instance->_renderers.find(renderer);
        if (find != _instance->_renderers.end()) {
            _instance->_renderers.erase(find);
        }
    }


    //GUI Drawable Registration Methods
    void Renderer::registerGuiDrawable(IImGuiDrawable* drawable) {
        if (!_instance) { return; }
        _instance->_imGuiDrawables.insert(drawable);
    }

    void Renderer::unregisterGuiDrawable(IImGuiDrawable* drawable) {
        if (!_instance) { return; }

        const auto find = _instance->_imGuiDrawables.find(drawable);
        if (find != _instance->_imGuiDrawables.end()) {
            _instance->_imGuiDrawables.erase(find);
        }
    }


    //Camera Registration Methods
    void Renderer::registerCamera(ICamera* camera) {
        if (!_instance) { return; }

        const auto find = _instance->_camerasToDraw.find(camera);
        if (find == _instance->_camerasToDraw.end()) {
            _instance->_camerasToDraw.insert(std::make_pair(camera, RenderInfoGroup()));
        }
    }

    void Renderer::unregisterCamera(ICamera* camera) {
        if (!_instance) { return; }

        const auto find = _instance->_camerasToDraw.find(camera);
        if (find != _instance->_camerasToDraw.end()) {
            _instance->_camerasToDraw.erase(find);
        }
    }
#pragma endregion
}