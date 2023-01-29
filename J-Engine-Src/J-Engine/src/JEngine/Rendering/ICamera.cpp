#include <JEngine/Rendering/ICamera.h>
#include <JEngine/IO/Serialization/Serialization.h>

namespace JEngine {
    const JColor32 ICamera::DEFAULT_CLEAR_COLOR = JColor32(26, 26, 26, 0);

    ICamera::ICamera() : 
        _screenRect(0, 0, 1, 1), _viewRect(0, 0, 1, 1), 
        _layerMask(0xFF), _flags(AUTO_RENDER_CAMERA), _tintColor(JColor32::White),
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

    void ICamera::setCameraDisabled(const bool state) { _flags.setBit(DISABLE_CAMERA, state); }
    const bool ICamera::isCameraDisabled() const { return bool(_flags & DISABLE_CAMERA); }

    void ICamera::setClearFlags(const ClearFlags flags) { _clearFlags = flags; }
    const ICamera::ClearFlags ICamera::getClearFlags() const { return _clearFlags; }

    void ICamera::setClearColor(const JColor32& color) { _clearColor = color; }
    const JColor32& ICamera::getClearColor() const { return _clearColor; }

    void ICamera::setManualRenderMode(const bool state) { _flags.setBit(AUTO_RENDER_CAMERA, !state); }
    const bool ICamera::getManualRenderModeState() const { return !bool(_flags & AUTO_RENDER_CAMERA); }

    void ICamera::setCameraTint(const JColor32& color) { _tintColor = color; }
    const JColor32& ICamera::getCameraTint() const { return _tintColor; }

    void ICamera::setLayerMask(const LayerMask mask) { _layerMask = mask; }
    const LayerMask ICamera::getLayerMask() const { return _layerMask; }

    const bool ICamera::shouldRender() const { return !bool(_flags & DISABLE_CAMERA); }

    void ICamera::transformProjection(JMatrix& projection) const { projection *= getTransformMatrix().getInverse(); }

    const bool ICamera::serializeJson(json& jsonF) const  {
        jsonF["depth"] = _depth;
        _flags.serializeJson(jsonF["flags"]);
        jsonF["clearFlags"] = uint32_t(_clearFlags);
        _clearColor.serializeJson(jsonF["clearColor"]);
        _tintColor.serializeJson(jsonF["cameraTint"]);
        _layerMask.serializeJson(jsonF["layerMask"]);
        _viewRect.serializeJson(jsonF["viewRect"]);
        _screenRect.serializeJson(jsonF["screenRect"]);

        const UUID uuid = _blendMaterial.getPtr() ? _blendMaterial.getPtr()->getUUID() : UUIDFactory::UUID_EMPTY;
        jsonF["blendMaterial"] = UUIDFactory::getUUIDStr(uuid);

        return true;
    }

    const bool ICamera::deserializeJson(json& jsonF) {
        _depth = jsonF.value("depth", 0.0f);
        _flags.deserializeJson(jsonF["flags"]);
        _clearFlags = ClearFlags(jsonF.value("clearFlags", uint32_t(ClearFlags::Clear_Color | Clear_Depth)));
        _clearColor.deserializeJson(jsonF["clearColor"]);
        _tintColor.deserializeJson(jsonF["cameraTint"]);
        _layerMask.deserializeJson(jsonF["layerMask"]);
        _viewRect.deserializeJson(jsonF["viewRect"]);
        _screenRect.deserializeJson(jsonF["screenRect"]);

        UUID uuid = UUIDFactory::UUID_EMPTY;
        UUIDFactory::parseUUID(jsonF["blendMaterial"].get<std::string>(), uuid);
        //TODO: Find material by UUID
  
        return true;
    }

    const bool ICamera::serializeBinary(std::ostream& stream) const {
        Serialization::serializeBinary(stream, _depth);
        _flags.serializeBinary(stream);
        Serialization::serializeBinary(stream, _clearFlags);
        _clearColor.serializeBinary(stream);
        _tintColor.serializeBinary(stream);
        _layerMask.serializeBinary(stream);
        _viewRect.serializeBinary(stream);
        _screenRect.serializeBinary(stream);

        const UUID uuid = _blendMaterial.getPtr() ? _blendMaterial.getPtr()->getUUID() : UUIDFactory::UUID_EMPTY;
        Serialization::serializeBinary(stream, uuid);

        return true;
    }

    const bool ICamera::deserializeBinary(std::istream& stream)  {
        Serialization::deserializeBinary(stream, _depth);
        _flags.deserializeBinary(stream);
        Serialization::deserializeBinary(stream, _clearFlags);
        _clearColor.deserializeBinary(stream);
        _tintColor.deserializeBinary(stream);
        _layerMask.deserializeBinary(stream);
        _viewRect.deserializeBinary(stream);
        _screenRect.deserializeBinary(stream);

        UUID uuid =  UUIDFactory::UUID_EMPTY;
        Serialization::deserializeBinary(stream, uuid);
        //TODO: Find material by UUID

        return true;
    }

    const bool ICamera::render() { return renderInternal(false); }
    const bool ICamera::renderInternal(const bool isAuto) {
        if (!PREPARE_RENDER || !RENDER_OBJECTS || !WRITE_TO_BUFFER) {
            return false;
        }

        if (!shouldRender() || (!isAuto && !(_flags & AUTO_RENDER_CAMERA))) { return false; }
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
