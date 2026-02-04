//========================================================
// 
// ゲーム画面でのUI表示処理[gameui.h]
// Author : KajinoHiyori
// 
//========================================================
#ifndef _GAMEUI_H_
#define	_GAMEUI_H_

//*****************************************************************************
// ポーズ状態
//*****************************************************************************
typedef enum PAUSE_STATE
{
	PAUSE_STATE_NEUTRAL = 0,		// なにもない
	PAUSE_STATE_CLOCK,				// 時計
	PAUSE_STATE_MAGICBOOK,			// 魔導書
	PAUSE_STATE_MAX,
}PAUSE_STATE;

//*****************************************************************************
// ポーズメニュー
//*****************************************************************************
typedef enum PAUSE_MENU
{
	PAUSE_MENU_CLOCK = 0,			// 時計
	PAUSE_MENU_MAGICBOOK,			// 魔導書
	PAUSE_MENU_CONTINUE,			// ゲームに戻る
	PAUSE_MENU_RESTART,				// ゲームをやり直す
	PAUSE_MENU_QUIT,				// タイトル画面に戻る
	PAUSE_MENU_MAX					// 
}PAUSE_MENU;

// UIの種類
typedef enum
{
	GAMEUI_TYPE_PHONE = 0,	// スマホ
	GAMEUI_TYPE_PAUSE,		// PAUSEタイトル
	GAMEUI_TYPE_CLOCK,		// 時計
	GAMEUI_TYPE_MAGICBOOK,	// 魔導書
	GAMEUI_TYPE_CONTINUE,	// CONTINUE
	GAMEUI_TYPE_RETRY,		// RETRY
	GAMEUI_TYPE_QUIT,		// QUIT
	GAMEUI_TYPE_MAX
}GAMEUI_TYPE;

// GAMEUIの出現状態を管理
typedef enum
{
	GAMEUI_STATE_NONE = -1,	// 何もしていない状態
	GAMEUI_STATE_ONSCREEN,	// 画面内
	GAMEUI_STATE_APPEAR,		// 出現状態
	GAMEUI_STATE_DISAPPEAR,	// はける状態
	GAMEUI_STATE_OFFSCREEN,	// 画面外
}GAMEUI_STATE;

// プロトタイプ宣言
void InitGameUI(void);
void UninitGameUI(void);
void UpdateGameUI(void);
void DrawGameUI(void);

#endif
