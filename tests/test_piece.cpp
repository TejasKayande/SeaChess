
#define NOMINMAX
#include <gtest/gtest.h>
#include "../source/core/piece.hpp"

using namespace Chess;

TEST(PieceTest, CreatePiece) {
    Piece p(PType::KNIGHT, PColor::LIGHT);

    EXPECT_EQ(p.type(), PType::KNIGHT);
    EXPECT_EQ(p.color(), PColor::LIGHT);
}

TEST(PieceTest, EmptyPiece) {
    Piece p;

    EXPECT_TRUE(p.isEmpty());
}

TEST(PieceTest, Equality) {
    Piece a(PType::ROOK, PColor::DARK);
    Piece b(PType::ROOK, PColor::DARK);
    Piece c(PType::BISHOP, PColor::DARK);

    EXPECT_EQ(a, b);
    EXPECT_NE(a, c);
}

TEST(PieceTest, Assignment) {
    Piece a(PType::QUEEN, PColor::LIGHT);
    Piece b = a;

    EXPECT_EQ(a, b);
}

TEST(PieceTest, ColorCheck) {
    Piece p(PType::QUEEN, PColor::LIGHT);

    EXPECT_TRUE(p.isColor(PColor::LIGHT));
    EXPECT_FALSE(p.isColor(PColor::DARK));
}

TEST(PieceTest, DefaultPiece) {
    Piece p = Piece::nopiece();

    EXPECT_TRUE(p.isEmpty());
}