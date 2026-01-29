//========================================================
// 
// 魔導書のUI表示処理[magicui.h]
// Author : KajinoHiyori
// 
//========================================================
#ifndef _MAGICUI_H_
#define	_MAGICUI_H_

#if 0
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
	GAMEUI_TYPE_MAX
}GAMEUI_TYPE;

// UIの状態
typedef enum
{
	GAMEUI_STATE_NONE,	// 何もしていない
	GAMEUI_STATE_1PPAUSE,	// 1Pのポーズ
	GAMEUI_STATE_2PPAUSE,	// 2Pのポーズ
}GAMEUI_STATE;

// プロトタイプ宣言
void InitGameUI(void);
void UninitGameUI(void);
void UpdateGameUI(void);
void DrawGameUI(void);
void SetGameUI(int pauseMenu);
#endif
#endif
