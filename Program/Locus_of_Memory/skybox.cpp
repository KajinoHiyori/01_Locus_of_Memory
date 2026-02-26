//=============================================================================
//
//	スカイボックス処理 [skybox.cpp]
//	Author : SHUMA AIZU
// 
//=============================================================================

#include "main.h"
#include "skybox.h"
#include "input.h"
#include "color.h"
#include "clock.h"
#include "debugproc.h"

//*****************************************************************************
// マクロ定義
//*****************************************************************************
#define MAX_SKYBOX			(1)							// スカイボックスの最大数
#define SKYBOX_VERTICAL		(64 - 1)					// 縦の分割数
#define SKYBOX_HORIZONTAL	(64 - 1)					// 横の分割数
#define SKYBOX_RADIUS		(10000.0f)					// 半径
#define MOVE_SKY			(0.00025f)					// 空の移動量

//*****************************************************************************
// グローバル変数
//*****************************************************************************
LPDIRECT3DTEXTURE9 g_pTextureSkyBox = NULL;				// テクスチャへのポインタ
SkyBox g_aSkyBox[MAX_SKYBOX];							// スカイボックスの情報
D3DXCOLOR g_col, g_colNext;	// 色を管理

//=============================================================================
//	スカイボックスの初期化処理
//=============================================================================
void InitSkyBox(void)
{
	// デバイスの取得
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	SkyBox* pSkyBox = &g_aSkyBox[0];

	// テクスチャの読み込み
	D3DXCreateTextureFromFile(pDevice,
		"data\\TEXTURE\\sky001.jpg",
		&g_pTextureSkyBox);

	memset(pSkyBox, NULL, sizeof(SkyBox) * MAX_SKYBOX);

	SetSkyBox(D3DXVECTOR3(0.0f, -50.0f, 0.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f), SKYBOX_RADIUS, SKYBOX_VERTICAL, SKYBOX_HORIZONTAL);

	// 色の初期化
	g_col = COLOR_WHITE;
	g_colNext = COLOR_WHITE;
}

//=============================================================================
//	スカイボックスの終了処理
//=============================================================================
void UninitSkyBox(void)
{
	// テクスチャの破棄
	if (g_pTextureSkyBox != NULL)
	{
		g_pTextureSkyBox->Release();
		g_pTextureSkyBox = NULL;
	}
	
	for (int nCntSkyBox = 0; nCntSkyBox < MAX_SKYBOX; nCntSkyBox++)
	{
		// 頂点バッファの破棄
		if (g_aSkyBox[nCntSkyBox].pVtxBuff != NULL)
		{
			g_aSkyBox[nCntSkyBox].pVtxBuff->Release();
			g_aSkyBox[nCntSkyBox].pVtxBuff = NULL;
		}

		// インデックスバッファの破棄
		if (g_aSkyBox[nCntSkyBox].pIdxBuff != NULL)
		{
			g_aSkyBox[nCntSkyBox].pIdxBuff->Release();
			g_aSkyBox[nCntSkyBox].pIdxBuff = NULL;
		}
	}
}

//=============================================================================
//	スカイボックスの描画処理
//=============================================================================
void DrawSkyBox(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();	// デバイスの取得
	D3DXMATRIX mtxRot, mtxTrans;				// 計算用マトリックス

	SkyBox* pSkyBox = &g_aSkyBox[0];

	pDevice->SetRenderState(D3DRS_LIGHTING, FALSE);		// ライトを無効にする

	// アルファテストを有効にする
	pDevice->SetRenderState(D3DRS_ALPHATESTENABLE, TRUE);		// アルファテストを有効にする
	pDevice->SetRenderState(D3DRS_ALPHAFUNC, D3DCMP_GREATER);	// 比較方法(基準値より大きければ描画)
	pDevice->SetRenderState(D3DRS_ALPHAREF, 0);					// アルファテストの参照値を設定(～以上で描画, intで設定)

	for (int nCntSkyBox = 0; nCntSkyBox < MAX_SKYBOX; nCntSkyBox++, pSkyBox++)
	{
		// ワールドマトリックスの初期化
		D3DXMatrixIdentity(&g_aSkyBox[nCntSkyBox].mtxWorld);

		// 向きを反映
		D3DXMatrixRotationYawPitchRoll(&mtxRot, pSkyBox->rot.y, pSkyBox->rot.x, pSkyBox->rot.z);
		D3DXMatrixMultiply(&pSkyBox->mtxWorld, &pSkyBox->mtxWorld, &mtxRot);

		// 位置を反映
		D3DXMatrixTranslation(&mtxTrans, pSkyBox->pos.x, pSkyBox->pos.y, pSkyBox->pos.z);
		D3DXMatrixMultiply(&pSkyBox->mtxWorld, &pSkyBox->mtxWorld, &mtxTrans);

		// ワールドマトリックスの設定
		pDevice->SetTransform(D3DTS_WORLD, &pSkyBox->mtxWorld);

		// 頂点バッファをデータストリームに設定
		pDevice->SetStreamSource(0, pSkyBox->pVtxBuff, 0, sizeof(VERTEX_3D));

		// インデックスバッファをデータストリームに設定
		pDevice->SetIndices(pSkyBox->pIdxBuff);

		// 頂点フォーマットの設定
		pDevice->SetFVF(FVF_VERTEX_3D);

		// テクスチャの設定
		pDevice->SetTexture(0, g_pTextureSkyBox);

		if (pSkyBox->bUse == true)
		{
			// スカイボックスの描画
			pDevice->DrawIndexedPrimitive(D3DPT_TRIANGLESTRIP,
				0,
				0,
				(pSkyBox->nVertical - 1) * pSkyBox->nHorizontal,
				0,
				((pSkyBox->nHorizontal - 1) * (pSkyBox->nVertical - 2) * 2) + ((pSkyBox->nVertical - 3) * 4));

		}
	}

	pDevice->SetRenderState(D3DRS_LIGHTING, TRUE);		// ライトを有効にする

	// アルファテストを無効にする
	pDevice->SetRenderState(D3DRS_ALPHATESTENABLE, FALSE);		// アルファテストを無効化
	pDevice->SetRenderState(D3DRS_ALPHAFUNC, D3DCMP_ALWAYS);	// 比較方法(すべて描画)
	pDevice->SetRenderState(D3DRS_ALPHAREF, 255);				// 基準値を設定(すべてを描画している)
}

//=============================================================================
//	スカイボックスの更新処理
//=============================================================================
void UpdateSkyBox(void)
{
	SkyBox* pSkyBox = &g_aSkyBox[0];

	for (int nCntSkyBox = 0; nCntSkyBox < MAX_SKYBOX; nCntSkyBox++, nCntSkyBox++)
	{
		if (pSkyBox->bUse == false)
		{
			continue;
		}

		pSkyBox->tex.x += MOVE_SKY;

		VERTEX_3D* pVtx;			// 頂点情報へのポインタ

		// 頂点バッファをロックし,頂点情報へのポインタを取得
		pSkyBox->pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

		float fAngleVertical = (D3DX_PI * 0.5f) - (D3DX_PI / pSkyBox->nVertical);

		for (int nCntVtxVertical = 0; nCntVtxVertical < pSkyBox->nVertical - 1; nCntVtxVertical++)
		{
			for (int nCntVtxHorizontal = 0; nCntVtxHorizontal < pSkyBox->nHorizontal; nCntVtxHorizontal++)
			{
				// テクスチャ座標の設定
				pVtx[nCntVtxHorizontal].tex = D3DXVECTOR2((1.0f / (pSkyBox->nHorizontal / 2)) * nCntVtxHorizontal + pSkyBox->tex.x, (pSkyBox->tex.y / pSkyBox->nVertical) * nCntVtxVertical);
				pVtx[nCntVtxHorizontal].col = ChangeSkyColor();
			}

			// ポインタを進める
			pVtx += pSkyBox->nHorizontal;
		}

		// 頂点バッファをアンロックする
		pSkyBox->pVtxBuff->Unlock();
	}
}

//=============================================================================
//	スカイボックスの設定処理
//=============================================================================
void SetSkyBox(D3DXVECTOR3 pos, D3DXVECTOR3 rot, float fRadius, int nVertical, int nHorizontal)
{
	// デバイスの取得
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	SkyBox* pSkyBox = &g_aSkyBox[0];

	for (int nCntSkyBox = 0; nCntSkyBox < MAX_SKYBOX; nCntSkyBox++, pSkyBox++)
	{
		if (pSkyBox->bUse == true)
		{
			continue;
		}

		// 分割数を頂点数に合わせる
		nVertical++;
		nHorizontal++;

		// スカイボックスの設定
		pSkyBox->pos = pos;
		pSkyBox->rot = rot;
		pSkyBox->tex = D3DXVECTOR2(1.0f, 1.0f);
		pSkyBox->fRadius = fRadius;
		pSkyBox->nVertical = nVertical;
		pSkyBox->nHorizontal = nHorizontal;

		// 頂点バッファの生成
		pDevice->CreateVertexBuffer(sizeof(VERTEX_3D) * (nVertical - 1) * nHorizontal,
			D3DUSAGE_WRITEONLY,
			FVF_VERTEX_3D,
			D3DPOOL_MANAGED,
			&pSkyBox->pVtxBuff,
			NULL);

		// 初期化
		VERTEX_3D* pVtx;			// 頂点情報へのポインタ

		// 頂点バッファをロックし,頂点情報へのポインタを取得
		pSkyBox->pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

		float fAngleVertical = (D3DX_PI * 0.5f) - (D3DX_PI / nVertical);

		for (int nCntVtxVertical = 0; nCntVtxVertical < nVertical - 1; nCntVtxVertical++)
		{
			D3DXVECTOR3 diff;
			D3DXVECTOR3 vecDir;
			float fRadius;
			float fAngleHorizontal = -D3DX_PI;

			diff = D3DXVECTOR3(sinf(0.0f) * pSkyBox->fRadius, cosf(fAngleVertical) * pSkyBox->fRadius, cosf(0.0f) * pSkyBox->fRadius);

			fRadius = SQRTF(diff.x, diff.y);

			for (int nCntVtxHorizontal = 0; nCntVtxHorizontal < nHorizontal; nCntVtxHorizontal++)
			{
				// 頂点座標の設定
				pVtx[nCntVtxHorizontal].pos = D3DXVECTOR3(sinf(fAngleHorizontal) * fRadius, sinf(fAngleVertical) * pSkyBox->fRadius, cosf(fAngleHorizontal) * fRadius);

				// 法線ベクトルの設定
				vecDir = D3DXVECTOR3(pVtx[nCntVtxHorizontal].pos.x, 0.0f, pVtx[nCntVtxHorizontal].pos.z);
				D3DXVec3Normalize(&vecDir, &vecDir);
				pVtx[nCntVtxHorizontal].nor = vecDir;

				// 頂点カラーの設定
				pVtx[nCntVtxHorizontal].col = ChangeSkyColor();

				// テクスチャ座標の設定
				pVtx[nCntVtxHorizontal].tex = D3DXVECTOR2((1.0f / (nHorizontal / 2)) * nCntVtxHorizontal, (1.0f / nVertical) * nCntVtxVertical);

				// 角度を加算
				fAngleHorizontal += (D3DX_PI / (nHorizontal - 1)) * 2.0f;	// 合計6.28加算したいので倍にする
			}

			// 角度を減算
			fAngleVertical += -(D3DX_PI / nVertical) * 0.5f;			// 半円で留めたいので半分にして減算

			// ポインタを進める
			pVtx += nHorizontal;
		}

		// 頂点バッファをアンロックする
		pSkyBox->pVtxBuff->Unlock();

		// インデックスバッファの設定
		pDevice->CreateIndexBuffer(sizeof(WORD) * ((nHorizontal) * 2 * (nVertical - 2) + ((nVertical - 3) * 2)),
			D3DUSAGE_WRITEONLY,
			D3DFMT_INDEX16,
			D3DPOOL_MANAGED,
			&pSkyBox->pIdxBuff,
			NULL);

		WORD* pIdx;		// インデックス情報へのポインタ

		// インデックスバッファをロックし、頂点情報へのポインタを取得
		pSkyBox->pIdxBuff->Lock(0, 0, (void**)&pIdx, 0);

		for (int nCntIdxVertical = 0; nCntIdxVertical < nVertical - 2; nCntIdxVertical++)
		{
			for (int nCntIdxHorizontal = 0; nCntIdxHorizontal < nHorizontal; nCntIdxHorizontal++)
			{
				if (nCntIdxVertical != 0 && nCntIdxHorizontal == 0)
				{// 高さが最初の行以外かつ幅が最初の行
					pIdx[0] = (nHorizontal * (nCntIdxVertical + 1)) + nCntIdxHorizontal;

					pIdx++;
				}

				pIdx[0] = (nHorizontal * (nCntIdxVertical + 1)) + nCntIdxHorizontal;
				pIdx[1] = nHorizontal * nCntIdxVertical + nCntIdxHorizontal;

				pIdx += 2;

				if (nCntIdxVertical != nHorizontal - 2 && nCntIdxHorizontal == nHorizontal - 1)
				{// 高さが最後の行以外かつ幅が最後の行
					pIdx[0] = nHorizontal * nCntIdxVertical + nCntIdxHorizontal;

					pIdx++;
				}
			}
		}

		// インデックスバッファをアンロックする
		pSkyBox->pIdxBuff->Unlock();

		pSkyBox->bUse = true;
		break;
	}
}

//=============================================================================
//	スカイボックスの色を変える
//=============================================================================
D3DXCOLOR ChangeSkyColor(void)
{
	int nTime = GetTime();
	MODE mode = GetMode();
	D3DXCOLOR colLocal = COLOR_WHITE;	// ローカルで色の値を保存

	switch (mode)
	{
	case MODE_TITLE:	// タイトル
		colLocal = COLOR_SKY_NIGHT;	// 夜空
		break;

	case MODE_TUTORIAL:	// チュートリアル
		colLocal = COLOR_SKY_NOON;	// 青空
		break;

	case MODE_GAME:	// ゲーム
		if (nTime >= 0 && nTime <= 559)	// 0:00~5:59
		{
			colLocal = COLOR_SKY007;
		}
		else if (nTime >= 600 && nTime <= 759)	// 6:00~7:59
		{
			colLocal = COLOR_SKY000;
		}
		else if (nTime >= 800 && nTime <= 1059)	// 8:00~10:59
		{
			colLocal = COLOR_SKY000;
		}
		else if (nTime >= 1100 && nTime <= 1359)	// 11:00~13:59
		{
			colLocal = COLOR_SKY001;
		}
		else if (nTime >= 1400 && nTime <= 1559)	// 14:00~15:59
		{
			colLocal = COLOR_SKY002;
		}
		else if (nTime >= 1600 && nTime <= 1659)	// 16:00~16:59
		{
			colLocal = COLOR_SKY003;
		}
		else if (nTime >= 1700 && nTime <= 1759)	// 17:00~17:59
		{
			colLocal = COLOR_SKY004;
		}
		else if (nTime >= 1800 && nTime <= 1859)	// 18:00~18:59
		{
			colLocal = COLOR_SKY005;
		}
		else if (nTime >= 1900 && nTime <= 1959)	// 19:00~19:59
		{
			colLocal = COLOR_SKY006;
		}
		else if (nTime >= 2000)	// 20:00以降
		{
			colLocal = COLOR_SKY007;
		}
		break;

	case MODE_RESULT:	// リザルト
		colLocal = COLOR_SKY_NIGHT;	// 夜空
		break;

	case MODE_DIAGNOSIS:	// 診断結果
		colLocal = COLOR_SKY_NIGHT;	// 夜空
		break;

	case MODE_START:	// ロゴ画面
		colLocal = COLOR_SKY_NOON;	// 青空
		break;

	default:	// エラー
		colLocal = COLOR_SKY_NOON;
		break;
	}



	return colLocal;
}