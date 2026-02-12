//=============================================================================
//
//	魔法陣の処理 [magiccircle.h]
//	Author : SHUMA AIZU
// 
//=============================================================================

#ifndef _MAGICCIRCLE_H_
#define _MAGICCIRCLE_H_

#include "main.h"

//*****************************************************************************
// マクロ定義
//*****************************************************************************
#define MAGICCIRCLE_WIDTH			(100.0f)			// 魔法陣の幅
#define MAGICCIRCLE_HEIGHT			(50.0f)				// 魔法陣の高さ
#define MAX_MAGICCIRCLE			(4)					// 魔法陣の最大数

//*****************************************************************************
// 魔法陣の構造体定義
//*****************************************************************************
typedef struct
{
	D3DXVECTOR3 pos;							// 位置
	D3DXVECTOR3 rot;							// 向き
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
void SetMagicCircle(D3DXVECTOR3 pos, D3DXVECTOR3 rot);
void SetPositionMagicCircle(int IdxMagicCircle, D3DXVECTOR3 pos);

#endif