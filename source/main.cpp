#include "base.hpp"
#include "window.hpp"
#include "chess.hpp"
#include "render.hpp"
#include "game_state.hpp"
#include "movegen.hpp"

#include <raylib.h>

/*
TODO(Tejas):
- [ ] Abstract Input like in Text-Editor project.
- [ ] Create a Config/Setting file for constants.
*/

auto main(void) -> int {

    ::InitWindow(Window::WINDOW_WIDTH, Window::WINDOW_HEIGHT, "Chess");

    State::GameState *gs = new State::GameState();

    while (!::WindowShouldClose()) {

        gs->update();

        ::BeginDrawing();
        ::BeginBlendMode(BLEND_ALPHA);
        {
            ::ClearBackground(BLACK);
            gs->render();
        }
        ::EndDrawing();
    }

    delete gs;

    ::CloseWindow();

    return 0;
}
