//=============================================================================
//
//	フィールド処理 [field.h]
//	Author : SHUMA AIZU
// 
//=============================================================================

#ifndef _FIELD_H_
#define _FIELD_H_

#include "main.h"

//*****************************************************************************
// マクロ定義
//*****************************************************************************
#define MAX_FIELDVTX						(1000)				// 頂点数の最大
#define MAX_FIELDIDX						(512)				// インデックスの最大

//*****************************************************************************
// フィールドの構造体定義
//*****************************************************************************
typedef struct Field
{
	LPDIRECT3DVERTEXBUFFER9 pVtxBuff;			// 頂点バッファへのポインタ
	LPDIRECT3DINDEXBUFFER9 pIdxBuff;			// インデックスバッファへのポインタ
	D3DXVECTOR3 pos;							// 位置
	D3DXVECTOR3 rot;							// 向き
	D3DXVECTOR2 tex;							// テクスチャの位置
	float fWidth;								// 幅
	float fHeigth;								// 高さ
	float fDepth;								// 奥行き
	WORD nIdx[MAX_FIELDIDX];					// インデックス
	int nNumVtx;								// 頂点数
	int nNumIdx;								// インデックス数
	bool bUse;									// 使用状態
	D3DXMATRIX mtxWorld;						// ワールドマトリックス
}Field;

typedef struct Field* LPFIELD;

//*****************************************************************************
// プロトタイプ宣言
//*****************************************************************************
void InitField(void);
void UninitField(void);
void UpdateField(void);
void DrawField(void);
void SetField(D3DXVECTOR3 pos, D3DXVECTOR3 rot, D3DXVECTOR3* pVtxPos, int nNumVtx, int* pIdxInfo, int nNumIdx);
void LoadField(const char* pFileName, D3DXVECTOR3 pos, D3DXVECTOR3 rot);

#endif // _FIELD_H_