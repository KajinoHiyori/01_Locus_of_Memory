//=============================================================================
//
//	魔法陣の処理 [magiccircle.h]
//	Author : SHUMA AIZU
// 
//=============================================================================

#ifndef _MAGICCIRCLE_H_
#define _MAGICCIRCLE_H_

#include "main.h"
#include "magic.h"

//*****************************************************************************
// マクロ定義
//*****************************************************************************
#define MAGICCIRCLE_RADIUS			(150.0f)			// 魔法陣の半径
#define MAX_MAGICCIRCLE				(4)					// 魔法陣の最大数

//*****************************************************************************
// 魔法陣の構造体定義
//*****************************************************************************
typedef struct
{
	D3DXVECTOR3 pos;							// 位置
	D3DXVECTOR3* PosParent;						// 親の位置
	D3DXVECTOR3 rot;							// 向き
	MAGICTYPE MagicType;						// 魔法の種類
	float fRadius;								// 今の半径
	float fRadiusDest;							// 目標半径
	int nLife;									// 仮置きの寿命
	D3DXMATRIX mtxWorld;						// ワールドマトリックス
	bool bUse;									// 使用状態
}MagicCircle;

//*****************************************************************************
// プロトタイプ宣言
//*****************************************************************************
void InitMagicCircle(void);
void UninitMagicCircle(void);
void UpdateMagicCircle(void);
void DrawMagicCircle(void);
void SetMagicCircle(MAGICTYPE MagicType, D3DXVECTOR3 *PosParent);
void SetPositionMagicCircle(int IdxMagicCircle, D3DXVECTOR3 pos);

#endif