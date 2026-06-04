#pragma once

#include "../core/board.hpp"
#include "../core/move.hpp"
#include "../core/movegen.hpp"

namespace Engine {

    int evaluate(Chess::Board *board);

    Move getBestMove(Chess::Board *board);

} // namespace Engine