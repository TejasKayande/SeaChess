#pragma once

#include "core/base.hpp"
#include "window.hpp"
#include "core/piece.hpp"
#include "core/square.hpp"
#include "core/board.hpp"
#include "core/move.hpp"
#include "core/movegen.hpp"
#include "render.hpp"

#include <raylib.h>

namespace State {

    class GameState {

    public:

        GameState();
        ~GameState();

        void update();
        void render();

    private:

        Chess::Square m_sel_square;
        bool m_is_board_flipped;

        Chess::Board   *m_board;

        // NOTE(Tejas): List of current legal moves for the selected piece, if any.
        MoveList m_move_list; 

        // TODO(Tejas): Im not sure if this belongs here, and if not I dont know
        //              where it should go.
        ::Sound m_move_sound;
        ::Sound m_capture_sound;
        ::Sound m_castle_sound;

    private:
        Render::Visual _buildVisual();
    };

} // namespace Game
