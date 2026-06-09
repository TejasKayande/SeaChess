
#include "render.hpp"

using namespace Render;

namespace { // Anonymous namespace for helper functions

    void renderSquareBackgroud(const Window::Section &area, Chess::Square sq, Theme theme) {

        if (!sq.isValid()) return;

        int px = area.x + sq.file() * Window::SQUARE_DIM;
        int py = area.y + sq.rank() * Window::SQUARE_DIM;

        ::Color square_color = ((sq.rank() + sq.file()) % 2 == 0) ? theme.board_light : theme.board_dark;
        ::DrawRectangle(px, py, Window::SQUARE_DIM, Window::SQUARE_DIM, square_color);
    }

    void renderSquareHighlight(const Window::Section &area, Chess::Square sq, ::Color color) {

        if (!sq.isValid()) return;

        int px = area.x + sq.file() * Window::SQUARE_DIM;
        int py = area.y + sq.rank() * Window::SQUARE_DIM;

        ::DrawRectangle(px, py, Window::SQUARE_DIM, Window::SQUARE_DIM, color);
    }

    void renderSquareCoord(const Window::Section &area, Chess::Square sq, int rank, int file, Theme theme) {

        // NOTE(Tejas): The Square passed here is just to get the position, it
        //              has no meaning in terms of actual square, this is just
        //              for the pixels

        if (!sq.isValid()) return;

        int px = area.x + sq.file() * Window::SQUARE_DIM;
        int py = area.y + sq.rank() * Window::SQUARE_DIM;

        int offset_from_square = 5; // in pixels

        ::Color text_color =  ((sq.rank() + sq.file()) % 2 == 0) ? theme.board_dark : theme.board_light;
        int xx = px + offset_from_square;
        int yy = py + offset_from_square;

        if (sq.file() == 0) {
            std::string ch = std::to_string(rank + 1);
            ::DrawTextEx(*theme.font, ch.c_str(), Vector2{(float)xx, (float)yy}, 24, 2, text_color);
        }

        if (sq.rank() == 7) {
            xx = px + Window::SQUARE_DIM - 15;
            yy = py + Window::SQUARE_DIM - 25;
            char ch[2] = { '\0' };
            ch[0] = (char)('h' - file);
            ::DrawTextEx(*theme.font, ch, Vector2{(float)xx, (float)yy}, 24, 2, text_color);
        }
    }

    void renderPieceOnSquare(const Window::Section &area, const Chess::Square &sq, const Chess::Piece &pc) {

        if (!sq.isValid()) return;
        if (pc.isEmpty())  return;

        int file = sq.file();
        int rank = sq.rank();

        int px = area.x + file * Window::SQUARE_DIM;
        int py = area.y + rank * Window::SQUARE_DIM;

        const ::Texture2D* tex = nullptr;

        if (pc.color() == Chess::Piece::LIGHT) {
            switch (pc.type()) {
            case Chess::Piece::PAWN:   tex = &Assets::LIGHT_PAWN;   break;
            case Chess::Piece::KNIGHT: tex = &Assets::LIGHT_KNIGHT; break;
            case Chess::Piece::BISHOP: tex = &Assets::LIGHT_BISHOP; break;
            case Chess::Piece::ROOK:   tex = &Assets::LIGHT_ROOK;   break;
            case Chess::Piece::QUEEN:  tex = &Assets::LIGHT_QUEEN;  break;
            case Chess::Piece::KING:   tex = &Assets::LIGHT_KING;   break;
            default: break;
            }
        } else if (pc.color() == Chess::Piece::DARK) {
            switch (pc.type()) {
            case Chess::Piece::PAWN:   tex = &Assets::DARK_PAWN;   break;
            case Chess::Piece::KNIGHT: tex = &Assets::DARK_KNIGHT; break;
            case Chess::Piece::BISHOP: tex = &Assets::DARK_BISHOP; break;
            case Chess::Piece::ROOK:   tex = &Assets::DARK_ROOK;   break;
            case Chess::Piece::QUEEN:  tex = &Assets::DARK_QUEEN;  break;
            case Chess::Piece::KING:   tex = &Assets::DARK_KING;   break;
            default: break;
            }
        }

        if (tex) ::DrawTexture(*tex, px, py, WHITE);
    }

    void renderPieceAtMouse(const Window::Section &area, const Chess::Piece &pc) {

        int mouse_x = ::GetMouseX();
        int mouse_y = ::GetMouseY();

        const ::Texture2D* tex = nullptr;

        if (pc.color() == Chess::Piece::LIGHT) {
            switch (pc.type()) {
            case Chess::Piece::PAWN:   tex = &Assets::LIGHT_PAWN;   break;
            case Chess::Piece::KNIGHT: tex = &Assets::LIGHT_KNIGHT; break;
            case Chess::Piece::BISHOP: tex = &Assets::LIGHT_BISHOP; break;
            case Chess::Piece::ROOK:   tex = &Assets::LIGHT_ROOK;   break;
            case Chess::Piece::QUEEN:  tex = &Assets::LIGHT_QUEEN;  break;
            case Chess::Piece::KING:   tex = &Assets::LIGHT_KING;   break;
            default: break;
            }
        } else if (pc.color() == Chess::Piece::DARK) {
            switch (pc.type()) {
            case Chess::Piece::PAWN:   tex = &Assets::DARK_PAWN;   break;
            case Chess::Piece::KNIGHT: tex = &Assets::DARK_KNIGHT; break;
            case Chess::Piece::BISHOP: tex = &Assets::DARK_BISHOP; break;
            case Chess::Piece::ROOK:   tex = &Assets::DARK_ROOK;   break;
            case Chess::Piece::QUEEN:  tex = &Assets::DARK_QUEEN;  break;
            case Chess::Piece::KING:   tex = &Assets::DARK_KING;   break;
            default: break;
            }
        }

        if (tex) ::DrawTexture(*tex, mouse_x - Window::SQUARE_DIM / 2, mouse_y - Window::SQUARE_DIM / 2, WHITE);
    }
} // Anonymous namespace

void Render::renderBoard(const Window::Section &area, const Visual &visual) {

    Chess::Piece piece_on_mouse = Chess::Piece::nopiece();

    for (int i = 0; i < Chess::MAX_RANK * Chess::MAX_FILE; i++) {

        renderSquareBackgroud(area, Chess::Square(i), visual.theme);

        Chess::Square sq(i);

        if (visual.board[i].flag & HighlightType::SELECTED) {
            renderSquareHighlight(area, sq, visual.theme.highlight);
            piece_on_mouse = visual.board[i].piece;
        } else if (visual.board[i].flag & HighlightType::LEGAL)  {
            renderSquareHighlight(area, sq, visual.theme.legal);
        } else if (visual.board[i].flag & HighlightType::CHECK) {
            renderSquareHighlight(area, sq, visual.theme.check);
        }

        renderSquareCoord(area, sq, visual.board[i].rank, visual.board[i].file, visual.theme);

        if (!(visual.board[i].flag & HighlightType::SELECTED)) renderPieceOnSquare(area, sq, visual.board[i].piece);
        else renderPieceAtMouse(area, visual.board[i].piece);
    }

    if (!piece_on_mouse.isEmpty()) {
        renderPieceAtMouse(area, piece_on_mouse);
    }
}

void Render::renderMenu(const Window::Section &area, const Menu::Menu *menu, Theme theme) {

    ::DrawRectangleRec(area, theme.menu_bg);
    // ::DrawLine(Window::WINDOW_WIDTH / 2, 0, Window::WINDOW_WIDTH / 2, Window::WINDOW_HEIGHT, ::Color{0, 0, 0, 255});

    int line_gap = 100;

    const int WINDOW_WIDTH_CENTER = Window::WINDOW_WIDTH / 2;
    int menu_title_width = ::MeasureTextEx(*theme.font, menu->getTitle().c_str(), 50, 2).x;

    ::DrawTextEx(*theme.font, menu->getTitle().c_str(),
                 Vector2{ (float)WINDOW_WIDTH_CENTER - (menu_title_width / 2), (float)50 },
                 50, 2, ::Color({255, 255, 255, 255}));

    ::Color text_color = ::Color(255, 255, 255, 230);
    int i = 1;
    for (Menu::MenuItem item : menu->getItems()) {

        ::Color text_color = ::Color({0, 255, 255, 255});

        // TODO(Tejas): This is a disgrace...
        if (i - 1 == menu->getPtr()) {
            text_color = ::Color({255, 0, 255, 255});
        }

        int width = ::MeasureTextEx(*theme.font, item.label.c_str(), 50, 2).x;

        ::DrawTextEx(*theme.font, item.label.c_str(),
                     Vector2{ (float)(Window::WINDOW_WIDTH / 2) - (width / 2), (float)50 + (line_gap * i)},
                     50, 2, text_color);
        i++;
    }
}

void Render::renderInfo(const Window::Section &area) {

    ::DrawRectangleRec(area, ::Color{100, 100, 100, 255});
}

void Render::renderStatus(const Window::Section &area, const StatusBar &status, Theme theme) {

    ::DrawRectangleRec(area, theme.menu_bg);

    std::string final;

    final += "Turn: " + status.turn + " | "  + 
             "Game Mode: " + status.game_mode + " | " + 
             "Player in Check: " + status.check + " | " + 
             "Evaluation: " + status.eval;

    ::DrawTextEx(*theme.font, final.c_str(),
                 Vector2{ (float)area.x + 10, (float)area.y + 2 },
                 16, 1, ::Color({255, 255, 255, 255}));
}