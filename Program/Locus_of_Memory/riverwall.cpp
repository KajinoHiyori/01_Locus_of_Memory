//=============================================================================
//
//	川沿いの壁の処理 [riverwall.cpp]
//	Author : SHUMA AIZU
// 
//=============================================================================

#include "main.h"
#include "riverwall.h"
#include "input.h"

#include "debugproc.h"

//*****************************************************************************
// マクロ定義
//*****************************************************************************
#define MAX_CUSTOMMESH				(5)								// 壁の最大数
#define TEX_SPLIT					(10)							// テクスチャの分割数
#define TEX_DEFAULT					(D3DXVECTOR2(1.0f, 1.0f));		// テクスチャの初期位置

//*****************************************************************************
// グローバル変数
//*****************************************************************************
LPDIRECT3DTEXTURE9 g_pCustomTextureMesh = NULL;	// テクスチャへのポインタ
CustomMesh g_RiverWall;							// 壁の情報

//=============================================================================
//	メッシュフィールドの初期化処理
//=============================================================================
void InitRiverWall(void)
{
	// デバイスの取得
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	LPCUSTOMMESH pRiverWall = &g_RiverWall;	// 先頭アドレス

	// テクスチャの読み込み
	D3DXCreateTextureFromFile(pDevice,
		"data\\TEXTURE\\stone000.jpg",
		&g_pCustomTextureMesh);

	// 数分初期化(後でmemsetに変えてもいいかも)
	for (int nCntRiverWall = 0; nCntRiverWall < MAX_CUSTOMMESH; nCntRiverWall++, pRiverWall++)
	{
		pRiverWall->pVtxBuff = NULL;
		pRiverWall->pIdxBuff = NULL;
		pRiverWall->pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		pRiverWall->rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		pRiverWall->type = MESHFIELDTYPE_MAX;
		pRiverWall->bUse = false;
	}

	LoadRiverWall("data\\SCRIPTS\\MESH\\riverwall000.bin", D3DXVECTOR3(1800.0f, 0.0f, 3500.0f), INIT_D3DXVEC3);
}

//=============================================================================
//	メッシュフィールドの終了処理
//=============================================================================
void UninitRiverWall(void)
{
	LPCUSTOMMESH pRiverWall = &g_RiverWall;		// 先頭アドレス

	// テクスチャの破棄
	if (g_pCustomTextureMesh != NULL)
	{
		g_pCustomTextureMesh->Release();
		g_pCustomTextureMesh = NULL;
	}

	// 頂点バッファの破棄
	if (pRiverWall->pVtxBuff != NULL)
	{
		pRiverWall->pVtxBuff->Release();
		pRiverWall->pVtxBuff = NULL;
	}

	// インデックスバッファの破棄
	if (pRiverWall->pIdxBuff != NULL)
	{
		pRiverWall->pIdxBuff->Release();
		pRiverWall->pIdxBuff = NULL;
	}
}

//=============================================================================
//	メッシュフィールドの描画処理
//=============================================================================
void DrawRiverWall(void)
{
	LPCUSTOMMESH pRiverWall = &g_RiverWall;	// 先頭アドレス

	LPDIRECT3DDEVICE9 pDevice = GetDevice();	// デバイスの取得
	D3DXMATRIX mtxRot, mtxTrans;				// 計算用マトリックス

	if (pRiverWall->bUse == false)
	{// 使っていなければ弾く
		return;
	}

	// ワールドマトリックスの初期化
	D3DXMatrixIdentity(&pRiverWall->mtxWorld);

	// 向きを反映
	D3DXMatrixRotationYawPitchRoll(&mtxRot, pRiverWall->rot.y, pRiverWall->rot.x, pRiverWall->rot.z);
	D3DXMatrixMultiply(&pRiverWall->mtxWorld, &pRiverWall->mtxWorld, &mtxRot);

	// 位置を反映
	D3DXMatrixTranslation(&mtxTrans, pRiverWall->pos.x, pRiverWall->pos.y, pRiverWall->pos.z);
	D3DXMatrixMultiply(&pRiverWall->mtxWorld, &pRiverWall->mtxWorld, &mtxTrans);

	// ワールドマトリックスの設定
	pDevice->SetTransform(D3DTS_WORLD, &pRiverWall->mtxWorld);

	// 頂点バッファをデータストリームに設定
	pDevice->SetStreamSource(0, pRiverWall->pVtxBuff, 0, sizeof(VERTEX_3D));

	// インデックスバッファをデータストリームに設定
	pDevice->SetIndices(pRiverWall->pIdxBuff);

	// 頂点フォーマットの設定
	pDevice->SetFVF(FVF_VERTEX_3D);

	// テクスチャの設定
	pDevice->SetTexture(0, g_pCustomTextureMesh);

	// メッシュフィールドの描画
	pDevice->DrawIndexedPrimitive(D3DPT_TRIANGLESTRIP,
		0,
		0,
		pRiverWall->nNumVtx,
		0,
		pRiverWall->nNumIdx - 2);
}

//=============================================================================
//	メッシュフィールドの更新処理
//=============================================================================
void UpdateRiverWall(void)
{

}

//=============================================================================
//	壁の設定処理
//=============================================================================
void SetRiverWall(D3DXVECTOR3 pos, D3DXVECTOR3 rot, D3DXVECTOR3* pVtxPos, int nNumVtx, int* pIdxInfo, int nNumIdx)
{
	// デバイスの取得
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	LPCUSTOMMESH pRiverWall = &g_RiverWall;		// 壁へのポインタ

	if (pRiverWall->bUse == true)
	{// 使っていたら弾く
		return;
	}

	// 壁の設定
	pRiverWall->pos = pos;
	pRiverWall->rot = rot;
	pRiverWall->tex = TEX_DEFAULT;
	pRiverWall->type = MESHFIELDTYPE_000;
	pRiverWall->nNumVtx = nNumVtx;
	pRiverWall->nNumIdx = nNumIdx;

	// 頂点バッファの生成
	pDevice->CreateVertexBuffer(sizeof(VERTEX_3D) * nNumVtx,
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_3D_MULTI,
		D3DPOOL_MANAGED,
		&pRiverWall->pVtxBuff,
		NULL);

	// 初期化
	VERTEX_3D* pVtx;			// 頂点情報へのポインタ

	// 頂点バッファをロックし,頂点情報へのポインタを取得
	pRiverWall->pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	for (int nCntVtx = 0; nCntVtx < nNumVtx; nCntVtx++, pVtxPos++)
	{// 頂点情報を設定
		pVtx[nCntVtx].pos = *pVtxPos;
		D3DXVec3Normalize(&pVtx[nCntVtx].nor, pVtxPos);
		pVtx[nCntVtx].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);

		// (メッシュ構造を上下にちょうど割ってるので半々でテクスチャ設定)
		if (nCntVtx < nNumVtx / 2)
		{
			pVtx[nCntVtx].tex = D3DXVECTOR2(1.0f * nCntVtx, 0.0f);
		}
		else
		{
			pVtx[nCntVtx].tex = D3DXVECTOR2(1.0f * (nCntVtx - nNumVtx / 2), 1.0f);
		}
	}

	// 頂点バッファをアンロックする
	pRiverWall->pVtxBuff->Unlock();

	// インデックスバッファの設定
	pDevice->CreateIndexBuffer(sizeof(WORD) * nNumIdx,
		D3DUSAGE_WRITEONLY,
		D3DFMT_INDEX16,
		D3DPOOL_MANAGED,
		&pRiverWall->pIdxBuff,
		NULL);

	WORD* pIdx;		// インデックス情報へのポインタ

	// インデックスバッファをロックし、頂点情報へのポインタを取得
	pRiverWall->pIdxBuff->Lock(0, 0, (void**)&pIdx, 0);

	for (int nCntIdx = 0; nCntIdx < nNumIdx; nCntIdx++)
	{// インデックスを指定
		pRiverWall->nIdx[nCntIdx] = *pIdxInfo;
		pIdx[nCntIdx] = pRiverWall->nIdx[nCntIdx];

		pIdxInfo++;
	}

	// インデックスバッファをアンロックする
	pRiverWall->pIdxBuff->Unlock();

	pRiverWall->bUse = true;
}

//=============================================================================
//	メッシュの読み込み処理
//=============================================================================
void LoadRiverWall(const char* pFileName, D3DXVECTOR3 pos, D3DXVECTOR3 rot)
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

	// メッシュを設定
	SetRiverWall(pos, rot, &VtxPos[0], nNumVtx, &aIdx[0], nNumIdx);
}