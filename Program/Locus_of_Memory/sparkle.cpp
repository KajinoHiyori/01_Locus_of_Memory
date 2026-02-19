//=============================================================================
//
//	画面上の粒の処理 [sparkle.h]
//	Author : SHUMA AIZU
// 
//=============================================================================

#include"main.h"
#include"sparkle.h"
#include"input.h"
#include"camera.h"
#include "fog.h"
#include "debugproc.h"
#include "color.h"

//*****************************************************************************
// マクロ定義
//*****************************************************************************
#define MAX_SPARKLE				(2048)						// 粒の最大数
#define SPARKLE_RADIUS			(25.0f)						// 粒の半径
#define SPARKLEALPHA_FADE		(0.025f)					// 粒のフェードスピード
#define SPARKLETEX_SPRIT		(5)							// テクスチャの一列の分割数
#define SPARKLETEX_SPRITPOS		(1.0f / SPARKLETEX_SPRIT)	// テクスチャの分割座標
#define MAX_SPARKLERADIUS		(150)						// 粒の半径の最大
#define MIN_SPARKLERADIUS		(100)						// 粒の半径の最小
#define MAX_SPARKLELIFE			(180)						// 粒の寿命の最大
#define MIN_SPARKLELIFE			(60)						// 粒の寿命の最小

//*****************************************************************************
// グローバル変数
//*****************************************************************************
LPDIRECT3DTEXTURE9 g_pTextureBuffSparkle = {};		// テクスチャへのポインタ
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffSparkle = NULL;	// 頂点バッファへのポインタ
Sparkle g_aSparkle[MAX_SPARKLE];					// 粒の情報
int g_aNumSparkleIdx[MAX_SPARKLE];					// 使用している粒のインデックス
int g_nNumSparkle;									// 使用している粒の数

//=============================================================================
// 粒の初期化処理
//=============================================================================
void InitSparkle(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	Sparkle* pSparkle = &g_aSparkle[0];

	// テクスチャ読み込み
	D3DXCreateTextureFromFile(pDevice, 
		"data/TEXTURE/EFFECT/effect000.jpg", 
		&g_pTextureBuffSparkle);

	// 初期化
	memset(pSparkle, NULL, sizeof(Sparkle) * MAX_SPARKLE);
	memset(&g_aNumSparkleIdx[0], -1, sizeof(int) * MAX_SPARKLE);
	g_nNumSparkle = 0;

	// 頂点バッファの生成
	pDevice->CreateVertexBuffer(sizeof(VERTEX_3D) * MAX_SPARKLE * 4, 
		D3DUSAGE_WRITEONLY, 
		FVF_VERTEX_3D, 
		D3DPOOL_MANAGED, 
		&g_pVtxBuffSparkle, NULL);

	VERTEX_3D* pVtx;    // 頂点情報の設定

	// 頂点バッファをロックし、頂点情報へのポインタを取得
	g_pVtxBuffSparkle->Lock(0, 0, (void**)&pVtx, 0);

	for (int nCntSparkle = 0; nCntSparkle < MAX_SPARKLE; nCntSparkle++, pSparkle++)
	{
		// 頂点座標の設定
		pVtx[0].pos = D3DXVECTOR3(-pSparkle->fRadius, pSparkle->fRadius, 0.0f);
		pVtx[1].pos = D3DXVECTOR3(pSparkle->fRadius, pSparkle->fRadius, 0.0f);
		pVtx[2].pos = D3DXVECTOR3(-pSparkle->fRadius, -pSparkle->fRadius, 0.0f);
		pVtx[3].pos = D3DXVECTOR3(pSparkle->fRadius, -pSparkle->fRadius, 0.0f);

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
		pVtx[1].tex = D3DXVECTOR2(1.0f, 0.0f);
		pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
		pVtx[3].tex = D3DXVECTOR2(1.0f, 1.0f);

		pVtx += 4;
	}

	g_pVtxBuffSparkle->Unlock();
}

//=============================================================================
// 粒の終了処理
//=============================================================================
void UninitSparkle(void)
{
	// テクスチャの破棄
	if (g_pTextureBuffSparkle != NULL)
	{
		g_pTextureBuffSparkle->Release();
		g_pTextureBuffSparkle = NULL;
	}

	// 頂点バッファを破棄
	if (g_pVtxBuffSparkle != NULL)
	{
		g_pVtxBuffSparkle->Release();
		g_pVtxBuffSparkle = NULL;
	}

}

//=============================================================================
// 粒の更新処理
//=============================================================================
void UpdateSparkle(void)
{
	Sparkle* pSparkle = &g_aSparkle[0];					// 粒へのポインタ

	for (; g_nNumSparkle < MAX_SPARKLE;)
	{// 常に最大数を維持する
		SetSparkle();
	}

	PrintDebugProc("NumSparkle %d\n", g_nNumSparkle);

	VERTEX_3D* pVtx;    // 頂点情報の設定

	// 頂点バッファをロックし、頂点情報へのポインタを取得
	g_pVtxBuffSparkle->Lock(0, 0, (void**)&pVtx, 0);

	for (int nCntSparkle = 0; nCntSparkle < g_nNumSparkle; nCntSparkle++)
	{
		if (pSparkle->bUse == false)
		{
			PrintDebugProc("データ異常発生中 : sparkle\n");
		}

		pSparkle->pos += pSparkle->move * pSparkle->fSpeed;		// 移動させる
		g_aSparkle[nCntSparkle].fRadius -= SPARKLEALPHA_FADE;	// 半径を小さくする
		pSparkle->nLife--;										// 寿命を減らす

		pSparkle->fAngle += pSparkle->fSpeed;

		pSparkle->fAngle = AngleNormalize(pSparkle->fAngle);

		pSparkle->pos.x += sinf(pSparkle->fAngle) * 0.05f;

		if (pSparkle->nLife < 0)
		{// 寿命が尽きたら
			// 少しずつ見えなくする
			pSparkle->col.a += -SPARKLEALPHA_FADE;
		}
		else
		{// 寿命があれば
			// 少しずつ見えるようにする
			pSparkle->col.a += SPARKLEALPHA_FADE;
		}

		if (pSparkle->fRadius < 0)
		{// 半径が0以下になったら
			// 解放処理
			ReleaseSparkle(nCntSparkle);
			nCntSparkle--;
			continue;
		}

		if (pSparkle->col.a < 0)
		{// 見えなくなったら
			// 解放処理
			ReleaseSparkle(nCntSparkle);
			nCntSparkle--;
			continue;
		}

		// 頂点座標の設定
		pVtx[0].pos = D3DXVECTOR3(-pSparkle->fRadius, pSparkle->fRadius, 0.0f);
		pVtx[1].pos = D3DXVECTOR3(pSparkle->fRadius, pSparkle->fRadius, 0.0f);
		pVtx[2].pos = D3DXVECTOR3(-pSparkle->fRadius, -pSparkle->fRadius, 0.0f);
		pVtx[3].pos = D3DXVECTOR3(pSparkle->fRadius, -pSparkle->fRadius, 0.0f);

		// 頂点カラーの設定
		pVtx[0].col = pSparkle->col;
		pVtx[1].col = pSparkle->col;
		pVtx[2].col = pSparkle->col;
		pVtx[3].col = pSparkle->col;

		// ポインタを進める
		pSparkle++;
		pVtx += 4;
	}

	g_pVtxBuffSparkle->Unlock();
}

//=============================================================================
// 粒の描画処理
//=============================================================================
void DrawSparkle(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();		// デバイスの取得

	Sparkle* pSparkle = &g_aSparkle[0];				// 粒へのポインタ
	D3DXMATRIX mtxRot, mtxTrans;					// 計算用マトリックス
	D3DXMATRIX mtxView;								// ビューマトリックス

	SetFogEnable(false);		// 霧を消す

	pDevice->SetRenderState(D3DRS_LIGHTING, FALSE);		// ライトを無効にする

	// αブレンディングを加算合成して設定
	pDevice->SetRenderState(D3DRS_BLENDOP, D3DBLENDOP_ADD);
	pDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
	pDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_ONE);

	//// Zテストを無効にする
	pDevice->SetRenderState(D3DRS_ZWRITEENABLE, FALSE);		// Zバッファ更新の有効/無効の設定

	for (int nCntSparkle = 0; nCntSparkle < g_nNumSparkle; nCntSparkle++, pSparkle++)
	{
		if (pSparkle->bUse == false)
		{
			PrintDebugProc("データ異常発生中 : sparkle\n");
		}

		// ワールドマトリックスの初期化
		D3DXMatrixIdentity(&pSparkle->mtxWorld);

		// ビューマトリックスを取得
		pDevice->GetTransform(D3DTS_VIEW, &mtxView);

		// 粒をカメラに対して正面に向ける
		D3DXMatrixInverse(&pSparkle->mtxWorld, NULL, &mtxView);	// 逆行列を求める

		pSparkle->mtxWorld._41 = 0.0f;		// マトリックス(行列)の内容
		pSparkle->mtxWorld._42 = 0.0f;
		pSparkle->mtxWorld._43 = 0.0f;

		// 位置を反映
		D3DXMatrixTranslation(&mtxTrans, pSparkle->pos.x, pSparkle->pos.y, pSparkle->pos.z);
		D3DXMatrixMultiply(&pSparkle->mtxWorld, &pSparkle->mtxWorld, &mtxTrans);

		// ワールドマトリックスの設定
		pDevice->SetTransform(D3DTS_WORLD, &pSparkle->mtxWorld);

		// 頂点バッファをデータストリームに設定
		pDevice->SetStreamSource(0, g_pVtxBuffSparkle, 0, sizeof(VERTEX_3D));

		// 頂点フォーマットの設定
		pDevice->SetFVF(FVF_VERTEX_3D);

		// テクスチャの設定
		pDevice->SetTexture(0, g_pTextureBuffSparkle);

		// 粒の描画
		pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, nCntSparkle * 4, 2);
	}

	// Zテストを有効にする
	pDevice->SetRenderState(D3DRS_ZWRITEENABLE, TRUE);

	// αブレンディングを戻す
	pDevice->SetRenderState(D3DRS_BLENDOP, D3DBLENDOP_ADD);
	pDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
	pDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);

	SetFogEnable(true);		// 霧を戻す

	pDevice->SetRenderState(D3DRS_LIGHTING, TRUE);		// ライトを有効にする
}

//=============================================================================
// 粒の設定処理
//=============================================================================
void SetSparkle(void)
{
	Sparkle* pSparkle = &g_aSparkle[g_nNumSparkle];		// 対象粒へのポインタ

	D3DXVECTOR3 move;

	move.x = (float)((float)(rand() % 201 - 100) / 100);
	move.y = (float)((float)(rand() % 201 - 100) / 100);
	move.z = (float)((float)(rand() % 201 - 100) / 100);

	// 各値設定
	pSparkle->pos = D3DXVECTOR3(-5000.0f, 0.0f, -5000.0f) + RANDAM_VEC3(10000, 1500, 10000);
	pSparkle->move = move;
	pSparkle->col = COLOR_RANDOM + D3DXCOLOR(0.5f, 0.5f, 0.5f, 0.0f);
	pSparkle->col.a = 0.0f;
	pSparkle->fRadius = (float)((rand() % MAX_SPARKLERADIUS + MIN_SPARKLERADIUS) / 10);
	pSparkle->fAngle = ((float)(rand() % 629 - 314) / 100);
	pSparkle->fSpeed = ((float)(rand() % 30 - 15) / 100);
	pSparkle->nLife = rand() % MAX_SPARKLELIFE + MIN_SPARKLELIFE;
	pSparkle->bUse = true;

	if (GetMode() == MODE_TITLE || GetMode() == MODE_START)
	{
		pSparkle->pos.y += 500.0f;
	}

	VERTEX_3D* pVtx;    // 頂点情報の設定

	// 頂点バッファをロックし、頂点情報へのポインタを取得
	g_pVtxBuffSparkle->Lock(0, 0, (void**)&pVtx, 0);

	// 頂点座標の設定
	pVtx[0].pos = D3DXVECTOR3(-pSparkle->fRadius, pSparkle->fRadius, 0.0f);
	pVtx[1].pos = D3DXVECTOR3(pSparkle->fRadius, pSparkle->fRadius, 0.0f);
	pVtx[2].pos = D3DXVECTOR3(-pSparkle->fRadius, -pSparkle->fRadius, 0.0f);
	pVtx[3].pos = D3DXVECTOR3(pSparkle->fRadius, -pSparkle->fRadius, 0.0f);

	// 頂点カラーの設定
	pVtx[0].col = pSparkle->col;
	pVtx[1].col = pSparkle->col;
	pVtx[2].col = pSparkle->col;
	pVtx[3].col = pSparkle->col;

	// 頂点バッファをアンロック
	g_pVtxBuffSparkle->Unlock();

	// 使用数を加算
	g_nNumSparkle++;
}

//=============================================================================
// 粒の解放処理
//=============================================================================
void ReleaseSparkle(int nIdx)
{
	g_aSparkle[nIdx] = g_aSparkle[g_nNumSparkle - 1];				// 最後尾の情報を代入
	memset(&g_aSparkle[g_nNumSparkle - 1], NULL, sizeof(Sparkle));	// 最後尾の情報をリセット
	g_nNumSparkle--;												// 使用数を減らす
}