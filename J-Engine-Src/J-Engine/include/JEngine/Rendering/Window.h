#pragma once
#include <JEngine/Math/Units/JVector.h>
#include <JEngine/Math/Units/JMatrix.h>
#include <JEngine/Math/Units/JRect.h>
#include <JEngine/Utility/Action.h>
#include <JEngine/Math/Graphics/JColor32.h>
#include <JEngine/Rendering/FrameBuffer.h>
#include <GL/glew.h>
#include <map>

struct GLFWwindow;
namespace JEngine {

    struct WindowProps {
        std::string title;
        uint32_t width;
        uint32_t height;

        WindowProps(const std::string& title = "J-Engine",
            const uint32_t width = 1600,
            const uint32_t height = 900) : title(title), width(width), height(height) {}
    };

    class Window {
    public:
        typedef Action<const int32_t, const int32_t, const double, const bool> ResizeEvent;

        Window();
        virtual ~Window();

        bool isInitialized() const;

        bool init(const char* title, const int32_t width, const int32_t height);
        bool tick();

        size_t getWidth() const;
        size_t getHeight() const;

        const JRectf& getWorldRect() const;
        const JMatrix4f& getWorldProjectionMatrix() const;
        const JMatrix4f& getScreenProjectionMatrix() const;

        void clear(const JColor32& clearColor, const uint32_t clearFlags = GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        void finalizeFrame();
        void pollEvents();

        void close();

        static Window* setInstance(Window* window);
        static Window* getInstance();
    
        FrameBuffer& getScreenBuffer() { return _screenBuffer; }
        const FrameBuffer& getScreenBuffer() const { return _screenBuffer; }

        GLFWwindow* getWindowPtr();
        const GLFWwindow* getWindowPtr() const;

        bool isMinimized() const { return _minimized; };

        void resetViewport();
        void updateViewport(const JVector2i& viewRect, const JVector2i& viewSize, const uint8_t flags = 0x3);

        uint64_t addOnWindowResizeCB(const ResizeEvent::Func& cb);
        bool removeOnWindowResizeCB(const uint64_t id);

    private:
        static std::map<GLFWwindow*, Window*> _glWindowToWindow;
        static Window* _instance;

        JRectf _worldRect;
        JMatrix4f _worldProjection;
        JMatrix4f _screenProjection;
        GLFWwindow* _window;
        JVector2i _size;
        ResizeEvent _onResize;
        FrameBuffer _screenBuffer;
        bool _minimized;

        void onWindowResize(const int32_t width, const int32_t height);
        static void windowResizeCallback(GLFWwindow* window, const int32_t width, const int32_t height);
    };
}