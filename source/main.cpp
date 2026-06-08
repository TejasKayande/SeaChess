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


// TODO(Tejas): Setup a perft test suite with different positions.
#define RUN_PERFTEST 0

#if RUN_PERFTEST
#include "core/perft.hpp"
bool runPerftest() {

    using namespace PerfTest;

    Chess::Board* board = new Chess::Board();
    board->setFen("r3k2r/Pppp1ppp/1b3nbN/nP6/BBP1P3/q4N2/Pp1P2PP/R2Q1RK1 w kq -");
    
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

    State::GameState *gs = new State::GameState();

    bool running = true;

    while (!::WindowShouldClose() && running) {

        if (gs->update() == State::WindowEvent::QUIT) running = false;

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

// SEEMS LIKE THE ENGINE IS GENERATION RIGHT AMOUNT OF MOVES...
// I THOUGHT THIS WAS GOING TO TAKE A LONG TIME...
// WE DID IT LIKE IN 28 mins. THATS GOOD>

// BRB...