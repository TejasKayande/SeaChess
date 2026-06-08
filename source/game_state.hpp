#pragma once

#include "core/base.hpp"
#include "window.hpp"
#include "core/piece.hpp"
#include "core/square.hpp"
#include "core/board.hpp"
#include "core/move.hpp"
#include "core/movegen.hpp"

#include "render.hpp"
#include "assets.hpp"
#include "menu.hpp"
#include "status_bar.hpp"

#include <raylib.h>

namespace State {

    enum WindowEvent {
        NONE = 0,
        QUIT
    };

    class GameState {

    public:

        GameState();
        ~GameState();

        WindowEvent update();
        void render();

    private:

        bool m_running;

        Chess::Square m_sel_square;
        bool m_is_board_flipped;

        Chess::Board   *m_board;

        // NOTE(Tejas): List of current legal moves for the selected piece, if any.
        MoveList m_move_list; 

        Move m_last_move; 

        Theme m_theme;

        // NOTE(Tejas): This is so we can keep track if we are in a sub menu.
        Menu::Menu *m_current_menu;

        Menu::Menu m_main_menu;
        Menu::Menu m_theme_menu;

    private:

        Render::Visual m_buildVisual();

        void m_setTheme(Theme theme);
    };

} // namespace Game
