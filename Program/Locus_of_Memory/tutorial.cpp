//=============================================================================
//
//	Locus of Memory [tutorial.cpp]
//	Author : HAYATO NAMBA
// 
//=============================================================================
#include"tutorial.h"
#include "color.h"
#include"player.h"
#include"input.h"
#include "object.h"
#include "skybox.h"
#include "effect.h"
#include "particle.h"
#include "brickwall.h"
//#include"sound.h"
#include"fade.h"
//#include"pause.h"
#include"camera.h"
#include"light.h"
//#include"polygon.h"
//#include"model.h"
#include"player.h"
#include"shadow.h"
#include "magic.h"
//#include"billboard.h"
//#include"wall.h"
//#include"field.h"
#include"meshfield.h"
#include "uimanager.h"
#include "custommesh.h"
#include "vibration.h"
//#include"mashwall.h"
//#include"meshsky.h"
//#include"block.h"
//#include"score.h"
//#include"timer.h"

TUTORIALSTATE g_TutorialState = TUTORIALSTATE_NONE;		// ゲームの状態
int g_nCounterTutorialState = 0;				// 状態管理カウンター
int g_nCntFade = 0;

//=======================================================
// ゲームの初期化処理
//=======================================================
void InitTutorial(void)
{
	// プレイヤーの初期化設定
	//InitBG();

	//InitPause();

	InitLight();

	// 魔法の初期化処理
	InitMagic();

	//InitField();

	//InitMeshsky();

	//InitMeshfield();



	//InitMashwall();

	//InitBullet();

	//InitBillboard();

	InitShadow();

	InitPlayer();

	//InitModel();

	//InitBlock();

	//InitWall();

	//InitScore();
	//
	//PlaySound(SOUND_LABEL_GAME);

	// UIの状態を初期化
	SetPauseFalse();
}
//=======================================================
// ゲームの終了処理
//=======================================================
void UninitTutorial(void)
{
	// プレイヤーの終了処理
	//UninitPause();

	UninitLight();

	// 魔法の終了処理
	UninitMagic();

	//UninitField();

	//UninitMeshsky();

	//UninitMeshfield();

	//UninitMashwall();

	//UninitBullet();

	UninitShadow();

	//UninitBillboard();

	UninitPlayer();

	//UninitModel();

	//UninitBlock();

	//UninitWall();

	//UninitScore();

	//StopSound();

}
//=======================================================
// ゲートの更新処理
//=======================================================
void UpdateTutorial(void)
{
	Player* pPlayer = GetPlayer();
	FADE* pFade = GetFade();
	//Timer* pTimer = GetTimer();

	// プレイヤーの更新処理
	//UpdateBG();

	UpdateLight();

	// 魔法の更新処理
	UpdateMagic();

	//UpdateField();

	//UpdateMeshsky();

	UpdateMeshField();

	UpdateCustomMesh();

	//UpdateMashwall();

	//UpdateBullet();

	UpdateShadow();

	//UpdateBillboard();

	UpdatePlayer();

	//UpdateMotion();

	//UpdateModel();

	//UpdateBlock();

	//UpdateWall();

	//UpdateScore();

	switch (g_TutorialState)
	{
	case TUTORIALSTATE_NORMAL:		// 通常状態

		break;

	case TUTORIALSTATE_END:			// 終了状態

		g_nCounterTutorialState--;		// 状態管理カウンター減少

		if (g_nCounterTutorialState <= 0 || GetKeyboardTrigger(DIK_RETURN) == true && *pFade == FADE_NONE)
		{// 0以下になった

			g_TutorialState = TUTORIALSTATE_NONE;

			// フェード設定(ゲーム画面に移行)
			SetFade(MODE_GAME, COLOR_WHITE);

			// サウンド停止
			//StopSound(SOUND_LABEL_BGM000);
		}
	}

	

	if (GetKeyboardTrigger(DIK_O) == true && *pFade == FADE_NONE)
	{// 0以下になった
		g_TutorialState = TUTORIALSTATE_NONE;

		// フェード設定(ゲーム画面に移行)
		SetFade(MODE_GAME, COLOR_WHITE);

		for (int nCntVibration = 0; nCntVibration < MAX_PLAYER; nCntVibration++)
		{
			VibrationType(VIBRATIONTYPE_NOTHING, VIBRATION_CLEAR, nCntVibration);
		}

		// サウンド停止
		//StopSound(SOUND_LABEL_BGM000);
	}


}
//=======================================================
// ゲームの描画処理
//=======================================================
void DrawTutorial(void)
{
	// プレイヤーの描画処理
	//DrawBG();

	//DrawField();

	//DrawMeshsky();

	DrawMeshField();

	DrawCustomMesh();

	// 魔法の描画処理
	DrawMagic();

	//DrawMashwall();

	//DrawBullet();

	DrawShadow();

	//DrawBillboard();

	// オブジェクトの描画処理（仮）
	DrawObject();

	// エフェクトの描画処理
	DrawEffect();

	// パーティクルの描画処理
	DrawParticle();

	DrawPlayer();

	// スカイボックスの描画処理
	DrawSkyBox();

	// 塀の描画処理
	DrawBrickWall();

	//DrawModel();

	//DrawBlock();

	//DrawWall();

	//DrawScore();
}

void SetTutorialState(TUTORIALSTATE state, int nCounter)
{
	g_TutorialState = state;				// ゲーム状態設定
	g_nCounterTutorialState = nCounter;		// 状態管理カウンター設定
}
TUTORIALSTATE GetTutorialState(void)
{
	return g_TutorialState;
}