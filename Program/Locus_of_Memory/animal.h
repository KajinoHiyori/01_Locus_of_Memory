//==========================================================================
//
//	魔法動物の処理[animal.h]
//	Author:OBIKA SOUMA
//
//==========================================================================

#ifndef _ANIMAL_H_
#define _ANIMAL_H_

#include "main.h"

//=====================================
//	マクロ定義
//=====================================
#define MAX_ANIMAL (128)			// 動物の最大数
#define MAX_ANIMALMODEL (4)			// モデルの種類の最大数

//=====================================
//	構造体
//=====================================
// 動物の構造体
typedef struct
{
	D3DXVECTOR3 pos;			// 位置
	D3DXVECTOR3 rot;			// 向き
	D3DXMATRIX	mtxWorld;		// ワールドマトリックス
	int nType;					// モデルの種類
	bool bUse;					// 使用状態
}Animal;

// モデルの構造体
typedef struct
{
	LPDIRECT3DTEXTURE9 apTexture;
	LPD3DXMESH pMesh;
	LPD3DXBUFFER pBuffMat;
	DWORD dwNumMat;
	D3DXVECTOR3 vtxMin, vtxMax;
}AnimalModel;

//=====================================
//	プロトタイプ宣言
//=====================================

void InitAnimal(void);
//void UninitAnimal(void);
//void UpdateAnimal(void);
//void DrawAnimal(void);
//void SetAnimal(D3DXVECTOR3 pos, D3DXVECTOR3 rot, int nType);

#endif