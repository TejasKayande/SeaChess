
#include "render.hpp"

#include "../assets/texture/lPawn.h"
#include "../assets/texture/lKnight.h"
#include "../assets/texture/lBishop.h"
#include "../assets/texture/lRook.h"
#include "../assets/texture/lQueen.h"
#include "../assets/texture/lKing.h"
#include "../assets/texture/dPawn.h"
#include "../assets/texture/dKnight.h"
#include "../assets/texture/dBishop.h"
#include "../assets/texture/dRook.h"
#include "../assets/texture/dQueen.h"
#include "../assets/texture/dKing.h"

#include "../assets/font/Inter-Regular.h"
#include "../assets/font/Roboto-Regular.h"

using namespace Render;

// TODO(Tejas): Load these directly into the exe
struct _Assets {
    ::Texture2D lPawn, lKnight, lBishop, lRook, lQueen, lKing;
    ::Texture2D dPawn, dKnight, dBishop, dRook, dQueen, dKing;
    ::Font inter_regular_24;
    ::Font inter_regular_50;
};

// TODO(Tejas): For now this is fine, but it would be better if we can have some
// sort of asset manager.
static _Assets G_assets;

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
            ::DrawTextEx(G_assets.inter_regular_24, ch.c_str(), Vector2{(float)xx, (float)yy}, 24, 2, text_color);
        }

        if (sq.rank() == 7) {
            xx = px + Window::SQUARE_DIM - 15;
            yy = py + Window::SQUARE_DIM - 25;
            char ch[2] = { '\0' };
            ch[0] = (char)('h' - file);
            ::DrawTextEx(G_assets.inter_regular_24, ch, Vector2{(float)xx, (float)yy}, 24, 2, text_color);
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
            case Chess::Piece::PAWN:   tex = &G_assets.lPawn;   break;
            case Chess::Piece::KNIGHT: tex = &G_assets.lKnight; break;
            case Chess::Piece::BISHOP: tex = &G_assets.lBishop; break;
            case Chess::Piece::ROOK:   tex = &G_assets.lRook;   break;
            case Chess::Piece::QUEEN:  tex = &G_assets.lQueen;  break;
            case Chess::Piece::KING:   tex = &G_assets.lKing;   break;
            default: break;
            }
        } else if (pc.color() == Chess::Piece::DARK) {
            switch (pc.type()) {
            case Chess::Piece::PAWN:   tex = &G_assets.dPawn;   break;
            case Chess::Piece::KNIGHT: tex = &G_assets.dKnight; break;
            case Chess::Piece::BISHOP: tex = &G_assets.dBishop; break;
            case Chess::Piece::ROOK:   tex = &G_assets.dRook;   break;
            case Chess::Piece::QUEEN:  tex = &G_assets.dQueen;  break;
            case Chess::Piece::KING:   tex = &G_assets.dKing;   break;
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
            case Chess::Piece::PAWN:   tex = &G_assets.lPawn;   break;
            case Chess::Piece::KNIGHT: tex = &G_assets.lKnight; break;
            case Chess::Piece::BISHOP: tex = &G_assets.lBishop; break;
            case Chess::Piece::ROOK:   tex = &G_assets.lRook;   break;
            case Chess::Piece::QUEEN:  tex = &G_assets.lQueen;  break;
            case Chess::Piece::KING:   tex = &G_assets.lKing;   break;
            default: break;
            }
        } else if (pc.color() == Chess::Piece::DARK) {
            switch (pc.type()) {
            case Chess::Piece::PAWN:   tex = &G_assets.dPawn;   break;
            case Chess::Piece::KNIGHT: tex = &G_assets.dKnight; break;
            case Chess::Piece::BISHOP: tex = &G_assets.dBishop; break;
            case Chess::Piece::ROOK:   tex = &G_assets.dRook;   break;
            case Chess::Piece::QUEEN:  tex = &G_assets.dQueen;  break;
            case Chess::Piece::KING:   tex = &G_assets.dKing;   break;
            default: break;
            }
        }

        if (tex) ::DrawTexture(*tex, mouse_x - Window::SQUARE_DIM / 2, mouse_y - Window::SQUARE_DIM / 2, WHITE);
    }
} // Anonymous namespace

void Render::initAssets() {

    ::Image lPawn_img    = ::LoadImageFromMemory(".png", lPawn_png  , lPawn_png_len);
    ::Image lKnight_img  = ::LoadImageFromMemory(".png", lKnight_png, lKnight_png_len);
    ::Image lBishop_img  = ::LoadImageFromMemory(".png", lBishop_png, lBishop_png_len);
    ::Image lRook_img    = ::LoadImageFromMemory(".png", lRook_png  , lRook_png_len);
    ::Image lQueen_img   = ::LoadImageFromMemory(".png", lQueen_png , lQueen_png_len);
    ::Image lKing_img    = ::LoadImageFromMemory(".png", lKing_png  , lKing_png_len);

    ::Image dPawn_img    = ::LoadImageFromMemory(".png", dPawn_png  , dPawn_png_len);
    ::Image dKnight_img  = ::LoadImageFromMemory(".png", dKnight_png, dKnight_png_len);
    ::Image dBishop_img  = ::LoadImageFromMemory(".png", dBishop_png, dBishop_png_len);
    ::Image dRook_img    = ::LoadImageFromMemory(".png", dRook_png  , dRook_png_len);
    ::Image dQueen_img   = ::LoadImageFromMemory(".png", dQueen_png , dQueen_png_len);
    ::Image dKing_img    = ::LoadImageFromMemory(".png", dKing_png  , dKing_png_len);
    
    G_assets.lPawn   = ::LoadTextureFromImage(lPawn_img);
    G_assets.lKnight = ::LoadTextureFromImage(lKnight_img);
    G_assets.lBishop = ::LoadTextureFromImage(lBishop_img);
    G_assets.lRook   = ::LoadTextureFromImage(lRook_img);
    G_assets.lQueen  = ::LoadTextureFromImage(lQueen_img);
    G_assets.lKing   = ::LoadTextureFromImage(lKing_img);

    G_assets.dPawn   = ::LoadTextureFromImage(dPawn_img);
    G_assets.dKnight = ::LoadTextureFromImage(dKnight_img);
    G_assets.dBishop = ::LoadTextureFromImage(dBishop_img);
    G_assets.dRook   = ::LoadTextureFromImage(dRook_img);
    G_assets.dQueen  = ::LoadTextureFromImage(dQueen_img);
    G_assets.dKing   = ::LoadTextureFromImage(dKing_img);

    G_assets.inter_regular_24 = ::LoadFontFromMemory(".ttf", __Inter_Regular_ttf, __Inter_Regular_ttf_len, 24, nullptr, 0);
    G_assets.inter_regular_50 = ::LoadFontFromMemory(".ttf", __Roboto_Regular_ttf, __Roboto_Regular_ttf_len, 50, nullptr, 0);

    ::UnloadImage(lPawn_img);
    ::UnloadImage(lKnight_img);
    ::UnloadImage(lBishop_img);
    ::UnloadImage(lRook_img);
    ::UnloadImage(lQueen_img);
    ::UnloadImage(lKing_img);

    ::UnloadImage(dPawn_img);
    ::UnloadImage(dKnight_img);
    ::UnloadImage(dBishop_img);
    ::UnloadImage(dRook_img);
    ::UnloadImage(dQueen_img);
    ::UnloadImage(dKing_img);
}

void Render::deinitAssets() {
    
    ::UnloadTexture(G_assets.lPawn);
    ::UnloadTexture(G_assets.lKnight);
    ::UnloadTexture(G_assets.lBishop);
    ::UnloadTexture(G_assets.lRook);
    ::UnloadTexture(G_assets.lQueen);
    ::UnloadTexture(G_assets.lKing);

    ::UnloadTexture(G_assets.dPawn);
    ::UnloadTexture(G_assets.dKnight);
    ::UnloadTexture(G_assets.dBishop);
    ::UnloadTexture(G_assets.dRook);
    ::UnloadTexture(G_assets.dQueen);
    ::UnloadTexture(G_assets.dKing);

    ::UnloadFont(G_assets.inter_regular_24);
    ::UnloadFont(G_assets.inter_regular_50);
}

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
    int menu_title_width = ::MeasureTextEx(G_assets.inter_regular_50, menu->getTitle().c_str(), 50, 2).x;

    ::DrawTextEx(G_assets.inter_regular_50, menu->getTitle().c_str(),
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

        int width = ::MeasureTextEx(G_assets.inter_regular_50, item.label.c_str(), 50, 2).x;

        ::DrawTextEx(G_assets.inter_regular_50, item.label.c_str(),
                     Vector2{ (float)(Window::WINDOW_WIDTH / 2) - (width / 2), (float)50 + (line_gap * i)},
                     50, 2, text_color);
        i++;
    }
}

void Render::renderInfo(const Window::Section &area) {

    ::DrawRectangleRec(area, ::Color{100, 100, 100, 255});
}

void Render::renderStatus(const Window::Section &area) {

    ::DrawRectangleRec(area, ::Color{18, 18, 18, 255});
}
