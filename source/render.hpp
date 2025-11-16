#pragma once

#include "base.hpp"
#include "window.hpp"
#include "chess.hpp"

#include <raylib.h>

namespace Render {

    

    void renderBoard(const Section &area, const Chess::Board& board);
    void renderMenu(const Section &area);
    void renderInfo(const Section &area);
    void renderStatus(const Section &area);

} // namespace Render
