#pragma once

#include "base.hpp"
#include "piece.hpp"
#include "square.hpp"
#include "move.hpp"

#include <string>

/*
  NOTE(Tejas): the board starts (i.e. when rank = 0 and file = 0) at top left
               square, where there is a White King Side Rook.

  NOTE(Tejas): The conventional Chess considers top right corner to be 0, 0
              (i.e, rank = 1 and file = a), but it ease to visualize rendering
              when using the following way (atleast for me!)... 

                (0) (1) (2) (3) (4) (5) (6) (7)

                 h   g   f   e   d   c   b   a
               +---+---+---+---+---+---+---+---+
        (0)  1 | R | N | B | K | Q | B | N | R |
               +---+---+---+---+---+---+---+---+
        (1)  2 | P | P | P | P | P | P | P | P |
               +---+---+---+---+---+---+---+---+
        (2)  3 |   |   |   |   |   |   |   |   |
               +---+---+---+---+---+---+---+---+
        (3)  4 |   |   |   |   |   |   |   |   |
               +---+---+---+---+---+---+---+---+
        (4)  5 |   |   |   |   |   |   |   |   |
               +---+---+---+---+---+---+---+---+
        (5)  6 |   |   |   |   |   |   |   |   |
               +---+---+---+---+---+---+---+---+
        (6)  7 | p | p | p | p | p | p | p | p |
               +---+---+---+---+---+---+---+---+
        (7)  8 | r | n | b | k | q | b | n | r |
               +---+---+---+---+---+---+---+---+

  NOTE(Tejas): Flip Board is a pure Visual effect, the white pawns always need
               to promote on the 7th rank and the black pawns always need
               to promote on the 0th rank.
*/

namespace Chess { 

    constexpr int MAX_FILE = 8;
    constexpr int MAX_RANK = 8;

    enum CastlingRights : u8 {
        LIGHT_KING_SIDE  = 1 << 0,
        LIGHT_QUEEN_SIDE = 1 << 1,
        DARK_KING_SIDE   = 1 << 2,
        DARK_QUEEN_SIDE  = 1 << 3,
    };

    // TODO(Tejas): Delete the copy and move constructors.
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

        bool makeMove(const Move& m);
        bool unMakeMove(const Move& m);

        BitBoard getOccupied() const;
        BitBoard getOccupied(Player p) const;

        BitBoard getPiecesOfType(PType type, Player p) const;

        u8 getCastlingRights() const;
        Square getEnPassantTarget() const;

    private:

        // TODO(Tejas): Replace these with arrays or something, this is pretty
        //              bad for cache locality.
        BitBoard _lPawn, _lKnight, _lBishop, _lRook, _lQueen, _lKing;
        BitBoard _dPawn, _dKnight, _dBishop, _dRook, _dQueen, _dKing; 

        BitBoard _lOccupied;
        BitBoard _dOccupied;

        Player _turn;

        u8 _castling_rights;
        Square _en_passant_target;

    private:

        void _updateOccupancy();
    };
} // namespace Chess
