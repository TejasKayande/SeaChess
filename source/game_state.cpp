
#include "game_state.hpp"

using namespace State;

GameState::GameState() {

    m_board  = new Chess::Board();
    m_visual = new Render::Visual();

    m_move_list = MoveList();

    Render::initAssets();
    MoveGen::init();
}

GameState::~GameState() {

    Render::deinitAssets();

    delete m_board;
    delete m_visual;
}

void GameState::update() {

    // TODO(Tejas): Seperate updation for Menu game Game, and perhaps the Status and
    //              Information sections as well

    if (::IsKeyPressed(KEY_X)) Window::toggleMenu();
    if (::IsKeyPressed(KEY_F)) m_visual->is_board_flipped = !m_visual->is_board_flipped;

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
                    m_move_list.clear();
                    MoveGen::Legal::generateMovesForSquare(m_board, sq, m_move_list);
                    m_visual->legal_squares = MoveGen::PseudoLegal::convertMoveListToBitBoard(m_move_list);
                }
            }

            // NOTE(Tejas): if a piece was already selected
            else {

                if (sq.isSquareOnBitBoard(m_visual->legal_squares)) {

                    // TODO(Tejas): This is pretty inefficient, we can optimize
                    //              it by storing the legal moves in a hashset
                    //              or something...
                    for (const Move &move : m_move_list) {
                        if (move.to == sq) {
                            if (m_board->makeMove(move)) {
                                m_visual->legal_squares = 0;
                                m_board->changeTurn();
                            }
                            m_visual->selected_square = Chess::Square::invalid();
                            break;
                        }
                    }
                }
            }
        }
    }

    // NOTE(Tejas): Right Mouse Button
    if (::IsMouseButtonPressed(1)) {
        m_visual->selected_square = Chess::Square::invalid();
        m_visual->legal_squares = 0;
    }

    if (MoveGen::Legal::inCheck(m_board, m_board->getTurn())) {
        std::cout << "Player " << ((m_board->getTurn() == Chess::Player::LIGHT) ? "Light" : "Dark") << " is in Check!" << std::endl;
    }
}

void GameState::render() {

    if (Window::isOnMenu()) {
        Render::renderMenu(Window::getMenuSection());
    } else {
        Render::renderBoard(Window::getBoardSection(), m_board, m_visual);
        Render::renderStatus(Window::getStatusSection());
        Render::renderInfo(Window::getInformationSection());
    }
}
