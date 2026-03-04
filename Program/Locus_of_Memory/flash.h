//=============================================================================
//
//	Locus of Memory [fade.h]
//	Author : HAYATO NAMBA
// 
//=============================================================================
#ifndef _Flash_H        // このマクロ定義がされなかったら
#define _Flash_H        // 2重インクルード防止のマクロを定義する

#include"main.h"

#define SCREEN_ZERO	(0)

typedef enum
{
	FADE_NONE = 0,
	FADE_IN,
	FADE_OUT,
	FADE_MAX
}FADE;

// プロトタイプ宣言
void InitFlash(MODE modeNext, D3DXCOLOR col);
void UninitFlash(void);
void UpdateFlash(void);
void DrawFlash(void);
void SetFlash(MODE modeNext, D3DXCOLOR col);
FADE* GetFlash(void);

#endif