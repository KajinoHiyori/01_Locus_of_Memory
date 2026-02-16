//=============================================================================
//
//	カスタムメッシュ処理 [meshfield.cpp]
//	Author : SHUMA AIZU
// 
//=============================================================================

#include "main.h"
#include "custommesh.h"
#include "input.h"

#include "debugproc.h"

//*****************************************************************************
// マクロ定義
//*****************************************************************************
#define MESHFIELD_SPLIT_WIDHT		(30 + 1)		// 横の分割数
#define MESHFIELD_SPLIT_DEPTH		(30 + 1)		// 縦の分割数
#define MAX_MESHFIELD				(32)			// メッシュフィールドの最大数
#define MAX_CUSTOMMESH				(5)				// カスタムメッシュの最大数

//*****************************************************************************
// グローバル変数
//*****************************************************************************
LPDIRECT3DTEXTURE9 g_apCustomTextureMesh[MESHFIELDTYPE_MAX] = {};	// テクスチャへのポインタ
CustomMesh g_aCustomMesh[MAX_CUSTOMMESH];							// カスタムメッシュの情報

const char* c_pCustomMeshTextureName[MESHFIELDTYPE_MAX] =
{
	"data\\TEXTURE\\river000.jpg",
	"data\\TEXTURE\\road000.jpg",
	"data\\TEXTURE\\road000.jpg",
};

//=============================================================================
//	メッシュフィールドの初期化処理
//=============================================================================
void InitCustomMesh(void)
{
	// デバイスの取得
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	LPCUSTOMMESH pCustomMesh = &g_aCustomMesh[0];	// 先頭アドレス

	// テクスチャの読み込み
	for (int nCntTex = 0; nCntTex < MESHFIELDTYPE_MAX; nCntTex++)
	{
		D3DXCreateTextureFromFile(pDevice,
			c_pCustomMeshTextureName[nCntTex],
			&g_apCustomTextureMesh[nCntTex]);
	}

	// 数分初期化(後でmemsetに変えてもいいかも)
	for (int nCntCustomMesh = 0; nCntCustomMesh < MAX_CUSTOMMESH; nCntCustomMesh++, pCustomMesh++)
	{
		pCustomMesh->pVtxBuff = NULL;
		pCustomMesh->pIdxBuff = NULL;
		pCustomMesh->pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		pCustomMesh->rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		pCustomMesh->type = MESHFIELDTYPE_MAX;
		pCustomMesh->bUse = false;
	}
}

//=============================================================================
//	メッシュフィールドの終了処理
//=============================================================================
void UninitCustomMesh(void)
{
	LPCUSTOMMESH pCustomMesh = &g_aCustomMesh[0];		// 先頭アドレス

	// テクスチャの破棄
	for (int nCntTex = 0; nCntTex < MESHFIELDTYPE_MAX; nCntTex++)
	{
		if (g_apCustomTextureMesh[nCntTex] != NULL)
		{
			g_apCustomTextureMesh[nCntTex]->Release();
			g_apCustomTextureMesh[nCntTex] = NULL;
		}
	}

	for (int nCntCustomMesh = 0; nCntCustomMesh < MAX_CUSTOMMESH; nCntCustomMesh++, pCustomMesh++)
	{
		// 頂点バッファの破棄
		if (pCustomMesh->pVtxBuff != NULL)
		{
			pCustomMesh->pVtxBuff->Release();
			pCustomMesh->pVtxBuff = NULL;
		}

		// インデックスバッファの破棄
		if (pCustomMesh->pIdxBuff != NULL)
		{
			pCustomMesh->pIdxBuff->Release();
			pCustomMesh->pIdxBuff = NULL;
		}
	}
}

//=============================================================================
//	メッシュフィールドの描画処理
//=============================================================================
void DrawCustomMesh(void)
{
	LPCUSTOMMESH pCustomMesh = &g_aCustomMesh[0];	// 先頭アドレス

	LPDIRECT3DDEVICE9 pDevice = GetDevice();	// デバイスの取得
	D3DXMATRIX mtxRot, mtxTrans;				// 計算用マトリックス

	for (int nCntCustomMesh = 0; nCntCustomMesh < MAX_CUSTOMMESH; nCntCustomMesh++, pCustomMesh++)
	{
		if (pCustomMesh->bUse == false)
		{// 使っていなければ弾く
			continue;
		}

		// ワールドマトリックスの初期化
		D3DXMatrixIdentity(&pCustomMesh->mtxWorld);

		// 向きを反映
		D3DXMatrixRotationYawPitchRoll(&mtxRot, pCustomMesh->rot.y, pCustomMesh->rot.x, pCustomMesh->rot.z);
		D3DXMatrixMultiply(&pCustomMesh->mtxWorld, &pCustomMesh->mtxWorld, &mtxRot);

		// 位置を反映
		D3DXMatrixTranslation(&mtxTrans, pCustomMesh->pos.x, pCustomMesh->pos.y, pCustomMesh->pos.z);
		D3DXMatrixMultiply(&pCustomMesh->mtxWorld, &pCustomMesh->mtxWorld, &mtxTrans);

		// ワールドマトリックスの設定
		pDevice->SetTransform(D3DTS_WORLD, &pCustomMesh->mtxWorld);

		// 頂点バッファをデータストリームに設定
		pDevice->SetStreamSource(0, pCustomMesh->pVtxBuff, 0, sizeof(VERTEX_3D));

		// インデックスバッファをデータストリームに設定
		pDevice->SetIndices(pCustomMesh->pIdxBuff);

		// 頂点フォーマットの設定
		pDevice->SetFVF(FVF_VERTEX_3D);

		// テクスチャの設定
		pDevice->SetTexture(0, g_apCustomTextureMesh[pCustomMesh->type]);

		// メッシュフィールドの描画
		pDevice->DrawIndexedPrimitive(D3DPT_TRIANGLESTRIP,
			0,
			0,
			pCustomMesh->nNumVtx,
			0,
			pCustomMesh->nNumIdx - 4);
	}
}

//=============================================================================
//	メッシュフィールドの更新処理
//=============================================================================
void UpdateCustomMesh(void)
{

}

//=============================================================================
//	カスタムメッシュの設定処理
//=============================================================================
void SetCustomMesh(D3DXVECTOR3 pos, D3DXVECTOR3 rot, D3DXVECTOR3* pVtxPos, int nNumVtx, int* pIdxInfo, int nNumIdx)
{
	// デバイスの取得
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	LPCUSTOMMESH pCustomMesh = &g_aCustomMesh[0];

	for (int nCntCustomMesh = 0; nCntCustomMesh < MAX_CUSTOMMESH; nCntCustomMesh++, pCustomMesh++)
	{
		if (pCustomMesh->bUse == true)
		{
			continue;
		}

		// カスタムメッシュの設定
		pCustomMesh->pos = pos;
		pCustomMesh->rot = rot;
		pCustomMesh->type = MESHFIELDTYPE_000;
		pCustomMesh->nNumVtx = nNumVtx;
		pCustomMesh->nNumIdx = nNumIdx;

		// 頂点バッファの生成
		pDevice->CreateVertexBuffer(sizeof(VERTEX_3D) * nNumVtx,
			D3DUSAGE_WRITEONLY,
			FVF_VERTEX_3D,
			D3DPOOL_MANAGED,
			&pCustomMesh->pVtxBuff,
			NULL);

		// 初期化
		VERTEX_3D* pVtx;			// 頂点情報へのポインタ

		// 頂点バッファをロックし,頂点情報へのポインタを取得
		pCustomMesh->pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

		for (int nCntVtx = 0; nCntVtx < nNumVtx; nCntVtx++)
		{
			pVtx[nCntVtx].pos = *pVtxPos;
			pVtx[nCntVtx].nor = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
			pVtx[nCntVtx].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
			pVtx[nCntVtx].tex = D3DXVECTOR2(1.0f * nCntVtx, 1.0f * nCntVtx);

			pVtxPos++;
		}

		// 頂点バッファをアンロックする
		pCustomMesh->pVtxBuff->Unlock();

		// インデックスバッファの設定
		pDevice->CreateIndexBuffer(sizeof(WORD) * nNumIdx,
			D3DUSAGE_WRITEONLY,
			D3DFMT_INDEX16,
			D3DPOOL_MANAGED,
			&pCustomMesh->pIdxBuff,
			NULL);

		WORD* pIdx;		// インデックス情報へのポインタ

		// インデックスバッファをロックし、頂点情報へのポインタを取得
		pCustomMesh->pIdxBuff->Lock(0, 0, (void**)&pIdx, 0);

		for (int nCntIdx = 0; nCntIdx < nNumIdx; nCntIdx++)
		{
			pCustomMesh->nIdx[nCntIdx] = *pIdxInfo;
			pIdx[nCntIdx] = pCustomMesh->nIdx[nCntIdx];

			pIdxInfo++;
		}

		// インデックスバッファをアンロックする
		pCustomMesh->pIdxBuff->Unlock();

		pCustomMesh->bUse = true;
		break;

	}
}

//=============================================================================
//	メッシュの読み込み処理
//=============================================================================
void LoadCustomMesh(const char* pFileName, D3DXVECTOR3 pos, D3DXVECTOR3 rot)
{
	FILE* pFile = fopen(pFileName, "rb");		// 引数のファイルを開く

	if (pFile == NULL)
	{// 開けなかったら終了
		return;
	}

	// 各値格納用ローカル変数
	int nNumVtx = 0;
	D3DXVECTOR3 VtxPos[MAX_VTX] = {};
	int nNumIdx = 0;
	int aIdx[MAX_IDX] = {};

	// ファイルから取得した情報を格納
	fread(&nNumVtx, sizeof(int), 1, pFile);
	fread(&VtxPos[0], sizeof(D3DXVECTOR3), nNumVtx, pFile);
	fread(&nNumIdx, sizeof(int), 1, pFile);
	fread(&aIdx[0], sizeof(int), nNumIdx, pFile);

	fclose(pFile);

	SetCustomMesh(pos, rot, &VtxPos[0], nNumVtx, &aIdx[0], nNumIdx);
}