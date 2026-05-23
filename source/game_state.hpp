#pragma once

#include "core/base.hpp"
#include "window.hpp"
#include "core/piece.hpp"
#include "core/square.hpp"
#include "core/board.hpp"
#include "core/move.hpp"
#include "core/movegen.hpp"
#include "render.hpp"

namespace State {

    class GameState {

    public:

        GameState();
        ~GameState();

        void update();
        void render();

    private:
        Chess::Board   *m_board;
        Render::Visual *m_visual;
    };

} // namespace Game
