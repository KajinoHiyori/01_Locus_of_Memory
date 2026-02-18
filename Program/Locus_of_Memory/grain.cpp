//=============================================================================
//
//	画面上の粒の処理 [grain.h]
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
#define MAX_GRAIN			(128)					// 粒の最大数
#define GRAIN_RADIUS		(25.0f)					// 粒の半径
#define GRAINALPHA_FADE		(0.025f)				// 粒のフェードスピード
#define GRAINTEX_SPRIT		(5)						// テクスチャの一列の分割数
#define GRAINTEX_SPRITPOS	(1.0f / GRAINTEX_SPRIT)	// テクスチャの分割座標

//*****************************************************************************
// グローバル変数
//*****************************************************************************
LPDIRECT3DTEXTURE9 g_pTextureBuffGrain = {};		// テクスチャへのポインタ
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffGrain = NULL;	// 頂点バッファへのポインタ
Grain g_aGrain[MAX_GRAIN];							// 粒の情報
int g_aNumGrainIdx[MAX_GRAIN];						// 使用している粒のインデックス
int g_nNumGrain;									// 使用している粒の数

//======================================================================================
// 粒の初期化処理
//======================================================================================
void InitGrain(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	Grain* pGrain = &g_aGrain[0];

	//テクスチャ読み込み
	D3DXCreateTextureFromFile(pDevice, 
		"data/TEXTURE/grain.jpg", 
		&g_pTextureBuffGrain);

	//初期化
	memset(pGrain, NULL, sizeof(Grain) * MAX_GRAIN);
	memset(&g_aNumGrainIdx[0], -1, sizeof(int) * MAX_GRAIN);
	g_nNumGrain = 0;

	//頂点バッファの生成
	pDevice->CreateVertexBuffer(sizeof(VERTEX_3D) * MAX_GRAIN * 4, 
		D3DUSAGE_WRITEONLY, 
		FVF_VERTEX_3D, 
		D3DPOOL_MANAGED, 
		&g_pVtxBuffGrain, NULL);

	VERTEX_3D* pVtx;    //頂点情報の設定

	//頂点バッファをロックし、頂点情報へのポインタを取得
	g_pVtxBuffGrain->Lock(0, 0, (void**)&pVtx, 0);

	for (int nCntGrain = 0; nCntGrain < MAX_GRAIN; nCntGrain++, pGrain++)
	{
		//頂点座標の設定
		pVtx[0].pos = D3DXVECTOR3(-pGrain->fRadius, pGrain->fRadius, 0.0f);
		pVtx[1].pos = D3DXVECTOR3(pGrain->fRadius, pGrain->fRadius, 0.0f);
		pVtx[2].pos = D3DXVECTOR3(-pGrain->fRadius, -pGrain->fRadius, 0.0f);
		pVtx[3].pos = D3DXVECTOR3(pGrain->fRadius, -pGrain->fRadius, 0.0f);

		//rhwの設定
		pVtx[0].nor = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
		pVtx[1].nor = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
		pVtx[2].nor = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
		pVtx[3].nor = D3DXVECTOR3(0.0f, 1.0f, 0.0f);

		//頂点カラーの設定
		pVtx[0].col = COLOR_WHITE;
		pVtx[1].col = COLOR_WHITE;
		pVtx[2].col = COLOR_WHITE;
		pVtx[3].col = COLOR_WHITE;

		//テクスチャ座標の設定
		pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
		pVtx[1].tex = D3DXVECTOR2(0.2f, 0.0f);
		pVtx[2].tex = D3DXVECTOR2(0.0f, 0.2f);
		pVtx[3].tex = D3DXVECTOR2(0.2f, 0.2f);

		pVtx += 4;
	}

	g_pVtxBuffGrain->Unlock();
}

//======================================================================================
// 粒の終了処理
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

}

//======================================================================================
// 粒の更新処理
//======================================================================================
void UpdateGrain(void)
{
	Grain* pGrain = &g_aGrain[0];					// 粒へのポインタ

	for (; g_nNumGrain < 64;)
	{
		D3DXVECTOR3 pos = RANDAM_VEC3(500, 500, 500);
		SetGrain(pos, INIT_D3DXVEC3, COLOR_ORANGE, (GRAINTYPE)(rand() % GRAINTYPE_MAX), rand() % 180 + 60, (float)(rand() % 30 + 15));
	}

	PrintDebugProc("NumGrain %d\n", g_nNumGrain);

	VERTEX_3D* pVtx;    //頂点情報の設定

	//頂点バッファをロックし、頂点情報へのポインタを取得
	g_pVtxBuffGrain->Lock(0, 0, (void**)&pVtx, 0);

	for (int nCntGrain = 0; nCntGrain < g_nNumGrain; nCntGrain++)
	{
		if (pGrain->bUse == false)
		{
			PrintDebugProc("データ異常発生中 : grain\n");
		}

		pGrain->pos += pGrain->move;						// 移動させる
		g_aGrain[nCntGrain].fRadius -= GRAINALPHA_FADE;		// 半径を小さくする
		pGrain->nLife--;									// 寿命を減らす

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

		//頂点座標の設定
		pVtx[0].pos = D3DXVECTOR3(-pGrain->fRadius, pGrain->fRadius, 0.0f);
		pVtx[1].pos = D3DXVECTOR3(pGrain->fRadius, pGrain->fRadius, 0.0f);
		pVtx[2].pos = D3DXVECTOR3(-pGrain->fRadius, -pGrain->fRadius, 0.0f);
		pVtx[3].pos = D3DXVECTOR3(pGrain->fRadius, -pGrain->fRadius, 0.0f);

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
		pGrain++;
		pVtx += 4;
	}

	g_pVtxBuffGrain->Unlock();
}

//======================================================================================
// 粒の描画処理
//======================================================================================
void DrawGrain(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();		// デバイスの取得

	Grain* pGrain = &g_aGrain[0];					// 粒へのポインタ
	D3DXMATRIX mtxRot, mtxTrans;					// 計算用マトリックス
	D3DXMATRIX mtxView;								// ビューマトリックス

	SetFogEnable(false);		// 霧を消す

	pDevice->SetRenderState(D3DRS_LIGHTING, FALSE);		// ライトを無効にする

	//αブレンディングを加算合成して設定
	pDevice->SetRenderState(D3DRS_BLENDOP, D3DBLENDOP_ADD);
	pDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
	pDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_ONE);

	// Zテストを無効にする
	pDevice->SetRenderState(D3DRS_ZFUNC, D3DCMP_ALWAYS);	// Zテストの比較方法を変更(Zバッファの前後関係に関わらず描画する)
	pDevice->SetRenderState(D3DRS_ZWRITEENABLE, FALSE);		// Zバッファ更新の有効/無効の設定

	for (int nCntGrain = 0; nCntGrain < g_nNumGrain; nCntGrain++, pGrain++)
	{
		if (pGrain->bUse == false)
		{
			PrintDebugProc("データ異常発生中 : grain\n");
		}

		//ワールドマトリックスの初期化
		D3DXMatrixIdentity(&pGrain->mtxWorld);

		//ビューマトリックスを取得
		pDevice->GetTransform(D3DTS_VIEW, &mtxView);

		//粒をカメラに対して正面に向ける
		D3DXMatrixInverse(&pGrain->mtxWorld, NULL, &mtxView);	//逆行列を求める

		pGrain->mtxWorld._41 = 0.0f;		//マトリックス(行列)の内容
		pGrain->mtxWorld._42 = 0.0f;
		pGrain->mtxWorld._43 = 0.0f;

		//位置を反映
		D3DXMatrixTranslation(&mtxTrans, pGrain->pos.x, pGrain->pos.y, pGrain->pos.z);
		D3DXMatrixMultiply(&pGrain->mtxWorld, &pGrain->mtxWorld, &mtxTrans);

		//ワールドマトリックスの設定
		pDevice->SetTransform(D3DTS_WORLD, &pGrain->mtxWorld);

		//頂点バッファをデータストリームに設定
		pDevice->SetStreamSource(0, g_pVtxBuffGrain, 0, sizeof(VERTEX_3D));

		//頂点フォーマットの設定
		pDevice->SetFVF(FVF_VERTEX_3D);

		//テクスチャ
		pDevice->SetTexture(0, g_pTextureBuffGrain);

		//粒の描画
		pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, nCntGrain * 4, 2);
	}

	// Zテストを有効にする
	pDevice->SetRenderState(D3DRS_ZFUNC, D3DCMP_LESSEQUAL);
	pDevice->SetRenderState(D3DRS_ZWRITEENABLE, TRUE);

	//αブレンディングを戻す
	pDevice->SetRenderState(D3DRS_BLENDOP, D3DBLENDOP_ADD);
	pDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
	pDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);

	SetFogEnable(true);		// 霧を戻す

	pDevice->SetRenderState(D3DRS_LIGHTING, TRUE);		// ライトを有効にする
}

//======================================================================================
// 粒の設定処理
//======================================================================================
void SetGrain(D3DXVECTOR3 pos, D3DXVECTOR3 move, D3DXCOLOR col, GRAINTYPE type, int nLife, float fRadius)
{
	Grain* pGrain = &g_aGrain[g_nNumGrain];		// 対象粒へのポインタ

	col.a = 0.0f;

	// 各値設定
	pGrain->pos = pos;
	pGrain->move = move;
	pGrain->col = col;
	pGrain->type = type;
	pGrain->fRadius = fRadius;
	pGrain->nLife = nLife;
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
// 粒の解放処理
//======================================================================================
void ReleaseGrain(int nIdx)
{
	g_aGrain[nIdx] = g_aGrain[g_nNumGrain - 1];					// 最後尾の情報を代入
	memset(&g_aGrain[g_nNumGrain - 1], NULL, sizeof(Grain));	// 最後尾の情報をリセット
	g_nNumGrain--;												// 使用数を減らす
}