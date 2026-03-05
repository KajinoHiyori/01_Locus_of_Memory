//=============================================================================
//
//	イベント用オブジェクトの処理 [eventobject.h]
//	Author : SHUMA AIZU
// 
//=============================================================================

#include "eventobject.h"

//*****************************************************************************
// マクロ定義
//*****************************************************************************
#define MAX_EVENTOBJECT		(256)		// 置けるイベント用オブジェクトの最大

//*****************************************************************************
// グローバル変数
//*****************************************************************************
EventObject g_aEventObject[MAX_EVENTOBJECT] = {};	// イベント用オブジェクト情報

//=============================================================================
// イベント用オブジェクトの初期化処理
//=============================================================================
void InitEventObject(void)
{
	// イベント用オブジェクトへのポインタ
	EventObject* pEventObject = &g_aEventObject[0];

	// ただ単純な初期化
	memset(pEventObject, NULL, sizeof(EventObject) * MAX_EVENTOBJECT);
}

//=============================================================================
// イベント用オブジェクトの終了処理
//=============================================================================
void UninitEventObject(void)
{

}

//=============================================================================
// イベント用オブジェクトの更新処理
//=============================================================================
void UpdateEventObject(void)
{

}

//==============================================================================
// イベント用オブジェクトの描画処理
//==============================================================================
void DrawEventObject(void)
{

}

//==============================================================================
// イベント用オブジェクトの設定処理
//==============================================================================
void SetEventObjectNormal(D3DXVECTOR3 pos, D3DXVECTOR3 rot, OBJECTTYPE type, ColliderInfo* pColliderInfo, int nNumCollider, bool isCollision, bool isCollider)
{
	// イベントオブジェクトへのポインタ
	EventObject* pEventObject = &g_aEventObject[0];

	for (int nCntObject = 0; nCntObject < MAX_EVENTOBJECT; nCntObject++, pEventObject++)
	{
		if (pEventObject->bUse == true)
		{// 使っていたら弾く
			continue;
		}

		// 弧度法への変換
		rot.x = DegreeToRadian(rot.x);
		rot.y = DegreeToRadian(rot.y);
		rot.z = DegreeToRadian(rot.z);

		// 各種変数設定
		pEventObject->pos = pos;
		pEventObject->rot = rot;
		pEventObject->bUse = true;

#if 1
		// 当たり判定
		if (isCollider == true)
		{// コライダーを使っているなら
			// 当たり判定を設定
			pEventObject->nCollisionIdx = SetCollision();

			pColliderInfo->Collidertype.pos = pos;

			// 使うコライダーの分だけ
			for (int nCntCollider = 0; nCntCollider < nNumCollider; nCntCollider++, pColliderInfo++)
			{
				// コライダー情報をポインタから設定する
				SetCollider(pEventObject->nCollisionIdx, *pColliderInfo);
			}
		}
		else
		{
			pEventObject->nCollisionIdx = -1;
		}
#endif
		break;
	}
}

//==============================================================================
// イベント用オブジェクトの設定処理
//==============================================================================
void SetEventObjectParent(D3DXVECTOR3 pos, D3DXVECTOR3 rot, PARENTMODELTYPE parentmodeltype, MOTIONDATATYPE motiondatatype, ColliderInfo* pColliderInfo, int nNumCollider, bool isCollision, bool isCollider)
{
	// イベントオブジェクトへのポインタ
	EventObject* pEventObject = &g_aEventObject[0];

	for (int nCntParentObject = 0; nCntParentObject < MAX_EVENTOBJECT; nCntParentObject++, pEventObject++)
	{
		if (pEventObject->bUse == true)
		{// 使っていたら弾く
			continue;
		}

		// 弧度法への変換
		rot.x = DegreeToRadian(rot.x);
		rot.y = DegreeToRadian(rot.y);
		rot.z = DegreeToRadian(rot.z);

		// 階層構造モデル情報を設定
		pEventObject->ObjectInfo.ParentObject.pModelData = SetModelData(parentmodeltype);					// モデルデータ設定
		pEventObject->ObjectInfo.ParentObject.type = parentmodeltype;

		if (motiondatatype == MOTIONDATATYPE_MAX)
		{
			
		}
		else
		{
			// モーションデータのポインタを取得、保持
			pEventObject->ObjectInfo.ParentObject.motion.pMotionData = SetMotionData(motiondatatype);	// モーションデータ設定

			// ニュートラルモーションで開始
			SetMotion(&pEventObject->ObjectInfo.ParentObject.motion,
				pEventObject->ObjectInfo.ParentObject.pModelData,
				&pEventObject->ObjectInfo.ParentObject.OffSetData,
				MOTIONTYPE_NEUTRAL, true, false, 10);
		}

		// 各種設定
		pEventObject->pos = pos;							// 位置
		pEventObject->rot = rot;							// 向き
		pEventObject->ObjectType = EVENTOBJECTTYPE_PARENT;	// 通常か階層構造か
		pEventObject->bUse = true;							// 使用状態に

		// オフセット情報を格納するメンバ変数へのポインタを取得
		Model* pModel = &pEventObject->ObjectInfo.ParentObject.pModelData->aModel[0];		// モデル情報
		D3DXVECTOR3* pOffSetPos = &pEventObject->ObjectInfo.ParentObject.OffSetData.pos[0];	// オブジェクトのオフセット座標
		D3DXVECTOR3* pOffSetRot = &pEventObject->ObjectInfo.ParentObject.OffSetData.rot[0];	// オブジェクトのオフセット向き

		for (int nCntModel = 0; nCntModel < pEventObject->ObjectInfo.ParentObject.pModelData->nNumParts; nCntModel++, pOffSetPos++, pOffSetRot++, pModel++)
		{// ローカル座標を設定
			*pOffSetPos += pModel->posLocal;
			*pOffSetRot += pModel->rotLocal;
		}

		// 当たり判定
		if (isCollider == true)
		{// コライダーを使っているなら
			// 当たり判定を設定
			pEventObject->nCollisionIdx = SetCollision();

			// 使っているコライダーの分だけ
			for (int nCntCollider = 0; nCntCollider < nNumCollider; nCntCollider++, pColliderInfo++)
			{
				// コライダー情報をポインタから設定する
				SetCollider(pEventObject->nCollisionIdx, *pColliderInfo);
			}
		}
		else
		{// 使っていなければインデックスを無効に
			pEventObject->nCollisionIdx = -1;
		}

		break;
	}
}