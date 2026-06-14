#include "window.hpp"
#include "render.hpp"
#include "game_state.hpp"
#include "core/board.hpp"
#include "core/movegen.hpp"

#include <raylib.h>

/*
TODO(Tejas):
- [ ] Create a Config/Setting file for constants.

- [ ] No dynamic datastructures for move generation.
- [ ] Fix Fen parsing for castling rights and en-passant square.

- [ ] Make the window resizable.
*/

auto main(void) -> int {

    ::InitWindow(Window::WINDOW_WIDTH, Window::WINDOW_HEIGHT, "Chess");
    ::InitAudioDevice();

    ::SetExitKey(KEY_NULL);

    State::GameState gs;
    bool running = true;

    while (!::WindowShouldClose() && running) {

        if (gs.update() == State::WindowEvent::QUIT) running = false;

        ::BeginDrawing();
        ::BeginBlendMode(BLEND_ALPHA);
        {
            ::ClearBackground(BLACK);
            gs.render();
        }
        ::EndDrawing();
    }

    ::CloseWindow();
    return 0;
}