#pragma once

namespace JEngine {
    class UndoRedo {
    public:

    private:
        static UndoRedo _global;

        UndoRedo();
        ~UndoRedo();
    };
}