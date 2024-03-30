#pragma once
#include <vector>
#include <unordered_set>
#include <JEngine/Core/IObject.h>
#include <JEditor/JEditor.h>

namespace JEngine::Editor {
    class Selection {
    public:
        Selection();

        static Selection& getGlobal();

        bool isSelected(IObject* obj) const;

        bool addToSelection(IObject* obj);
        bool removeFromSelection(IObject* obj);

        size_t addToSelection(IObject** objs, size_t count);
        size_t removeOFromSelection(IObject** objs, size_t count);

        template<size_t bufSize>
        void addObjects(IObject* (&objs)[bufSize]) {
            addObjects(objs, bufSize);
        }

        template<size_t bufSize>
        size_t removeObjects(IObject* (&objs)[bufSize]) {
            return removeObjects(objs, bufSize);
        }

        template<typename T>
        size_t getAllOfType(std::vector<T*>& objs) const {
            size_t count = 0;
            for (const IObject* obj : _selection) {
                const T* type = dynamic_cast<const T*>(obj);
                if (type) {
                    count++;
                    objs.push_back(const_cast<T*>(type));
                }
            }
            return count;
        }

        template<typename T>
        size_t getAllOfType(T** buffer, size_t maxCount) const {
            size_t count = 0;
            for (const IObject* obj : _selection) {
                const T* type = dynamic_cast<const T*>(obj);
                if (type) {
                    buffer[count++] = const_cast<T*>(type);
                    if (count >= maxCount) { break; }
                }
            }
            return count;
        }


        template<typename T, size_t bufSize>
        size_t getAllOfType(IObject* (&objs)[bufSize]) const {
            return getAllOfType(objs, bufSize);
        }

        void clear();

    private:
        static Selection _globalSelect;

        std::unordered_set<IObject*> _selectionSet;
        std::vector<IObject*> _selection;
    };
}