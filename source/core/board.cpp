
#include "board.hpp"
#include <sstream>
#include <bitset>

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

    // NOTE(Tejas): The 0, 0 Square is h1 and the 7, 7 Square is a8

    reset();

    _lPawn = _lKnight = _lBishop = _lRook = _lQueen = _lKing = 0;
    _dPawn = _dKnight = _dBishop = _dRook = _dQueen = _dKing = 0;
    _lOccupied = _dOccupied = 0;
    _castling_rights = 0;
    _turn = Player::LIGHT;

    std::istringstream ss(fen);
    std::string board_part, turn_part, castle_part, en_passant_part;
    ss >> board_part >> turn_part >> castle_part >> en_passant_part;

    int rank = 7;
    int file = 7;

    for (char c : board_part) {

        if (c == '/') {
            rank--;
            file = 7;
            continue;
        }

        if (std::isdigit(c)) {
            file -= (c - '0');
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

            default: continue;
        }

        setPieceAt(sq, piece);

        file--;
    }

    if (!turn_part.empty())
        _turn = (turn_part == "w" ? Player::LIGHT : Player::DARK);

    if (!castle_part.empty()) {
        if (castle_part.find('K') != std::string::npos) _castling_rights |= CastlingRights::LIGHT_KING_SIDE;
        if (castle_part.find('Q') != std::string::npos) _castling_rights |= CastlingRights::LIGHT_QUEEN_SIDE;
        if (castle_part.find('k') != std::string::npos) _castling_rights |= CastlingRights::DARK_KING_SIDE;
        if (castle_part.find('q') != std::string::npos) _castling_rights |= CastlingRights::DARK_QUEEN_SIDE;
    }

    if (!en_passant_part.empty() && !(en_passant_part == "-")) {

        char file_char = en_passant_part[0];
        char rank_char = en_passant_part[1];

        int ep_file = 7 - (file_char - 'a');
        int ep_rank = rank_char - '1';

        _en_passant_target = Square(ep_rank, ep_file);
    }
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
    _lQueen  = 0x0000000000000010;
    _lKing   = 0x0000000000000008;

    _dPawn   = 0x00FF000000000000;
    _dRook   = 0x8100000000000000;
    _dKnight = 0x4200000000000000;
    _dBishop = 0x2400000000000000;
    _dQueen  = 0x1000000000000000;
    _dKing   = 0x0800000000000000;

    _castling_rights = 0x00 | CastlingRights::LIGHT_KING_SIDE | CastlingRights::LIGHT_QUEEN_SIDE | 
                       CastlingRights::DARK_KING_SIDE  | CastlingRights::DARK_QUEEN_SIDE;

    _updateOccupancy();
    _en_passant_target = Square::invalid();

    _turn = Player::LIGHT;
}

bool Board::makeMove(const Move& m) {

    bool move_made = false;

    Piece moving_piece = getPieceAt(m.from);

    if (moving_piece.isEmpty() || moving_piece.color() != getTurn()) return false;

    if (m.type != MoveType::DOUBLE_PAWN_PUSH) _en_passant_target = Square::invalid();

    switch (m.type) {

        case  MoveType::CAPTURE: 
        case  MoveType::QUIET: {

            setPieceAt(m.from, Piece::nopiece());
            setPieceAt(m.to, moving_piece);

            if (moving_piece.type() == PType::KING) {
                _castling_rights &= (moving_piece.color() == PColor::LIGHT) ? 
                                    ~(CastlingRights::LIGHT_KING_SIDE | CastlingRights::LIGHT_QUEEN_SIDE)
                                  : ~(CastlingRights::DARK_KING_SIDE  | CastlingRights::DARK_QUEEN_SIDE);
            }

            if ((moving_piece.type() == PType::ROOK)) {
                if (m.from == Square(0, 0))      _castling_rights &= ~CastlingRights::LIGHT_KING_SIDE;  // a1
                else if (m.from == Square(0, 7)) _castling_rights &= ~CastlingRights::LIGHT_QUEEN_SIDE; // h1
                else if (m.from == Square(7, 0)) _castling_rights &= ~CastlingRights::DARK_KING_SIDE;   // a8
                else if (m.from == Square(7, 7)) _castling_rights &= ~CastlingRights::DARK_QUEEN_SIDE;  // h8
            }

            if (m.type == MoveType::CAPTURE && m.captured_piece.type() == PType::ROOK) {

                if (m.to == Square(0, 0))      _castling_rights &= ~CastlingRights::LIGHT_KING_SIDE;  // a1
                else if (m.to == Square(0, 7)) _castling_rights &= ~CastlingRights::LIGHT_QUEEN_SIDE; // h1
                else if (m.to == Square(7, 0)) _castling_rights &= ~CastlingRights::DARK_KING_SIDE;   // a8
                else if (m.to == Square(7, 7)) _castling_rights &= ~CastlingRights::DARK_QUEEN_SIDE;  // h8
            }

            move_made = true;

        } break;

        case MoveType::DOUBLE_PAWN_PUSH: {

            setPieceAt(m.from, Piece::nopiece());
            setPieceAt(m.to, moving_piece);

            int dir = (moving_piece.color() == PColor::LIGHT) ? -8 : +8;
            _en_passant_target = Square(m.to.toIndex() + dir);

            move_made = true;

        } break;

        case MoveType::KING_CASTLE: {

            setPieceAt(m.from, Piece::nopiece());
            setPieceAt(m.to, moving_piece);

            if (m.to == Square(0, 1)) {
                setPieceAt(Square(0, 0), Piece::nopiece());
                setPieceAt(Square(0, 2), Piece(PType::ROOK, PColor::LIGHT));
            }

            if (m.to == Square(7, 1)) {
                setPieceAt(Square(7, 0), Piece::nopiece());
                setPieceAt(Square(7, 2), Piece(PType::ROOK, PColor::DARK));
            }

            _castling_rights &= (moving_piece.color() == PColor::LIGHT) ? 
                                ~(CastlingRights::LIGHT_KING_SIDE | CastlingRights::LIGHT_QUEEN_SIDE)
                              : ~(CastlingRights::DARK_KING_SIDE  | CastlingRights::DARK_QUEEN_SIDE);

            move_made = true;
        } break;

        case MoveType::QUEEN_CASTLE: {

            setPieceAt(m.from, Piece::nopiece());
            setPieceAt(m.to, moving_piece);

            if (m.to == Square(0, 5)) {
                setPieceAt(Square(0, 7), Piece::nopiece());
                setPieceAt(Square(0, 4), Piece(PType::ROOK, PColor::LIGHT));
            } 

            if (m.to == Square(7, 5)) {
                setPieceAt(Square(7, 7), Piece::nopiece());
                setPieceAt(Square(7, 4), Piece(PType::ROOK, PColor::DARK));
             }

            _castling_rights &= (moving_piece.color() == PColor::LIGHT) ? 
                                ~(CastlingRights::LIGHT_KING_SIDE | CastlingRights::LIGHT_QUEEN_SIDE)
                              : ~(CastlingRights::DARK_KING_SIDE  | CastlingRights::DARK_QUEEN_SIDE);

            move_made = true;

        } break;

        case MoveType::EN_PASSANT: {

            setPieceAt(m.from, Piece::nopiece());
            setPieceAt(m.to, moving_piece);

            int dir = (moving_piece.color() == PColor::LIGHT) ? -8 : +8;
            Square captured_pawn_sq(m.to.toIndex() + dir);
            setPieceAt(captured_pawn_sq, Piece::nopiece());

            move_made = true;
        } break;

        case MoveType::PROMO_CAPTURE_QUEEN:
        case MoveType::PROMO_QUEEN: {
            setPieceAt(m.from, Piece::nopiece());
            setPieceAt(m.to, Piece(PType::QUEEN, moving_piece.color()));

            move_made = true;
        } break;

        case MoveType::PROMO_CAPTURE_ROOK:
        case MoveType::PROMO_ROOK: {
            setPieceAt(m.from, Piece::nopiece());
            setPieceAt(m.to, Piece(PType::ROOK, moving_piece.color()));

            move_made = true;
        } break;

        case MoveType::PROMO_CAPTURE_BISHOP:
        case MoveType::PROMO_BISHOP: {
            setPieceAt(m.from, Piece::nopiece());
            setPieceAt(m.to, Piece(PType::BISHOP, moving_piece.color()));

            move_made = true;
        } break;

        case MoveType::PROMO_CAPTURE_KNIGHT:
        case MoveType::PROMO_KNIGHT: {
            setPieceAt(m.from, Piece::nopiece());
            setPieceAt(m.to, Piece(PType::KNIGHT, moving_piece.color()));

            move_made = true;
        } break;

        default: {} break;
    }

    if (move_made) changeTurn();

    return move_made;
}

bool Board::unMakeMove(const Move& m) {

    // NOTE(Tejas): This can only undo move that was the latest on the board.
    bool move_unmade = false;

    Piece moving_piece = getPieceAt(m.to);
    if (moving_piece.isEmpty()) return false;

    switch (m.type) {

    case MoveType::QUIET: 
    case MoveType::DOUBLE_PAWN_PUSH: {

        setPieceAt(m.to, Piece::nopiece());
        setPieceAt(m.from, moving_piece);

        move_unmade = true;

    } break;

    case MoveType::PROMO_KNIGHT:
    case MoveType::PROMO_BISHOP:
    case MoveType::PROMO_ROOK:
    case MoveType::PROMO_QUEEN: {

        setPieceAt(m.to, Piece::nopiece());
        setPieceAt(m.from, Chess::Piece(PType::PAWN, moving_piece.color()));

        move_unmade = true;
    } break;
         
    case MoveType::EN_PASSANT: {

        setPieceAt(m.to, Piece::nopiece());
        setPieceAt(m.from, moving_piece);

        int dir = (moving_piece.color() == PColor::LIGHT) ? -8 : +8;
        Square captured_pawn_sq(m.to.toIndex() + dir);
        setPieceAt(captured_pawn_sq, Piece(PType::PAWN, (moving_piece.color() == PColor::LIGHT) ? PColor::DARK : PColor::LIGHT));

        move_unmade = true;
    } break;

    case MoveType::KING_CASTLE: {

        setPieceAt(m.to, Piece::nopiece());
        setPieceAt(m.from, moving_piece);

        if (m.to == Square(0, 1)) {
            setPieceAt(Square(0, 0), Piece(PType::ROOK, PColor::LIGHT));
            setPieceAt(Square(0, 2), Piece::nopiece());

            _castling_rights |= CastlingRights::LIGHT_KING_SIDE | CastlingRights::LIGHT_QUEEN_SIDE;
        }

        if (m.to == Square(7, 1)) {
            setPieceAt(Square(7, 0), Piece(PType::ROOK, PColor::DARK));
            setPieceAt(Square(7, 2), Piece::nopiece());

            _castling_rights |= CastlingRights::DARK_KING_SIDE | CastlingRights::DARK_QUEEN_SIDE;
        }

        move_unmade = true;

    } break;

    case MoveType::QUEEN_CASTLE: {

        setPieceAt(m.to, Piece::nopiece());
        setPieceAt(m.from, moving_piece);

        if (m.to == Square(0, 5)) {
            setPieceAt(Square(0, 7), Piece(PType::ROOK, PColor::LIGHT));
            setPieceAt(Square(0, 4), Piece::nopiece());
        } 

        if (m.to == Square(7, 5)) {
            setPieceAt(Square(7, 7), Piece(PType::ROOK, PColor::DARK));
            setPieceAt(Square(7, 4), Piece::nopiece());
         }

        move_unmade = true;

    } break;
         
    case MoveType::PROMO_CAPTURE_KNIGHT:
    case MoveType::PROMO_CAPTURE_BISHOP:
    case MoveType::PROMO_CAPTURE_ROOK:
    case MoveType::PROMO_CAPTURE_QUEEN: {

        setPieceAt(m.to, m.captured_piece);
        setPieceAt(m.from, Chess::Piece(PType::PAWN, moving_piece.color()));

        move_unmade = true;

    } break;

    case MoveType::CAPTURE: {

        setPieceAt(m.to, m.captured_piece);
        setPieceAt(m.from, moving_piece);

        move_unmade = true;
        
    } break;


    default: {} break;

    }

    if (move_unmade) changeTurn();
    return move_unmade;
}

BitBoard Board::getOccupied() const {

    return _lOccupied | _dOccupied;
}

BitBoard Board::getOccupied(Player p) const {

    return (p == Player::LIGHT) ? _lOccupied : _dOccupied;
}

BitBoard Board::getPiecesOfType(PType type, Player p) const {

    switch (type) {
        case PType::PAWN:   return (p == Player::LIGHT) ? _lPawn : _dPawn;
        case PType::KNIGHT: return (p == Player::LIGHT) ? _lKnight : _dKnight;
        case PType::BISHOP: return (p == Player::LIGHT) ? _lBishop : _dBishop;
        case PType::ROOK:   return (p == Player::LIGHT) ? _lRook : _dRook;
        case PType::QUEEN:  return (p == Player::LIGHT) ? _lQueen : _dQueen;
        case PType::KING:   return (p == Player::LIGHT) ? _lKing : _dKing;
        default: return 0;
    }
}

u8 Board::getCastlingRights() const { 

    return _castling_rights; 
}

Square Board::getEnPassantTarget() const {

    return _en_passant_target;
}
