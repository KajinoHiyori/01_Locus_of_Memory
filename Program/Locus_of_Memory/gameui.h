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
	GAMEUI_TYPE_CLOCK = 0,	// 時計
	GAMEUI_TYPE_MAGICBOOK,	// 魔導書
	GAMEUI_TYPE_CONTINUE,	// CONTINUE
	GAMEUI_TYPE_RETRY,		// RETRY
	GAMEUI_TYPE_QUIT,		// QUIT
	GAMEUI_TYPE_PAUSE,		// PAUSEタイトル
	GAMEUI_TYPE_PHONE,		// スマホ
	GAMEUI_TYPE_1PALPHA,		// ポーズ背景を暗くする[1P]
	GAMEUI_TYPE_2PALPHA,		// ポーズ背景を暗くする[2P]
	GAMEUI_TYPE_MAX
}GAMEUI_TYPE;

// プロトタイプ宣言
void InitGameUI(void);
void UninitGameUI(void);
void UpdateGameUI(void);
void DrawGameUI(void);
void SetGameUI(int pauseMenu);

#endif
