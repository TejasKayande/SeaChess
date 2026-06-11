#include "window.hpp"
#include "render.hpp"
#include "game_state.hpp"
#include "core/board.hpp"
#include "core/movegen.hpp"

#include "core/perft.hpp"

#include <raylib.h>

#ifdef __EMSCRIPTEN__
#include <emscripten/emscripten.h>
#endif

/*
TODO(Tejas):
- [ ] Create a Config/Setting file for constants.

- [ ] Add Magic Bitboards for move generation of sliding pieces
- [ ] No dynamic datastructures for move generation.
- [ ] Fix Fen parsing for castling rights and en-passant square.

- [ ] Make the window resizable.
- [ ] Add Board themes and menu for selecting them.
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

static State::GameState *gs = nullptr;
static bool running;

void mainLoop(void) {

    if (gs->update() == State::WindowEvent::QUIT) running = false;

    ::BeginDrawing();
    ::BeginBlendMode(BLEND_ALPHA);
    {
        ::ClearBackground(BLACK);
        gs->render();
    }
    ::EndDrawing();
}

auto main(void) -> int {

    {
        if (runPerftest()) return 0;
    }

    ::InitWindow(Window::WINDOW_WIDTH, Window::WINDOW_HEIGHT, "Chess");
    ::InitAudioDevice();

    gs = new State::GameState();
    running = true;

#ifdef __EMSCRIPTEN__
    emscripten_set_main_loop(mainLoop, 0, true);
#else
    while (!::WindowShouldClose() && running) {

        mainLoop();

        // if (gs->update() == State::WindowEvent::QUIT) running = false;

        // ::BeginDrawing();
        // ::BeginBlendMode(BLEND_ALPHA);
        // {
        //     ::ClearBackground(BLACK);
        //     gs->render();
        // }
        // ::EndDrawing();
    }
#endif

    delete gs;

    ::CloseWindow();
    return 0;
}
// a1a2: 44  b2b3: 43
// a1b1: 44  g2g3: 43
// a1c1: 44  a3a4: 43
// a1d1: 44  d5d6: 40
// a3a4: 43  g2g4: 43
// a3b4: 42  g2h3: 44
// b2b3: 43  a3b4: 42
// c3a2: 43  d5e6: 46
// c3a4: 43  d5c6: 43
// c3b1: 43  c3b1: 43
// c3b5: 41  c3d1: 43
// c3d1: 43  c3a2: 43
// d2c1: 44  c3a4: 43
// d2e3: 44  c3b5: 41
// d2f4: 44  e5d3: 44
// d2g5: 43  e5c4: 43
// d2h6: 42  e5g4: 45
// d5c6: 43  e5c6: 42
// d5d6: 40  e5g6: 43
// d5e6: 46  e5d7: 46
// e1c1: 46  e5f7: 45
// e1d1: 44  d2c1: 44
// e1f1: 44  d2e3: 44
// e1g1: 44  d2f4: 44
// e2a6: 37  d2g5: 43
// e2b5: 42  d2h6: 42
// e2c4: 42  e2d1: 45
// e2d1: 45  e2f1: 45
// e2d3: 43  e2d3: 43
// e2f1: 45  e2c4: 42
// e5c4: 43  e2b5: 42
// e5c6: 42  e2a6: 37
// e5d3: 44  a1b1: 44
// e5d7: 46  a1c1: 44
// e5f7: 45  a1d1: 44
// e5g4: 45  a1a2: 44
// e5g6: 43  h1f1: 44
// f3d3: 43  h1g1: 44
// f3e3: 44  f3d3: 43
// f3f4: 44  f3e3: 44
// f3f5: 46  f3g3: 44
// f3f6: 40  f3h3: 44
// f3g3: 44  f3f4: 44
// f3g4: 44  f3g4: 44
// f3h3: 44  f3f5: 46
// f3h5: 44  f3h5: 44
// g2g3: 43  f3f6: 40
// g2g4: 43  e1d1: 44
// g2h3: 44  e1f1: 44
// h1f1: 44  e1g1: 44
// h1g1: 44  e1c1: 44