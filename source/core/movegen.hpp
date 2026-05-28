#pragma once

#include "base.hpp"
#include "square.hpp"
#include "piece.hpp"
#include "board.hpp"
#include "move.hpp"
#include <vector>

namespace MoveGen {

    void init();

    namespace Attack {

        // TODO(Tejas): we can have a function that returns if a square is attacked.

        void initAttackTables();

        BitBoard getAllAttacks(Chess::Board board, Chess::Player player);

        BitBoard pawnAttacks(Chess::Square sq, Chess::Player p);
        BitBoard knightAttacks(Chess::Square sq);
        BitBoard bishopAttacks(Chess::Square sq, BitBoard occ);
        BitBoard rookAttacks(Chess::Square sq, BitBoard occ);
        BitBoard queenAttacks(Chess::Square sq, BitBoard occ);
        BitBoard kingAttacks(Chess::Square sq);
    }

    namespace PseudoLegal {

        void generateAllMoves(const Chess::Board *board, Chess::Player player, MoveList &move_list);
        void generateMovesForSquare(const Chess::Board *board, Chess::Square sq, MoveList &move_list);

        // NOTE(Tejas): generate moves for all the pieces on the board of that type.
        void generatePawnMoves(const Chess::Board *board, Chess::Player p, MoveList &move_list);
        void generateKnightMoves(const Chess::Board *board, Chess::Player p, MoveList &move_list);
        void generateBishopMoves(const Chess::Board *board, Chess::Player p, MoveList &move_list);
        void generateRookMoves(const Chess::Board *board, Chess::Player p, MoveList &move_list);
        void generateQueenMoves(const Chess::Board *board, Chess::Player p, MoveList &move_list);
        void generateKingMoves(const Chess::Board *board, Chess::Player p, MoveList &move_list);

        BitBoard convertMoveListToBitBoard(const MoveList &move_list);
    }

    namespace Legal {

        bool inCheck(const Chess::Board *board, Chess::Player p);

        void generateAllMoves(const Chess::Board *board, MoveList &move_list);
        void generateMovesForSquare(const Chess::Board *board, Chess::Square sq, MoveList &move_list);
    }

} // namespace MoveGen
