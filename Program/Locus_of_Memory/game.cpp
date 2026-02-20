//=============================================================================
//
//	Locus of Memory [game.cpp]
//	Author : HAYATO NAMBA
// 
//=============================================================================
#include "main.h"
#include"game.h"
#include "color.h"
#include"input.h"
#include "debugproc.h"
#include"fade.h"
#include"sound.h"
#include "fog.h"
#include"effect.h"
#include"particle.h"
#include"camera.h"
#include"player.h"
#include "magic.h"
#include "uimanager.h"
#include "magicui.h"
#include "magicbubble.h"
#include "magiccircle.h"
#include "spellui.h"
#include "clock.h"
#include "battery.h"
#include "object.h"
#include "skybox.h"
#include "effect.h"
#include "particle.h"
#include"light.h"
#include"shadow.h"
#include "vibration.h"
#include"meshfield.h"
#include "brickwall.h"
#include "loadscript.h"
#include "grain.h"
#include "sparkle.h"
#include "goal.h"
#include "custommesh.h"

// マクロ定義
#define GAMEPOS_1P		(D3DXVECTOR3(-950.0f, 0.0f, -2800.0f))	// 1Pの位置[GAME]
#define GAMEPOS_2P		(D3DXVECTOR3(-950.0f, 0.0f, -2770.0f))	// 2Pの位置[GAME]
#define PLAYER_ROT		(D3DXVECTOR3(0.0f, -1.57f, 0.0f))	// プレイヤーの向き

GAMESTATE g_gameState = GAMESTATE_NONE;		// ゲームの状態
int g_nCounterGameState = 0;				// 状態管理カウンター

//=======================================================
// ゲームの初期化処理
//=======================================================
void InitGame(void)
{
	VIBRATION* pVibration = GetVibration();

	// ライトの初期化処理
	InitLight();

	// 影の初期化処理
	InitShadow();

	// プレイヤーの初期化処理
	InitPlayer();

	// 魔法の初期化処理
	InitMagic();

	// 魔法発動状態表示の初期化処理
	InitSpellUI();

	// 魔法陣の初期化処理
	InitMagicCircle();

	// 魔導書表示の初期化処理
	InitMagicUI();

	// 時計の初期化処理
	InitClock();

	// バッテリーの初期化処理
	InitBattery();

	// 3D空間UIの初期化処理
	InitUIManager();

	// 
	InitMagicBubble();

	// エフェクトの初期化処理
	InitEffect();

	// パーティクルの初期化処理
	InitParticle();

	// BGM再生
	PlaySound(SOUND_LABEL_000);

	pVibration->Vibration = false;

	// 魔法関連オブジェクトの読み込み
	LoadMagicObject(MAGICOBJECTINFO_SCRIPT);

	// ゲームの状態を通常状態に
	g_gameState = GAMESTATE_NORMAL;

	// プレイヤーの設置
	OPERATIONTYPE operationtyoe = GetOperationType();
	switch (operationtyoe)
	{
	case OPERATIONTYPE_2P:	// 2人操作
		SetPlayer(0, GAMEPOS_1P, PLAYER_ROT, PARENTMODELTYPE_PLAYER1P);
		SetPlayer(1, GAMEPOS_2P, PLAYER_ROT, PARENTMODELTYPE_PLAYER2P);
		break;

	default:	// 1人操作
		SetPlayer(0, GAMEPOS_1P, PLAYER_ROT, PARENTMODELTYPE_PLAYER1P);
		break;
	}

}
//=======================================================
// ゲームの終了処理
//=======================================================
void UninitGame(void)
{
	// ライトの終了処理
	UninitLight();

	// 影の終了処理
	UninitShadow();

	// プレイヤーの終了処理
	UninitPlayer();

	// 魔法の終了処理
	UninitMagic();

	// 魔法陣の終了処理
	UninitMagicCircle();

	// BGM終了
	StopSound();

	// ランダムオブジェクトの解放処理
	UninitRandomObject();
}
//=======================================================
// ゲームの更新処理
//=======================================================
void UpdateGame(void)
{
	Player* pPlayer = GetPlayer();
	FADE* pFade = GetFade();

	// ライトの更新処理
	UpdateLight();

	// メッシュフィールドの更新処理
	UpdateMeshField();

	// カスタムメッシュの更新処理
	UpdateCustomMesh();

	// 影の更新処理
	UpdateShadow();

	// プレイヤーの更新処理
	UpdatePlayer();

	// ゴールの更新処理
	UpdateGoal();

	// 魔法の更新処理
	UpdateMagic();

	// 魔法陣の更新処理
	UpdateMagicCircle();

	// 文字の更新処理
	UpdateGrain();

	// 粒の更新処理
	UpdateSparkle();

	// 魔法発動状態表示の更新処理
	UpdateSpellUI();

	// 魔導書表示の更新処理
	UpdateMagicUI();

	// 3D空間UIの更新処理
	UpdateUIManager();

	// 
	UpdateMagicBubble();

	// 時計の更新処理
	UpdateClock();

	// バッテリーの更新処理
	UpdateBattery();


	switch (g_gameState)
	{
	case GAMESTATE_NORMAL:		// 通常状態

		break;

	case GAMESTATE_CLEAR:			// 終了状態
		g_nCounterGameState--;		// 状態管理カウンター減少

		if (g_nCounterGameState <= 0 || GetKeyboardTrigger(DIK_RETURN) == true && *pFade == FADE_NONE)
		{// 0以下になった
			g_gameState = GAMESTATE_NONE;

			// フェード設定(リザルト画面に移行)
			SetFade(MODE_RESULT, COLOR_WHITE);
		}

		break;

	case GAMESTATE_TIMEOVER:
		g_nCounterGameState--;		// 状態管理カウンター減少

		if (g_nCounterGameState <= 0 && *pFade == FADE_NONE)
		{// 0以下になった
			g_gameState = GAMESTATE_NONE;

			// フェード設定(リザルト画面に移行)
			SetFade(MODE_RESULT, COLOR_BLACK);
		}
		break;

	case GAMESTATE_BATTERYOVER:
		g_nCounterGameState--;		// 状態管理カウンター減少

		if (g_nCounterGameState <= 0 && *pFade == FADE_NONE)
		{// 0以下になった
			g_gameState = GAMESTATE_NONE;

			// フェード設定(リザルト画面に移行)
			SetFade(MODE_RESULT, COLOR_BLACK);
		}
		break;
	}


	if (GetKeyboardTrigger(DIK_O) == true && *pFade == FADE_NONE)
	{// 0以下になった
		g_gameState = GAMESTATE_NONE;

		// フェード設定(リザルト画面に移行)
		SetFade(MODE_RESULT, COLOR_WHITE);

		for (int nCntVibration = 0; nCntVibration < MAX_PLAYER; nCntVibration++)
		{
			VibrationType(VIBRATIONTYPE_NOTHING, VIBRATION_CLEAR, nCntVibration);
		}
	}
}
//=======================================================
// ゲームの描画処理
//=======================================================
void DrawGame(void)
{
	// メッシュフィールドの描画処理
	DrawMeshField();

	// カスタムメッシュの描画処理
	DrawCustomMesh();

	// 影の描画処理
	DrawShadow();

	// スカイボックスの描画処理
	DrawSkyBox();

	// 塀の描画処理
	DrawBrickWall();

	// オブジェクトの描画処理（仮）
	DrawObject();

	// ゴールの描画処理
	DrawGoal();

	// エフェクトの描画処理
	DrawEffect();

	// パーティクルの描画処理
	DrawParticle();

	// 文字の描画処理
	DrawGrain();

	// 粒の描画処理
	DrawSparkle();

	SetFogEnable(false);		// 霧を無効

	// 3D空間UIの描画処理
	DrawUIManager();

	SetFogEnable(false);		// 霧を無効

	// プレイヤーの描画処理
	DrawPlayer();

	// 魔法の描画処理
	DrawMagic();

	// 魔法陣の描画処理
	DrawMagicCircle();

	SetFogEnable(false);		// 霧を無効

	DrawUIManager();

	// 
	DrawMagicBubble();

	// 魔法発動状態表示の描画処理
	DrawSpellUI();

	// 魔導書表示の描画処理
	DrawMagicUI();

	// 時計の描画処理
	DrawClock();

	// バッテリーの描画処理
	DrawBattery();

	SetFogEnable(true);			// 霧を有効

}

//=======================================================
// ゲームの状態設定処理
//=======================================================
void SetGameState(GAMESTATE state, int nCounter)
{
	g_gameState = state;				// ゲーム状態設定
	g_nCounterGameState = nCounter;		// 状態管理カウンター設定
}

//=======================================================
// ゲームの状態取得処理
//=======================================================
GAMESTATE GetGameState(void)
{
	return g_gameState;
}