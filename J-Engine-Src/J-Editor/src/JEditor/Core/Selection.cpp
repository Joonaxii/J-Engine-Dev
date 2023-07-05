#include <JEditor/Core/Selection.h>

namespace JEngine::Editor {
    Selection Selection::_globalSelect{};

    Selection::Selection() { }

    Selection& Selection::getGlobal() {
        return _globalSelect;
    }

    bool Selection::isSelected(IObject* obj) const  {
        return _selectionSet.find(obj) != _selectionSet.end();
    }

    bool Selection::addToSelection(IObject* obj) {
        if (_selectionSet.insert(obj).second) {
            _selection.push_back(obj);
            return true;
        }
        return false;
    }

    bool Selection::removeFromSelection(IObject* obj) {
        if (_selectionSet.erase(obj)) {
            _selection.erase(std::find(_selection.begin(), _selection.end(), obj));
            return true;
        }
        return false;
    }

    size_t Selection::addToSelection(IObject** objs, size_t count) {
        size_t added = 0;
        for (size_t i = 0; i < count; i++) {
            if (addToSelection(objs[i])) {
                added++; 
            }
        }
        return added;
    }

    size_t Selection::removeOFromSelection(IObject** objs, size_t count) {
        size_t removed = 0;
        for (size_t i = 0; i < count; i++) {
            if (removeFromSelection(objs[i])) {
                removed++;
            }
        }
        return removed;
    }

    void Selection::clear() {
        _selection.clear();
        _selectionSet.clear();
    }
}