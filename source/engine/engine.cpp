
#include "engine.hpp"
#include "../core/movegen.hpp"

#include <random>
#include <iostream>
#include <cstdlib>
#include <time.h>
#include <bit>

namespace {

    constexpr int PAWN_VALUE   = 100;
    constexpr int KNIGHT_VALUE = 300;
    constexpr int BISHOP_VALUE = 330;
    constexpr int ROOK_VALUE   = 500;
    constexpr int QUEEN_VALUE  = 900;

    constexpr int BISHOP_PAIR_BONUS = 30;

    // NOTE(Tejas): This assumes light is at the bottom, we have to flip the
    //              ranks to get the DARK_KNIGHT_PST.
    constexpr int KNIGHT_PST[64] = {
        -50,-40,-30,-30,-30,-30,-40,-50,
        -40,-20,  0,  0,  0,  0,-20,-40,
        -30,  0, 10, 15, 15, 10,  0,-30,
        -30,  5, 15, 20, 20, 15,  5,-30,
        -30,  0, 15, 20, 20, 15,  0,-30,
        -30,  5, 10, 15, 15, 10,  5,-30,
        -40,-20,  0,  5,  5,  0,-20,-40,
        -50,-40,-30,-30,-30,-30,-40,-50
    };

    constexpr int mirrorSquare(int sq_idx) {
        return sq_idx ^ 56;
    }

    int negamax(Chess::Board *board, int depth, int alpha, int beta) {

        if (depth == 0) return Engine::evaluate(board);

        MoveList move_list;
        MoveGen::Legal::generateAllMoves(board, move_list);

        if (move_list.empty()) {
            if (MoveGen::Legal::inCheck(board, board->getTurn()))
                return -100000;

            return 0; // stalemate
        }

        int best = -100000;

        for (Move move : move_list) {

            if (board->makeMove(move)) {

                int score = -negamax(board, depth - 1, -beta, -alpha);
                board->unMakeMove(move);

                best = std::max(best, score);
                alpha = std::max(alpha, score);

                if (alpha >= beta) break;
            }
        }

        return best;
    }

} // namespace Anonymous

int Engine::evaluate(Chess::Board *board) {

    int material = 0, pst = 0, bishop_pair = 0;

    const struct MaterialValue {
        Chess::PType type;
        int value;
    } pieces[] = {
        { Chess::Piece::PAWN,   PAWN_VALUE   },
        { Chess::Piece::KNIGHT, KNIGHT_VALUE },
        { Chess::Piece::BISHOP, BISHOP_VALUE },
        { Chess::Piece::ROOK,   ROOK_VALUE   },
        { Chess::Piece::QUEEN,  QUEEN_VALUE  }
    };

    // NOTE(Tejas): Count the material.
    for (auto entry : pieces) {

        BitBoard bb = board->getPiecesOfType(entry.type, Chess::Player::LIGHT);
        while (bb) {
            int sq = Base::popLSB(bb);
            material += entry.value;
            if (entry.type == Chess::Piece::KNIGHT) pst += KNIGHT_PST[sq];
        }

        bb = board->getPiecesOfType(entry.type, Chess::Player::DARK);
        while (bb) {
            int sq = Base::popLSB(bb);
            material -= entry.value;
            if (entry.type == Chess::Piece::KNIGHT) pst -= KNIGHT_PST[mirrorSquare(sq)];
        }
    }

    // NOTE(Tejas): A bishop pair is considered an advantage, so we will give a small bonus for it
    BitBoard whiteBishops = board->getPiecesOfType(Chess::Piece::BISHOP, Chess::Player::LIGHT);
    BitBoard blackBishops = board->getPiecesOfType(Chess::Piece::BISHOP, Chess::Player::DARK);

    // NOTE(Tejas): A player could have more than 2 bishops but I think its okay
    //              if we just award only the pair bonus.
    if (std::popcount(whiteBishops) >= 2) bishop_pair += BISHOP_PAIR_BONUS;
    if (std::popcount(blackBishops) >= 2) bishop_pair -= BISHOP_PAIR_BONUS;

    int score = material + pst + bishop_pair;

    return board->getTurn() == Chess::Player::LIGHT ? score : -score; 
}

Move Engine::getBestMove(Chess::Board *board) {

    int depth = 4;

    Move best_move;

    MoveList move_list;
    MoveGen::Legal::generateAllMoves(board, move_list);

    if (move_list.empty()) return best_move;

    int best_score = -100000;
    Chess::Board temp_board = *board;

    for (Move move : move_list) {

        if (temp_board.makeMove(move)) {

            int eval = -negamax(&temp_board, depth - 1, -100000, 100000);

            temp_board.unMakeMove(move);

            std::cout
                << "Move: "
                << move.from.toString()
                << " -> "
                << move.to.toString()
                << " Eval: "
                << eval
                << std::endl;

            if (eval > best_score) {
                best_score = eval;
                best_move = move;
            }
        }
    }

    return best_move;
}