#include "window.hpp"
#include "render.hpp"
#include "game_state.hpp"
#include "core/board.hpp"
#include "core/movegen.hpp"

#include <raylib.h>

/*
TODO(Tejas):
- [ ] Abstract Input like in Text-Editor project.
- [ ] Create a Config/Setting file for constants.
- [ ] Abstract Chess Core into seperate module.

- [ ] For a clicked piece we only need to generate the legal moves for that piece
- [ ] Add Magic Bitboards for move generation of sliding pieces
- [ ] No dynamic datastructures for move generation.
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
