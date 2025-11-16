
#include "base.hpp"
#include "window.hpp"

#include <raylib.h>

auto main(void) -> int {

    Window window;

    while (!WindowShouldClose()) {

        if (IsKeyPressed(KEY_F)) window.toggleMenu();

        BeginDrawing();
        BeginBlendMode(BLEND_ALPHA);
        {
            ClearBackground(BLACK);

            DrawRectangleRec(window.getBoardSection(), BROWN);
            DrawRectangleRec(window.getStatusSection(), GREEN);
            DrawRectangleRec(window.getInformationSection(), YELLOW);

            if (window.isOnMenu()) {
                Color menu_background = Color(0x44, 0x44, 0x44, 0x99);
                DrawRectangleRec(window.getMenuSection(), menu_background);
            }
        }
        EndDrawing();
    }

    printf("Hello World"); 
    return 0;
}
