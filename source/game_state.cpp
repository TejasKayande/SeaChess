
#include "game_state.hpp"

using namespace State;

GameState::GameState() {

    m_board = new Chess::Board();
    m_visual = new Render::Visual;
}

GameState::~GameState() {

    delete m_board;
    delete m_visual;
}

void GameState::update() {

    if (::IsKeyPressed(KEY_F))
        m_visual->is_board_flipped = !m_visual->is_board_flipped;

    
    if (::IsMouseButtonPressed(0)) {

        int x = ::GetMouseX();
        int y = ::GetMouseY();

        Chess::Square sq = Window::getSquare(x, y, m_visual->is_board_flipped);

        if (sq.isValid()) {

            Chess::Piece pc = m_board->getPieceAt(sq);

            // NOTE(Tejas): if no piece was selected
            if (!m_visual->selected_square.isValid()) {

                if (!pc.isEmpty() && pc.isColor(m_board->getTurn())) {

                    m_visual->selected_square = sq;
                }
            }

            // NOTE(Tejas): if a piece was already selected
            else {

                Chess::Square f_sq(sq.rank(), sq.file());

                m_board->move(m_visual->selected_square, sq);
                m_visual->selected_square = Chess::Square::invalid();
                m_board->changeTurn();
            }
        }
    }

    // NOTE(Tejas): Right Mouse Button
    if (::IsMouseButtonPressed(1)) {
        m_visual->selected_square = Chess::Square::invalid();
    }
}
