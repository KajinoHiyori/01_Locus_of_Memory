//=============================================================================
//
//	オーラの処理 [aura.h]
//	Author : SHUMA AIZU
// 
//=============================================================================

#ifndef _AURA_H_
#define _AURA_H_

#include "main.h"

//*****************************************************************************
// マクロ定義
//*****************************************************************************
#define AURA_RADIUS			(750.0f)			// オーラの半径
#define AURA_HEIGHT			(175.0f)			// オーラの高さ
#define AURA_SPLIT_WIDHT	(32 + 1)			// 横の分割数
#define AURA_SPLIT_HEIGTH	(1 + 1)				// 縦の分割数
#define MAX_AURA			(4)					// オーラの最大数

//*****************************************************************************
// オーラの構造体定義
//*****************************************************************************
typedef struct Aura
{
	LPDIRECT3DVERTEXBUFFER9 pVtxBuff;			// 頂点バッファへのポインタ
	LPDIRECT3DINDEXBUFFER9 pIdxBuff;			// インデックスバッファへのポインタ
	D3DXVECTOR3 pos;							// 位置
	D3DXVECTOR3 rot;							// 向き
	D3DXMATRIX mtxWorld;						// ワールドマトリックス
	float fAngle;								// 角度
	float fRadius;								// 半径
	bool bUse;									// 使用状態
}Aura;

//*****************************************************************************
// プロトタイプ宣言
//*****************************************************************************
void InitAura(void);
void UninitAura(void);
void UpdateAura(void);
void DrawAura(void);
void SetAura(D3DXVECTOR3 pos, D3DXVECTOR3 rot);

#endif