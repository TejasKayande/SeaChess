#include "chess.hpp"
#include <sstream>

using namespace Chess;

Board::Board() 
: _lPawn(0), _lKnight(0), _lBishop(0), _lRook(0), _lQueen(0), _lKing(0),
  _dPawn(0), _dKnight(0), _dBishop(0), _dRook(0), _dQueen(0), _dKing(0),
  _lOccupied(0), _dOccupied(0), _turn(Player::LIGHT)
{
    reset();
}   

void Board::_updateOccupancy() {
    _lOccupied = _lPawn | _lKnight | _lBishop | _lRook | _lQueen | _lKing;
    _dOccupied = _dPawn | _dKnight | _dBishop | _dRook | _dQueen | _dKing;
}

void Board::setFen(const std::string& fen) {

    _lPawn = _lKnight = _lBishop = _lRook = _lQueen = _lKing = 0;
    _dPawn = _dKnight = _dBishop = _dRook = _dQueen = _dKing = 0;
    _lOccupied = _dOccupied = 0;

    std::istringstream ss(fen);
    std::string boardPart, turnPart;
    ss >> boardPart >> turnPart;

    int rank = 7;
    int file = 0;

    for (char c : boardPart) {
        if (c == '/') {
            rank--;
            file = 0;
            continue;
        }

        if (isdigit(c)) {
            file += c - '0';
            continue;
        }

        Square sq(rank, file);
        Piece piece;

        switch (c) {
            case 'P': piece = Piece(PType::PAWN,   PColor::LIGHT); break;
            case 'N': piece = Piece(PType::KNIGHT, PColor::LIGHT); break;
            case 'B': piece = Piece(PType::BISHOP, PColor::LIGHT); break;
            case 'R': piece = Piece(PType::ROOK,   PColor::LIGHT); break;
            case 'Q': piece = Piece(PType::QUEEN,  PColor::LIGHT); break;
            case 'K': piece = Piece(PType::KING,   PColor::LIGHT); break;

            case 'p': piece = Piece(PType::PAWN,   PColor::DARK); break;
            case 'n': piece = Piece(PType::KNIGHT, PColor::DARK); break;
            case 'b': piece = Piece(PType::BISHOP, PColor::DARK); break;
            case 'r': piece = Piece(PType::ROOK,   PColor::DARK); break;
            case 'q': piece = Piece(PType::QUEEN,  PColor::DARK); break;
            case 'k': piece = Piece(PType::KING,   PColor::DARK); break;

            default: break;
        }

        if (!piece.isEmpty()) {
            setPieceAt(sq, piece);
        }

        file++;
    }

    _turn = (turnPart == "w" ? Player::LIGHT : Player::DARK);
}

std::string Board::getFen() const {

    std::ostringstream fen;

    for (int rank = 7; rank >= 0; --rank) {
        int emptyCount = 0;

        for (int file = 0; file < 8; ++file) {
            Square sq(rank, file);
            Piece p = getPieceAt(sq);

            if (p.isEmpty()) {
                emptyCount++;
            } else {
                if (emptyCount > 0) {
                    fen << emptyCount;
                    emptyCount = 0;
                }

                char c = '?';
                switch (p.type()) {
                    case PType::PAWN:   c = 'p'; break;
                    case PType::KNIGHT: c = 'n'; break;
                    case PType::BISHOP: c = 'b'; break;
                    case PType::ROOK:   c = 'r'; break;
                    case PType::QUEEN:  c = 'q'; break;
                    case PType::KING:   c = 'k'; break;
                    default: break;
                }

                if (p.color() == PColor::LIGHT)
                    c = toupper(c);

                fen << c;
            }
        }

        if (emptyCount > 0)
            fen << emptyCount;

        if (rank > 0)
            fen << '/';
    }

    fen << ' ' << (_turn == Player::LIGHT ? 'w' : 'b');
    fen << " - - 0 1"; // default placeholders

    return fen.str();
}

Piece Board::getPieceAt(const Square& sq) const {

    BitBoard mask = (1ULL << sq.index);

    if (_lOccupied & mask) {
        if (_lPawn   & mask) return Piece(PType::PAWN  , PColor::LIGHT);
        if (_lKnight & mask) return Piece(PType::KNIGHT, PColor::LIGHT);
        if (_lBishop & mask) return Piece(PType::BISHOP, PColor::LIGHT);
        if (_lRook   & mask) return Piece(PType::ROOK  , PColor::LIGHT);
        if (_lQueen  & mask) return Piece(PType::QUEEN , PColor::LIGHT);
        if (_lKing   & mask) return Piece(PType::KING  , PColor::LIGHT);
    }

    if (_dOccupied & mask) {
        if (_dPawn   & mask) return Piece(PType::PAWN  , PColor::DARK);
        if (_dKnight & mask) return Piece(PType::KNIGHT, PColor::DARK);
        if (_dBishop & mask) return Piece(PType::BISHOP, PColor::DARK);
        if (_dRook   & mask) return Piece(PType::ROOK  , PColor::DARK);
        if (_dQueen  & mask) return Piece(PType::QUEEN , PColor::DARK);
        if (_dKing   & mask) return Piece(PType::KING  , PColor::DARK);
    }

    return Piece();
}

PType Board::getPieceTypeAt(const Square& sq) const {

    return getPieceAt(sq).type();
}

PColor Board::getPieceColorAt(const Square& sq) const {

    return getPieceAt(sq).color();
}

void Board::setPieceAt(const Square& sq, const Piece& piece) {

    const BitBoard mask = 1ULL << sq.index;

    removePieceAt(sq);

    if (!piece.isEmpty()) {
        if (piece.color() == PColor::LIGHT) {
            switch (piece.type()) {
                case PType::PAWN:   _lPawn   |= mask; break;
                case PType::KNIGHT: _lKnight |= mask; break;
                case PType::BISHOP: _lBishop |= mask; break;
                case PType::ROOK:   _lRook   |= mask; break;
                case PType::QUEEN:  _lQueen  |= mask; break;
                case PType::KING:   _lKing   |= mask; break;
                default: break;
            }
        } else if (piece.color() == PColor::DARK) {
            switch (piece.type()) {
                case PType::PAWN:   _dPawn   |= mask; break;
                case PType::KNIGHT: _dKnight |= mask; break;
                case PType::BISHOP: _dBishop |= mask; break;
                case PType::ROOK:   _dRook   |= mask; break;
                case PType::QUEEN:  _dQueen  |= mask; break;
                case PType::KING:   _dKing   |= mask; break;
                default: break;
            }
        }
    }

    _updateOccupancy();
}

void Board::removePieceAt(const Square& sq) {

    const BitBoard mask = ~(1ULL << sq.index);

    _lPawn   &= mask;
    _lKnight &= mask;
    _lBishop &= mask;
    _lRook   &= mask;
    _lQueen  &= mask;
    _lKing   &= mask;

    _dPawn   &= mask;
    _dKnight &= mask;
    _dBishop &= mask;
    _dRook   &= mask;
    _dQueen  &= mask;
    _dKing   &= mask;

    _updateOccupancy();
}

Player Board::getTurn() const {

    return _turn;
}

void Board::changeTurn() {

    _turn = (_turn == Player::LIGHT) ? Player::DARK : Player::LIGHT;
}

void Board::reset() {

    _lPawn   = 0x000000000000FF00;
    _lRook   = 0x0000000000000081;
    _lKnight = 0x0000000000000042;
    _lBishop = 0x0000000000000024;
    _lQueen  = 0x0000000000000008;
    _lKing   = 0x0000000000000010;

    _dPawn   = 0x00FF000000000000;
    _dRook   = 0x8100000000000000;
    _dKnight = 0x4200000000000000;
    _dBishop = 0x2400000000000000;
    _dQueen  = 0x0800000000000000;
    _dKing   = 0x1000000000000000;

    _updateOccupancy();

    _turn = Player::LIGHT;
}
