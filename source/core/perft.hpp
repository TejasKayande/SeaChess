#pragma once

#include "board.hpp"
#include "movegen.hpp"

#include <chrono>

namespace PerfTest { // for perftest

    unsigned long perft(Chess::Board* board, int depth);
    void divide(Chess::Board* board, int depth);

    void runPerftest(int depth);
}
