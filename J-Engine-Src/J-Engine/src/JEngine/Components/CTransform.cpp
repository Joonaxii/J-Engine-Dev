#include <JEngine/Components/CTransform.h>
#include <JEngine/Core/GameObject.h>
#include <JEngine/Core/Scene.h>
#include <JEngine/Utility/StringHelpers.h>

namespace JEngine {
    CTransform::CTransform() : Component(),
        _parent(nullptr),
        _lPos(), _lRot(), _lSca(1, 1) { }

    CTransform::~CTransform() { 
        destroyChildren();
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
        JENGINE_CORE_ASSERT(index >= 0 && index < _children.size(), "Index out of range!");
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

    void CTransform::doDelete() {
        Component::deleteComponent<CTransform>(this);
    }

    void CTransform::deserializeBinaryImpl(const Stream& stream) {
        uint32_t childCount{};

        Serialization::deserialize(_parent, stream);
        Serialization::deserialize(_lPos, stream);
        Serialization::deserialize(_lRot, stream);
        Serialization::deserialize(_lSca, stream);

        Serialization::deserialize(childCount, stream);

        _children.clear();
        TCompRef<CTransform> trCH{};
        for (uint32_t i = 0; i < childCount; i++) {
            Serialization::deserialize(trCH, stream);
            if (trCH.isValid()) {
                _children.emplace_back(trCH);
            }
        }
    }
    void CTransform::serializeBinaryImpl(const Stream& stream) const {
        Serialization::serialize(_parent, stream);
        Serialization::serialize(_lPos, stream);
        Serialization::serialize(_lRot, stream);
        Serialization::serialize(_lSca, stream);

        Serialization::serialize(uint32_t(_children.size()), stream);
        for (auto& child : _children) {
            Serialization::serialize(child, stream);
        }
    }

    void CTransform::deserializeYAMLImpl(yamlNode& node) {
        Serialization::deserialize(_parent.uuid, node["parent"]);
        Serialization::deserialize(_lPos, node["position"]);
        Serialization::deserialize(_lRot, node["rotation"]);
        Serialization::deserialize(_lSca, node["scale"]);

        auto& children = node["children"];

        _children.clear();
        if (children.IsSequence()) {
            TCompRef<CTransform> trCH{};
            uint32_t childCount = uint32_t(children.size());
            for (uint32_t i = 0; i < childCount; i++) {
                Serialization::deserialize(trCH.uuid, children[i]);
                if (trCH.isValid()) {
                    _children.emplace_back(trCH);
                }
            }
        }
    }
    void CTransform::serializeYAMLImpl(yamlEmit& emit) const {
        emit << YAML::Key << "parent" << YAML::Value << _parent;
        emit << YAML::Key << "position" << YAML::Value << _lPos;
        emit << YAML::Key << "rotation" << YAML::Value << _lRot;
        emit << YAML::Key << "scale" << YAML::Value << _lSca;

        emit << YAML::Key << "children" << YAML::Value << YAML::BeginSeq;
        for (auto& child : _children) {
            emit << child.uuid;
        }
        emit << YAML::EndSeq;
    }

    void CTransform::deserializeJSONImpl(json& jsonF) {
        Serialization::deserialize(_parent.uuid, jsonF["parent"]);
        Serialization::deserialize(_lPos, jsonF["position"]);
        Serialization::deserialize(_lRot, jsonF["rotation"]);
        Serialization::deserialize(_lSca, jsonF["scale"]);

        auto& children = jsonF["children"];
        _children.clear();
        if (children.is_array()) {
            TCompRef<CTransform> trCH{};
            uint32_t childCount = uint32_t(children.size());
            for (uint32_t i = 0; i < childCount; i++) {
                Serialization::deserialize(trCH.uuid, children[i]);
                if (trCH.isValid()) {
                    _children.emplace_back(trCH);
                }
            }
        }
    }
    void CTransform::serializeJSONImpl(json& jsonF) const {
        Serialization::serialize(_parent.uuid, jsonF["parent"]);
        Serialization::serialize(_lPos, jsonF["position"]);
        Serialization::serialize(_lRot, jsonF["rotation"]);
        Serialization::serialize(_lSca, jsonF["scale"]);

        auto& chArr = jsonF["children"] = json::array_t{};
        for (auto& child : _children) {
            Serialization::serialize(child.uuid, chArr.emplace_back());
        }
    }
}