#define NOMINMAX
#include <gtest/gtest.h>
#include "../source/core/board.hpp"

using namespace Chess;

TEST(FenTest, EmptyBoard) {
    Board b;

    b.setFen("8/8/8/8/8/8/8/8 w");

    for (int rank = 0; rank < 8; ++rank) {
        for (int file = 0; file < 8; ++file) {
            EXPECT_TRUE(b.getPieceAt(Square(rank, file)).isEmpty());
        }
    }

    EXPECT_EQ(b.getTurn(), Player::LIGHT);
}