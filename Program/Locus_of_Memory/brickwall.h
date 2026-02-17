//===================================================================
//
// 外壁処理[brickwall.h]
// Author : KajinoHiyori
// 
//===================================================================
#ifndef _BRICKWALL_H_
#define	_BRICKWALL_H_

// 空の構造体
typedef struct
{
	LPDIRECT3DVERTEXBUFFER9 pVtxBuff;	// 頂点バッファへのポインタ
	LPDIRECT3DINDEXBUFFER9 pIdxBuff;	// インデックスバッファへのポインタ
	D3DXMATRIX mtxWorld;				// ワールドマトリックス
	D3DXVECTOR3 pos;					// メッシュフィールドの位置
	D3DXVECTOR3 rot;					// メッシュフィールドの向き
	bool bUse;							// 使用状態
}BrickWall;

// プロトタイプ宣言
void InitBrickWall(void);
void UninitBrickWall(void);
void UpdateBrickWall(void);
void DrawBrickWall(void);
void SetBrickWall(D3DXVECTOR3 pos, D3DXVECTOR3 rot);
#endif
