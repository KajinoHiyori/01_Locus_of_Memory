//=============================================================================
//
//	Locus of Memory [game.cpp]
//	Author : HAYATO NAMBA
// 
//=============================================================================
#include "main.h"
#include"game.h"
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

GAMESTATE g_gameState = GAMESTATE_NONE;		// ゲームの状態
int g_nCounterGameState = 0;				// 状態管理カウンター

//=======================================================
// ゲームの初期化処理
//=======================================================
void InitGame(void)
{
	VIBRATION* pVibration = GetVibration();

	// プレイヤーの初期化設定
	//InitBG();

	//InitPause();

	// ライトの初期化処理
	InitLight();

	//InitField();

	//InitMeshsky();

	//InitWall();

	//InitMashwall();

	//InitBullet();

	//InitBillboard();

	InitShadow();

	InitPlayer();

	// 魔法の初期化処理
	InitMagic();

	// ポーズの初期化処理
	//InitPause();

	InitSpellUI();

	// 魔法陣の初期化処理
	InitMagicCircle();

	InitMagicUI();

	InitClock();

	InitBattery();

	//InitModel();

	//InitBlock();

	//InitScore();

	//InitTimer();

	InitUIManager();

	InitMagicBubble();

	InitEffect();

	InitParticle();

	// 粒の初期化処理
	InitGrain();

	PlaySound(SOUND_LABEL_000);

	pVibration->Vibration = false;

	LoadMagicObject(MAGICOBJECTINFO_SCRIPT);

}
//=======================================================
// ゲームの終了処理
//=======================================================
void UninitGame(void)
{
	// プレイヤーの終了処理
	//UninitPause();

	// ライトの終了処理
	UninitLight();

	//UninitField();

	//UninitWall();

	//UninitMashwall();

	//UninitBullet();

	UninitShadow();

	//UninitBillboard();

	UninitPlayer();

	//UninitModel();

	//UninitBlock();

	// 魔法の終了処理
	UninitMagic();

	// ポーズの終了処理
	//UninitPause();

	UninitSpellUI();

	// 魔法陣の終了処理
	UninitMagicCircle();

	UninitMagicUI();

	UninitClock();

	UninitBattery();

	UninitUIManager();

	UninitMagicBubble();

	//UninitScore();

	//UninitTimer();

	// 粒の終了処理
	UninitGrain();

	StopSound();

	UninitRandomObject();

}
//=======================================================
// ゲームの更新処理
//=======================================================
void UpdateGame(void)
{
	Player* pPlayer = GetPlayer();
	FADE* pFade = GetFade();
	//Timer* pTimer = GetTimer();

	// プレイヤーの更新処理
	//UpdateBG();

	// ライトの更新処理
	UpdateLight();

	//UpdateField();

	//UpdateMeshsky();

	// メッシュフィールドの更新処理
	UpdateMeshField();

	//UpdateWall();

	//UpdateMashwall();

	//UpdateBullet();

	UpdateShadow();

	//UpdateBillboard();

	UpdatePlayer();

	//UpdateMotion();

	//UpdateModel();

	//UpdateBlock();

	//UpdateScore();

	//UpdateTimer();

	// 魔法の更新処理
	UpdateMagic();

	// 魔法陣の更新処理
	UpdateMagicCircle();

	// 粒の更新処理
	UpdateGrain();

	UpdateSpellUI();

	UpdateMagicUI();

	UpdateUIManager();

	UpdateMagicBubble();

	UpdateClock();

	UpdateBattery();

	switch (g_gameState)
	{
	case GAMESTATE_NORMAL:		// 通常状態

		break;

	case GAMESTATE_END:			// 終了状態

		g_nCounterGameState--;		// 状態管理カウンター減少

		if (g_nCounterGameState <= 0 || GetKeyboardTrigger(DIK_RETURN) == true && *pFade == FADE_NONE)
		{// 0以下になった

			g_gameState = GAMESTATE_NONE;

			// フェード設定(リザルト画面に移行)
			SetFade(MODE_RESULT);

			// サウンド停止
			//StopSound(SOUND_LABEL_BGM000);
		}
	}


	if (GetKeyboardTrigger(DIK_O) == true && *pFade == FADE_NONE)
	{// 0以下になった

		g_gameState = GAMESTATE_NONE;

		// フェード設定(リザルト画面に移行)
		SetFade(MODE_RESULT);

		for (int nCntVibration = 0; nCntVibration < MAX_PLAYER; nCntVibration++)
		{
			VibrationType(VIBRATIONTYPE_NOTHING, VIBRATION_CLEAR, nCntVibration);
		}

		// サウンド停止
		//StopSound(SOUND_LABEL_BGM000);
	}

	//else if (/*GetKeyboardTrigger(DIK_L) == true && *pFade == FADE_NONE ||*/ pTimer->Time <= 0 && *pFade == FADE_NONE)
	//{// 0以下になった

	//	g_gameState = GAMESTATE_NONE;

	//	// フェード設定(リザルト画面に移行)
	//	SetFade(MODE_LOSE);

	//	// サウンド停止
	//	StopSound(SOUND_LABEL_BGM000);
	//}

}
//=======================================================
// ゲームの描画処理
//=======================================================
void DrawGame(void)
{
	// プレイヤーの描画処理
	//DrawBG();

	//DrawField();

	//DrawMeshsky();

	// メッシュフィールドの描画処理
	DrawMeshField();

	//DrawWall();

	//DrawMashwall();

	//DrawBullet();

	// 影の描画処理
	DrawShadow();

	//DrawBillboard();

	// スカイボックスの描画処理
	DrawSkyBox();

	// 塀の描画処理
	DrawBrickWall();

	// オブジェクトの描画処理（仮）
	DrawObject();

	// エフェクトの描画処理
	DrawEffect();

	// パーティクルの描画処理
	DrawParticle();

	SetFogEnable(false);		// 霧を無効

	DrawUIManager();

	SetFogEnable(false);		// 霧を無効

	DrawPlayer();

	// 粒の描画処理
	DrawGrain();

	// 魔法の描画処理
	DrawMagic();

	// 魔法陣の描画処理
	DrawMagicCircle();

	//DrawModel();

	//DrawBlock();

	//DrawScore();

	//DrawTimer();

	SetFogEnable(false);		// 霧を無効

	DrawUIManager();

	DrawMagicBubble();

	DrawSpellUI();

	DrawMagicUI();

	DrawClock();

	DrawBattery();

	SetFogEnable(true);			// 霧を有効

}

//=============================================================================
// ポーズメニュー設定処理
//=============================================================================
void SetEnablePause(bool bPause)
{
	//g_bPause = bPause;
}

void SetGameState(GAMESTATE state, int nCounter)
{
	g_gameState = state;				// ゲーム状態設定
	g_nCounterGameState = nCounter;		// 状態管理カウンター設定
}
GAMESTATE GetGameState(void)
{
	return g_gameState;
}