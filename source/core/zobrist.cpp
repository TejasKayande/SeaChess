#include "zobrist.hpp"
#include <random>

namespace Chess {
    namespace Zobrist {

        u64 pieceKeys[16][64];
        u64 enPassantKeys[8];
        u64 castlingKeys[16];
        u64 sideToMoveKey;

        void init() {
            std::mt19937_64 rng(123456789ULL);

            for (int p = 0; p < 16; ++p) {
                for (int sq = 0; sq < 64; ++sq) {
                    pieceKeys[p][sq] = rng();
                }
            }

            for (int i = 0; i < 8; ++i) enPassantKeys[i] = rng();
            for (int i = 0; i < 16; ++i) castlingKeys[i] = rng();

            sideToMoveKey = rng();
        }

    } // namespace Zobrist
} // namespace Chess
