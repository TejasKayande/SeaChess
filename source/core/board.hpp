#pragma once

#include "base.hpp"
#include "piece.hpp"
#include "square.hpp"
#include "move.hpp"

#include <string>

namespace Chess { 

    constexpr int MAX_FILE = 8;
    constexpr int MAX_RANK = 8;

    enum CastlingRights : u8 {
        LIGHT_KING_SIDE  = 1 << 0,
        LIGHT_QUEEN_SIDE = 1 << 1,
        DARK_KING_SIDE   = 1 << 2,
        DARK_QUEEN_SIDE  = 1 << 3,
    };

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

        BitBoard getOccupied() const;
        BitBoard getOccupied(Player p) const;

        BitBoard getPiecesOfType(PType type, Player p) const;

        u8 getCastlingRights() const;

    private:

        // TODO(Tejas): Replace these with arrays or something, this is pretty
        //              bad for cache locality.
        BitBoard _lPawn, _lKnight, _lBishop, _lRook, _lQueen, _lKing;
        BitBoard _dPawn, _dKnight, _dBishop, _dRook, _dQueen, _dKing; 

        BitBoard _lOccupied;
        BitBoard _dOccupied;

        Player _turn;

        u8 _castling_rights;

    private:

        void _updateOccupancy();
    };
} // namespace Chess