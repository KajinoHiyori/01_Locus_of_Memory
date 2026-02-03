//=============================================================================
//
//	カメラの処理 [camera.h]
//	Author : SHUMA AIZU
// 
//=============================================================================

#ifndef _CAMERA_H_
#define _CAMERA_H_

#include "main.h"

#include "title.h"
#include "player.h"

//*****************************************************************************
// マクロ定義
//*****************************************************************************
#define CAMERAPOSR_DIS			(100.0f)									// 視点と注視点との距離
#define CAMERA_INERTIA			(0.115f)									// カメラ移動の慣性係数
#define MAX_CAMERA				(2)											// カメラの最大数
#define CAMERA_2PPLAY			(2)											// 2Pプレイのカメラ数
#define AHEADVEIW				(15.0f)										// 前方表示の距離
#define UPVEIW					(75.0f)										// 頭上表示の距離

// 各種初期値
#define INIT_NUMCAMERA			(1)											// カメラの初期数
#define INIT_VECU				(D3DXVECTOR3(0.0f, 1.0f, 0.0f))				// カメラの上方向ベクトル
#define INIT_1PCAMERAPOSV		(D3DXVECTOR3(0.0f, 100.0f, 0.0f))			// 1Pカメラの視点
#define INIT_1PCAMERAPOSR		(D3DXVECTOR3(0.0f, 50.0f, 0.0f))			// 1Pカメラの注視点
#define INIT_1PCAMERAROT		(D3DXVECTOR3(0.0f, D3DX_PI, 0.0f))			// 1Pカメラの向き
#define INIT_2PCAMERAPOSV		(D3DXVECTOR3(0.0f, 100.0f, 0.0f))			// 2Pカメラの視点
#define INIT_2PCAMERAPOSR		(D3DXVECTOR3(0.0f, 50.0f, 0.0f))			// 2Pカメラの注視点
#define INIT_2PCAMERAROT		(D3DXVECTOR3(0.0f, D3DX_PI, 0.0f))			// 2Pカメラの向き
#define INIT_RADIUS				(200.0f)									// 視点と注視点との距離(半径)
#define INIT_CAMERASPEED		(0.025f)									// カメラの移動速度

// タイトル
#define INIT_TITLECAMERA_POSV	(D3DXVECTOR3(3500.0f, 550.0f, -2275.0f))	// タイトルカメラの視点
#define INIT_TITLECAMERA_POSR	(D3DXVECTOR3(3375.0f, 605.0f, -2125.0f))	// タイトルカメラの注視点
#define INIT_TITLECAMERA_ROT	(D3DXVECTOR3(0.0f, 2.44f, 0.0f))			// タイトルカメラの向き

// ビューポート生成用マクロ
#define DEFAULT_VEIWPORT		(SetViewPort((DWORD)0.0f, (DWORD)0.0f, (DWORD)SCREEN_WIDTH, (DWORD)SCREEN_HEIGHT))
#define INIT_1PVEIWPORT			(SetViewPort((DWORD)0.0f, (DWORD)0.0f, (DWORD)(SCREEN_WIDTH / 2), (DWORD)SCREEN_HEIGHT))
#define INIT_2PVEIWPORT			(SetViewPort((DWORD)(SCREEN_WIDTH / 2), (DWORD)0.0f, (DWORD)(SCREEN_WIDTH / 2), (DWORD)SCREEN_HEIGHT))
#define INIT_3PVEIWPORT			(SetViewPort((DWORD)0.0f, (DWORD)(SCREEN_HEIGHT / 2), (DWORD)(SCREEN_WIDTH / 2), (DWORD)(SCREEN_HEIGHT / 2)))
#define INIT_4PVEIWPORT			(SetViewPort((DWORD)(SCREEN_WIDTH / 2), (DWORD)(SCREEN_HEIGHT / 2), (DWORD)(SCREEN_WIDTH / 2), (DWORD)(SCREEN_HEIGHT / 2)))

//*****************************************************************************
// カメラの構造体定義
//*****************************************************************************
typedef struct Camera
{
	D3DXVECTOR3 posV;				// 視点
	D3DXVECTOR3 posR;				// 注視点
	D3DXVECTOR3 vecU;				// 上方向ベクトル
	D3DXVECTOR3 posVDest;			// 目的の視点
	D3DXVECTOR3 posRDest;			// 目的の注視点
	D3DXVECTOR3 rot;				// 向き
	D3DXVECTOR3 rotDest;			// 目的の向き
	float fSpeed;					// 移動スピード
	float fRadiusVertical;			// 縦軸距離(基本)
	float fRadiusHorizonttal;		// 横軸距離
	int nRollCounter;				// 回り込みカウント
	Player* pPlayer;				// 追従してるプレイヤーのポインタ
	D3DXMATRIX mtxProjection;		// プロジェクションマトリックス
	D3DXMATRIX mtxView;				// ビューマトリックス
	D3DVIEWPORT9 viewport;			// ビューポート
}Camera;

//*****************************************************************************
// カメラ情報構造体定義
//*****************************************************************************
typedef struct CameraInfo
{
	D3DXVECTOR3 posV;				// 視点
	D3DXVECTOR3 posR;				// 注視点
	D3DXVECTOR3 vecU;				// 上方向ベクトル
	D3DXVECTOR3 rot;				// 向き
	D3DVIEWPORT9 viewport;			// ビューポート
}CameraInfo;

//*****************************************************************************
// プロトタイプ宣言
//*****************************************************************************
void InitCamera(void);
void UninitCamera(void);
void UpdateCamera(MODE mode);
void UpdateStartCamera(void);
void UpdateTitleCamera(void);
void UpdateGameCamera(void);
void UpdateResultCamera(void);
void UpdateDiagnosisCamera(void);
void SetCamera(int nIdx);
void SetModeCamera(MODE mode);
Camera* GetCamera(void);
void SetNumCamera(void);
int GetNumCamera(void);
D3DVIEWPORT9 SetViewPort(DWORD X, DWORD Y, DWORD Width, DWORD Height);

#endif