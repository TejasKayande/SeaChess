#define NOMINMAX
#include <gtest/gtest.h>
#include "../source/chess.hpp"

#undef internal
#undef global
#undef persist

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

TEST(FenTest, SinglePiece) {
    Board b;

    b.setFen("8/8/8/8/8/8/8/K7 w");

    EXPECT_EQ(b.getPieceTypeAt(Square(0, 0)), PType::KING);
    EXPECT_EQ(b.getPieceColorAt(Square(0, 0)), PColor::LIGHT);

    EXPECT_TRUE(b.getPieceAt(Square(0, 1)).isEmpty());
}

TEST(FenTest, MixedPieces) {
    Board b;

    b.setFen("8/8/3k4/8/4Q3/8/8/8 b");

    EXPECT_EQ(b.getPieceTypeAt(Square(5, 3)), PType::KING);
    EXPECT_EQ(b.getPieceColorAt(Square(5, 3)), PColor::DARK);

    EXPECT_EQ(b.getPieceTypeAt(Square(3, 4)), PType::QUEEN);
    EXPECT_EQ(b.getPieceColorAt(Square(3, 4)), PColor::LIGHT);

    EXPECT_EQ(b.getTurn(), Player::DARK);
}

TEST(FenTest, DigitSkipping) {
    Board b;

    b.setFen("8/8/8/3p4/8/8/8/8 w");

    EXPECT_EQ(b.getPieceTypeAt(Square(4, 3)), PType::PAWN);
    EXPECT_EQ(b.getPieceColorAt(Square(4, 3)), PColor::DARK);

    // ensure neighbors are empty
    EXPECT_TRUE(b.getPieceAt(Square(4, 2)).isEmpty());
    EXPECT_TRUE(b.getPieceAt(Square(4, 4)).isEmpty());
}

TEST(FenTest, TurnParsing) {
    Board b;

    b.setFen("8/8/8/8/8/8/8/8 b");
    EXPECT_EQ(b.getTurn(), Player::DARK);

    b.setFen("8/8/8/8/8/8/8/8 w");
    EXPECT_EQ(b.getTurn(), Player::LIGHT);
}

TEST(FenTest, RoundTripSimple) {
    Board b;

    std::string fen = "8/8/8/8/8/8/8/K7 w";

    b.setFen(fen);
    std::string out = b.getFen();

    EXPECT_TRUE(out.find("K7") != std::string::npos);
}

TEST(FenTest, RoundTripFullPosition) {
    Board b;

    std::string fen = "rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w";

    b.setFen(fen);
    std::string out = b.getFen();

    EXPECT_TRUE(out.find("rnbqkbnr") != std::string::npos);
    EXPECT_TRUE(out.find("PPPPPPPP") != std::string::npos);
}

TEST(FenTest, IgnoresInvalidCharacters) {
    Board b;

    b.setFen("8/8/8/8/8/8/8/X7 w");

    // X should be ignored square empty
    for (int rank = 0; rank < 8; ++rank) {
        for (int file = 0; file < 8; ++file) {
            EXPECT_TRUE(b.getPieceAt(Square(rank, file)).isEmpty());
        }
    }
}