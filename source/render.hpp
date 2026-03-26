#pragma once

#include "base.hpp"
#include "window.hpp"
#include "chess.hpp"

#include <raylib.h>

namespace Render {

    // TODO(Tejas): Should we have functions to modify these values? I dont know...
    struct Visual {
        Chess::Square selected_square;
        bool          is_board_flipped;
    };

    void initAssets();
    void deinitAssets();

    void renderBoard(const Window::Section &area, const Chess::Board *board, const Visual *visual);
    void renderMenu(const Window::Section &area);
    void renderInfo(const Window::Section &area);
    void renderStatus(const Window::Section &area);

} // namespace Render
