#define NOMINMAX

#include <gtest/gtest.h>
#include "../source/core/square.hpp"

using namespace Chess;

TEST(SquareTest, InvalidConstruction) {
    Square sq(8, 0);

    EXPECT_FALSE(sq.isValid());
}

TEST(SquareTest, EqualityOperators) {
    Square a(2, 2);
    Square b(2, 2);
    Square c(3, 3);

    EXPECT_TRUE(a == b);
    EXPECT_TRUE(a != c);
}

TEST(SquareTest, AssignmentOperator) {
    Square a(2, 2);
    Square b = a;

    EXPECT_TRUE(a == b);
}

TEST(SquareTest, RankAndFile) {
    int rank = 4;
    int file = 6;
    Square a(rank, file);

    EXPECT_TRUE(a.rank() == rank);
    EXPECT_TRUE(a.file() == file);
}

TEST(SquareTest, IndexMapping) {
    Square sq(6, 2);

    u8 idx = sq.index;
    Square recovered = Square(idx);

    EXPECT_EQ(sq, recovered);
}

TEST(SquareTest, InvalidSquareStatic) {
    Square sq = Square::invalid();

    EXPECT_FALSE(sq.isValid());
}