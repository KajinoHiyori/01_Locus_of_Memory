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

	for (int nCntMagicCircle = 0; nCntMagicCircle < MAX_MAGICCIRCLE; nCntMagicCircle++)
	{
		g_amagiccircle[nCntMagicCircle].pos = D3DXVECTOR3(0.0f, 10.0f, 500.0f);
		g_amagiccircle[nCntMagicCircle].rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		g_amagiccircle[nCntMagicCircle].bUse = false;
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
		pVtx[0].pos = D3DXVECTOR3(-MAGICCIRCLE_RADIUS, 0.0f, MAGICCIRCLE_RADIUS);
		pVtx[1].pos = D3DXVECTOR3(MAGICCIRCLE_RADIUS, 0.0f, MAGICCIRCLE_RADIUS);
		pVtx[2].pos = D3DXVECTOR3(-MAGICCIRCLE_RADIUS, 0.0f, -MAGICCIRCLE_RADIUS);
		pVtx[3].pos = D3DXVECTOR3(MAGICCIRCLE_RADIUS, 0.0f, -MAGICCIRCLE_RADIUS);

		// 法線ベクトルの設定
		pVtx[0].nor = NORMAL_PLANE;
		pVtx[1].nor = NORMAL_PLANE;
		pVtx[2].nor = NORMAL_PLANE;
		pVtx[3].nor = NORMAL_PLANE;

		// 頂点カラーの設定
		pVtx[0].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
		pVtx[1].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
		pVtx[2].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
		pVtx[3].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);

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

		D3DXMatrixMultiply(&pMagicCircle->mtxWorld, &pMagicCircle->mtxWorld, pMagicCircle->mtxParent);

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
}

//=============================================================================
//	魔法陣の更新処理
//=============================================================================
void UpdateMagicCircle(void)
{

}

//=============================================================================
//	魔法陣の設定処理
//=============================================================================
void SetMagicCircle(MAGICTYPE MagicType, D3DXMATRIX* mtxParent)
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
		pMagicCircle->mtxParent = mtxParent;
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