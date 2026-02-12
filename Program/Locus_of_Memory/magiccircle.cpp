//=============================================================================
//
//	魔法陣の処理 [magiccircle.cpp]
//	Author : SHUMA AIZU
// 
//=============================================================================

#include "main.h"
#include "magiccircle.h"
#include "input.h"

//*****************************************************************************
// マクロ定義
//*****************************************************************************

//*****************************************************************************
// グローバル変数
//*****************************************************************************
LPDIRECT3DTEXTURE9 g_pTextureMagicCircle = NULL;		// テクスチャへのポインタ
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffMagicCircle = NULL;	// 頂点バッファへのポインタ
MagicCircle g_amagiccircle[MAX_MAGICCIRCLE] = {};		// 魔法陣の情報

//=============================================================================
//	魔法陣の初期化処理
//=============================================================================
void InitMagicCircle(void)
{
	// デバイスの取得
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	// テクスチャの読み込み
	D3DXCreateTextureFromFile(pDevice,
		"data\\TEXTURE\\magiccircle000.jpg",
		&g_pTextureMagicCircle);

	for (int nCntMagicCircle = 0; nCntMagicCircle < MAX_MAGICCIRCLE; nCntMagicCircle++)
	{
		g_amagiccircle[nCntMagicCircle].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
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
		pVtx[0].pos = D3DXVECTOR3(MAGICCIRCLE_WIDTH, MAGICCIRCLE_HEIGHT, 0.0f);
		pVtx[1].pos = D3DXVECTOR3(-MAGICCIRCLE_WIDTH, MAGICCIRCLE_HEIGHT, 0.0f);
		pVtx[2].pos = D3DXVECTOR3(MAGICCIRCLE_WIDTH, 0.0f, 0.0f);
		pVtx[3].pos = D3DXVECTOR3(-MAGICCIRCLE_WIDTH, 0.0f, 0.0f);

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
	// テクスチャの破棄
	if (g_pTextureMagicCircle != NULL)
	{
		g_pTextureMagicCircle->Release();
		g_pTextureMagicCircle = NULL;
	}
	

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
	LPDIRECT3DDEVICE9 pDevice = GetDevice();	// デバイスの取得
	D3DXMATRIX mtxRot, mtxTrans;				// 計算用マトリックス

	for (int nCntMagicCircle = 0; nCntMagicCircle < MAX_MAGICCIRCLE; nCntMagicCircle++)
	{
		// ワールドマトリックスの初期化
		D3DXMatrixIdentity(&g_amagiccircle[nCntMagicCircle].mtxWorld);

		// 向きを反映
		D3DXMatrixRotationYawPitchRoll(&mtxRot, g_amagiccircle[nCntMagicCircle].rot.y, g_amagiccircle[nCntMagicCircle].rot.x, g_amagiccircle[nCntMagicCircle].rot.z);
		D3DXMatrixMultiply(&g_amagiccircle[nCntMagicCircle].mtxWorld, &g_amagiccircle[nCntMagicCircle].mtxWorld, &mtxRot);

		// 位置を反映
		D3DXMatrixTranslation(&mtxTrans, g_amagiccircle[nCntMagicCircle].pos.x, g_amagiccircle[nCntMagicCircle].pos.y, g_amagiccircle[nCntMagicCircle].pos.z);
		D3DXMatrixMultiply(&g_amagiccircle[nCntMagicCircle].mtxWorld, &g_amagiccircle[nCntMagicCircle].mtxWorld, &mtxTrans);

		// ワールドマトリックスの設定
		pDevice->SetTransform(D3DTS_WORLD, &g_amagiccircle[nCntMagicCircle].mtxWorld);

		// 頂点バッファをデータストリームに設定
		pDevice->SetStreamSource(0, g_pVtxBuffMagicCircle, 0, sizeof(VERTEX_3D));

		// 頂点フォーマットの設定
		pDevice->SetFVF(FVF_VERTEX_3D);

		// テクスチャの設定
		pDevice->SetTexture(0, g_pTextureMagicCircle);

		if (g_amagiccircle[nCntMagicCircle].bUse == true)
		{
			// 魔法陣の描画
			pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, nCntMagicCircle * 4, 2);
		}
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
void SetMagicCircle(D3DXVECTOR3 pos, D3DXVECTOR3 rot)
{
	for (int nCntMagicCircle = 0; nCntMagicCircle < MAX_MAGICCIRCLE; nCntMagicCircle++)
	{
		if (g_amagiccircle[nCntMagicCircle].bUse == false)
		{
			// 魔法陣の設定
			g_amagiccircle[nCntMagicCircle].pos = pos;
			g_amagiccircle[nCntMagicCircle].rot = rot;
			g_amagiccircle[nCntMagicCircle].bUse = true;
			break;
		}
	}
}

//=============================================================================
//	魔法陣の処理
//=============================================================================
void SetPositionMagicCircle(int IdxMagicCircle, D3DXVECTOR3 pos)
{
	g_amagiccircle[IdxMagicCircle].pos = pos;
}