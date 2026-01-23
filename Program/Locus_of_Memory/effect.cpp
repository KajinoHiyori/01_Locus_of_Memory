//========================================================================
// 
// 3Dポリゴンの描画[polygon.cpp]
//
//========================================================================

#include"main.h"
#include"effect.h"
#include"input.h"
#include"color.h"

#define MAX_EFFECT	(4096)

//構造体の定義
typedef struct
{
	D3DXVECTOR3 pos;		//位置
	D3DXVECTOR3 move;		//移動量
	D3DXCOLOR col;			//色
	D3DXMATRIX g_mtxWorldPolygon;	//ワールドマトリックス
	float fRadius;			//半径
	int nLife;				//寿命（色）
	bool bUse;				//使用しているか
}Effect;

//グローバル変数
LPDIRECT3DTEXTURE9 g_pTextureBuffPolygon = NULL;
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffPolygon = NULL;
Effect g_aEffect[MAX_EFFECT];

void InitEffect(void)
{

	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	//テクスチャ読み込み
	D3DXCreateTextureFromFile(pDevice,
		"Data\\Texture\\shadow000.jpg",
		&g_pTextureBuffPolygon);

	//初期化
	for (int nCntEffect = 0; nCntEffect < MAX_EFFECT; nCntEffect++)
	{
		g_aEffect[nCntEffect].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		g_aEffect[nCntEffect].move = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		g_aEffect[nCntEffect].col = D3DXCOLOR(255, 255, 255, 255);
		g_aEffect[nCntEffect].fRadius = 0;
		g_aEffect[nCntEffect].nLife = 0;
		g_aEffect[nCntEffect].bUse = false;
	}

	//頂点バッファの生成
	pDevice->CreateVertexBuffer(sizeof(VERTEX_3D) * MAX_EFFECT*4,
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_3D,
		D3DPOOL_MANAGED,
		&g_pVtxBuffPolygon,
		NULL);


	VERTEX_3D* pVtx;    //頂点情報の設定

	//頂点バッファをロックし、頂点情報へのポインタを取得
	g_pVtxBuffPolygon->Lock(0, 0, (void**)&pVtx, 0);

	for (int nCntEffect = 0; nCntEffect < MAX_EFFECT; nCntEffect++, pVtx += 4)
	{
		//頂点座標の設定
		pVtx[0].pos = D3DXVECTOR3(-50.0f, 50.0f, 0.0f);
		pVtx[1].pos = D3DXVECTOR3(50.0f, 50.0f, 0.0f);
		pVtx[2].pos = D3DXVECTOR3(-50.0f, 0.0f, 0.0f);
		pVtx[3].pos = D3DXVECTOR3(50.0f, 0.0f, 0.0f);

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

	g_pVtxBuffPolygon->Unlock();
}

//ポリゴンの終了処理
void UninitEffect(void)
{
	//テクスチャの破棄
	if (g_pTextureBuffPolygon != NULL)
	{
		g_pTextureBuffPolygon->Release();
		g_pTextureBuffPolygon = NULL;
	}

	//頂点バッファを破棄
	if (g_pVtxBuffPolygon != NULL)
	{
		g_pVtxBuffPolygon->Release();
		g_pVtxBuffPolygon = NULL;
	}

}

//ポリゴンの更新処理
void UpdateEffect(void)
{
	if (GetKeyboardTrigger(DIK_SPACE) == true)
	{
		SetEffect(D3DXVECTOR3(0.0f, 0.0f, 0.0f), COLOR_RED, 20);
	}
	for (int nCntEffect = 0; nCntEffect < MAX_EFFECT; nCntEffect++)
	{
		if (g_aEffect[nCntEffect].bUse == true)
		{
			g_aEffect[nCntEffect].nLife--;
			if (g_aEffect[nCntEffect].nLife < 0)
			{
				g_aEffect[nCntEffect].bUse = false;
			}
		}
	}
}

void DrawEffect(void)
{

	LPDIRECT3DDEVICE9 pDevice = GetDevice();
	D3DXMATRIX mtxRot, mtxTrans;
	D3DXMATRIX mtxView;

	for (int nCntEffect = 0; nCntEffect < MAX_EFFECT; nCntEffect++)
	{
		if (g_aEffect[nCntEffect].bUse == true)
		{
			//ワールドマトリックスの初期化
			D3DXMatrixIdentity(&g_aEffect[nCntEffect].g_mtxWorldPolygon);

			//ビューマトリックスを取得
			pDevice->GetTransform(D3DTS_VIEW, &mtxView);

			//ポリゴンをカメラに対して正面に向ける
			D3DXMatrixInverse(&g_aEffect[nCntEffect].g_mtxWorldPolygon, NULL, &mtxView);	//逆行列を求める

			g_aEffect[nCntEffect].g_mtxWorldPolygon._41 = 0.0f;		//マトリックス(行列)の内容
			g_aEffect[nCntEffect].g_mtxWorldPolygon._42 = 0.0f;
			g_aEffect[nCntEffect].g_mtxWorldPolygon._43 = 0.0f;

			//位置を反映
			D3DXMatrixTranslation(&mtxTrans, g_aEffect[nCntEffect].pos.x, g_aEffect[nCntEffect].pos.y, g_aEffect[nCntEffect].pos.z);
			D3DXMatrixMultiply(&g_aEffect[nCntEffect].g_mtxWorldPolygon, &g_aEffect[nCntEffect].g_mtxWorldPolygon, &mtxTrans);

			//ワールドマトリックスの設定
			pDevice->SetTransform(D3DTS_WORLD, &g_aEffect[nCntEffect].g_mtxWorldPolygon);

			//頂点バッファをデータストリームに設定
			pDevice->SetStreamSource(0, g_pVtxBuffPolygon, 0, sizeof(VERTEX_3D));

			//頂点フォーマットの設定
			pDevice->SetFVF(FVF_VERTEX_3D);

			//テクスチャ
			pDevice->SetTexture(0, g_pTextureBuffPolygon);

			//αブレンディングを加算合成して設定
			pDevice->SetRenderState(D3DRS_BLENDOP, D3DBLENDOP_ADD);
			pDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
			pDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_ONE);

			//ポリゴンの描画
			pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, 0, 2);

			//αブレンディングを戻す
			pDevice->SetRenderState(D3DRS_BLENDOP, D3DBLENDOP_ADD);
			pDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
			pDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_ONE);

		}

	}
}

//================
//設定処理
//================
void SetEffect(D3DXVECTOR3 pos, D3DXCOLOR col,int nLife)
{


	for (int nCntEffect = 0; nCntEffect < MAX_EFFECT; nCntEffect++)
	{
		//使われていなければ
		if (g_aEffect[nCntEffect].bUse == false)
		{
			g_aEffect[nCntEffect].pos = pos;
			g_aEffect[nCntEffect].col = col;
			g_aEffect[nCntEffect].nLife = nLife;
			g_aEffect[nCntEffect].bUse = true;
			break;
		}
	}
}