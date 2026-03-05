//=============================================================================
//
//	イベント用オブジェクトの処理 [eventobject.h]
//	Author : SHUMA AIZU
// 
//=============================================================================

#ifndef _EVECTOBJECT_H_
#define _EVECTOBJECT_H_

#include "main.h"
#include "object.h"
#include "motion.h"

//*****************************************************************************
// マクロ定義
//*****************************************************************************

//*****************************************************************************
// イベント用オブジェクトの構造体定義
//*****************************************************************************
typedef struct EventObject
{
	D3DXMATRIX		mtxWorld;		// ワールドマトリックス
	D3DXVECTOR3		pos;			// オブジェクトの位置
	D3DXVECTOR3		rot;			// オブジェクトの向き
	PARENTMODELTYPE	type;			// 階層構造オブジェクトの種類
	EVENTTYPE		EventType;		// イベントの種類
	Motion			motion;			// モーション情報
	ModelData*		pModelData;		// モデルの情報
	OffSetData		OffSetData;		// モーション中の階層構造情報
	int				nEventIdx;		// イベントのインデックス
	int				nCollisionIdx;	// 当たり判定のインデックス
	bool			bUse;			// 使用状態
}EventObject;

//*****************************************************************************
// プロトタイプ宣言
//*****************************************************************************
void InitEventObject(void);
void UninitEventObject(void);
void UpdateEventObject(void);
void DrawEventObject(void);
void SetEventObject(D3DXVECTOR3 pos, float fRadius);

#endif // !_GOAL_H_