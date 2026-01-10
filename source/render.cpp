
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

// TODO(Tejas): Free These!
internal void _initTexture() {
    
    G_tex.lPawn   = ::LoadTexture("../assets/texture/lPawn.png");
    G_tex.lKnight = ::LoadTexture("../assets/texture/lKnight.png");
    G_tex.lBishop = ::LoadTexture("../assets/texture/lBishop.png");
    G_tex.lRook   = ::LoadTexture("../assets/texture/lRook.png");
    G_tex.lQueen  = ::LoadTexture("../assets/texture/lQueen.png");
    G_tex.lKing   = ::LoadTexture("../assets/texture/lKing.png");

    G_tex.dPawn   = ::LoadTexture("../assets/texture/dPawn.png");
    G_tex.dKnight = ::LoadTexture("../assets/texture/dKnight.png");
    G_tex.dBishop = ::LoadTexture("../assets/texture/dBishop.png");
    G_tex.dRook   = ::LoadTexture("../assets/texture/dRook.png");
    G_tex.dQueen  = ::LoadTexture("../assets/texture/dQueen.png");
    G_tex.dKing   = ::LoadTexture("../assets/texture/dKing.png");

    G_tex_init = true;
}

void Render::renderBoard(const Window::Section &area, const Chess::Board& board, const Visual &visual) {

    if (!G_tex_init) _initTexture();

    for (int rank = 0; rank < Chess::MAX_RANK; rank++) {

        for (int file = 0; file < Chess::MAX_FILE; file++) {

            int x = area.x + file * Window::SQUARE_DIM;
            int y = area.y + rank * Window::SQUARE_DIM;

            // TODO(Tejas): Modify for themes
            // TODO(Tejas): Add Flip Board

            { // NOTE(Tejas): Rendering the Square Background
                ::Color square_color = ((rank + file) % 2 == 0) ? RAYWHITE : DARKBROWN; 
                ::DrawRectangle(x, y, Window::SQUARE_DIM, Window::SQUARE_DIM, square_color);
            }

            Chess::Square sq(rank, file);

            { // NOTE(Tejas): Rendering the Hightlighted Square
                if (sq == visual.sel_square) {
                    std::cout << "h\n";
                    ::DrawRectangle(x, y, Window::SQUARE_DIM, Window::SQUARE_DIM, ::Color(100, 100, 255, 255));
                }
            }

            { // NOTE(Tejas): Rendering the Square Coordinate
                ::Color text_color =  ((rank + file) % 2 == 0) ? DARKBROWN : RAYWHITE;
                int xx = x + 5;
                int yy = y + 5;
                if (file == 0) {
                    std::string ch = std::to_string(rank + 1);
                    ::DrawText(ch.c_str(), xx, yy, 15, text_color);
                }
                if (rank == 7) {
                    yy = y + Window::SQUARE_DIM - 15;
                    char ch[2] = {(char)(file + 1 + 96), '\0'};
                    ::DrawText(ch, xx, yy, 15, text_color);
                }
            }

            Chess::Piece p = board.getPieceAt(sq);
            if (p.isEmpty()) continue;

            const ::Texture2D* tex = nullptr;

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
                ::DrawTextureEx(*tex, Vector2{(float)x, (float)y}, 0.0f, scale, WHITE);
            }
        }
    }   
}

void Render::renderMenu(const Window::Section &area) {

    Color menu_background = Color(0x44, 0x44, 0x44, 0x99);
    ::DrawRectangleRec(area, menu_background);
}

void Render::renderInfo(const Window::Section &area) {

    ::DrawRectangleRec(area, ::Color(18, 18, 18, 255));
}

void Render::renderStatus(const Window::Section &area) {

    ::DrawRectangleRec(area, ::Color(200, 200, 200, 255));
}
