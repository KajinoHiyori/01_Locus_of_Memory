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
typedef enum
{
	SOUND_LABEL_TITLEBGM = 0,		// titleBGM
	SOUND_LABEL_TUTORIALBGM,		// tutorialBGM
	SOUND_LABEL_GAMEBGM,			// gameBGM
	SOUND_LABEL_DIAGNOSISBGM,		// dignosisBGM
	SOUND_LABEL_FAILDRESULTBGM,		// faildBGM
	SOUND_LABEL_GOALRESULTBGM,		// goalBGM
	SOUND_LABEL_ENTER,				// 決定
	SOUND_LABEL_SELECT,				// 選択
	SOUND_LABEL_LEVITATION,			// 浮遊
	SOUND_LABEL_COMBUSTION,			// 燃焼
	SOUND_LABEL_FLASH,				// フラッシュ
	SOUND_LABEL_SUNSETDELAY,		// 時間停止
	SOUND_LABEL_RAINPRAY,			// 雨乞い
	SOUND_LABEL_FREEZE,				// 凍結
	SOUND_LABEL_GROWTH,				// 成長
	SOUND_LABEL_ACCELERATION,		// 加速
	SOUND_LABEL_TIMEREVERT,			// 巻き戻し
	SOUND_LABEL_WALK,				// 歩き
	SOUND_LABEL_RUN,				// 走り
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
