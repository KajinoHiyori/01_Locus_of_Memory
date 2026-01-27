//========================================================
// 
// ゲーム画面でのUI表示処理[gameui.h]
// Author : KajinoHiyori
// 
//========================================================
#ifndef _GAMEUI_H_
#define	_GAMEUI_H_

// UIの種類
typedef enum
{
	PAUSE_MENU_CLOCK = 0,	// 時計
	PAUSE_MENU_MAGICBOOK,	// 魔導書
	PAUSE_MENU_CONTINUE,	// CONTINUE
	PAUSE_MENU_RETRY,		// RETRY
	PAUSE_MENU_QUIT,		// QUIT
	PAUSE_MENU_PAUSE,		// PAUSEタイトル
	PAUSE_MENU_PHONE,		// スマホ
	PAUSE_MENU_1PALPHA,		// ポーズ背景を暗くする[1P]
	PAUSE_MENU_2PALPHA,		// ポーズ背景を暗くする[2P]
	PAUSE_MENU_MAX
}PAUSE_MENU;

// プロトタイプ宣言
void InitPause(void);
void UninitPause(void);
void UpdatePause(void);
void DrawPause(void);
void SetPause(int pauseMenu);

#endif
