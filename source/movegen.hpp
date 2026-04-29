#pragma once

#include "base.hpp"
#include "chess.hpp"
#include <vector>

// TODO(Tejas):
// - [ ] Seperate Attack, Pseudo-Legal and Legal move into seperate files.

namespace MoveGen {

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

    namespace PseudoLegal {

        void generateAllMoves(const Chess::Board *board, MoveList &moveList);

        void generatePawnMoves(const Chess::Board *board, MoveList &moveList);
        void generateKnightMoves(const Chess::Board *board, MoveList &moveList);
        void generateBishopMoves(const Chess::Board *board, MoveList &moveList);
        void generateRookMoves(const Chess::Board *board, MoveList &moveList);
        void generateQueenMoves(const Chess::Board *board, MoveList &moveList);
        void generateKingMoves(const Chess::Board *board, MoveList &moveList);
    }
    
} // namespace MoveGen
