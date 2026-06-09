#include "assets.hpp"

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
#include "../assets/font/JetBrainsMono-Regular.h"

#include "../assets/sound/move.h"
#include "../assets/sound/capture.h"
#include "../assets/sound/castle.h"

static bool g_initialized = false;

void Assets::init() {

    if (g_initialized) return;

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
    
    LIGHT_PAWN   = ::LoadTextureFromImage(lPawn_img);
    LIGHT_KNIGHT = ::LoadTextureFromImage(lKnight_img);
    LIGHT_BISHOP = ::LoadTextureFromImage(lBishop_img);
    LIGHT_ROOK   = ::LoadTextureFromImage(lRook_img);
    LIGHT_QUEEN  = ::LoadTextureFromImage(lQueen_img);
    LIGHT_KING   = ::LoadTextureFromImage(lKing_img);

    DARK_PAWN   = ::LoadTextureFromImage(dPawn_img);
    DARK_KNIGHT = ::LoadTextureFromImage(dKnight_img);
    DARK_BISHOP = ::LoadTextureFromImage(dBishop_img);
    DARK_ROOK   = ::LoadTextureFromImage(dRook_img);
    DARK_QUEEN  = ::LoadTextureFromImage(dQueen_img);
    DARK_KING   = ::LoadTextureFromImage(dKing_img);

    ROBERTO_REGULAR        = ::LoadFontFromMemory(".ttf", __Roboto_Regular_ttf, __Roboto_Regular_ttf_len, 128, nullptr, 0);
    INTER_REGULAR          = ::LoadFontFromMemory(".ttf", __Inter_Regular_ttf, __Inter_Regular_ttf_len, 128, nullptr, 0);
    JETBRAINS_MONO_REGULAR = ::LoadFontFromMemory(".ttf", __JetBrainsMono_Regular_ttf, __JetBrainsMono_Regular_ttf_len, 128, nullptr, 0);

    ::Wave move_wav    = ::LoadWaveFromMemory(".wav", __move_wav, __move_wav_len);
    ::Wave capture_wav = ::LoadWaveFromMemory(".wav", __capture_wav, __capture_wav_len);
    ::Wave castle_wav  = ::LoadWaveFromMemory(".wav", __castle_wav, __castle_wav_len);

    MOVE_SOUND    = ::LoadSoundFromWave(move_wav);
    CAPTURE_SOUND = ::LoadSoundFromWave(capture_wav);
    CASTLE_SOUND  = ::LoadSoundFromWave(castle_wav);

    ::UnloadWave(move_wav);
    ::UnloadWave(capture_wav);
    ::UnloadWave(castle_wav);

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

    g_initialized = true;
}

void Assets::deinit() {

    if (!g_initialized) return;

    ::UnloadSound(CASTLE_SOUND);
    ::UnloadSound(CAPTURE_SOUND);
    ::UnloadSound(MOVE_SOUND);

    ::UnloadTexture(LIGHT_PAWN);
    ::UnloadTexture(LIGHT_KNIGHT);
    ::UnloadTexture(LIGHT_BISHOP);
    ::UnloadTexture(LIGHT_ROOK);
    ::UnloadTexture(LIGHT_QUEEN);
    ::UnloadTexture(LIGHT_KING);

    ::UnloadTexture(DARK_PAWN);
    ::UnloadTexture(DARK_KNIGHT);
    ::UnloadTexture(DARK_BISHOP);
    ::UnloadTexture(DARK_ROOK);
    ::UnloadTexture(DARK_QUEEN);
    ::UnloadTexture(DARK_KING);

    ::UnloadFont(ROBERTO_REGULAR);
    ::UnloadFont(INTER_REGULAR);
    ::UnloadFont(JETBRAINS_MONO_REGULAR);
}