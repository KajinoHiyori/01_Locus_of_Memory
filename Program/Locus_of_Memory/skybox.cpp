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
#define MAX_SKYBOX			(1)				// スカイボックスの最大数
#define SKYBOX_VERTICAL		(64 - 1)		// 縦の分割数
#define SKYBOX_HORIZONTAL	(64 - 1)		// 横の分割数
#define SKYBOX_RADIUS		(10000.0f)		// 半径
#define MOVE_SKY			(0.00025f)		// 空の移動量
#define MAX_MINUTE			(60.0f)			// 最大分数

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
	D3DXCreateTextureFromFile(pDevice, "data\\TEXTURE\\sky_000.png", &g_pTextureSkyBox);

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
		pDevice->SetStreamSource(0, pSkyBox->pVtxBuff, 0, sizeof(VERTEX_3D_MULTI));

		// インデックスバッファをデータストリームに設定
		pDevice->SetIndices(pSkyBox->pIdxBuff);

		// 頂点フォーマットの設定
		pDevice->SetFVF(FVF_VERTEX_3D_MULTI);

		// マルチテクスチャの方(第1引数, nIdx 1)のテクスチャ色(第2引数)で
		// 今描画してる色(第4引数, Idx 0のポリゴン色 * テクスチャ色)にアルファブレンド(第3引数)
		SetTextureStageStateColor(1, D3DTA_TEXTURE, D3DTOP_SELECTARG1, D3DBLENDOP_ADD);

		// テクスチャの設定
		pDevice->SetTexture(0, NULL);
		pDevice->SetTexture(1, g_pTextureSkyBox);
		
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

	ResetTextureStageStateColor(2);
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

		VERTEX_3D_MULTI* pVtx;			// 頂点情報へのポインタ

		// 頂点バッファをロックし,頂点情報へのポインタを取得
		pSkyBox->pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

		float fAngleVertical = (D3DX_PI * 0.5f) - (D3DX_PI / pSkyBox->nVertical);

		for (int nCntVtxVertical = 0; nCntVtxVertical < pSkyBox->nVertical - 1; nCntVtxVertical++)
		{
			for (int nCntVtxHorizontal = 0; nCntVtxHorizontal < pSkyBox->nHorizontal; nCntVtxHorizontal++)
			{
				// テクスチャ座標の設定
				pVtx[nCntVtxHorizontal].tex = D3DXVECTOR2((1.0f / (pSkyBox->nHorizontal / 2)) * nCntVtxHorizontal + pSkyBox->tex.x, (pSkyBox->tex.y / pSkyBox->nVertical) * nCntVtxVertical);
				pVtx[nCntVtxHorizontal].texM = D3DXVECTOR2((1.0f / (pSkyBox->nHorizontal / 2)) * nCntVtxHorizontal + pSkyBox->tex.x, (pSkyBox->tex.y / pSkyBox->nVertical) * nCntVtxVertical);
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
		pDevice->CreateVertexBuffer(sizeof(VERTEX_3D_MULTI) * (nVertical - 1) * nHorizontal,
			D3DUSAGE_WRITEONLY,
			FVF_VERTEX_3D_MULTI,
			D3DPOOL_MANAGED,
			&pSkyBox->pVtxBuff,
			NULL);

		// 初期化
		VERTEX_3D_MULTI* pVtx;			// 頂点情報へのポインタ

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
				pVtx[nCntVtxHorizontal].texM = D3DXVECTOR2((1.0f / (nHorizontal / 2)) * nCntVtxHorizontal, (1.0f / nVertical) * nCntVtxVertical);

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
		colLocal = UpdateSkyColor();
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

//=============================================================================
//	色の差分で変化させる
//=============================================================================
D3DXCOLOR UpdateSkyColor(void)
{
	D3DXCOLOR colLocal = COLOR_WHITE;		// ローカルで色を保存
	int nMinute = GetMinute();				// 時間を取得
	int nTime = GetTime();					// 分を取得
	float fRed, fGreen, fBlue, fDiffCol;	// 色を保存
	float fRateTime = (float)nMinute / MAX_MINUTE;	// 時間の経過割合

	// 経過時間に応じて次の色を指定しなおす
	switch (nTime)
	{
	case 0:	// 0:00
		g_col = COLOR_SKY00;
		g_colNext = COLOR_SKY01;
		break;

	case 100:	// 1:00
		g_col = COLOR_SKY01;
		g_colNext = COLOR_SKY02;
		break;

	case 200:	// 2:00
		g_col = COLOR_SKY02;
		g_colNext = COLOR_SKY03;
		break;

	case 300:	// 3:00
		g_col = COLOR_SKY03;
		g_colNext = COLOR_SKY04;
		break;

	case 400:	// 4:00
		g_col = COLOR_SKY04;
		g_colNext = COLOR_SKY05;
		break;

	case 500:	// 5:00
		g_col = COLOR_SKY05;
		g_colNext = COLOR_SKY06;
		break;

	case 600:	// 6:00
		g_col = COLOR_SKY06;
		g_colNext = COLOR_SKY07;
		break;

	case 700:	// 7:00
		g_col = COLOR_SKY07;
		g_colNext = COLOR_SKY08;
		break;

	case 800:	// 8:00
		g_col = COLOR_SKY08;
		g_colNext = COLOR_SKY09;
		break;

	case 900:	// 9:00
		g_col = COLOR_SKY09;
		g_colNext = COLOR_SKY10;
		break;

	case 1000:	// 10:00
		g_col = COLOR_SKY10;
		g_colNext = COLOR_SKY11;
		break;

	case 1100:	// 11:00
		g_col = COLOR_SKY11;
		g_colNext = COLOR_SKY12;
		break;

	case 1200:	// 12:00
		g_col = COLOR_SKY12;
		g_colNext = COLOR_SKY13;
		break;

	case 1300:	// 13:00
		g_col = COLOR_SKY13;
		g_colNext = COLOR_SKY14;
		break;

	case 1400:	// 14:00
		g_col = COLOR_SKY14;
		g_colNext = COLOR_SKY15;
		break;

	case 1500:	// 15:00
		g_col = COLOR_SKY15;
		g_colNext = COLOR_SKY16;
		break;

	case 1600:	// 16:00
		g_col = COLOR_SKY16;
		g_colNext = COLOR_SKY17;
		break;

	case 1700:	// 17:00
		g_col = COLOR_SKY17;
		g_colNext = COLOR_SKY18;
		break;

	case 1800:	// 18:00
		g_col = COLOR_SKY18;
		g_colNext = COLOR_SKY19;
		break;

	case 1900:	// 19:00
		g_col = COLOR_SKY19;
		g_colNext = COLOR_SKY20;
		break;

	case 2000:	// 20:00
		g_col = COLOR_SKY20;
		g_colNext = COLOR_SKY21;
		break;

	case 2100:	// 21:00
		g_col = COLOR_SKY21;
		g_colNext = COLOR_SKY22;
		break;

	case 2200:	// 22:00
		g_col = COLOR_SKY22;
		g_colNext = COLOR_SKY23;
		break;

	case 2300:	// 23:00
		g_col = COLOR_SKY23;
		g_colNext = COLOR_SKY00;
		break;

	default:
		break;
	}

	// 赤色の管理
	fDiffCol = g_colNext.r - g_col.r;
	fRed = g_col.r + fDiffCol * fRateTime;
	// 緑色の管理
	fDiffCol = g_colNext.g - g_col.g;
	fGreen = g_col.g + fDiffCol * fRateTime;
	// 青色の管理
	fDiffCol = g_colNext.b - g_col.b;
	fBlue = g_col.b + fDiffCol * fRateTime;

	colLocal = D3DXCOLOR(fRed, fGreen, fBlue, 1.0f);

	return colLocal;
}
