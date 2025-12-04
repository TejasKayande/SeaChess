
#include "render.hpp"

using namespace Render;

// NOTE(Tejas): since I only have one type of texuture that will last for the
//              life of the application, Im not going to worry about UnLoading
//              the texture for now.
struct _Texture {
    Texture2D lPawn, lKnight, lBishop, lRook, lQueen, lKing;
    Texture2D dPawn, dKnight, dBishop, dRook, dQueen, dKing;
};

global _Texture G_tex;
global bool    G_tex_init = false;

internal void _initTexture() {
    
    G_tex.lPawn   = LoadTexture("../assets/texture/lPawn.png");
    G_tex.lKnight = LoadTexture("../assets/texture/lKnight.png");
    G_tex.lBishop = LoadTexture("../assets/texture/lBishop.png");
    G_tex.lRook   = LoadTexture("../assets/texture/lRook.png");
    G_tex.lQueen  = LoadTexture("../assets/texture/lQueen.png");
    G_tex.lKing   = LoadTexture("../assets/texture/lKing.png");

    G_tex.dPawn   = LoadTexture("../assets/texture/dPawn.png");
    G_tex.dKnight = LoadTexture("../assets/texture/dKnight.png");
    G_tex.dBishop = LoadTexture("../assets/texture/dBishop.png");
    G_tex.dRook   = LoadTexture("../assets/texture/dRook.png");
    G_tex.dQueen  = LoadTexture("../assets/texture/dQueen.png");
    G_tex.dKing   = LoadTexture("../assets/texture/dKing.png");

    G_tex_init = true;
}

void Render::renderBoard(const Window::Section &area, const Chess::Board& board) {

    if (!G_tex_init) _initTexture();

    DrawRectangle(area.x, area.y, area.width, area.height, BROWN);

    for (int rank = 0; rank < Chess::MAX_RANK; rank++) {

        for (int file = 0; file < Chess::MAX_FILE; file++) {

            Color squareColor = ((rank + file) % 2 == 0) ? RAYWHITE : DARKBROWN;

            int x = area.x + file * Window::TEXTURE_SIZE;
            int y = area.y + (7 - rank) * Window::TEXTURE_SIZE;

            DrawRectangle(x, y, Window::TEXTURE_SIZE, Window::TEXTURE_SIZE, squareColor);

            Chess::Square sq(rank, file);
            Chess::Piece p = board.getPieceAt(sq);
            if (p.isEmpty()) continue;

            const Texture2D* tex = nullptr;

            if (p.color() == Chess::Piece::LIGHT) {
                switch (p.type()) {
                    case Chess::Piece::PAWN:   tex = &G_tex.lPawn;   break;
                    case Chess::Piece::KNIGHT: tex = &G_tex.lKnight; break;
                    case Chess::Piece::BISHOP: tex = &G_tex.lBishop; break;
                    case Chess::Piece::ROOK:   tex = &G_tex.lRook;   break;
                    case Chess::Piece::QUEEN:  tex = &G_tex.lQueen;  break;
                    case Chess::Piece::KING:   tex = &G_tex.lKing;   break;
                    default: break;
                }
            } else if (p.color() == Chess::Piece::DARK) {
                switch (p.type()) {
                    case Chess::Piece::PAWN:   tex = &G_tex.dPawn;   break;
                    case Chess::Piece::KNIGHT: tex = &G_tex.dKnight; break;
                    case Chess::Piece::BISHOP: tex = &G_tex.dBishop; break;
                    case Chess::Piece::ROOK:   tex = &G_tex.dRook;   break;
                    case Chess::Piece::QUEEN:  tex = &G_tex.dQueen;  break;
                    case Chess::Piece::KING:   tex = &G_tex.dKing;   break;
                    default: break;
                }
            }

            if (tex) {
                float scale = static_cast<float>(Window::TEXTURE_SIZE) / tex->width;
                DrawTextureEx(*tex, Vector2{(float)x, (float)y}, 0.0f, scale, WHITE);
            }
        }
    }   
}

void Render::renderMenu(const Window::Section &area) {

    Color menu_background = Color(0x44, 0x44, 0x44, 0x99);
    DrawRectangleRec(area, menu_background);
}

void Render::renderInfo(const Window::Section &area) {

    DrawRectangleRec(area, YELLOW);
}

void Render::renderStatus(const Window::Section &area) {

    DrawRectangleRec(area, GREEN);
}
