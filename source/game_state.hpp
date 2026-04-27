#pragma once

#include "base.hpp"
#include "window.hpp"
#include "chess.hpp"
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
