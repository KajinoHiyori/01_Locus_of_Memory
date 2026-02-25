//==========================================================================
//
//	魔法動物の処理[animal.h]
//	Author:OBIKA SOUMA
//
//==========================================================================
#ifndef _ANIMAL_H_
#define _ANIMAL_H_

#include "main.h"
#include "object.h"

// ドラゴンの種類
typedef enum
{
	DRAGONTYPE_NONE = 0,	// 何もしていない状態
	DRAGONTYPE_FLYING,		// 飛行状態
	DRAGONTYPE_FIRE,		// 火竜状態
	DRAGONTYPE_MAX
}DRAGONTYPE;

//=====================================
//	プロトタイプ宣言
//=====================================
void InitAnimal(void);
void UninitAnimal(void);
void UpdateAnimal(void);
void DrawAnimal(void);
void SetDragon(int nIdx);
void SetDragonType(int nCntDragon, DRAGONTYPE type);
void UpdateFlyDragon(int nCntDragon);
void UpdateFireDragon(int nCntDragon);

#endif