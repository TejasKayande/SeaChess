#pragma once

#include "base.hpp"
#include "piece.hpp"
#include "square.hpp"

#include <string>

namespace Chess { 

    constexpr int MAX_FILE = 8;
    constexpr int MAX_RANK = 8;

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

        void move(Square from, Square to);

        BitBoard getOccupied() const;
        BitBoard getOccupied(Player p) const;

        BitBoard getPiecesOfType(PType type, Player p) const;

    private:

        BitBoard _lPawn, _lKnight, _lBishop, _lRook, _lQueen, _lKing;
        BitBoard _dPawn, _dKnight, _dBishop, _dRook, _dQueen, _dKing; 

        BitBoard _lOccupied;
        BitBoard _dOccupied;

        Player _turn;

    private:

        void _updateOccupancy();
    };
} // namespace Chess