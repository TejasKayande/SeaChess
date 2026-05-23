#pragma once
#include "base.hpp"

namespace Chess {

    constexpr int PIECE_COUNT = 6;
    constexpr int COLOR_COUNT = 2;

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

        // NOTE(Tejas): Here we dont need to check if the color is NO_COLOR
        //              because we can just check that in Piece Type if the
        //              Piece is not a valid Piece
        enum _Color : u8 {
            LIGHT    = 0,
            DARK     = 1,
        };

        constexpr Piece() : code(0) {}
        constexpr Piece(_Type type, _Color color) : code (static_cast<u8>((color << 3) | type)) {}

        constexpr _Type type() const { return static_cast<_Type>(code & 0b111); }
        constexpr _Color color() const { return static_cast<_Color>((code >> 3) & 1); }

        constexpr bool isEmpty() const { return code == 0x0; }
        constexpr bool isColor(_Color c) const { return color() == c; }

        constexpr bool operator==(const Piece& other) const { return code == other.code; }
        constexpr bool operator!=(const Piece& other) const { return code != other.code; }

        constexpr static Piece nopiece() { return Piece(); }
    };

    using PType  = Piece::_Type;
    using PColor = Piece::_Color;
    using Player = PColor;

} // namespace Chess