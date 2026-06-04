
#include "engine.hpp"

#include <random>

Move Engine::getBestMove(Chess::Board *board) {

    MoveList move_list;
    MoveGen::Legal::generateAllMoves(board, move_list);

    if (move_list.empty()) {
        // TODO(Tejas): we need to check if its a checkmate or stalemate here.
        return Move();
    }

    int rand_idx = rand() % move_list.size();

    return move_list[rand_idx];
}