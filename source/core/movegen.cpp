
#include "movegen.hpp"

#include <bitset>
#include <bit>
using namespace MoveGen;

// TODO(Tejas):
// - [ ] Since all of the move generation functions have a similar structure, we
//       can probably abstract out the common code 
// - [ ] Study about PEXT/BMI2 based move generation for sliding pieces.

// - [ ] Fix the claustro-fuck of the Rook and Bishop magic mask generation

namespace {

    BitBoard ROOK_MASKS[64];
    int      ROOK_RELEVANT_BITS[64];
    BitBoard ROOK_OCCUPANCY_VARIATIONS[64][4096]; // 4096 = 2^12, since the max number of relevent bits for a rook is 12.
    BitBoard ROOK_ATTACKS[64][4096];
    u64      ROOK_MAGICS[64];
    int      ROOK_SHIFTS[64];
    BitBoard ROOK_MAGIC_ATTACKS[64][4096];

    BitBoard BISHOP_MASKS[64];
    int      BISHOP_RELEVANT_BITS[64];
    BitBoard BISHOP_OCCUPANCY_VARIATIONS[64][512]; // 2^9 max
    BitBoard BISHOP_ATTACKS[64][512];
    u64      BISHOP_MAGICS[64];
    int      BISHOP_SHIFTS[64];
    BitBoard BISHOP_MAGIC_ATTACKS[64][512];

    BitBoard PAWN_ATTACKS[Chess::COLOR_COUNT][64];
    BitBoard KNIGHT_ATTACKS[64];
    BitBoard KING_ATTACKS[64];

    u64 randomMagicCandidate() {
        return Base::randomU64()
             & Base::randomU64()
             & Base::randomU64();
    }

    void initAttackTables() {

        for (int sq_idx = 0; sq_idx < 64; sq_idx++) {

            Chess::Square sq(sq_idx);

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
                        int idx = Chess::Square(nr, nf).toIndex();
                        mask |= (1ULL << idx);
                    }
                }

                KNIGHT_ATTACKS[sq_idx] = mask;
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
                        int idx = Chess::Square(nr, nf).toIndex();
                        mask |= (1ULL << idx);
                    }
                }

                KING_ATTACKS[sq_idx] = mask;
            }

            // NOTE(Tejas): Light Pawn Attacks
            {
                BitBoard mask = 0;

                int nr = rank + 1;

                if (nr < 8) {

                    if (file - 1 >= 0) {
                        int idx = Chess::Square(nr, file - 1).toIndex();
                        mask |= (1ULL << idx);
                    }

                    if (file + 1 < Chess::MAX_FILE) {
                        int idx = Chess::Square(nr, file + 1).toIndex();
                        mask |= (1ULL << idx);
                    }
                }

                PAWN_ATTACKS[Chess::Player::LIGHT][sq_idx] = mask;
            }

            // NOTE(Tejas): Dark Pawn Attacks
            {
                BitBoard mask = 0;

                int nr = rank - 1;

                if (nr >= 0) {

                    if (file - 1 >= 0) {
                        int idx = Chess::Square(nr, file - 1).toIndex();
                        mask |= (1ULL << idx);
                    }

                    if (file + 1 < 8) {
                        int idx = Chess::Square(nr, file + 1).toIndex();
                        mask |= (1ULL << idx);
                    }
                }

                PAWN_ATTACKS[Chess::Player::DARK][sq_idx] = mask;
            }
        }
    }

    BitBoard occupancyFromIndex(int index, int bits, BitBoard mask) {

        // NOTE(Tejas): index: the index of the occupancy variation we want to generate.
        //              bits: the number of relevant bits, we get this from ROOK_RELEVANT_BITS.
        //              mask: the ROOK_MASKS for the square.

        BitBoard occupancy = 0;

        for (int i = 0; i < bits; i++) {

            int sq_idx = Base::popLSB(mask);
            if (index & (1 << i)) occupancy |= (1ULL << sq_idx);
        }

        return occupancy;
    }

    BitBoard bishopAttacksSlow(Chess::Square sq, BitBoard occ) {

        BitBoard attacks = 0;

        int rank = sq.rank();
        int file = sq.file();

        // NOTE(Tejas): Right-Up Diagonal
        for (int nr = rank + 1, nf = file + 1; nr < 8 && nf < 8; nr++, nf++) {
            int idx = Chess::Square(nr, nf).toIndex();
            attacks |= (1ULL << idx);

            if (occ & (1ULL << idx)) break;
        }

        // NOTE(Tejas): Left-Up Diagonal
        for (int nr = rank + 1, nf = file - 1; nr < 8 && nf >= 0; nr++, nf--) {
            int idx = Chess::Square(nr, nf).toIndex();
            attacks |= (1ULL << idx);

            if (occ & (1ULL << idx)) break;
        }

        // Note(Tejas): Right-Down Diagonal
        for (int nr = rank - 1, nf = file + 1; nr >= 0 && nf < 8; nr--, nf++) {
            int idx = Chess::Square(nr, nf).toIndex();
            attacks |= (1ULL << idx);

            if (occ & (1ULL << idx)) break;
        }

        // Note(Tejas): Left-Down Diagonal
        for (int nr = rank - 1, nf = file - 1; nr >= 0 && nf >= 0; nr--, nf--) {
            int idx = Chess::Square(nr, nf).toIndex();
            attacks |= (1ULL << idx);

            if (occ & (1ULL << idx)) break;
        }

        return attacks;
    }

    BitBoard rookAttacksSlow(Chess::Square sq, BitBoard occ) {

        // NOTE(Tejas): This is to compute the Magic BitBoard attack tables.

        BitBoard attacks = 0;

        int rank = sq.rank();
        int file = sq.file();

        // NOTE(Tejas): Up
        for (int nr = rank + 1; nr < 8; nr++) {
            int idx = Chess::Square(nr, file).toIndex();
            attacks |= (1ULL << idx);

            if (occ & (1ULL << idx)) break;
        }

        // NOTE(Tejas): Down
        for (int nr = rank - 1; nr >= 0; nr--) {
            int idx = Chess::Square(nr, file).toIndex();
            attacks |= (1ULL << idx);

            if (occ & (1ULL << idx)) break;
        }

        // NOTE(Tejas): Right
        for (int nf = file + 1; nf < 8; nf++) {
            int idx = Chess::Square(rank, nf).toIndex();
            attacks |= (1ULL << idx);

            if (occ & (1ULL << idx)) break;
        }

        // NOTE(Tejas): Left
        for (int nf = file - 1; nf >= 0; nf--) {
            int idx = Chess::Square(rank, nf).toIndex();
            attacks |= (1ULL << idx);

            if (occ & (1ULL << idx)) break;
        }

        return attacks;
    }

    BitBoard generateBishopMask(u8 sq_idx) {

        BitBoard mask = 0; 
        Chess::Square sq(sq_idx);

        for (int dr = 1; sq.rank() + dr < 7 && sq.file() + dr < 7; dr++) {
            int idx = Chess::Square(sq.rank() + dr, sq.file() + dr).toIndex();
            mask |= (1ULL << idx);
        }

        for (int dr = 1; sq.rank() + dr < 7 && sq.file() - dr >= 1; dr++) {
            int idx = Chess::Square(sq.rank() + dr, sq.file() - dr).toIndex();
            mask |= (1ULL << idx);
        }

        for (int dr = 1; sq.rank() - dr >= 1 && sq.file() + dr < 7; dr++) {
            int idx = Chess::Square(sq.rank() - dr, sq.file() + dr).toIndex();
            mask |= (1ULL << idx);
        }

        for (int dr = 1; sq.rank() - dr >= 1 && sq.file() - dr >= 1; dr++) {
            int idx = Chess::Square(sq.rank() - dr, sq.file() - dr).toIndex();
            mask |= (1ULL << idx);
        }

        return mask;
    }

    BitBoard generateRookMask(u8 sq_idx) {

        BitBoard mask = 0; 
        Chess::Square sq(sq_idx);

        // NOTE(Tejas): We ignore the edge squares since they don't have any blockers
        for (int rank = sq.rank() + 1; rank < 7; rank++) {
            int idx = Chess::Square(rank, sq.file()).toIndex();
            mask |= (1ULL << idx);
        }

        for (int rank = sq.rank() - 1; rank >= 1; rank--) {
            int idx = Chess::Square(rank, sq.file()).toIndex();
            mask |= (1ULL << idx);
        }

        for (int file = sq.file() + 1; file < 7; file++) {
            int idx = Chess::Square(sq.rank(), file).toIndex();
            mask |= (1ULL << idx);
        }

        for (int file = sq.file() - 1; file >= 1; file--) {
            int idx = Chess::Square(sq.rank(), file).toIndex();
            mask |= (1ULL << idx);
        }

        return mask;
    }

    int bishopMagicIndex(int sq_idx, BitBoard occ) {
        occ &= BISHOP_MASKS[sq_idx];
        return int((occ * BISHOP_MAGICS[sq_idx]) >> BISHOP_SHIFTS[sq_idx]);
    }

    int rookMagicIndex(int sq_idx, BitBoard occ) {
        occ &= ROOK_MASKS[sq_idx];
        return int((occ * ROOK_MAGICS[sq_idx]) >> ROOK_SHIFTS[sq_idx]);
    }

    bool testBishopMagic(int sq_idx, u64 magic) {

        int count = 1 << BISHOP_RELEVANT_BITS[sq_idx];

        static BitBoard used[512];

        std::fill(std::begin(used), std::end(used), 0ULL);

        for (int occ_idx = 0; occ_idx < count; occ_idx++) {

            BitBoard occ = BISHOP_OCCUPANCY_VARIATIONS[sq_idx][occ_idx];

            BitBoard attacks = BISHOP_ATTACKS[sq_idx][occ_idx];

            u64 index = (occ * magic) >> BISHOP_SHIFTS[sq_idx];

            if (used[index] == 0ULL) used[index] = attacks;
            else if (used[index] != attacks) return false;
        }

        return true;
    }

    bool testRookMagic(int sq_idx, u64 magic) {

        int count = 1 << ROOK_RELEVANT_BITS[sq_idx];

        static BitBoard used[4096];

        std::fill(std::begin(used), std::end(used), 0ULL);

        for (int i = 0; i < count; i++) {

            BitBoard occ = ROOK_OCCUPANCY_VARIATIONS[sq_idx][i];

            BitBoard attack = ROOK_ATTACKS[sq_idx][i];

            u64 index = ((occ * magic) >> ROOK_SHIFTS[sq_idx]);

            if (used[index] == 0) used[index] = attack;
            else if (used[index] != attack) return false;
        }

        return true;
    }

    u64 findBishopMagic(int sq_idx) {

        while (true) {
            u64 magic = randomMagicCandidate();
            if (testBishopMagic(sq_idx, magic)) return magic;
        }
    }

    u64 findRookMagic(int sq_idx) {
        while (true) {
            u64 magic = randomMagicCandidate();
            if (testRookMagic(sq_idx, magic)) return magic;
        }
    }

    void generateBishopMagicBitBoards() {

        // Phase 1: masks + shifts
        for (int sq_idx = 0; sq_idx < 64; sq_idx++) {
            BISHOP_MASKS[sq_idx] = generateBishopMask(sq_idx);
            BISHOP_RELEVANT_BITS[sq_idx] = std::popcount(BISHOP_MASKS[sq_idx]);
            BISHOP_SHIFTS[sq_idx] = 64 - BISHOP_RELEVANT_BITS[sq_idx];
        }

        // Phase 2: occupancy variations
        for (int sq = 0; sq < 64; sq++) {
        
            int bits  = BISHOP_RELEVANT_BITS[sq];
            int count = 1 << bits;
        
            for (int occ_idx = 0; occ_idx < count; occ_idx++) {
                BISHOP_OCCUPANCY_VARIATIONS[sq][occ_idx] = occupancyFromIndex(occ_idx, bits, BISHOP_MASKS[sq]);
            }
        }

        // Phase 3: reference attack table
        for (int sq = 0; sq < 64; sq++) {
        
            int bits = BISHOP_RELEVANT_BITS[sq];
            int count = 1 << bits;
        
            for (int occ_idx = 0; occ_idx < count; occ_idx++) {
            
                BitBoard occ = BISHOP_OCCUPANCY_VARIATIONS[sq][occ_idx];
                BISHOP_ATTACKS[sq][occ_idx] = bishopAttacksSlow(Chess::Square(sq), occ);
            }
        }

        // Phase 4: find magics
        for (int sq = 0; sq < 64; sq++) BISHOP_MAGICS[sq] = findBishopMagic(sq);

        // Phase 5: build magic lookup table
        for (int sq = 0; sq < 64; sq++) {

            int bits  = BISHOP_RELEVANT_BITS[sq];
            int count = 1 << bits;

            for (int occ_idx = 0; occ_idx < count; occ_idx++) {
                BitBoard occ = BISHOP_OCCUPANCY_VARIATIONS[sq][occ_idx];
                u64 magic_idx = bishopMagicIndex(sq, occ);
                BISHOP_MAGIC_ATTACKS[sq][magic_idx] = BISHOP_ATTACKS[sq][occ_idx];
            }
        }
    }

    void generateRookMagicBitBoards() {

        // Phase 1: masks + shifts
        for (int sq_idx = 0; sq_idx < 64; sq_idx++) {
            ROOK_MASKS[sq_idx] = generateRookMask(sq_idx);
            ROOK_RELEVANT_BITS[sq_idx] = std::popcount(ROOK_MASKS[sq_idx]);
            ROOK_SHIFTS[sq_idx] = 64 - ROOK_RELEVANT_BITS[sq_idx];
        }

        // Phase 2: occupancy variations
        for (int sq = 0; sq < 64; sq++) {

            int bits = ROOK_RELEVANT_BITS[sq];
            int count = 1 << bits;

            for (int occ_idx = 0; occ_idx < count; occ_idx++) {
                ROOK_OCCUPANCY_VARIATIONS[sq][occ_idx] = occupancyFromIndex(occ_idx, bits,ROOK_MASKS[sq]);
            }
        }

        // Phase 3: reference attack table
        for (int sq = 0; sq < 64; sq++) {

            int bits = ROOK_RELEVANT_BITS[sq];
            int count = 1 << bits;

            for (int occ_idx = 0; occ_idx < count; occ_idx++) {
                BitBoard occ = ROOK_OCCUPANCY_VARIATIONS[sq][occ_idx];
                ROOK_ATTACKS[sq][occ_idx] = rookAttacksSlow(Chess::Square(sq), occ);
            }
        }

        // Phase 4: find magics
        for (int sq = 0; sq < 64; sq++) ROOK_MAGICS[sq] = findRookMagic(sq);

        // Phase 5: build magic lookup table
        std::fill(&ROOK_MAGIC_ATTACKS[0][0], &ROOK_MAGIC_ATTACKS[0][0] + 64 * 4096, 0ULL);

        for (int sq = 0; sq < 64; sq++) {

            int bits = ROOK_RELEVANT_BITS[sq];
            int count = 1 << bits;

            for (int occ_idx = 0; occ_idx < count; occ_idx++) {
                BitBoard occ = ROOK_OCCUPANCY_VARIATIONS[sq][occ_idx];
                u64 magic_idx = rookMagicIndex(sq, occ);
                ROOK_MAGIC_ATTACKS[sq][magic_idx] = ROOK_ATTACKS[sq][occ_idx];
            }
        }
    }

    void generateCastlingMoves(const Chess::Board *board, Chess::Player player, MoveList &move_list) {

        u8 castling_rights = board->getCastlingRights();
        if (!castling_rights) return;

        Chess::Player enemy = (player == Chess::Player::LIGHT) ? Chess::Player::DARK : Chess::Player::LIGHT;
        Chess::Square from = (player == Chess::Player::LIGHT) ? Chess::Square(0, 3) : Chess::Square(7, 3);

        // NOTE(Tejas): This should never happen, but just in case.
        if (board->getPieceTypeAt(from) != Chess::PType::KING) return;

        BitBoard all_occ = board->getOccupied();

        if (player == Chess::Player::LIGHT) {
            // King-side castle
            if (castling_rights & Chess::CastlingRights::LIGHT_KING_SIDE)  {

                // NOTE(Tejas): we need to check for squares to be empty and not under attack: 
                //              f1(0, 2), g1(0, 3)
                u64 f1_mask = (1ULL << Chess::Square(0, 2).toIndex());
                u64 g1_mask = (1ULL << Chess::Square(0, 1).toIndex());
                u64 e1_mask = (1ULL << Chess::Square(0, 3).toIndex());
                if (!(all_occ & f1_mask) && !(all_occ & g1_mask)) {
                    // NOTE(Tejas): we also need to check if the king is currently in
                    //              check or if it passes through an attacked square
                    BitBoard enemy_attacks = Attack::getAllAttacks(*board, enemy);
                    if (!(enemy_attacks & e1_mask) && !(enemy_attacks & f1_mask) && !(enemy_attacks & g1_mask)) {
                        move_list.push_back(Move(from, Chess::Square(0, 1), Move::KING_CASTLE));
                    }
                }
            }

            if (castling_rights & Chess::CastlingRights::LIGHT_QUEEN_SIDE) {
                // NOTE(Tejas): Square to check: d1(0, 4), c1(0, 5), b1(0, 6)
                u64 d1_mask = (1ULL << Chess::Square(0, 4).toIndex());
                u64 c1_mask = (1ULL << Chess::Square(0, 5).toIndex());
                u64 b1_mask = (1ULL << Chess::Square(0, 6).toIndex());
                u64 e1_mask = (1ULL << Chess::Square(0, 3).toIndex());

                if (!(all_occ & d1_mask) && !(all_occ & c1_mask) && !(all_occ & b1_mask)) {
                    // NOTE(Tejas): we also need to check if the king is currently in
                    //              check or if it passes through an attacked square
                    BitBoard enemy_attacks = Attack::getAllAttacks(*board, enemy);
                    if (!(enemy_attacks & e1_mask) && !(enemy_attacks & d1_mask) && !(enemy_attacks & c1_mask)) {
                        move_list.push_back(Move(from, Chess::Square(0, 5), Move::QUEEN_CASTLE));
                    }
                }
            }
        }

        if (player == Chess::Player::DARK) {
            // King-side castle
            if (castling_rights & Chess::CastlingRights::DARK_KING_SIDE)  {

                // NOTE(Tejas): we need to check for squares to be empty and not under attack: 
                //              f8(7, 2), g8(0, 3)
                u64 f8_mask = (1ULL << Chess::Square(7, 2).toIndex());
                u64 g8_mask = (1ULL << Chess::Square(7, 1).toIndex());
                u64 e8_mask = (1ULL << Chess::Square(7, 3).toIndex());
                if (!(all_occ & f8_mask) && !(all_occ & g8_mask)) {
                    // NOTE(Tejas): we also need to check if the king is currently in
                    //              check or if it passes through an attacked square
                    BitBoard enemy_attacks = Attack::getAllAttacks(*board, enemy);
                    if (!(enemy_attacks & e8_mask) && !(enemy_attacks & f8_mask) && !(enemy_attacks & g8_mask)) {
                        move_list.push_back(Move(from, Chess::Square(7, 1), Move::KING_CASTLE));
                    }
                }
            }

            if (castling_rights & Chess::CastlingRights::DARK_QUEEN_SIDE) {
                // NOTE(Tejas): Square to check: d1(0, 4), c1(0, 5), b1(0, 6)
                u64 d8_mask = (1ULL << Chess::Square(7, 4).toIndex());
                u64 c8_mask = (1ULL << Chess::Square(7, 5).toIndex());
                u64 b8_mask = (1ULL << Chess::Square(7, 6).toIndex());
                u64 e8_mask = (1ULL << Chess::Square(7, 3).toIndex());

                if (!(all_occ & d8_mask) && !(all_occ & c8_mask) && !(all_occ & b8_mask)) {
                    // NOTE(Tejas): we also need to check if the king is currently in
                    //              check or if it passes through an attacked square
                    BitBoard enemy_attacks = Attack::getAllAttacks(*board, enemy);
                    if (!(enemy_attacks & e8_mask) && !(enemy_attacks & d8_mask) && !(enemy_attacks & c8_mask)) {
                        move_list.push_back(Move(from, Chess::Square(7, 5), Move::QUEEN_CASTLE));
                    }
                }
            }
        }
    }

    void generatePawnPromotionMoves(MoveList &move_list, Chess::Board, Chess::Square from, 
                                    Chess::Square to, bool capture = false, 
                                    Chess::Piece captured_piece = Chess::Piece::nopiece()) 
    {
        if (capture)  {
            move_list.push_back(Move(from, to, Move::PROMO_CAPTURE_QUEEN , captured_piece));
            move_list.push_back(Move(from, to, Move::PROMO_CAPTURE_ROOK  , captured_piece));
            move_list.push_back(Move(from, to, Move::PROMO_CAPTURE_KNIGHT, captured_piece));
            move_list.push_back(Move(from, to, Move::PROMO_CAPTURE_BISHOP, captured_piece));
        } else {
            move_list.push_back(Move(from, to, Move::PROMO_QUEEN));
            move_list.push_back(Move(from, to, Move::PROMO_ROOK));
            move_list.push_back(Move(from, to, Move::PROMO_KNIGHT));
            move_list.push_back(Move(from, to, Move::PROMO_BISHOP));
        }
    }

} // Anonymous namespace

void MoveGen::init() {

    initAttackTables();
    generateRookMagicBitBoards();
    generateBishopMagicBitBoards();
}

BitBoard Attack::getAllAttacks(Chess::Board board, Chess::Player player) {

    BitBoard friendly_occ = board.getOccupied(player);

    BitBoard attacks = 0ULL;

    while (friendly_occ) {

        int sq_idx = Base::popLSB(friendly_occ);
        Chess::Square sq(sq_idx);
        Chess::Piece piece = board.getPieceAt(sq);

        switch (piece.type()) {
            case Chess::Piece::PAWN:   attacks |= pawnAttacks(sq, piece.color()); break;
            case Chess::Piece::KNIGHT: attacks |= knightAttacks(sq); break;
            case Chess::Piece::BISHOP: attacks |= bishopAttacks(sq, board.getOccupied()); break;
            case Chess::Piece::ROOK:   attacks |= rookAttacks(sq, board.getOccupied()); break;
            case Chess::Piece::QUEEN:  attacks |= queenAttacks(sq, board.getOccupied()); break;
            case Chess::Piece::KING:   attacks |= kingAttacks(sq); break;
            default: break;
        }
    }

    return attacks;
}

BitBoard Attack::pawnAttacks(Chess::Square sq, Chess::Player p) {

    return PAWN_ATTACKS[p][sq.index];
}

BitBoard Attack::knightAttacks(Chess::Square sq) {

    return KNIGHT_ATTACKS[sq.index];    
}

BitBoard Attack::bishopAttacks(Chess::Square sq, BitBoard occ) {

    u64 idx = bishopMagicIndex(sq.index, occ);
    return BISHOP_MAGIC_ATTACKS[sq.index][idx];
}

BitBoard Attack::rookAttacks(Chess::Square sq, BitBoard occ) {

    u64 idx = rookMagicIndex(sq.index, occ);
    return ROOK_MAGIC_ATTACKS[sq.index][idx];
}

BitBoard Attack::queenAttacks(Chess::Square sq, BitBoard occ) {

    return bishopAttacks(sq, occ) | rookAttacks(sq, occ);
}

BitBoard Attack::kingAttacks(Chess::Square sq) {

    return KING_ATTACKS[sq.index];
}

void PseudoLegal::generateAllMoves(const Chess::Board *board, Chess::Player player, MoveList &move_list) {

    generatePawnMoves(board, player, move_list);
    generateKnightMoves(board, player, move_list);
    generateBishopMoves(board, player, move_list);
    generateRookMoves(board, player, move_list);
    generateQueenMoves(board, player, move_list);
    generateKingMoves(board, player, move_list);
}

void PseudoLegal::generateMovesForSquare(const Chess::Board *board, Chess::Square sq, MoveList &move_list) {

    Chess::Piece piece = board->getPieceAt(sq);

    if (piece.isEmpty()) return;

    MoveList all_moves;
    generateAllMoves(board, piece.color(), all_moves);

    for (const Move &move : all_moves) {
        if (move.from == sq) {
            move_list.push_back(move);
        }
    } 
}

void PseudoLegal::generatePawnMoves(const Chess::Board *board, Chess::Player player, MoveList &move_list) {

    Chess::Player enemy = (player == Chess::Player::LIGHT) ? Chess::Player::DARK : Chess::Player::LIGHT;

    BitBoard pawns = board->getPiecesOfType(Chess::Piece::PAWN, player);

    BitBoard friendly_occ = board->getOccupied(player);
    BitBoard enemy_occ    = board->getOccupied(enemy);
    BitBoard all_occ      = friendly_occ | enemy_occ;

    const int promo_rank = (player == Chess::Player::LIGHT) ? 7 : 0;

    int dir = (player == Chess::Player::LIGHT) ? +8 : -8;

    while (pawns) {

        int from_idx = Base::popLSB(pawns);
        Chess::Square from(from_idx);

        int rank = from.rank();

        int to_idx = from_idx + dir;

        if (to_idx >= 0 && to_idx < 64 && !(all_occ & (1ULL << to_idx))) {
            Chess::Square to(to_idx);

            if (to.rank() == promo_rank) {
                generatePawnPromotionMoves(move_list, *board, from, to); 
            } else {
                move_list.push_back(Move(from, to, Move::QUIET));
            }

            // NOTE(Tejas): Double Push
            bool is_start_rank = (player == Chess::Player::LIGHT) ? (rank == 1) : (rank == 6);
            int to_idx2 = from_idx + 2 * dir;

            if (is_start_rank && !(all_occ & (1ULL << to_idx2))) {
                move_list.push_back(Move(from, Chess::Square(to_idx2), Move::DOUBLE_PAWN_PUSH));
            }
        }

        // NOTE(Tejas): Captures
        BitBoard attacks = Attack::pawnAttacks(from, player);
        BitBoard targets = attacks & enemy_occ;

        while (targets) {
            int to_idx = Base::popLSB(targets);
            Chess::Square to(to_idx);

            if (to.rank() == promo_rank) {
                generatePawnPromotionMoves(move_list, *board, from, to, true, board->getPieceAt(to));
            } else {
                move_list.push_back(Move(from, to, Move::CAPTURE, board->getPieceAt(to)));
            }
        }

        // NOTE(Tejas): En Passant
        Chess::Square ep_target = board->getEnPassantTarget();
        if (ep_target.isValid() && attacks & (1ULL << ep_target.toIndex())) {
            move_list.push_back(Move(from, ep_target, Move::EN_PASSANT));
        }
    }
}

void PseudoLegal::generateKnightMoves(const Chess::Board *board, Chess::Player player, MoveList &move_list) {

    Chess::Player enemy = (player == Chess::Player::LIGHT) ? Chess::Player::DARK : Chess::Player::LIGHT;

    BitBoard knights = board->getPiecesOfType(Chess::Piece::KNIGHT, player);

    BitBoard friendly_occ = board->getOccupied(player);
    BitBoard enemy_occ    = board->getOccupied(enemy);

    while (knights) {

        int from_idx = Base::popLSB(knights);

        Chess::Square from(from_idx);

        BitBoard attacks = Attack::knightAttacks(from);
        BitBoard targets = attacks & ~friendly_occ;

        while (targets) {

            int to_idx = Base::popLSB(targets);
            Chess::Square to(to_idx);

            if (enemy_occ & (1ULL << to_idx)) {
                move_list.push_back(Move(from, to, Move::CAPTURE, board->getPieceAt(to)));
            } else {
                move_list.push_back(Move(from, to, Move::QUIET));
            }
        }
    }
}

void PseudoLegal::generateBishopMoves(const Chess::Board *board, Chess::Player player, MoveList &move_list) {

    Chess::Player enemy = (player == Chess::Player::LIGHT) ? Chess::Player::DARK : Chess::Player::LIGHT;

    BitBoard bishops = board->getPiecesOfType(Chess::Piece::BISHOP, player);

    BitBoard friendly_occ = board->getOccupied(player);
    BitBoard enemy_occ    = board->getOccupied(enemy);

    while (bishops) {

        int from_idx = Base::popLSB(bishops);

        Chess::Square from(from_idx);

        BitBoard attacks = Attack::bishopAttacks(from, friendly_occ | enemy_occ);
        BitBoard targets = attacks & ~friendly_occ;

        while (targets) {

            int to_idx = Base::popLSB(targets);
            Chess::Square to(to_idx);

            if (enemy_occ & (1ULL << to_idx)) {
                move_list.push_back(Move(from, to, Move::CAPTURE, board->getPieceAt(to)));
            } else {
                move_list.push_back(Move(from, to, Move::QUIET));
            }
        }
    }
}

void PseudoLegal::generateRookMoves(const Chess::Board *board, Chess::Player player, MoveList &move_list) {

    // TODO(Tejas): should we use Turn or Pass Player explicitly here?..

    Chess::Player enemy = (player == Chess::Player::LIGHT) ? Chess::Player::DARK : Chess::Player::LIGHT;

    BitBoard rooks = board->getPiecesOfType(Chess::Piece::ROOK, player);

    BitBoard friendly_occ = board->getOccupied(player);
    BitBoard enemy_occ    = board->getOccupied(enemy);

    while (rooks) {

        int from_idx = Base::popLSB(rooks);

        Chess::Square from(from_idx);

        BitBoard attacks = Attack::rookAttacks(from, friendly_occ | enemy_occ);
        BitBoard targets = attacks & ~friendly_occ;

        while (targets) {

            int to_idx = Base::popLSB(targets);
            Chess::Square to(to_idx);

            if (enemy_occ & (1ULL << to_idx)) {
                move_list.push_back(Move(from, to, Move::CAPTURE, board->getPieceAt(to)));
            } else {
                move_list.push_back(Move(from, to, Move::QUIET));
            }
        }
    }
}

void PseudoLegal::generateQueenMoves(const Chess::Board *board, Chess::Player player, MoveList &move_list) {

    // TODO(Tejas): should we use Turn or Pass Player explicitly here?..

    Chess::Player enemy = (player == Chess::Player::LIGHT) ? Chess::Player::DARK : Chess::Player::LIGHT;

    BitBoard queens = board->getPiecesOfType(Chess::Piece::QUEEN, player);

    BitBoard friendly_occ = board->getOccupied(player);
    BitBoard enemy_occ    = board->getOccupied(enemy);

    while (queens) {

        int from_idx = Base::popLSB(queens);

        Chess::Square from(from_idx);

        BitBoard attacks = Attack::queenAttacks(from, friendly_occ | enemy_occ);
        BitBoard targets = attacks & ~friendly_occ;

        while (targets) {

            int to_idx = Base::popLSB(targets);
            Chess::Square to(to_idx);

            if (enemy_occ & (1ULL << to_idx)) {
                move_list.push_back(Move(from, to, Move::CAPTURE, board->getPieceAt(to)));
            } else {
                move_list.push_back(Move(from, to, Move::QUIET));
            }
        }
    }
}

void PseudoLegal::generateKingMoves(const Chess::Board *board, Chess::Player player, MoveList &move_list) {

    Chess::Player enemy = (player == Chess::Player::LIGHT) ? Chess::Player::DARK : Chess::Player::LIGHT;

    BitBoard king = board->getPiecesOfType(Chess::Piece::KING, player);

    BitBoard friendly_occ = board->getOccupied(player);
    BitBoard enemy_occ    = board->getOccupied(enemy);

    int from_idx = Base::popLSB(king);  // only one king
    Chess::Square from(from_idx);

    BitBoard attacks = Attack::kingAttacks(from);
    BitBoard targets = attacks & ~friendly_occ;

    while (targets) {

        int to_idx = Base::popLSB(targets);
        Chess::Square to(to_idx);

        if (enemy_occ & (1ULL << to_idx)) {
            move_list.push_back(Move(from, to, Move::CAPTURE, board->getPieceAt(to)));
        } else {
            move_list.push_back(Move(from, to, Move::QUIET));
        }
    }

    generateCastlingMoves(board, player, move_list);
}

bool Legal::isCheckmate(const Chess::Board *board, Chess::Player player) {

    // TODO(Tejas): There has to be a better way to do this...

    Chess::Player enemy = (player == Chess::Player::LIGHT)
                        ? Chess::Player::DARK
                        : Chess::Player::LIGHT;

    if (!inCheck(board, player)) return false;

    MoveList legal_moves;
    Legal::generateAllMoves(board, legal_moves);

    return legal_moves.empty();
}

bool Legal::inCheck(const Chess::Board *board, Chess::Player player) {

    Chess::Player enemy = (player == Chess::Player::LIGHT)
                        ? Chess::Player::DARK
                        : Chess::Player::LIGHT;

    BitBoard king_bb = board->getPiecesOfType(Chess::Piece::KING, player);
    int king_idx = Base::popLSB(king_bb);
    Chess::Square king_sq(king_idx);

    BitBoard enemy_attacks = Attack::getAllAttacks(*board, enemy);
    if (enemy_attacks & (1ULL << king_idx))
        return true;

    return false;
}

void Legal::generateAllMoves(const Chess::Board* board, MoveList& move_list) {

    MoveList pseudo_moves;
    PseudoLegal::generateAllMoves(board, board->getTurn(), pseudo_moves);

    Chess::Board temp_board = *board;

    for (const Move &move : pseudo_moves) {

        // TODO(Tejas): Use undo move instead of making a copy.
        if (temp_board.makeMove(move)) {
            if (!inCheck(&temp_board, board->getTurn())) {
                move_list.push_back(move);
            }
            temp_board.unMakeMove(move);
        }
    }
}

void Legal::generateMovesForSquare(const Chess::Board* board, Chess::Square sq, MoveList& move_list) {

    MoveList pseudo_moves;
    PseudoLegal::generateMovesForSquare(board, sq, pseudo_moves);

    Chess::Board temp_board = *board;

    for (const Move &move : pseudo_moves) {

        if (temp_board.makeMove(move)) {
            if (!inCheck(&temp_board, board->getTurn())) {
                move_list.push_back(move);
            }
            temp_board.unMakeMove(move);
        }
    }
}

BitBoard MoveGen::convertMoveListToBitBoard(const MoveList &move_list) {

    BitBoard move_bb = 0;

    for (const Move &move : move_list) {
        int to_idx = move.to.index;
        move_bb |= (1ULL << to_idx);
    }

    return move_bb;
}

BitBoard MoveGen::flipBitBoard(BitBoard bb) {

    BitBoard result = 0;

    while (bb) {
        int sq = Base::popLSB(bb);
        result |= (1ULL << (63 - sq));
    }

    return result;
}