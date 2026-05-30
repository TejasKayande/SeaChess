#include "window.hpp"
#include "render.hpp"
#include "game_state.hpp"
#include "core/board.hpp"
#include "core/movegen.hpp"

#include "core/perft.hpp"

#include <raylib.h>


/*
TODO(Tejas):
- [ ] Create a Config/Setting file for constants.

- [ ] Add Magic Bitboards for move generation of sliding pieces
- [ ] No dynamic datastructures for move generation.
- [ ] Fix Fen parsing for castling rights and en-passant square.

- [ ] Make the window resizable.
- [ ] Add Board themes and menu for selecting them.
*/

#define RUN_PERFTEST 0

#if RUN_PERFTEST
#include "core/perft.hpp"
#include <thread>
bool runPerftest() {

    using namespace PerfTest;
    
    int depth = 6;
    std::thread perft_thread(PerfTest::runPerftest, 6);
    perft_thread.join();

    return true;
}
#else
bool runPerftest() {
    return false;
}
#endif

auto main(void) -> int {

    {
        if (runPerftest()) exit(0);
    }

    ::SetTraceLogLevel(LOG_ERROR);
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
