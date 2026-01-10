#pragma once

#include "base.hpp"
#include "chess.hpp"

#include <raylib.h>

namespace Window {

    // NOTE(Tejas): This is so that in the future if we want to remove raylib
    using Section = Rectangle;

    enum SectionID {
        MENU = 0,
        BOARD,
        STATUS,
        INFORMATION,
    };

    // NOTE(Tejas): In pixels
    constexpr int TEXTURE_SIZE = 70;
    constexpr int SQUARE_DIM   = TEXTURE_SIZE; 
    constexpr int BOARD_SIZE   = SQUARE_DIM * 8;

    constexpr int INFORMATION_WIDTH = BOARD_SIZE / 2;
    constexpr int STATUS_HEIGHT     = 20;

    constexpr int WINDOW_WIDTH  = BOARD_SIZE + INFORMATION_WIDTH;
    constexpr int WINDOW_HEIGHT = BOARD_SIZE + STATUS_HEIGHT;

    Section getBoardSection();
    Section getStatusSection();
    Section getInformationSection();
    Section getMenuSection();

    void toggleMenu();
    bool isOnMenu();

    SectionID getSectionID(int x, int y);

    bool onBoard(int x, int y);
    bool onStatus(int x, int y);
    bool onInformation(int x, int y);

    Chess::Square getSquare(int x, int y);

} // namespace Window
