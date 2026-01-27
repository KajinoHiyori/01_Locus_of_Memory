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
Animal g_aAnimal[MAX_ANIMAL];
AnimalModel g_aAnimalModel[MAX_ANIMALMODEL];

//===============================
//	モデルファイル
//===============================
const char* c_apFilenameAnimal[MAX_ANIMALMODEL] =
{
	"data\\MODEL\\animal\\bird000.x",		// 鳥
	"data\\MODEL\\animal\\den000.x",		// 鳥の巣
	"data\\MODEL\\animal\\dragon000.x",		// ドラゴン(上顎)
	"data\\MODEL\\animal\\dragon001.x"		// ドラゴン(下顎)
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
	for (int nCntAnimal = 0; nCntAnimal < MAX_ANIMAL; nCntAnimal++)
	{
		g_aAnimal[nCntAnimal].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		g_aAnimal[nCntAnimal].rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		g_aAnimal[nCntAnimal].nType = 0;
		g_aAnimal[nCntAnimal].bUse = false;
	}

	// 頂点の初期化
	for (int nCntAnimalModel = 0; nCntAnimalModel < MAX_ANIMALMODEL; nCntAnimalModel++)
	{
		g_aAnimalModel[nCntAnimalModel].vtxMin = D3DXVECTOR3(10000.0f, 10000.0f, 10000.0f);
		g_aAnimalModel[nCntAnimalModel].vtxMax = D3DXVECTOR3(-10000.0f, -10000.0f, -10000.0f);

		D3DXLoadMeshFromX(c_apFilenameAnimal[nCntAnimalModel],
			D3DXMESH_SYSTEMMEM,
			pDevice, NULL,
			&g_aAnimalModel[nCntAnimalModel].pBuffMat,
			NULL,
			&g_aAnimalModel[nCntAnimalModel].dwNumMat,
			&g_aAnimalModel[nCntAnimalModel].pMesh);

		// 頂点数を取得
		nNumVtx = g_aAnimalModel[nCntAnimalModel].pMesh->GetNumVertices();

		// 頂点フォーマットのサイズを取得
		dwSizeFVF = D3DXGetFVFVertexSize(g_aAnimalModel[nCntAnimalModel].pMesh->GetFVF());

		// 頂点バッファをロック
		g_aAnimalModel[nCntAnimalModel].pMesh->LockVertexBuffer(D3DLOCK_READONLY, (void**)&pVtxBuff);

		for (int nCntVtx = 0; nCntVtx < nNumVtx; nCntVtx++)
		{
			D3DXVECTOR3 vtx = *(D3DXVECTOR3*)pVtxBuff;				// 頂点座標の代入

			if (g_aAnimalModel[nCntAnimalModel].vtxMin.x > vtx.x)
			{	  
				g_aAnimalModel[nCntAnimalModel].vtxMin.x = vtx.x;
			}	 
			if (g_aAnimalModel[nCntAnimalModel].vtxMin.z > vtx.z)
			{	  
				g_aAnimalModel[nCntAnimalModel].vtxMin.z = vtx.z;
			}	  
			if (g_aAnimalModel[nCntAnimalModel].vtxMin.y > vtx.y)
			{	 
				g_aAnimalModel[nCntAnimalModel].vtxMin.y = vtx.y;
			}	  

			if (g_aAnimalModel[nCntAnimalModel].vtxMax.x < vtx.x)
			{	  
				g_aAnimalModel[nCntAnimalModel].vtxMax.x = vtx.x;
			}	   
			if (g_aAnimalModel[nCntAnimalModel].vtxMax.z < vtx.z)
			{	   
				g_aAnimalModel[nCntAnimalModel].vtxMax.z = vtx.z;
			}	  
			if (g_aAnimalModel[nCntAnimalModel].vtxMax.y < vtx.y)
			{	   
				g_aAnimalModel[nCntAnimalModel].vtxMax.y = vtx.y;
			}

			pVtxBuff += dwSizeFVF;									// 頂点フォーマットのサイズ分ポインタを進める
		}

		// 頂点バッファをアンロック
		g_aAnimalModel[nCntAnimalModel].pMesh->UnlockVertexBuffer();
	}
}

//===============================
//	動物の終了処理
//===============================
