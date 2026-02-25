//=============================================================================
//
//	2Dエフェクトの管理 [2deffect.cpp]
//	Author : Kajino Hiyori
// 
//=============================================================================
#include "2deffect.h"
#include "main.h"

#if 0
// マクロ定義
#define EFFECTLIFE_DIS		(2)				// 寿命の減少量
#define EFFECTRADIUS_DIS	(0.1f)			// エフェクト半径の減少量
#define EFFECTALPHA_DIS		(0.08f)			// エフェクトアルファ値の減少量
#define EFFECTALPHA_ADD		(0.06f)			// エフェクトアルファ値の増加量
#define	REVISION_PI_2X		(D3DX_PI * 2)	// 角度補正
#define COEFFICIENT_ROT		(0.05f)			// 角度の補正係数
#define MAX_ALPHA			(1.0f)			// アルファ値の最大数
#define MIN_ALPHA			(0.0f)			// アルファ値の最小数

// グローバル変数
LPDIRECT3DTEXTURE9 g_pTextureEffect = NULL;			// テクスチャへのポインタ
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffEffect = NULL;	// 頂点バッファのへのポインタ
Effect g_aEffect[MAX_EFFECT];

//========================================================================
// エフェクトの初期化処理
//========================================================================
void InitEffect(void)
{
	LPDIRECT3DDEVICE9 pDevice;

	// デバイスの取得
	pDevice = GetDevice();
	// テクスチャの読み込み
	D3DXCreateTextureFromFile(pDevice,"data\\TEXTURE\\effect000.jpg", &g_pTextureEffect);

	// エフェクトの情報の初期化処理
	for (int nCntEffect = RESET_DATA; nCntEffect < MAX_EFFECT; nCntEffect++)
	{
		g_aEffect[nCntEffect].type = EFFECTTYPE_NONE;
		g_aEffect[nCntEffect].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		g_aEffect[nCntEffect].move = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		g_aEffect[nCntEffect].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
		g_aEffect[nCntEffect].fRadius = EFFECT_RADIUS;
		g_aEffect[nCntEffect].nLife = EFFECT_LIFE;
		g_aEffect[nCntEffect].bUse = false;	// 使用していない状態にする
	}
	// 頂点バッファの生成
	pDevice->CreateVertexBuffer(sizeof(VERTEX_2D) * 4 * MAX_EFFECT,
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_2D,
		D3DPOOL_MANAGED,
		&g_pVtxBuffEffect,
		NULL);

	VERTEX_2D* pVtx;
	// 頂点バッファをロックし、頂点情報へのポインタを取得
	g_pVtxBuffEffect->Lock(0, 0, (void**)&pVtx, 0);

	for (int nCntEffect = RESET_DATA; nCntEffect < MAX_EFFECT; nCntEffect++)
	{
		// 位置の設定
		pVtx[0].pos.x = g_aEffect[nCntEffect].pos.x - g_aEffect[nCntEffect].fRadius;
		pVtx[0].pos.y = g_aEffect[nCntEffect].pos.y - g_aEffect[nCntEffect].fRadius;
		pVtx[0].pos.z = g_aEffect[nCntEffect].pos.z;
		pVtx[1].pos.x = g_aEffect[nCntEffect].pos.x + g_aEffect[nCntEffect].fRadius;
		pVtx[1].pos.y = g_aEffect[nCntEffect].pos.y - g_aEffect[nCntEffect].fRadius;
		pVtx[1].pos.z = g_aEffect[nCntEffect].pos.z;
		pVtx[2].pos.x = g_aEffect[nCntEffect].pos.x - g_aEffect[nCntEffect].fRadius;
		pVtx[2].pos.y = g_aEffect[nCntEffect].pos.y + g_aEffect[nCntEffect].fRadius;
		pVtx[2].pos.z = g_aEffect[nCntEffect].pos.z;
		pVtx[3].pos.x = g_aEffect[nCntEffect].pos.x + g_aEffect[nCntEffect].fRadius;
		pVtx[3].pos.y = g_aEffect[nCntEffect].pos.y + g_aEffect[nCntEffect].fRadius;
		pVtx[3].pos.z = g_aEffect[nCntEffect].pos.z;

		// rhwの設定
		pVtx[0].rhw = 1.0f;
		pVtx[1].rhw = 1.0f;
		pVtx[2].rhw = 1.0f;
		pVtx[3].rhw = 1.0f;

		// 色の設定
		pVtx[0].col = g_aEffect[nCntEffect].col;
		pVtx[1].col = g_aEffect[nCntEffect].col;
		pVtx[2].col = g_aEffect[nCntEffect].col;
		pVtx[3].col = g_aEffect[nCntEffect].col;

		// テクスチャ座標の設定
		pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
		pVtx[1].tex = D3DXVECTOR2(1.0f, 0.0f);
		pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
		pVtx[3].tex = D3DXVECTOR2(1.0f, 1.0f);

		pVtx += 4;
	}
	// 頂点バッファをアンロック
	g_pVtxBuffEffect->Unlock();
}

//========================================================================
// エフェクトの終了処理
//========================================================================
void UninitEffect(void)
{
	// テクスチャの破棄
	if (g_pTextureEffect != NULL)
	{
		g_pTextureEffect->Release();
		g_pTextureEffect = NULL;
	}

	// 頂点バッファの破棄
	if (g_pVtxBuffEffect != NULL)
	{
		g_pVtxBuffEffect->Release();
		g_pVtxBuffEffect = NULL;
	}
}

//========================================================================
// エフェクトの更新処理
//========================================================================
void UpdateEffect(void)
{
	// 吸収エフェクトの変数を格納
	float fRotMove = RESET_DATA;	// 現在の移動方向
	float fRotDest = RESET_DATA;	// 目標の移動方向
	float fRotDiff = RESET_DATA;	// 目標の移動方向までの差分

	// プレイヤー情報の取得
	Player* pPlayer = GetPlayer();

	// 頂点座標の更新
	VERTEX_2D* pVtx;
	// 頂点バッファをロックし、頂点情報へのポインタを取得
	g_pVtxBuffEffect->Lock(0, 0, (void**)&pVtx, 0);

	for (int nCntEffect = 0; nCntEffect < MAX_EFFECT; nCntEffect++)
	{
		if (g_aEffect[nCntEffect].bUse == true)
		{ // エフェクトが使用されている場合
#if 0
			switch (g_aEffect[nCntEffect].type)
			{
			case EFFECTTYPE_NORMAL:	// 通常状態の場合
				
				break;

			case EFFECTTYPE_ABSORPTION:	// 吸収状態のとき、角度の補正を行う
				fRotMove = atan2f(g_aEffect[nCntEffect].move.x, g_aEffect[nCntEffect].move.y);	// 現在の移動方向
				fRotDest = atan2f(g_aEffect[nCntEffect].dest.x - g_aEffect[nCntEffect].pos.x, g_aEffect[nCntEffect].dest.y - g_aEffect[nCntEffect].pos.y);	// 目標の移動方向
				fRotDiff = fRotDest - fRotMove;	// 目標の移動方向までの差分
				// 角度の補正を行う
				if (fRotDiff > D3DX_PI)
				{
					fRotDiff -= REVISION_PI_2X;
				}
				else if (fRotDiff < -D3DX_PI)
				{
					fRotDiff += REVISION_PI_2X;
				}
				fRotMove += fRotDiff * COEFFICIENT_ROT;	// 角度の補正
				// 角度の補正を行う
				if (fRotMove > D3DX_PI)
				{
					fRotMove -= REVISION_PI_2X;
				}
				else if (fRotMove < -D3DX_PI)
				{
					fRotMove += REVISION_PI_2X;
				}
				g_aEffect[nCntEffect].move.x = sinf(fRotMove) *1;
				g_aEffect[nCntEffect].move.y = cosf(fRotMove) *1;
				g_aEffect[nCntEffect].move.z = 0.0f;
				
				break;
			}
#endif			
			// 各種更新
			g_aEffect[nCntEffect].pos += g_aEffect[nCntEffect].move;
			g_aEffect[nCntEffect].nLife -= EFFECTLIFE_DIS;

			// エフェクトも重力に従う
			g_aEffect[nCntEffect].move.y += GRAVITY;

			// 位置の設定
			pVtx[0].pos.x = g_aEffect[nCntEffect].pos.x - g_aEffect[nCntEffect].fRadius;
			pVtx[0].pos.y = g_aEffect[nCntEffect].pos.y - g_aEffect[nCntEffect].fRadius;
			pVtx[0].pos.z = g_aEffect[nCntEffect].pos.z;
			pVtx[1].pos.x = g_aEffect[nCntEffect].pos.x + g_aEffect[nCntEffect].fRadius;
			pVtx[1].pos.y = g_aEffect[nCntEffect].pos.y - g_aEffect[nCntEffect].fRadius;
			pVtx[1].pos.z = g_aEffect[nCntEffect].pos.z;
			pVtx[2].pos.x = g_aEffect[nCntEffect].pos.x - g_aEffect[nCntEffect].fRadius;
			pVtx[2].pos.y = g_aEffect[nCntEffect].pos.y + g_aEffect[nCntEffect].fRadius;
			pVtx[2].pos.z = g_aEffect[nCntEffect].pos.z;
			pVtx[3].pos.x = g_aEffect[nCntEffect].pos.x + g_aEffect[nCntEffect].fRadius;
			pVtx[3].pos.y = g_aEffect[nCntEffect].pos.y + g_aEffect[nCntEffect].fRadius;
			pVtx[3].pos.z = g_aEffect[nCntEffect].pos.z;

			// 色の設定
			pVtx[0].col = g_aEffect[nCntEffect].col;
			pVtx[1].col = g_aEffect[nCntEffect].col;
			pVtx[2].col = g_aEffect[nCntEffect].col;
			pVtx[3].col = g_aEffect[nCntEffect].col;
			// 画面外にエフェクトが出た場合
			if (g_aEffect[nCntEffect].pos.x < 0 || g_aEffect[nCntEffect].pos.x > SCREEN_WIDTH || g_aEffect[nCntEffect].pos.y < 0 || g_aEffect[nCntEffect].pos.y > SCREEN_HEIGHT)
			{
				g_aEffect[nCntEffect].bUse = false;
			}
			// エフェクトの寿命が尽きた時
			if (g_aEffect[nCntEffect].nLife <= 0)
			{
				g_aEffect[nCntEffect].bUse = false;
			}
		}
		pVtx += 4;
	}

	// 頂点バッファをアンロック
	g_pVtxBuffEffect->Unlock();
}

//========================================================================
// エフェクトの描画処理
//========================================================================
void DrawEffect(void)
{
	int nCntEffect;

	LPDIRECT3DDEVICE9 pDevice;	// デバイスへのポインタ
	// デバイスの取得
	pDevice = GetDevice();

	// 頂点バッファをデータストリームに設定
	pDevice->SetStreamSource(0, g_pVtxBuffEffect, 0, sizeof(VERTEX_2D));

	// 頂点フォーマットの設定
	pDevice->SetFVF(FVF_VERTEX_2D);
	
#if 0
	// アルファブレンディングを加算合成に設定
	pDevice->SetRenderState(D3DRS_BLENDOP, D3DBLENDOP_ADD);
	pDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
	pDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_ONE);
#endif

	for (nCntEffect = 0; nCntEffect < MAX_EFFECT; nCntEffect++)
	{ // エフェクトが使用されている場合
		if (g_aEffect[nCntEffect].bUse == true)
		{
			// テクスチャの設定
			pDevice->SetTexture(0, NULL);
			// ポリゴンの描画
			pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, nCntEffect * 4, 2);
		}
	}

#if 0
	// アルファブレンディングを元に戻す
	pDevice->SetRenderState(D3DRS_BLENDOP, D3DBLENDOP_ADD);
	pDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
	pDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);
#endif
}

//========================================================================
// エフェクトの設定処理
//========================================================================
void SetEffect(EFFECTTYPE type, D3DXVECTOR3 pos, D3DXVECTOR3 move, D3DXVECTOR3 dest, D3DXCOLOR col, float fRadius, int nLife)
{
	// 頂点座標の更新
	VERTEX_2D* pVtx;
	// 頂点バッファをロックし、頂点情報へのポインタを取得
	g_pVtxBuffEffect->Lock(0, 0, (void**)&pVtx, 0);

	for (int nCntEffect = 0; nCntEffect < MAX_EFFECT; nCntEffect++)
	{
		if (g_aEffect[nCntEffect].bUse == false)
		{ // エフェクトが使用されていない場合
			g_aEffect[nCntEffect].type = type;
			g_aEffect[nCntEffect].pos = pos;
			g_aEffect[nCntEffect].move = move;
			g_aEffect[nCntEffect].dest = dest;
			g_aEffect[nCntEffect].col = col;
			g_aEffect[nCntEffect].nLife = nLife;
			g_aEffect[nCntEffect].fRadius = fRadius;

			pVtx += (nCntEffect * 4);
			// 位置の設定
			pVtx[0].pos.x = g_aEffect[nCntEffect].pos.x - g_aEffect[nCntEffect].fRadius;
			pVtx[0].pos.y = g_aEffect[nCntEffect].pos.y - g_aEffect[nCntEffect].fRadius;
			pVtx[0].pos.z = g_aEffect[nCntEffect].pos.z;
			pVtx[1].pos.x = g_aEffect[nCntEffect].pos.x + g_aEffect[nCntEffect].fRadius;
			pVtx[1].pos.y = g_aEffect[nCntEffect].pos.y - g_aEffect[nCntEffect].fRadius;
			pVtx[1].pos.z = g_aEffect[nCntEffect].pos.z;
			pVtx[2].pos.x = g_aEffect[nCntEffect].pos.x - g_aEffect[nCntEffect].fRadius;
			pVtx[2].pos.y = g_aEffect[nCntEffect].pos.y + g_aEffect[nCntEffect].fRadius;
			pVtx[2].pos.z = g_aEffect[nCntEffect].pos.z;
			pVtx[3].pos.x = g_aEffect[nCntEffect].pos.x + g_aEffect[nCntEffect].fRadius;
			pVtx[3].pos.y = g_aEffect[nCntEffect].pos.y + g_aEffect[nCntEffect].fRadius;
			pVtx[3].pos.z = g_aEffect[nCntEffect].pos.z;

			// 色の設定
			pVtx[0].col = g_aEffect[nCntEffect].col;
			pVtx[1].col = g_aEffect[nCntEffect].col;
			pVtx[2].col = g_aEffect[nCntEffect].col;
			pVtx[3].col = g_aEffect[nCntEffect].col;

			g_aEffect[nCntEffect].bUse = true; // 使用している状態にする
			break;
		}
	}
	// 頂点バッファをアンロック
	g_pVtxBuffEffect->Unlock();
}

//========================================================================
// エフェクトの取得
//========================================================================
Effect* GetEffect(void)
{
	return &g_aEffect[RESET_DATA];	// エフェクト情報の先頭アドレスを渡す
}
#endif