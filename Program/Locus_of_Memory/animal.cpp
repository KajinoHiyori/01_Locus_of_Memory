//========================================================================
//
//	魔法動物の処理[animal.cpp]
//	Author:OBIKA SOUMA
//
//========================================================================
#include "animal.h"
#include "motion.h"
#include "input.h"

// マクロ定義
#define MAX_DRAGON	(10)	// ドラゴンの最大数
#define FLY_POS		(D3DXVECTOR3(0.0f, 3000.0f, 3500.0f))		// ドラゴンの位置[飛竜]
#define MOVERADIUS	(3000.0f)									// ドラゴンの移動周期[飛竜]
#define FIRE_POS	(D3DXVECTOR3(2476.0f, 349.0f, -4812.0f))	// ドラゴンの位置[火竜]
#define FIRE_ROT	(D3DXVECTOR3(0.0f, 2.60f, 0.0f))			// ドラゴンの角度[火竜]

// ドラゴンの構造体
typedef struct
{
	DRAGONTYPE type;	// ドラゴンの種類
	int nIdx;			// 階層構造オブジェクトのインデックス
	bool bUse;			// ドラゴンの使用状態
}Dragon;

// グローバル変数
Dragon g_aDragon[MAX_DRAGON];
int g_nCounterDragon;

//===============================
//	動物の初期化処理
//===============================
void InitAnimal(void)
{
	g_nCounterDragon = 0;	// ドラゴンの総数をカウント

	for (int nCntDragon = 0; nCntDragon < MAX_DRAGON; nCntDragon++)
	{
		g_aDragon[nCntDragon].type = DRAGONTYPE_NONE;
		g_aDragon[nCntDragon].nIdx = -1;
		g_aDragon[nCntDragon].bUse = false;
	}
}

//===============================
//	動物の終了処理
//===============================
void UninitAnimal(void)
{

}

//===============================
//	動物の更新処理
//===============================
void UpdateAnimal(void)
{
	MODE mode = GetMode();

	for (int nCntDragon = 0; nCntDragon < MAX_DRAGON; nCntDragon++)
	{
		if (g_aDragon[nCntDragon].bUse == false)
		{
			continue;
		}
		
		switch (g_aDragon[nCntDragon].type)
		{
		case DRAGONTYPE_FLYING:	// 飛竜
			UpdateFlyDragon(nCntDragon);
			break;

		case DRAGONTYPE_FIRE:	// 火竜
			UpdateFireDragon(nCntDragon);
			break;
		}
	}

	for (int nCntObj = 0; nCntObj < MAX_PARENTOBJECT; nCntObj++)
	{
		//EventObject* pParentObject = GetParentObjectInfo(nCntObj);
		//if (pParentObject->type == PARENTMODELTYPE_DRAGON && pParentObject->motion.bFinishMotion == true)
		//{
		//	SetMotion(&pParentObject->motion, pParentObject->pModelData, &pParentObject->OffSetData, MOTIONTYPE_NEUTRAL, true, true, 10);
		//	break;
		//}
	}
}

//===============================
//	動物の描画処理
//===============================
void DrawAnimal(void)
{

}

//===============================
//	ドラゴンのインデックスを取得
//===============================
void SetDragon(int nIdx)
{
	g_aDragon[g_nCounterDragon].bUse = true;
	g_aDragon[g_nCounterDragon].nIdx = nIdx;
	g_nCounterDragon++;
}

//===============================
//	ドラゴンを非表示
//===============================
void ResetDragon(int nIdx)
{
	ParentObject* pParentObject = GetParentObjectInfo(g_aDragon[nIdx].nIdx);

	if (pParentObject->bUse == true)
	{
		g_aDragon[g_nCounterDragon].bUse = false;
		pParentObject->bUse = false;
	}
}

//===============================
//	ドラゴンのタイプを設定
//===============================
void SetDragonType(int nCntDragon, DRAGONTYPE type)
{
	ParentObject* pParentObject = GetParentObjectInfo(g_aDragon[nCntDragon].nIdx);
	g_aDragon[nCntDragon].type = type;
	pParentObject->bUse = true;
	g_aDragon[nCntDragon].bUse = true;
	if (g_aDragon[nCntDragon].type == DRAGONTYPE_FLYING)
	{
		pParentObject->pos = FLY_POS;
	}
	else
	{
		pParentObject->pos = FIRE_POS;
		pParentObject->rot = FIRE_ROT;
		SetMotion(&pParentObject->motion, pParentObject->pModelData, &pParentObject->OffSetData, MOTIONTYPE_NEUTRAL, true, false, 10);
	}
}

//===============================
//	飛竜の更新
//===============================
void UpdateFlyDragon(int nCntDragon)
{
	ParentObject* pParentObject = GetParentObjectInfo(g_aDragon[nCntDragon].nIdx);

	static float fAngle = 0.0f;
	D3DXVECTOR3 pos = FLY_POS;
	D3DXVECTOR3 posDest;

	fAngle += 0.005f;

	fAngle = AngleNormalize(fAngle);

	posDest.x = pos.x + sinf(fAngle) * MOVERADIUS;
	posDest.y = pos.y;
	posDest.z = pos.z + cosf(fAngle) * MOVERADIUS;

	pParentObject->rot.y = atan2f(pParentObject->pos.x - posDest.x, pParentObject->pos.z - posDest.z);

	pParentObject->rot.y = AngleNormalize(pParentObject->rot.y);

	pParentObject->pos = posDest;
}

//===============================
//	火竜の更新
//===============================
void UpdateFireDragon(int nCntDragon)
{
	ParentObject* pParentObject = GetParentObjectInfo(g_aDragon[nCntDragon].nIdx);
	if (pParentObject->motion.motionType == MOTIONTYPE_MOVE && pParentObject->motion.nKey == 10)
	{
		SetMotion(&pParentObject->motion, pParentObject->pModelData, &pParentObject->OffSetData, MOTIONTYPE_NEUTRAL, true, true, 10);
	}
}
