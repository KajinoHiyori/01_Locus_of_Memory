//========================================================================
//
//	魔法動物の処理[animal.cpp]
//	Author:OBIKA SOUMA
//
//========================================================================

#include "animal.h"

//===============================
//	グローバル変数
//===============================
Animal g_Animal;

//===============================
//	モデルファイル
//===============================
const char* c_apFilenameAnimal[MAX_ANIMAL] =
{
	"data\\MODEL\\animal\\dragon000.x",		// ドラゴン(上顎)
	"data\\MODEL\\animal\\dragon001.x",		// ドラゴン(下顎)
	"data\\MODEL\\animal\\dragon002.x",		// ドラゴン(首)
	"data\\MODEL\\animal\\dragon003.x"		// ドラゴン(手)
};

//===============================
//	動物の初期化処理
//===============================
void InitAnimal(void)
{
	int nNumVtx;							// 頂点数
	DWORD dwSizeFVF;						// 頂点フォーマットのサイズ
	BYTE* pVtxBuff;							// 頂点バッファへのポインタ
	LPDIRECT3DDEVICE9 pDevice;

	// デバイスの取得
	pDevice = GetDevice();

	// 動物の初期化
	g_Animal.pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	g_Animal.rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	g_Animal.nType = 0;
	g_Animal.bUse = false;

	for (int nCntModel = 0; nCntModel < MAX_ANIMAL; nCntModel++)
	{
		D3DXLoadMeshFromX(c_apFilenameAnimal[nCntModel],
			D3DXMESH_SYSTEMMEM,
			pDevice, NULL,
			&g_Animal.aModel[nCntModel].pBuffMat,
			NULL,
			&g_Animal.aModel[nCntModel].dwNumMat,
			&g_Animal.aModel[nCntModel].pMesh);
	}

	g_Animal.nNumModel = MAX_ANIMAL;

	// 各パーツの階層構造設定
	g_Animal.aModel[0].nIdxModelParent = -1;							// 親モデルのインデックスを設定
	g_Animal.aModel[0].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);				// パーツ[0]の位置(オフセット)
	g_Animal.aModel[0].rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);				// パーツ[0]の向き

	g_Animal.aModel[1].nIdxModelParent = 0;								// 親モデルのインデックスを設定
	g_Animal.aModel[1].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);				// パーツ[1]の位置(オフセット)
	g_Animal.aModel[1].rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);				// パーツ[1]の向き

	g_Animal.aModel[2].nIdxModelParent = 0;								// 親モデルのインデックスを設定
	g_Animal.aModel[2].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);				// パーツ[2]の位置(オフセット)
	g_Animal.aModel[2].rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);				// パーツ[2]の向き

	g_Animal.aModel[3].nIdxModelParent = 0;								// 親モデルのインデックスを設定
	g_Animal.aModel[3].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);				// パーツ[3]の位置(オフセット)
	g_Animal.aModel[3].rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);				// パーツ[3]の向き

}

//===============================
//	動物の終了処理
//===============================
void UninitAnimal(void)
{
	for (int nCntModel = 0; nCntModel < MAX_ANIMAL; nCntModel++)
	{
		// テクスチャの破棄
		if (g_Animal.aModel[nCntModel].pMesh != NULL)
		{
			g_Animal.aModel[nCntModel].pMesh->Release();
			g_Animal.aModel[nCntModel].pMesh = NULL;
		}

		// 頂点バッファの破棄
		if (g_Animal.aModel[nCntModel].pBuffMat != NULL)
		{
			g_Animal.aModel[nCntModel].pBuffMat->Release();
			g_Animal.aModel[nCntModel].pBuffMat = NULL;
		}
	}
}

//===============================
//	動物の更新処理
//===============================
void UpdateAnimal(void)
{

}

//===============================
//	動物の描画処理
//===============================
void DrawAnimal(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();
	D3DXMATRIX mtxRot, mtxTrans;			// 計算用マトリックス
	D3DMATERIAL9 matDet;					// 現在のマテリアル保存用
	D3DXMATERIAL* pMat;						// マテリアルデータへのポインタ

	// 現在のマテリアルを取得
	pDevice->GetMaterial(&matDet);

	// ワールドマトリックスの初期化
	D3DXMatrixIdentity(&g_Animal.mtxWorld);

	// 向きを反映
	D3DXMatrixRotationYawPitchRoll(&mtxRot, g_Animal.rot.y,
		g_Animal.rot.x,
		g_Animal.rot.z);
	D3DXMatrixMultiply(&g_Animal.mtxWorld, &g_Animal.mtxWorld, &mtxRot);

	// 位置を反映
	D3DXMatrixTranslation(&mtxTrans,
		g_Animal.pos.x,
		g_Animal.pos.y,
		g_Animal.pos.z);
	D3DXMatrixMultiply(&g_Animal.mtxWorld, &g_Animal.mtxWorld, &mtxTrans);

	//  ワールドマトリックスの設定
	pDevice->SetTransform(D3DTS_WORLD, &g_Animal.mtxWorld);

	// 全モデル(パーツ)の描画
	for (int nCntModel = 0; nCntModel < g_Animal.nNumModel; nCntModel++)
	{
		D3DXMATRIX mtxRotModel, mmtxTransModel;						// 計算用マトリックス
		D3DXMATRIX mtxParent;										// 親のマトリックス

		// ワールドマトリックスの初期化
		D3DXMatrixIdentity(&g_Animal.aModel[nCntModel].mtxWorld);

		// 向きを反映
		D3DXMatrixRotationYawPitchRoll(&mtxRot, g_Animal.aModel[nCntModel].rot.y,
			g_Animal.aModel[nCntModel].rot.x,
			g_Animal.aModel[nCntModel].rot.z);
		D3DXMatrixMultiply(&g_Animal.aModel[nCntModel].mtxWorld, &g_Animal.aModel[nCntModel].mtxWorld, &mtxRot);

		// 位置を反映
		D3DXMatrixTranslation(&mtxTrans,
			g_Animal.aModel[nCntModel].pos.x,
			g_Animal.aModel[nCntModel].pos.y,
			g_Animal.aModel[nCntModel].pos.z);
		D3DXMatrixMultiply(&g_Animal.aModel[nCntModel].mtxWorld, &g_Animal.aModel[nCntModel].mtxWorld, &mtxTrans);

		// パーツの「親のマトリックス」を設定
		if (g_Animal.aModel[nCntModel].nIdxModelParent != -1)
		{// 親モデルがある場合
			mtxParent = g_Animal.aModel[g_Animal.aModel[nCntModel].nIdxModelParent].mtxWorld;
		}
		else
		{// 親モデルがない場合
			mtxParent = g_Animal.mtxWorld;
		}

		// 算出した「パーツのワールドマトリックス」と「親のマトリックス」を掛け合わせる
		D3DXMatrixMultiply(&g_Animal.aModel[nCntModel].mtxWorld,
			&g_Animal.aModel[nCntModel].mtxWorld,
			&mtxParent);

		//  ワールドマトリックスの設定
		pDevice->SetTransform(D3DTS_WORLD, &g_Animal.aModel[nCntModel].mtxWorld);

		// マテリアルデータへのポインタを取得
		pMat = (D3DXMATERIAL*)g_Animal.aModel[nCntModel].pBuffMat->GetBufferPointer();

		for (int nCntMat = 0; nCntMat < (int)g_Animal.aModel[nCntModel].dwNumMat; nCntMat++)
		{
			// マテリアルの設定
			pDevice->SetMaterial(&pMat[nCntMat].MatD3D);

			// テクスチャの設定
			pDevice->SetTexture(0, NULL);

			// モデルパーツの描画
			g_Animal.aModel[nCntModel].pMesh->DrawSubset(nCntMat);
		}
	}
	// 保存していたマテリアルを戻す
	pDevice->SetMaterial(&matDet);
}