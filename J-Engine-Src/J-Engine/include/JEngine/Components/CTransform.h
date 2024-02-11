#pragma once
#include <JEngine/Collections/Vector.h>
#include <JEngine/Components/Component.h>
#include <JEngine/Math/Units/JMatrix.h>
#include <JEngine/Math/Units/JVector.h>
#include <JEngine/Utility/EnumUtils.h>

namespace JEngine {
    enum SpaceType : uint8_t {
        SPACE_FULL,
        SPACE_PART,
    };

    enum class Space : uint8_t {
        None,
        Local,
        World,
    };
    CREATE_ENUM_OPERATORS_SELF(Space);

    template<>
    inline constexpr size_t EnumNames<Space, SPACE_FULL>::Count{ 3 };

    template<>
    inline constexpr std::string_view EnumNames<Space, SPACE_FULL>::Names[Count]
    {
        "None",
        "Local",
        "World",
    };

    template<>
    inline constexpr size_t EnumNames<Space, SPACE_PART>::Count{ 2 };

    template<>
    inline constexpr uint8_t EnumNames<Space, SPACE_PART>::MinValue{ uint8_t(Space::World - Space::Local) };

    template<>
    inline constexpr std::string_view EnumNames<Space, SPACE_PART>::Names[Count]
    {
        "Local",
        "World",
    };


    class GameObject;
    class CTransform : public Component {
    public:
        CTransform();
        ~CTransform();

        bool setParent(TCompRef<CTransform> parent);
        TCompRef<CTransform> getParent() const { return _parent; }

        void transform(const JVector3f& translation, const JVector3f& rotation, const JVector3f& scale, Space space = Space::World);
        void translate(const JVector3f& translation, Space space = Space::World);
        void rotate(const JVector3f& rotation, Space space = Space::World);
        void scale(const JVector3f& scale, Space space = Space::World);

        void setTRS(const JVector3f& pos, const JVector3f& rot, const JVector3f& scale, Space space = Space::World);

        void setPosition(const JVector3f& pos, Space space = Space::World);
        void setRotation(const JVector3f& rot, Space space = Space::World);
        void setScale(const JVector3f& scale, Space space = Space::World);

        JVector3f getPosition(Space space = Space::World) const {
            return space == Space::World && _parent ? _parent->getWorldMatrix().transformPoint(_lPos) : _lPos;
        }
        JVector3f getRotation(Space space = Space::World) const {
            return space == Space::World && _parent ? _parent->getRotation() + _lRot : _lRot;
        }
        JVector3f getScale(Space space = Space::World) const {
            return space == Space::World && _parent ? _parent->getWorldMatrix().transformVector(_lSca) : _lSca;
        }

        JMatrix4f getLocalMatrix() const { return JMatrix4f(_lPos, _lRot, _lSca); }
        JMatrix4f getWorldMatrix() const { return _parent ? _parent->getWorldMatrix() * getLocalMatrix() : getLocalMatrix(); }

        TCompRef<CTransform> addChild(TCompRef<CTransform> tr);
        bool removeChild(TCompRef<CTransform> tr);

        size_t getChildCount() const { return _children.size(); }

        TCompRef<CTransform> getChildAt(size_t index) const;
        TCompRef<CTransform> find(std::string_view name) const;

        static void bindFields(Type& type);

    protected:
        friend class GameObject;
        friend class SerializedItem;

        void onInit() override;
        JE_COMPONENT(JEngine::CTransform)

    private:
        JVector3f _lPos;
        JVector3f _lRot;
        JVector3f _lSca;

        TCompRef<CTransform> _parent{};
        Vector<TCompRef<CTransform>> _children{};

        void transform(const JVector3f* tra, const JVector3f* rot, const JVector3f* sca, Space space);
        void update(Space space, const JVector3f* pos, const JVector3f* rot, const JVector3f* scale);

        friend class GameObject;
        void destroyChildren();
    };

    template<>
    inline constexpr ComponentFlags ComponentInfo<CTransform>::Flags = COMP_IS_TRANSFORM;

    template<>
    inline constexpr const char* ComponentInfo<CTransform>::Name = "CTransform";
}
REGISTER_COMPONENT(JEngine::CTransform);