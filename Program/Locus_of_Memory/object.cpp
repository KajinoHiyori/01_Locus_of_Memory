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

// マクロ定義
#define NUM_OBJECT	(OBJECTTYPE_MAX)	// モデルの数
#define BLANK		(0.001f)		// 空白
#define DEFALT		(D3DXVECTOR3(0.0f, 0.0f, 0.0f))					// xyzが0.0fの場合
#define NORMAL		(D3DXVECTOR3(0.0f, 1.0f, 0.0f))					// 基本の法線
#define VTX_MIN		(D3DXVECTOR3(10000.0f, 10000.0f, 10000.0f))		// オブジェクトの大きさの初期化値(最小)
#define VTX_MAX		(D3DXVECTOR3(-10000.0f, -10000.0f, -10000.0f))	// オブジェクトの大きさの初期化値(最大)

// グローバル変数
ObjectModel g_aObjectModel[NUM_OBJECT];		// モデルの種類を管理
Object g_aObject[MAX_OBJECT];				// オブジェクトの情報を格納
ModelData g_aModelData[MAX_PARENTMODEL];	// 階層構造を持ったモデルデータ

int g_nNumObjectModel;	// モデル数を管理

//======================================================================================
// オブジェクトの初期化処理
//======================================================================================
void InitObject(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();	// デバイスの取得
	//D3DXMATERIAL* pMat;
	//int nNumVtx = 0;	// 頂点数
	//DWORD dwSizeFVF;			// 頂点フォーマットのサイズ
	//BYTE* pVtxBuff;				// 頂点バッファへのポインタ
	g_nNumObjectModel = 0;

	ModelData* pModelData = &g_aModelData[0];

	// ModelDataの初期化
	memset(pModelData, NULL, sizeof(ModelData) * MAX_PARENTMODEL);

	// Object情報の初期化
	for (int nCntObject = 0; nCntObject < MAX_OBJECT; nCntObject++)
	{
		g_aObject[nCntObject].pos = DEFALT;
		g_aObject[nCntObject].rot = DEFALT;
		g_aObject[nCntObject].type = OBJECTTYPE_HOUSE000;
		g_aObject[nCntObject].nIdxShadow = -1;
		g_aObject[nCntObject].fSize = g_aObjectModel[g_aObject[nCntObject].type].vtxMax.x;
		g_aObject[nCntObject].bUse = false;
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

	LoadModel(MODEL_SCRIPT);

	//LoadParentModel(PARENTMODEL_1PSCRIPT);
	//LoadParentModel(PARENTMODEL_2PSCRIPT);
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
	Player* pPlayer = GetPlayer();
	MODE mode = GetMode();

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
}

//=============================================================================
//	オブジェクトの当たり判定処理
//=============================================================================
void CollisionObject(D3DXVECTOR3* pPos, D3DXVECTOR3* pPosOld, D3DXVECTOR3* pMove, float fRadius)
{
	Object* pObject = &g_aObject[0];				// 先頭アドレス
	D3DXMATRIX mtxRot, mtxTrans, mtxScale;			// 計算用マトリックス
	D3DXVECTOR3 posMax, posMin;						// 
	D3DXVECTOR3 posA, posB, posC, posD;

	for (int nCntModel = 0; nCntModel < MAX_OBJECT; nCntModel++, pObject++)
	{
		if (pObject->bUse == false)
		{// 使用していなかったら戻る
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

		if (pPos->y + fRadius >= pObject->pos.y + pObjectModel->vtxMin.y && pPos->y + fRadius <= pObject->pos.y + pObjectModel->vtxMax.y)
		{
			//// 当たり判定
			//if (pObject->bCollision == true)
			//{
				CrossCollision(pPos, pPosOld, posB, posA, true, false);
				CrossCollision(pPos, pPosOld, posC, posB, true, false);
				CrossCollision(pPos, pPosOld, posD, posC, true, false);
				CrossCollision(pPos, pPosOld, posA, posD, true, false);
			//}

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
			}

			// 下から
			if (pPosOld->y + fRadius <= pObject->pos.y + pObjectModel->vtxMin.y)
			{
				pPos->y = pObject->pos.y + pObjectModel->vtxMin.y - fRadius;
			}
		}
	}
}

//======================================================================================
// オブジェクトを配置
//======================================================================================
void SetObject(OBJECTTYPE type, D3DXVECTOR3 pos, D3DXVECTOR3 rot, bool isShadow, bool isCollision)
{
	for (int nCntObject = 0; nCntObject < MAX_OBJECT; nCntObject++)
	{
		if (g_aObject[nCntObject].bUse == false)
		{
			g_aObject[nCntObject].pos = pos;
			g_aObject[nCntObject].rot = rot;
			g_aObject[nCntObject].type = type;
			g_aObject[nCntObject].bUse = true;

			// 影のIDを設定
			g_aObject[nCntObject].nIdxShadow = SetShadow(SHADOWTYPE_SQUARE, 180.0f, 180.0f);

			break;
		}
	}
}

//======================================================================================
// オブジェクトの情報を渡す
//======================================================================================
Object* GetObjectInfo(void)
{
	return &g_aObject[0];
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
void LoadParentModelOffSet(D3DXVECTOR3 pos, D3DXVECTOR3 rot, int nIdxModel, int nIdxModelParent, int nNumParentModel)
{
	Model* pModel = &g_aModelData[nNumParentModel].aModel[nIdxModel];

	pModel->nIdxModel = nIdxModel;
	pModel->nIdxModelParent = nIdxModelParent;
	pModel->pos = pModel->posLocal = pos;
	pModel->rot = pModel->rotLocal = rot;
}