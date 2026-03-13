//=============================================================================
//
// サウンド処理 [sound.h]
// Author : SHUMA AIZU
//          AKIRA TANAKA
//
//=============================================================================
#ifndef _SOUND_H_
#define _SOUND_H_

#include "main.h"

//*****************************************************************************
// サウンド一覧
//*****************************************************************************
// 
// 1Pと2Pとで、再生と停止を分ける必要があるためこのような形式を取っています
typedef enum
{
	SOUND_LABEL_TITLEBGM = 0,		// titleBGM
	SOUND_LABEL_TUTORIALBGM,		// tutorialBGM
	SOUND_LABEL_GAMEBGM,			// gameBGM
	SOUND_LABEL_DIAGNOSISBGM,		// dignosisBGM
	SOUND_LABEL_FAILDRESULTBGM,		// faildBGM
	SOUND_LABEL_GOALRESULTBGM,		// goalBGM
	SOUND_LABEL_ENTER0,				// 決定[1P]
	SOUND_LABEL_ENTER1,				// 決定[2P]
	SOUND_LABEL_SELECT0,			// 選択[1P]
	SOUND_LABEL_SELECT1,			// 選択[2P]
	SOUND_LABEL_LEVITATION0,		// 浮遊[1P]
	SOUND_LABEL_LEVITATION1,		// 浮遊[2P]
	SOUND_LABEL_COMBUSTION0,		// 燃焼[1P]
	SOUND_LABEL_COMBUSTION1,		// 燃焼[2P]
	SOUND_LABEL_FLASH0,				// フラッシュ[1P]
	SOUND_LABEL_FLASH1,				// フラッシュ[2P]
	SOUND_LABEL_SUNSETDELAY0,		// 時間停止[1P]
	SOUND_LABEL_SUNSETDELAY1,		// 時間停止[2P]
	SOUND_LABEL_RAINPRAY0,			// 雨乞い[1P]
	SOUND_LABEL_RAINPRAY1,			// 雨乞い[2P]
	SOUND_LABEL_FREEZE0,			// 凍結[1P]
	SOUND_LABEL_FREEZE1,			// 凍結[2P]
	SOUND_LABEL_GROWTH0,			// 成長[1P]
	SOUND_LABEL_GROWTH1,			// 成長[2P]
	SOUND_LABEL_ACCELERATION0,		// 加速[1P]
	SOUND_LABEL_ACCELERATION1,		// 加速[2P]
	SOUND_LABEL_TIMEREVERT0,		// 巻き戻し[1P]
	SOUND_LABEL_TIMEREVERT1,		// 巻き戻し[2P]
	SOUND_LABEL_WALK0,				// 歩き[1P]
	SOUND_LABEL_WALK1,				// 歩き[2P]
	SOUND_LABEL_RUN0,				// 走り[1P]
	SOUND_LABEL_RUN1,				// 走り[2P]
	SOUND_LABEL_PAUSE0,				// ポーズ[1P]
	SOUND_LABEL_PAUSE1,				// ポーズ[2P]
	SOUND_LABEL_SPELL0,				// SPELL[1P]
	SOUND_LABEL_SPELL1,				// SPELL[2P]
	SOUND_LABEL_MAGICFAILD0,		// 魔法イベント不発[1P]
	SOUND_LABEL_MAGICFAILD1,		// 魔法イベント不発[2P]
	SOUND_LABEL_JUMP0,				// ジャンプ[1P]
	SOUND_LABEL_JUMP1,				// ジャンプ[2P]
	SOUND_LABEL_LANDING0,			// 着地[1P]
	SOUND_LABEL_LANDING1,			// 着地[2P]
	SOUND_LABEL_COMMAND0,			// コマンド入力[1P]
	SOUND_LABEL_COMMAND1,			// コマンド入力[2P]
	SOUND_LABEL_QUESTCLEAR,			// クエストクリア
	SOUND_LABEL_GETMAGIC0,			// 魔法取得
	SOUND_LABEL_GETMAGIC1,			// 魔法取得
	SOUND_LABEL_MAX,
} SOUND_LABEL;

//*****************************************************************************
// プロトタイプ宣言
//*****************************************************************************
HRESULT InitSound(HWND hWnd);
void UninitSound(void);
HRESULT PlaySound(SOUND_LABEL label);
void StopSound(SOUND_LABEL label);
void StopSound(void);
bool CheckSound(SOUND_LABEL label);

#endif
