#include <JEngine/Input/Input.h>
#include <iostream>
#include <WinUser.h>

namespace JEngine {

    static bool isKeyDown(Input::InputCode code) {
        return (GetAsyncKeyState(code) & 0x8000) >> 15U;
    }

    static void updateControllerState(Input::Gamepad& pad, Input::InputState* states, float deadZone, uint64_t tick) {
        auto ogStates = states;
        bool state;
        uint8_t* padB = reinterpret_cast<uint8_t*>(&pad);

        //Buttons
        for (size_t i = 0, j = 1; i < 16; i++, states++, j <<= 1) {
            state = bool(pad.getButtons() & j);
            states->update(state, false, state ? 1.0f : 0.0f, tick, true);
        }

        float* padF = reinterpret_cast<float*>(padB + 2);

        //Triggers
        for (size_t i = 0; i < 2; i++, states++) {
            const float val = *padF++;
            state = val > deadZone;

            states->update(state, false, val, tick, true);
            (states + 6)->update(state, false, state ? 1.0f : 0.0f, tick, true);
        }

        //Sticks
        const JVector2f* padV2f = reinterpret_cast<JVector2f*>(padB + 10);

        int32_t stateA;
        int32_t stateB;
        auto stickBtn = states + 6;
        for (size_t i = 0, j = 0; i < 2; i++, states += 2, stickBtn += 4, j += 4) {
            const auto val = *padV2f++;

            stateA = Math::sign<float, int32_t>(val.x, deadZone);
            stateB = Math::sign<float, int32_t>(val.y, deadZone);

            //Stick XY
            states->update(stateA, false, stateA ? val.x : 0, tick, true);
            (states + 1)->update(stateB, false, stateB ? val.y : 0, tick, true);

            //Axis Buttons U->L->D->R

            state = stateB == 1;
            stickBtn->update(state, false, state ? val.y : 0, tick, true);

            state = stateA == -1;
            (stickBtn + 1)->update(state, false, state ? -val.x : 0, tick, true);

            state = stateB == -1;
            (stickBtn + 2)->update(state, false, state ? -val.y : 0, tick, true);

            state = stateA == 1;
            (stickBtn + 3)->update(state, false, state ? val.x : 0, tick, true);
        }
    }

    Input::Input() : HAS_FOCUS(false), _deadZones{ 0 }, _inputs{}, _gamePads{}, _frame{}, _mData{}, _mDelta{}, _onDeviceConnectionChange{} {}

    void Input::INTERNAL_init() {
        memset(_inputs, 0, sizeof(_inputs));
        std::fill_n(_deadZones, MAX_CONTROLLERS + 1, Input::DEFAULT_DEAD_ZONE);
    }

    void Input::INTERNAL_clear() {
        _mData = {};
        _mDelta = {};

        for (int32_t i = 0; i < KEYBOARD_INPUTS + CONTROLLER_INPUTS * MAX_CONTROLLERS; i++) {
            _inputs[i].reset();
        }
    }

    const JVector2f& Input::INTERNAL_getMousePosition() const { return _mData.position; }
    const JVector2f& Input::INTERNAL_getMousePositionDelta() const { return _mDelta.position; }

    const JVector2f& Input::INTERNAL_getMouseWheel() const { return _mData.scroll; }
    const JVector2f& Input::INTERNAL_getMouseWheelDelta() const { return _mDelta.scroll; }

    uint64_t Input::INTERNAL_addDeviceConnectionCB(const Action<DeviceIndex, bool>::Func& func) {
        return _onDeviceConnectionChange.add(func);
    }
    void Input::INTERNAL_removeDeviceConnectionCB(uint64_t id) {
        _onDeviceConnectionChange.remove(id);
    }

    void Input::INTERNAL_update(bool hasFocus) {
        MouseData mDat;
        INTERNAL_update(mDat, hasFocus);
    }

    void Input::INTERNAL_update(const MouseData& mouseData, bool hasFocus) {
        HAS_FOCUS = hasFocus;
        if (!hasFocus) { return; }

        auto prevM = _mData;
        _mData = mouseData;
        _mDelta = _mData - prevM;
        isKeyDown(InputCode::INP_A);
        uint8_t keyStates[256]{ 0 };
        auto ret = GetKeyboardState(keyStates);

        bool state;
        for (int i = 0; i < 256; i++) {
            const auto b = keyStates[i];
            state = (b & 0x80) != 0;
            _inputs[i].update(state, bool(b & 0x1), state ? 1.0f : 0.0f, _frame);
        }

        updateMouseData(_mDelta);

        XINPUT_STATE xPad;
        for (DWORD i = 0, j = KEYBOARD_INPUTS; i < MAX_CONTROLLERS; i++, j += CONTROLLER_INPUTS) {
            auto ret = XInputGetState(i, &xPad);
            bool isConnected = ret == ERROR_SUCCESS;
            bool curConnected = _gamePads[i].isConnected();

            _gamePads[i].update(isConnected, xPad.Gamepad);

            if (isConnected != curConnected) {
                _onDeviceConnectionChange(DeviceIndex((i + 1) << 1), isConnected);
            }
        }
        _frame++;
    }

    bool Input::INTERNAL_isDown(InputCode code, DeviceIndex index) const {
        if (!HAS_FOCUS) { return false; }

        InputState buffer[MAX_CONTROLLERS + 1];
        const auto states = getInputStates(code, index, buffer);

        for (size_t i = 0; i < states; i++) {
            if (buffer[i].isDown()) { return true; }
        }
        return false;
    }
    bool Input::INTERNAL_isHeld(InputCode code, DeviceIndex index) const {
        if (!HAS_FOCUS) { return false; }

        InputState buffer[MAX_CONTROLLERS + 1];
        const auto states = getInputStates(code, index, buffer);

        for (size_t i = 0; i < states; i++) {
            if (buffer[i].isHeld()) { return true; }
        }
        return false;
    }
    void Input::INTERNAL_setDeadZones(DeviceIndex devices, const float* deadZones) {
        for (size_t i = 0, j = 1, k = 0; i < MAX_CONTROLLERS; i++, j <<= 1) {
            if (j & devices) {
                _deadZones[i] = deadZones[k++];
            }
        }
    }

    void Input::INTERNAL_setDeadZone(DeviceIndex devices, float deadZone) {
        for (size_t i = 0, j = 1; i < MAX_CONTROLLERS; i++, j <<= 1) {
            if (j & devices) {
                _deadZones[i] = deadZone;
            }
        }
    }

    float Input::INTERNAL_getDeadZone(DeviceIndex device) const {
        const int32_t ind = Math::potToIndex(device);
        return ind > -1 ? _deadZones[ind] : 0;
    }

    void Input::INTERNAL_getDeadZones(DeviceIndex devices, float* deadZones) const {
        for (size_t i = 0, k = 0, j = 1; i < MAX_CONTROLLERS; i++, j <<= 1) {
            if (j & devices) {
                deadZones[k++] = _deadZones[i];
            }
        }
    }

    bool Input::INTERNAL_isUp(InputCode code, DeviceIndex index) const {
        if (!HAS_FOCUS) { return false; }

        InputState buffer[MAX_CONTROLLERS + 1];
        const auto states = getInputStates(code, index, buffer);

        for (size_t i = 0; i < states; i++) {
            if (buffer[i].isUp()) { return true; }
        }
        return false;
    }
    bool Input::INTERNAL_isToggled(InputCode code, DeviceIndex index) const {
        InputState buffer[MAX_CONTROLLERS + 1];
        const auto states = getInputStates(code, index, buffer);

        for (size_t i = 0; i < states; i++) {
            if (buffer[i].isToggled()) { return true; }
        }
        return false;
    }

    Input::Gamepad Input::INTERNAL_getGamepad(DeviceIndex device) const {
        const int32_t ind = Math::potToIndex(device);
        return INTERNAL_getGamepad(ind - 1);
    }
    Input::Gamepad Input::INTERNAL_getGamepad(int32_t device) const {
        if (device > -1) { return _gamePads[device]; }
        return {};
    }
    void Input::INTERNAL_getGamepads(DeviceIndex devices, Gamepad* pads) const {
        for (size_t i = 0, j = 2, k = 0; i < MAX_CONTROLLERS; i++, j <<= 1) {
            if (j & devices) {
                pads[k++] = _gamePads[i];
            }
        }
    }

    void Input::INTERNAL_vibrateDevices(DeviceIndex devices, double left, double right) const {
        INTERNAL_vibrateDevices(devices, Math::deNormalize<uint16_t>(left), Math::deNormalize<uint16_t>(right));
    }

    void Input::INTERNAL_vibrateDevices(DeviceIndex devices, uint16_t left, uint16_t right) const {
        for (int32_t i = 0, j = 2; i < MAX_CONTROLLERS; i++, j <<= 1)
        {
            if (j & devices) {
                INTERNAL_vibrateDevice(i, left, right);
            }
        }
    }

    bool Input::INTERNAL_anyDown(DeviceIndex devices, InputResult& result, const InputCode* ignored, size_t ignoredLength) const {
        return any(0, devices, result, ignored, ignoredLength);
    }
    bool Input::INTERNAL_anyHeld(DeviceIndex devices, InputResult& result, const InputCode* ignored, size_t ignoredLength) const {
        return any(1, devices, result, ignored, ignoredLength);
    }
    bool Input::INTERNAL_anyUp(DeviceIndex devices, InputResult& result, const InputCode* ignored, size_t ignoredLength) const {
        return any(2, devices, result, ignored, ignoredLength);
    }
    bool Input::INTERNAL_anyToggled(DeviceIndex devices, InputResult& result, const InputCode* ignored, size_t ignoredLength) const {
        return any(3, devices, result, ignored, ignoredLength);
    }

    float Input::INTERNAL_getAxis(InputCode neg, InputCode pos, DeviceIndex devices) const {
        float axis = 0;

        if (INTERNAL_isHeld(neg, devices)) { axis--; }
        if (INTERNAL_isHeld(pos, devices)) { axis++; }

        return axis;
    }

    JVector2f Input::INTERNAL_getVector(InputCode negX, InputCode posX, InputCode negY, InputCode posY, DeviceIndex devices) const {
        return JVector2f(INTERNAL_getAxis(negX, posX, devices), INTERNAL_getAxis(negY, posY, devices));
    }

    Input::InputState Input::INTERNAL_getInputState(DeviceIndex device, InputCode code) const {
        const int32_t devInd = Math::potToIndex(device);
        if (devInd < 0) { return {}; }
        if (devInd == 0) { return _inputs[code]; }
        return _inputs[KEYBOARD_INPUTS + (devInd - 1) * CONTROLLER_INPUTS + code];
    }

    int32_t Input::getInputStates(InputCode code, DeviceIndex devices, InputState* states) const {
        int32_t kCode = code;

        int32_t sInd = 0;
        int32_t curSize = KEYBOARD_INPUTS;

        for (uint32_t i = 0, j = 1, k = 0; i < MAX_CONTROLLERS + 1; i++, j <<= 1) {
            if ((j & devices) && kCode < curSize) {
                states[sInd++] = _inputs[k + kCode];
            }

            if (i == 0) {
                kCode -= KEYBOARD_INPUTS;
                curSize = CONTROLLER_INPUTS;
                if (kCode < 0) { break; }

                k += KEYBOARD_INPUTS;
                continue;
            }
            k += CONTROLLER_INPUTS;
        }
        return sInd;
    }

    void Input::updateMouseData(const MouseData& mData) {
        InputState* states = _inputs + 256;

        const float* mDataF = reinterpret_cast<const float*>(&mData);
        const float dead = _deadZones[0];
        for (size_t i = 0, j = 0; i < 2; i++, j += 2, states += 2) {

            const float x = mDataF[j];
            const float y = mDataF[j + 1];

            const int32_t stateX = x > dead ? 1 : x < -dead ? -1 : 0;
            const int32_t stateY = y > dead ? 1 : y < -dead ? -1 : 0;

            states[0].update(stateX != 0, false, x, _frame, true);
            states[1].update(stateY != 0, false, y, _frame, true);

            states[4].update(stateY == 1, false, y, _frame, true);
            states[5].update(stateX == -1, false, -x, _frame, true);

            states[6].update(stateY == -1, false, -y, _frame, true);
            states[7].update(stateX == 1, false, x, _frame, true);
        }
    }

    bool Input::any(int32_t mode, DeviceIndex devices, InputResult& result, const InputCode* ignored, size_t ignoredLength) const {
        result = { InputCode::INP_None, DeviceIndex::DEV_None, 0 };

        bool resultB;
        uint64_t tick = 0;
        int32_t offset = 0;
        size_t curSize = KEYBOARD_INPUTS;

        for (size_t i = 0, j = 1, k = 0; i < MAX_CONTROLLERS + 1; i++, j <<= 1) {
            if (j & devices) {
                for (size_t l = 0; l < curSize; l++) {
                    if (i == 0) {
                        switch (l) {
                            case InputCode::INP_Shift:
                            case InputCode::INP_Ctrl:
                            case InputCode::INP_Alt:
                                continue;
                        }
                    }

                    const auto& state = _inputs[k + l];
                    tick = state.getTickDown();

                    switch (mode)
                    {
                        default:
                            resultB = state.isDown();
                            break;
                        case 1:
                            resultB = state.isHeld();
                            break;
                        case 2:
                            resultB = state.isUp();
                            break;
                        case 3:
                            resultB = state.isToggled();
                            break;
                    }

                    if (resultB && result.tick < tick) {
                        if (ignored && ignoredLength > 0) {
                            auto iCode = InputCode(offset + l);
                            auto find = std::find(ignored, ignored + ignoredLength, iCode);

                            if (find != ignored + ignoredLength) {
                                result.device = DeviceIndex(j);
                                result.code = iCode;
                                result.tick = tick;
                            }
                        }
                    }
                }
            }

            k += offset;
            if (i == 0) {
                offset = KEYBOARD_INPUTS;
                curSize = CONTROLLER_INPUTS;
            }
        }
        return int32_t(result.device) != 0;
    }

    void Input::INTERNAL_vibrateDevice(int32_t device, double left, double right) const {
        INTERNAL_vibrateDevice(device, Math::deNormalize<uint16_t>(left), Math::deNormalize<uint16_t>(right));
    }

    void Input::INTERNAL_vibrateDevice(int32_t index, uint16_t left, uint16_t right) const {
        if (index < 0 || index >= MAX_CONTROLLERS) { return; }
        if (_gamePads[index].isConnected()) {
            XINPUT_VIBRATION vibration;
            ZeroMemory(&vibration, sizeof(XINPUT_VIBRATION));

            vibration.wLeftMotorSpeed = left;
            vibration.wRightMotorSpeed = right;

            XInputSetState(index, &vibration);
        }
    }
}