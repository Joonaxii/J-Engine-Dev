#pragma once
#include <JEngine/IO/Serialization/Serializable.h>
#include <cstdint>
#include <chrono>
#include <string>

namespace JEngine {
    struct JTimeIndex {
    public:
        static constexpr uint8_t ENGINE_TIME_SPACE = 0x80;

        constexpr JTimeIndex() : _value(0x00) {}
        constexpr JTimeIndex(uint8_t index) : _value(index) {}

        constexpr uint8_t getIndex() const { return _value & 0x1F; }
        constexpr bool isEngine() const { return _value > 0x1F; }

        constexpr operator uint8_t() const { return _value; }
        constexpr operator int32_t() const { return _value; }
        constexpr operator uint32_t() const { return _value; }

        JTimeIndex& operator=(uint8_t value) {
            _value = value;
            return *this;
        }

        JTimeIndex& operator=(int32_t value) {
            _value = uint8_t(value);
            return *this;
        }

        JTimeIndex& operator=(uint32_t value) {
            _value = uint8_t(value);
            return *this; 
        }
    private:
        friend struct Serializable<JTimeIndex>;
        friend struct YAML::convert<JTimeIndex>;
         uint8_t _value;
    };

    class JTime {
    public:
        static constexpr uint8_t MAX_TIME_SPACES = 32;
        struct TimeSpace {
        public:
            TimeSpace() : _time(0), _timeScale(1.0), _deltaTime(0) {}

            template<typename P = float> 
            P getDeltaTime() const { return P(_deltaTime); }

            template<typename P = float>
            P getTime() const { return P(_time); }

            template<typename P = float>
            P getTimeScale() const { return P(_timeScale); }

            template<typename P = float>
            void setTimeScale(const P scale) { _timeScale = double(scale); }

            void reset() { _time = 0; _timeScale = 1.0; _deltaTime = 0; }

            template<typename P = float>
            void update(const P delta) {
                _deltaTime = delta * _timeScale;
                _time += _deltaTime;
            }

        private:

            double _time;
            double _timeScale;
            double _deltaTime;
        };

        JTime();

        void tick();
        void reset();

        template<typename P = float>
        P getEngineTime() const { return P(_time); }

        template<typename P = float>
        P getEngineDeltaTime() const { return P(_delta); }

        uint64_t getFrameCount() const;

        TimeSpace& getTimeSpace(uint8_t space);
        const TimeSpace& getTimeSpace(uint8_t space) const;

        template<typename P = float>
        P getDeltaTime(uint8_t space) const { return getTimeSpace(space).getDeltaTime<P>(); }

        template<typename P = float>
        P getTime(uint8_t space) const { return getTimeSpace(space).getTime<P>(); }

        template<typename P = float>
        P getTimeScale(uint8_t space) const { return getTimeSpace(space).getTimeScale<P>(); }

        template<typename P = float>
        void setTimeScale(uint8_t space, P scale) { return getTimeSpace(space).setTimeScale<P>(scale); }


        template<typename P = float>
        P getDeltaTime(JTimeIndex space) const { 
            return space.isEngine() ? getEngineDeltaTime<P>() : getTimeSpace(space.getIndex()).getDeltaTime<P>(); 
        }

        template<typename P = float>
        P getTime(JTimeIndex space) const {
            return space.isEngine() ? getEngineTime<P>() : getTimeSpace(space.getIndex()).getTime<P>(); 
        }

        template<typename P = float>
        P getTimeScale(JTimeIndex space) const {
            if (space.isEngine()) { return P(1); }
            return getTimeSpace(space.getIndex()).getTimeScale<P>();
        }

        template<typename P = float>
        void setTimeScale(JTimeIndex space, P scale) {
            if (space.isEngine()) { return; }
            return getTimeSpace(space.getIndex()).setTimeScale<P>(scale);
        }

    private:
        static std::string TIME_SPACE_NAMES[MAX_TIME_SPACES];

        TimeSpace _timeSpaces[MAX_TIME_SPACES];
        uint64_t _frameNum;
        std::chrono::high_resolution_clock _clock;
        double _delta;
        double _time;
        std::chrono::steady_clock::time_point _timeP;
    };
}


#pragma region Serialization
//YAML
namespace YAML {
    inline yamlEmit& operator<<(yamlEmit& yamlOut, const JEngine::JTimeIndex& itemRef) {
        yamlOut << YAML::Flow << YAML::Hex << uint8_t(itemRef);
        return yamlOut;
    }

    template<>
    struct convert<JEngine::JTimeIndex> {
        static Node encode(const JEngine::JTimeIndex& rhs) {
            Node node;
            node.push_back(rhs._value);
            return node;
        }

        static bool decode(const Node& node, JEngine::JTimeIndex& rhs) {
            if (node.size() < 1) { return false; }
            rhs._value = node.as<uint8_t>();
            return true;
        }
    };
}

//JSON
namespace JEngine {
    template<>
    inline bool Serializable<JTimeIndex>::deserializeJson(JTimeIndex& itemRef, const json& jsonF, const JTimeIndex& defaultValue) {
        Serialization::deserialize(itemRef._value, jsonF, defaultValue._value);
        return true;
    }

    template<>
    inline bool Serializable<JTimeIndex>::serializeJson(const JTimeIndex& itemRef, json& jsonF) {
        Serialization::serialize(itemRef._value, jsonF);
        return true;
    }
}
#pragma endregion