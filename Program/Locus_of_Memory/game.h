//=============================================================================
//
//	Locus of Memory [game.h]
//	Author : HAYATO NAMBA
// 
//=============================================================================
#ifndef _GAME_H_        // このマクロ定義がされなかったら
#define _GAME_H_        // 2重インクルード防止のマクロを定義する

#include"main.h"

// ゲームの状態列挙型の定義
typedef enum
{
	GAMESTATE_NONE = 0,		// 何もしていない状態
	GAMESTATE_NORMAL,		// 通常状態
	GAMESTATE_CLEAR,		// クリア状態
	GAMESTATE_TIMEOVER,		// 時間切れ状態
	GAMESTATE_BATTERYOVER,	// バッテリー切れ状態
	GAMESTATE_MAX
}GAMESTATE;

typedef enum
{
	EVENTSTATE_NORMAL = 0,
	EVENTSTATE_LOOKDOWN,	// 見下ろし状態
	EVENTSTATE_BOOK,		// 本を見る状態
	EVENTSTATE_MAX
}EVENTSTATE;

// プロトタイプ宣言
void InitGame(void);
void UninitGame(void);
void UpdateGame(void);
void DrawGame(void);
void SetGameState(GAMESTATE state, int nCounter);
GAMESTATE GetGameState(void);
EVENTSTATE *GetEventState(void);

#endif