#pragma once
#include <functional>
#include <vector>
#include <cstdint>

namespace JEngine {
    template<typename... TIn>
    class Action {
    public:
        using Func = std::function<void(TIn...)>;

        Action() : _id(0), _events {} {}
        ~Action() {
            _events.clear();
        }

        const uint64_t add(const Func& eventIn) {
            _events.push_back({ _id, eventIn });
            return _id++;
        }

        const bool remove(const uint64_t id) {
            bool found = false;
            _events.erase(std::remove_if(_events.begin(), _events.end(), [id, &found](const FuncBind& b)
                {
                    if (b.id == id) {
                        found = true;
                        return true;
                    }
                    return false;
                }), _events.end());
            return found;
        }

        //Action& operator+= (Func& eventIn) {
        //    _events.push_back(&eventIn);
        //    return *this;
        //}

        //Action& operator-= (Func& eventIn) {
        //    auto find = std::find(_events.begin(), _events.end(), eventIn);
        //    if (find != _events.end()) {
        //        _events.erase(find);
        //    }
        //    return *this;
        //}

        Action& operator()(TIn&... input) {
            for (size_t i = 0; i < _events.size(); i++) {
                _events[i](input...);
            }
            return *this;
        }
    private:
        uint64_t _id;
        struct FuncBind {
            uint64_t id;
            Func func;

            FuncBind(const uint64_t id, const Func& func) : id(id), func(func) {  }

            FuncBind& operator()(TIn&... input) {
                func(input...);
                return *this;
            }
        };

        std::vector<FuncBind> _events;
    };
}