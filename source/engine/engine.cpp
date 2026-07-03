
#include "engine.hpp"
#include "../core/movegen.hpp"

#include <random>
#include <iostream>
#include <cstdlib>
#include <time.h>
#include <bit>
#include <chrono>

namespace {

    struct SearchContext {

        std::chrono::steady_clock::time_point start;
        int time_limit_ms;

        bool stop = false;

        u64 nodes = 0;
        Move bestMove;
    };

    constexpr int PAWN_VALUE   = 100;
    constexpr int KNIGHT_VALUE = 300;
    constexpr int BISHOP_VALUE = 330;
    constexpr int ROOK_VALUE   = 500;
    constexpr int QUEEN_VALUE  = 900;

    constexpr int BISHOP_PAIR_BONUS = 30;

    constexpr int CASTLED_BONUS = 170;
    constexpr int CENTER_KING_PENALTY = 25;

    // NOTE(Tejas): This assumes light is at the bottom, we have to flip the
    //              ranks to get the DARK_KNIGHT_PST.
    constexpr int BISHOP_PST[64] = {
        -20,-10,-10,-10,-10,-10,-10,-20,
        -10,  5,  0,  0,  0,  0,  5,-10,
        -10, 10, 10, 10, 10, 10, 10,-10,
        -10,  0, 10, 10, 10, 10,  0,-10,
        -10,  5,  5, 10, 10,  5,  5,-10,
        -10,  0,  5, 10, 10,  5,  0,-10,
        -10,  0,  0,  0,  0,  0,  0,-10,
        -20,-10,-10,-10,-10,-10,-10,-20
    };

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

    constexpr int PAWN_PST[64] = {
         0,   0,   0,   0,   0,   0,   0,   0,
        50,  50,  50,  50,  50,  50,  50,  50,
        10,  10,  20,  30,  30,  20,  10,  10,
         5,   5,  10,  25,  25,  10,   5,   5,
         0,   0,   0,  20,  20,   0,   0,   0,
         5,  -5, -10,   0,   0, -10,  -5,   5,
         5,  10,  10, -20, -20,  10,  10,   5,
         0,   0,   0,   0,   0,   0,   0,   0
    };

    constexpr int mirrorSquare(int sq_idx) {
        return sq_idx ^ 56;
    }

    bool isCastled(int kingSq, Chess::Player side) {

        // TODO(Tejas): maybe add constants for these in square.hpp?
        const Chess::Square G1 = Chess::Square(0, 1);
        const Chess::Square C1 = Chess::Square(0, 5);
        const Chess::Square G8 = Chess::Square(7, 1);
        const Chess::Square C8 = Chess::Square(7, 5);

        if (side == Chess::Player::LIGHT)
            return G1 == kingSq || C1 == kingSq;

        return G8 == kingSq || C8 == kingSq;
    }

    bool isCenterKing(int kingSq) {

        // TODO(Tejas): maybe add constants for these in square.hpp?
        const Chess::Square E1 = Chess::Square(0, 3);
        const Chess::Square E8 = Chess::Square(7, 3);
        const Chess::Square D1 = Chess::Square(0, 4);
        const Chess::Square D8 = Chess::Square(7, 4);
        return E1 == kingSq  || D1 == kingSq  || E8 == kingSq  || D8 == kingSq;
    }

    int negamax(Chess::Board *board, int depth, int play, int alpha, int beta, SearchContext& search_context) {

        if (search_context.stop) return 0;

        search_context.nodes++;

        if ((search_context.nodes & 2047) == 0) {

            auto elapsed =
                std::chrono::duration_cast<std::chrono::milliseconds>(
                    std::chrono::steady_clock::now() - search_context.start
                );

            if (elapsed.count() >= search_context.time_limit_ms) {
                search_context.stop = true;
                return 0;
            }
        }

        if (depth == 0) return Engine::evaluate(board);

        MoveList move_list;
        MoveGen::Legal::generateAllMoves(board, move_list);

        if (move_list.empty()) {
            if (MoveGen::Legal::inCheck(board, board->getTurn()))
                return -100000 + play;

            return 0; // stalemate
        }

        int best = -100000;

        for (Move move : move_list) {

            Chess::Board temp_board = *board;

            if (temp_board.makeMove(move)) {

                int score = -negamax(&temp_board, depth - 1, play + 1, -beta, -alpha, search_context);
                // board->unMakeMove(move);

                if (search_context.stop) return 0;

                best = std::max(best, score);
                alpha = std::max(alpha, score);

                if (alpha >= beta) break;
            }
        }

        return best;
    }

} // namespace Anonymous

int Engine::evaluate(Chess::Board *board) {

    int material = 0, pst = 0, bishop_pair = 0, king_safety = 0;

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
            if (entry.type == Chess::Piece::BISHOP) pst += BISHOP_PST[sq];
            if (entry.type == Chess::Piece::KNIGHT) pst += KNIGHT_PST[sq];
            if (entry.type == Chess::Piece::PAWN)   pst += PAWN_PST[sq];
        }

        bb = board->getPiecesOfType(entry.type, Chess::Player::DARK);
        while (bb) {
            int sq = Base::popLSB(bb);
            material -= entry.value;
            if (entry.type == Chess::Piece::BISHOP) pst -= BISHOP_PST[mirrorSquare(sq)];
            if (entry.type == Chess::Piece::KNIGHT) pst -= KNIGHT_PST[mirrorSquare(sq)];
            if (entry.type == Chess::Piece::PAWN)   pst -= PAWN_PST[mirrorSquare(sq)];
        }
    }

    // NOTE(Tejas): A bishop pair is considered an advantage, so we will give a small bonus for it
    BitBoard whiteBishops = board->getPiecesOfType(Chess::Piece::BISHOP, Chess::Player::LIGHT);
    BitBoard blackBishops = board->getPiecesOfType(Chess::Piece::BISHOP, Chess::Player::DARK);

    // NOTE(Tejas): A player could have more than 2 bishops but I think its okay
    //              if we just award only the pair bonus.
    if (std::popcount(whiteBishops) >= 2) bishop_pair += BISHOP_PAIR_BONUS;
    if (std::popcount(blackBishops) >= 2) bishop_pair -= BISHOP_PAIR_BONUS;

    // White king
    BitBoard whiteKing =
        board->getPiecesOfType(Chess::Piece::KING, Chess::Player::LIGHT);

    int whiteKingSq = Base::popLSB(whiteKing);

    if (isCastled(whiteKingSq, Chess::Player::LIGHT))
        king_safety += CASTLED_BONUS;
    else if (isCenterKing(whiteKingSq))
        king_safety -= CENTER_KING_PENALTY;


    // Black king
    BitBoard blackKing =
        board->getPiecesOfType(Chess::Piece::KING, Chess::Player::DARK);

    int blackKingSq = Base::popLSB(blackKing);

    if (isCastled(blackKingSq, Chess::Player::DARK))
        king_safety -= CASTLED_BONUS;
    else if (isCenterKing(blackKingSq))
        king_safety += CENTER_KING_PENALTY;

    int score = material + pst + bishop_pair + king_safety;

    return board->getTurn() == Chess::Player::LIGHT ? score : -score; 
}

Move Engine::getBestMove(Chess::Board *board) {

    int depth = 5;

    SearchContext search_context;
    search_context.start = std::chrono::steady_clock::now();
    search_context.time_limit_ms = std::numeric_limits<int>::max();

    Move best_move;

    MoveList move_list;
    MoveGen::Legal::generateAllMoves(board, move_list);

    if (move_list.empty()) return best_move;

    int best_score = -100000;
    // Chess::Board temp_board = *board;

    for (Move move : move_list) {

        Chess::Board temp_board = *board;
        if (temp_board.makeMove(move)) {

            int eval = -negamax(&temp_board, depth - 1, 1, -100000, 100000, search_context);

            // temp_board.unMakeMove(move);

            if (eval > best_score) {
                best_score = eval;
                best_move = move;
            }
        }
    }

    return best_move;
}

Move Engine::searchTimed(Chess::Board *board, int time_ms) {

    SearchContext search_context;
    search_context.start = std::chrono::steady_clock::now();
    search_context.time_limit_ms = time_ms;


     Move bestMove;

    for (int depth = 1; !search_context.stop; depth++) {

        Move currentBest;
        int bestScore = -100000;

        MoveList moveList;
        MoveGen::Legal::generateAllMoves(board, moveList);

        if (moveList.empty())
            break;

        for (Move move : moveList) {

            Chess::Board temp = *board;
            if (!temp.makeMove(move)) continue;

            int score = -negamax(&temp, depth - 1, 1, -100000, 100000, search_context);

            if (search_context.stop)
                break;

            if (score > bestScore) {
                bestScore = score;
                currentBest = move;
            }
        }

        if (!search_context.stop) {

            bestMove = currentBest;

            std::cout
                << "info depth " << depth
                << " score cp " << bestScore
                << std::endl;
        }
    }

    return bestMove;
}