//=============================================================================
//
//	2Dエフェクトの管理 [2deffect.cpp]
//	Author : Kajino Hiyori
// 
//=============================================================================
#include "2deffect.h"
#include "fog.h"
#include "color.h"
#include "input.h"
#include "main.h"

// マクロ定義
#define EFFECT2DLIFE_DIS	(1)				// 寿命の減少量
#define EFFECT2DRADIUS_DIS	(0.1f)			// エフェクト半径の減少量
#define EFFECT2DALPHA_DIS	(0.08f)			// エフェクトアルファ値の減少量
#define EFFECT2DALPHA_ADD	(0.06f)			// エフェクトアルファ値の増加量
#define	REVISION_PI_2X		(D3DX_PI * 2)	// 角度補正
#define COEFFICIENT_ROT		(0.05f)			// 角度の補正係数
#define MAX_ALPHA			(1.0f)			// アルファ値の最大数
#define MIN_ALPHA			(0.0f)			// アルファ値の最小数
#define TEXTURE_DIVISION	(5)				// テクスチャの分割数
#define TEXTURE_SIZE		(0.2f)			// テクスチャのサイズ
#define SPELLTEX_SIZE		(50.0f)			// 文字テクスチャの大きさ

// グローバル変数
LPDIRECT3DTEXTURE9 g_pTexture2DEffect = NULL;			// テクスチャへのポインタ
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuff2DEffect = NULL;	// 頂点バッファのへのポインタ
Effect2D g_a2DEffect[MAX_EFFECT2D];

//========================================================================
// エフェクトの初期化処理
//========================================================================
void Init2DEffect(void)
{
	LPDIRECT3DDEVICE9 pDevice;

	// デバイスの取得
	pDevice = GetDevice();
	// テクスチャの読み込み
	D3DXCreateTextureFromFile(pDevice,"data\\TEXTURE\\grain.jpg", &g_pTexture2DEffect);

	// エフェクトの情報の初期化処理
	for (int nCnt2DEffect = 0; nCnt2DEffect < MAX_EFFECT2D; nCnt2DEffect++)
	{
		g_a2DEffect[nCnt2DEffect].type = EFFECTTYPE2D_NONE;
		g_a2DEffect[nCnt2DEffect].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		g_a2DEffect[nCnt2DEffect].move = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		g_a2DEffect[nCnt2DEffect].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
		g_a2DEffect[nCnt2DEffect].fRadius = EFFECT2D_RADIUS;
		g_a2DEffect[nCnt2DEffect].nLife = EFFECT2D_LIFE;
		g_a2DEffect[nCnt2DEffect].bUse = false;	// 使用していない状態にする
	}
	// 頂点バッファの生成
	pDevice->CreateVertexBuffer(sizeof(VERTEX_2D) * 4 * MAX_EFFECT2D,
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_2D,
		D3DPOOL_MANAGED,
		&g_pVtxBuff2DEffect,
		NULL);

	VERTEX_2D* pVtx;
	// 頂点バッファをロックし、頂点情報へのポインタを取得
	g_pVtxBuff2DEffect->Lock(0, 0, (void**)&pVtx, 0);

	for (int nCnt2DEffect = 0; nCnt2DEffect < MAX_EFFECT2D; nCnt2DEffect++)
	{
		// 位置の設定
		pVtx[0].pos.x = g_a2DEffect[nCnt2DEffect].pos.x - g_a2DEffect[nCnt2DEffect].fRadius;
		pVtx[0].pos.y = g_a2DEffect[nCnt2DEffect].pos.y - g_a2DEffect[nCnt2DEffect].fRadius;
		pVtx[0].pos.z = g_a2DEffect[nCnt2DEffect].pos.z;
		pVtx[1].pos.x = g_a2DEffect[nCnt2DEffect].pos.x + g_a2DEffect[nCnt2DEffect].fRadius;
		pVtx[1].pos.y = g_a2DEffect[nCnt2DEffect].pos.y - g_a2DEffect[nCnt2DEffect].fRadius;
		pVtx[1].pos.z = g_a2DEffect[nCnt2DEffect].pos.z;
		pVtx[2].pos.x = g_a2DEffect[nCnt2DEffect].pos.x - g_a2DEffect[nCnt2DEffect].fRadius;
		pVtx[2].pos.y = g_a2DEffect[nCnt2DEffect].pos.y + g_a2DEffect[nCnt2DEffect].fRadius;
		pVtx[2].pos.z = g_a2DEffect[nCnt2DEffect].pos.z;
		pVtx[3].pos.x = g_a2DEffect[nCnt2DEffect].pos.x + g_a2DEffect[nCnt2DEffect].fRadius;
		pVtx[3].pos.y = g_a2DEffect[nCnt2DEffect].pos.y + g_a2DEffect[nCnt2DEffect].fRadius;
		pVtx[3].pos.z = g_a2DEffect[nCnt2DEffect].pos.z;

		// rhwの設定
		pVtx[0].rhw = 1.0f;
		pVtx[1].rhw = 1.0f;
		pVtx[2].rhw = 1.0f;
		pVtx[3].rhw = 1.0f;

		// 色の設定
		pVtx[0].col = g_a2DEffect[nCnt2DEffect].col;
		pVtx[1].col = g_a2DEffect[nCnt2DEffect].col;
		pVtx[2].col = g_a2DEffect[nCnt2DEffect].col;
		pVtx[3].col = g_a2DEffect[nCnt2DEffect].col;

		// テクスチャ座標の設定
		pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
		pVtx[1].tex = D3DXVECTOR2(TEXTURE_SIZE, 0.0f);
		pVtx[2].tex = D3DXVECTOR2(0.0f, TEXTURE_SIZE);
		pVtx[3].tex = D3DXVECTOR2(TEXTURE_SIZE, TEXTURE_SIZE);

		pVtx += 4;
	}
	// 頂点バッファをアンロック
	g_pVtxBuff2DEffect->Unlock();
}

//========================================================================
// エフェクトの終了処理
//========================================================================
void Uninit2DEffect(void)
{
	// テクスチャの破棄
	if (g_pTexture2DEffect != NULL)
	{
		g_pTexture2DEffect->Release();
		g_pTexture2DEffect = NULL;
	}

	// 頂点バッファの破棄
	if (g_pVtxBuff2DEffect != NULL)
	{
		g_pVtxBuff2DEffect->Release();
		g_pVtxBuff2DEffect = NULL;
	}
}

//========================================================================
// エフェクトの更新処理
//========================================================================
void Update2DEffect(void)
{
	// 吸収エフェクトの変数を格納
	float fRotMove = 0;	// 現在の移動方向
	float fRotDest = 0;	// 目標の移動方向
	float fRotDiff = 0;	// 目標の移動方向までの差分

	// 頂点座標の更新
	VERTEX_2D* pVtx;
	// 頂点バッファをロックし、頂点情報へのポインタを取得
	g_pVtxBuff2DEffect->Lock(0, 0, (void**)&pVtx, 0);

	for (int nCnt2DEffect = 0; nCnt2DEffect < MAX_EFFECT2D; nCnt2DEffect++)
	{
		if (g_a2DEffect[nCnt2DEffect].bUse == true)
		{ // エフェクトが使用されている場合
#if 0
			switch (g_a2DEffect[nCnt2DEffect].type)
			{
			case EFFECTTYPE2D_NORMAL:	// 通常状態の場合
				
				break;

			case EFFECTTYPE2D_ABSORPTION:	// 吸収状態のとき、角度の補正を行う
				fRotMove = atan2f(g_a2DEffect[nCnt2DEffect].move.x, g_a2DEffect[nCnt2DEffect].move.y);	// 現在の移動方向
				fRotDest = atan2f(g_a2DEffect[nCnt2DEffect].dest.x - g_a2DEffect[nCnt2DEffect].pos.x, g_a2DEffect[nCnt2DEffect].dest.y - g_a2DEffect[nCnt2DEffect].pos.y);	// 目標の移動方向
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
				g_a2DEffect[nCnt2DEffect].move.x = sinf(fRotMove) *1;
				g_a2DEffect[nCnt2DEffect].move.y = cosf(fRotMove) *1;
				g_a2DEffect[nCnt2DEffect].move.z = 0.0f;
				
				break;
			}
#endif			
			// 各種更新
			g_a2DEffect[nCnt2DEffect].pos += g_a2DEffect[nCnt2DEffect].move;
			g_a2DEffect[nCnt2DEffect].nLife -= EFFECT2DLIFE_DIS;

			// 位置の設定
			pVtx[0].pos.x = g_a2DEffect[nCnt2DEffect].pos.x - g_a2DEffect[nCnt2DEffect].fRadius;
			pVtx[0].pos.y = g_a2DEffect[nCnt2DEffect].pos.y - g_a2DEffect[nCnt2DEffect].fRadius;
			pVtx[0].pos.z = g_a2DEffect[nCnt2DEffect].pos.z;
			pVtx[1].pos.x = g_a2DEffect[nCnt2DEffect].pos.x + g_a2DEffect[nCnt2DEffect].fRadius;
			pVtx[1].pos.y = g_a2DEffect[nCnt2DEffect].pos.y - g_a2DEffect[nCnt2DEffect].fRadius;
			pVtx[1].pos.z = g_a2DEffect[nCnt2DEffect].pos.z;
			pVtx[2].pos.x = g_a2DEffect[nCnt2DEffect].pos.x - g_a2DEffect[nCnt2DEffect].fRadius;
			pVtx[2].pos.y = g_a2DEffect[nCnt2DEffect].pos.y + g_a2DEffect[nCnt2DEffect].fRadius;
			pVtx[2].pos.z = g_a2DEffect[nCnt2DEffect].pos.z;
			pVtx[3].pos.x = g_a2DEffect[nCnt2DEffect].pos.x + g_a2DEffect[nCnt2DEffect].fRadius;
			pVtx[3].pos.y = g_a2DEffect[nCnt2DEffect].pos.y + g_a2DEffect[nCnt2DEffect].fRadius;
			pVtx[3].pos.z = g_a2DEffect[nCnt2DEffect].pos.z;

			// 色の設定
			pVtx[0].col = g_a2DEffect[nCnt2DEffect].col;
			pVtx[1].col = g_a2DEffect[nCnt2DEffect].col;
			pVtx[2].col = g_a2DEffect[nCnt2DEffect].col;
			pVtx[3].col = g_a2DEffect[nCnt2DEffect].col;
			// 画面外にエフェクトが出た場合
			if (g_a2DEffect[nCnt2DEffect].pos.x < 0 || g_a2DEffect[nCnt2DEffect].pos.x > SCREEN_WIDTH || g_a2DEffect[nCnt2DEffect].pos.y < 0 || g_a2DEffect[nCnt2DEffect].pos.y > SCREEN_HEIGHT)
			{
				g_a2DEffect[nCnt2DEffect].bUse = false;
			}
			// エフェクトの寿命が尽きた時
			if (g_a2DEffect[nCnt2DEffect].nLife <= 0)
			{
				g_a2DEffect[nCnt2DEffect].bUse = false;
			}
		}
		pVtx += 4;
	}

	// 頂点バッファをアンロック
	g_pVtxBuff2DEffect->Unlock();
}

//========================================================================
// エフェクトの描画処理
//========================================================================
void Draw2DEffect(void)
{
	int nCnt2DEffect;

	LPDIRECT3DDEVICE9 pDevice;	// デバイスへのポインタ
	// デバイスの取得
	pDevice = GetDevice();

	// 頂点バッファをデータストリームに設定
	pDevice->SetStreamSource(0, g_pVtxBuff2DEffect, 0, sizeof(VERTEX_2D));

	// 頂点フォーマットの設定
	pDevice->SetFVF(FVF_VERTEX_2D);
	
	//αブレンディングを加算合成して設定
	pDevice->SetRenderState(D3DRS_BLENDOP, D3DBLENDOP_ADD);
	pDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
	pDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_ONE);

	SetFogEnable(false);		// 霧を消す

	pDevice->SetRenderState(D3DRS_LIGHTING, FALSE);		// ライトを無効にする

	for (nCnt2DEffect = 0; nCnt2DEffect < MAX_EFFECT2D; nCnt2DEffect++)
	{ // エフェクトが使用されている場合
		if (g_a2DEffect[nCnt2DEffect].bUse == true)
		{
			// テクスチャの設定
			pDevice->SetTexture(0, g_pTexture2DEffect);
			// ポリゴンの描画
			pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, nCnt2DEffect * 4, 2);
		}
	}

	//αブレンディングを戻す
	pDevice->SetRenderState(D3DRS_BLENDOP, D3DBLENDOP_ADD);
	pDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
	pDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);

	SetFogEnable(true);		// 霧を戻す

	pDevice->SetRenderState(D3DRS_LIGHTING, TRUE);		// ライトを有効にする
}

//========================================================================
// エフェクトの設定処理
//========================================================================
void Set2DEffect(EFFECTTYPE2D type, D3DXVECTOR3 pos, D3DXVECTOR3 move, D3DXVECTOR3 dest, D3DXCOLOR col, float fRadius, int nLife)
{
	// 頂点座標の更新
	VERTEX_2D* pVtx;
	// 頂点バッファをロックし、頂点情報へのポインタを取得
	g_pVtxBuff2DEffect->Lock(0, 0, (void**)&pVtx, 0);

	// ランダム位置をローカルで設定
	float fWidth, fHeight = 0.0f;

	for (int nCnt2DEffect = 0; nCnt2DEffect < MAX_EFFECT2D; nCnt2DEffect++)
	{
		if (g_a2DEffect[nCnt2DEffect].bUse == false)
		{ // エフェクトが使用されていない場合
			g_a2DEffect[nCnt2DEffect].type = type;
			g_a2DEffect[nCnt2DEffect].pos = pos;
			g_a2DEffect[nCnt2DEffect].move = move;
			g_a2DEffect[nCnt2DEffect].dest = dest;
			g_a2DEffect[nCnt2DEffect].col = COLOR_YELLOW;
			g_a2DEffect[nCnt2DEffect].nLife = nLife;
			g_a2DEffect[nCnt2DEffect].fRadius = fRadius;

			// エフェクトの読み込み位置をランダムにする
			fWidth = (float)(rand() % TEXTURE_DIVISION) * 0.2f;
			fHeight = (float)(rand() % TEXTURE_DIVISION) * 0.2f;

			pVtx += (nCnt2DEffect * 4);

			// 位置の設定
			pVtx[0].pos.x = g_a2DEffect[nCnt2DEffect].pos.x - g_a2DEffect[nCnt2DEffect].fRadius;
			pVtx[0].pos.y = g_a2DEffect[nCnt2DEffect].pos.y - g_a2DEffect[nCnt2DEffect].fRadius;
			pVtx[0].pos.z = g_a2DEffect[nCnt2DEffect].pos.z;
			pVtx[1].pos.x = g_a2DEffect[nCnt2DEffect].pos.x + g_a2DEffect[nCnt2DEffect].fRadius;
			pVtx[1].pos.y = g_a2DEffect[nCnt2DEffect].pos.y - g_a2DEffect[nCnt2DEffect].fRadius;
			pVtx[1].pos.z = g_a2DEffect[nCnt2DEffect].pos.z;
			pVtx[2].pos.x = g_a2DEffect[nCnt2DEffect].pos.x - g_a2DEffect[nCnt2DEffect].fRadius;
			pVtx[2].pos.y = g_a2DEffect[nCnt2DEffect].pos.y + g_a2DEffect[nCnt2DEffect].fRadius;
			pVtx[2].pos.z = g_a2DEffect[nCnt2DEffect].pos.z;
			pVtx[3].pos.x = g_a2DEffect[nCnt2DEffect].pos.x + g_a2DEffect[nCnt2DEffect].fRadius;
			pVtx[3].pos.y = g_a2DEffect[nCnt2DEffect].pos.y + g_a2DEffect[nCnt2DEffect].fRadius;
			pVtx[3].pos.z = g_a2DEffect[nCnt2DEffect].pos.z;

			// 色の設定
			pVtx[0].col = g_a2DEffect[nCnt2DEffect].col;
			pVtx[1].col = g_a2DEffect[nCnt2DEffect].col;
			pVtx[2].col = g_a2DEffect[nCnt2DEffect].col;
			pVtx[3].col = g_a2DEffect[nCnt2DEffect].col;

			// テクスチャの読み込みをランダム化
			pVtx[0].tex = D3DXVECTOR2(fWidth,					fHeight);
			pVtx[1].tex = D3DXVECTOR2(fWidth + TEXTURE_SIZE,	fHeight);
			pVtx[2].tex = D3DXVECTOR2(fWidth,					fHeight + TEXTURE_SIZE);
			pVtx[3].tex = D3DXVECTOR2(fWidth + TEXTURE_SIZE,	fHeight + TEXTURE_SIZE);

			g_a2DEffect[nCnt2DEffect].bUse = true; // 使用している状態にする
			break;
		}
	}
	// 頂点バッファをアンロック
	g_pVtxBuff2DEffect->Unlock();
}
