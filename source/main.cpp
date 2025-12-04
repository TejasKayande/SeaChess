
#include "base.hpp"
#include "window.hpp"
#include "chess.hpp"
#include "render.hpp"

#include <raylib.h>

global Chess::Board G_board;

void update() {

    // NOTE(Tejas): Left Mouse Button
    if (IsMouseButtonPressed(0)) {
        int x = GetMouseX();
        int y = GetMouseY();
        Window::SectionID si = Window::getSectionID(x, y);
        std::cout << (si) << std::endl;
    }
}

void render() {
    
    Render::renderBoard(Window::getBoardSection(), G_board);
    Render::renderStatus(Window::getStatusSection());
    Render::renderInfo(Window::getInformationSection());

    if (Window::isOnMenu()) {
        Render::renderMenu(Window::getMenuSection());
    }
}

auto main(void) -> int {

    InitWindow(Window::WINDOW_WIDTH, Window::WINDOW_HEIGHT, "Chess");

    while (!WindowShouldClose()) {

        if (IsKeyPressed(KEY_F)) Window::toggleMenu();

        update();

        BeginDrawing();
        BeginBlendMode(BLEND_ALPHA);
        {
            ClearBackground(BLACK);
            render();
        }
        EndDrawing();
    }

    CloseWindow();
    return 0;
}
