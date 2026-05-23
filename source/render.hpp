#pragma once

#include "core/base.hpp"
#include "window.hpp"
#include "core/square.hpp"
#include "core/piece.hpp"
#include "core/board.hpp"

#include <raylib.h>

namespace Render {

    // TODO(Tejas): Should we have functions to modify these values? I dont know...
    struct Visual {
        Chess::Square selected_square = Chess::Square::invalid();
        bool is_board_flipped = false;
        BitBoard legal_squares = 0ULL;
    };

    void initAssets();
    void deinitAssets();

    void renderBoard(const Window::Section &area, const Chess::Board *board, const Visual *visual);
    void renderMenu(const Window::Section &area);
    void renderInfo(const Window::Section &area);
    void renderStatus(const Window::Section &area);

} // namespace Render
