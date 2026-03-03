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
#define MESHFIELD_SPLIT_WIDHT		(30 + 1)						// 横の分割数
#define MESHFIELD_SPLIT_DEPTH		(30 + 1)						// 縦の分割数
#define MAX_CUSTOMMESH				(5)								// カスタムメッシュの最大数
#define TEX_SPLIT					(10)							// テクスチャの分割数
#define TEX_DEFAULT					(D3DXVECTOR2(1.0f, 1.0f));		// テクスチャの初期位置

#define RIVER_MOVE					(0.0001f)						// 川のテクスチャの流れる速度

#define VTX_MIN		(D3DXVECTOR3(10000.0f, 10000.0f, 10000.0f))		// オブジェクトの大きさの初期化値(最小)
#define VTX_MAX		(D3DXVECTOR3(-10000.0f, -10000.0f, -10000.0f))	// オブジェクトの大きさの初期化値(最大)

//*****************************************************************************
// グローバル変数
//*****************************************************************************
LPDIRECT3DTEXTURE9 g_apCustomTextureMesh[MESHFIELDTYPE_MAX] = {};	// テクスチャへのポインタ
CustomMesh g_aCustomMesh[MAX_CUSTOMMESH];							// カスタムメッシュの情報

const char* c_pCustomMeshTextureName[MESHFIELDTYPE_MAX] =
{
	"data\\TEXTURE\\river000.jpg",
	"data\\TEXTURE\\river000.png",
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
		pDevice->SetStreamSource(0, pCustomMesh->pVtxBuff, 0, sizeof(VERTEX_3D_MULTI));

		// インデックスバッファをデータストリームに設定
		pDevice->SetIndices(pCustomMesh->pIdxBuff);

		// 頂点フォーマットの設定
		pDevice->SetFVF(FVF_VERTEX_3D_MULTI);

		// マルチテクスチャの方(第1引数, nIdx 1)のテクスチャ色(第2引数)で
		// 今描画してる色(第4引数, Idx 0のポリゴン色 * テクスチャ色)にアルファブレンド(第3引数)
		SetTextureStageStateColor(1, D3DTA_TEXTURE, D3DTOP_BLENDTEXTUREALPHA, D3DTA_CURRENT);

		// テクスチャの設定
		pDevice->SetTexture(0, g_apCustomTextureMesh[0]);
		pDevice->SetTexture(1, g_apCustomTextureMesh[1]);

		// メッシュフィールドの描画
		pDevice->DrawIndexedPrimitive(D3DPT_TRIANGLESTRIP,
			0,
			0,
			pCustomMesh->nNumVtx,
			0,
			pCustomMesh->nNumIdx - 2);

		// 基準のステートに戻す
		ResetTextureStageStateColor(2);

	}
}

//=============================================================================
//	メッシュフィールドの更新処理
//=============================================================================
void UpdateCustomMesh(void)
{
	LPCUSTOMMESH pCustomMesh = &g_aCustomMesh[0];		// カスタムメッシュへのポインタ

	// 初期化
	VERTEX_3D_MULTI* pVtx;			// 頂点情報へのポインタ

	for (int nCntCustomMesh = 0; nCntCustomMesh < MAX_CUSTOMMESH; nCntCustomMesh++)
	{
		if (pCustomMesh->bUse == false)
		{// 使っていなければ弾く
			continue;
		}

		// 川のテクスチャ移動量を加算
		pCustomMesh->tex.x += -RIVER_MOVE;
		pCustomMesh->tex.y += RIVER_MOVE;

		// 頂点バッファをロックし,頂点情報へのポインタを取得
		pCustomMesh->pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

		for (int nCntVtx = 0; nCntVtx < pCustomMesh->nNumVtx; nCntVtx++)
		{
			pVtx[nCntVtx].nor = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
			pVtx[nCntVtx].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);

			// 全体の大きさから現在頂点の位置する割合を算出
			float fDestX = pVtx[nCntVtx].pos.x / (pCustomMesh->fWidth);
			float fDestZ = pVtx[nCntVtx].pos.z / (pCustomMesh->fDepth);

			// 対応したテクスチャ座標を設定
			pVtx[nCntVtx].tex = D3DXVECTOR2(fDestX * TEX_SPLIT, -fDestZ * TEX_SPLIT);
			pVtx[nCntVtx].texM = D3DXVECTOR2(fDestX * TEX_SPLIT + pCustomMesh->tex.x, -fDestZ * TEX_SPLIT + pCustomMesh->tex.y);
		}

		// 頂点バッファをアンロックする
		pCustomMesh->pVtxBuff->Unlock();
	}
}

//=============================================================================
//	カスタムメッシュの設定処理
//=============================================================================
void SetCustomMesh(D3DXVECTOR3 pos, D3DXVECTOR3 rot, D3DXVECTOR3* pVtxPos, int nNumVtx, int* pIdxInfo, int nNumIdx)
{
	// デバイスの取得
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	LPCUSTOMMESH pCustomMesh = &g_aCustomMesh[0];		// カスタムメッシュへのポインタ

	for (int nCntCustomMesh = 0; nCntCustomMesh < MAX_CUSTOMMESH; nCntCustomMesh++, pCustomMesh++)
	{
		if (pCustomMesh->bUse == true)
		{// 使っていたら弾く
			continue;
		}

		D3DXVECTOR3 vtxMin = VTX_MIN;	// 最大値
		D3DXVECTOR3 vtxMax = VTX_MAX;	// 最小値

		// カスタムメッシュの設定
		pCustomMesh->pos = pos;
		pCustomMesh->rot = rot;
		pCustomMesh->tex = TEX_DEFAULT;
		pCustomMesh->type = MESHFIELDTYPE_000;
		pCustomMesh->nNumVtx = nNumVtx;
		pCustomMesh->nNumIdx = nNumIdx;

		// 頂点バッファの生成
		pDevice->CreateVertexBuffer(sizeof(VERTEX_3D_MULTI) * nNumVtx,
			D3DUSAGE_WRITEONLY,
			FVF_VERTEX_3D_MULTI,
			D3DPOOL_MANAGED,
			&pCustomMesh->pVtxBuff,
			NULL);

		// 初期化
		VERTEX_3D_MULTI* pVtx;			// 頂点情報へのポインタ

		// 頂点バッファをロックし,頂点情報へのポインタを取得
		pCustomMesh->pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

		for (int nCntVtx = 0; nCntVtx < nNumVtx; nCntVtx++)
		{// 頂点情報を設定
			pVtx[nCntVtx].pos = *pVtxPos;
			pVtx[nCntVtx].nor = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
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
		pCustomMesh->fWidth = vtxMax.x - vtxMin.x;
		pCustomMesh->fHeigth = vtxMax.y - vtxMin.y;
		pCustomMesh->fDepth = vtxMax.z - vtxMin.z;

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
		{// インデックスを指定
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

	// メッシュを設定
	SetCustomMesh(pos, rot, &VtxPos[0], nNumVtx, &aIdx[0], nNumIdx);
}