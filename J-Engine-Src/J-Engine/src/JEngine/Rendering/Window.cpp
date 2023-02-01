#include <JEngine/Rendering/Window.h>
#include <JEngine/Rendering/RenderingConsts.h>
#include <JEngine/Rendering/OpenGL/GLHelpers.h>
#include <JEngine/Math/Math.h>
#include <unordered_map>

namespace JEngine {
    std::unordered_map<GLFWwindow*, Window*> Window::_glWindowToWindow{};

    void onMaximize(GLFWwindow* window, int maximized) {

    }

    Window::Window() : _screenBuffer(GL_RGBA32F), _window(nullptr), _size(), _worldRect(), _worldProjection(), _onResize() { }
    Window::~Window() {
        close();
    }

    const bool Window::init(const char* title, const int32_t width, const int32_t height) {
        if (_window != NULL) { return true; }

        glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
        glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
        glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
        glfwWindowHint(GLFW_RESIZABLE, GL_TRUE);

        _window = glfwCreateWindow(width, height, title, NULL, NULL);
        if (_window == NULL) {
            std::cout << "Failed to initialize GLFW Window!" << std::endl;
            return false;
        }

        glfwSetWindowMaximizeCallback(_window, onMaximize);
        glfwMakeContextCurrent(_window);

        _size.x = width;
        _size.y = height;

        _glWindowToWindow.insert(std::make_pair(_window, this));


        //Init Glew
        if (glewInit() != GLEW_OK) {
            std::cout << "Failed to initialize GLEW!" << std::endl;
            glfwTerminate();
            return false;
        }

        onWindowResize(width, height);
        glfwSetWindowSizeCallback(_window, windowResizeCallback);
        return true;
    }

    const bool Window::tick() {
        GLCall(if (_window == NULL || glfwWindowShouldClose(_window)) { return false; });
        return true;
    }

    const size_t Window::getWidth() const {
        return _size.x;
    }

    const size_t Window::getHeight() const {
        return _size.y;
    }

    const JRectf& Window::getWorldRect() const {
        return _worldRect;
    }

    const JMatrix4f& Window::getWorldProjectionMatrix() const {
        return _worldProjection;
    }
        
    const JMatrix4f& Window::getScreenProjectionMatrix() const {
        return _screenProjection;
    }

    const bool Window::isInitialized() const {
        return _window != nullptr;
    }

    void Window::clear(const JColor32& clearColor, const uint32_t clearFlags) {
        GLCall(glClearColor(
            Math::normalize<uint8_t, GLclampf>(clearColor.r),
            Math::normalize<uint8_t, GLclampf>(clearColor.g),
            Math::normalize<uint8_t, GLclampf>(clearColor.b),
            Math::normalize<uint8_t, GLclampf>(clearColor.a)));
        GLCall(glClear(clearFlags));
    }

    void Window::finalizeFrame() {
        GLCall(glfwSwapBuffers(_window));
    }

    void Window::pollEvents() {
        if (!_window) { return; }
        GLCall(glfwPollEvents());
    }

    void Window::close() {
        if (_window == NULL) { return; }

        _screenBuffer.release();
        _screenBuffer.releaseTexture();
        _screenBuffer.releaseDepthBuffer();

        _glWindowToWindow.erase(_window);

        GLCall(glfwSetWindowSizeCallback(_window, NULL));
        glfwDestroyWindow(_window);
        _window = NULL;
    }

    GLFWwindow* Window::getWindowPtr() {
        return _window;
    }

    const GLFWwindow* Window::getWindowPtr() const {
        return _window;
    }

    void Window::resetViewport() {
        glViewport(0, 0, _size.x, _size.y);
        glScissor(0, 0, _size.x, _size.y);
    }

    void Window::updateViewport(const JVector2i& viewRect, const JVector2i& viewSize, const uint8_t flags) {
        if (flags & 0x1) {
            glViewport(viewRect.x, viewRect.y, viewSize.x, viewSize.y);
        }
        if (flags & 0x2) {
            glScissor(viewRect.x, viewRect.y, viewSize.x, viewSize.y);
        }
    }

    const uint64_t Window::addOnWindowResizeCB(const ResizeEvent::Func& cb) {
        return _onResize.add(const_cast<ResizeEvent::Func&>(cb));
    }

    const bool Window::removeOnWindowResizeCB(const uint64_t id) {
        return _onResize.remove(id);
    }

    void Window::onWindowResize(const int32_t width, const int32_t height) {
        _size.x = width;
        _size.y = height;

        const float aspect = _size.x / float(_size.y);
        static constexpr float orthoSize = 0.5f;

        const float w = orthoSize * aspect;
        _worldRect       = JRectf({ -0.5f * aspect, -0.5f }, { 0.5f * aspect, 0.5f });
        _worldProjection = JMatrix4f::ortho(-w, w, -orthoSize, orthoSize);

        _screenProjection = _worldProjection;
        _screenProjection.translate(-w, -orthoSize);
        _screenProjection.scale(aspect, 1.0f);

        FrameBufferSpecs specs;
        specs.width = width;
        specs.height = height;

        _screenBuffer.invalidate(specs);
        _onResize(width, height, -1, false);
    }

    void Window::windowResizeCallback(GLFWwindow* window, const int32_t width, const int32_t height) {
        if (window) {
            auto win = _glWindowToWindow[window];
            if (win) {
                win->onWindowResize(width, height);
            }
        }
    }
}