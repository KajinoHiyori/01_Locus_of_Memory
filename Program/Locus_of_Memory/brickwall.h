//==========================================
// 
// 3Dポリゴンの描画[meshsky.h]
// 
//==========================================
#ifndef _MESHSKY_H_
#define	_MESHSKY_H_
#include "main.h"

// 空の構造体
typedef struct
{
	LPDIRECT3DVERTEXBUFFER9 pVtxBuff;	// 頂点バッファへのポインタ
	LPDIRECT3DINDEXBUFFER9 pIdxBuff;	// インデックスバッファへのポインタ
	D3DXMATRIX mtxWorld;				// ワールドマトリックス
	D3DXVECTOR3 pos;					// メッシュフィールドの位置
	D3DXVECTOR3 rot;					// メッシュフィールドの向き
	bool bUse;							// 使用状態
}MeshSky;

// プロトタイプ宣言
void InitMeshSky(void);
void UninitMeshSky(void);
void UpdateMeshSky(void);
void DrawMeshSky(void);
void SetMeshSky(D3DXVECTOR3 pos, D3DXVECTOR3 rot);
D3DXVECTOR3 GetMeshSkyPos(void);
#endif
