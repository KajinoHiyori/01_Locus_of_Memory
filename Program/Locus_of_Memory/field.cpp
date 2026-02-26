//=============================================================================
//
//	フィールド処理 [field.cpp]
//	Author : SHUMA AIZU
// 
//=============================================================================

#include "main.h"
#include "field.h"
#include "input.h"

#include "debugproc.h"

//*****************************************************************************
// マクロ定義
//*****************************************************************************
#define FIELD_SPLIT_WIDHT		(30 + 1)						// 横の分割数
#define FIELD_SPLIT_DEPTH		(30 + 1)						// 縦の分割数
#define MAX_FIELD				(5)								// フィールドの最大数
#define TEX_SPLIT				(10)							// テクスチャの分割数
#define TEX_DEFAULT				(D3DXVECTOR2(1.0f, 1.0f));		// テクスチャの初期位置

#define MAX_FIELDTEX			(3)								// テクスチャ数の最大

#define VTX_MIN		(D3DXVECTOR3(10000.0f, 10000.0f, 10000.0f))		// オブジェクトの大きさの初期化値(最小)
#define VTX_MAX		(D3DXVECTOR3(-10000.0f, -10000.0f, -10000.0f))	// オブジェクトの大きさの初期化値(最大)

//*****************************************************************************
// グローバル変数
//*****************************************************************************
LPDIRECT3DTEXTURE9 g_apFieldTexture[MAX_FIELDTEX] = {};	// テクスチャへのポインタ
Field g_aField[MAX_FIELD];								// フィールドの情報

const char* c_pFieldTextureName[MAX_FIELDTEX] =
{
	"data\\TEXTURE\\road000.jpg",
	"data\\TEXTURE\\road000.jpg",
	"data\\TEXTURE\\road000.jpg",
};

//=============================================================================
//	メッシュフィールドの初期化処理
//=============================================================================
void InitField(void)
{
	// デバイスの取得
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	LPFIELD pField = &g_aField[0];	// 先頭アドレス

	// テクスチャの読み込み
	for (int nCntTex = 0; nCntTex < MAX_FIELDTEX; nCntTex++)
	{
		D3DXCreateTextureFromFile(pDevice,
			c_pFieldTextureName[nCntTex],
			&g_apFieldTexture[nCntTex]);
	}

	// 数分初期化(後でmemsetに変えてもいいかも)
	for (int nCntField = 0; nCntField < MAX_FIELD; nCntField++, pField++)
	{
		pField->pVtxBuff = NULL;
		pField->pIdxBuff = NULL;
		pField->pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		pField->rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		pField->bUse = false;
	}
}

//=============================================================================
//	メッシュフィールドの終了処理
//=============================================================================
void UninitField(void)
{
	LPFIELD pField = &g_aField[0];		// 先頭アドレス

	// テクスチャの破棄
	for (int nCntTex = 0; nCntTex < MAX_FIELDTEX; nCntTex++)
	{
		if (g_apFieldTexture[nCntTex] != NULL)
		{
			g_apFieldTexture[nCntTex]->Release();
			g_apFieldTexture[nCntTex] = NULL;
		}
	}

	for (int nCntField = 0; nCntField < MAX_FIELD; nCntField++, pField++)
	{
		// 頂点バッファの破棄
		if (pField->pVtxBuff != NULL)
		{
			pField->pVtxBuff->Release();
			pField->pVtxBuff = NULL;
		}

		// インデックスバッファの破棄
		if (pField->pIdxBuff != NULL)
		{
			pField->pIdxBuff->Release();
			pField->pIdxBuff = NULL;
		}
	}
}

//=============================================================================
//	メッシュフィールドの描画処理
//=============================================================================
void DrawField(void)
{
	LPFIELD pField = &g_aField[0];	// 先頭アドレス

	LPDIRECT3DDEVICE9 pDevice = GetDevice();	// デバイスの取得
	D3DXMATRIX mtxRot, mtxTrans;				// 計算用マトリックス

	for (int nCntField = 0; nCntField < MAX_FIELD; nCntField++, pField++)
	{
		if (pField->bUse == false)
		{// 使っていなければ弾く
			continue;
		}

		// ワールドマトリックスの初期化
		D3DXMatrixIdentity(&pField->mtxWorld);

		// 向きを反映
		D3DXMatrixRotationYawPitchRoll(&mtxRot, pField->rot.y, pField->rot.x, pField->rot.z);
		D3DXMatrixMultiply(&pField->mtxWorld, &pField->mtxWorld, &mtxRot);

		// 位置を反映
		D3DXMatrixTranslation(&mtxTrans, pField->pos.x, pField->pos.y, pField->pos.z);
		D3DXMatrixMultiply(&pField->mtxWorld, &pField->mtxWorld, &mtxTrans);

		// ワールドマトリックスの設定
		pDevice->SetTransform(D3DTS_WORLD, &pField->mtxWorld);

		// 頂点バッファをデータストリームに設定
		pDevice->SetStreamSource(0, pField->pVtxBuff, 0, sizeof(VERTEX_3D_MULTI));

		// インデックスバッファをデータストリームに設定
		pDevice->SetIndices(pField->pIdxBuff);

		// 頂点フォーマットの設定
		pDevice->SetFVF(FVF_VERTEX_3D_MULTI);

		// テクスチャの設定
		pDevice->SetTexture(0, g_apFieldTexture[0]);

		// メッシュフィールドの描画
		pDevice->DrawIndexedPrimitive(D3DPT_TRIANGLESTRIP,
			0,
			0,
			pField->nNumVtx,
			0,
			pField->nNumIdx - 2);
	}
}

//=============================================================================
//	メッシュフィールドの更新処理
//=============================================================================
void UpdateField(void)
{
	LPFIELD pField = &g_aField[0];		// フィールドへのポインタ

	// 初期化
	VERTEX_3D_MULTI* pVtx;			// 頂点情報へのポインタ

	for (int nCntField = 0; nCntField < MAX_FIELD; nCntField++)
	{
		if (pField->bUse == false)
		{// 使っていなければ弾く
			continue;
		}

		// 頂点バッファをロックし,頂点情報へのポインタを取得
		pField->pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

		for (int nCntField = 0; nCntField < MAX_FIELD; nCntField++)
		{
			for (int nCntVtx = 0; nCntVtx < pField->nNumVtx; nCntVtx++)
			{
				pVtx[nCntVtx].nor = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
				pVtx[nCntVtx].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);

				// 全体の大きさから現在頂点の位置する割合を算出
				float fDestX = pVtx[nCntVtx].pos.x / (pField->fWidth);
				float fDestZ = pVtx[nCntVtx].pos.z / (pField->fDepth);

				// 対応したテクスチャ座標を設定
				pVtx[nCntVtx].tex = D3DXVECTOR2(fDestX * TEX_SPLIT, -fDestZ * TEX_SPLIT);
				pVtx[nCntVtx].texM = D3DXVECTOR2(fDestX * TEX_SPLIT + pField->tex.x, -fDestZ * TEX_SPLIT + pField->tex.y);
			}
		}

		// 頂点バッファをアンロックする
		pField->pVtxBuff->Unlock();
	}
}

//=============================================================================
//	フィールドの設定処理
//=============================================================================
void SetFieldMesh(D3DXVECTOR3 pos, D3DXVECTOR3 rot, D3DXVECTOR3* pVtxPos, int nNumVtx, int* pIdxInfo, int nNumIdx)
{
	// デバイスの取得
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	LPFIELD pField = &g_aField[0];		// フィールドへのポインタ

	for (int nCntField = 0; nCntField < MAX_FIELD; nCntField++, pField++)
	{
		if (pField->bUse == true)
		{// 使っていたら弾く
			continue;
		}

		D3DXVECTOR3 vtxMin = VTX_MIN;	// 最大値
		D3DXVECTOR3 vtxMax = VTX_MAX;	// 最小値

		// フィールドの設定
		pField->pos = pos;
		pField->rot = rot;
		pField->tex = TEX_DEFAULT;
		pField->nNumVtx = nNumVtx;
		pField->nNumIdx = nNumIdx;

		// 頂点バッファの生成
		pDevice->CreateVertexBuffer(sizeof(VERTEX_3D_MULTI) * nNumVtx,
			D3DUSAGE_WRITEONLY,
			FVF_VERTEX_3D_MULTI,
			D3DPOOL_MANAGED,
			&pField->pVtxBuff,
			NULL);

		// 初期化
		VERTEX_3D_MULTI* pVtx;			// 頂点情報へのポインタ

		// 頂点バッファをロックし,頂点情報へのポインタを取得
		pField->pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

		for (int nCntVtx = 0; nCntVtx < nNumVtx; nCntVtx++)
		{// 頂点情報を設定
			pVtx[nCntVtx].pos = *pVtxPos;
			D3DXVec3Normalize(&pVtx[nCntVtx].nor, pVtxPos);
			pVtx[nCntVtx].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
			pVtx[nCntVtx].tex = D3DXVECTOR2(1.0f * nCntVtx, 1.0f * nCntVtx);
			pVtx[nCntVtx].texM = D3DXVECTOR2(1.0f * nCntVtx, 1.0f * nCntVtx);

			D3DXVECTOR3 vtx = *pVtxPos;

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

			pVtxPos++;
		}

		// 全体の大きさを出す
		pField->fWidth = vtxMax.x - vtxMin.x;
		pField->fHeigth = vtxMax.y - vtxMin.y;
		pField->fDepth = vtxMax.z - vtxMin.z;

		// 頂点バッファをアンロックする
		pField->pVtxBuff->Unlock();

		// インデックスバッファの設定
		pDevice->CreateIndexBuffer(sizeof(WORD) * nNumIdx,
			D3DUSAGE_WRITEONLY,
			D3DFMT_INDEX16,
			D3DPOOL_MANAGED,
			&pField->pIdxBuff,
			NULL);

		WORD* pIdx;		// インデックス情報へのポインタ

		// インデックスバッファをロックし、頂点情報へのポインタを取得
		pField->pIdxBuff->Lock(0, 0, (void**)&pIdx, 0);

		for (int nCntIdx = 0; nCntIdx < nNumIdx; nCntIdx++)
		{// インデックスを指定
			pField->nIdx[nCntIdx] = *pIdxInfo;
			pIdx[nCntIdx] = pField->nIdx[nCntIdx];

			pIdxInfo++;
		}

		// インデックスバッファをアンロックする
		pField->pIdxBuff->Unlock();

		pField->bUse = true;
		break;

	}
}

//=============================================================================
//	メッシュの読み込み処理
//=============================================================================
void LoadFieldMesh(const char* pFileName, D3DXVECTOR3 pos, D3DXVECTOR3 rot)
{
	FILE* pFile = fopen(pFileName, "rb");		// 引数のファイルを開く

	if (pFile == NULL)
	{// 開けなかったら終了
		return;
	}

	// 各値格納用ローカル変数
	int nNumVtx = 0;
	D3DXVECTOR3 VtxPos[MAX_FIELDVTX] = {};
	int nNumIdx = 0;
	int aIdx[MAX_FIELDIDX] = {};

	// ファイルから取得した情報を格納
	fread(&nNumVtx, sizeof(int), 1, pFile);
	fread(&VtxPos[0], sizeof(D3DXVECTOR3), nNumVtx, pFile);
	fread(&nNumIdx, sizeof(int), 1, pFile);
	fread(&aIdx[0], sizeof(int), nNumIdx, pFile);

	fclose(pFile);

	// メッシュを設定
	SetFieldMesh(pos, rot, &VtxPos[0], nNumVtx, &aIdx[0], nNumIdx);
}