#include "base.hpp"
#include "window.hpp"
#include "chess.hpp"
#include "render.hpp"

#include <raylib.h>

global Chess::Board   *G_board;
global Render::Visual G_visual;

void updateMenu() {}

void updateGame() {

    // NOTE(Tejas): Left Mouse Button
    if (::IsMouseButtonPressed(0)) {
        int x = ::GetMouseX();
        int y = ::GetMouseY();

        Chess::Square sq = Window::getSquare(x, y);

        if (sq.isValid()) {
            Chess::Piece pc = G_board->getPieceAt(sq);

            // NOTE(Tejas): if no piece was selected
            if (!G_visual.sel_square.isValid()) {
                if (!pc.isEmpty() && pc.isColor(G_board->getTurn())) {
                    G_visual.sel_square = sq;
                }
            }

            // NOTE(Tejas): if a piece was already selected
            else {
                G_board->move(G_visual.sel_square, sq);
                G_visual.sel_square = Chess::Square::invalid();
                G_board->changeTurn();
            }
        }
    }

    // NOTE(Tejas): Left Mouse Button
    if (::IsMouseButtonPressed(1)) {
        // G_board->changeTurn();
        G_visual.sel_square = Chess::Square::invalid();
    }
}

void update() {
    
    if (::IsKeyPressed(KEY_X)) Window::toggleMenu();
    if (::IsKeyPressed(KEY_F)) G_visual.board_flipped = !G_visual.board_flipped;

    if (Window::isOnMenu()) {
        updateMenu();   
    } else {
        updateGame();   
    }
}

void render() {
    
    Render::renderBoard(Window::getBoardSection(), *G_board, G_visual);
    Render::renderStatus(Window::getStatusSection());
    Render::renderInfo(Window::getInformationSection());

    if (Window::isOnMenu()) {
        Render::renderMenu(Window::getMenuSection());
    }
}

auto main(void) -> int {

    ::InitWindow(Window::WINDOW_WIDTH, Window::WINDOW_HEIGHT, "Chess");
    G_board = new Chess::Board();

    while (!::WindowShouldClose()) {

        update();

        ::BeginDrawing();
        ::BeginBlendMode(BLEND_ALPHA);
        {
            ::ClearBackground(BLACK);
            render();
        }
        ::EndDrawing();
    }

    delete G_board;
    ::CloseWindow();

    return 0;
}
