#pragma once
#include <JEngine/IO/Serialization/Serializable.h>

namespace JEngine {
    template<typename T>
    struct ImportSettings {
        virtual bool deserialize(const yamlNode& node) = 0;
        virtual bool serialize(yamlEmit& node) const = 0;

        virtual bool deserialize(const Stream& stream) = 0;
        virtual bool serialize(const Stream& stream) const = 0;

        virtual void reset() {}
        virtual void copyFrom(const T& other) { }
        virtual bool hasChanged(const T& other) const { return false; }
    };
}