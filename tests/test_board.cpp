#define NOMINMAX
#include <gtest/gtest.h>
#include "../source/core/board.hpp"

using namespace Chess;

TEST(BoardTest, SetAndGetPiece) {
    Board b;

    Square sq(3, 3);
    Piece p(PType::QUEEN, PColor::LIGHT);

    b.setPieceAt(sq, p);

    EXPECT_EQ(b.getPieceAt(sq), p);
}

TEST(BoardTest, RemovePiece) {
    Board b;

    Square sq(4, 4);
    Piece p(PType::BISHOP, PColor::DARK);

    b.setPieceAt(sq, p);
    b.removePieceAt(sq);

    EXPECT_TRUE(b.getPieceAt(sq).isEmpty());
}

TEST(BoardTest, TurnSwitching) {
    Board b;

    EXPECT_EQ(b.getTurn(), Player::LIGHT);

    b.changeTurn();
    EXPECT_EQ(b.getTurn(), Player::DARK);

    b.changeTurn();
    EXPECT_EQ(b.getTurn(), Player::LIGHT);
}

TEST(BoardTest, MovePiece) {
    Board b;
    b.reset();

    Square from(1, 0);
    Square to(2, 0);

    Piece p = b.getPieceAt(from);

    b.makeMove({from, to, MoveType::QUIET});

    EXPECT_TRUE(b.getPieceAt(from).isEmpty());
    EXPECT_EQ(b.getPieceAt(to), p);
}

TEST(BoardTest, ResetInitialPosition) {
    Board b;
    b.reset();

    // Light pieces (rank 0)
    EXPECT_EQ(b.getPieceTypeAt(Square(0, 0)), PType::ROOK);
    EXPECT_EQ(b.getPieceTypeAt(Square(0, 1)), PType::KNIGHT);
    EXPECT_EQ(b.getPieceTypeAt(Square(0, 2)), PType::BISHOP);
    EXPECT_EQ(b.getPieceTypeAt(Square(0, 3)), PType::KING);
    EXPECT_EQ(b.getPieceTypeAt(Square(0, 4)), PType::QUEEN);
    EXPECT_EQ(b.getPieceTypeAt(Square(0, 5)), PType::BISHOP);
    EXPECT_EQ(b.getPieceTypeAt(Square(0, 6)), PType::KNIGHT);
    EXPECT_EQ(b.getPieceTypeAt(Square(0, 7)), PType::ROOK);

    // Dark pieces (rank 7)
    EXPECT_EQ(b.getPieceTypeAt(Square(7, 0)), PType::ROOK);
    EXPECT_EQ(b.getPieceTypeAt(Square(7, 1)), PType::KNIGHT);
    EXPECT_EQ(b.getPieceTypeAt(Square(7, 2)), PType::BISHOP);
    EXPECT_EQ(b.getPieceTypeAt(Square(7, 3)), PType::KING);
    EXPECT_EQ(b.getPieceTypeAt(Square(7, 4)), PType::QUEEN);
    EXPECT_EQ(b.getPieceTypeAt(Square(7, 5)), PType::BISHOP);
    EXPECT_EQ(b.getPieceTypeAt(Square(7, 6)), PType::KNIGHT);
    EXPECT_EQ(b.getPieceTypeAt(Square(7, 7)), PType::ROOK);
}