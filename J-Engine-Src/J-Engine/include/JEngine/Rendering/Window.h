#pragma once
#include <JEngine/Math/Units/JVector.h>
#include <JEngine/Math/Units/JMatrix.h>
#include <JEngine/Math/Units/JRect.h>
#include <JEngine/Utility/Action.h>
#include <JEngine/Math/Graphics/JColor32.h>
#include <JEngine/Rendering/FrameBuffer.h>
#include <GL/glew.h>

struct GLFWwindow;
namespace JEngine { 
    class Window {
    public:
        typedef Action<const int32_t, const int32_t, const double, const bool> ResizeEvent;

        Window();
        ~Window();

        const bool init(const char* title, const int32_t width, const int32_t height);
        const bool tick();

        const size_t getWidth() const;
        const size_t getHeight() const;

        const JRectf& getWorldRect() const;
        const JMatrix& getWorldProjectionMatrix() const;
        const JMatrix& getScreenProjectionMatrix() const;
        const bool isInitialized() const;

        void clear(const JColor32& clearColor, const uint32_t clearFlags = GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        void finalizeFrame();
        void pollEvents();

        void close();

        FrameBuffer& getScreenBuffer() { return _screenBuffer; }
        const FrameBuffer& getScreenBuffer() const { return _screenBuffer; }

        GLFWwindow* getWindowPtr();
        const GLFWwindow* getWindowPtr() const;

        void resetViewport();
        void updateViewport(const JVector2i& viewRect, const JVector2i& viewSize, const uint8_t flags = 0x3);

        const uint64_t addOnWindowResizeCB(const ResizeEvent::Func& cb);
        const bool removeOnWindowResizeCB(const uint64_t id);

    private:
        static std::unordered_map<GLFWwindow*, Window*> _glWindowToWindow;

        JRectf _worldRect;
        JMatrix _worldProjection;
        JMatrix _screenProjection;
        GLFWwindow* _window;
        JVector2i _size;
        ResizeEvent _onResize;
        FrameBuffer _screenBuffer;

        void onWindowResize(const int32_t width, const int32_t height);

        static void windowResizeCallback(GLFWwindow* window, const int32_t width, const int32_t height);
    };
}