//=============================================================================
//
//	魔法陣の処理 [magiccircle.cpp]
//	Author : SHUMA AIZU
// 
//=============================================================================

#include "main.h"
#include "magiccircle.h"
#include "magic.h"
#include "spellui.h"
#include "input.h"

//*****************************************************************************
// マクロ定義
//*****************************************************************************
#define MAGICCIRCLE_ROTATION			(0.01f)			// 魔法陣の回転量
#define MAGICCIRCLEOPEN_COEFFICIENT		(0.025f)		// 魔法陣の展開の速度係数
#define MAGICCIRCLECLAUSE_COEFFICIENT	(0.075f)		// 魔法陣の収束の速度係数

//*****************************************************************************
// グローバル変数
//*****************************************************************************
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffMagicCircle = NULL;	// 頂点バッファへのポインタ
MagicCircle g_amagiccircle[MAX_MAGICCIRCLE] = {};		// 魔法陣の情報

//=============================================================================
//	魔法陣の初期化処理
//=============================================================================
void InitMagicCircle(void)
{
	// デバイスの取得
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	MagicCircle* pMagicCircle = &g_amagiccircle[0];

	// 初期化
	for (int nCntMagicCircle = 0; nCntMagicCircle < MAX_MAGICCIRCLE; nCntMagicCircle++, pMagicCircle++)
	{
		pMagicCircle->pos = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
		pMagicCircle->rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		pMagicCircle->fRadius = 0.0f;
		pMagicCircle->fRadiusDest = MAGICCIRCLE_RADIUS;
		pMagicCircle->PosParent = nullptr;
		pMagicCircle->bUse = false;
	}

	// 頂点バッファの生成
	pDevice->CreateVertexBuffer(sizeof(VERTEX_3D) * 4 * MAX_MAGICCIRCLE,
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_3D,
		D3DPOOL_MANAGED,
		&g_pVtxBuffMagicCircle,
		NULL);

	// 初期化
	VERTEX_3D *pVtx;			// 頂点情報へのポインタ

	// 頂点バッファをロックし,頂点情報へのポインタを取得
	g_pVtxBuffMagicCircle->Lock(0, 0, (void * *)&pVtx, 0);

	for (int nCntMagicCircle = 0; nCntMagicCircle < MAX_MAGICCIRCLE; nCntMagicCircle++)
	{
		// 頂点座標の設定
		pVtx[0].pos = INIT_D3DXVEC3;
		pVtx[1].pos = INIT_D3DXVEC3;
		pVtx[2].pos = INIT_D3DXVEC3;
		pVtx[3].pos = INIT_D3DXVEC3;

		// 法線ベクトルの設定
		pVtx[0].nor = NORMAL_PLANE;
		pVtx[1].nor = NORMAL_PLANE;
		pVtx[2].nor = NORMAL_PLANE;
		pVtx[3].nor = NORMAL_PLANE;

		// 頂点カラーの設定
		pVtx[0].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.5f);
		pVtx[1].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.5f);
		pVtx[2].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.5f);
		pVtx[3].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.5f);

		// テクスチャ座標の設定
		pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
		pVtx[1].tex = D3DXVECTOR2(1.0f, 0.0f);
		pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
		pVtx[3].tex = D3DXVECTOR2(1.0f, 1.0f);

		pVtx += 4;
	}

	// 頂点バッファをアンロックする
	g_pVtxBuffMagicCircle->Unlock();
}

//=============================================================================
//	魔法陣の終了処理
//=============================================================================
void UninitMagicCircle(void)
{	
	// 頂点バッファの破棄
	if (g_pVtxBuffMagicCircle != NULL)
	{
		g_pVtxBuffMagicCircle->Release();
		g_pVtxBuffMagicCircle = NULL;
	}
}

//=============================================================================
//	魔法陣の描画処理
//=============================================================================
void DrawMagicCircle(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();			// デバイスの取得
	MagicCircle* pMagicCircle = &g_amagiccircle[0];		// 先頭アドレス
	D3DXMATRIX mtxRot, mtxTrans;						// 計算用マトリックス

	// ライトをオフにする
	pDevice->SetRenderState(D3DRS_LIGHTING, FALSE);

	for (int nCntMagicCircle = 0; nCntMagicCircle < MAX_MAGICCIRCLE; nCntMagicCircle++, pMagicCircle++)
	{
		if (pMagicCircle->bUse == false)
		{
			continue;
		}

		// ワールドマトリックスの初期化
		D3DXMatrixIdentity(&pMagicCircle->mtxWorld);

		// 向きを反映
		D3DXMatrixRotationYawPitchRoll(&mtxRot, pMagicCircle->rot.y, pMagicCircle->rot.x, pMagicCircle->rot.z);
		D3DXMatrixMultiply(&pMagicCircle->mtxWorld, &pMagicCircle->mtxWorld, &mtxRot);

		// 位置を反映
		D3DXMatrixTranslation(&mtxTrans, pMagicCircle->pos.x, pMagicCircle->pos.y, pMagicCircle->pos.z);
		D3DXMatrixMultiply(&pMagicCircle->mtxWorld, &pMagicCircle->mtxWorld, &mtxTrans);

		// ワールドマトリックスの設定
		pDevice->SetTransform(D3DTS_WORLD, &pMagicCircle->mtxWorld);

		// 頂点バッファをデータストリームに設定
		pDevice->SetStreamSource(0, g_pVtxBuffMagicCircle, 0, sizeof(VERTEX_3D));

		// 頂点フォーマットの設定
		pDevice->SetFVF(FVF_VERTEX_3D);

		// テクスチャの設定
		pDevice->SetTexture(0, GetSpellTexture((SPELLUI_TEX)pMagicCircle->MagicType));

		// 魔法陣の描画
		pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, nCntMagicCircle * 4, 2);
	}

	// ライトをオンにする
	pDevice->SetRenderState(D3DRS_LIGHTING, FALSE);
}

//=============================================================================
//	魔法陣の更新処理
//=============================================================================
void UpdateMagicCircle(void)
{
	MagicCircle* pMagicCircle = &g_amagiccircle[0];

	// 初期化
	VERTEX_3D* pVtx;			// 頂点情報へのポインタ

	// 頂点バッファをロックし,頂点情報へのポインタを取得
	g_pVtxBuffMagicCircle->Lock(0, 0, (void**)&pVtx, 0);

	for (int nCntMagicCircle = 0; nCntMagicCircle < MAX_MAGICCIRCLE; nCntMagicCircle++, pMagicCircle++, pVtx += 4)
	{
		if (pMagicCircle->bUse == false)
		{// 使っていなければ弾く
			continue;
		}

		// 位置をプレイヤーと同期
		pMagicCircle->pos.x = pMagicCircle->PosParent->x;
		pMagicCircle->pos.z = pMagicCircle->PosParent->z;

		pMagicCircle->rot.y += MAGICCIRCLE_ROTATION;				// 魔法陣を回す

		pMagicCircle->rot.y = AngleNormalize(pMagicCircle->rot.y);	// 範囲内に

		pMagicCircle->nLife--;										// 寿命を減らす

		if (pMagicCircle->nLife > 0)
		{// 寿命があれば展開
			pMagicCircle->fRadius += (pMagicCircle->fRadiusDest - pMagicCircle->fRadius) * MAGICCIRCLEOPEN_COEFFICIENT;
		}
		else
		{// なければ収束
			pMagicCircle->fRadius += (0.0f - pMagicCircle->fRadius) * MAGICCIRCLECLAUSE_COEFFICIENT;
		}

		// 頂点座標の設定
		pVtx[0].pos = D3DXVECTOR3(-pMagicCircle->fRadius, 0.0f, pMagicCircle->fRadius);
		pVtx[1].pos = D3DXVECTOR3(pMagicCircle->fRadius, 0.0f, pMagicCircle->fRadius);
		pVtx[2].pos = D3DXVECTOR3(-pMagicCircle->fRadius, 0.0f, -pMagicCircle->fRadius);
		pVtx[3].pos = D3DXVECTOR3(pMagicCircle->fRadius, 0.0f, -pMagicCircle->fRadius);

		if (pMagicCircle->fRadius <= 0.1f && pMagicCircle->nLife < 0)
		{// 寿命がなくなっているかつ半径が0に近ければ終了する
			pMagicCircle->bUse = false;
		}
	}

	// 頂点バッファをアンロックする
	g_pVtxBuffMagicCircle->Unlock();
}

//=============================================================================
//	魔法陣の設定処理
//=============================================================================
void SetMagicCircle(MAGICTYPE MagicType, D3DXVECTOR3* PosParent)
{
	MagicCircle* pMagicCircle = &g_amagiccircle[0];

	for (int nCntMagicCircle = 0; nCntMagicCircle < MAX_MAGICCIRCLE; nCntMagicCircle++, pMagicCircle++)
	{
		if (pMagicCircle->bUse == true)
		{
			continue;
		}

		// 魔法陣の設定
		pMagicCircle->MagicType = MagicType;
		pMagicCircle->PosParent = PosParent;
		pMagicCircle->nLife = 300;
		pMagicCircle->bUse = true;
		break;
	}
}

//=============================================================================
//	魔法陣の処理
//=============================================================================
void SetPositionMagicCircle(int IdxMagicCircle, D3DXVECTOR3 pos)
{
	g_amagiccircle[IdxMagicCircle].pos = pos;
}