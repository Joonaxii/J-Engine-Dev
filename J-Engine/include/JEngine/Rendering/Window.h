#pragma once
#include <JEngine/Math/Units/JVector.h>
#include <JEngine/Math/Units/JMatrix.h>
#include <JEngine/Math/Units/JRect.h>
#include <JEngine/Utility/Flags.h>
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
        static constexpr uint8_t DEFAULT_SWAP_INTERVAL = 0x1;

        typedef Action<const int32_t, const int32_t, const double, const bool> ResizeEvent;
        typedef Action<const bool> FocusEvent;
        typedef Action<const bool> MinimizeEvent;

        Window();
        virtual ~Window();

        bool isInitialized() const;

        bool init(const char* title, int32_t width, int32_t height, uint8_t swapInterval = DEFAULT_SWAP_INTERVAL);
        bool tick();

        size_t getWidth() const;
        size_t getHeight() const;

        const JRectf& getWorldRect() const;
        const JMatrix4f& getWorldProjectionMatrix() const;
        const JMatrix4f& getScreenProjectionMatrix() const;

        void clear(const JColor32& clearColor, uint32_t clearFlags = GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        void finalizeFrame();
        void pollEvents();

        void close();

        static Window* setInstance(Window* window);
        static Window* getInstance();
    
        FrameBuffer& getScreenBuffer() { return _screenBuffer; }
        const FrameBuffer& getScreenBuffer() const { return _screenBuffer; }

        GLFWwindow* getWindowPtr();
        const GLFWwindow* getWindowPtr() const;

        bool isMinimized() const { return _flags.isBitSet(JWIN_FL_MINIMIZED); };
        bool isFocused() const { return _flags.isBitSet(JWIN_FL_FOCUSED); };

        void setSwapInterval(uint8_t swapInterval);
        uint8_t getSwapInterval() const { return _swapInterval; }

        void resetViewport();
        void updateViewport(const JVector2i& viewRect, const JVector2i& viewSize, uint8_t flags = 0x3);

        uint64_t addOnWindowResizeCB(const ResizeEvent::Func& cb);
        bool removeOnWindowResizeCB(uint64_t id);
        
        uint64_t addOnWindowFocusCB(const FocusEvent::Func& cb);
        bool removeOnWindowFocusCB(uint64_t id);

        uint64_t addOnWindowMinimizeCB(const MinimizeEvent::Func& cb);
        bool removeOnWindowMinimizeCB(uint64_t id);

        static bool isOSWindowFocused(void* window);

    private:
        static std::map<GLFWwindow*, Window*> _glWindowToWindow;
        static Window* _instance;

        enum : uint8_t {
            JWIN_FL_MINIMIZED = 0x1,
            JWIN_FL_FOCUSED = 0x2,
        };
        MinimizeEvent _onMinimize;
        ResizeEvent _onResize;
        FocusEvent _onFocus;

        JRectf _worldRect;
        JMatrix4f _worldProjection;
        JMatrix4f _screenProjection;
        GLFWwindow* _window;
        JVector2i _size;
        FrameBuffer _screenBuffer;

        uint8_t _swapInterval; 
        UI8Flags _flags;

        void onWindowResize(int32_t width, int32_t height);
        static void windowResizeCallback(GLFWwindow* window, int32_t width, int32_t height);

        void onWindowFocus(int32_t focused);
        static void windowFocusCallback(GLFWwindow* window, int32_t focused);
    };
}