//=============================================================================
//
//	イベント用オブジェクトの処理 [eventobject.h]
//	Author : SHUMA AIZU
// 
//=============================================================================

#include "eventobject.h"
#include "loadscript.h"

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

	// 初期化
	memset(pEventObject, NULL, sizeof(EventObject) * MAX_EVENTOBJECT);

	LoadEventObject("data\\SCRIPTS\\OBJECT\\Eventobject000.txt");
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
	// イベント用オブジェクトへのポインタ
	EventObject* pEventObject = &g_aEventObject[0];

	for (int nCntEventObject = 0; nCntEventObject < MAX_EVENTOBJECT; nCntEventObject++, pEventObject++)
	{
		if (pEventObject->bUse == false)
		{// 使っていなければ弾く
			continue;
		}

		if (pEventObject->ObjectInfo.ParentObject.motion.bFinishMotion == true && 
			pEventObject->isEvent == true && pEventObject->ObjectInfo.ParentObject.type == PARENTMODELTYPE_HOUSE)
		{
			pEventObject->fAlpha -= 0.01f;

			if (pEventObject->fAlpha < 0.0f)
			{
				pEventObject->bUse = false;
			}
		}

		if (pEventObject->ObjectType == EVENTOBJECTTYPE_PARENT && pEventObject->ObjectInfo.ParentObject.motion.pMotionData != NULL)
		{// オブジェクトが階層構造モデルかつモーションがあるならば
			// モーションを更新
			UpdateMotion(&pEventObject->ObjectInfo.ParentObject.motion,
				pEventObject->ObjectInfo.ParentObject.pModelData,
				&pEventObject->ObjectInfo.ParentObject.OffSetData);
		}
	}
}

//==============================================================================
// イベント用オブジェクトの描画処理
//==============================================================================
void DrawEventObject(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();	// デバイスの取得
	D3DXMATRIX mtxRot, mtxTrans;				// 計算用マトリックス
	D3DMATERIAL9 matDef;						// 現在のマテリアルを保存
	D3DXMATERIAL* pMat;							// マテリアルデータへのポインタ
	D3DXMATERIAL MatCpy;						// 書き換え用マテリアル

	// イベント用オブジェクトへのポインタ
	EventObject* pEventObject = &g_aEventObject[0];

	for (int nCntEventObject = 0; nCntEventObject < MAX_EVENTOBJECT; nCntEventObject++, pEventObject++)
	{
		if (pEventObject->bUse == false)
		{// 使っていなければ弾く
			continue;
		}

		if (pEventObject->ObjectType == EVENTOBJECTTYPE_NORMAL)
		{// 通常オブジェクト
			// ワールドマトリックスの初期化(デフォルトの値にする)
			D3DXMatrixIdentity(&pEventObject->mtxWorld);

			// 向きを反映
			D3DXMatrixRotationYawPitchRoll(&mtxRot, pEventObject->rot.y, pEventObject->rot.x, pEventObject->rot.z);
			D3DXMatrixMultiply(&pEventObject->mtxWorld, &pEventObject->mtxWorld, &mtxRot);

			// 位置を反映
			D3DXMatrixTranslation(&mtxTrans, pEventObject->pos.x, pEventObject->pos.y, pEventObject->pos.z);
			D3DXMatrixMultiply(&pEventObject->mtxWorld, &pEventObject->mtxWorld, &mtxTrans);

			// ワールドマトリックスの設定
			pDevice->SetTransform(D3DTS_WORLD, &pEventObject->mtxWorld);

			// 現在のマテリアルを取得
			pDevice->GetMaterial(&matDef);

			// マテリアルデータへのポインタを取得
			pMat = (D3DXMATERIAL*)pEventObject->ObjectInfo.NormalObject.pModelData->pBuffMat->GetBufferPointer();

			for (int nCntMat = 0; nCntMat < (int)pEventObject->ObjectInfo.NormalObject.pModelData->dwNumMat; nCntMat++)
			{
				// マテリアルの設定
				pDevice->SetMaterial(&pMat[nCntMat].MatD3D);

				pDevice->SetTexture(0, pEventObject->ObjectInfo.NormalObject.pModelData->apTexture[nCntMat]);

				// オブジェクトパーツの描画
				pEventObject->ObjectInfo.NormalObject.pModelData->pMesh->DrawSubset(nCntMat);	// ここでモデルの形を指定しているため、g_aObjectModelの中身を設定する必要がある
			}

			// 保存していたマテリアルに戻す
			pDevice->SetMaterial(&matDef);

		}
		else if (pEventObject->ObjectType == EVENTOBJECTTYPE_PARENT)
		{// 階層構造オブジェクト
			// ワールドマトリックスの初期化
			D3DXMatrixIdentity(&pEventObject->mtxWorld);

			// 向きを反映
			D3DXMatrixRotationYawPitchRoll(&mtxRot, pEventObject->rot.y, pEventObject->rot.x, pEventObject->rot.z);
			D3DXMatrixMultiply(&pEventObject->mtxWorld, &pEventObject->mtxWorld, &mtxRot);

			// 位置を反映
			D3DXMatrixTranslation(&mtxTrans, pEventObject->pos.x, pEventObject->pos.y, pEventObject->pos.z);
			D3DXMatrixMultiply(&pEventObject->mtxWorld, &pEventObject->mtxWorld, &mtxTrans);

			// ワールドマトリックスの設定
			pDevice->SetTransform(D3DTS_WORLD, &pEventObject->mtxWorld);

			// 現在のマテリアルを取得
			pDevice->GetMaterial(&matDef);

			// 全モデル(パーツ)の描画
			for (int nCntParentModel = 0; nCntParentModel < pEventObject->ObjectInfo.ParentObject.pModelData->nNumParts; nCntParentModel++)
			{
				D3DXMATRIX mtxRotOffSetModel, mtxTransOffSetModel;	// 計算用マトリックス
				D3DXMATRIX mtxParent;								// 親のマトリックス

				// パーツのワールドマトリックスを初期化
				D3DXMatrixIdentity(&pEventObject->ObjectInfo.ParentObject.pModelData->aModel[nCntParentModel].mtxWorld);

				// パーツの向きを反映
				D3DXMatrixRotationYawPitchRoll(&mtxRotOffSetModel,
					pEventObject->ObjectInfo.ParentObject.OffSetData.rot[nCntParentModel].y, 
					pEventObject->ObjectInfo.ParentObject.OffSetData.rot[nCntParentModel].x, 
					pEventObject->ObjectInfo.ParentObject.OffSetData.rot[nCntParentModel].z);

				D3DXMatrixMultiply(&pEventObject->ObjectInfo.ParentObject.pModelData->aModel[nCntParentModel].mtxWorld, 
					&pEventObject->ObjectInfo.ParentObject.pModelData->aModel[nCntParentModel].mtxWorld, 
					&mtxRotOffSetModel);

				// パーツの位置を反映(オフセット)
				D3DXMatrixTranslation(&mtxTransOffSetModel, 
					pEventObject->ObjectInfo.ParentObject.OffSetData.pos[nCntParentModel].x,
					pEventObject->ObjectInfo.ParentObject.OffSetData.pos[nCntParentModel].y, 
					pEventObject->ObjectInfo.ParentObject.OffSetData.pos[nCntParentModel].z);

				D3DXMatrixMultiply(&pEventObject->ObjectInfo.ParentObject.pModelData->aModel[nCntParentModel].mtxWorld, 
					&pEventObject->ObjectInfo.ParentObject.pModelData->aModel[nCntParentModel].mtxWorld, 
					&mtxTransOffSetModel);

				// パーツの「親のマトリックス」を設定
				if (pEventObject->ObjectInfo.ParentObject.pModelData->aModel[nCntParentModel].nIdxModelParent != -1)
				{// 親モデルがある場合
					mtxParent = pEventObject->ObjectInfo.ParentObject.pModelData->aModel[pEventObject->ObjectInfo.ParentObject.pModelData->aModel[nCntParentModel].nIdxModelParent].mtxWorld;
				}
				else
				{// 親モデルがない場合
					mtxParent = pEventObject->mtxWorld;
				}

				// 算出した「パーツのワールドマトリックス」と「親のマトリックス」を掛け合わせる
				D3DXMatrixMultiply(&pEventObject->ObjectInfo.ParentObject.pModelData->aModel[nCntParentModel].mtxWorld,
					&pEventObject->ObjectInfo.ParentObject.pModelData->aModel[nCntParentModel].mtxWorld,
					&mtxParent);

				// パーツのワールドマトリックスを設定
				pDevice->SetTransform(D3DTS_WORLD, &pEventObject->ObjectInfo.ParentObject.pModelData->aModel[nCntParentModel].mtxWorld);

				// マテリアルデータへのポインタを取得
				pMat = (D3DXMATERIAL*)pEventObject->ObjectInfo.ParentObject.pModelData->aModel[pEventObject->ObjectInfo.ParentObject.pModelData->aModel[nCntParentModel].nIdxModel].pBuffMat->GetBufferPointer();


				for (int nCntMat = 0; nCntMat < (int)pEventObject->ObjectInfo.ParentObject.pModelData->aModel[pEventObject->ObjectInfo.ParentObject.pModelData->aModel[nCntParentModel].nIdxModel].dwNumMat; nCntMat++)
				{
					MatCpy = pMat[nCntMat];		// 今のマテリアルをコピー

					MatCpy.MatD3D.Diffuse.a = pEventObject->fAlpha;	// アルファ値を適用

					// マテリアルの設定
					pDevice->SetMaterial(&MatCpy.MatD3D);

					// テクスチャの設定
					pDevice->SetTexture(0, pEventObject->ObjectInfo.ParentObject.pModelData->aModel[pEventObject->ObjectInfo.ParentObject.pModelData->aModel[nCntParentModel].nIdxModel].apTexture[nCntMat]);

					// パーツの描画
					pEventObject->ObjectInfo.ParentObject.pModelData->aModel[pEventObject->ObjectInfo.ParentObject.pModelData->aModel[nCntParentModel].nIdxModel].pMesh->DrawSubset(nCntMat);
				}
			}

			// 保存していたマテリアルを戻す
			pDevice->SetMaterial(&matDef);
		}
	}
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
		pEventObject->ObjectInfo.NormalObject.type = type;
		pEventObject->ObjectInfo.NormalObject.pModelData = GetObjectModel(type);
		pEventObject->pos = pos;
		pEventObject->rot = rot;
		pEventObject->fAlpha = 1.0f;
		pEventObject->bUse = true;

		// 当たり判定
		if (isCollider == true)
		{// コライダーを使っているなら
			// 当たり判定を設定
			pEventObject->nCollisionIdx = SetCollision(pos, rot);

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
		pEventObject->fAlpha = 1.0f;						// アルファ値
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
			pEventObject->nCollisionIdx = SetCollision(pos, rot);

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

//==============================================================================
// イベント用オブジェクトの取得処理
//==============================================================================
EventObject* GetEventObject(int nIdx)
{
	return &g_aEventObject[nIdx];
}