#include <JEngine/Rendering/ICamera.h>

namespace JEngine {
    const JColor32 ICamera::DEFAULT_CLEAR_COLOR = JColor32(26, 26, 26, 0);

    ICamera::ICamera() : ICamera(AUTO_RENDER_CAMERA) {}

    ICamera::ICamera(uint8_t flags) : 
        _screenRect(0, 0, 1, 1), _viewRect(0, 0, 1, 1), 
        _layerMask(0xFF), _cameraFlags(flags), _tintColor(JColor32::White),
        _clearColor(DEFAULT_CLEAR_COLOR), _depth(0.0f), _clearFlags(ClearFlags(Clear_Depth | Clear_Color)) 
    { 
        if ((_cameraFlags & CAMERA_NO_REGISTRATION) == 0) {
            registerCamera(this); 
        }
    }

    ICamera::~ICamera() {
        if ((_cameraFlags & CAMERA_NO_REGISTRATION) == 0) {
            unregisterCamera(this);
        }
    }

    int32_t ICamera::compareTo(const ICamera& other) const {
        if (_depth < other._depth) { return 1; }
        if (_depth > other._depth) { return -1; }
        return 0;
    }

    void ICamera::setViewRect(const JRectf& viewRect) { _viewRect = viewRect; }
    const JRectf& ICamera::getViewRect() const { return _viewRect; }
    
    void ICamera::setScreenRect(const JRectf& screenRect) { _screenRect = screenRect; }
    const JRectf& ICamera::getScreenRect() const { return _screenRect;  }

    void ICamera::setCameraDepth(const float depth) { _depth = depth; }
    float ICamera::getCameraDepth() const { return _depth; }

    void ICamera::setCameraDisabled(const bool state) { _cameraFlags.setBit(DISABLE_CAMERA, state); }
    bool ICamera::isCameraDisabled() const { return bool(_cameraFlags & DISABLE_CAMERA); }

    void ICamera::setClearFlags(const ClearFlags flags) { _clearFlags = flags; }
    ICamera::ClearFlags ICamera::getClearFlags() const { return _clearFlags; }

    void ICamera::setClearColor(const JColor32& color) { _clearColor = color; }
    const JColor32& ICamera::getClearColor() const { return _clearColor; }

    void ICamera::setManualRenderMode(const bool state) { _cameraFlags.setBit(AUTO_RENDER_CAMERA, !state); }
    bool ICamera::getManualRenderModeState() const { return !bool(_cameraFlags & AUTO_RENDER_CAMERA); }

    void ICamera::setCameraTint(const JColor32& color) { _tintColor = color; }
    const JColor32& ICamera::getCameraTint() const { return _tintColor; }

    void ICamera::setLayerMask(const LayerMask mask) { _layerMask = mask; }
    LayerMask ICamera::getLayerMask() const { return _layerMask; }

    bool ICamera::shouldRender() const { return !bool(_cameraFlags & DISABLE_CAMERA); }

    void ICamera::transformProjection(JMatrix4f& projection) const { projection *= getTransformMatrix().getInverse(); }

    bool ICamera::serializeJson(json& jsonF) const {
        Serialization::serialize(_cameraFlags, jsonF["cameraFlags"]);
        Serialization::serialize(_depth, jsonF["depth"]);

        Serialization::serialize(_clearFlags, jsonF["clearFlags"]);
        Serialization::serialize(_clearColor, jsonF["clearColor"]);

        Serialization::serialize(_tintColor, jsonF["cameraTint"]);
        Serialization::serialize(_layerMask, jsonF["layerMask"]);
        Serialization::serialize(_viewRect, jsonF["viewRect"]);
        Serialization::serialize(_screenRect, jsonF["screenRect"]);

        //UUID8 uuid = _blendMaterial.getPtr() ? _blendMaterial.getPtr()->getUUID() : UUID8::Empty;
        //Serialization::serialize(uuid, jsonF["blendMaterial"]);
        return true;
    }

    bool ICamera::deserializeJson(json& jsonF) {
        Serialization::deserialize(_cameraFlags, jsonF["cameraFlags"]);
        Serialization::deserialize(_depth, jsonF["depth"]);

        Serialization::deserialize(_clearFlags, jsonF["clearFlags"]);
        Serialization::deserialize(_clearColor, jsonF["clearColor"]);

        Serialization::deserialize(_tintColor, jsonF["cameraTint"]);
        Serialization::deserialize(_layerMask, jsonF["layerMask"]);
        Serialization::deserialize(_viewRect, jsonF["viewRect"]);
        Serialization::deserialize(_screenRect, jsonF["screenRect"]);

        UUID8 uuid{};
        //Serialization::deserialize(uuid, jsonF["blendMaterial"]);
        //TODO: Add fetching of material by UUID
        return true;
    }

    bool ICamera::serializeBinary(const Stream& stream) const {
        Serialization::serialize(_cameraFlags, stream);
        Serialization::serialize(_depth, stream);

        Serialization::serialize(reinterpret_cast<const uint32_t&>(_clearFlags), stream);
        Serialization::serialize(_clearColor, stream);

        Serialization::serialize(_tintColor, stream);
        Serialization::serialize(_layerMask, stream);
        Serialization::serialize(_viewRect, stream);
        Serialization::serialize(_screenRect, stream);

        //const UUID8 uuid = _blendMaterial.getPtr() ? _blendMaterial.getPtr()->getUUID() : UUID8::Empty;
        //Serialization::serialize(uuid, stream);
        return true;
    }

    bool ICamera::deserializeBinary(const Stream& stream)  {
        Serialization::deserialize(_cameraFlags, stream);
        Serialization::deserialize(_depth, stream);

        Serialization::deserialize(reinterpret_cast<uint32_t&>(_clearFlags), stream);
        Serialization::deserialize(_clearColor, stream);

        Serialization::deserialize(_tintColor, stream);
        Serialization::deserialize(_layerMask, stream);
        Serialization::deserialize(_viewRect, stream);
        Serialization::deserialize(_screenRect, stream);

        //UUID8 uuid{};
        //Serialization::deserialize(uuid, stream);
        //TODO: Find material by UUID

        return true;
    }

    bool ICamera::render() { return renderInternal(false); }
    bool ICamera::renderInternal(const bool isAuto) {
        if (!PREPARE_RENDER || !RENDER_OBJECTS || !WRITE_TO_BUFFER) {
            return false;
        }

        if (!shouldRender() || (!isAuto && !(_cameraFlags & AUTO_RENDER_CAMERA))) { return false; }
        auto& trMatrix = getTransformMatrix();
        CameraRenderData renderData;

        renderData.doBlend = true;
        renderData.screenRect = _screenRect;
        renderData.viewRect = _viewRect;

        PREPARE_RENDER(_clearColor, _clearFlags, renderData);

        trMatrix.transformRect(renderData.worldRect);
        transformProjection(renderData.projection);

        RENDER_OBJECTS(this, _layerMask, renderData);
        WRITE_TO_BUFFER(nullptr, _tintColor, renderData);
        return true;
    }
}
