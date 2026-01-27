#if 0


#endif // 0
//======================================================================================
// 
// 3Dアクションゲーム_MASTER KEY[diagnosis.cpp]
//
//======================================================================================
#include "main.h"
#include "fade.h"
#include "diagnosis.h"
#include "camera.h"
#include "light.h"
#include "input.h"
#include "diagnosisui.h"

// マクロ定義
#define WIDTH			(650.0f)	// 幅
#define HEIGHT			(370.0f)	// 高さ
#define DISTANCE		(100.0f)	// 視点注視点間の距離
#define DEFAULT			(D3DXVECTOR3(0.0f, 0.0f, 0.0f))	// xyz0.0f
#define POS_V			(D3DXVECTOR3(0.0f, -50.0f, -200.0f))	// 視点の位置
#define POS_R			(D3DXVECTOR3(0.0f, -20.0f, 0.0f))	// 注視点の位置
#define ROT				(D3DXVECTOR3(0.0f, -D3DX_PI / 4, 0.0f))	// カメラのrot
#define VEC_U			(D3DXVECTOR3(0.0f, 1.0f, 0.0f))			// カメラの向き

// グローバル変数
bool g_bFadeScane;				// フェード状態を管理
DIAGNOSISTYPE g_diagnosisType;	// 操作方法

//======================================================================================
// タイトルの初期化処理
//======================================================================================
void InitDiagnosis(void)
{
	g_bFadeScane = false;	// trueで遷移中

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
	InitDiagnosisUI();

	g_diagnosisType = DIAGNOSISTYPE_1P;
}

//======================================================================================
// タイトルの終了処理
//======================================================================================
void UninitDiagnosis(void)
{
	// カメラの終了処理
	UninitCamera();

	// ライトの終了処理
	UninitLight();

	// タイトルUIの終了処理
	UninitDiagnosisUI();
}

//======================================================================================
// タイトルの更新処理
//======================================================================================
void UpdateDiagnosis(void)
{
	// 現在のフェードの状態を管理
	FADE *pfade = GetFade();

	// ライトの更新処理
	UpdateLight();

	// タイトルUIの更新処理
	UpdateDiagnosisUI();


	if ((GetKeyboardTrigger(DIK_RETURN) == true || GetJoypadTrigger(JOYKEY_A, 0) == true || GetJoypadTrigger(JOYKEY_START, 0) == true) && *pfade == FADE_NONE)
	{ // ENTERキー / Aボタン / STARTボタンが押された場合、ゲーム画面に遷移
		g_bFadeScane = true;
		SetFade(MODE_TITLE);
	}
	
}

//======================================================================================
// タイトルの描画処理
//======================================================================================
void DrawDiagnosis(void)
{
	// タイトルUIの描画処理
	DrawDiagnosisUI();
}

//======================================================================================
// 操作方法の決定
//======================================================================================
void SetDiagnosisType(DIAGNOSISTYPE type)
{
	g_diagnosisType = type;
}

//======================================================================================
// 操作方法の取得
//======================================================================================
DIAGNOSISTYPE GetDiagnosisType(void)
{
	return g_diagnosisType;
}
