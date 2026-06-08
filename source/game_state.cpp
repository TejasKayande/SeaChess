
#include "game_state.hpp"
#include "engine/engine.hpp"

using namespace State;

GameState::GameState() {

    m_is_board_flipped = true;  // flipped board mean white is at the bottom
    m_sel_square = Chess::Square::invalid();

    m_board  = new Chess::Board();

    m_move_list = MoveList();

    Assets::init();
    MoveGen::init();

    m_last_move = Move();

    m_theme = Themes::DEFAULT;

    m_theme_menu = Menu::Menu("Theme Menu", {
        { 
            "Default", 
            [this]() { 
                m_setTheme(Themes::DEFAULT); 
                m_current_menu = &m_main_menu;
                Window::toggleMenu();
            }, 
            nullptr 
        },

        { 
            "Classic Wood", 
            [this]() { 
                m_setTheme(Themes::CLASSIC_WOOD); 
                m_current_menu = &m_main_menu;
                Window::toggleMenu();
            }, 
            nullptr 
        },

        { 
            "Slate Blue", 
            [this]() { 
                m_setTheme(Themes::SLATE_BLUE); 
                m_current_menu = &m_main_menu;
                Window::toggleMenu();
            }, 
            nullptr 
        },

        { 
            "Emerald", 
            [this]() { 
                m_setTheme(Themes::EMERALD); 
                m_current_menu = &m_main_menu;
                Window::toggleMenu();
            }, 
            nullptr 
        },

        { 
            "Back to Main Menu", 
            [this]() { 
                m_current_menu = &m_main_menu; 
            }, 
            nullptr 
        }
    });

    m_main_menu = Menu::Menu("Main Menu", {
        { 
            "New Game", 
            [this]() { 
                m_board->reset(); 
            }, 
            nullptr 
        },

        { 
            "Load Fen", 
            [this]() { 
                std::cout << "Load Fen!" << std::endl; 
            }, 
            nullptr 
        },

        { "Themes", []() { }, &m_theme_menu },

        { 
            "Quit", 
            [this]() { 
                m_running = false; 
            }, 
            nullptr 
        }
    });

    m_current_menu = &m_main_menu;

    m_running = true;
}

GameState::~GameState() {

    Assets::deinit();
    delete m_board;
}

WindowEvent GameState::update() {

    // TODO(Tejas): Seperate updation for Menu game Game, and perhaps the Status and
    //              Information sections as well

    if (!m_running) return WindowEvent::QUIT;

    if (::IsKeyPressed(KEY_X)) Window::toggleMenu();
    if (::IsKeyPressed(KEY_F)) m_is_board_flipped = !m_is_board_flipped;

    if (Window::isOnMenu()) {
        if (::IsKeyPressed(KEY_UP))   m_current_menu->prev();
        if (::IsKeyPressed(KEY_DOWN)) m_current_menu->next();

        if (::IsKeyPressed(KEY_ENTER)) {
            Menu::MenuItem item = m_current_menu->getItems()[m_current_menu->getPtr()];
            if (item.proc) item.proc();
            if (item.sub_menu) m_current_menu = item.sub_menu;
        }
    }

    if (::IsKeyPressed(KEY_LEFT)) {
        if (m_board->unMakeMove(m_last_move)) {
            m_board->changeTurn();
            m_move_list.clear();
            m_last_move = Move();
            m_sel_square = Chess::Square::invalid();
        }
    }

    bool move_made = false;

    if (::IsKeyPressed(KEY_UP)) {
        if (m_board->getTurn() == Chess::Player::DARK) {
            Move best_move = Engine::getBestMove(m_board);
            m_board->makeMove(best_move);
            m_board->changeTurn();
            m_move_list.clear();
            m_sel_square = Chess::Square::invalid();
            m_last_move = best_move;
            move_made = true;
            ::PlaySound(Assets::MOVE_SOUND);
        }
    }

    if (::IsMouseButtonPressed(0)) {

        int x = ::GetMouseX();
        int y = ::GetMouseY();

        Chess::Square sq = Window::getSquare(x, y, m_is_board_flipped);

        if (sq.isValid()) {

            Chess::Piece pc = m_board->getPieceAt(sq);

            // NOTE(Tejas): if no piece was selected
            if (!m_sel_square.isValid()) {

                if (!pc.isEmpty() && pc.isColor(m_board->getTurn())) {

                    m_sel_square = sq;
                    m_move_list.clear();
                    MoveGen::Legal::generateMovesForSquare(m_board, sq, m_move_list);
                }
            }

            // NOTE(Tejas): if a piece was already selected
            else {
                // TODO(Tejas): This is pretty inefficient, we can optimize
                //              it by storing the legal moves in a hashset
                //              or something...
                for (const Move &move : m_move_list) {
                    if (move.to == sq) {
                        if (m_board->makeMove(move)) {

                            m_board->changeTurn();

                            switch (move.type) {
                                case Move::KING_CASTLE:
                                case Move::QUEEN_CASTLE: {
                                    ::PlaySound(Assets::CASTLE_SOUND);
                                    
                                } break;

                                case Move::CAPTURE:
                                case Move::PROMO_CAPTURE_KNIGHT:
                                case Move::PROMO_CAPTURE_BISHOP:
                                case Move::PROMO_CAPTURE_ROOK:
                                case Move::PROMO_CAPTURE_QUEEN:
                                case Move::EN_PASSANT: {
                                    ::PlaySound(Assets::CAPTURE_SOUND);
                                } break;

                                default: {
                                    ::PlaySound(Assets::MOVE_SOUND);
                                    break;
                                } break;
                            }
                        }

                        move_made = true;

                        m_move_list.clear();
                        m_sel_square = Chess::Square::invalid();

                        m_last_move = move;
                        break;
                    }
                }
            }
        }
    }

    // NOTE(Tejas): Right Mouse Button
    if (::IsMouseButtonPressed(1)) {
        m_sel_square = Chess::Square::invalid();
        m_move_list.clear();
    }

    if (move_made) {
        if (MoveGen::Legal::isCheckmate(m_board, m_board->getTurn())) {
            std::cout << "Checkmate! Player " << ((m_board->getTurn() == Chess::Player::LIGHT) ? "Dark" : "Light") << " wins!" << std::endl;
        }
    }

    return WindowEvent::NONE;
}

void GameState::render() {

    if (Window::isOnMenu()) {

        Render::renderMenu(Window::getMenuSection(), m_current_menu, m_theme);

    } else {

        Render::Visual visual = m_buildVisual();

        Render::renderBoard(Window::getBoardSection(), visual);
        Render::renderStatus(Window::getStatusSection());
        Render::renderInfo(Window::getInformationSection());
    }
}

Render::Visual GameState::m_buildVisual() {

    Render::Visual visual = { };
    visual.theme = m_theme;
    visual.show_legal = true;
    visual.show_sel   = true;
    visual.show_check = true;

    BitBoard legal_bb = MoveGen::convertMoveListToBitBoard(m_move_list);
    int idx = 0, didx = 1;
    Chess::Square sel_square = m_sel_square;
    if (m_is_board_flipped) {
        legal_bb = MoveGen::flipBitBoard(legal_bb);
        idx = (Chess::MAX_RANK * Chess::MAX_FILE) - 1;
        didx = -1;
        sel_square = Chess::Square((Chess::MAX_RANK - 1) - m_sel_square.rank(), (Chess::MAX_FILE - 1) - m_sel_square.file());
    }

    for (int i = 0; i < Chess::MAX_RANK * Chess::MAX_FILE; i++, idx += didx) {
        Chess::Square sq(i);
        visual.board[idx].piece = m_board->getPieceAt(sq);
        visual.board[idx].rank = sq.rank();
        visual.board[idx].file = sq.file();

        if (sq == sel_square) visual.board[i].flag |= Render::SELECTED;
        if (legal_bb & (1ULL << i)) visual.board[i].flag |= Render::LEGAL;
    }

    return visual;
}

void GameState::m_setTheme(Theme theme) {
    m_theme = theme;
}