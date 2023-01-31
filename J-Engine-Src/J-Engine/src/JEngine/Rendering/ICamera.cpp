#include <JEngine/Rendering/ICamera.h>
#include <JEngine/IO/Serialization/Serialization.h>

namespace JEngine {
    const JColor32 ICamera::DEFAULT_CLEAR_COLOR = JColor32(26, 26, 26, 0);

    ICamera::ICamera() : 
        _screenRect(0, 0, 1, 1), _viewRect(0, 0, 1, 1), 
        _layerMask(0xFF), _cameraFlags(AUTO_RENDER_CAMERA), _tintColor(JColor32::White),
        _clearColor(DEFAULT_CLEAR_COLOR), _depth(0.0f), _clearFlags(ClearFlags(Clear_Depth | Clear_Color)) 
    { registerCamera(this); }

    ICamera::~ICamera() { unregisterCamera(this); }

    const int32_t ICamera::compareTo(const ICamera& other) const {
        if (_depth < other._depth) { return 1; }
        if (_depth > other._depth) { return -1; }
        return 0;
    }

    void ICamera::setViewRect(const JRectf& viewRect) { _viewRect = viewRect; }
    const JRectf& ICamera::getViewRect() const { return _viewRect; }
    
    void ICamera::setScreenRect(const JRectf& screenRect) { _screenRect = screenRect; }
    const JRectf& ICamera::getScreenRect() const { return _screenRect;  }

    void ICamera::setCameraDepth(const float depth) { _depth = depth; }
    const float ICamera::getCameraDepth() const { return _depth; }

    void ICamera::setCameraDisabled(const bool state) { _cameraFlags.setBit(DISABLE_CAMERA, state); }
    const bool ICamera::isCameraDisabled() const { return bool(_cameraFlags & DISABLE_CAMERA); }

    void ICamera::setClearFlags(const ClearFlags flags) { _clearFlags = flags; }
    const ICamera::ClearFlags ICamera::getClearFlags() const { return _clearFlags; }

    void ICamera::setClearColor(const JColor32& color) { _clearColor = color; }
    const JColor32& ICamera::getClearColor() const { return _clearColor; }

    void ICamera::setManualRenderMode(const bool state) { _cameraFlags.setBit(AUTO_RENDER_CAMERA, !state); }
    const bool ICamera::getManualRenderModeState() const { return !bool(_cameraFlags & AUTO_RENDER_CAMERA); }

    void ICamera::setCameraTint(const JColor32& color) { _tintColor = color; }
    const JColor32& ICamera::getCameraTint() const { return _tintColor; }

    void ICamera::setLayerMask(const LayerMask mask) { _layerMask = mask; }
    const LayerMask ICamera::getLayerMask() const { return _layerMask; }

    const bool ICamera::shouldRender() const { return !bool(_cameraFlags & DISABLE_CAMERA); }

    void ICamera::transformProjection(JMatrix4f& projection) const { projection *= getTransformMatrix().getInverse(); }

    const bool ICamera::serializeJson(json& jsonF) const {
        Serialization::serialize(_cameraFlags, jsonF["cameraFlags"]);
        Serialization::serialize(_depth, jsonF["depth"]);

        Serialization::serialize(_clearFlags, jsonF["clearFlags"]);
        Serialization::serialize(_clearColor, jsonF["clearColor"]);

        Serialization::serialize(_tintColor, jsonF["cameraTint"]);
        Serialization::serialize(_layerMask, jsonF["layerMask"]);
        Serialization::serialize(_viewRect, jsonF["viewRect"]);
        Serialization::serialize(_screenRect, jsonF["screenRect"]);

        UUID uuid = _blendMaterial.getPtr() ? _blendMaterial.getPtr()->getUUID() : UUIDFactory::Empty;
        Serialization::serialize(uuid, jsonF["blendMaterial"]);
        return true;
    }

    const bool ICamera::deserializeJson(json& jsonF) {
        Serialization::deserialize(_cameraFlags, jsonF["cameraFlags"]);
        Serialization::deserialize(_depth, jsonF["depth"]);

        Serialization::deserialize(_clearFlags, jsonF["clearFlags"]);
        Serialization::deserialize(_clearColor, jsonF["clearColor"]);

        Serialization::deserialize(_tintColor, jsonF["cameraTint"]);
        Serialization::deserialize(_layerMask, jsonF["layerMask"]);
        Serialization::deserialize(_viewRect, jsonF["viewRect"]);
        Serialization::deserialize(_screenRect, jsonF["screenRect"]);

        UUID uuid{};
        Serialization::deserialize(uuid, jsonF["blendMaterial"]);
        //TODO: Add fetching of material by UUID
        return true;
    }

    const bool ICamera::serializeBinary(std::ostream& stream) const {
        Serialization::serialize(_cameraFlags, stream);
        Serialization::serialize(_depth, stream);

        Serialization::serialize(reinterpret_cast<const uint32_t&>(_clearFlags), stream);
        Serialization::serialize(_clearColor, stream);

        Serialization::serialize(_tintColor, stream);
        Serialization::serialize(_layerMask, stream);
        Serialization::serialize(_viewRect, stream);
        Serialization::serialize(_screenRect, stream);

        const UUID uuid = _blendMaterial.getPtr() ? _blendMaterial.getPtr()->getUUID() : UUIDFactory::Empty;
        Serialization::serialize(uuid, stream);
        return true;
    }

    const bool ICamera::deserializeBinary(std::istream& stream)  {
        Serialization::deserialize(_cameraFlags, stream);
        Serialization::deserialize(_depth, stream);

        Serialization::deserialize(reinterpret_cast<uint32_t&>(_clearFlags), stream);
        Serialization::deserialize(_clearColor, stream);

        Serialization::deserialize(_tintColor, stream);
        Serialization::deserialize(_layerMask, stream);
        Serialization::deserialize(_viewRect, stream);
        Serialization::deserialize(_screenRect, stream);

        UUID uuid{};
        Serialization::deserialize(uuid, stream);
        //TODO: Find material by UUID

        return true;
    }

    const bool ICamera::render() { return renderInternal(false); }
    const bool ICamera::renderInternal(const bool isAuto) {
        if (!PREPARE_RENDER || !RENDER_OBJECTS || !WRITE_TO_BUFFER) {
            return false;
        }

        if (!shouldRender() || (!isAuto && !(_cameraFlags & AUTO_RENDER_CAMERA))) { return false; }
        auto& trMatrix = getTransformMatrix();
        CameraRenderData renderData;

        renderData.screenRect = _screenRect;
        renderData.viewRect = _viewRect;

        PREPARE_RENDER(_clearColor, _clearFlags, renderData);

        trMatrix.transformRect(renderData.worldRect);
        transformProjection(renderData.projection);

        RENDER_OBJECTS(this, _layerMask, renderData);
        WRITE_TO_BUFFER(_blendMaterial, _tintColor, renderData);
        return true;
    }
}
