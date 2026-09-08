#include "move_stack.hpp"

namespace Chess {

    void MoveStack::pushMove(const UndoContext& undo_ctx) {
        _history.push_back(undo_ctx);
    }

    bool MoveStack::popMove(UndoContext& out_undo_ctx) {
        if (_history.empty()) return false;
        out_undo_ctx = _history.back();
        _history.pop_back();
        return true;
    }

    bool MoveStack::peekMove(UndoContext& out_undo_ctx) const {
        if (_history.empty()) return false;
        out_undo_ctx = _history.back();
        return true;
    }

    bool MoveStack::isEmpty() const {
        return _history.empty();
    }

    size_t MoveStack::getSize() const {
        return _history.size();
    }

    void MoveStack::clearHistory() {
        _history.clear();
    }

    const std::vector<UndoContext>& MoveStack::getHistory() const {
        return _history;
    }

} // namespace Chess
