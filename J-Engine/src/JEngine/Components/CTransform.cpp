#include <JEngine/Components/CTransform.h>
#include <JEngine/Core/GameObject.h>
#include <JEngine/Core/Scene.h>
#include <JEngine/Utility/StringHelpers.h>
#include <JEngine/GUI/Gui.h>

namespace JEngine {

    static bool drawGui(SerializedItem& item) {
        bool changed = false;
        auto pos = item[STR(_lPos)];
        auto rot = item[STR(_lRot)];
        auto sca = item[STR(_lSca)];

        JE_CORE_ASSERT(pos != nullptr, "Did not find field for Transform position!");
        JE_CORE_ASSERT(rot != nullptr, "Did not find field for Transform rotation!");
        JE_CORE_ASSERT(sca != nullptr, "Did not find field for Transform scale!");

        changed |= Gui::drawBuiltIn("Position", VType::VTYPE_VEC_3F, pos->getData(), pos->getStyle());
        changed |= Gui::drawBuiltIn("Rotation", VType::VTYPE_VEC_3F, rot->getData(), rot->getStyle());
        changed |= Gui::drawBuiltIn("Scale", VType::VTYPE_VEC_3F, sca->getData(), sca->getStyle());
        return changed;
    }

    CTransform::CTransform() : Component(),
        _parent(nullptr),
        _lPos(), _lRot(), _lSca(1, 1) { }

    CTransform::~CTransform() { 
        destroyChildren();
        constexpr std::string_view test = EnumNames<Space>::getEnumName(Space::Local);
    }

    void CTransform::bindFields(Type& type) {
        ADD_FIELD(type, _lPos, CTransform, VType::VTYPE_VEC_3F, "position");
        ADD_FIELD(type, _lRot, CTransform, VType::VTYPE_VEC_3F, "rotation");
        ADD_FIELD(type, _lSca, CTransform, VType::VTYPE_VEC_3F, "scale");
        ADD_FIELD(type, _parent, CTransform, VType::VTYPE_COMPONENT_REF, "parent", BUILD_FLAGS(0, CTransform));
        ADD_FIELD(type, _children, CTransform, VType::VTYPE_COMPONENT_REF, "children", BUILD_FLAGS(FieldFlags::IS_VECTOR, CTransform));
        type.customDraw = drawGui;
    }

    bool CTransform::setParent(TCompRef<CTransform> parent) {
        if (parent == getObject()->getTransform()) { return false; }
        if (_parent == parent) { return true; }

        if (_parent) {
            _parent->removeChild(this);
        }

        _parent = parent;
        if (_parent) {
            _parent->_children.emplace_back(this);
        }

        Component* comp = this;
        constexpr uint32_t test = comp->TYPE_HASH;
        return true;
    }

    void CTransform::setTRS(const JVector3f& pos, const JVector3f& rot, const JVector3f& scale, Space space) {
        switch (space) {
            case Space::Local:
                _lPos = pos;
                _lRot = rot;
                _lSca = scale;;
                break;
            case Space::World:
                update(Space::Local, &pos, &rot, &scale);
                break;
        }
    }

    void CTransform::transform(const JVector3f& tra, const JVector3f& rot, const JVector3f& sca, Space space) {
        transform(&tra, &rot, &sca, space);
    }

    void CTransform::translate(const JVector3f& translation, Space space) {
        switch (space) {
            case Space::Local:
                _lPos += translation;
                break;
            case Space::World:
                transform(&translation, nullptr, nullptr, Space::World);
                break;
        }
    }

    void CTransform::rotate(const JVector3f& rotation, Space space) {
        switch (space) {
            case Space::Local:
                _lRot += rotation;
                break;
            case Space::World:
                transform(nullptr, &rotation, nullptr, Space::World);
                break;
        }
    }

    void CTransform::scale(const JVector3f& scale, Space space) {
        switch (space) {
            case Space::Local:
                _lSca *= scale;
                break;
            case Space::World:
                transform(nullptr, nullptr, &scale, Space::World);
                break;
        }
    }

    void CTransform::setPosition(const JVector3f& pos, Space space) {
        switch (space) {
            case Space::Local:
                _lPos = pos;
                break;
            case Space::World:
                update(Space::World, &pos, nullptr, nullptr);
                break;
        }
    }

    void CTransform::setRotation(const JVector3f& rot, Space space) {
        switch (space) {
            case Space::Local:
                _lRot = rot;
                break;
            case Space::World: {
                update(Space::World, nullptr, &rot, nullptr);
                break;
            }
        }
    }

    void CTransform::setScale(const JVector3f& scale, Space space) {
        switch (space) {
            case Space::Local:
                _lSca = scale;
                break;
            case Space::World:
                update(Space::World, nullptr, nullptr, &scale);
                break;
        }
    }

    TCompRef<CTransform> CTransform::getChildAt(const size_t index) const {
        JE_CORE_ASSERT(index >= 0 && index < _children.size(), "Index out of range!");
        return _children[index];
    }

    TCompRef<CTransform> CTransform::find(std::string_view name) const {
        for (TCompRef<CTransform> child : _children) {
            if (child->getObject() && Helpers::strEquals(name, child->getObject()->getName())) {
                return child;
            }
        }
        return nullptr;
    }

    void CTransform::transform(const JVector3f* tra, const JVector3f* rot, const JVector3f* sca, Space space) {
        if (_parent && space == Space::World) {
            auto inv = getWorldMatrix().getInverse();
            if (tra) {
                _lPos += inv.transformPoint(*tra);
            }

            if (rot) {
                _lRot = _lRot - (_parent->getRotation(Space::World) - *rot);
            }

            if (sca) {
                _lSca *= inv.transformVector(*sca);
            }
            return;
        }
        _lPos = tra ? *tra + _lPos : _lPos;
        _lRot = rot ? *rot + _lRot : _lRot;
        _lSca = sca ? *sca * _lSca : _lSca;
    }

    void CTransform::update(Space space, const JVector3f* pos, const JVector3f* rot, const JVector3f* scale) {
        if (_parent && space == Space::World) {
            auto inv = getWorldMatrix().getInverse();
            if (pos) {
                _lPos = inv.transformPoint(*pos);
            }

            if (rot) {
                _lRot = _parent->getRotation(Space::World) - *rot;
            }

            if (scale) {
                _lSca = inv.transformVector(*scale);
            }
            return;
        }
        _lPos = pos ? *pos : _lPos;
        _lRot = rot ? *rot : _lRot;
        _lSca = scale ? *scale : _lSca;
    }

    TCompRef<CTransform> CTransform::addChild(TCompRef<CTransform> tr) {
        if (tr && tr != this && tr->_parent != this) {
            if (tr->_parent) {
                tr->_parent->removeChild(tr);
            }
            tr->_parent = this;
            _children.emplace_back(tr);
        }
        return tr;
    }

    bool CTransform::removeChild(TCompRef<CTransform> tr) {
        if (tr && tr != this) {
            auto find = std::find(_children.begin(), _children.end(), tr);
            if (find != _children.end()) {
                _children.erase(find);
                tr->_parent = nullptr;
                return true;
            }
        }
        return false;
    }

    void CTransform::destroyChildren() {
        for (size_t i = 0; i < _children.size(); i++) {
            Scene::destroyObject(_children[i]->getObject());
        }
        _children.clear();
    }

    void CTransform::onInit() {
        if (_parent) {
            _parent->removeChild(this);
            _parent = nullptr;
        }
        destroyChildren();

        _lPos = { 0, 0 };
        _lRot = { 0.0f, 0.0f, 0.0f };
        _lSca = { 1, 1 };
    }
}