//=============================================================================
//
//	Locus of Memory [tutorial.cpp]
//	Author : HAYATO NAMBA
// 
//=============================================================================
#include"tutorial.h"
#include"main.h"
#include "color.h"
#include "debugproc.h"
#include "title.h"
#include"player.h"
#include"input.h"
#include "object.h"
#include "eventobject.h"
#include "magiceffect.h"
#include "skybox.h"
#include "effect.h"
#include "particle.h"
#include "brickwall.h"
#include "loadscript.h"
#include"sound.h"
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
#include "custommesh.h"
#include "vibration.h"
//#include"mashwall.h"
//#include"meshsky.h"
//#include"block.h"
//#include"score.h"
//#include"timer.h"
#include "grain.h"
#include "sparkle.h"
#include "animal.h"
#include "field.h"
#include "riverwall.h"

// UI関連の初期化
#include "uimanager.h"
#include "magicui.h"
#include "magicbubble.h"
#include "magiccircle.h"
#include "spellui.h"
#include "clock.h"
#include "battery.h"
#include "tutorialui.h"
#include "questionmark.h"
#include "readyui.h"

// マクロ定義
#define TUTORIALPOS_1P	(D3DXVECTOR3(-2820.0f, 0.0f, -3790.0f))		// 1Pの位置[TUTORIAL]			
#define TUTORIALPOS_2P	(D3DXVECTOR3(-2850.0f, 0.0f, -3790.0f))		// 2Pの位置[TUTORIAL]
#define GATE_POS		(D3DXVECTOR3(-1000.0f, 120.0f, -3650.0f))	// ゲートの位置
#define GATE_SIZE		(120.0f)	// ゲートの大きさ

int g_nCounterTutorialState = 0;				// 状態管理カウンター
int g_nCntFade = 0;
bool g_abReady[MAX_PLAYER];	// プレイヤーのゴール状態を管理

//=======================================================
// ゲームの初期化処理
//=======================================================
void InitTutorial(void)
{
	// プレイヤーのゴール状態を管理
	g_abReady[0] = false;
	g_abReady[1] = false;

	// プレイヤーの初期化設定
	//InitBG();

	//InitPause();

	InitLight();

	// 魔法の初期化処理
	InitMagic();

	// イベントオブジェクトの初期化処理
	InitEventObject();

	// マジックエフェクトの初期化処理
	InitMagicEffect();

	//InitField();

	//InitMeshsky();

	//InitMeshfield();

	// 魔法発動状態表示の初期化処理
	InitSpellUI();

	// 魔法陣の初期化処理
	InitMagicCircle();

	// 魔導書表示の初期化処理
	InitMagicUI();

	// バッテリーの初期化処理
	InitBattery();

	// 3D空間UIの初期化処理
	InitUIManager();

	// 吹き出しの初期化処理
	InitMagicBubble();

	// ?の初期化処理
	InitQuestionMark();

	// チュートリアルUIの初期化処理
	InitTutorialUI();

	// 準備完了状態の初期化処理
	InitReadyUI();

	//InitMashwall();

	//InitBullet();

	//InitBillboard();

	// 影の初期化処理
	InitShadow();

	// プレイヤーの初期化処理
	InitPlayer();

	//InitModel();

	//InitBlock();

	//InitWall();

	//InitScore();
	//
	//PlaySound(SOUND_LABEL_GAME);

	// UIの状態を初期化
	SetPauseFalse();

	// 魔法関連オブジェクトの読み込み
	LoadMagicObject(MAGICOBJECTINFO_SCRIPT);

	// プレイヤーの設置
	OPERATIONTYPE operationtyoe = GetOperationType();
	switch (operationtyoe)
	{
	case OPERATIONTYPE_2P:	// 2人操作
		SetPlayer(0, TUTORIALPOS_1P, INIT_D3DXVEC3, PARENTMODELTYPE_PLAYER1P);
		SetPlayer(1, TUTORIALPOS_2P, INIT_D3DXVEC3, PARENTMODELTYPE_PLAYER2P);
		break;

	default:	// 1人操作
		SetPlayer(0, TUTORIALPOS_1P, INIT_D3DXVEC3, PARENTMODELTYPE_PLAYER1P);
		break;
	}

	// ドラゴンの状態を設定
	SetDragonType(0, DRAGONTYPE_FIRE);
}

//=======================================================
// ゲームの終了処理
//=======================================================
void UninitTutorial(void)
{
	// プレイヤーの終了処理
	//UninitPause();

	// ライトの終了処理
	UninitLight();

	// 魔法の終了処理
	UninitMagic();

	// イベントオブジェクトの終了処理
	UninitEventObject();

	// マジックエフェクトの終了処理
	UninitMagicEffect();

	//UninitField();

	//UninitMeshsky();

	//UninitMeshfield();

	//UninitMashwall();

	//UninitBullet();

	// 影の終了処理
	UninitShadow();

	//UninitBillboard();

	// プレイヤーの終了処理
	UninitPlayer();

	// チュートリアルUIの終了処理
	UninitTutorialUI();

	// ?の終了処理
	UninitQuestionMark();

	// 準備完了状態の終了処理
	UninitReadyUI();

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
	FADE* pFade = GetFade();

	// ライトの更新処理
	UpdateLight();
	// チュートリアルUIの更新処理
	UpdateTutorialUI();

	// メッシュフィールドの更新処理
	UpdateMeshField();

	// フィールドの更新処理
	UpdateField();

	// 川の壁の更新処理
	UpdateRiverWall();

	// カスタムメッシュの更新処理
	UpdateCustomMesh();

	// 影の更新処理
	UpdateShadow();

	// イベントオブジェクトの更新処理
	UpdateEventObject();

	// プレイヤーの更新処理
	UpdatePlayer();

	// 動物の更新処理
	UpdateAnimal();

	// ゴールの更新処理
	//UpdateGoal();

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

	// マジックエフェクトの更新処理
	UpdateMagicEffect();

	// 吹き出しの更新処理
	UpdateMagicBubble();

	// ?の更新処理
	UpdateQuestionMark();

	// 準備完了状態の更新処理
	UpdateReadyUI();

	// バッテリーの更新処理
	UpdateBattery();

	// ゲーム画面への遷移状態を管理
	GoGameGate();

	//UpdateMotion();

	//UpdateModel();

	//UpdateBlock();

	//UpdateWall();

	//UpdateScore();

	if (GetKeyboardTrigger(DIK_O) == true && *pFade == FADE_NONE)
	{// 0以下になった
		// フェード設定(ゲーム画面に移行)
		SetFade(MODE_GAME, COLOR_WHITE);

		for (int nCntVibration = 0; nCntVibration < MAX_PLAYER; nCntVibration++)
		{
			VibrationType(VIBRATIONTYPE_NOTHING, VIBRATION_CLEAR, nCntVibration);
		}

		// サウンド停止
		//StopSound(SOUND_LABEL_BGM000);
	}
	
	// 操作人数の取得
	OPERATIONTYPE operationType = GetOperationType();

	switch (operationType)
	{
	case OPERATIONTYPE_2P:	// 2人操作
		if (g_abReady[0] == true && g_abReady[1] == true && *pFade == FADE_NONE)
		{ // 1P2P共に準備が済んでいる && フェードをしていない場合
			// フェード設定(ゲーム画面に移行)
			SetFade(MODE_GAME, COLOR_WHITE);

			for (int nCntVibration = 0; nCntVibration < MAX_PLAYER; nCntVibration++)
			{
				VibrationType(VIBRATIONTYPE_NOTHING, VIBRATION_CLEAR, nCntVibration);
			}
		}
		break;

	default:	// 1人操作
		if (g_abReady[0] == true && *pFade == FADE_NONE)
		{ // 1P操作 && 1Pの準備が済んでいる && フェードをしていない場合
			// フェード設定(ゲーム画面に移行)
			SetFade(MODE_GAME, COLOR_WHITE);

			for (int nCntVibration = 0; nCntVibration < MAX_PLAYER; nCntVibration++)
			{
				VibrationType(VIBRATIONTYPE_NOTHING, VIBRATION_CLEAR, nCntVibration);
			}
		}
		break;
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

	// メッシュフィールドの描画処理
	DrawMeshField();

	// フィールドの描画処理
	DrawField();

	// 川の壁の描画処理
	DrawRiverWall();

	// カスタムメッシュの描画処理
	DrawCustomMesh();

	// イベントオブジェクトの描画処理
	DrawEventObject();

	// 魔法の描画処理
	DrawMagic();

	//DrawMashwall();

	//DrawBullet();

	// 影の描画処理
	DrawShadow();

	//DrawBillboard();

	// 動物の描画処理
	DrawAnimal();

	// オブジェクトの描画処理（仮）
	DrawObject();

	// プレイヤーの描画処理
	DrawPlayer();

	// スカイボックスの描画処理
	DrawSkyBox();

	// 塀の描画処理
	DrawBrickWall();

	// マジックエフェクトの描画処理
	DrawMagicEffect();

	// エフェクトの描画処理
	DrawEffect();

	// パーティクルの描画処理
	DrawParticle();

	// 文字の描画処理
	DrawGrain();

	// 粒の描画処理
	DrawSparkle();

	// ポーズ管理UIの描画処理
	DrawUIManager();

	// 魔導書の描画処理
	DrawMagicUI();

	// 吹き出しの描画処理
	DrawMagicBubble();

	// 魔法陣の描画処理
	DrawMagicCircle();

	// spellの描画処理
	DrawSpellUI();

	// 時計の描画処理
	DrawClock();

	// バッテリーの描画処理
	DrawBattery();

	// チュートリアルUIの描画処理
	DrawTutorialUI();

	// ?の描画処理
	DrawQuestionMark();

	// 準備完了状態の描画処理
	DrawReadyUI();

	//DrawModel();

	//DrawBlock();

	//DrawWall();

	//DrawScore();
}

//=======================================================
// ゲーム画面への遷移を管理
//=======================================================
void GoGameGate(void)
{
	Player* pPlayer = GetPlayer();
	OPERATIONTYPE operationType = GetOperationType();
	float afLength[MAX_PLAYER] = { 0.0f };

	for (int nCntPlayer = 0; nCntPlayer < MAX_PLAYER; nCntPlayer++, pPlayer++)
	{
		// ゲートとプレイヤーの距離を測定
		if (pPlayer->bUse == false)
		{
			ResetReadyUI(nCntPlayer);
			continue;
		}

		afLength[nCntPlayer] = sqrtf((pPlayer->pos.x - GATE_POS.x) * (pPlayer->pos.x - GATE_POS.x) + (pPlayer->pos.z - GATE_POS.z) * (pPlayer->pos.z - GATE_POS.z)) * 0.5f;
		if (afLength[nCntPlayer] < GATE_SIZE)	// 遷移完了状態に移行できる範囲にいる
		{
			if ((GetKeyboardTrigger(DIK_RETURN) == true && nCntPlayer == 0) || GetJoypadTrigger(JOYKEY_X, nCntPlayer) == true)
			{
				PlaySound(SOUND_LABEL_ENTER);
				g_abReady[nCntPlayer] = true;
			}
		}
		else
		{
			g_abReady[nCntPlayer] = false;
		}
	}

	// 準備完了状態のUIを管理
	switch (operationType)
	{
	case OPERATIONTYPE_1P:	// 1Pジョイパッド操作
		if (afLength[0] < GATE_SIZE)
		{ // 遷移完了状態に移行できる範囲にいる
			if (g_abReady[0] == true)
			{ // ゴールフラグが立っている
				SetReadyUI(0, READYUITYPE_READY);
			}
			else
			{ // ゴールフラグが立っていない
				SetReadyUI(0, READYUITYPE_TUTORIALX);
			}
		}
		else
		{ // 遷移完了状態に移行できる範囲にいない
			ResetReadyUI(0);
		}
		break;

	case OPERATIONTYPE_2P:	// 2人操作
		if (afLength[0] < GATE_SIZE && afLength[1] < GATE_SIZE)
		{ // 1P2P共に遷移完了状態に移行できる範囲にいる
			for (int nCntPlayer = 0; nCntPlayer < MAX_PLAYER; nCntPlayer++)
			{
				if (g_abReady[nCntPlayer] == true)
				{ // ゴールフラグが立っている
					SetReadyUI(nCntPlayer, READYUITYPE_READY);
				}
				else
				{ // ゴールフラグが立っていない
					SetReadyUI(nCntPlayer, READYUITYPE_TUTORIALX);
				}
			}
		}
		if (afLength[0] < GATE_SIZE && afLength[1] > GATE_SIZE)
		{ // 1Pが遷移完了状態に移行できる範囲にいて、2Pが遷移完了状態に移行できる範囲にいない
			SetReadyUI(0, READYUITYPE_WAITING);
			ResetReadyUI(1);
		}
		else if (afLength[0] > GATE_SIZE && afLength[1] < GATE_SIZE)
		{ // 1Pが遷移完了状態に移行できる範囲におらず、2Pが遷移完了状態に移行できる範囲にいる
			ResetReadyUI(0);
			SetReadyUI(1, READYUITYPE_WAITING);
		}
		break;

	case OPERATIONTYPE_KEYBOARD:	// キーボード操作
		if (afLength[0] < GATE_SIZE)
		{ // 遷移完了状態に移行できる範囲にいる
			if (g_abReady[0] == true)
			{ // ゴールフラグが立っている
				SetReadyUI(0, READYUITYPE_READY);
			}
			else
			{ // ゴールフラグが立っていない
				SetReadyUI(0, READYUITYPE_TUTORIALENTER);
			}
		}
		else
		{ // 遷移完了状態に移行できる範囲にいない
			ResetReadyUI(0);
		}
		break;
	}
}

//=======================================================
// ゴール可能状態を返す
//=======================================================
bool GetGate(int nIdx)
{
	return g_abReady[nIdx];
}
