 
#include <gtest/gtest.h>
#include "../source/core/board.hpp"
#include "../source/core/movegen.hpp"

#include <iostream>

using namespace Chess;
using namespace MoveGen;

unsigned long perftest(Chess::Board *board, int depth) {

    if (depth == 0) return 1;

    MoveList move_list;
    MoveGen::Legal::generateAllMoves(board, move_list);

    long nodes = 0;

    for (const Move &move : move_list) {
        Chess::Board copy = *board;
        if (copy.makeMove(move)) {
            copy.changeTurn();
            nodes += perftest(&copy, depth - 1);
        }
    }

    return nodes;
}


TEST(MoveGen, PerftTest) {

    MoveGen::init();

    unsigned long result = perftest(new Chess::Board(), 1);
    EXPECT_EQ(result, 20);

    result = perftest(new Chess::Board(), 2);
    EXPECT_EQ(result, 400);

    result = perftest(new Chess::Board(), 3);
    EXPECT_EQ(result, 8902);

    result = perftest(new Chess::Board(), 4);
    EXPECT_EQ(result, 197281);

    result = perftest(new Chess::Board(), 5);
    EXPECT_EQ(result, 4865609);
}