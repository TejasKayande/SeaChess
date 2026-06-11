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

- [ ] No dynamic datastructures for move generation.
- [ ] Fix Fen parsing for castling rights and en-passant square.

- [ ] Make the window resizable.
*/


// TODO(Tejas): Setup a perft test suite with different positions.
#define RUN_PERFTEST 0

#if RUN_PERFTEST
#include "core/perft.hpp"
bool runPerftest() {

    using namespace PerfTest;

    Chess::Board* board = new Chess::Board();
    board->setFen("r3k2r/p1ppqpb1/bn2pnp1/3PN3/1p2P3/2N2Q1p/PPPBBPPP/R3K2R w KQkq -");

    // board->setFen("r3k2r/p2pqpb1/bnp1pnp1/3PN3/1p2P3/P1N2Q1p/1PPBBPPP/2KR3R b kq -");
    
    int depth = 4;
    PerfTest::runPerftest(board, depth);

    delete board;
    return true;
}
#else
bool runPerftest() { return false; }
#endif

auto main(void) -> int {

    {
        if (runPerftest()) return 0;
    }

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