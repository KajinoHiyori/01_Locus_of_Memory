//========================================================
// 
// 3Dアクションゲーム_MASTER KEY[key.h]
// 
//========================================================
#if 0
#ifndef _KEY_H_
#define	_KEY_H_
#include "main.h"

// マクロ定義
#define MAX_MAT			(16)	// マテリアルの最大数
#define MAX_KEYITEM		(3)		// カギの最大数

// モデルの種類
typedef struct
{
	LPD3DXMESH			pMesh;					// メッシュ(頂点情報)へのポインタ
	LPD3DXBUFFER		pBuffMat;				// マテリアルへのポインタ
	DWORD				dwNumMat;				// マテリアルの数
	LPDIRECT3DTEXTURE9	apTexture[MAX_MAT];		// テクスチャへの	ポインタ
	D3DXVECTOR3			vtxMin;					// カギの大きさ(最小)
	D3DXVECTOR3			vtxMax;					// カギの大きさ(最大)
}KeyModel;

// モデルの数
typedef enum
{
	KEYTYPE_RED,	// カギの種類[赤]
	KEYTYPE_GREEN,	// カギの種類[緑]
	KEYTYPE_CYAN,	// カギの種類[シアン]
	KEYTYPE_MAX
}KEYTYPE;

// モデルの構造体
typedef struct
{
	D3DXMATRIX		mtxWorld;	// ワールドマトリックス
	D3DXVECTOR3		pos;		// カギの位置
	D3DXVECTOR3		posOld;		// カギの位置
	D3DXVECTOR3		rot;		// カギの向き
	D3DXVECTOR3		move;		// カギの移動量
	KEYTYPE			type;		// モデルの種類
	int				nIdxShadow;	// 影のインデックス
	bool			bUse;		// 使用状態
	bool			bUseOld;	// 過去の使用状態
}KeyItem;

// プロトタイプ宣言
void InitKey(void);
void UninitKey(void);
void UpdateKey(void);
void DrawKey(void);
void SetKey(D3DXVECTOR3 pos, D3DXVECTOR3 rot, KEYTYPE type);
void CollisionKey(D3DXVECTOR3* pPos, D3DXVECTOR3* pPosOld, D3DXVECTOR3* pMove, D3DXVECTOR3 vtxMin, D3DXVECTOR3 vtxMax);
KeyItem* GetKeyItem(void);
#endif

#endif