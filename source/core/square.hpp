#pragma once
#include "base.hpp"

using BitBoard = u64;

#define GET_INDEX_FROM_SQUARE(rank, file) (((rank) << 3) | (file))

namespace Chess {

    struct Square {

        // NOTE(Tejas): For now lets hard code the board to be 8x8
        u8 index; 

        constexpr static int INVALID = 64;

        constexpr Square() : index(INVALID) {}
        constexpr Square(u8 idx) : index((idx < 64) ? idx : INVALID) {}
        constexpr Square(u8 rank, u8 file)
            : index((rank < 8 && file < 8) ? ((rank << 3) | file) : INVALID) {}

        constexpr u8 rank() const { return index >> 3; }
        constexpr u8 file() const { return (index & 7); }

        constexpr bool operator==(const Square &other) const { return (index == other.index); }
        constexpr bool operator!=(const Square &other) const { return (index != other.index); }

        constexpr bool isSquareOnBitBoard(BitBoard bb)  const { return (bb & (1ULL << index)) != 0; }

        constexpr bool isValid() const { return index < INVALID; }
        constexpr static Square invalid() { return Square(INVALID, INVALID); }
    };

} // namespace Chess