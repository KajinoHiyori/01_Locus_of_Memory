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
// 使用するオブジェクトの種類
//*****************************************************************************
typedef enum EVENTOBJECTTYPE
{
	EVENTOBJECTTYPE_NONE = -1,// 何もない
	EVENTOBJECTTYPE_NORMAL,		// 通常
	EVENTOBJECTTYPE_PARENT,		// 階層構造
	EVENTOBJECTTYPE_MAX
}EVENTOBJECTTYPE;

//*****************************************************************************
// イベント用通常オブジェクトの構造体定義
//*****************************************************************************
typedef struct EventObject_Normal
{
	OBJECTTYPE type;			// モデルの種類
	ObjectModel* pModelData;	// モデルの情報
}EventObject_Normal;

//*****************************************************************************
// イベント用階層構造オブジェクトの構造体定義
//*****************************************************************************
typedef struct EventObject_Parent
{
	PARENTMODELTYPE	type;		// 階層構造オブジェクトの種類
	Motion motion;				// モーション情報
	ModelData* pModelData;		// モデルの情報
	OffSetData OffSetData;		// モーション中の階層構造情報
}EventObject_Parent;

//*****************************************************************************
// イベント用オブジェクトの共用体定義
//*****************************************************************************
union EventObjectInfo
{
	EventObject_Normal NormalObject;	// 通常モデル
	EventObject_Parent ParentObject;	// 階層構造モデル
};

//*****************************************************************************
// イベント用オブジェクトの構造体定義
//*****************************************************************************
typedef struct EventObject
{
	D3DXMATRIX mtxWorld;		// ワールドマトリックス
	D3DXVECTOR3 pos;			// オブジェクトの位置
	D3DXVECTOR3 rot;			// オブジェクトの向き
	EventObjectInfo ObjectInfo;	// オブジェクト情報
	EVENTOBJECTTYPE ObjectType;	// オブジェクトの種類
	EVENTTYPE EventType;		// イベントの種類
	float fAlpha;				// アルファ値
	int nCollisionIdx;			// 当たり判定のインデックス
	bool isEvent;				// イベント中か
	bool bUse;					// 使用状態
}EventObject;

//*****************************************************************************
// プロトタイプ宣言
//*****************************************************************************
void InitEventObject(void);
void UninitEventObject(void);
void UpdateEventObject(void);
void DrawEventObject(void);
void SetEventObjectNormal(D3DXVECTOR3 pos, D3DXVECTOR3 rot, OBJECTTYPE type, ColliderInfo* pColliderInfo, int nNumCollider, bool isCollision, bool isCollider);
void SetEventObjectParent(D3DXVECTOR3 pos, D3DXVECTOR3 rot, PARENTMODELTYPE parentmodeltype, MOTIONDATATYPE motiondatatype, ColliderInfo* pColliderInfo, int nNumCollider, bool isCollision, bool isCollider);
EventObject* GetEventObject(int nIdx);

#endif // !_EVENTOBJECT_H_