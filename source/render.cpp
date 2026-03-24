
#include "render.hpp"

using namespace Render;

// NOTE(Tejas): since I only have one type of texuture that will last for the
//              life of the application, Im not going to worry about UnLoading
//              the texture for now.
struct _ChessTextures {
    ::Texture2D lPawn, lKnight, lBishop, lRook, lQueen, lKing;
    ::Texture2D dPawn, dKnight, dBishop, dRook, dQueen, dKing;
};

global _ChessTextures G_tex;
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

internal void renderSquareBackgroud(const Window::Section &area, Chess::Square sq) {

    if (!sq.isValid()) return;

    int px = area.x + sq.file() * Window::SQUARE_DIM;
    int py = area.y + sq.rank() * Window::SQUARE_DIM;

    ::Color square_color = ((sq.rank() + sq.file()) % 2 == 0) ? ::RAYWHITE : ::DARKBROWN; 
    ::DrawRectangle(px, py, Window::SQUARE_DIM, Window::SQUARE_DIM, square_color);
}

internal void renderSquareHighlight(const Window::Section &area, Chess::Square sq, bool is_flipped) {

    if (!sq.isValid()) return;

    int rank = sq.rank();
    int file = sq.file();

    if (is_flipped) {
        rank = (Chess::MAX_RANK - 1) - rank;
        file = (Chess::MAX_FILE - 1) - file;
    }

    int px = area.x + file * Window::SQUARE_DIM;
    int py = area.y + rank * Window::SQUARE_DIM;
    
    ::DrawRectangle(px, py, Window::SQUARE_DIM, Window::SQUARE_DIM, ::Color(100, 100, 255, 255));
}

internal void renderSquareCoord(const Window::Section &area, Chess::Square sq, bool is_flipped) {

    if (!sq.isValid()) return;

    int px = area.x + sq.file() * Window::SQUARE_DIM;
    int py = area.y + sq.rank() * Window::SQUARE_DIM;
    
    ::Color text_color =  ((sq.rank() + sq.file()) % 2 == 0) ? ::DARKBROWN : ::RAYWHITE;
    int xx = px + 5;
    int yy = py + 5;
    if (sq.file() == 0) {
        std::string ch;
        if (!is_flipped) ch = std::to_string(sq.rank() + 1);
        else ch = std::to_string(Chess::MAX_RANK - sq.rank());
        ::DrawText(ch.c_str(), xx, yy, 15, text_color);
    }
    if (sq.rank() == 0) {
        xx = px + Window::SQUARE_DIM - 15;
        char ch[2] = { '\0' };
        if (is_flipped) ch[0] = (char)(sq.file() + 'a');
        else ch[0] = (char)('h' - sq.file());
        ::DrawText(ch, xx, yy, 15, text_color);
    }
}

internal void renderPieceOnSquare(const Window::Section &area, const Chess::Square &sq,
                                  const Chess::Piece &pc, bool is_flipped) {

    if (!sq.isValid()) return;
    if (pc.isEmpty())  return;

    int file = sq.file();
    int rank = sq.rank();

    if (is_flipped) {
        file = (Chess::MAX_FILE - 1) - file;
        rank = (Chess::MAX_RANK - 1) - rank;
    }

    int px = area.x + file * Window::SQUARE_DIM;
    int py = area.y + rank * Window::SQUARE_DIM;

    const ::Texture2D* tex = nullptr;

    if (pc.color() == Chess::Piece::LIGHT) {
        switch (pc.type()) {
        case Chess::Piece::PAWN:   tex = &G_tex.lPawn;   break;
        case Chess::Piece::KNIGHT: tex = &G_tex.lKnight; break;
        case Chess::Piece::BISHOP: tex = &G_tex.lBishop; break;
        case Chess::Piece::ROOK:   tex = &G_tex.lRook;   break;
        case Chess::Piece::QUEEN:  tex = &G_tex.lQueen;  break;
        case Chess::Piece::KING:   tex = &G_tex.lKing;   break;
        default: break;
        }
    } else if (pc.color() == Chess::Piece::DARK) {
        switch (pc.type()) {
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
        ::DrawTextureEx(*tex, Vector2{(float)px, (float)py}, 0.0f, scale, WHITE);
    }
}

void Render::renderBoard(const Window::Section &area, const Chess::Board& board, const Visual &visual) {

    if (!G_tex_init) _initTexture();

    // FIXME(Tejas): Combine these 2 loops...

    for (int rank = 0; rank < Chess::MAX_RANK; rank++) {

        for (int file = 0; file < Chess::MAX_FILE; file++) {

            int x = area.x + file * Window::SQUARE_DIM;
            int y = area.y + rank * Window::SQUARE_DIM;

            Chess::Square sq(rank, file);

            renderSquareBackgroud(area, sq);
            renderSquareCoord(area, sq, visual.board_flipped);
        }
    }   

    // NOTE(Tejas): Because for flip board we are may render piece translated to its flipped position,
    for (int rank = 0; rank < Chess::MAX_RANK; rank++) {

        for (int file = 0; file < Chess::MAX_FILE; file++) {

            int x = area.x + file * Window::SQUARE_DIM;
            int y = area.y + rank * Window::SQUARE_DIM;

            Chess::Square sq(rank, file);

            if (sq == visual.sel_square) renderSquareHighlight(area, sq, visual.board_flipped);

            Chess::Piece pc  = board.getPieceAt(sq);
            renderPieceOnSquare(area, sq, pc, visual.board_flipped);
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
