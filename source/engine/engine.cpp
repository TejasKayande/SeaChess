
#include "engine.hpp"

#include <random>
#include <iostream>
#include <cstdlib>
#include <time.h>

namespace {

    constexpr int PAWN_VALUE   = 100;
    constexpr int KNIGHT_VALUE = 300;
    constexpr int BISHOP_VALUE = 330;
    constexpr int ROOK_VALUE   = 500;
    constexpr int QUEEN_VALUE  = 900;

    int getMaterialCountForPiece(Chess::Board *board, Chess::Piece piece) {

        BitBoard pieces_bb = board->getPiecesOfType(piece.type(), piece.color());
        if (pieces_bb == 0) return 0;

        int count = 0;
        while (pieces_bb) {
            Base::popLSB(pieces_bb);
            switch (piece.type()) {
                case Chess::Piece::PAWN:   count += PAWN_VALUE; break;
                case Chess::Piece::KNIGHT: count += KNIGHT_VALUE; break;
                case Chess::Piece::BISHOP: count += BISHOP_VALUE; break;
                case Chess::Piece::ROOK:   count += ROOK_VALUE; break;
                case Chess::Piece::QUEEN:  count += QUEEN_VALUE; break;
                default: break;
            }
        }

        return count;
    }

} // namespace Anonymous

int Engine::evaluate(Chess::Board *board) {

    int score = 0;

    score += getMaterialCountForPiece(board, Chess::Piece(Chess::Piece::PAWN, Chess::Player::LIGHT));
    score += getMaterialCountForPiece(board, Chess::Piece(Chess::Piece::KNIGHT, Chess::Player::LIGHT));
    score += getMaterialCountForPiece(board, Chess::Piece(Chess::Piece::BISHOP, Chess::Player::LIGHT));
    score += getMaterialCountForPiece(board, Chess::Piece(Chess::Piece::ROOK, Chess::Player::LIGHT));
    score += getMaterialCountForPiece(board, Chess::Piece(Chess::Piece::QUEEN, Chess::Player::LIGHT));

    score -= getMaterialCountForPiece(board, Chess::Piece(Chess::Piece::PAWN, Chess::Player::DARK));
    score -= getMaterialCountForPiece(board, Chess::Piece(Chess::Piece::KNIGHT, Chess::Player::DARK));
    score -= getMaterialCountForPiece(board, Chess::Piece(Chess::Piece::BISHOP, Chess::Player::DARK));
    score -= getMaterialCountForPiece(board, Chess::Piece(Chess::Piece::ROOK, Chess::Player::DARK));
    score -= getMaterialCountForPiece(board, Chess::Piece(Chess::Piece::QUEEN, Chess::Player::DARK));

    return score; 
}

Move Engine::getBestMove(Chess::Board *board) {

    MoveList move_list;
    MoveGen::Legal::generateAllMoves(board, move_list);

    if (move_list.empty()) {
        return Move();
    }

    srand(time(NULL));
    int rand_idx = rand() % move_list.size();

    std::cout << "Eval for " << (int)board->getTurn() << ": " << evaluate(board) << std::endl;
    return move_list[rand_idx];
}
