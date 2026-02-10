//========================================================
// 
// 3D空間用UI処理[uimanager.cpp]
// Author : KajinoHiyori
// 
//========================================================
#ifndef _UIMANAGER_H_
#define _UIMANAGER_H_

// 選択状態
typedef enum
{
	UITYPE_CLOCK = 0,	// 時計状態
	UITYPE_CONTINUE,	// continue
	UITYPE_RETRY,		// retry
	UITYPE_QUIT,		// quit
	UITYPE_MAX
}UITYPE;

// UIの状態
typedef enum
{
	UISTATE_NONDISPLAY = 0,	// 非表示
	UISTATE_APPEAR,			// 出現
	UISTATE_SELECT,			// 選択状態
	UISTATE_CLOCK,			// 時計
	UISTATE_DISAPPEAR,		// 収縮
	UISTATE_MAX
}UISTATE;

// UIのテクスチャの種類
typedef enum
{
	UITEX_BG = 0,		// 背景
	UITEX_BATTERY,		// バッテリー[残量によって変化]
	UITEX_BATTERYFRAME,	// バッテリーのフレーム
	UITEX_PAUSEMENU,	// ポーズメニュー
	UITEX_CLOCKMENU,	// 時計[メニュー]
	UITEX_CLOCK,		// 時計[選択状態]
	UITEX_CONTINUE,		// continue
	UITEX_RETRY,		// retry
	UITEX_QUIT,			// quit
	UITEX_FILTER,		// フィルター
	UITEX_MAX
}UITEX;

// プロトタイプ宣言
void InitUIManager(void);
void UninitUIManager(void);
void UpdateUIManager(void);
void DrawUIManager(void);
bool GetPause(int nIdx);
void SetUIManager(int nIdx);
UISTATE GetUIState(int nIdx);

#endif