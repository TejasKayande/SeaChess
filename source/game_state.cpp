
#include "game_state.hpp"

#include "../assets/sound/move.h"
#include "../assets/sound/capture.h"
#include "../assets/sound/castle.h"

using namespace State;

GameState::GameState() {

    m_is_board_flipped = true;  // flipped board mean white is at the bottom
    m_sel_square = Chess::Square::invalid();

    m_board  = new Chess::Board();

    m_move_list = MoveList();

    Render::initAssets();
    MoveGen::init();

    ::Wave move_wav    = ::LoadWaveFromMemory(".wav", __move_wav, __move_wav_len);
    ::Wave capture_wav = ::LoadWaveFromMemory(".wav", __capture_wav, __capture_wav_len);
    ::Wave castle_wav  = ::LoadWaveFromMemory(".wav", __castle_wav, __castle_wav_len);

    m_move_sound    = ::LoadSoundFromWave(move_wav);
    m_capture_sound = ::LoadSoundFromWave(capture_wav);
    m_castle_sound  = ::LoadSoundFromWave(castle_wav);

    UnloadWave(move_wav);
    UnloadWave(capture_wav);
    UnloadWave(castle_wav);
}

GameState::~GameState() {

    UnloadSound(m_castle_sound);
    UnloadSound(m_capture_sound);
    UnloadSound(m_move_sound);

    Render::deinitAssets();

    delete m_board;
}

void GameState::update() {

    // TODO(Tejas): Seperate updation for Menu game Game, and perhaps the Status and
    //              Information sections as well

    if (::IsKeyPressed(KEY_X)) Window::toggleMenu();
    if (::IsKeyPressed(KEY_F)) m_is_board_flipped = !m_is_board_flipped;

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
                                    ::PlaySound(m_castle_sound);
                                    
                                } break;

                                case Move::CAPTURE:
                                case Move::PROMO_CAPTURE_KNIGHT:
                                case Move::PROMO_CAPTURE_BISHOP:
                                case Move::PROMO_CAPTURE_ROOK:
                                case Move::PROMO_CAPTURE_QUEEN:
                                case Move::EN_PASSANT: {
                                    ::PlaySound(m_capture_sound);
                                } break;

                                default: {
                                    ::PlaySound(m_move_sound);
                                    break;
                                } break;
                            }
                        }

                        m_move_list.clear();
                        m_sel_square = Chess::Square::invalid();

                        if (MoveGen::Legal::isCheckmate(m_board, m_board->getTurn())) {
                            std::cout << "Checkmate! Player " << ((m_board->getTurn() == Chess::Player::LIGHT) ? "Light" : "Dark") << " wins!" << std::endl;
                        }
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
}

void GameState::render() {

    if (Window::isOnMenu()) {

        Render::renderMenu(Window::getMenuSection());

    } else {

        Render::Visual visual = _buildVisual();

        Render::renderBoard(Window::getBoardSection(), visual);
        Render::renderStatus(Window::getStatusSection());
        Render::renderInfo(Window::getInformationSection());
    }
}

Render::Visual GameState::_buildVisual() {

    Render::Visual visual = { };
    visual.theme = Themes::EMERALD;
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