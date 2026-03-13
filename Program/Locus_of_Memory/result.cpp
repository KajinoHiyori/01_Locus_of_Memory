//=============================================================================
//
//	Locus of Memory [result.cpp]
//	Author : HAYATO NAMBA
// 
//=============================================================================
#include"main.h"
#include"result.h"
#include "resultui.h"
#include "game.h"
#include"input.h"
#include"fade.h"
#include "object.h"
#include"vibration.h"
#include "object.h"
#include "skybox.h"
#include "effect.h"
#include "particle.h"
#include "brickwall.h"
#include "debugproc.h"
#include "color.h"
#include "animal.h"
#include"sound.h"
#include "field.h"

// グローバル変数
LPDIRECT3DTEXTURE9    g_pTextureResult = NULL;        // テクスチャへのポインタ
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffResult = NULL;    // 頂点バッファへのポインタ

//=======================================================
// リザルトの初期化処理
//=======================================================
void InitResult(void)
{
    GAMESTATE gameState = GetGameState();

    // リザルトUIの初期化処理
    InitResultUI();

    switch (gameState)
    {
    case GAMESTATE_CLEAR:
        PlaySound(SOUND_LABEL_GOALRESULTBGM);
        break;

    default:
        PlaySound(SOUND_LABEL_FAILDRESULTBGM);
        break;
    }

    // ドラゴンのタイプを決定
    SetDragonType(0, DRAGONTYPE_FLYING);
}
//=======================================================
// リザルトの終了処理
//=======================================================
void UninitResult(void)
{
    // リザルトUIの終了処理
    UninitResultUI();

    StopSound();

    UninitAnimal();
}
//=======================================================
// リザルトの更新処理
//=======================================================
void UpdateResult(void)
{
    PrintDebugProc("ここはリザルト画面\n");
    FADE* pFade = GetFade();

    // 動物の更新処理
    UpdateAnimal();

    // リザルトUIの更新処理
    UpdateResultUI();

    if (GetKeyboardTrigger(DIK_RETURN) == true && *pFade == FADE_NONE || GetJoypadTrigger(JOYKEY_A, 0) == true && *pFade == FADE_NONE)
    {
        //StopSound();

        SetFade(MODE_DIAGNOSIS, COLOR_WHITE);
    }
}
//=======================================================
// リザルトの描画処理
//=======================================================
void DrawResult(void)
{
    // エフェクトの描画処理
    DrawEffect();

    // パーティクルの描画処理
    DrawParticle();

    // 動物の描画処理
    DrawAnimal();

    // オブジェクトの描画処理（仮）
    DrawObject();

    // スカイボックスの描画処理
    DrawSkyBox();

    // 塀の描画処理
    DrawBrickWall();

    // 地面の描画処理
    DrawField();

    // リザルトUIの描画処理
    DrawResultUI();

}