
#include "base.hpp"
#include "window.hpp"
#include "chess.hpp"
#include "render.hpp"

#include <raylib.h>

auto main(void) -> int {

    InitWindow(Window::WINDOW_WIDTH, Window::WINDOW_HEIGHT, "Chess");

    Chess::Board b;

    while (!WindowShouldClose()) {

        if (IsKeyPressed(KEY_F)) Window::toggleMenu();

        BeginDrawing();
        BeginBlendMode(BLEND_ALPHA);
        {
            ClearBackground(BLACK);

            Render::renderBoard(Window::getBoardSection(), b);
            Render::renderStatus(Window::getStatusSection());
            Render::renderInfo(Window::getInformationSection());

            if (Window::isOnMenu()) {
                Render::renderMenu(Window::getMenuSection());
            }
        }
        EndDrawing();
    }

    CloseWindow();

    printf("Hello World"); 
    return 0;
}
