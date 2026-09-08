#pragma once

#include "base.hpp"
#include "move.hpp"
#include "square.hpp"
#include <vector>

namespace Chess {

    struct UndoContext {
        Move move;
        u8 castling_rights;
        Square en_passant_target;
    };

    class MoveStack {
    public:
        MoveStack() = default;
        ~MoveStack() = default;

        void pushMove(const UndoContext& undo_ctx);
        bool popMove(UndoContext& out_undo_ctx);
        bool peekMove(UndoContext& out_undo_ctx) const;

        bool isEmpty() const;
        size_t getSize() const;
        void clearHistory();

        const std::vector<UndoContext>& getHistory() const;

    private:
        std::vector<UndoContext> _history;
    };

} // namespace Chess
