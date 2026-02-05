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

// マクロ定義
#define MAX_EFFECT		(16384)				// エフェクトの最大数
#define EFFECT_RADIUS	(25.0f)				// エフェクトの半径
#define MAX_EFFECTTEX	(EFFECT_TEX_MAX)	// エフェクトのテクスチャの数

//構造体の定義
typedef struct
{
	D3DXVECTOR3 pos;			// 位置
	D3DXVECTOR3 move;			// 移動量
	D3DXCOLOR	col;			// 色
	D3DXMATRIX	mtxWorldEffect;	// ワールドマトリックス
	EFFECT_TYPE	type;			// エフェクトの種類
	EFFECT_TEX	tex;			// エフェクトのテクスチャの種類
	float		fRadius;		// 半径
	int			nLife;			// 寿命（色）
	bool		bUse;			// 使用しているか
}Effect;

// テクスチャの読み込み
const char* c_apFilenameEffect[MAX_EFFECTTEX] =
{
	"data\\TEXTURE\\effect000.jpg",
	"data\\TEXTURE\\effect001.jpg",
};

//グローバル変数
LPDIRECT3DTEXTURE9 g_pTextureBuffEffect[MAX_EFFECTTEX] = {};
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffEffect = NULL;
Effect g_aEffect[MAX_EFFECT];

//======================================================================================
// エフェクトの初期化処理
//======================================================================================
void InitEffect(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	//テクスチャ読み込み
	for (int nCntEffect = 0; nCntEffect < MAX_EFFECTTEX; nCntEffect++)
	{
		D3DXCreateTextureFromFile(pDevice, c_apFilenameEffect[nCntEffect], &g_pTextureBuffEffect[nCntEffect]);
	}

	//初期化
	for (int nCntEffect = 0; nCntEffect < MAX_EFFECT; nCntEffect++)
	{
		g_aEffect[nCntEffect].pos		= D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		g_aEffect[nCntEffect].move		= D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		g_aEffect[nCntEffect].col		= D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
		g_aEffect[nCntEffect].type		= EFFECT_TYPE_NORMAL;
		g_aEffect[nCntEffect].tex		= EFFECT_TEX_CIRCLE;
		g_aEffect[nCntEffect].fRadius	= EFFECT_RADIUS;
		g_aEffect[nCntEffect].nLife		= 0;
		g_aEffect[nCntEffect].bUse		= false;
	}

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
}

//======================================================================================
// エフェクトの終了処理
//======================================================================================
void UninitEffect(void)
{
	//テクスチャの破棄
	for (int nCntEffect = 0; nCntEffect < MAX_EFFECTTEX; nCntEffect++)
	{
		if (g_pTextureBuffEffect[nCntEffect] != NULL)
		{
			g_pTextureBuffEffect[nCntEffect]->Release();
			g_pTextureBuffEffect[nCntEffect] = NULL;
		}
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
	VERTEX_3D* pVtx;    //頂点情報の設定

	Camera* pCamera = GetCamera();

	if (GetKeyboardTrigger(DIK_SPACE) == true)
	{
		//SetEffect(D3DXVECTOR3(0.0f, 50.0f, 0.0f), D3DXVECTOR3(0.0f,50.0f,0.0f), D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f), 20);
	}

	//頂点バッファをロックし、頂点情報へのポインタを取得
	g_pVtxBuffEffect->Lock(0, 0, (void**)&pVtx, 0);

	for (int nCntEffect = 0; nCntEffect < MAX_EFFECT; nCntEffect++, pVtx += 4)
	{
		if (g_aEffect[nCntEffect].bUse == true)
		{
			//頂点座標の設定
			pVtx[0].pos = D3DXVECTOR3(-g_aEffect[nCntEffect].fRadius, g_aEffect[nCntEffect].fRadius, 0.0f);
			pVtx[1].pos = D3DXVECTOR3(g_aEffect[nCntEffect].fRadius, g_aEffect[nCntEffect].fRadius, 0.0f);
			pVtx[2].pos = D3DXVECTOR3(-g_aEffect[nCntEffect].fRadius, -g_aEffect[nCntEffect].fRadius, 0.0f);
			pVtx[3].pos = D3DXVECTOR3(g_aEffect[nCntEffect].fRadius, -g_aEffect[nCntEffect].fRadius, 0.0f);

			//頂点カラーの設定
			pVtx[0].col = g_aEffect[nCntEffect].col;
			pVtx[1].col = g_aEffect[nCntEffect].col;
			pVtx[2].col = g_aEffect[nCntEffect].col;
			pVtx[3].col = g_aEffect[nCntEffect].col;

			/*g_aEffect[nCntEffect].fRadius -= 0.01f;*/
			g_aEffect[nCntEffect].pos += g_aEffect[nCntEffect].move;
			g_aEffect[nCntEffect].nLife--;

			if (g_aEffect[nCntEffect].nLife < 0)
			{
				g_aEffect[nCntEffect].bUse = false;
			}
			if (g_aEffect[nCntEffect].fRadius < 0)
			{
				g_aEffect[nCntEffect].bUse = false;
			}
		}
	}

	g_pVtxBuffEffect->Unlock();
}

//======================================================================================
// エフェクトの描画処理
//======================================================================================
void DrawEffect(void)
{

	LPDIRECT3DDEVICE9 pDevice = GetDevice();
	D3DXMATRIX mtxRot, mtxTrans;
	D3DXMATRIX mtxView;

	SetFogEnable(false);		//一旦fogを消す

	for (int nCntEffect = 0; nCntEffect < MAX_EFFECT; nCntEffect++)
	{
		if (g_aEffect[nCntEffect].bUse == true)
		{
			//ワールドマトリックスの初期化
			D3DXMatrixIdentity(&g_aEffect[nCntEffect].mtxWorldEffect);

			//ビューマトリックスを取得
			pDevice->GetTransform(D3DTS_VIEW, &mtxView);

			//エフェクトをカメラに対して正面に向ける
			D3DXMatrixInverse(&g_aEffect[nCntEffect].mtxWorldEffect, NULL, &mtxView);	//逆行列を求める

			g_aEffect[nCntEffect].mtxWorldEffect._41 = 0.0f;		//マトリックス(行列)の内容
			g_aEffect[nCntEffect].mtxWorldEffect._42 = 0.0f;
			g_aEffect[nCntEffect].mtxWorldEffect._43 = 0.0f;

			//位置を反映
			D3DXMatrixTranslation(&mtxTrans, g_aEffect[nCntEffect].pos.x, g_aEffect[nCntEffect].pos.y, g_aEffect[nCntEffect].pos.z);
			D3DXMatrixMultiply(&g_aEffect[nCntEffect].mtxWorldEffect, &g_aEffect[nCntEffect].mtxWorldEffect, &mtxTrans);

			//ワールドマトリックスの設定
			pDevice->SetTransform(D3DTS_WORLD, &g_aEffect[nCntEffect].mtxWorldEffect);

			//頂点バッファをデータストリームに設定
			pDevice->SetStreamSource(0, g_pVtxBuffEffect, 0, sizeof(VERTEX_3D));

			//頂点フォーマットの設定
			pDevice->SetFVF(FVF_VERTEX_3D);

			//テクスチャ
			pDevice->SetTexture(0, g_pTextureBuffEffect[g_aEffect[nCntEffect].tex]);

			// Zテストを無効にする
			pDevice->SetRenderState(D3DRS_ZFUNC, D3DCMP_ALWAYS);	// Zテストの比較方法を変更(Zバッファの前後関係に関わらず描画する)
			pDevice->SetRenderState(D3DRS_ZWRITEENABLE, FALSE);		// Zバッファ更新の有効/無効の設定

			//αブレンディングを加算合成して設定
			pDevice->SetRenderState(D3DRS_BLENDOP, D3DBLENDOP_ADD);
			pDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
			pDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_ONE);

			//エフェクトの描画
			pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, nCntEffect * 4, 2);

			//αブレンディングを戻す
			pDevice->SetRenderState(D3DRS_BLENDOP, D3DBLENDOP_ADD);
			pDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
			pDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);

			// Zテストを有効にする
			pDevice->SetRenderState(D3DRS_ZFUNC, D3DCMP_LESSEQUAL);
			pDevice->SetRenderState(D3DRS_ZWRITEENABLE, TRUE);
		}

	}
	SetFogEnable(true);		//fogをtrueに
}

//======================================================================================
// エフェクトの設定処理
//======================================================================================
void SetEffect(EFFECT_TYPE type, EFFECT_TEX tex, D3DXVECTOR3 pos, D3DXVECTOR3 move, D3DXCOLOR col,int nLife)
{
	for (int nCntEffect = 0; nCntEffect < MAX_EFFECT; nCntEffect++)
	{
		//使われていなければ
		if (g_aEffect[nCntEffect].bUse == false)
		{
			g_aEffect[nCntEffect].pos = pos;
			g_aEffect[nCntEffect].move = move;
			g_aEffect[nCntEffect].col = col;
			g_aEffect[nCntEffect].type = type;
			g_aEffect[nCntEffect].tex = tex;
			g_aEffect[nCntEffect].fRadius = EFFECT_RADIUS;
			g_aEffect[nCntEffect].nLife = nLife;
			g_aEffect[nCntEffect].bUse = true;

			VERTEX_3D* pVtx;    //頂点情報の設定
			//頂点バッファをロックし、頂点情報へのポインタを取得
			g_pVtxBuffEffect->Lock(0, 0, (void**)&pVtx, 0);
			
			pVtx += nCntEffect * 4;

			//頂点カラーの設定
			pVtx[0].col = g_aEffect[nCntEffect].col;
			pVtx[1].col = g_aEffect[nCntEffect].col;
			pVtx[2].col = g_aEffect[nCntEffect].col;
			pVtx[3].col = g_aEffect[nCntEffect].col;

			g_pVtxBuffEffect->Unlock();
			break;
		}
	}
}