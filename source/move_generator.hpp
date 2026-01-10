#pragma once

#include "base.hpp"
#include "chess.hpp"

namespace MoveGenerator {

    struct Move {

        Chess::Square from;
        Chess::Square to;

        Chess::Piece moved;
        Chess::Piece captured;

        enum _MoveFlag : u8 {
            QUITE      = 0,
            CAPTURE    = 1 << 0,
            PROMOTION  = 1 << 1,
            EN_PASSANT = 1 << 2,
            CASTLE     = 1 << 3
        };

        u8 flag;
    };

    using MFlag = Move::_MoveFlag;
    
} // namespace MoveGenerator
