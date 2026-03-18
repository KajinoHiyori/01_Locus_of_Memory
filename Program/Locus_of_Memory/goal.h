//=============================================================================
//
//	ゴール地点の処理 [goal.h]
//	Author : SHUMA AIZU
// 
//=============================================================================

#ifndef _GOAL_H_
#define _GOAL_H_

#include "main.h"
#include "object.h"
#include "motion.h"

//*****************************************************************************
// マクロ定義
//*****************************************************************************
#define GOAL_POS	(D3DXVECTOR3(1000.0f, 45.0f, 2800.0f))	// ゴールの位置

//*****************************************************************************
// ゴールオブジェクト (本) の構造体定義
//*****************************************************************************
typedef struct Book
{
	D3DXMATRIX		mtxWorld;	// ワールドマトリックス
	D3DXVECTOR3		pos;		// オブジェクトの位置
	D3DXVECTOR3		rot;		// オブジェクトの向き
	Motion			motion;		// モーション情報
	ModelData*		pModelData;	// モデルの情報
	OffSetData		OffSetData;	// モーション中の階層構造情報
	float fRadius;				// 半径
}Book;

//*****************************************************************************
// プロトタイプ宣言
//*****************************************************************************
void InitGoal(void);
void UninitGoal(void);
void UpdateGoal(void);
void DrawGoal(void);
void SetGoal(D3DXVECTOR3 pos, float fRadius);
void CollisionGoal(void);
bool GetGoalState(void);
void ResetGoalState(void);

#endif // !_GOAL_H_