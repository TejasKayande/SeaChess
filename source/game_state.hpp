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

    public:
        // TODO(Tejas): Do we want to make this private??
        Chess::Board   *m_board;
        Render::Visual *m_visual;
    };

} // namespace Game
