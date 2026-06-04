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

        NONE = 0,

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

    Chess::Piece captured_piece = Chess::Piece::nopiece(); // for unmaking moves

    Move() : from(Chess::Square::invalid()), 
             to(Chess::Square::invalid()), 
             type(NONE), 
             captured_piece(Chess::Piece::nopiece()) {}

    Move(Chess::Square f, Chess::Square t, _MoveType mt, Chess::Piece captured = Chess::Piece::nopiece()) 
        : from(f), to(t), type(mt), captured_piece(captured) {}
};

using MoveType = Move::_MoveType;

// TODO(Tejas): Replace this with a custom MoveList, for we know that in any
//              given position there can be at most 218 legal moves.
//              So we dont really need a dynamic overhead of std::vector for this.
using MoveList = std::vector<Move>;
