#pragma once

#include "core/base.hpp"
#include "window.hpp"
#include "core/square.hpp"
#include "core/piece.hpp"
#include "core/board.hpp"

#include <raylib.h>

struct Theme {

    ::Color board_dark;
    ::Color board_light;

    ::Color highlight;
    ::Color legal;

    ::Color check;

    ::Color menu_bg;
};

namespace Themes {

    inline constexpr Theme DEFAULT = {
        .board_dark  = ::Color{0x44, 0x44, 0x44, 0xFF},
        .board_light = ::Color{0xEE, 0xEE, 0xEE, 0xFF},
        .highlight   = ::Color{255, 255, 0, 200},
        .legal       = ::Color{255, 0, 255, 200},
        .check       = ::Color{255, 0, 0, 200},
        .menu_bg     = ::Color{0x44, 0x44, 0x44, 0x99}
    };

    inline constexpr Theme CLASSIC_WOOD = {
        .board_dark  = ::Color{181, 136, 99, 255},
        .board_light = ::Color{240, 217, 181, 255},
        .highlight   = ::Color{255, 215, 0, 180},
        .legal       = ::Color{50, 205, 50, 180},
        .check       = ::Color{220, 20, 60, 200},
        .menu_bg     = ::Color{40, 30, 20, 180}
    };

    inline constexpr Theme SLATE_BLUE = {
        .board_dark  = ::Color{74, 92, 122, 255},
        .board_light = ::Color{222, 227, 230, 255},
        .highlight   = ::Color{255, 193, 7, 180},
        .legal       = ::Color{0, 188, 212, 180},
        .check       = ::Color{244, 67, 54, 200},
        .menu_bg     = ::Color{30, 41, 59, 180}
    };

    inline constexpr Theme EMERALD = {
        .board_dark  = ::Color{46, 94, 62, 255},
        .board_light = ::Color{225, 235, 220, 255},
        .highlight   = ::Color{255, 202, 40, 180},
        .legal       = ::Color{76, 175, 80, 180},
        .check       = ::Color{229, 57, 53, 200},
        .menu_bg     = ::Color{22, 44, 29, 180}
    };

} // namespace Themes

namespace Render {

    enum HighlightType {
        NONE     = 1 << 0,
        SELECTED = 1 << 1,
        LEGAL    = 1 << 2,
        CHECK    = 1 << 3,
    };

    struct SquareVisual {
        Chess::Piece piece;
        int rank, file; // this is for the coords.
        u8 flag;
    };

    struct Visual {
        SquareVisual board[Chess::MAX_RANK * Chess::MAX_FILE];
        Theme theme;
        bool show_legal;
        bool show_sel;
        bool show_check;
    };

    void initAssets();
    void deinitAssets();

    void renderBoard(const Window::Section &area, const Visual &visual);

    // void renderBoard(const Window::Section &area, const Chess::Board *board, const Visual *visual);
    void renderMenu(const Window::Section &area);
    void renderInfo(const Window::Section &area);
    void renderStatus(const Window::Section &area);

} // namespace Render
