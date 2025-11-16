#pragma once

#include "base.hpp"

using BitBoard = u64;
#define U64(x) (x##ULL)

#define GET_INDEX_FROM_SQUARE(rank, file) (((rank) << 3) | (file))
#define GET_SQUARE_FROM_INDEX(idx) Square((idx) >> 3, (idx) & 7)

namespace Chess {

    constexpr int MAX_FILE = 8;
    constexpr int MAX_RANK = 8;

    constexpr int PIECE_COUNT = 6;
    constexpr int COLOR_COUNT = 2;

    struct Square {

        u8 index; 

        constexpr Square() : index(0) {}
        constexpr Square(int rank, int file) : index((rank << 3) | file) {}

        constexpr u8 rank() const { return index >> 3; }
        constexpr u8 file() const { return (index & 7); }

        constexpr bool operator==(const Square &other) const { return (index == other.index); }
        constexpr bool operator!=(const Square &other) const { return (index != other.index); }
    };

    struct Piece {

        u8 code;

        enum _Type : u8 {
            NO_PIECE = 0,
            PAWN     = 1,
            KNIGHT   = 2,
            BISHOP   = 3,
            ROOK     = 4,
            QUEEN    = 5,
            KING     = 6,
        };

        // NOTE(Tejas): Here the bit manipulation is easy when we have 0 and 1 as valid colors
        enum _Color : u8 {
            LIGHT    = 0,
            DARK     = 1,
            NO_COLOR = 2
        };

        constexpr Piece() : code(0) {}
        constexpr Piece(_Type type, _Color color) : code (static_cast<u8>((color << 3) | type)) {}

        constexpr _Type type() const { return static_cast<_Type>(code & 0b111); }
        constexpr _Color color() const { return static_cast<_Color>((code >> 3) & 1); }

        constexpr bool isEmpty() const { return code == 0x0; }
        constexpr bool isColor(_Color c) const { return color() == c; }

        constexpr bool operator==(const Piece& other) const { return code == other.code; }
        constexpr bool operator!=(const Piece& other) const { return code != other.code; }
    };

    using PType  = Piece::_Type;
    using PColor = Piece::_Color;
    using Player = PColor;

    class Board {

    public:

        Board();    
        ~Board() = default;

        void setFen(const std::string& fen);
        std::string getFen() const;

        Piece getPieceAt(const Square& sq) const;
        PType getPieceTypeAt(const Square& sq) const;
        PColor getPieceColorAt(const Square& sq) const;

        void setPieceAt(const Square& sq, const Piece& piece);
        void removePieceAt(const Square& sq);

        Player getTurn() const;
        void changeTurn();

        void reset();

    private:

        BitBoard _lPawn, _lKnight, _lBishop, _lRook, _lQueen, _lKing;
        BitBoard _dPawn, _dKnight, _dBishop, _dRook, _dQueen, _dKing; 

        BitBoard _lOccupied;
        BitBoard _dOccupied;

        Player _turn;

    private:

        void _updateOccupancy();
    };

} // namespace Chess
