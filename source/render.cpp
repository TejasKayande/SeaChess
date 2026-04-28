
#include "render.hpp"
#include "movegen.hpp"

using namespace Render;

// TODO(Tejas):
// - [ ] Refactor renderer to use Base::Color on the API.

struct _Assets {
    ::Texture2D lPawn, lKnight, lBishop, lRook, lQueen, lKing;
    ::Texture2D dPawn, dKnight, dBishop, dRook, dQueen, dKing;
    ::Font inter_regular_24;
    ::Font inter_regular_50;
};

global _Assets G_assets;

internal void renderSquareBackgroud(const Window::Section &area, Chess::Square sq) {

    if (!sq.isValid()) return;

    int px = area.x + sq.file() * Window::SQUARE_DIM;
    int py = area.y + sq.rank() * Window::SQUARE_DIM;

    ::Color square_color = ((sq.rank() + sq.file()) % 2 == 0) ? ::RAYWHITE : ::DARKBROWN; 
    ::DrawRectangle(px, py, Window::SQUARE_DIM, Window::SQUARE_DIM, square_color);
}

internal void renderSquareHighlight(const Window::Section &area, Chess::Square sq, 
                                    bool is_flipped, ::Color color = ::Color(100, 100, 255, 255)) {

    if (!sq.isValid()) return;

    int rank = sq.rank();
    int file = sq.file();

    if (is_flipped) {
        rank = (Chess::MAX_RANK - 1) - rank;
        file = (Chess::MAX_FILE - 1) - file;
    }

    int px = area.x + file * Window::SQUARE_DIM;
    int py = area.y + rank * Window::SQUARE_DIM;
    
    ::DrawRectangle(px, py, Window::SQUARE_DIM, Window::SQUARE_DIM, color);
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

internal void renderLegal(const Window::Section &area, const Chess::Board *board, const Visual *visual) {

    // temp...
    BitBoard legal_moves = 0;

    if (visual->selected_square.isValid()) {

        Chess::Piece pc = board->getPieceAt(visual->selected_square);

        if (!pc.isEmpty()) {

            switch (pc.type()) {

            case Chess::Piece::PAWN: {
                legal_moves = MoveGen::Attack::pawnAttacks(visual->selected_square, pc.color());                
            } break;

            case Chess::Piece::KNIGHT: {
                legal_moves = MoveGen::Attack::knightAttacks(visual->selected_square);                
            } break;

            case Chess::Piece::KING: {
                legal_moves = MoveGen::Attack::kingAttacks(visual->selected_square);                
            } break;

            case Chess::Piece::BISHOP: {
                legal_moves = MoveGen::Attack::bishopAttacks(visual->selected_square, board->getOccupied());                
            } break;


            case Chess::Piece::ROOK: {
                legal_moves = MoveGen::Attack::rookAttacks(visual->selected_square, board->getOccupied());                
            } break;

            case Chess::Piece::QUEEN: {
                legal_moves = MoveGen::Attack::queenAttacks(visual->selected_square, board->getOccupied());                
            } break;

            }
        }
    }

    for (int sq_idx = 0; sq_idx < 64; sq_idx++) {

        if ((legal_moves & (1ULL << sq_idx)) != 0) {
            Chess::Square to = GET_SQUARE_FROM_INDEX(sq_idx);
            renderSquareHighlight(area, to, visual->is_board_flipped, ::Color(255, 100, 255, 200));
        }
    }
}

void Render::initAssets() {
    
    // TODO(Tejas): Free These!
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

void Render::renderBoard(const Window::Section &area, const Chess::Board *board, const Visual *visual) {

    // FIXME(Tejas): Combine these 2 loops...

    for (int rank = 0; rank < Chess::MAX_RANK; rank++) {

        for (int file = 0; file < Chess::MAX_FILE; file++) {

            int x = area.x + file * Window::SQUARE_DIM;
            int y = area.y + rank * Window::SQUARE_DIM;

            Chess::Square sq(rank, file);

            renderSquareBackgroud(area, sq);
            renderSquareCoord(area, sq, visual->is_board_flipped);
        }
    }   

    // NOTE(Tejas): Because for flip board we are may render piece translated to its flipped position,
    for (int rank = 0; rank < Chess::MAX_RANK; rank++) {

        for (int file = 0; file < Chess::MAX_FILE; file++) {

            int x = area.x + file * Window::SQUARE_DIM;
            int y = area.y + rank * Window::SQUARE_DIM;

            Chess::Square sq(rank, file);

            if (sq == visual->selected_square) renderSquareHighlight(area, sq, visual->is_board_flipped);

            Chess::Piece pc  = board->getPieceAt(sq);
            renderPieceOnSquare(area, sq, pc, visual->is_board_flipped);
        }
    }

    // TODO(Tejas): Remove this...
    renderLegal(area, board, visual);
}

void Render::renderMenu(const Window::Section &area) {

    ::Color menu_background = ::Color(0x44, 0x44, 0x44, 0x99);
    Window::Section menu_section = Window::getMenuSection();
    ::DrawTextEx(G_assets.inter_regular_50, "Menu",
                 Vector2{ (float)menu_section.width / 2 - 50, (float)50 },
                 50, 2, ::Color(0, 255, 255, 255));
    ::DrawRectangleRec(area, menu_background);
}

void Render::renderInfo(const Window::Section &area) {

    ::DrawRectangleRec(area, ::Color(100, 100, 100, 255));
}

void Render::renderStatus(const Window::Section &area) {

    ::DrawRectangleRec(area, ::Color(18, 18, 18, 255));
}
