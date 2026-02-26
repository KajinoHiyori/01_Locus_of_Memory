//=============================================================================
//
//	鍵取得処理 [key.h]
//	Author : Kajino Hiyori
// 
//=============================================================================
#ifndef _KEY_H_
#define	_KEY_H_
#include "main.h"

// マクロ定義
#define MAX_KEYMAT			(16)	// マテリアルの最大数
#define MAX_KEYITEM		(3)		// カギの最大数

// モデルの種類
typedef struct
{
	LPD3DXMESH			pMesh;					// メッシュ(頂点情報)へのポインタ
	LPD3DXBUFFER		pBuffMat;				// マテリアルへのポインタ
	DWORD				dwNumMat;				// マテリアルの数
	LPDIRECT3DTEXTURE9	apTexture[MAX_KEYMAT];		// テクスチャへの	ポインタ
}KeyModel;

// モデルの数
typedef enum
{
	KEYTYPE_ONE,	// カギの種類
	KEYTYPE_TWO,	// カギの種類
	KEYTYPE_THREE,	// カギの種類
	KEYTYPE_MAX
}KEYTYPE;

// 鍵の状態
typedef enum
{
	KEYSTATE_NORMAL = 0,	// 通常[フィールド上に配置]
	KEYSTATE_GETTING,		// 取得されている
	KEYSTATE_GOTTEN,		// 取得された
	KEYSTATE_MAX
}KEYSTATE;

// モデルの構造体
typedef struct
{
	D3DXMATRIX		mtxWorld;	// ワールドマトリックス
	D3DXVECTOR3		pos;		// カギの位置
	D3DXVECTOR3		posOld;		// カギの位置
	D3DXVECTOR3		rot;		// カギの向き
	D3DXVECTOR3		move;		// カギの移動量
	KEYTYPE			type;		// モデルの種類
	KEYSTATE		state;		// 鍵の状態
	float			fCollision;	// 当たり判定の半径
	float			fScale;		// 大きさ管理
	int				nIdxShadow;	// 影のインデックス
	bool			bUse;		// 使用状態
}KeyItem;

// プロトタイプ宣言
void InitKey(void);
void UninitKey(void);
void UpdateKey(void);
void DrawKey(void);
void SetKey(D3DXVECTOR3 pos, D3DXVECTOR3 rot, KEYTYPE type);
void CollisionKey(int nIdx, D3DXVECTOR3* pPos, D3DXVECTOR3* pPosOld, D3DXVECTOR3* pMove);
KeyItem* GetKeyItem(void);
#endif