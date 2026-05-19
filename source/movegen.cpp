
#include "movegen.hpp"

using namespace MoveGen;

// TODO(Tejas):
// - [ ] For all forward pawn moves, we label them as QUIET moves. They could be
//       promotions. Same with captures.
// - [ ] Since all of the move generation functions have a similar structure, we
//       can probably abstract out the common code 
// - [ ] Add lookup tables for sliding pieces as well: magic bitboards or
//       PEXT/BMI2 indexed lookup tables.

namespace {
    BitBoard PAWN_ATTACKS[Chess::COLOR_COUNT][64];
    BitBoard KNIGHT_ATTACKS[64];
    BitBoard KING_ATTACKS[64];
} // Anonymous namespace

void MoveGen::init() {

    Attack::initAttackTables();
}

void Attack::initAttackTables() {

    for (int sq_idx = 0; sq_idx < 64; sq_idx++) {

        Chess::Square sq = GET_SQUARE_FROM_INDEX(sq_idx);

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
                    int idx = GET_INDEX_FROM_SQUARE(nr, nf);
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
                    int idx = GET_INDEX_FROM_SQUARE(nr, file - 1);
                    mask |= (1ULL << idx);
                }

                if (file + 1 < Chess::MAX_FILE) {
                    int idx = GET_INDEX_FROM_SQUARE(nr, file + 1);
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
                    int idx = GET_INDEX_FROM_SQUARE(nr, file - 1);
                    mask |= (1ULL << idx);
                }

                if (file + 1 < 8) {
                    int idx = GET_INDEX_FROM_SQUARE(nr, file + 1);
                    mask |= (1ULL << idx);
                }
            }

            PAWN_ATTACKS[Chess::Player::DARK][sq_idx] = mask;
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

    int dir = (player == Chess::Player::LIGHT) ? +8 : -8;

    while (pawns) {

        int from_idx = Base::popLSB(pawns);
        Chess::Square from(from_idx);

        int rank = from.rank();

        int to_idx = from_idx + dir;

        if (to_idx >= 0 && to_idx < 64 && !(all_occ & (1ULL << to_idx))) {
            Chess::Square to(to_idx);

            move_list.push_back({from, to, Move::QUIET});

            // NOTE(Tejas): Double Push
            bool is_start_rank = (player == Chess::Player::LIGHT) ? (rank == 1) : (rank == 6);
            int to_idx2 = from_idx + 2 * dir;

            if (is_start_rank && !(all_occ & (1ULL << to_idx2))) {
                move_list.push_back({from, Chess::Square(to_idx2), Move::DOUBLE_PAWN_PUSH});
            }
        }

        // NOTE(Tejas): Captures
        BitBoard attacks = Attack::pawnAttacks(from, player);
        BitBoard targets = attacks & enemy_occ;

        while (targets) {
            int to_idx = Base::popLSB(targets);
            Chess::Square to(to_idx);

            move_list.push_back({from, to, Move::CAPTURE});
        }
    }
}

void PseudoLegal::generateKnightMoves(const Chess::Board *board, Chess::Player player, MoveList &move_list) {

    // TODO(Tejas): should we use Turn or Pass Player explicitly here?..

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
                move_list.push_back({from, to, Move::CAPTURE});
            } else {
                move_list.push_back({from, to, Move::QUIET});
            }
        }
    }
}

void PseudoLegal::generateBishopMoves(const Chess::Board *board, Chess::Player player, MoveList &move_list) {

    // TODO(Tejas): should we use Turn or Pass Player explicitly here?..

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
                move_list.push_back({from, to, Move::CAPTURE});
            } else {
                move_list.push_back({from, to, Move::QUIET});
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
                move_list.push_back({from, to, Move::CAPTURE});
            } else {
                move_list.push_back({from, to, Move::QUIET});
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
                move_list.push_back({from, to, Move::CAPTURE});
            } else {
                move_list.push_back({from, to, Move::QUIET});
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
            move_list.push_back({from, to, Move::CAPTURE});
        } else {
            move_list.push_back({from, to, Move::QUIET});
        }
    }

    // TODO(Tejas): Castling Here...
}

BitBoard PseudoLegal::convertMoveListToBitBoard(const MoveList &move_list) {

    BitBoard move_bb = 0;

    for (const Move &move : move_list) {
        int to_idx = move.to.index;
        move_bb |= (1ULL << to_idx);
    }

    return move_bb;
}

 // TODO(Tejas): This is a pretty expensive function, we can optimize it by
 // doing some sort of incremental attack detection. For example, when we make a
 // move, we can check if it opens up an attack on the king, or if it moves the
 // king into check, etc. This way we don't have to
bool Legal::inCheck(const Chess::Board *board, Chess::Player player) {

    Chess::Player enemy = (player == Chess::Player::LIGHT)
                        ? Chess::Player::DARK
                        : Chess::Player::LIGHT;

    // 1. Get king square
    BitBoard king_bb = board->getPiecesOfType(Chess::Piece::KING, player);
    int king_idx = Base::popLSB(king_bb);
    Chess::Square king_sq(king_idx);

    BitBoard occ = board->getOccupied(Chess::Player::LIGHT) |
                   board->getOccupied(Chess::Player::DARK);

    // 2. Pawn attacks
    BitBoard enemy_pawns = board->getPiecesOfType(Chess::Piece::PAWN, enemy);
    while (enemy_pawns) {
        int sq_idx = Base::popLSB(enemy_pawns);
        if (Attack::pawnAttacks(Chess::Square(sq_idx), enemy) & (1ULL << king_idx))
            return true;
    }

    // 3. Knight attacks
    BitBoard enemy_knights = board->getPiecesOfType(Chess::Piece::KNIGHT, enemy);
    while (enemy_knights) {
        int sq_idx = Base::popLSB(enemy_knights);
        if (Attack::knightAttacks(Chess::Square(sq_idx)) & (1ULL << king_idx))
            return true;
    }

    // 4. Bishop / Queen (diagonals)
    BitBoard enemy_bishops = board->getPiecesOfType(Chess::Piece::BISHOP, enemy) |
                            board->getPiecesOfType(Chess::Piece::QUEEN, enemy);

    while (enemy_bishops) {
        int sq_idx = Base::popLSB(enemy_bishops);
        if (Attack::bishopAttacks(Chess::Square(sq_idx), occ) & (1ULL << king_idx))
            return true;
    }

    // 5. Rook / Queen (straight)
    BitBoard enemy_rooks = board->getPiecesOfType(Chess::Piece::ROOK, enemy) |
                          board->getPiecesOfType(Chess::Piece::QUEEN, enemy);

    while (enemy_rooks) {
        int sq_idx = Base::popLSB(enemy_rooks);
        if (Attack::rookAttacks(Chess::Square(sq_idx), occ) & (1ULL << king_idx))
            return true;
    }

    // 6. King attacks (important edge case)
    BitBoard enemy_king = board->getPiecesOfType(Chess::Piece::KING, enemy);
    int enemy_king_idx = Base::popLSB(enemy_king);

    if (Attack::kingAttacks(Chess::Square(enemy_king_idx)) & (1ULL << king_idx))
        return true;

    return false;
}

void Legal::generateAllMoves(const Chess::Board* board, MoveList& move_list) {

}

void Legal::generateMovesForSquare(const Chess::Board* board, Chess::Square sq, MoveList& move_list) {

}
