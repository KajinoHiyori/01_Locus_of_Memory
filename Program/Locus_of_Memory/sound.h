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
