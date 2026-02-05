//========================================================================
//
// バッテリー処理[buttery.cpp]
// Author : KajinoHiyori
//
//========================================================================
#include "battery.h"
#include "title.h"
#include "player.h"
#include "main.h"
#include "color.h"

// マクロ定義
#define NUM_PLACE		(3)				// バッテリーの最大数
#define NUM_SIZE		(40)			// 数字のサイズ
#define SPELLUI_POSY	(482.0f)		// 左のUIのX軸
#define LEFT_POS		(D3DXVECTOR3(120.0f, SPELLUI_POSY, 0.0f))		// onscreenの左のUI座標
#define RIGHT_POS		(D3DXVECTOR3(1160.0f, SPELLUI_POSY, 0.0f))		// onscreenの右のUI座標
#define PHONE_WIDTH		(108.0f)		// スマホの幅
#define PHONE_HEIGHT	(228.0f)		// スマホの高さ
#define MAX_BATTERY		(100)			// 最大バッテリー
#define MIN_BATTERY		(0)				// 最小バッテリー

// バッテリーの構造体定義
typedef struct
{
	D3DXVECTOR3 Pos;	// 位置
	int nBattery;		// バッテリー残量
	bool bDisp;			// 表示状態
}Battery;

// グローバル変数
LPDIRECT3DTEXTURE9 g_pTextureBattery = NULL;
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffBattery = NULL;
Battery g_aBattery[MAX_PLAYER];

//======================================================================================
// バッテリーの初期化処理
//======================================================================================
void InitBattery(void)
{
	LPDIRECT3DDEVICE9 pDevice;
	// デバイスの取得
	pDevice = GetDevice();

	// 操作方法の取得
	OPERATIONTYPE operationType = GetOperationType();

	// テクスチャの読み込み
	D3DXCreateTextureFromFile(pDevice, "data\\TEXTURE\\number.png", &g_pTextureBattery);

	for (int nCntPlayer = 0; nCntPlayer < MAX_PLAYER; nCntPlayer++)
	{
		switch (operationType)
		{
		case OPERATIONTYPE_2P:	// 2人操作
			if (nCntPlayer == 0)
			{
				g_aBattery[nCntPlayer].Pos = LEFT_POS;
			}
			else if (nCntPlayer == 1)
			{
				g_aBattery[nCntPlayer].Pos = RIGHT_POS;
			}
			break;

		default:
			if (nCntPlayer > 0)
			{
				continue;
			}
			g_aBattery[nCntPlayer].Pos = RIGHT_POS;
			break;
		}
		g_aBattery[nCntPlayer].nBattery = MAX_BATTERY;
		g_aBattery[nCntPlayer].bDisp = false;
	}

	// 頂点バッファの生成
	pDevice->CreateVertexBuffer(sizeof(VERTEX_2D) * 4 * NUM_PLACE * MAX_PLAYER, D3DUSAGE_WRITEONLY, FVF_VERTEX_2D, D3DPOOL_MANAGED, &g_pVtxBuffBattery, NULL);

	VERTEX_2D* pVtx;
	// 頂点バッファをロックし、頂点情報へのポインタを取得
	g_pVtxBuffBattery->Lock(0, 0, (void**)&pVtx, 0);
	for (int nCntPlayer = 0; nCntPlayer < MAX_PLAYER; nCntPlayer++)
	{
		for (int nCntBattery = 0; nCntBattery < NUM_PLACE; nCntBattery++)
		{
			// 位置の設定
			pVtx[0].pos.x = g_aBattery[nCntBattery].Pos.x + nCntBattery * NUM_SIZE;
			pVtx[0].pos.y = g_aBattery[nCntBattery].Pos.y;
			pVtx[0].pos.z = g_aBattery[nCntBattery].Pos.z;
			pVtx[1].pos.x = g_aBattery[nCntBattery].Pos.x + nCntBattery * NUM_SIZE + NUM_SIZE;
			pVtx[1].pos.y = g_aBattery[nCntBattery].Pos.y;
			pVtx[1].pos.z = g_aBattery[nCntBattery].Pos.z;
			pVtx[2].pos.x = g_aBattery[nCntBattery].Pos.x + nCntBattery * NUM_SIZE;
			pVtx[2].pos.y = g_aBattery[nCntBattery].Pos.y + NUM_SIZE;
			pVtx[2].pos.z = g_aBattery[nCntBattery].Pos.z;
			pVtx[3].pos.x = g_aBattery[nCntBattery].Pos.x + nCntBattery * NUM_SIZE + NUM_SIZE;
			pVtx[3].pos.y = g_aBattery[nCntBattery].Pos.y + NUM_SIZE;
			pVtx[3].pos.z = g_aBattery[nCntBattery].Pos.z;

			// rhwの設定
			pVtx[0].rhw = 1.0f;
			pVtx[1].rhw = 1.0f;
			pVtx[2].rhw = 1.0f;
			pVtx[3].rhw = 1.0f;

			// 色の設定
			pVtx[0].col = COLOR_WHITE;
			pVtx[1].col = COLOR_WHITE;
			pVtx[2].col = COLOR_WHITE;
			pVtx[3].col = COLOR_WHITE;

			// テクスチャ座標の設定
			pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
			pVtx[1].tex = D3DXVECTOR2(0.1f, 0.0f);
			pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
			pVtx[3].tex = D3DXVECTOR2(0.1f, 1.0f);

			pVtx += 4;
		}
	}
	// 頂点バッファをアンロック
	g_pVtxBuffBattery->Unlock();
}

//======================================================================================
// バッテリーの終了処理
//======================================================================================
void UninitBattery(void)
{
	// テクスチャの破棄
	if (g_pTextureBattery != NULL)
	{
		g_pTextureBattery->Release();
		g_pTextureBattery = NULL;
	}

	// 頂点バッファの破棄
	if (g_pVtxBuffBattery != NULL)
	{
		g_pVtxBuffBattery->Release();
		g_pVtxBuffBattery = NULL;
	}
}

//======================================================================================
// バッテリーの更新処理
//======================================================================================
void UpdateBattery(void)
{

}

//======================================================================================
// バッテリーの描画処理
//======================================================================================
void DrawBattery(void)
{
	LPDIRECT3DDEVICE9 pDevice;	// デバイスへのポインタ
	// デバイスの取得
	pDevice = GetDevice();

	// 頂点バッファをデータストリームに設定
	pDevice->SetStreamSource(0, g_pVtxBuffBattery, 0, sizeof(VERTEX_2D));

	// 頂点フォーマットの設定
	pDevice->SetFVF(FVF_VERTEX_2D);
	// テクスチャの設定
	pDevice->SetTexture(0, g_pTextureBattery);

	for (int nCntPlayer = 0; nCntPlayer < MAX_PLAYER; nCntPlayer++)
	{
		if (g_aBattery[nCntPlayer].bDisp == true)
		{
			for (int nCntBattery = 0; nCntBattery < NUM_PLACE; nCntBattery++)
			{
				// ポリゴンの描画
				pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, nCntBattery * 4, 2);
			}
		}
	}
}

//======================================================================================
// バッテリーの設置処理
//======================================================================================
void SetBattery(int nIdx, int nBattery, D3DXVECTOR3 pos)
{
	int aTexU[NUM_PLACE];	// 各桁の数値を格納

	g_aBattery[nIdx].nBattery = nBattery;
	aTexU[0] = g_aBattery[nIdx].nBattery % 1000 / 100;
	aTexU[1] = g_aBattery[nIdx].nBattery % 100 / 10;
	aTexU[2] = g_aBattery[nIdx].nBattery % 10 / 1;

	VERTEX_2D* pVtx;
	// 頂点バッファをロックし、頂点情報へのポインタを取得
	g_pVtxBuffBattery->Lock(0, 0, (void**)&pVtx, 0);
	for (int nCntBattery = 0; nCntBattery < NUM_PLACE; nCntBattery++, pVtx += 4)
	{
		// 位置の設定
		pVtx[0].pos.x = g_aBattery[nCntBattery].Pos.x + nCntBattery * NUM_SIZE;
		pVtx[0].pos.y = g_aBattery[nCntBattery].Pos.y;
		pVtx[0].pos.z = g_aBattery[nCntBattery].Pos.z;
		pVtx[1].pos.x = g_aBattery[nCntBattery].Pos.x + nCntBattery * NUM_SIZE + NUM_SIZE;
		pVtx[1].pos.y = g_aBattery[nCntBattery].Pos.y;
		pVtx[1].pos.z = g_aBattery[nCntBattery].Pos.z;
		pVtx[2].pos.x = g_aBattery[nCntBattery].Pos.x + nCntBattery * NUM_SIZE;
		pVtx[2].pos.y = g_aBattery[nCntBattery].Pos.y + NUM_SIZE;
		pVtx[2].pos.z = g_aBattery[nCntBattery].Pos.z;
		pVtx[3].pos.x = g_aBattery[nCntBattery].Pos.x + nCntBattery * NUM_SIZE + NUM_SIZE;
		pVtx[3].pos.y = g_aBattery[nCntBattery].Pos.y + NUM_SIZE;
		pVtx[3].pos.z = g_aBattery[nCntBattery].Pos.z;

		// テクスチャ座標の設定
		pVtx[0].tex = D3DXVECTOR2(aTexU[nCntBattery] * 0.1f, 0.0f);
		pVtx[1].tex = D3DXVECTOR2(aTexU[nCntBattery] * 0.1f + 0.1f, 0.0f);
		pVtx[2].tex = D3DXVECTOR2(aTexU[nCntBattery] * 0.1f, 1.0f);
		pVtx[3].tex = D3DXVECTOR2(aTexU[nCntBattery] * 0.1f + 0.1f, 1.0f);
	}
	// 頂点バッファをアンロック
	g_pVtxBuffBattery->Unlock();

	g_aBattery[nIdx].bDisp = true;
	g_aBattery[nIdx].Pos = pos;
}

//======================================================================================
// バッテリーの増減処理
//======================================================================================
void ChangeBattery(int nIdx, int nValue)
{
	int aTexU[NUM_PLACE];	// 各桁の数値を格納

	g_aBattery[nIdx].nBattery += nValue;
	// カンストしたら加算を停止する
	if (g_aBattery[nIdx].nBattery >= MAX_BATTERY)
	{
		g_aBattery[nIdx].nBattery = MAX_BATTERY;
	}
	// 最低値になったら0に戻る
	if (g_aBattery[nIdx].nBattery >= MIN_BATTERY)
	{
		g_aBattery[nIdx].nBattery = MIN_BATTERY;
	}

	aTexU[0] = g_aBattery[nIdx].nBattery % 1000 / 100;
	aTexU[1] = g_aBattery[nIdx].nBattery % 100 / 10;
	aTexU[2] = g_aBattery[nIdx].nBattery % 10 / 1;

	VERTEX_2D* pVtx;
	// 頂点バッファをロックし、頂点情報へのポインタを取得
	g_pVtxBuffBattery->Lock(0, 0, (void**)&pVtx, 0);
	for (int nCntBattery = 0; nCntBattery < NUM_PLACE; nCntBattery++, pVtx += 4)
	{
		// テクスチャ座標の設定
		pVtx[0].tex = D3DXVECTOR2(aTexU[nCntBattery] * 0.1f, 0.0f);
		pVtx[1].tex = D3DXVECTOR2(aTexU[nCntBattery] * 0.1f + 0.1f, 0.0f);
		pVtx[2].tex = D3DXVECTOR2(aTexU[nCntBattery] * 0.1f, 1.0f);
		pVtx[3].tex = D3DXVECTOR2(aTexU[nCntBattery] * 0.1f + 0.1f, 1.0f);
	}
	// 頂点バッファをアンロック
	g_pVtxBuffBattery->Unlock();
}

//======================================================================================
// バッテリーの非表示処理
//======================================================================================
void DissapearBattery(int nIdx)
{
	g_aBattery[nIdx].bDisp = false;
}
