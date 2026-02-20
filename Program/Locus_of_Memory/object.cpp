//======================================================================================
// 
// object処理[object.cpp]
// Author : Kajino Hiyori
//
//======================================================================================
#include "main.h"
#include "object.h"
#include "player.h"
#include "meshfield.h"
#include "shadow.h"
#include "player.h"
#include "loadscript.h"

#include "input.h"

// マクロ定義
#define NUM_OBJECT	(OBJECTTYPE_MAX)	// モデルの数
#define BLANK		(0.001f)		// 空白
#define DEFALT		(D3DXVECTOR3(0.0f, 0.0f, 0.0f))					// xyzが0.0fの場合
#define NORMAL		(D3DXVECTOR3(0.0f, 1.0f, 0.0f))					// 基本の法線
#define VTX_MIN		(D3DXVECTOR3(10000.0f, 10000.0f, 10000.0f))		// オブジェクトの大きさの初期化値(最小)
#define VTX_MAX		(D3DXVECTOR3(-10000.0f, -10000.0f, -10000.0f))	// オブジェクトの大きさの初期化値(最大)

// グローバル変数
ObjectModel g_aObjectModel[NUM_OBJECT];			// モデルの種類を管理
Object g_aObject[MAX_OBJECT];					// オブジェクトの情報を格納
ParentObject g_aParentObject[MAX_PARENTOBJECT];	// 階層構造オブジェクトの情報を格納
ModelData g_aModelData[MAX_PARENTMODEL];		// 階層構造を持ったモデルデータ

int g_nNumObjectModel;				// モデル数を管理
int g_nNumRandObj;					// ランダムオブジェクトの数
int g_nIdxRandObj[MAX_OBJECT];		// ランダムオブジェクトの番号

//======================================================================================
// オブジェクトの初期化処理
//======================================================================================
void InitObject(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();	// デバイスの取得
	g_nNumObjectModel = 0;
	g_nNumRandObj = 0;

	ModelData* pModelData = &g_aModelData[0];	// 先頭アドレス
	ParentObject* pParentObject = &g_aParentObject[0];
		
	memset(pModelData, NULL, sizeof(ModelData) * MAX_PARENTMODEL);			// モデルデータの初期化
	memset(pParentObject, NULL, sizeof(ParentObject) * MAX_PARENTOBJECT);	// 階層構造オブジェクト情報の初期化
	memset(&g_nIdxRandObj, -1, sizeof(int) * MAX_OBJECT);					// ランダムオブジェクトの番号初期化

	// Object情報の初期化
	for (int nCntObject = 0; nCntObject < MAX_OBJECT; nCntObject++)
	{
		g_aObject[nCntObject].pos = DEFALT;
		g_aObject[nCntObject].rot = DEFALT;
		g_aObject[nCntObject].type = OBJECTTYPE_HOUSE000;
		g_aObject[nCntObject].EventType = EVENTTYPE_NONE;
		g_aObject[nCntObject].fAlpha = 1.0f;
		g_aObject[nCntObject].nIdxShadow = -1;
		g_aObject[nCntObject].fSize = g_aObjectModel[g_aObject[nCntObject].type].vtxMax.x;
		g_aObject[nCntObject].bUse = false;
	}

	for (int nCntParentObject = 0; nCntParentObject < MAX_PARENTOBJECT; nCntParentObject++, pParentObject++)
	{
		pParentObject->EventType = EVENTTYPE_NONE;
		pParentObject->fAlpha = 1.0f;
	}

	// モデル情報の初期化
	for (int nCntModel = 0; nCntModel < NUM_OBJECT; nCntModel++)
	{
		g_aObjectModel[nCntModel].pMesh = NULL;
		g_aObjectModel[nCntModel].pBuffMat = NULL;
		g_aObjectModel[nCntModel].dwNumMat = 0;
		g_aObjectModel[nCntModel].vtxMax = VTX_MAX;
		g_aObjectModel[nCntModel].vtxMin = VTX_MIN;
	}
}

//======================================================================================
// オブジェクトの終了処理
//======================================================================================
void UninitObject(void)
{
	// メッシュの破棄
	for (int nCntModel = 0; nCntModel < NUM_OBJECT; nCntModel++)
	{
		if (g_aObjectModel[nCntModel].pMesh != NULL)
		{
			g_aObjectModel[nCntModel].pMesh->Release();
			g_aObjectModel[nCntModel].pMesh = NULL;
		}
	}

	for (int nCntModelData = 0; nCntModelData < MAX_PARENTMODEL; nCntModelData++)
	{
		for (int nCntModel = 0; nCntModel < MAX_PARTS; nCntModel++)
		{
			if (g_aModelData[nCntModelData].aModel[nCntModel].pMesh != NULL)
			{
				g_aModelData[nCntModelData].aModel[nCntModel].pMesh->Release();
				g_aModelData[nCntModelData].aModel[nCntModel].pMesh = NULL;
			}
		}
	}

	// テクスチャの破棄
	for (int nCntModel = 0; nCntModel < NUM_OBJECT; nCntModel++)
	{
		for (int nCntMat = 0; nCntMat < (int)g_aObjectModel[nCntModel].dwNumMat; nCntMat++)
		{
			if (g_aObjectModel[nCntModel].apTexture[nCntMat] != NULL)
			{
				g_aObjectModel[nCntModel].apTexture[nCntMat]->Release();
				g_aObjectModel[nCntModel].apTexture[nCntMat] = NULL;
			}
		}
	}

	for (int nCntModelData = 0; nCntModelData < MAX_PARENTMODEL; nCntModelData++)
	{
		for (int nCntModel = 0; nCntModel < MAX_PARTS; nCntModel++)
		{
			for (int nCntMat = 0; nCntMat < (int)g_aModelData[nCntModelData].aModel[nCntModel].dwNumMat; nCntMat++)
			{
				if (g_aModelData[nCntModelData].aModel[nCntModel].apTexture[nCntMat] != NULL)
				{
					g_aModelData[nCntModelData].aModel[nCntModel].apTexture[nCntMat]->Release();
					g_aModelData[nCntModelData].aModel[nCntModel].apTexture[nCntMat] = NULL;
				}
			}
		}
	}

	// マテリアルの破棄
	for (int nCntModel = 0; nCntModel < NUM_OBJECT; nCntModel++)
	{
		if (g_aObjectModel[nCntModel].pBuffMat != NULL)
		{
			g_aObjectModel[nCntModel].pBuffMat->Release();
			g_aObjectModel[nCntModel].pBuffMat = NULL;
		}
	}

	for (int nCntModelData = 0; nCntModelData < MAX_PARENTMODEL; nCntModelData++)
	{
		for (int nCntModel = 0; nCntModel < MAX_PARTS; nCntModel++)
		{
			if (g_aModelData[nCntModelData].aModel[nCntModel].pBuffMat != NULL)
			{
				g_aModelData[nCntModelData].aModel[nCntModel].pBuffMat->Release();
				g_aModelData[nCntModelData].aModel[nCntModel].pBuffMat = NULL;
			}
		}
	}
}

//======================================================================================
// オブジェクトの更新処理
//======================================================================================
void UpdateObject(void)
{
	Player* pPlayer = GetPlayer();

	ParentObject* pParentObject = &g_aParentObject[0];		// 先頭アドレス

	for (int nCntObject = 0; nCntObject < MAX_OBJECT; nCntObject++)
	{
		if (g_aObject[nCntObject].bUse == true)
		{
			for (int nCntPlayer = 0; nCntPlayer < MAX_PLAYER; nCntPlayer++, pPlayer++)
			{
				//if (CrossCollision(&pPlayer->pos, &pPlayer->posOld, g_aObjectModel[g_aObject[nCntObject].type].vtxMin, g_aObjectModel[g_aObject[nCntObject].type].vtxMax, true, true) == true)
				//{
				//
				//}
			}
			// 影の位置を更新
			SetPositionShadow(g_aObject[nCntObject].nIdxShadow, g_aObject[nCntObject].pos);
		}
	}

	// 階層構造オブジェクト
	for (int nCntParentObject = 0; nCntParentObject < MAX_PARENTOBJECT; nCntParentObject++, pParentObject++)
	{
		if (pParentObject->bUse == false)
		{// 使っていなかったら弾く
			continue;
		}

		if (pParentObject->type == PARENTMODELTYPE_DRAGON)
		{
			UpdateTitleDragon(pParentObject);
		}

#ifdef _DEBUG
		// モーションテスト
		if (GetKeyboardTrigger(DIK_5) == true)
		{
			SetMotion(&pParentObject->motion, pParentObject->pModelData, &pParentObject->OffSetData, MOTIONTYPE_MOVE, false, true, 10);
		}
#endif

		switch (pParentObject->EventType)
		{
		case EVENTTYPE_001_1:
			UpdateObjectEvent001(pParentObject);
			break;
		}

		// モーションの更新
		UpdateMotion(&pParentObject->motion, pParentObject->pModelData, &pParentObject->OffSetData);
	}
}

//======================================================================================
// オブジェクトの描画処理
//======================================================================================
void DrawObject(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();	// デバイスの取得
	D3DXMATRIX mtxRot, mtxTrans;	// 計算用マトリックス
	D3DMATERIAL9 matDef;	// 現在のマテリアルを保存
	D3DXMATERIAL* pMat;		// マテリアルデータへのポインタ
	D3DXMATERIAL MatCpy;	// 書き換え用マテリアル
	//Player* pPlayer = GetPlayer();
	MODE mode = GetMode();

	// 影の描画用変数
	D3DXMATRIX mtxShadow;		// シャドウマトリックス
	D3DLIGHT9 light;			// ライトの情報
	D3DXVECTOR4 posLight;		// ライトの位置
	D3DXVECTOR3 pos, normal;	// 平面の点, 法線ベクトル
	D3DXPLANE plane;			// 平面の情報
	D3DXMATERIAL mat;			// マテリアル保持用

	ParentObject* pParentObject = &g_aParentObject[0];

	for (int nCntObject = 0; nCntObject < MAX_OBJECT; nCntObject++)
	{
		if (g_aObject[nCntObject].bUse == true)
		{
			// ワールドマトリックスの初期化(デフォルトの値にする)
			D3DXMatrixIdentity(&g_aObject[nCntObject].mtxWorld);

			// 向きを反映
			D3DXMatrixRotationYawPitchRoll(&mtxRot, g_aObject[nCntObject].rot.y, g_aObject[nCntObject].rot.x, g_aObject[nCntObject].rot.z);
			D3DXMatrixMultiply(&g_aObject[nCntObject].mtxWorld, &g_aObject[nCntObject].mtxWorld, &mtxRot);

			// 位置を反映
			D3DXMatrixTranslation(&mtxTrans, g_aObject[nCntObject].pos.x, g_aObject[nCntObject].pos.y, g_aObject[nCntObject].pos.z);
			D3DXMatrixMultiply(&g_aObject[nCntObject].mtxWorld, &g_aObject[nCntObject].mtxWorld, &mtxTrans);

			// ワールドマトリックスの設定
			pDevice->SetTransform(D3DTS_WORLD, &g_aObject[nCntObject].mtxWorld);

			// 現在のマテリアルを取得
			pDevice->GetMaterial(&matDef);

			// マテリアルデータへのポインタを取得
			pMat = (D3DXMATERIAL*)g_aObjectModel[g_aObject[nCntObject].type].pBuffMat->GetBufferPointer();

			for (int nCntMat = 0; nCntMat < (int)g_aObjectModel[g_aObject[nCntObject].type].dwNumMat; nCntMat++)
			{
				// マテリアルの設定
				pDevice->SetMaterial(&pMat[nCntMat].MatD3D);

				pDevice->SetTexture(0, g_aObjectModel[g_aObject[nCntObject].type].apTexture[nCntMat]);
				
				// オブジェクトパーツの描画
				g_aObjectModel[g_aObject[nCntObject].type].pMesh->DrawSubset(nCntMat);	// ここでモデルの形を指定しているため、g_aObjectModelの中身を設定する必要がある
			}

			// 保存していたマテリアルに戻す
			pDevice->SetMaterial(&matDef);
		}
	}

	// 階層構造オブジェクト
	for (int nCntParentObject = 0; nCntParentObject < MAX_PARENTOBJECT; nCntParentObject++, pParentObject++)
	{
		if (pParentObject->bUse == false)
		{// 使っていなかったら弾く
			continue;
		}

		if (pParentObject->type == PARENTMODELTYPE_DRAGON)	// ドラゴンの場合
		{
			// ライトの位置を設定
			pDevice->GetLight(0, &light);
			posLight = D3DXVECTOR4(-light.Direction.x, -light.Direction.y, -light.Direction.z, 0.0f);

			// 平面情報を生成
			pos = D3DXVECTOR3(0.0f, 0.05f, 0.0f);		// 高さが0ならなんでもいい
			normal = NORMAL_PLANE;
			D3DXPlaneFromPointNormal(&plane, &pos, &normal);

			// 現在のマテリアルを取得
			pDevice->GetMaterial(&matDef);

			// 全モデル(パーツ)の描画
			for (int nCntOffSetModel = 0; nCntOffSetModel < pParentObject->pModelData->nNumModel; nCntOffSetModel++)
			{
				D3DXMATRIX mtxRotOffSetModel, mtxTransOffSetModel;	// 計算用マトリックス
				D3DXMATRIX mtxParent;								// 親のマトリックス

				// ワールドマトリックスの初期化(デフォルトの値にする)
				D3DXMatrixIdentity(&mtxShadow);

				// シャドウマトリックスの生成
				D3DXMatrixShadow(&mtxShadow, &posLight, &plane);
				D3DXMatrixMultiply(&mtxShadow, &pParentObject->pModelData->aModel[nCntOffSetModel].mtxWorld, &mtxShadow);

				// パーツのワールドマトリックスを設定
				pDevice->SetTransform(D3DTS_WORLD, &mtxShadow);

				// マテリアルデータへのポインタを取得
				pMat = (D3DXMATERIAL*)pParentObject->pModelData->aModel[nCntOffSetModel].pBuffMat->GetBufferPointer();

				for (int nCntMat = 0; nCntMat < (int)pParentObject->pModelData->aModel[nCntOffSetModel].dwNumMat; nCntMat++)
				{
					mat = pMat[nCntMat];

					mat.MatD3D.Diffuse.r = 0.0f;
					mat.MatD3D.Diffuse.g = 0.0f;
					mat.MatD3D.Diffuse.b = 0.0f;

					// マテリアルの設定
					pDevice->SetMaterial(&mat.MatD3D);

					// テクスチャの設定
					pDevice->SetTexture(0, pParentObject->pModelData->aModel[nCntOffSetModel].apTexture[nCntMat]);

					// プレイヤー(パーツ)の描画
					pParentObject->pModelData->aModel[nCntOffSetModel].pMesh->DrawSubset(nCntMat);
				}
			}
			// 保存していたマテリアルを戻す
			pDevice->SetMaterial(&matDef);
		}

		// ワールドマトリックスの初期化
		D3DXMatrixIdentity(&pParentObject->mtxWorld);

		// 向きを反映
		D3DXMatrixRotationYawPitchRoll(&mtxRot, pParentObject->rot.y, pParentObject->rot.x, pParentObject->rot.z);
		D3DXMatrixMultiply(&pParentObject->mtxWorld, &pParentObject->mtxWorld, &mtxRot);

		// 位置を反映
		D3DXMatrixTranslation(&mtxTrans, pParentObject->pos.x, pParentObject->pos.y, pParentObject->pos.z);
		D3DXMatrixMultiply(&pParentObject->mtxWorld, &pParentObject->mtxWorld, &mtxTrans);

		// ワールドマトリックスの設定
		pDevice->SetTransform(D3DTS_WORLD, &pParentObject->mtxWorld);

		// 現在のマテリアルを取得
		pDevice->GetMaterial(&matDef);

		// 全モデル(パーツ)の描画
		for (int nCntParentModel = 0; nCntParentModel < pParentObject->pModelData->nNumParts; nCntParentModel++)
		{
			D3DXMATRIX mtxRotOffSetModel, mtxTransOffSetModel;	// 計算用マトリックス
			D3DXMATRIX mtxParent;								// 親のマトリックス

			// パーツのワールドマトリックスを初期化
			D3DXMatrixIdentity(&pParentObject->pModelData->aModel[nCntParentModel].mtxWorld);

			// パーツの向きを反映
			D3DXMatrixRotationYawPitchRoll(&mtxRotOffSetModel, pParentObject->OffSetData.rot[nCntParentModel].y, pParentObject->OffSetData.rot[nCntParentModel].x, pParentObject->OffSetData.rot[nCntParentModel].z);
			D3DXMatrixMultiply(&pParentObject->pModelData->aModel[nCntParentModel].mtxWorld, &pParentObject->pModelData->aModel[nCntParentModel].mtxWorld, &mtxRotOffSetModel);

			// パーツの位置を反映(オフセット)
			D3DXMatrixTranslation(&mtxTransOffSetModel, pParentObject->OffSetData.pos[nCntParentModel].x, pParentObject->OffSetData.pos[nCntParentModel].y, pParentObject->OffSetData.pos[nCntParentModel].z);
			D3DXMatrixMultiply(&pParentObject->pModelData->aModel[nCntParentModel].mtxWorld, &pParentObject->pModelData->aModel[nCntParentModel].mtxWorld, &mtxTransOffSetModel);

			// パーツの「親のマトリックス」を設定
			if (pParentObject->pModelData->aModel[nCntParentModel].nIdxModelParent != -1)
			{// 親モデルがある場合
				mtxParent = pParentObject->pModelData->aModel[pParentObject->pModelData->aModel[nCntParentModel].nIdxModelParent].mtxWorld;
			}
			else
			{// 親モデルがない場合
				mtxParent = pParentObject->mtxWorld;
			}

			// 算出した「パーツのワールドマトリックス」と「親のマトリックス」を掛け合わせる
			D3DXMatrixMultiply(&pParentObject->pModelData->aModel[nCntParentModel].mtxWorld,
				&pParentObject->pModelData->aModel[nCntParentModel].mtxWorld,
				&mtxParent);

			// パーツのワールドマトリックスを設定
			pDevice->SetTransform(D3DTS_WORLD, &pParentObject->pModelData->aModel[nCntParentModel].mtxWorld);

			// マテリアルデータへのポインタを取得
			pMat = (D3DXMATERIAL*)pParentObject->pModelData->aModel[pParentObject->pModelData->aModel[nCntParentModel].nIdxModel].pBuffMat->GetBufferPointer();


			for (int nCntMat = 0; nCntMat < (int)pParentObject->pModelData->aModel[pParentObject->pModelData->aModel[nCntParentModel].nIdxModel].dwNumMat; nCntMat++)
			{
				MatCpy = pMat[nCntMat];		// 今のマテリアルをコピー

				MatCpy.MatD3D.Diffuse.a = pParentObject->fAlpha;	// アルファ値を適用

				// マテリアルの設定
				pDevice->SetMaterial(&MatCpy.MatD3D);

				// テクスチャの設定
				pDevice->SetTexture(0, pParentObject->pModelData->aModel[pParentObject->pModelData->aModel[nCntParentModel].nIdxModel].apTexture[nCntMat]);

				// パーツの描画
				pParentObject->pModelData->aModel[pParentObject->pModelData->aModel[nCntParentModel].nIdxModel].pMesh->DrawSubset(nCntMat);
			}
		}

		// 保存していたマテリアルを戻す
		pDevice->SetMaterial(&matDef);
	}
}

//=============================================================================
//	オブジェクトの当たり判定処理
//=============================================================================
bool CollisionObject(D3DXVECTOR3* pPos, D3DXVECTOR3* pPosOld, D3DXVECTOR3* pMove, float fRadius)
{
	Object* pObject = &g_aObject[0];				// 先頭アドレス
	D3DXMATRIX mtxRot, mtxTrans, mtxScale;			// 計算用マトリックス
	D3DXVECTOR3 posMax, posMin;						// 
	D3DXVECTOR3 posA, posB, posC, posD;
	bool isRand = false;

	for (int nCntModel = 0; nCntModel < MAX_OBJECT; nCntModel++, pObject++)
	{
		if (pObject->bUse == false)
		{// 使用していなかったら戻る
			continue;
		}

		if (pObject->bCollision == false)
		{// 当たり判定しないなら戻る
			continue;
		}

		ObjectModel* pObjectModel = &g_aObjectModel[pObject->type];	// モデルタイプ

		posA = D3DXVECTOR3(pObjectModel->vtxMin.x, 0.0f, pObjectModel->vtxMax.z);
		posB = D3DXVECTOR3(pObjectModel->vtxMax.x, 0.0f, pObjectModel->vtxMax.z);
		posC = D3DXVECTOR3(pObjectModel->vtxMax.x, 0.0f, pObjectModel->vtxMin.z);
		posD = D3DXVECTOR3(pObjectModel->vtxMin.x, 0.0f, pObjectModel->vtxMin.z);

		// ワールドマトリックスの初期化
		D3DXMatrixIdentity(&pObject->mtxWorld);

		// 向きを反映
		D3DXMatrixRotationYawPitchRoll(&mtxRot, pObject->rot.y, pObject->rot.x, pObject->rot.z);
		D3DXMatrixMultiply(&pObject->mtxWorld, &pObject->mtxWorld, &mtxRot);

		// 位置を反映
		D3DXMatrixTranslation(&mtxTrans, pObject->pos.x, pObject->pos.y, pObject->pos.z);
		D3DXMatrixMultiply(&pObject->mtxWorld, &pObject->mtxWorld, &mtxTrans);

		// 位置と向きを反映した頂点座標を入れる
		D3DXVec3TransformCoord(&posA, &posA, &pObject->mtxWorld);
		D3DXVec3TransformCoord(&posB, &posB, &pObject->mtxWorld);
		D3DXVec3TransformCoord(&posC, &posC, &pObject->mtxWorld);
		D3DXVec3TransformCoord(&posD, &posD, &pObject->mtxWorld);

		if (pPos->y + fRadius > pObject->pos.y + pObjectModel->vtxMin.y && pPos->y + fRadius < pObject->pos.y + pObjectModel->vtxMax.y)
		{// 当たり判定
			CrossCollision(pPos, pPosOld, posB, posA, true, false);
			CrossCollision(pPos, pPosOld, posC, posB, true, false);
			CrossCollision(pPos, pPosOld, posD, posC, true, false);
			CrossCollision(pPos, pPosOld, posA, posD, true, false);
		}

		// モデルの範囲内か判定
		if (pPos->x + fRadius > pObject->pos.x + pObjectModel->vtxMin.x && pPos->x + fRadius < pObject->pos.x + pObjectModel->vtxMax.x &&
			pPos->y + fRadius > pObject->pos.y + pObjectModel->vtxMin.y && pPos->y + fRadius < pObject->pos.y + pObjectModel->vtxMax.y &&
			pPos->z + fRadius > pObject->pos.z + pObjectModel->vtxMin.z && pPos->z + fRadius < pObject->pos.z + pObjectModel->vtxMax.z)
		{
			// 上から
			if (pPosOld->y + fRadius >= pObject->pos.y + pObjectModel->vtxMax.y)
			{
				pPos->y = pObject->pos.y + pObjectModel->vtxMax.y - fRadius;
				isRand = true;
			}

			// 下から
			if (pPosOld->y + fRadius <= pObject->pos.y + pObjectModel->vtxMin.y)
			{
				pPos->y = pObject->pos.y + pObjectModel->vtxMin.y - fRadius;
			}
		}
	}

	return isRand;
}

//======================================================================================
// オブジェクトを配置
//======================================================================================
void SetObject(OBJECTTYPE type, D3DXVECTOR3 pos, D3DXVECTOR3 rot, bool isShadow, bool isCollision, bool isRandObj)
{
	for (int nCntObject = 0; nCntObject < MAX_OBJECT; nCntObject++)
	{
		if (g_aObject[nCntObject].bUse == false)
		{
			// 弧度法への変換
			rot.x = DegreeToRadian(rot.x);
			rot.y = DegreeToRadian(rot.y);
			rot.z = DegreeToRadian(rot.z);

			g_aObject[nCntObject].pos = pos;
			g_aObject[nCntObject].rot = rot;
			g_aObject[nCntObject].type = type;
			g_aObject[nCntObject].bCollision = isCollision;
			g_aObject[nCntObject].bUse = true;

			if (isRandObj == true)
			{// ランダムオブジェクトだったら番号を記録
				g_nIdxRandObj[g_nNumRandObj] = nCntObject;
				g_nNumRandObj++;
			}

			// 影のIDを設定
			g_aObject[nCntObject].nIdxShadow = SetShadow(SHADOWTYPE_SQUARE, 180.0f, 180.0f);

			break;
		}
	}
}

//======================================================================================
// 階層構造オブジェクトを配置
//======================================================================================
void SetParentObject(D3DXVECTOR3 pos, D3DXVECTOR3 rot, PARENTMODELTYPE parentmodeltype)
{
	ParentObject* pParentObject = &g_aParentObject[0];		// 先頭アドレス

	for (int nCntParentObject = 0; nCntParentObject < MAX_PARENTOBJECT; nCntParentObject++, pParentObject++)
	{
		if (pParentObject->bUse == true)
		{// 使っていたら弾く
			continue;
		}

		// 弧度法への変換
		rot.x = DegreeToRadian(rot.x);
		rot.y = DegreeToRadian(rot.y);
		rot.z = DegreeToRadian(rot.z);

		pParentObject->pModelData = SetModelData(parentmodeltype);					// モデルデータ設定
		pParentObject->type = parentmodeltype;
		switch (parentmodeltype)
		{
		case PARENTMODELTYPE_PLAYER1P:
			pParentObject->motion.pMotionData = SetMotionData(MOTIONDATATYPE_PLAYER);	// モーションデータ設定
			break;
		case PARENTMODELTYPE_PLAYER2P:
			pParentObject->motion.pMotionData = SetMotionData(MOTIONDATATYPE_PLAYER);	// モーションデータ設定
			break;
		case PARENTMODELTYPE_HOUSE:
			pParentObject->motion.pMotionData = SetMotionData(MOTIONDATATYPE_HOUSE);	// モーションデータ設定
			break;
		case PARENTMODELTYPE_DRAGON:
			pParentObject->motion.pMotionData = SetMotionData(MOTIONDATATYPE_DRAGON);	// モーションデータ設定
			break;
		}

		// 各種設定
		pParentObject->pos = pos;
		pParentObject->rot = rot;
		pParentObject->bUse = true;

		// ニュートラルモーションで開始
		SetMotion(&pParentObject->motion, pParentObject->pModelData, &pParentObject->OffSetData, MOTIONTYPE_NEUTRAL, true, false, 10);

		break;
	}
}

//======================================================================================
// オブジェクトの情報を渡す
//======================================================================================
Object* GetObjectInfo(int nIdx)
{
	return &g_aObject[nIdx];
}

//======================================================================================
// 階層構造オブジェクトの情報を渡す
//======================================================================================
ParentObject* GetParentObjectInfo(int nIdx)
{
	return &g_aParentObject[nIdx];
}

//======================================================================================
// オブジェクトのモデル情報を渡す
//======================================================================================
ObjectModel* GetObjectModel(void)
{
	return &g_aObjectModel[0];
}

//======================================================================================
// オブジェクトファイルの読み込み処理
//======================================================================================
void LoadObjectModel(const char* pModelPath)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();	// デバイスの取得
	D3DXMATERIAL* pMat;
	int nNumVtx = 0;			// 頂点数
	DWORD dwSizeFVF;			// 頂点フォーマットのサイズ
	BYTE* pVtxBuff;				// 頂点バッファへのポインタ

	// Xファイルの読み込み
	D3DXVECTOR3 vtxMin = VTX_MIN;	// オブジェクトの大きさをローカルで保存
	D3DXVECTOR3 vtxMax = VTX_MAX;	// オブジェクトの大きさをローカルで保存

	D3DXLoadMeshFromX(pModelPath, D3DXMESH_SYSTEMMEM, pDevice, NULL, &g_aObjectModel[g_nNumObjectModel].pBuffMat, NULL, &g_aObjectModel[g_nNumObjectModel].dwNumMat, &g_aObjectModel[g_nNumObjectModel].pMesh);

	// 頂点数を取得
	nNumVtx = g_aObjectModel[g_nNumObjectModel].pMesh->GetNumVertices();

	// 頂点フォーマットのサイズを取得
	dwSizeFVF = D3DXGetFVFVertexSize(g_aObjectModel[g_nNumObjectModel].pMesh->GetFVF());

	// 頂点バッファをロック
	g_aObjectModel[g_nNumObjectModel].pMesh->LockVertexBuffer(D3DLOCK_READONLY, (void**)&pVtxBuff);

	// オブジェクトの大きさを取得
	for (int nCntVtx = 0; nCntVtx < nNumVtx; nCntVtx++, pVtxBuff += dwSizeFVF)
	{
		D3DXVECTOR3 vtx = *(D3DXVECTOR3*)pVtxBuff;	// 頂点座標の代入
		// X座標の比較
		if (vtxMin.x >= vtx.x)		// 保存されている最小のX座標より小さい場合
		{
			vtxMin.x = vtx.x;
		}
		else if (vtxMax.x <= vtx.x)	// 保存されている最大のX座標より大きい場合
		{
			vtxMax.x = vtx.x;
		}

		// Y座標の比較
		if (vtxMin.y >= vtx.y)		// 保存されている最小のY座標より小さい場合
		{
			vtxMin.y = vtx.y;
		}
		else if (vtxMax.y <= vtx.y)	// 保存されている最大のY座標より大きい場合
		{
			vtxMax.y = vtx.y;
		}

		// Z座標の比較
		if (vtxMin.z >= vtx.z)		// 保存されている最小のZ座標より小さい場合
		{
			vtxMin.z = vtx.z;
		}
		else if (vtxMax.z <= vtx.z)	// 保存されている最大のZ座標より大きい場合
		{
			vtxMax.z = vtx.z;
		}
	}

	// 頂点バッファをアンロック
	g_aObjectModel[g_nNumObjectModel].pMesh->UnlockVertexBuffer();

	if (vtxMin.y > -3.0f)
	{
		vtxMin.y = -5.0f;
	}

	// オブジェクトの当たり判定を各配列に代入
	g_aObjectModel[g_nNumObjectModel].vtxMin = vtxMin;
	g_aObjectModel[g_nNumObjectModel].vtxMax = vtxMax;

	// マテリアルデータへのポインタを取得
	pMat = (D3DXMATERIAL*)g_aObjectModel[g_nNumObjectModel].pBuffMat->GetBufferPointer();

	// テクスチャの読み込み
	for (int nCntMat = 0; nCntMat < (int)g_aObjectModel[g_nNumObjectModel].dwNumMat; nCntMat++)
	{
		if (pMat[nCntMat].pTextureFilename != NULL)	// テクスチャファイルが存在する場合
		{
			D3DXCreateTextureFromFile(pDevice, pMat[nCntMat].pTextureFilename, &g_aObjectModel[g_nNumObjectModel].apTexture[nCntMat]);
		}
	}

	g_nNumObjectModel++;	// 読み込んだモデル数加算
}

//======================================================================================
// 階層構造モデルファイル紐づけ処理
//======================================================================================
ModelData* SetModelData(PARENTMODELTYPE type)
{
	return &g_aModelData[type];
}

//======================================================================================
// 階層構造モデルファイルの読み込み処理
//======================================================================================
void LoadParentModel(const char* pModelPath, int nNumParentModel)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();	// デバイスの取得

	D3DXMATERIAL* pMat;

	Model* pModel = &g_aModelData[nNumParentModel].aModel[g_aModelData[nNumParentModel].nNumModel];

	// Xファイルの読み込み
	D3DXLoadMeshFromX(pModelPath,
		D3DXMESH_SYSTEMMEM,
		pDevice,
		NULL,
		&pModel->pBuffMat,
		NULL,
		&pModel->dwNumMat,
		&pModel->pMesh);

	// マテリアルデータへのポインタを取得
	pMat = (D3DXMATERIAL*)pModel->pBuffMat->GetBufferPointer();

	for (int nCntMat = 0; nCntMat < (int)pModel->dwNumMat; nCntMat++)
	{
		if (pMat[nCntMat].pTextureFilename != NULL)
		{// テクスチャファイルが存在する
			// テクスチャの読み込み
			D3DXCreateTextureFromFile(pDevice,
				pMat[nCntMat].pTextureFilename,
				&pModel->apTexture[nCntMat]);
		}
	}

	g_aModelData[nNumParentModel].nNumModel++;
}

//=============================================================================
//	階層構造モデルのオフセット読み込み処理
//=============================================================================
void LoadParentModelOffSet(D3DXVECTOR3 pos, D3DXVECTOR3 rot, int nIdxModel, int nIdxModelParent, int nNumParentModel, int nCntParts)
{
	g_aModelData[nNumParentModel].nNumParts++;
	Model* pModel = &g_aModelData[nNumParentModel].aModel[nCntParts];

	pModel->nIdxModel = nIdxModel;
	pModel->nIdxModelParent = nIdxModelParent;
	pModel->pos = pModel->posLocal = pos;
	pModel->rot = pModel->rotLocal = rot;
}

//=============================================================================
//	ランダムオブジェクトの終了処理
//=============================================================================
void UninitRandomObject(void)
{
	// Object情報の初期化
	for (int nCntObject = 0; nCntObject < g_nNumRandObj; nCntObject++)
	{
		g_aObject[g_nIdxRandObj[nCntObject]].pos = DEFALT;
		g_aObject[g_nIdxRandObj[nCntObject]].rot = DEFALT;
		g_aObject[g_nIdxRandObj[nCntObject]].type = OBJECTTYPE_HOUSE000;
		g_aObject[g_nIdxRandObj[nCntObject]].nIdxShadow = -1;
		g_aObject[g_nIdxRandObj[nCntObject]].fSize = g_aObjectModel[g_aObject[nCntObject].type].vtxMax.x;
		g_aObject[g_nIdxRandObj[nCntObject]].bUse = false;
	}

	memset(&g_nIdxRandObj, -1, sizeof(int) * MAX_OBJECT);					// ランダムオブジェクトの番号初期化
	g_nNumRandObj = 0;
}

//=============================================================================
//	イベントでのオブジェクト更新処理
//=============================================================================
void UpdateObjectEvent001(ParentObject* pParentObject)
{
	if (pParentObject->motion.bFinishMotion == true)
	{
		pParentObject->fAlpha -= 0.01f;

		if (pParentObject->fAlpha < 0.0f)
		{
			pParentObject->bUse = false;
		}
	}
}

#define DRAGON_DEFAULTPOS		(D3DXVECTOR3(0.0f, 3000.0f, 3500.0f))
#define DRAGON_MOVERADIUS		(3000.0f)

//=============================================================================
//	タイトルでのドラゴンの更新処理
//=============================================================================
void UpdateTitleDragon(ParentObject* pParentObject)
{
	static float fAngle = 0.0f;
	D3DXVECTOR3 pos = DRAGON_DEFAULTPOS;
	D3DXVECTOR3 posDest;

	fAngle += 0.005f;

	fAngle = AngleNormalize(fAngle);

	posDest.x = pos.x + sinf(fAngle) * DRAGON_MOVERADIUS;
	posDest.y = pos.y;
	posDest.z = pos.z + cosf(fAngle) * DRAGON_MOVERADIUS;

	pParentObject->rot.y = atan2f(pParentObject->pos.x - posDest.x, pParentObject->pos.z - posDest.z);

	pParentObject->rot.y = AngleNormalize(pParentObject->rot.y);

	pParentObject->pos = posDest;

}