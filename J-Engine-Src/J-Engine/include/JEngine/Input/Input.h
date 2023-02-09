#pragma once
#include <JEngine/Math/Units/JVector.h>
#include <JEngine/Helpers/EnumMacros.h>
#include <JEngine/Math/Math.h>
#include <JEngine/IO/Serialization/Serializable.h>

#ifdef _WIN32_WINDOWS
#undef _WIN32_WINDOWS
#endif
#ifdef _WIN32_WINNT
#undef _WIN32_WINNT
#endif

#define _WIN32_WINDOWS 0x0501
#define _WIN32_WINNT   0x0501

#include <windows.h>
#include <Xinput.h>
#include <JEngine/Utility/Action.h>

namespace JEngine {
    class Input {
    private:
        struct XInputBatteryInfo {
            uint8_t type;
            uint8_t level;
        };

    public:
        struct MouseData {
            JVector2f position;
            JVector2f scroll;

            MouseData operator+(const MouseData& other) const {
                return { position + other.position, scroll + other.scroll };
            }

            MouseData operator-(const MouseData& other) const {
                return { position - other.position, scroll - other.scroll };
            }

            MouseData& operator+=(const MouseData& other) {
                position += other.position;
                scroll += other.scroll;
                return *this;
            }

            MouseData& operator-=(const MouseData& other) {
                position -= other.position;
                scroll -= other.scroll;
                return *this;
            }
        };


        enum DeviceIndex : uint8_t {
            DEV_None = 0,

            DEV_Keyboard = 0x1,

            DEV_One = 0x2,
            DEV_Two = 0x4,
            DEV_Three = 0x8,
            DEV_Four = 0x10,

            DEV_Controller = DEV_One | DEV_Three | DEV_Four,
            DEV_Any        = DEV_Keyboard | DEV_One | DEV_Three | DEV_Four
        };

        enum InputCode : uint16_t {
            INP_None = 0,

            INP_Mouse0 = 0x1,
            INP_Mouse1 = 0x2,
            INP_Mouse3 = 0x3,
            INP_Mouse4 = 0x4,
            INP_Mouse5 = 0x5,

            INP_Backspace = 0x8,
            INP_Tab = 0x9,

            INP_Shift = 0x10,
            INP_Ctrl = 0x11,
            INP_Alt = 0x12,

            INP_Clear = 0xC,
            INP_Enter = 0xD,

            INP_Pause = 0x13,
            INP_CapsLock = 0x14,

            INP_Excape = 0x1B,
            INP_Space = 0x20,

            INP_PageUp = 0x21,
            INP_PadeDown = 0x22,
            INP_End = 0x23,
            INP_Home = 0x24,

            INP_Left = 0x25,
            INP_Up = 0x26,
            INP_Right = 0x27,
            INP_Down = 0x28,

            INP_Print = 0x2A,
            INP_Insert = 0x2D,
            INP_Delete = 0x2E,

            INP_D0 = 0x30,
            INP_D1 = 0x31,
            INP_D2 = 0x32,
            INP_D3 = 0x33,
            INP_D4 = 0x34,
            INP_D5 = 0x35,
            INP_D6 = 0x36,
            INP_D7 = 0x37,
            INP_D8 = 0x38,
            INP_D9 = 0x39,

            INP_A = 'A',
            INP_B = 'B',
            INP_C = 'C',
            INP_D = 'D',
            INP_E = 'E',
            INP_F = 'F',
            INP_G = 'G',
            INP_H = 'H',
            INP_I = 'I',
            INP_J = 'J',
            INP_K = 'K',
            INP_L = 'L',
            INP_M = 'M',
            INP_N = 'N',
            INP_O = 'O',
            INP_P = 'P',
            INP_Q = 'Q',
            INP_R = 'R',
            INP_S = 'S',
            INP_T = 'T',
            INP_U = 'U',
            INP_V = 'V',
            INP_W = 'W',
            INP_X = 'X',
            INP_Y = 'Y',
            INP_Z = 'Z',

            INP_LWindows = 0x5B,
            INP_RWindows = 0x5C,
            INP_Apps = 0x5D,

            INP_Num0 = 0x60,
            INP_Num1 = 0x61,
            INP_Num2 = 0x62,
            INP_Num3 = 0x63,
            INP_Num4 = 0x64,
            INP_Num5 = 0x65,
            INP_Num6 = 0x66,
            INP_Num7 = 0x67,
            INP_Num8 = 0x68,
            INP_Num9 = 0x69,

            INP_NumMultiply = 0x6A,
            INP_NumPlus = 0x6B,
            INP_NumSeparator = 0x6C,
            INP_NumSubtract = 0x6D,
            INP_NumDecimal = 0x6E,
            INP_NumDivide = 0x6F,

            INP_F1 = 0x70,
            INP_F2 = 0x71,
            INP_F3 = 0x72,
            INP_F4 = 0x73,
            INP_F5 = 0x74,
            INP_F6 = 0x75,
            INP_F7 = 0x76,
            INP_F8 = 0x77,
            INP_F9 = 0x78,
            INP_F10 = 0x79,
            INP_F11 = 0x7A,
            INP_F12 = 0x7B,
            INP_F13 = 0x7C,
            INP_F14 = 0x7D,
            INP_F15 = 0x7E,
            INP_F16 = 0x7F,
            INP_F17 = 0x80,
            INP_F18 = 0x81,
            INP_F19 = 0x82,
            INP_F20 = 0x83,
            INP_F21 = 0x84,
            INP_F22 = 0x85,
            INP_F23 = 0x86,
            INP_F24 = 0x87,

            INP_NumLock = 0x90,
            INP_ScrollLock = 0x91,

            INP_LShift = 0xA0,
            INP_RShift = 0xA1,
            INP_LCtrl = 0xA2,
            INP_RCtrl = 0xA3,

            INP_LAlt = 0xA4,
            INP_RAlt = 0xA5,

            INP_SemiColon = 0xBA,
            INP_Plus = 0xBB,
            INP_Comma = 0xBC,
            INP_Minus = 0xBD,
            INP_Period = 0xBE,
            INP_Question = 0xBF,
            INP_Tilde = 0xC0,

            INP_LBracket = 0xDB,
            INP_FSlash = 0xDC,
            INP_RBracket = 0xDD,
            INP_Quote = 0xDE,

            //K-Axes
            INP_MDeltaX = 0x100,
            INP_MDeltaY = 0x101,

            INP_MScrollX = 0x102,
            INP_MScrollY = 0x103,

            //K-Axis Buttons
            INP_MouseUp = 0x104,
            INP_MouseLeft = 0x105,
            INP_MouseDown = 0x106,
            INP_MouseRight = 0x107,
           
            INP_ScrollUp = 0x108,
            INP_ScrollLeft = 0x109,
            INP_ScrollDown = 0x10A,
            INP_ScrollRight = 0x10B,

            //C-Buttons

            INP_DPadUp,
            INP_DPadDown,
            INP_DPadLeft,
            INP_DPadRight,

            INP_StartButton,
            INP_BackButton,

            INP_LStick,
            INP_RStick,

            INP_LShoulder,
            INP_RShoulder,

            INP_GuideButton,

            INP_CButton0 = 280,
            INP_CButton1,
            INP_CButton2,
            INP_CButton3,

            //C-Axes
            INP_TriggerL,
            INP_TriggerR,

            INP_StickLX,
            INP_StickLY,

            INP_StickRX,
            INP_StickRY,

            //C-Axis Buttons
            INP_PressTriggerL,
            INP_PressTriggerR,

            INP_StickLUp,
            INP_StickLLeft,
            INP_StickLDown,
            INP_StickLRight,

            INP_StickRUp,
            INP_StickRLeft,
            INP_StickRDown,
            INP_StickRRight,

            INP_KeyCount,
        };
 

        struct BatteryInfo {
        public:
            enum BatteryLevel : uint8_t {
                BATTERY_LVL_EMPTY,
                BATTERY_LVL_LOW,
                BATTERY_LVL_MEDIUM,
                BATTERY_LVL_FULL,
            };

            enum BatteryType : uint8_t {
                BATTERY_TYPE_DISCONNECTED,
                BATTERY_TYPE_WIRED,
                BATTERY_TYPE_ALKALINE,
                BATTERY_TYPE_NIMH,
                BATTERY_TYPE_UNKNOWN,
            };

            BatteryLevel level;
            BatteryType type;
        };

        struct Gamepad {
        public:
            uint16_t getButtons() const { return _buttons; }

            bool isConnected() const { return _isConnected; }

            bool isDown(const int16_t index) const {
                if (index < 0 || index >= 0x8000) { return false; }
                return bool(_buttons & index);
            }

            bool isDown(const InputCode code) const {
                return isDown(int16_t(code - KEYBOARD_INPUTS));
            }

            float getTriggerL() const { return _trigger.x; }
            float getTriggerR() const { return _trigger.y; }

            const JVector2f& getStickL() const { return _stickL; }
            const JVector2f& getStickR() const { return _stickR; }

            void update(const bool isConnected, const XINPUT_GAMEPAD& nativePad) {
                _isConnected = isConnected;
                
                _buttons = nativePad.wButtons;

                _trigger.x = float(Math::normalize(nativePad.bLeftTrigger));
                _trigger.y = float(Math::normalize(nativePad.bRightTrigger));

                _stickL.x = float(Math::normalize(nativePad.sThumbLX));
                _stickL.y = float(Math::normalize(nativePad.sThumbLY));

                _stickR.x = float(Math::normalize(nativePad.sThumbRX));
                _stickR.y = float(Math::normalize(nativePad.sThumbRY));
            }

        private:
            bool _isConnected;

            uint16_t _buttons;

            JVector2f _trigger;

            JVector2f _stickL;
            JVector2f _stickR;
        };

        struct InputResult {
            InputCode code;
            DeviceIndex device;
            uint64_t tick;
        };

        static constexpr float DEFAULT_DEAD_ZONE = 0.105f;
        static constexpr int32_t MAX_CONTROLLERS = 4;
        static constexpr int32_t KEYBOARD_INPUTS = 0x10C;
        static constexpr int32_t CONTROLLER_INPUTS = InputCode::INP_KeyCount - KEYBOARD_INPUTS;

        struct InputState {
        public:
            InputState() : _flags(0), _tick(0), _axisVal(0) { }

            void reset() {
                _flags = 0;
                _tick = 0;
                _axisVal = 0;
            }

            float getAxisValue() const { return _axisVal; }
            uint64_t getTickDown() const { return _tick; }

            inline bool isDown()     const { return bool(_flags & KEY_DOWN); }
            inline bool isHeld()     const { return bool(_flags & KEY_HELD); }
            inline bool isUp()       const { return bool(_flags & KEY_UP); }
            inline bool isToggled()  const { return bool(_flags & KEY_TOGGLE); }

            void update(const bool state, const bool toggled, const float value, const uint64_t tick, const bool autoToggle = false) {
                _axisVal = value;

                const bool down = isDown();
                const bool held = isHeld();
 
                _flags = 0;

                if (down && !state) { _flags |= KEY_UP; }
                if (!held && state)
                {
                    _flags |= KEY_DOWN;
                    _tick = tick;
                }

                if (autoToggle && (((_flags & (KEY_DOWN | KEY_TOGGLE)) == KEY_DOWN))) {
                    _flags |= KEY_TOGGLE;
                }
                else if (toggled) {
                    _flags |= KEY_TOGGLE;
                }

                if (state) { _flags |= KEY_HELD; }
            }

        private:
            uint8_t _flags;
            uint64_t _tick;
            float _axisVal;
        };

        static void init();

        static void clear();

        static void update(const bool hasFocus);
        static void update(const MouseData& mouseData, const bool hasFocus);

        static InputState getInputState(const DeviceIndex device, const InputCode code);

        static const JVector2f& getMousePosition();
        static const JVector2f& getMouseWheel();

        static const JVector2f& getMousePositionDelta();
        static const JVector2f& getMouseWheelDelta();

        static uint64_t addDeviceConnectionCB(const Action<const DeviceIndex, const bool>::Func& func);
        static void removeDeviceConnectionCB(const uint64_t id);

        static Gamepad getGamepad(const DeviceIndex device);
        static Gamepad getGamepad(const int32_t device);
        static void getGamepads(const DeviceIndex devices, Gamepad* pads);

        static void setDeadZone(const DeviceIndex devices, const float deadZone);
        static void setDeadZones(const DeviceIndex devices, const float* deadZones);

        static float getDeadZone(const DeviceIndex device);
        static void getDeadZones(const DeviceIndex devices, float* deadZones);

        static void vibrateDevices(const DeviceIndex devices, const double left, const double right);
        static void vibrateDevices(const DeviceIndex devices, const uint16_t left, const uint16_t right);

        static void vibrateDevice(const int32_t device, const double left, const double right);
        static void vibrateDevice(const int32_t device, const uint16_t left, const uint16_t right);

        static bool isDown(const DeviceIndex index, const InputCode code);
        static bool isHeld(const DeviceIndex index, const InputCode code);
        static bool isUp(const DeviceIndex index, const InputCode code);
        static bool isToggled(const DeviceIndex index, const InputCode code);

        static bool anyDown(const DeviceIndex devices, InputResult& result, const InputCode* ignored = nullptr, const size_t ignoredLength = 0);
        static bool anyHeld(const DeviceIndex devices, InputResult& result, const InputCode* ignored = nullptr, const size_t ignoredLength = 0);
        static bool anyUp(const DeviceIndex devices, InputResult& result, const InputCode* ignored = nullptr, const size_t ignoredLength = 0);
        static bool anyToggled(const DeviceIndex devices, InputResult& result, const InputCode* ignored = nullptr, const size_t ignoredLength = 0);

        static float getAxis(const DeviceIndex devices, const InputCode neg, const InputCode pos);
        static JVector2f getVector(const DeviceIndex devices,
            const InputCode negX, const InputCode posX,
            const InputCode negY, const InputCode posY);

    private:
        static bool HAS_FOCUS;
        static constexpr uint8_t KEY_DOWN     = 0x1;
        static constexpr uint8_t KEY_HELD     = 0x2;
        static constexpr uint8_t KEY_UP       = 0x4;
        static constexpr uint8_t KEY_TOGGLE   = 0x8;

        static float _deadZones[MAX_CONTROLLERS + 1];
        static InputState _inputs[KEYBOARD_INPUTS + CONTROLLER_INPUTS * MAX_CONTROLLERS];
        static Gamepad _gamePads[MAX_CONTROLLERS];
        static uint64_t _frame;
        static MouseData _mData;
        static MouseData _mDelta;

        static Action<const DeviceIndex, const bool> _onDeviceConnectionChange;

        static bool isKeyDown(const InputCode code);
        static int32_t getInputStates(const InputCode code, const DeviceIndex devices, InputState* states);

        static void updateMouseData(const MouseData& mData);

        static bool any(const int32_t mode, const DeviceIndex devices, InputResult& result, const InputCode* ignored = nullptr, const size_t ignoredLength = 0);
    };

#pragma region Serialization
    template<>
    inline bool Serializable<Input::DeviceIndex>::deserializeJson(Input::DeviceIndex& itemRef, json& jsonF, const Input::DeviceIndex& defaultValue) {
        itemRef = jsonF.is_number_integer() ? Input::DeviceIndex(jsonF.get<uint8_t>()) : defaultValue;
        return true;
    }

    template<>
    inline bool Serializable<Input::DeviceIndex>::serializeJson(const Input::DeviceIndex& itemRef, json& jsonF) {
        jsonF.update(uint8_t(itemRef));
        return true;
    } 
    

    template<>
    inline bool Serializable<Input::InputCode>::deserializeJson(Input::InputCode& itemRef, json& jsonF, const Input::InputCode& defaultValue) {
        itemRef = jsonF.is_number_integer() ? Input::InputCode(jsonF.get<uint16_t>()) : defaultValue;
        return true;
    }

    template<>
    inline bool Serializable<Input::InputCode>::serializeJson(const Input::InputCode& itemRef, json& jsonF) {
        jsonF.update(uint16_t(itemRef));
        return true;
    }
#pragma endregion
}

CREATE_ENUM_OPERATORS_SELF(JEngine::Input::DeviceIndex);