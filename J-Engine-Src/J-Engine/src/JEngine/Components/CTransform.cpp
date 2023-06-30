#include <JEngine/Components/CTransform.h>
#include <JEngine/Core/GameObject.h>

namespace JEngine {
    CTransform::CTransform() : Component(),
        _parent(nullptr),
        _lPos(), _lRot(), _lSca(1, 1) { }

    CTransform::~CTransform() { }

    void CTransform::dCtor() {
        Component::dCtor();
        destroyChildren();
    }

    bool CTransform::setParent(CTransform* parent) {
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

    void CTransform::setTRS(const JVector2f& pos, float rot, const JVector2f& scale, Space space) {
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

    void CTransform::transform(const JVector2f& tra, float rot, const JVector2f& sca, Space space) {
        transform(&tra, &rot, &sca, space);
    }

    void CTransform::translate(const JVector2f& translation, Space space) {
        switch (space) {
            case Space::Local:
                _lPos += translation;
                break;
            case Space::World:
                transform(&translation, nullptr, nullptr, Space::World);
                break;
        }
    }

    void CTransform::rotate(float rotation, Space space) {
        switch (space) {
            case Space::Local:
                _lRot += rotation;
                break;
            case Space::World:
                transform(nullptr, &rotation, nullptr, Space::World);
                break;
        }
    }

    void CTransform::scale(const JVector2f& scale, Space space) {
        switch (space) {
            case Space::Local:
                _lSca *= scale;
                break;
            case Space::World:
                transform(nullptr, nullptr, &scale, Space::World);
                break;
        }
    }

    void CTransform::setPosition(const JVector2f& pos, Space space) {
        switch (space) {
            case Space::Local:
                _lPos = pos;
                break;
            case Space::World:
                update(Space::World, &pos, nullptr, nullptr);
                break;
        }
    }

    void CTransform::setRotation(float rot, Space space) {
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

    void CTransform::setScale(const JVector2f& scale, Space space) {
        switch (space) {
            case Space::Local:
                _lSca = scale;
                break;
            case Space::World:
                update(Space::World, nullptr, nullptr, &scale);
                break;
        }
    }

    CTransform* CTransform::getChildAt(const size_t index) {
        JENGINE_CORE_ASSERT(index >= 0 && index < _children.size() && "Index out of range!");
        return _children[index];
    }

    const CTransform* CTransform::getChildAt(const size_t index) const {
        JENGINE_CORE_ASSERT(index >= 0 && index < _children.size() && "Index out of range!");
        return _children[index];
    }

    CTransform* CTransform::find(const char* name) const {
        for (const CTransform* child : _children) {
            if (child->getObject() && strcmp(name, child->getObject()->getName().c_str())) { 
                return const_cast<CTransform*>(child); 
            }
        }
        return nullptr;
    }

    void CTransform::transform(const JVector2f* tra, const float* rot, const JVector2f* sca, Space space) {
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

    void CTransform::update(Space space, const JVector2f* pos, const float* rot, const JVector2f* scale) {
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

    CTransform* CTransform::addChild(CTransform* tr) {
        if (tr && tr != this && tr->_parent != this) {
            if (tr->_parent) {
                tr->_parent->removeChild(tr);
            }
            tr->_parent = this;
            _children.emplace_back(tr);
        }
        return tr;
    }

    bool CTransform::removeChild(CTransform* tr) {
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
            GameObject::destroyObject(_children[i]->getObject());
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
        _lRot = 0.0f;
        _lSca = { 1, 1 };
    }

    void CTransform::onDestroy() {
        Component::deleteComponent(this);
    }

    void CTransform::deserializeBinaryImpl(const Stream& stream) {
        UUID8 parent{ UUID8::Empty };
        UUID8* children{nullptr};
        uint32_t childCount{};

        Serialization::deserialize(parent, stream);
        Serialization::deserialize(_lPos, stream);
        Serialization::deserialize(_lRot, stream);
        Serialization::deserialize(_lSca, stream);

        Serialization::deserialize(childCount, stream);

        children = reinterpret_cast<UUID8*>(_malloca(childCount * sizeof(UUID8)));
        JENGINE_CORE_ASSERT(children, "Failed to allocate child UUID buffer!");
        stream.readValue(children, childCount, false);

        //TODO: Add child/parent deserialization

        _freea(children);
    }

    void CTransform::serializeBinaryImpl(const Stream& stream) const {
        UUID8 parent = _parent ? _parent->getObject()->getUUID() : UUID8::Empty;
        Serialization::serialize(parent, stream);
        Serialization::serialize(_lPos, stream);
        Serialization::serialize(_lRot, stream);
        Serialization::serialize(_lSca, stream);

        //TODO: Add child/parent serialization
    }

    void CTransform::deserializeYAMLImpl(yamlNode& node) {
        UUID8 parent{ UUID8::Empty };
        UUID8* children{ nullptr };
        uint32_t childCount{};

        Serialization::deserialize(parent, node["parent"]);
        Serialization::deserialize(_lPos, node["position"]);
        Serialization::deserialize(_lRot, node["rotation"]);
        Serialization::deserialize(_lSca, node["scale"]);

        //TODO: Add child/parent deserialization
    }

    void CTransform::serializeYAMLImpl(yamlEmit& emit) const {
        UUID8 parent = _parent ? _parent->getObject()->getUUID() : UUID8::Empty;
        emit << YAML::Key << "parent" << YAML::Value << parent;
        emit << YAML::Key << "position" << YAML::Value << _lPos;
        emit << YAML::Key << "rotation" << YAML::Value << _lRot;
        emit << YAML::Key << "scale" << YAML::Value << _lSca;

        //TODO: Add child/parent serialization
    }

    void CTransform::deserializeJSONImpl(json& jsonF) {
        UUID8 parent{ UUID8::Empty };
        UUID8* children{ nullptr };
        uint32_t childCount{};

        Serialization::deserialize(parent, jsonF["parent"]);
        Serialization::deserialize(_lPos, jsonF["position"]);
        Serialization::deserialize(_lRot, jsonF["rotation"]);
        Serialization::deserialize(_lSca, jsonF["scale"]);

        //TODO: Add child/parent deserialization
    }

    void CTransform::serializeJSONImpl(json& jsonF) const {
        UUID8 parent = _parent ? _parent->getObject()->getUUID() : UUID8::Empty;
        Serialization::serialize(parent, jsonF["parent"]);
        Serialization::serialize(_lPos, jsonF["position"]);
        Serialization::serialize(_lRot, jsonF["rotation"]);
        Serialization::serialize(_lSca, jsonF["scale"]);

        //TODO: Add child/parent serialization
    }
}