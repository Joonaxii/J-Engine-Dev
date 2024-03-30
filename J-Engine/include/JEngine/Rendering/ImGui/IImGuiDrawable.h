#pragma once 
#include <JEngine/Rendering/SortingLayer.h>
#include <JEngine/Utility/Flags.h>
#include <JEngine/Rendering/ICamera.h>

namespace JEngine {
    class IImGuiDrawable {
    public:
        typedef void (*RegistrationEvent)(IImGuiDrawable* guiDrawable);

        IImGuiDrawable() : _camera(nullptr), _sortingLayer(), _flags(FLAG_VISIBLE) { registerGuiDrawable(this); }
        ~IImGuiDrawable() { unregisterGuiDrawable(this); }

        int32_t compareTo(const IImGuiDrawable& other) const {
            if (_camera == other._camera) {
                if (_camera) {
                    const int32_t comp = _camera->compareTo(*other._camera);
                    if (comp) { return comp; }
                    goto end;
                }
            }

            if (_camera && !other._camera) { return -1; }
            if (!_camera && other._camera) { return 1; }

            end:
            return _sortingLayer.compareTo(other._sortingLayer);
        }

        void setGuiLayer(const SortingLayer& layer) { _sortingLayer = layer; }
        const SortingLayer& getGuiLayer() const { return _sortingLayer; }

        void setGuiVisible(const bool value) { _flags.setBit(FLAG_VISIBLE, value); }
        virtual bool canRenderGui() const { return bool(_flags & FLAG_VISIBLE); }

        const ICamera* getGuiCamera() const { return _camera; }
        ICamera* getGuiCamera() { return _camera; }

        virtual void onImGui() { }

        static void setRegistrationMethods(RegistrationEvent reg, RegistrationEvent unreg) {
            REGISTER_GUI_DRAWABLE = reg;
            UNREGISTER_GUI_DRAWABLE = unreg;
        }
    private:

        static inline RegistrationEvent REGISTER_GUI_DRAWABLE = nullptr;
        static inline RegistrationEvent UNREGISTER_GUI_DRAWABLE = nullptr;

        static constexpr uint8_t FLAG_VISIBLE = 0x1;

        ICamera* _camera;

        UI8Flags _flags;
        SortingLayer _sortingLayer;
   
        static void registerGuiDrawable(IImGuiDrawable* rend) {
            if (REGISTER_GUI_DRAWABLE) {
                REGISTER_GUI_DRAWABLE(rend);
            }
        }
        static void unregisterGuiDrawable(IImGuiDrawable* rend) {
            if (UNREGISTER_GUI_DRAWABLE) {
                UNREGISTER_GUI_DRAWABLE(rend);
            }
        }
    };
}