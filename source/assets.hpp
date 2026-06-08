#pragma once
#include <raylib.h>

namespace Assets {

    // TODO(Tejas): Maybe add different namespaces for different types of assets

    inline ::Texture2D LIGHT_PAWN;
    inline ::Texture2D LIGHT_KNIGHT;
    inline ::Texture2D LIGHT_BISHOP;
    inline ::Texture2D LIGHT_ROOK;
    inline ::Texture2D LIGHT_QUEEN;
    inline ::Texture2D LIGHT_KING;
    inline ::Texture2D DARK_PAWN;
    inline ::Texture2D DARK_KNIGHT;
    inline ::Texture2D DARK_BISHOP;
    inline ::Texture2D DARK_ROOK;
    inline ::Texture2D DARK_QUEEN;
    inline ::Texture2D DARK_KING;

    inline ::Font INTER_REGULAR_24;
    inline ::Font INTER_REGULAR_50;

    inline ::Sound MOVE_SOUND;
    inline ::Sound CAPTURE_SOUND;
    inline ::Sound CASTLE_SOUND;

    void init();
    void deinit();

} // namespace Assets