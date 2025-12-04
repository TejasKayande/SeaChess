#pragma once

#include "base.hpp"
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
    constexpr int TEXTURE_SIZE = 100;
    constexpr int BOARD_SIZE   = TEXTURE_SIZE * 8;

    constexpr int INFORMATION_WIDTH = 300;
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

} // namespace Window
