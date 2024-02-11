#pragma once
#include <vector>
#include <Editor/Core/EditorEssentials.h>
#include <JEngine/Rendering/ImGui/ImGuiUtils.h>

namespace JEngine {
    struct SerializedItem;
    namespace detail {
        struct CustomSer {
            const Type* type;
            bool (*drawSelf)(SerializedItem& node);

            CustomSer() : type(nullptr), drawSelf(nullptr) {}
            CustomSer(const Type& type, bool (*drawSelf)(SerializedItem& node)) : type(&type), drawSelf(drawSelf) {}
        };
    }

    struct SerializedItem;
    struct CustomSerializables {

        static std::vector<detail::CustomSer*>& getCustomSers() {
            static std::vector<detail::CustomSer*> sers;
            return sers;
        }

        static detail::CustomSer* findCustomSerByID(uint32_t id) {
            auto& sers = getCustomSers();
            for (auto& ser : sers) {
                if (ser && ser->type && ser->type->hash == id) { return ser; }
            }
            return nullptr;
        }

        template<typename T>
        static detail::CustomSer& getCustom();

        template<typename T>
        static bool drawSelf(SerializedItem& node);

        template<typename T>
        static constexpr ValueType getValueType() {
            return ValueType::VTYPE_CUSTOM;
        }

        template<typename T>
        static constexpr ValueType getSubValueType() {
            return SubValueType::SUB_VTYPE_NONE;
        }

        template<typename T>
        static constexpr const GUIStyle& getDefaultStyle() {
            return Gui::Styles::Default;
        }

        static uint8_t customDraw(SerializedItem& node, uint32_t id) {
            auto ser = findCustomSerByID(id);
            if (ser && ser->drawSelf) {
                return ser->drawSelf(node) ? 0x01 : 0x00;
            }
            return 0x2;
        }
    };

    namespace Serialization {


        void registerAll();
    }
}

#define REGISTER_SERIALIZABLE(TYPE)\
\
inline constexpr bool JEngine::detail::HasCustomGui<TYPE>::Value = true;\
template<>\
inline JEngine::detail::CustomSer& JEngine::CustomSerializables::getCustom<TYPE>() { \
       static JEngine::detail::CustomSer custom( \
            JEngine::TypeHelpers::getType<TYPE>(), \
            JEngine::CustomSerializables::drawSelf<TYPE> \
            ); \
       static bool init{false}; \
       if(!init){ \
            JEngine::CustomSerializables::getCustomSers().push_back(&custom);  \
            init = true; \
       } \
       return custom; \
} \


#define DEFINE_SERIALIZABLE_TYPE(TYPE, VTYPE, STYPE)\
template<> \
inline constexpr JEngine::ValueType JEngine::CustomSerializables::getValueType<TYPE>() { \
    return VTYPE; \
} \
template<> \
inline constexpr JEngine::SubValueType JEngine::CustomSerializables::getSubValueType<TYPE>() { \
        return STYPE; \
} \

#define DEFINE_STYLED_SERIALIZABLE_TYPE(TYPE, VTYPE, STYPE, STYLE) \
DEFINE_SERIALIZABLE_TYPE(TYPE, VTYPE, STYPE) \
template<> \
inline constexpr const JEngine::GUIStyle& JEngine::CustomSerializables::getDefaultStyle<TYPE>() { \
    return STYLE; \
} \


