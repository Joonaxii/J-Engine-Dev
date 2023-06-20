#pragma once
#include <JEngine/Components/Component.h>
#include <JEngine/Math/Units/JMatrix.h>
#include <JEngine/Math/Units/JVector.h>

namespace JEngine {
    class CTransform : public Component {
    public:
        ~CTransform();

        virtual const char* getComponentName() const { return "Component"; }
    protected:
        friend class GameObject;
        CTransform(GameObject* obj, uint16_t flags = 0x00);

    private:
        JVector2f _wPos;
        JVector2f _lPos;

        float _wRot;
        float _lRot;

        JVector2f _wSca;
        JVector2f _lSca;

        JMatrix4f _matW;
        JMatrix4f _matL;

        CTransform* _parent;
        std::vector<CTransform*> _children{};
    };
}

template<>
inline const bool JEngine::IsTransform<JEngine::CTransform>::Value = true;