//=============================================================================
//
//	Locus of Memory [game.cpp]
//	Author : HAYATO NAMBA
// 
//=============================================================================
#include"game.h"
#include"player.h"
#include"input.h"
#include "debugproc.h"
#include "gameui.h"
#include "main.h"
//#include"sound.h"
#include"fade.h"
//#include"pause.h"
#include"camera.h"
#include"light.h"
//#include"polygon.h"
//#include"model.h"
#include"shadow.h"
//#include"billboard.h"
//#include"wall.h"
//#include"field.h"
//#include"mashwall.h"
//#include"meshsky.h"
//#include"block.h"
//#include"fence.h"
//#include"coin.h"
//#include"half_fence.h"
//#include"score.h"
//#include"timer.h"
#include"particle.h"

#include"meshfield.h"

GAMESTATE g_gameState = GAMESTATE_NONE;		// ゲームの状態
int g_nCounterGameState = 0;				// 状態管理カウンター

//=======================================================
// ゲームの初期化処理
//=======================================================
void InitGame(void)
{
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

	InitGameUI();

	//InitModel();

	//InitBlock();

	//InitScore();

	//InitTimer();

	InitParticle();
	
	//PlaySound(SOUND_LABEL_GAME);

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

	UninitGameUI();

	//UninitScore();

	//UninitTimer();

	UninitParticle();

	//StopSound();

}
//=======================================================
// ゲートの更新処理
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

	UpdateParticle();

	UpdateGameUI();
	

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

	// プレイヤーの描画処理
	DrawPlayer();

	//DrawModel();

	//DrawBlock();

	//DrawScore();

	//DrawTimer();

	DrawGameUI();

	DrawParticle();

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