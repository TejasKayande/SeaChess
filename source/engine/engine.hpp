#pragma once

#include "../core/board.hpp"
#include "../core/move.hpp"
#include "../core/movegen.hpp"
#include "tt.hpp"

namespace Engine {

    extern TranspositionTable g_tt;

    int evaluate(Chess::Board *board);

    Move getBestMove(Chess::Board *board, int depth);
    Move searchTimed(Chess::Board *board, int time_ms);

} // namespace Engine