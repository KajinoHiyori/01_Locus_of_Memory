//=============================================================================
//
//	Locus of Memory [game.cpp]
//	Author : HAYATO NAMBA
// 
//=============================================================================
#include "main.h"
#include "game.h"
#include "color.h"
#include "input.h"
#include "debugproc.h"
#include "fade.h"
#include "sound.h"
#include "fog.h"
#include "effect.h"
#include "particle.h"
#include "camera.h"
#include "player.h"
#include "magic.h"
#include "uimanager.h"
#include "magicui.h"
#include "magicbubble.h"
#include "readyui.h"
#include "magiccircle.h"
#include "magiceffect.h"
#include "spellui.h"
#include "clock.h"
#include "battery.h"
#include "object.h"
#include "eventobject.h"
#include "skybox.h"
#include "effect.h"
#include "particle.h"
#include "light.h"
#include "shadow.h"
#include "vibration.h"
#include "meshfield.h"
#include "brickwall.h"
#include "loadscript.h"
#include "grain.h"
#include "sparkle.h"
#include "goal.h"
#include "custommesh.h"
#include "animal.h"
#include "field.h"
#include "key.h"
#include "riverwall.h"
#include "questionmark.h"
#include "aura.h"
#include "questui.h"

// マクロ定義
#define GAMEPOS_1P		(D3DXVECTOR3(-745.0f, 0.0f, -3427.0f))	// 1Pの位置[GAME]
#define GAMEPOS_2P		(D3DXVECTOR3(-775.0f, 0.0f, -3427.0f))	// 2Pの位置[GAME]
#define PLAYER_ROT		(D3DXVECTOR3(0.0f, -1.57f, 0.0f))	// プレイヤーの向き

GAMESTATE g_gameState = GAMESTATE_NONE;		// ゲームの状態
EVENTSTATE g_eventState;
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

	// イベントオブジェクトの初期化処理
	InitEventObject();

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

	// 準備完了状態の初期化処理
	InitReadyUI();

	// 鍵の初期化処理
	InitKey();

	// エフェクトの初期化処理
	InitEffect();

	// パーティクルの初期化処理
	InitParticle();

	// マジックエフェクトの初期化
	InitMagicEffect();

	// オーラの初期化処理
	InitAura();

	// クエストUIの初期化処理
	InitQuestUI();

	pVibration->Vibration = false;

	// 魔法関連オブジェクトの読み込み
	LoadMagicObject(MAGICOBJECTINFO_SCRIPT);

	// ゲームの状態を通常状態に
	g_gameState = GAMESTATE_NORMAL;

	//俯瞰状態に
	g_eventState = EVENTSTATE_LOOKDOWN;

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

	// ドラゴンの状態を設定
	SetDragonType(0, DRAGONTYPE_FIRE);

	//ResetObject();
}
//=======================================================
// ゲームの終了処理
//=======================================================
void UninitGame(void)
{
	// BGMなどを停止
	StopSound();

	// ライトの終了処理
	UninitLight();

	// 影の終了処理
	UninitShadow();

	// イベントオブジェクトの終了処理
	UninitEventObject();

	// プレイヤーの終了処理
	UninitPlayer();

	// 魔法の終了処理
	UninitMagic();

	// 魔法陣の終了処理
	UninitMagicCircle();

	// ?の終了処理
	UninitQuestionMark();

	// 準備完了状態の終了処理
	UninitReadyUI();

	// 鍵の終了処理
	UninitKey();

	// オーラの終了処理
	UninitAura();

	// クエストUIの終了処理
	UninitQuestUI();

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

	// 鍵の更新処理
	UpdateKey();

	// オーラの更新処理
	UpdateAura();

	// クエストUIの更新処理
	UpdateQuestUI();

	switch (g_eventState)
	{
	case EVENTSTATE_NORMAL:
		break;

	case EVENTSTATE_LOOKDOWN:
		//俯瞰状態の時

		break;

	case EVENTSTATE_BOOK:
		break;

	}

	bool bNextMode = GetGoalState();

	switch (g_gameState)
	{
	case GAMESTATE_NORMAL:		// 通常状態
	
		break;

	case GAMESTATE_CLEAR:			// 終了状態
		g_nCounterGameState--;		// 状態管理カウンター減少

		if (bNextMode == true && *pFade == FADE_NONE)
		{// 0以下になった
			g_gameState = GAMESTATE_CLEAR;

			// フェード設定(リザルト画面に移行)
			SetFade(MODE_RESULT, COLOR_WHITE);

			// goalBGMを再生
			//PlaySound(SOUND_LABEL_GOALRESULTBGM);
		}

		break;

	case GAMESTATE_TIMEOVER:
		g_nCounterGameState--;		// 状態管理カウンター減少

		if (g_nCounterGameState <= 0 && *pFade == FADE_NONE)
		{// 0以下になった
			g_gameState = GAMESTATE_TIMEOVER;

			// フェード設定(リザルト画面に移行)
			SetFade(MODE_RESULT, COLOR_BLACK);

			// faildBGMを再生
			//PlaySound(SOUND_LABEL_FAILDRESULTBGM);
		}
		break;

	case GAMESTATE_BATTERYOVER:
		g_nCounterGameState--;		// 状態管理カウンター減少

		if (g_nCounterGameState <= 0 && *pFade == FADE_NONE)
		{// 0以下になった
			g_gameState = GAMESTATE_BATTERYOVER;

			// フェード設定(リザルト画面に移行)
			SetFade(MODE_RESULT, COLOR_BLACK);

			// faildBGMを再生
			//PlaySound(SOUND_LABEL_FAILDRESULTBGM);
		}
		break;
	}

	// Oでクリア遷移
	if (GetKeyboardTrigger(DIK_O) == true && *pFade == FADE_NONE)
	{// 0以下になった
		SetGameState(GAMESTATE_CLEAR, 0);

		// フェード設定(リザルト画面に移行)
		SetFade(MODE_RESULT, COLOR_WHITE);

		for (int nCntVibration = 0; nCntVibration < MAX_PLAYER; nCntVibration++)
		{
			VibrationType(VIBRATIONTYPE_NOTHING, VIBRATION_CLEAR, nCntVibration);
		}
	}
	// Iで失敗遷移
	if (GetKeyboardTrigger(DIK_I) == true && *pFade == FADE_NONE)
	{// 0以下になった
		SetGameState(GAMESTATE_BATTERYOVER, 0);
		// フェード設定(リザルト画面に移行)
		SetFade(MODE_RESULT, COLOR_BLACK);

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
	ProcessingSpeed(true);

	// メッシュフィールドの描画処理
	DrawMeshField();

	// フィールドの描画処理
	DrawField();

	// 川の壁の描画処理
	DrawRiverWall();

	// カスタムメッシュの描画処理
	DrawCustomMesh();

	// 影の描画処理
	DrawShadow();

	// スカイボックスの描画処理
	DrawSkyBox();

	// 塀の描画処理
	DrawBrickWall();

	// 動物の描画処理
	DrawAnimal();
	
	// オブジェクトの描画処理（仮）
	DrawObject();

	// イベントオブジェクトの描画処理
	DrawEventObject();
	
	// 鍵の描画処理
	DrawKey();

	// ゴールの描画処理
	DrawGoal();

	// エフェクトの描画処理
	DrawEffect();

	// パーティクルの描画処理
	DrawParticle();

	// マジックエフェクトの描画処理
	DrawMagicEffect();

	// 文字の描画処理
	DrawGrain();

	// 粒の描画処理
	DrawSparkle();

	// オーラの描画処理
	DrawAura();

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

	// UIの統括管理の描画処理
	DrawUIManager();

	// 吹き出しの描画処理
	DrawMagicBubble();

	// 魔法発動状態表示の描画処理
	DrawSpellUI();

	// 魔導書表示の描画処理
	DrawMagicUI();

	// 時計の描画処理
	DrawClock();

	// バッテリーの描画処理
	DrawBattery();

	// ?の描画処理
	DrawQuestionMark();

	// 準備完了状態の描画処理
	DrawReadyUI();

	// クエストUIの描画処理
	DrawQuestUI();

	SetFogEnable(true);			// 霧を有効

	ProcessingSpeed(false, "aaa");
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

//=======================================================
// ゲームのイベント状態取得処理
//=======================================================
EVENTSTATE *GetEventState(void)
{
	return &g_eventState;
}