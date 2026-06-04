#pragma once

#include "../core/board.hpp"
#include "../core/move.hpp"
#include "../core/movegen.hpp"

namespace Engine {

    Move getBestMove(Chess::Board *board);

} // namespace Engine