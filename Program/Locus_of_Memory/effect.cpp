//========================================================================
// 
// エフェクトの描画[effect.cpp]
// Author : MatuhasiEito
// Author : KajinoHiyori
//
//========================================================================
#include"main.h"
#include"effect.h"
#include"input.h"
#include"camera.h"
#include "fog.h"
#include "debugproc.h"
#include "color.h"

// マクロ定義
#define MAX_EFFECT		(16384)				// エフェクトの最大数
#define EFFECT_RADIUS	(25.0f)				// エフェクトの半径
#define MAX_EFFECTTEX	(EFFECT_TEX_MAX)	// エフェクトのテクスチャの数
#define EFFECTTEX_SPRIT	(1.0f / MAX_EFFECTTEX)
#define BATCHING		(true)

//構造体の定義
typedef struct
{
	D3DXVECTOR3 pos;			// 位置
	D3DXVECTOR3 move;			// 移動量
	D3DXCOLOR	col;			// 色
	D3DXMATRIX	mtxWorld;		// ワールドマトリックス
	EFFECT_TYPE	type;			// エフェクトの種類
	EFFECT_TEX	tex;			// エフェクトのテクスチャの種類
	float		fRadius;		// 半径
	int			nLife;			// 寿命（色）
	bool		bUse;			// 使用しているか
}Effect;

// テクスチャの読み込み
const char* c_apFilenameEffect[MAX_EFFECTTEX] =
{
	"data\\TEXTURE\\effect\\effect000.jpg",
	"data\\TEXTURE\\effect\\effect001.jpg",
	"data\\TEXTURE\\effect\\effect002.jpg",
	"data\\TEXTURE\\effect\\effect003.jpg",
	"data\\TEXTURE\\effect\\effect004.jpg",
};

//グローバル変数
LPDIRECT3DTEXTURE9 g_pTextureBuffEffect = NULL;
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffEffect = NULL;
LPDIRECT3DINDEXBUFFER9 g_pIdxBuffEffect = NULL;
Effect g_aEffect[MAX_EFFECT];

int g_nNumEffect;												// 使用しているエフェクトの数

//======================================================================================
// エフェクトの初期化処理
//======================================================================================
void InitEffect(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	//テクスチャ読み込み
	for (int nCntEffect = 0; nCntEffect < MAX_EFFECTTEX; nCntEffect++)
	{
		D3DXCreateTextureFromFile(pDevice, "data\\TEXTURE\\effect.jpg", &g_pTextureBuffEffect);
	}

	//初期化
	for (int nCntEffect = 0; nCntEffect < MAX_EFFECT; nCntEffect++)
	{
		g_aEffect[nCntEffect].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		g_aEffect[nCntEffect].move = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		g_aEffect[nCntEffect].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
		g_aEffect[nCntEffect].type = EFFECT_TYPE_NORMAL;
		g_aEffect[nCntEffect].tex = EFFECT_TEX_CIRCLE;
		g_aEffect[nCntEffect].fRadius = EFFECT_RADIUS;
		g_aEffect[nCntEffect].nLife = 0;
		g_aEffect[nCntEffect].bUse = false;
	}

	g_nNumEffect = 0;

	//頂点バッファの生成
	pDevice->CreateVertexBuffer(sizeof(VERTEX_3D) * MAX_EFFECT * 4, D3DUSAGE_WRITEONLY, FVF_VERTEX_3D, D3DPOOL_MANAGED, &g_pVtxBuffEffect, NULL);

	VERTEX_3D* pVtx;    //頂点情報の設定

	//頂点バッファをロックし、頂点情報へのポインタを取得
	g_pVtxBuffEffect->Lock(0, 0, (void**)&pVtx, 0);

	for (int nCntEffect = 0; nCntEffect < MAX_EFFECT; nCntEffect++, pVtx += 4)
	{
		//頂点座標の設定
		pVtx[0].pos = D3DXVECTOR3(-g_aEffect[nCntEffect].fRadius, g_aEffect[nCntEffect].fRadius, 0.0f);
		pVtx[1].pos = D3DXVECTOR3(g_aEffect[nCntEffect].fRadius, g_aEffect[nCntEffect].fRadius, 0.0f);
		pVtx[2].pos = D3DXVECTOR3(-g_aEffect[nCntEffect].fRadius, -g_aEffect[nCntEffect].fRadius, 0.0f);
		pVtx[3].pos = D3DXVECTOR3(g_aEffect[nCntEffect].fRadius, -g_aEffect[nCntEffect].fRadius, 0.0f);

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
		pVtx[1].tex = D3DXVECTOR2(1.0f, 0.0f);
		pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
		pVtx[3].tex = D3DXVECTOR2(1.0f, 1.0f);
	}

	g_pVtxBuffEffect->Unlock();

	// インデックスバッファの設定
	pDevice->CreateIndexBuffer(sizeof(WORD) * (MAX_EFFECT * (4 + 2)),
		D3DUSAGE_WRITEONLY,
		D3DFMT_INDEX16,
		D3DPOOL_MANAGED,
		&g_pIdxBuffEffect,
		NULL);

	WORD* pIdx;		// インデックス情報へのポインタ

	// インデックスバッファをロックし、頂点情報へのポインタを取得
	g_pIdxBuffEffect->Lock(0, 0, (void**)&pIdx, 0);

	for (int nCntIdx = 0; nCntIdx < MAX_EFFECT * 4; nCntIdx += 4)
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
	g_pIdxBuffEffect->Unlock();
}

//======================================================================================
// エフェクトの終了処理
//======================================================================================
void UninitEffect(void)
{
	//テクスチャの破棄
	if (g_pTextureBuffEffect != NULL)
	{
		g_pTextureBuffEffect->Release();
		g_pTextureBuffEffect = NULL;
	}

	//頂点バッファを破棄
	if (g_pVtxBuffEffect != NULL)
	{
		g_pVtxBuffEffect->Release();
		g_pVtxBuffEffect = NULL;
	}

}

//======================================================================================
// エフェクトの更新処理
//======================================================================================
void UpdateEffect(void)
{
	Effect* pEffect = &g_aEffect[0];

	if (GetKeyboardTrigger(DIK_SPACE) == true)
	{
		SetEffect(EFFECT_TYPE_NORMAL, EFFECT_TEX_CIRCLE, D3DXVECTOR3(0.0f, 50.0f, 0.0f), D3DXVECTOR3(0.0f, 5.0f, 0.0f), D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f), 20, 50.0f);
	}

	VERTEX_3D* pVtx;    //頂点情報の設定

	//頂点バッファをロックし、頂点情報へのポインタを取得
	g_pVtxBuffEffect->Lock(0, 0, (void**)&pVtx, 0);

	for (int nCntEffect = 0; nCntEffect < g_nNumEffect; nCntEffect++)
	{
		if (pEffect->bUse == false)
		{
			PrintDebugProc("※データ異常 : UpdateEffect\n");
			break;
		}

		if (pEffect->bUse == true)
		{
			if (pEffect->type == EFFECT_TYPE_RAIN)
			{
				if (pEffect->pos.y <= 0)
				{
					ReleaseEffect(nCntEffect);
					nCntEffect--;
					continue;
				}
			}
		}

		/*g_aEffect[nCntEffect].fRadius -= 0.01f;*/
		pEffect->pos += pEffect->move;
		pEffect->nLife--;

		if (pEffect->nLife < 0)
		{
			ReleaseEffect(nCntEffect);
			nCntEffect--;
			continue;
		}
		if (pEffect->fRadius < 0)
		{
			ReleaseEffect(nCntEffect);
			nCntEffect--;
			continue;
		}

		pEffect++;
	}

	PrintDebugProc("エフェクトの使用数 : %d", g_nNumEffect);
}

//======================================================================================
// エフェクトの描画処理
//======================================================================================
void DrawEffect(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();
	D3DXMATRIX mtxRot, mtxTrans;
	D3DXMATRIX mtxView;
	D3DXMATRIX mtx;
	Effect* pEffect = &g_aEffect[0];

	//ビューマトリックスを取得
	pDevice->GetTransform(D3DTS_VIEW, &mtxView);

	VERTEX_3D* pVtx;    // 頂点情報の設定

	// 頂点バッファをロックし、頂点情報へのポインタを取得
	g_pVtxBuffEffect->Lock(0, 0, (void**)&pVtx, 0);

	for (int nCntEffect = 0; nCntEffect < g_nNumEffect; nCntEffect++, pEffect++)
	{
		// ワールドマトリックスの初期化
		D3DXMatrixIdentity(&pEffect->mtxWorld);

		// エフェクトをカメラに対して正面に向ける
		D3DXMatrixInverse(&pEffect->mtxWorld, NULL, &mtxView);	// 逆行列を求める

		pEffect->mtxWorld._41 = 0.0f;		// マトリックス(行列)の内容
		pEffect->mtxWorld._42 = 0.0f;
		pEffect->mtxWorld._43 = 0.0f;

		// 位置を反映
		D3DXMatrixTranslation(&mtxTrans, pEffect->pos.x, pEffect->pos.y, pEffect->pos.z);
		D3DXMatrixMultiply(&pEffect->mtxWorld, &pEffect->mtxWorld, &mtxTrans);

		// 頂点座標の設定
		pVtx[0].pos = D3DXVECTOR3(-pEffect->fRadius, pEffect->fRadius, 0.0f);
		pVtx[1].pos = D3DXVECTOR3(pEffect->fRadius, pEffect->fRadius, 0.0f);
		pVtx[2].pos = D3DXVECTOR3(-pEffect->fRadius, -pEffect->fRadius, 0.0f);
		pVtx[3].pos = D3DXVECTOR3(pEffect->fRadius, -pEffect->fRadius, 0.0f);

#if BATCHING
		// 頂点座標の設定
		pVtx[0].pos = *D3DXVec3TransformCoord(&pVtx[0].pos, &pVtx[0].pos, &pEffect->mtxWorld);
		pVtx[1].pos = *D3DXVec3TransformCoord(&pVtx[1].pos, &pVtx[1].pos, &pEffect->mtxWorld);
		pVtx[2].pos = *D3DXVec3TransformCoord(&pVtx[2].pos, &pVtx[2].pos, &pEffect->mtxWorld);
		pVtx[3].pos = *D3DXVec3TransformCoord(&pVtx[3].pos, &pVtx[3].pos, &pEffect->mtxWorld);
#endif
		// 頂点カラーの設定
		pVtx[0].col = pEffect->col;
		pVtx[1].col = pEffect->col;
		pVtx[2].col = pEffect->col;
		pVtx[3].col = pEffect->col;

		// テクスチャ座標の設定
		pVtx[0].tex = D3DXVECTOR2(EFFECTTEX_SPRIT * pEffect->tex, 0.0f);
		pVtx[1].tex = D3DXVECTOR2(EFFECTTEX_SPRIT * pEffect->tex + EFFECTTEX_SPRIT, 0.0f);
		pVtx[2].tex = D3DXVECTOR2(EFFECTTEX_SPRIT * pEffect->tex, 1.0f);
		pVtx[3].tex = D3DXVECTOR2(EFFECTTEX_SPRIT * pEffect->tex + EFFECTTEX_SPRIT, 1.0f);

		pVtx += 4;
	}

	g_pVtxBuffEffect->Unlock();

	SetFogEnable(false);		//一旦fogを消す

	// Zテストを無効にする
	pDevice->SetRenderState(D3DRS_ZFUNC, D3DCMP_ALWAYS);	// Zテストの比較方法を変更(Zバッファの前後関係に関わらず描画する)
	pDevice->SetRenderState(D3DRS_ZWRITEENABLE, FALSE);		// Zバッファ更新の有効/無効の設定

	//αブレンディングを加算合成して設定
	pDevice->SetRenderState(D3DRS_BLENDOP, D3DBLENDOP_ADD);
	pDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
	pDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_ONE);

#if BATCHING
	//ワールドマトリックスの初期化
	D3DXMatrixIdentity(&mtx);

	// ワールドマトリックスの設定
	pDevice->SetTransform(D3DTS_WORLD, &mtx);

	// 頂点バッファをデータストリームに設定
	pDevice->SetStreamSource(0, g_pVtxBuffEffect, 0, sizeof(VERTEX_3D));

	// インデックスバッファをデータストリームに設定
	pDevice->SetIndices(g_pIdxBuffEffect);

	// 頂点フォーマットの設定
	pDevice->SetFVF(FVF_VERTEX_3D);

	// テクスチャ
	pDevice->SetTexture(0, g_pTextureBuffEffect);

	// エフェクトの描画
	pDevice->DrawIndexedPrimitive(D3DPT_TRIANGLELIST,
		0,
		0,
		g_nNumEffect * 4,
		0,
		g_nNumEffect * 2);
#endif

#if !BATCHING
	for (int nCntEffect = 0; nCntEffect < g_nNumEffect; nCntEffect++, pEffect++)
	{
		if (pEffect->bUse == false)
		{
			PrintDebugProc("※データ異常 : DrawEffect\n");
			break;
		}

		//ワールドマトリックスの初期化
		D3DXMatrixIdentity(&pEffect->mtxWorldEffect);

		//ビューマトリックスを取得
		pDevice->GetTransform(D3DTS_VIEW, &mtxView);

		//エフェクトをカメラに対して正面に向ける
		D3DXMatrixInverse(&pEffect->mtxWorldEffect, NULL, &mtxView);	//逆行列を求める

		pEffect->mtxWorldEffect._41 = 0.0f;		//マトリックス(行列)の内容
		pEffect->mtxWorldEffect._42 = 0.0f;
		pEffect->mtxWorldEffect._43 = 0.0f;

		//位置を反映
		D3DXMatrixTranslation(&mtxTrans, pEffect->pos.x, pEffect->pos.y, pEffect->pos.z);
		D3DXMatrixMultiply(&pEffect->mtxWorldEffect, &pEffect->mtxWorldEffect, &mtxTrans);

		//ワールドマトリックスの設定
		pDevice->SetTransform(D3DTS_WORLD, &pEffect->mtxWorldEffect);

		//頂点バッファをデータストリームに設定
		pDevice->SetStreamSource(0, g_pVtxBuffEffect, 0, sizeof(VERTEX_3D));

		//頂点フォーマットの設定
		pDevice->SetFVF(FVF_VERTEX_3D);

		//テクスチャ
		pDevice->SetTexture(0, g_pTextureBuffEffect[pEffect->tex]);

		//エフェクトの描画
		pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, nCntEffect * 4, 2);
	}
#endif

	//αブレンディングを戻す
	pDevice->SetRenderState(D3DRS_BLENDOP, D3DBLENDOP_ADD);
	pDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
	pDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);

	// Zテストを有効にする
	pDevice->SetRenderState(D3DRS_ZFUNC, D3DCMP_LESSEQUAL);
	pDevice->SetRenderState(D3DRS_ZWRITEENABLE, TRUE);

	SetFogEnable(true);		//fogをtrueに
}

//======================================================================================
// エフェクトの設定処理
//======================================================================================
void SetEffect(EFFECT_TYPE type, EFFECT_TEX tex, D3DXVECTOR3 pos, D3DXVECTOR3 move, D3DXCOLOR col, int nLife, float fRadius)
{
	if (g_nNumEffect >= MAX_EFFECT)
	{// 最大まで出していたら返す
		return;
	}

	Effect* pEffect = &g_aEffect[g_nNumEffect];		// 対象へのポインタ

	pEffect->pos = pos;
	pEffect->move = move;
	pEffect->col = col;
	pEffect->type = type;
	pEffect->tex = tex;
	pEffect->fRadius = fRadius;
	pEffect->nLife = nLife;
	pEffect->bUse = true;

	VERTEX_3D* pVtx;    //頂点情報の設定
	//頂点バッファをロックし、頂点情報へのポインタを取得
	g_pVtxBuffEffect->Lock(0, 0, (void**)&pVtx, 0);

	pVtx += g_nNumEffect * 4;

	//頂点座標の設定
	pVtx[0].pos = D3DXVECTOR3(-pEffect->fRadius, pEffect->fRadius, 0.0f);
	pVtx[1].pos = D3DXVECTOR3(pEffect->fRadius, pEffect->fRadius, 0.0f);
	pVtx[2].pos = D3DXVECTOR3(-pEffect->fRadius, -pEffect->fRadius, 0.0f);
	pVtx[3].pos = D3DXVECTOR3(pEffect->fRadius, -pEffect->fRadius, 0.0f);

	//頂点カラーの設定
	pVtx[0].col = pEffect->col;
	pVtx[1].col = pEffect->col;
	pVtx[2].col = pEffect->col;
	pVtx[3].col = pEffect->col;

	g_pVtxBuffEffect->Unlock();

	g_nNumEffect++;						// 使用数を増やす
}

//======================================================================================
// エフェクトの解放処理
//======================================================================================
void ReleaseEffect(int nIdx)
{
	g_aEffect[nIdx] = g_aEffect[g_nNumEffect - 1];				// 最後尾の情報を代入
	memset(&g_aEffect[g_nNumEffect - 1], NULL, sizeof(Effect));	// 最後尾の情報をリセット
	g_nNumEffect--;												// 使用数を減らす
}