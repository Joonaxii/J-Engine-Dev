#include <JEngine/Rendering/Window.h>
#include <JEngine/Rendering/RenderingConsts.h>
#include <JEngine/Rendering/OpenGL/GLHelpers.h>
#include <JEngine/Math/Math.h>

namespace JEngine {
    std::map<GLFWwindow*, Window*> Window::_glWindowToWindow{};
    Window* Window::_instance{ nullptr };

    Window* Window::setInstance(Window* window) {
        return _instance = window;
    }

    Window* Window::getInstance() {
        return _instance;
    }

    Window::Window() : _screenBuffer(GL_RGBA32F), _flags(JWIN_FL_MINIMIZED), _swapInterval(DEFAULT_SWAP_INTERVAL), _window(nullptr), _size(), _worldRect(), _worldProjection(), _onResize(), _onFocus(), _onMinimize() { }
    Window::~Window() {
        close();
    }

    bool Window::init(const char* title, int32_t width, int32_t height, uint8_t swapInterval) {
        if (_window) { return true; }

        //Init GLFW window
        glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
        glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
        glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
        glfwWindowHint(GLFW_RESIZABLE, GL_TRUE);

        _window = glfwCreateWindow(width, height, title, NULL, NULL);
        if (!_window) {
            JE_CORE_ERROR("Failed to initialize GLFW Window!");
            return false;
        }

        glfwMakeContextCurrent(_window);

        //Init Glew
        if (glewInit() != GLEW_OK) {
            JE_CORE_ERROR("Failed to initialize GLEW!");
            glfwTerminate();
            return false;
        }

        setSwapInterval(_swapInterval);

        //Init window resize event update set screen size related values
        _glWindowToWindow.insert(std::make_pair(_window, this));
        onWindowResize(width, height);
        onWindowFocus(glfwGetWindowAttrib(_window, GLFW_FOCUSED));

        glfwSetWindowSizeCallback(_window, windowResizeCallback);
        glfwSetWindowFocusCallback(_window, windowFocusCallback);
        JE_CORE_INFO("[J-Engine - Window] Init:\n  - Width: {0}\n  - Height: {1}\n  - Swap Interval: {2}", _size.x, _size.y, uint32_t(_swapInterval));
        return true;
    }

    bool Window::tick() {
        GLCall(if (_window == NULL || glfwWindowShouldClose(_window)) { return false; });
        return true;
    }

    size_t Window::getWidth() const { return _size.x; }
    size_t Window::getHeight() const { return _size.y; }

    const JRectf& Window::getWorldRect() const { return _worldRect; }

    const JMatrix4f& Window::getWorldProjectionMatrix() const { return _worldProjection; }
    const JMatrix4f& Window::getScreenProjectionMatrix() const { return _screenProjection; }

    bool Window::isInitialized() const { return _window != nullptr; }

    void Window::clear(const JColor32& clearColor, uint32_t clearFlags) {
        GLCall(glClearColor(
            Math::normalize<uint8_t, GLclampf>(clearColor.r),
            Math::normalize<uint8_t, GLclampf>(clearColor.g),
            Math::normalize<uint8_t, GLclampf>(clearColor.b),
            Math::normalize<uint8_t, GLclampf>(clearColor.a)));
        GLCall(glClear(clearFlags));
    }

    void Window::finalizeFrame() { GLCall(glfwSwapBuffers(_window)); }

    void Window::pollEvents() {
        if (!_window) { return; }
        GLCall(glfwPollEvents());
    }

    void Window::setSwapInterval(uint8_t swapInterval) {
        if (!_window) { return; }
        _swapInterval = swapInterval;
        glfwSwapInterval(_swapInterval);
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

    GLFWwindow* Window::getWindowPtr() { return _window; }
    const GLFWwindow* Window::getWindowPtr() const { return _window; }

    void Window::resetViewport() {
        glViewport(0, 0, _size.x, _size.y);
        glScissor(0, 0, _size.x, _size.y);
    }

    void Window::updateViewport(const JVector2i& viewRect, const JVector2i& viewSize, uint8_t flags) {
        if (flags & 0x1) {
            glViewport(viewRect.x, viewRect.y, viewSize.x, viewSize.y);
        }
        if (flags & 0x2) {
            glScissor(viewRect.x, viewRect.y, viewSize.x, viewSize.y);
        }
    }

    uint64_t Window::addOnWindowResizeCB(const ResizeEvent::Func& cb) {
        return _onResize.add(cb);
    }

    bool Window::removeOnWindowResizeCB(uint64_t id) {
        return _onResize.remove(id);
    }

    uint64_t Window::addOnWindowFocusCB(const FocusEvent::Func& cb) {
        return _onFocus.add(cb);
    }

    bool Window::removeOnWindowFocusCB(uint64_t id) {
        return _onFocus.remove(id);
    }

    bool Window::isOSWindowFocused(void* window) {
        return glfwGetWindowAttrib((GLFWwindow*)(window), GLFW_FOCUSED) != 0;
    }

    uint64_t Window::addOnWindowMinimizeCB(const MinimizeEvent::Func& cb) {
        return _onMinimize.add(cb);
    }

    bool Window::removeOnWindowMinimizeCB(uint64_t id) {
        return _onMinimize.remove(id);
    }

    void Window::onWindowResize(int32_t width, int32_t height) {
        bool wasMinimized = _flags.isBitSet(JWIN_FL_MINIMIZED);
        _flags.setBit(JWIN_FL_MINIMIZED, width <= 0 || height <= 0);
        if (_flags.isBitSet(JWIN_FL_MINIMIZED)) {
            if (!wasMinimized) {
                _onMinimize(true);
            }
            return;
        }

        if (width != _size.x || height != _size.y) {
            _size.x = width;
            _size.y = height;

            const float aspect = _size.x / float(_size.y);
            static constexpr float orthoSize = 0.5f;

            const float w = orthoSize * aspect;
            _worldRect = JRectf({ -0.5f * aspect, -0.5f }, { 0.5f * aspect, 0.5f });
            _worldProjection = JMatrix4f::ortho(-w, w, -orthoSize, orthoSize);

            _screenProjection = _worldProjection;
            _screenProjection.translate(-w, -orthoSize);
            _screenProjection.scale(aspect, 1.0f);

            FrameBufferSpecs specs{};
            specs.width = width;
            specs.height = height;

            _screenBuffer.invalidate(specs);
            _onResize(width, height, -1, false);
        }

        if (wasMinimized) {
            _onMinimize(false);
        }
    }

    void Window::windowResizeCallback(GLFWwindow* window, int32_t width, int32_t height) {
        if (window) {
            auto win = _glWindowToWindow[window];
            if (win) {
                win->onWindowResize(width, height);
            }
        }
    }

    void Window::onWindowFocus(int32_t focus) {
        _flags.setBit(JWIN_FL_FOCUSED, focus != 0);
        _onFocus(focus != 0);
    }

    void Window::windowFocusCallback(GLFWwindow* window, int32_t focused) {
        if (window) {
            auto win = _glWindowToWindow[window];
            if (win) {
                win->onWindowFocus(focused);
            }
        }
    }
}