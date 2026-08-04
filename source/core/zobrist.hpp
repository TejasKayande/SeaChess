#pragma once

#include "base.hpp"
#include "piece.hpp"

namespace Chess {
    namespace Zobrist {
    
        extern u64 pieceKeys[16][64]; // Indexed by piece.code and square index
        extern u64 enPassantKeys[8];  // Indexed by file (0-7)
        extern u64 castlingKeys[16];  // Indexed by castling rights (0-15)
        extern u64 sideToMoveKey;     // XORed if it's black's turn to move
    
        void init();
    
    } // namespace Zobrist
} // namespace Chess
