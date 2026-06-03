
#include "render.hpp"

using namespace Render;

// constexpr Theme theme = Themes::DEFAULT;
// constexpr Theme theme = Themes::CLASSIC_WOOD;
constexpr Theme theme = Themes::SLATE_BLUE;
// constexpr Theme theme = Themes::EMERALD;


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

    void renderSquareBackgroud(const Window::Section &area, Chess::Square sq) {

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

    void renderSquareCoord(const Window::Section &area, Chess::Square sq, bool is_flipped) {

        if (!sq.isValid()) return;

        int px = area.x + sq.file() * Window::SQUARE_DIM;
        int py = area.y + sq.rank() * Window::SQUARE_DIM;

        ::Color text_color =  ((sq.rank() + sq.file()) % 2 == 0) ? theme.board_light : theme.board_dark;
        int xx = px + 5;
        int yy = py + 5;
        if (sq.file() == 0) {
            std::string ch;
            if (!is_flipped) ch = std::to_string(sq.rank() + 1);
            else ch = std::to_string(Chess::MAX_RANK - sq.rank());
            ::DrawTextEx(G_assets.inter_regular_24, ch.c_str(), Vector2{(float)xx, (float)yy}, 24, 2, text_color);
        }
        if (sq.rank() == 7) {
            xx = px + Window::SQUARE_DIM - 15;
            yy = py + Window::SQUARE_DIM - 25;
            char ch[2] = { '\0' };
            if (is_flipped) ch[0] = (char)(sq.file() + 'a');
            else ch[0] = (char)('h' - sq.file());
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
    
    G_assets.lPawn   = ::LoadTexture("../assets/texture/lPawn.png");
    G_assets.lKnight = ::LoadTexture("../assets/texture/lKnight.png");
    G_assets.lBishop = ::LoadTexture("../assets/texture/lBishop.png");
    G_assets.lRook   = ::LoadTexture("../assets/texture/lRook.png");
    G_assets.lQueen  = ::LoadTexture("../assets/texture/lQueen.png");
    G_assets.lKing   = ::LoadTexture("../assets/texture/lKing.png");

    G_assets.dPawn   = ::LoadTexture("../assets/texture/dPawn.png");
    G_assets.dKnight = ::LoadTexture("../assets/texture/dKnight.png");
    G_assets.dBishop = ::LoadTexture("../assets/texture/dBishop.png");
    G_assets.dRook   = ::LoadTexture("../assets/texture/dRook.png");
    G_assets.dQueen  = ::LoadTexture("../assets/texture/dQueen.png");
    G_assets.dKing   = ::LoadTexture("../assets/texture/dKing.png");

    G_assets.inter_regular_24 = LoadFontEx("../assets/font/Inter-Regular.ttf", 24, 0, 0);
    G_assets.inter_regular_50 = LoadFontEx("../assets/font/Inter-Regular.ttf", 50, 0, 0);
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

void Render::renderBoard(const Window::Section &area, const Visual *visual) {

    Chess::Piece piece_on_mouse = Chess::Piece::nopiece();

    for (int i = 0; i < Chess::MAX_RANK * Chess::MAX_FILE; i++) {

        renderSquareBackgroud(area, Chess::Square(i));

        Chess::Square sq(i);

        if (visual->board[i].flag & HighlightType::SELECTED) {
            renderSquareHighlight(area, sq, visual->theme.highlight);
            piece_on_mouse = visual->board[i].piece;
        } else if (visual->board[i].flag & HighlightType::LEGAL)  {
            renderSquareHighlight(area, sq, visual->theme.legal);
        } else if (visual->board[i].flag & HighlightType::CHECK) {
            renderSquareHighlight(area, sq, visual->theme.check);
        }

        if (!(visual->board[i].flag & HighlightType::SELECTED)) renderPieceOnSquare(area, sq, visual->board[i].piece);
        else renderPieceAtMouse(area, visual->board[i].piece);
    }

    if (!piece_on_mouse.isEmpty()) {
        renderPieceAtMouse(area, piece_on_mouse);
    }
}

void Render::renderMenu(const Window::Section &area) {

    Window::Section menu_section = Window::getMenuSection();
    ::DrawTextEx(G_assets.inter_regular_50, "Menu",
                 Vector2{ (float)menu_section.width / 2 - 50, (float)50 },
                 50, 2, ::Color({0, 255, 255, 255}));
    ::DrawRectangleRec(area, theme.menu_bg);
}

void Render::renderInfo(const Window::Section &area) {

    ::DrawRectangleRec(area, ::Color{100, 100, 100, 255});
}

void Render::renderStatus(const Window::Section &area) {

    ::DrawRectangleRec(area, ::Color{18, 18, 18, 255});
}
