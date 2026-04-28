#pragma once

#include "base.hpp"
#include "chess.hpp"

namespace MoveGen {

    // struct Move {

    //     Chess::Square from;
    //     Chess::Square to;

    //     Chess::Piece moved;
    //     Chess::Piece captured;

    //     // TODO(Tejas): if the move was castle then we need to know what type of
    //     //              castle it was...
    //     enum _MoveFlag : u8 {
    //         QUIET      = 0,
    //         CAPTURE    = 1 << 0,
    //         PROMOTION  = 1 << 1,
    //         EN_PASSANT = 1 << 2,
    //         CASTLE     = 1 << 3
    //     };
    //     u8 flag;

    //     // TODO(Tejas): Work in progress...
    //     union {
    //         Chess::Piece  promoteTo;       // only used if the move is a promotion
    //         Chess::Square castleRookFrom, castleRookTo; // only used if the move is a castle
    //         Chess::Square enPassantCaptureSquare; // only used if the move is an en passant capture
    //     };
    // };

    // using MFlag = Move::_MoveFlag;

    void init();

    namespace Attack {

        void initAttackTables();

        BitBoard pawnAttacks(Chess::Square sq, Chess::Player p);
        BitBoard knightAttacks(Chess::Square sq);
        BitBoard bishopAttacks(Chess::Square sq, BitBoard occ);
        BitBoard rookAttacks(Chess::Square sq, BitBoard occ);
        BitBoard queenAttacks(Chess::Square sq, BitBoard occ);
        BitBoard kingAttacks(Chess::Square sq);
    }
    
} // namespace MoveGenerator
