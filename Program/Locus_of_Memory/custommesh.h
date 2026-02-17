//=============================================================================
//
//	カスタムメッシュ処理 [custommesh.h]
//	Author : SHUMA AIZU
// 
//=============================================================================

#ifndef _CUSTOMMESH_H_
#define _CUSTOMMESH_H_

#include "main.h"
#include "meshfield.h"

//*****************************************************************************
// マクロ定義
//*****************************************************************************
#define MAX_VTX						(1000)				// 頂点数の最大
#define MAX_IDX						(512)				// インデックスの最大

//*****************************************************************************
// カスタムメッシュの構造体定義
//*****************************************************************************
typedef struct CustomMesh
{
	LPDIRECT3DVERTEXBUFFER9 pVtxBuff;			// 頂点バッファへのポインタ
	LPDIRECT3DINDEXBUFFER9 pIdxBuff;			// インデックスバッファへのポインタ
	D3DXVECTOR3 pos;							// 位置
	D3DXVECTOR3 rot;							// 向き
	D3DXVECTOR2 tex;							// テクスチャの位置
	MESHFIELDTYPE type;							// 種類
	float fWidth;								// 幅
	float fHeigth;								// 高さ
	float fDepth;								// 奥行き
	WORD nIdx[MAX_IDX];							// インデックス
	int nNumVtx;								// 頂点数
	int nNumIdx;								// インデックス数
	bool bUse;									// 使用状態
	D3DXMATRIX mtxWorld;						// ワールドマトリックス
}CustomMesh;

typedef struct CustomMesh* LPCUSTOMMESH;

//*****************************************************************************
// プロトタイプ宣言
//*****************************************************************************
void InitCustomMesh(void);
void UninitCustomMesh(void);
void UpdateCustomMesh(void);
void DrawCustomMesh(void);
void SetCustomMesh(D3DXVECTOR3 pos, D3DXVECTOR3 rot, D3DXVECTOR3* pVtxPos, int nNumVtx, int* pIdxInfo, int nNumIdx);
void LoadCustomMesh(const char* pFileName, D3DXVECTOR3 pos, D3DXVECTOR3 rot);

#endif // _CUSTOMMESH_H_