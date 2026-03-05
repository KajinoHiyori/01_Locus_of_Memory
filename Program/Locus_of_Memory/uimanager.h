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
	UISTATE_APPEAR,			// 出現[通常]
	UISTATE_SELECT,			// 選択状態[通常]
	UISTATE_DISAPPEAR,		// 収縮[通常]
	UISTATE_CLOCKAPPEAR,	// 出現[時計]
	UISTATE_CLOCK,			// 時計
	UISTATE_CLOCKDISAPPEAR,	// 収縮[時計]
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
	UITEX_BGFILTER,		// 背景フィルター
	UITEX_MAX
}UITEX;

// プロトタイプ宣言
void InitUIManager(void);
void UninitUIManager(void);
void UpdateUIManager(void);
void DrawUIManager(void);
void SetPauseFalse(void);
void SetUIStateNext(int nIdx, UISTATE stateNext);
bool GetPause(int nIdx);
UISTATE GetUIState(int nIdx);
D3DXMATRIX GetUIMatrix(int nIdx);	// UIのマトリックス情報を取得
D3DXVECTOR3 GetUIPos(int nIdx);	// UIの位置を取得
D3DXVECTOR3 GetUIRot(int nIdx);	// UIの角度を取得
void UpdateUIBG(int nIdx);		// 背景の更新処理
void SetUIAppear(int nIdx);		// UIを出現状態にする
void SetUIDissapear(int nIdx);	// UIを消滅状態にする
void SetUISelect(int nIdx);		// UIを選択状態にする
void SetUINonDisp(int nIdx);	// UIを非表示状態にする
void SetClockAppear(int nIdx);		// 時計を出現状態にする
void SetClockMenu(int nIdx);		// 時計を表示状態にする
void SetClockDissapear(int nIdx);	// 時計を消滅状態にする
void SetClockNonDisp(int nIdx);		// 時計を非表示状態にする
#endif