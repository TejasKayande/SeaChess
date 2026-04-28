
#include "movegen.hpp"

using namespace MoveGen;

internal BitBoard PAWN_ATTACKS[Chess::COLOR_COUNT][64];
internal BitBoard KNIGHT_ATTACKS[64];
internal BitBoard KING_ATTACKS[64];

void MoveGen::init() {

    Attack::initAttackTables();
}

void Attack::initAttackTables() {

    for (int sqIdx = 0; sqIdx < 64; sqIdx++) {

        Chess::Square sq = GET_SQUARE_FROM_INDEX(sqIdx);

        int rank = sq.rank();
        int file = sq.file();

        // NOTE(Tejas): Knight Attacks
        {
            BitBoard mask = 0ULL;

            const int dr[8] = { +2, +2, +1, +1, -1, -1, -2, -2 };
            const int df[8] = { +1, -1, +2, -2, +2, -2, +1, -1 };

            for (int i = 0; i < 8; i++) {

                int nr = rank + dr[i];
                int nf = file + df[i];

                if (nr >= 0 && nr < 8 && nf >= 0 && nf < 8) {
                    int idx = GET_INDEX_FROM_SQUARE(nr, nf);
                    mask |= (1ULL << idx);
                }
            }

            KNIGHT_ATTACKS[sqIdx] = mask;
        }

        // NOTE(Tejas): King Attaks
        {
            BitBoard mask = 0ULL;

            const int dr[8] = { +1, +1, +1,  0,  0, -1, -1, -1 };
            const int df[8] = { +1,  0, -1, +1, -1, +1,  0, -1 };

            for (int i = 0; i < 8; i++) {

                int nr = rank + dr[i];
                int nf = file + df[i];

                if (nr >= 0 && nr < 8 && nf >= 0 && nf < 8) {
                    int idx = GET_INDEX_FROM_SQUARE(nr, nf);
                    mask |= (1ULL << idx);
                }
            }

            KING_ATTACKS[sqIdx] = mask;
        }

        // NOTE(Tejas): Light Pawn Attacks
        {
            BitBoard mask = 0;

            int nr = rank + 1;

            if (nr < 8) {

                if (file - 1 >= 0) {
                    int idx = GET_INDEX_FROM_SQUARE(nr, file - 1);
                    mask |= (1ULL << idx);
                }

                if (file + 1 < Chess::MAX_FILE) {
                    int idx = GET_INDEX_FROM_SQUARE(nr, file + 1);
                    mask |= (1ULL << idx);
                }
            }

            PAWN_ATTACKS[Chess::Player::LIGHT][sqIdx] = mask;
        }

        // NOTE(Tejas): Dark Pawn Attacks
        {
            BitBoard mask = 0;

            int nr = rank - 1;

            if (nr >= 0) {

                if (file - 1 >= 0) {
                    int idx = GET_INDEX_FROM_SQUARE(nr, file - 1);
                    mask |= (1ULL << idx);
                }

                if (file + 1 < 8) {
                    int idx = GET_INDEX_FROM_SQUARE(nr, file + 1);
                    mask |= (1ULL << idx);
                }
            }

            PAWN_ATTACKS[Chess::Player::DARK][sqIdx] = mask;
        }
    }
}

BitBoard Attack::pawnAttacks(Chess::Square sq, Chess::Player p) {

    return PAWN_ATTACKS[p][sq.index];
}

BitBoard Attack::knightAttacks(Chess::Square sq) {

    return KNIGHT_ATTACKS[sq.index];    
}

BitBoard Attack::bishopAttacks(Chess::Square sq, BitBoard occ) {

    BitBoard attacks = 0;

    int rank = sq.rank();
    int file = sq.file();

    // NOTE(Tejas): Right-Up Diagonal
    for (int nr = rank + 1, nf = file + 1; nr < 8 && nf < 8; nr++, nf++) {
        int idx = GET_INDEX_FROM_SQUARE(nr, nf);
        attacks |= (1ULL << idx);

        if (occ & (1ULL << idx)) break;
    }

    // NOTE(Tejas): Left-Up Diagonal
    for (int nr = rank + 1, nf = file - 1; nr < 8 && nf >= 0; nr++, nf--) {
        int idx = GET_INDEX_FROM_SQUARE(nr, nf);
        attacks |= (1ULL << idx);

        if (occ & (1ULL << idx)) break;
    }

    // Note(Tejas): Right-Down Diagonal
    for (int nr = rank - 1, nf = file + 1; nr >= 0 && nf < 8; nr--, nf++) {
        int idx = GET_INDEX_FROM_SQUARE(nr, nf);
        attacks |= (1ULL << idx);

        if (occ & (1ULL << idx)) break;
    }

    // Note(Tejas): Left-Down Diagonal
    for (int nr = rank - 1, nf = file - 1; nr >= 0 && nf >= 0; nr--, nf--) {
        int idx = GET_INDEX_FROM_SQUARE(nr, nf);
        attacks |= (1ULL << idx);

        if (occ & (1ULL << idx)) break;
    }

    return attacks;
}

BitBoard Attack::rookAttacks(Chess::Square sq, BitBoard occ) {

    BitBoard attacks = 0;

    int rank = sq.rank();
    int file = sq.file();

    // NOTE(Tejas): Up
    for (int nr = rank + 1; nr < 8; nr++) {
        int idx = GET_INDEX_FROM_SQUARE(nr, file);
        attacks |= (1ULL << idx);

        if (occ & (1ULL << idx)) break;
    }

    // NOTE(Tejas): Down
    for (int nr = rank - 1; nr >= 0; nr--) {
        int idx = GET_INDEX_FROM_SQUARE(nr, file);
        attacks |= (1ULL << idx);

        if (occ & (1ULL << idx)) break;
    }

    // NOTE(Tejas): Right
    for (int nf = file + 1; nf < 8; nf++) {
        int idx = GET_INDEX_FROM_SQUARE(rank, nf);
        attacks |= (1ULL << idx);

        if (occ & (1ULL << idx)) break;
    }

    // NOTE(Tejas): Left
    for (int nf = file - 1; nf >= 0; nf--) {
        int idx = GET_INDEX_FROM_SQUARE(rank, nf);
        attacks |= (1ULL << idx);

        if (occ & (1ULL << idx)) break;
    }

    return attacks;
}

BitBoard Attack::queenAttacks(Chess::Square sq, BitBoard occ) {

    return bishopAttacks(sq, occ) | rookAttacks(sq, occ);
}

BitBoard Attack::kingAttacks(Chess::Square sq) {

    return KING_ATTACKS[sq.index];
}