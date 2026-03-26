#include "base.hpp"
#include "window.hpp"
#include "chess.hpp"
#include "render.hpp"
#include "game_state.hpp"

#include <raylib.h>

/*
TODO(Tejas):
- [ ] Abstract Input like in Text-Editor project.
- [ ] Create a Config/Setting file for constants.
*/

void update(State::GameState *gs) {

    if (::IsKeyPressed(KEY_X)) Window::toggleMenu();
    if (::IsKeyPressed(KEY_F)) gs->m_visual->is_board_flipped = !gs->m_visual->is_board_flipped;

    if (Window::isOnMenu()) {
    } else {
        gs->update();   
    }
}

void render(State::GameState *gs) {

    if (Window::isOnMenu()) {
        Render::renderMenu(Window::getMenuSection());
    } else {
        Render::renderBoard(Window::getBoardSection(), gs->m_board, gs->m_visual);
        Render::renderStatus(Window::getStatusSection());
        Render::renderInfo(Window::getInformationSection());
    }
}

auto main(void) -> int {

    ::InitWindow(Window::WINDOW_WIDTH, Window::WINDOW_HEIGHT, "Chess");

    State::GameState *gs = new State::GameState();

    Render::initAssets();

    while (!::WindowShouldClose()) {

        update(gs);

        ::BeginDrawing();
        ::BeginBlendMode(BLEND_ALPHA);
        {
            ::ClearBackground(BLACK);
            render(gs);
        }
        ::EndDrawing();
    }

    Render::deinitAssets();

    delete gs;

    ::CloseWindow();

    return 0;
}
