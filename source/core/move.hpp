#pragma once

#include "base.hpp"
#include "piece.hpp"
#include "square.hpp"

#include <vector>

struct Move {

    Chess::Square from;
    Chess::Square to;

    // TODO(Tejas): if the move was castle then we need to know what type of
    //              castle it was...
    enum _MoveType : u8 {

        QUIET,
        CAPTURE,

        DOUBLE_PAWN_PUSH,

        EN_PASSANT,

        PROMO_KNIGHT,
        PROMO_BISHOP,
        PROMO_ROOK,
        PROMO_QUEEN,

        PROMO_CAPTURE_KNIGHT,
        PROMO_CAPTURE_BISHOP,
        PROMO_CAPTURE_ROOK,
        PROMO_CAPTURE_QUEEN,

        KING_CASTLE,
        QUEEN_CASTLE,
    } type;
};

using MoveType = Move::_MoveType;

// TODO(Tejas): Replace this with a custom MoveList, for we know that in any
//              given position there can be at most 218 legal moves.
//              So we dont really need a dynamic overhead of std::vector for this.
using MoveList = std::vector<Move>;