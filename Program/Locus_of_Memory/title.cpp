//======================================================================================
// 
// 3Dアクションゲーム_MASTER KEY[title.cpp]
//
//======================================================================================
#include "main.h"
#include "fade.h"
#include "title.h"
#include "camera.h"
#include "light.h"
#include "input.h"
#include "titleui.h"
#include "object.h"
#include "skybox.h"
#include "effect.h"
#include "particle.h"
#include "fog.h"
#include "debugproc.h"
#include "brickwall.h"
#include "color.h"
#include "grain.h"
#include "sparkle.h"
#include "animal.h"
#include "2deffect.h"

// マクロ定義
#define NEXT_MODE		(600)		// 次のモードへの遷移時間
#define WIDTH			(650.0f)	// 幅
#define HEIGHT			(370.0f)	// 高さ
#define DISTANCE		(100.0f)	// 視点注視点間の距離
#define DEFAULT			(D3DXVECTOR3(0.0f, 0.0f, 0.0f))	// xyz0.0f
#define POS_V			(D3DXVECTOR3(0.0f, -50.0f, -200.0f))	// 視点の位置
#define POS_R			(D3DXVECTOR3(0.0f, -20.0f, 0.0f))	// 注視点の位置
#define ROT				(D3DXVECTOR3(0.0f, -D3DX_PI / 4, 0.0f))	// カメラのrot
#define VEC_U			(D3DXVECTOR3(0.0f, 1.0f, 0.0f))			// カメラの向き

// グローバル変数
int g_nModeResult;			// リザルト画面への遷移
bool g_bFade;				// フェード状態を管理
OPERATIONTYPE g_operationType;	// 操作方法

// 霧の演出のテスト
float g_fTitleFogEnd;			// タイトル画面の霧の終了位置

//======================================================================================
// タイトルの初期化処理
//======================================================================================
void InitTitle(void)
{
	g_nModeResult = 0;
	g_bFade = false;	// trueで遷移中

	SetJoypadVibration(0, 0, 0, 0);
	SetJoypadVibration(0, 0, 0, 1);

	// カメラの位置を設定
	Camera* pCamera = GetCamera();
	pCamera->posV = POS_V;
	pCamera->posVDest = POS_V;
	pCamera->posR = POS_R;
	pCamera->posRDest = POS_R;
	pCamera->vecU = VEC_U;
	pCamera->rot = ROT;
	pCamera->rotDest = ROT;

	g_fTitleFogEnd = 5000.0f;

	// ライトの初期化処理
	InitLight();

	// タイトルUIの表示
	InitTitleUI();

	// 2Dエフェクトの初期化処理
	Init2DEffect();

	g_operationType = OPERATIONTYPE_1P;

	// ドラゴンのタイプを決定
	SetDragonType(0, DRAGONTYPE_FLYING);
}

//======================================================================================
// タイトルの終了処理
//======================================================================================
void UninitTitle(void)
{

	// ライトの終了処理
	UninitLight();

	// タイトルUIの終了処理
	UninitTitleUI();

	// 2Dエフェクトの終了処理
	Uninit2DEffect();
}

//======================================================================================
// タイトルの更新処理
//======================================================================================
void UpdateTitle(void)
{
	PrintDebugProc("ここはタイトル画面\n");

	// 現在のフェードの状態を管理
	FADE *pfade = GetFade();

	// ライトの更新処理
	UpdateLight();

	// ドラゴンの更新処理
	UpdateAnimal();

	// タイトルUIの更新処理
	UpdateTitleUI();

	// 2Dエフェクトの更新処理
	Update2DEffect();

	// 霧の演出のテスト
	if (g_fTitleFogEnd < 50000.0f)
	{
		g_fTitleFogEnd += (50000.0f * 0.00075f);
	}

	SetFog(COLOR_SKY_NIGHT, 0.0f, g_fTitleFogEnd);

	if (g_bFade == false)	// フェードをしていない場合
	{
		g_nModeResult++;
		if (GetKeyboardAny() == true || GetJoypadAny(0) == true)
		{
			g_nModeResult = 0;
		}
		if (g_nModeResult >= NEXT_MODE)
		{
			SetFade(MODE_DIAGNOSIS, COLOR_WHITE);
			g_bFade = true;
		}
		else if ((GetKeyboardTrigger(DIK_RETURN) == true || GetJoypadTrigger(JOYKEY_A, 0) == true || GetJoypadTrigger(JOYKEY_START, 0) == true) && *pfade == FADE_NONE)
		{ // ENTERキー / Aボタン / STARTボタンが押された場合、ゲーム画面に遷移
			g_nModeResult = 0;
			g_bFade = true;
			SetFade(MODE_TUTORIAL, COLOR_WHITE);
		}
	}
}

//======================================================================================
// タイトルの描画処理
//======================================================================================
void DrawTitle(void)
{
	// オブジェクトの描画処理（仮）
	DrawObject();

	// スカイボックスの描画処理
	DrawSkyBox();

	// 塀の描画処理
	DrawBrickWall();

	// 動物の描画処理
	DrawAnimal();

	// エフェクトの描画処理
	DrawEffect();

	// パーティクルの描画処理
	DrawParticle();

	// 文字の描画処理
	DrawGrain();

	// 粒の描画処理
	DrawSparkle();

	// タイトルUIの描画処理
	DrawTitleUI();

	// 2Dエフェクトの描画処理
	Draw2DEffect();
}

//======================================================================================
// 操作方法の決定
//======================================================================================
void SetOperationType(OPERATIONTYPE type)
{
	g_operationType = type;
}

//======================================================================================
// 操作方法の取得
//======================================================================================
OPERATIONTYPE GetOperationType(void)
{
	return g_operationType;
}