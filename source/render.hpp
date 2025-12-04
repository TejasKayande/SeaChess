#pragma once

#include "base.hpp"
#include "window.hpp"
#include "chess.hpp"

#include <raylib.h>

namespace Render {

    void renderBoard(const Window::Section &area, const Chess::Board& board);
    void renderMenu(const Window::Section &area);
    void renderInfo(const Window::Section &area);
    void renderStatus(const Window::Section &area);

} // namespace Render
