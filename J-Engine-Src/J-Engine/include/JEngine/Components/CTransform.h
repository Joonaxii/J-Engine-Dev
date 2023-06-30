#pragma once
#include <JEngine/Components/Component.h>
#include <JEngine/Math/Units/JMatrix.h>
#include <JEngine/Math/Units/JVector.h>
#include <JEngine/Utility/EnumUtils.h>

namespace JEngine {
    enum class Space : uint8_t {
        None,
        Local,
        World,
    };

    template<>
    inline constexpr int32_t EnumNames<Space>::Count{ 3 };

    template<>
    inline const char** EnumNames<Space>::getEnumNames() {
        static const char* names[] =
        {
            "",
            "Local",
            "World",
        };
        return names;
    }
    template<>
    inline bool EnumNames<Space>::noDraw(int32_t index) {
        return index < 1 || index >= Count;
    }

    class GameObject;
    class CTransform : public Component {
    public:
        CTransform();
        ~CTransform();

        virtual void dCtor() override;

        bool setParent(CTransform* parent);
        CTransform* getParent() { return _parent; }
        const CTransform* getParent() const { return _parent; }

        void transform(const JVector2f& translation, float rotation, const JVector2f& scale, Space space = Space::World);
        void translate(const JVector2f& translation, Space space = Space::World);
        void rotate(float rotation, Space space = Space::World);
        void scale(const JVector2f& scale, Space space = Space::World);

        void setTRS(const JVector2f& pos, float rot, const JVector2f& scale, Space space = Space::World);

        void setPosition(const JVector2f& pos, Space space = Space::World);
        void setRotation(float rot, Space space = Space::World);
        void setScale(const JVector2f& scale, Space space = Space::World);

        const JVector2f& getPosition(Space space = Space::World) const { 
            return space == Space::World && _parent ? _parent->getWorldMatrix().transformPoint(_lPos) : _lPos;
        }
        float getRotation(Space space = Space::World) const {
            return space == Space::World && _parent ? _parent->getRotation() + _lRot : _lRot; 
        }
        const JVector2f& getScale(Space space = Space::World) const { 
            return space == Space::World && _parent ? _parent->getWorldMatrix().transformVector(_lSca) : _lSca; 
        }

        JMatrix4f getLocalMatrix() const { return JMatrix4f(_lPos, _lRot, _lSca); }
        JMatrix4f getWorldMatrix() const { return _parent ? _parent->getWorldMatrix() * getLocalMatrix() : getLocalMatrix(); }

        CTransform* addChild(CTransform* tr);
        bool removeChild(CTransform* tr);

        size_t getChildCount() const { return _children.size(); }

        CTransform* getChildAt(size_t index);
        const CTransform* getChildAt(size_t index) const;

        CTransform* find(const char* name) const;

    protected:
        friend class GameObject;

        void onInit() override;
        void onDestroy() override;

        void deserializeBinaryImpl(const Stream& stream) override;
        void serializeBinaryImpl(const Stream& stream) const override;

        void deserializeYAMLImpl(yamlNode& node) override;
        void serializeYAMLImpl(yamlEmit& emit) const override;

        void deserializeJSONImpl(json& jsonF) override;
        void serializeJSONImpl(json& jsonF) const override;

    private:
        JVector2f _lPos;
        float _lRot;
        JVector2f _lSca;

        CTransform* _parent;
        std::vector<CTransform*> _children{};

        void transform(const JVector2f* tra, const float* rot, const JVector2f* sca, Space space);
        void update(Space space, const JVector2f* pos, const float* rot, const JVector2f* scale);

        friend class GameObject;
        void destroyChildren();
    };

    template<>
    inline constexpr ComponentFlags ComponentInfo<CTransform>::Flags = COMP_IS_TRANSFORM;

    template<>
    inline constexpr const char* ComponentInfo<CTransform>::Name = "Transform";
}