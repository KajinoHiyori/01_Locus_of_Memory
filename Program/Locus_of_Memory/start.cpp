#if 0

#endif // 0

//======================================================================================
// 
// 3Dアクションゲーム_MASTER KEY[Start.cpp]
//
//======================================================================================
#include "main.h"
#include "fade.h"
#include "start.h"
#include "camera.h"
#include "light.h"
#include "input.h"
#include "startui.h"

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
int g_nModeTitle;			// リザルト画面への遷移
bool g_bStartFade;				// フェード状態を管理

//======================================================================================
// タイトルの初期化処理
//======================================================================================
void InitStart(void)
{
	g_nModeTitle = 0;
	g_bStartFade = false;	// trueで遷移中

	SetJoypadVibration(0, 0, 0, 0);
	SetJoypadVibration(0, 0, 0, 1);

	// カメラの初期化処理
	InitCamera();

	// カメラの位置を設定
	Camera* pCamera = GetCamera();
	pCamera->posV = POS_V;
	pCamera->posVDest = POS_V;
	pCamera->posR = POS_R;
	pCamera->posRDest = POS_R;
	pCamera->vecU = VEC_U;
	pCamera->rot = ROT;
	pCamera->rotDest = ROT;

	// ライトの初期化処理
	InitLight();

	// タイトルUIの表示
	InitStartUI();
}

//======================================================================================
// タイトルの終了処理
//======================================================================================
void UninitStart(void)
{
	// カメラの終了処理
	UninitCamera();

	// ライトの終了処理
	UninitLight();

	// タイトルUIの終了処理
	UninitStartUI();
}

//======================================================================================
// タイトルの更新処理
//======================================================================================
void UpdateStart(void)
{
	// 現在のフェードの状態を管理
	FADE* pfade = GetFade();

	// ライトの更新処理
	UpdateLight();

	// タイトルUIの更新処理
	UpdateStartUI();

	if (g_bStartFade == false)	// フェードをしていない場合
	{
		g_nModeTitle++;

		if (g_nModeTitle == 180 && *pfade == FADE_NONE)
		{ // ENTERキー / Aボタン / STARTボタンが押された場合、ゲーム画面に遷移
			g_nModeTitle = 0;
			g_bStartFade = true;
			SetFade(MODE_TITLE);
		}
	}
}

//======================================================================================
// タイトルの描画処理
//======================================================================================
void DrawStart(void)
{
	// タイトルUIの描画処理
	DrawStartUI();
}