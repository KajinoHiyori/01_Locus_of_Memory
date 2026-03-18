//=============================================================================
//
//	画面上の文字の処理 [grain.h]
//	Author : SHUMA AIZU
// 
//=============================================================================

#include"main.h"
#include"grain.h"
#include"input.h"
#include"camera.h"
#include "fog.h"
#include "debugproc.h"
#include "color.h"

//*****************************************************************************
// マクロ定義
//*****************************************************************************
#define MAX_GRAIN			(2048)					// 文字の最大数
#define GRAIN_RADIUS		(25.0f)					// 文字の半径
#define GRAINALPHA_FADE		(0.025f)				// 文字のフェードスピード
#define GRAINTEX_SPRIT		(5)						// テクスチャの一列の分割数
#define GRAINTEX_SPRITPOS	(1.0f / GRAINTEX_SPRIT)	// テクスチャの分割座標
#define MAX_GRAINRADIUS		(150)					// 文字の半径の最大
#define MIN_GRAINRADIUS		(100)					// 文字の半径の最小
#define MAX_GRAINLIFE		(180)					// 文字の寿命の最大
#define MIN_GRAINLIFE		(60)					// 文字の寿命の最小

//*****************************************************************************
// グローバル変数
//*****************************************************************************
LPDIRECT3DTEXTURE9 g_pTextureBuffGrain = {};		// テクスチャへのポインタ
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffGrain = NULL;		// 頂点バッファへのポインタ
LPDIRECT3DINDEXBUFFER9 g_pIdxBuffGrain = NULL;
Grain g_aGrain[MAX_GRAIN];							// 文字の情報
int g_aNumGrainIdx[MAX_GRAIN];						// 使用している文字のインデックス
int g_nNumGrain;									// 使用している文字の数

//======================================================================================
// 文字の初期化処理
//======================================================================================
void InitGrain(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	Grain* pGrain = &g_aGrain[0];

	// テクスチャ読み込み
	D3DXCreateTextureFromFile(pDevice,
		"data/TEXTURE/grain.jpg",
		&g_pTextureBuffGrain);

	// 初期化
	memset(pGrain, NULL, sizeof(Grain) * MAX_GRAIN);
	memset(&g_aNumGrainIdx[0], -1, sizeof(int) * MAX_GRAIN);
	g_nNumGrain = 0;

	// 頂点バッファの生成
	pDevice->CreateVertexBuffer(sizeof(VERTEX_3D) * MAX_GRAIN * 4,
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_3D,
		D3DPOOL_MANAGED,
		&g_pVtxBuffGrain, NULL);

	VERTEX_3D* pVtx;    // 頂点情報の設定

	//頂点バッファをロックし、頂点情報へのポインタを取得
	g_pVtxBuffGrain->Lock(0, 0, (void**)&pVtx, 0);

	for (int nCntGrain = 0; nCntGrain < MAX_GRAIN; nCntGrain++, pGrain++)
	{
		// 頂点座標の設定
		pVtx[0].pos = D3DXVECTOR3(-pGrain->fRadius, pGrain->fRadius, 0.0f);
		pVtx[1].pos = D3DXVECTOR3(pGrain->fRadius, pGrain->fRadius, 0.0f);
		pVtx[2].pos = D3DXVECTOR3(-pGrain->fRadius, -pGrain->fRadius, 0.0f);
		pVtx[3].pos = D3DXVECTOR3(pGrain->fRadius, -pGrain->fRadius, 0.0f);

		// 法線の設定
		pVtx[0].nor = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
		pVtx[1].nor = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
		pVtx[2].nor = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
		pVtx[3].nor = D3DXVECTOR3(0.0f, 1.0f, 0.0f);

		// 頂点カラーの設定
		pVtx[0].col = COLOR_WHITE;
		pVtx[1].col = COLOR_WHITE;
		pVtx[2].col = COLOR_WHITE;
		pVtx[3].col = COLOR_WHITE;

		// テクスチャ座標の設定
		pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
		pVtx[1].tex = D3DXVECTOR2(0.2f, 0.0f);
		pVtx[2].tex = D3DXVECTOR2(0.0f, 0.2f);
		pVtx[3].tex = D3DXVECTOR2(0.2f, 0.2f);

		pVtx += 4;
	}

	g_pVtxBuffGrain->Unlock();

	// インデックスバッファの設定
	pDevice->CreateIndexBuffer(sizeof(WORD) * (MAX_GRAIN * (4 + 2)),
		D3DUSAGE_WRITEONLY,
		D3DFMT_INDEX16,
		D3DPOOL_MANAGED,
		&g_pIdxBuffGrain,
		NULL);

	WORD* pIdx;		// インデックス情報へのポインタ

	// インデックスバッファをロックし、頂点情報へのポインタを取得
	g_pIdxBuffGrain->Lock(0, 0, (void**)&pIdx, 0);

	for (int nCntIdx = 0; nCntIdx < MAX_GRAIN * 4; nCntIdx += 4)
	{
		pIdx[0] = nCntIdx + 2;
		pIdx[1] = nCntIdx;
		pIdx[2] = nCntIdx + 1;
		pIdx[3] = nCntIdx + 2;
		pIdx[4] = nCntIdx + 1;
		pIdx[5] = nCntIdx + 3;

		pIdx += 6;
	}

	// インデックスバッファをアンロックする
	g_pIdxBuffGrain->Unlock();
}

//======================================================================================
// 文字の終了処理
//======================================================================================
void UninitGrain(void)
{
	//テクスチャの破棄
	if (g_pTextureBuffGrain != NULL)
	{
		g_pTextureBuffGrain->Release();
		g_pTextureBuffGrain = NULL;
	}

	//頂点バッファを破棄
	if (g_pVtxBuffGrain != NULL)
	{
		g_pVtxBuffGrain->Release();
		g_pVtxBuffGrain = NULL;
	}

	// インデックスバッファを破棄
	if (g_pIdxBuffGrain != NULL)
	{
		g_pIdxBuffGrain->Release();
		g_pIdxBuffGrain = NULL;
	}
}

//======================================================================================
// 文字の更新処理
//======================================================================================
void UpdateGrain(void)
{
	Grain* pGrain = &g_aGrain[0];					// 文字へのポインタ
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	for (; g_nNumGrain < MAX_GRAIN;)
	{// 常に最大数を維持する
		SetGrain();
	}

	for (int nCntGrain = 0; nCntGrain < g_nNumGrain; nCntGrain++)
	{
		if (pGrain->bUse == false)
		{
			PrintDebugProc("データ異常発生中 : grain\n");
		}

		pGrain->pos += pGrain->move;						// 移動させる
		g_aGrain[nCntGrain].fRadius -= GRAINALPHA_FADE;		// 半径を小さくする
		pGrain->nLife--;									// 寿命を減らす

		pGrain->fAngle += pGrain->fSpeed;

		pGrain->fAngle = AngleNormalize(pGrain->fAngle);

		pGrain->pos.y += sinf(pGrain->fAngle) * 0.075f;

		if (pGrain->nLife < 0)
		{// 寿命が尽きたら
			// 少しずつ見えなくする
			pGrain->col.a += -GRAINALPHA_FADE;
		}
		else
		{// 寿命があれば
			// 少しずつ見えるようにする
			pGrain->col.a += GRAINALPHA_FADE;
		}

		if (pGrain->fRadius < 0)
		{// 半径が0以下になったら
			// 解放処理
			ReleaseGrain(nCntGrain);
			nCntGrain--;
			continue;
		}

		if (pGrain->col.a < 0)
		{// 見えなくなったら
			// 解放処理
			ReleaseGrain(nCntGrain);
			nCntGrain--;
			continue;
		}

		// ポインタを進める
		pGrain++;
	}
}

//======================================================================================
// 文字の描画処理
//======================================================================================
void DrawGrain(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();		// デバイスの取得

	Grain* pGrain = &g_aGrain[0];					// 文字へのポインタ
	D3DXMATRIX mtxRot, mtxTrans;					// 計算用マトリックス
	D3DXMATRIX mtx;
	D3DXMATRIX mtxView;								// ビューマトリックス

	// ビューマトリックスを取得
	pDevice->GetTransform(D3DTS_VIEW, &mtxView);

	VERTEX_3D* pVtx;    // 頂点情報の設定

	// 頂点バッファをロックし、頂点情報へのポインタを取得
	g_pVtxBuffGrain->Lock(0, 0, (void**)&pVtx, 0);

	for (int nCntGrain = 0; nCntGrain < g_nNumGrain; nCntGrain++, pGrain++)
	{
		if (pGrain->bUse == false)
		{
			PrintDebugProc("データ異常発生中 : grain\n");
		}

		//ワールドマトリックスの初期化
		D3DXMatrixIdentity(&pGrain->mtxWorld);

		// 文字をカメラに対して正面に向ける
		D3DXMatrixInverse(&pGrain->mtxWorld, NULL, &mtxView);	//逆行列を求める

		pGrain->mtxWorld._41 = 0.0f;		//マトリックス(行列)の内容
		pGrain->mtxWorld._42 = 0.0f;
		pGrain->mtxWorld._43 = 0.0f;

		//位置を反映
		D3DXMatrixTranslation(&mtxTrans, pGrain->pos.x, pGrain->pos.y, pGrain->pos.z);
		D3DXMatrixMultiply(&pGrain->mtxWorld, &pGrain->mtxWorld, &mtxTrans);

		//頂点座標の設定
		pVtx[0].pos = D3DXVECTOR3(-pGrain->fRadius, pGrain->fRadius, 0.0f);
		pVtx[1].pos = D3DXVECTOR3(pGrain->fRadius, pGrain->fRadius, 0.0f);
		pVtx[2].pos = D3DXVECTOR3(-pGrain->fRadius, -pGrain->fRadius, 0.0f);
		pVtx[3].pos = D3DXVECTOR3(pGrain->fRadius, -pGrain->fRadius, 0.0f);

		//頂点座標の設定
		pVtx[0].pos = *D3DXVec3TransformCoord(&pVtx[0].pos, &pVtx[0].pos, &pGrain->mtxWorld);
		pVtx[1].pos = *D3DXVec3TransformCoord(&pVtx[1].pos, &pVtx[1].pos, &pGrain->mtxWorld);
		pVtx[2].pos = *D3DXVec3TransformCoord(&pVtx[2].pos, &pVtx[2].pos, &pGrain->mtxWorld);
		pVtx[3].pos = *D3DXVec3TransformCoord(&pVtx[3].pos, &pVtx[3].pos, &pGrain->mtxWorld);

		//頂点カラーの設定
		pVtx[0].col = pGrain->col;
		pVtx[1].col = pGrain->col;
		pVtx[2].col = pGrain->col;
		pVtx[3].col = pGrain->col;

		//テクスチャ座標の設定
		pVtx[0].tex = D3DXVECTOR2(GRAINTEX_SPRITPOS * (pGrain->type % GRAINTEX_SPRIT), GRAINTEX_SPRITPOS * (pGrain->type / GRAINTEX_SPRIT));
		pVtx[1].tex = D3DXVECTOR2(GRAINTEX_SPRITPOS * (pGrain->type % GRAINTEX_SPRIT) + GRAINTEX_SPRITPOS, GRAINTEX_SPRITPOS * (pGrain->type / GRAINTEX_SPRIT));
		pVtx[2].tex = D3DXVECTOR2(GRAINTEX_SPRITPOS * (pGrain->type % GRAINTEX_SPRIT), GRAINTEX_SPRITPOS * (pGrain->type / GRAINTEX_SPRIT) + GRAINTEX_SPRITPOS);
		pVtx[3].tex = D3DXVECTOR2(GRAINTEX_SPRITPOS * (pGrain->type % GRAINTEX_SPRIT) + GRAINTEX_SPRITPOS, GRAINTEX_SPRITPOS * (pGrain->type / GRAINTEX_SPRIT) + GRAINTEX_SPRITPOS);

		// ポインタを進める
		pVtx += 4;
	}

	g_pVtxBuffGrain->Unlock();

	pDevice->SetRenderState(D3DRS_LIGHTING, FALSE);		// ライトを無効にする

	//αブレンディングを加算合成して設定
	pDevice->SetRenderState(D3DRS_BLENDOP, D3DBLENDOP_ADD);
	pDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
	pDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_ONE);

	// Zテストを無効にする
	pDevice->SetRenderState(D3DRS_ZWRITEENABLE, FALSE);		// Zバッファ更新の有効/無効の設定

	//ワールドマトリックスの初期化
	D3DXMatrixIdentity(&mtx);

	////ビューマトリックスを取得
	//pDevice->GetTransform(D3DTS_VIEW, &mtxView);

	////エフェクトをカメラに対して正面に向ける
	//D3DXMatrixInverse(&mtx, NULL, &mtxView);	//逆行列を求める

	//mtx._41 = 0.0f;		//マトリックス(行列)の内容
	//mtx._42 = 0.0f;
	//mtx._43 = 0.0f;

	// ワールドマトリックスの設定
	pDevice->SetTransform(D3DTS_WORLD, &mtx);

	// 頂点バッファをデータストリームに設定
	pDevice->SetStreamSource(0, g_pVtxBuffGrain, 0, sizeof(VERTEX_3D));

	// インデックスバッファをデータストリームに設定
	pDevice->SetIndices(g_pIdxBuffGrain);

	// 頂点フォーマットの設定
	pDevice->SetFVF(FVF_VERTEX_3D);

	// テクスチャの設定
	pDevice->SetTexture(0, g_pTextureBuffGrain);

	// 文字の描画
	pDevice->DrawIndexedPrimitive(D3DPT_TRIANGLELIST,
		0,
		0,
		g_nNumGrain * 4,
		0,
		g_nNumGrain * 2);

	// Zテストを有効にする
	pDevice->SetRenderState(D3DRS_ZWRITEENABLE, TRUE);

	//αブレンディングを戻す
	pDevice->SetRenderState(D3DRS_BLENDOP, D3DBLENDOP_ADD);
	pDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
	pDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);

	pDevice->SetRenderState(D3DRS_LIGHTING, TRUE);		// ライトを有効にする
}

//======================================================================================
// 文字の設定処理
//======================================================================================
void SetGrain(void)
{
	Grain* pGrain = &g_aGrain[g_nNumGrain];		// 対象文字へのポインタ

	// 各値設定
	pGrain->pos = D3DXVECTOR3(-5000.0f, 0.0f, -5000.0f) + RANDAM_VEC3(10000, 1500, 10000);
	pGrain->col = COLOR_RANDOM + D3DXCOLOR(0.5f, 0.5f, 0.5f, 0.0f);
	pGrain->col.a = 0.0f;
	pGrain->type = (GRAINTYPE)(rand() % GRAINTYPE_MAX);
	pGrain->fRadius = (float)((rand() % MAX_GRAINRADIUS + MIN_GRAINRADIUS) / 10);
	pGrain->fAngle = ((float)(rand() % 629 - 314) / 100);
	pGrain->fSpeed = ((float)(rand() % 30 - 15) / 100);
	pGrain->nLife = rand() % MAX_GRAINLIFE + MIN_GRAINLIFE;
	pGrain->bUse = true;

	VERTEX_3D* pVtx;    // 頂点情報の設定

	// 頂点バッファをロックし、頂点情報へのポインタを取得
	g_pVtxBuffGrain->Lock(0, 0, (void**)&pVtx, 0);

	// 頂点座標の設定
	pVtx[0].pos = D3DXVECTOR3(-pGrain->fRadius, pGrain->fRadius, 0.0f);
	pVtx[1].pos = D3DXVECTOR3(pGrain->fRadius, pGrain->fRadius, 0.0f);
	pVtx[2].pos = D3DXVECTOR3(-pGrain->fRadius, -pGrain->fRadius, 0.0f);
	pVtx[3].pos = D3DXVECTOR3(pGrain->fRadius, -pGrain->fRadius, 0.0f);

	// 頂点カラーの設定
	pVtx[0].col = pGrain->col;
	pVtx[1].col = pGrain->col;
	pVtx[2].col = pGrain->col;
	pVtx[3].col = pGrain->col;

	// 頂点バッファをアンロック
	g_pVtxBuffGrain->Unlock();

	// 使用数を加算
	g_nNumGrain++;
}

//======================================================================================
// 文字の解放処理
//======================================================================================
void ReleaseGrain(int nIdx)
{
	g_aGrain[nIdx] = g_aGrain[g_nNumGrain - 1];					// 最後尾の情報を代入
	memset(&g_aGrain[g_nNumGrain - 1], NULL, sizeof(Grain));	// 最後尾の情報をリセット
	g_nNumGrain--;												// 使用数を減らす
}