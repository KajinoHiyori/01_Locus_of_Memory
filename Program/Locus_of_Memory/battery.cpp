//========================================================================
//
// バッテリー処理[buttery.cpp]
// Author : KajinoHiyori
//
//========================================================================
#include "battery.h"
#include "uimanager.h"
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
#define NORMAL			(D3DXVECTOR3(0.0f, 1.0f, 0.0f))	// 法線ベクトル

// バッテリーの構造体定義
typedef struct
{
	D3DXMATRIX	mtxWorld;				// ワールドマトリックス
	D3DXVECTOR3 mainPos;				// 中心位置
	D3DXVECTOR3 rot;					// 向き
	D3DXVECTOR3 pos[NUM_PLACE];			// 位置
	D3DXCOLOR	col;					// 色
	int nBattery;		// バッテリー残量
	bool bDisp;			// 表示状態
}Battery;

// グローバル変数
LPDIRECT3DTEXTURE9 g_apTextureBattery[NUM_PLACE] = {};
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffBattery = NULL;
Battery g_aBattery[MAX_PLAYER];

const char* c_apFilenameBattery[NUM_PLACE] =
{
	"data\\TEXTURE\\Pause\\clock_000.png",
	"data\\TEXTURE\\Pause\\clock_001.png",
	"data\\TEXTURE\\Pause\\clock_002.png",
};

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
	for (int nCntBattery = 0; nCntBattery < NUM_PLACE; nCntBattery++)
	{
		D3DXCreateTextureFromFile(pDevice, c_apFilenameBattery[nCntBattery], &g_apTextureBattery[nCntBattery]);
	}

	for (int nCntPlayer = 0; nCntPlayer < MAX_PLAYER; nCntPlayer++)
	{
		for (int nCntBattety = 0; nCntBattety < NUM_PLACE; nCntBattety++)
		{
			g_aBattery[nCntPlayer].pos[nCntBattety] = D3DXVECTOR3(0.0f, 0.0f, 0.0f);	// 位置
		}
		g_aBattery[nCntPlayer].mainPos	= D3DXVECTOR3(0.0f, 0.0f, 0.0f);	// 中心位置
		g_aBattery[nCntPlayer].rot		= D3DXVECTOR3(0.0f, 0.0f, 0.0f);	// 向き
		g_aBattery[nCntPlayer].col		= COLOR_WHITE;	// 色
		g_aBattery[nCntPlayer].nBattery = MAX_BATTERY;	// バッテリー残量
		g_aBattery[nCntPlayer].bDisp	= false;		// 表示状態
	}

	// 頂点バッファの生成
	pDevice->CreateVertexBuffer(sizeof(VERTEX_3D) * 4 * NUM_PLACE * MAX_PLAYER, D3DUSAGE_WRITEONLY, FVF_VERTEX_3D, D3DPOOL_MANAGED, &g_pVtxBuffBattery, NULL);

	VERTEX_3D* pVtx;
	// 頂点バッファをロックし、頂点情報へのポインタを取得
	g_pVtxBuffBattery->Lock(0, 0, (void**)&pVtx, 0);
	for (int nCntPlayer = 0; nCntPlayer < MAX_PLAYER; nCntPlayer++)
	{
		for (int nCntBattery = 0; nCntBattery < NUM_PLACE; nCntBattery++)
		{
			// 頂点座標の設定
			pVtx[0].pos = D3DXVECTOR3(-PHONE_WIDTH, PHONE_HEIGHT, 0.0f);
			pVtx[1].pos = D3DXVECTOR3(PHONE_WIDTH, PHONE_HEIGHT, 0.0f);
			pVtx[2].pos = D3DXVECTOR3(-PHONE_WIDTH, -PHONE_HEIGHT, 0.0f);
			pVtx[3].pos = D3DXVECTOR3(PHONE_WIDTH, -PHONE_HEIGHT, 0.0f);

			// rhwの設定
			pVtx[0].nor = NORMAL;
			pVtx[1].nor = NORMAL;
			pVtx[2].nor = NORMAL;
			pVtx[3].nor = NORMAL;

			// 頂点カラーの設定
			pVtx[0].col = g_aBattery[nCntPlayer].col;
			pVtx[1].col = g_aBattery[nCntPlayer].col;
			pVtx[2].col = g_aBattery[nCntPlayer].col;
			pVtx[3].col = g_aBattery[nCntPlayer].col;


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
	for (int nCntBattery = 0; nCntBattery < NUM_PLACE; nCntBattery++)
	{
		if (g_apTextureBattery[nCntBattery] != NULL)
		{
			g_apTextureBattery[nCntBattery]->Release();
			g_apTextureBattery[nCntBattery] = NULL;
		}
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
	int aTexU[NUM_PLACE];	// 各桁の数値を格納

	for (int nCntPlayer = 0; nCntPlayer < MAX_PLAYER; nCntPlayer++)
	{
		// バッテリーのテクスチャ位置を更新
		aTexU[0] = g_aBattery[nCntPlayer].nBattery % 1000 / 100;
		aTexU[1] = g_aBattery[nCntPlayer].nBattery % 100 / 10;
		aTexU[2] = g_aBattery[nCntPlayer].nBattery % 10 / 1;

		// UIの位置や向きに合わせて時計の表示位置を更新
		g_aBattery[nCntPlayer].mainPos = GetUIPos(nCntPlayer);
		g_aBattery[nCntPlayer].rot = GetUIRot(nCntPlayer);

		for (int nCntClock = 0; nCntClock < NUM_PLACE; nCntClock++)
		{
			// 中心位置からの位置を求める
			g_aBattery[nCntPlayer].pos[nCntClock] = g_aBattery[nCntPlayer].mainPos;
		}
	}

	VERTEX_3D* pVtx;
	// 頂点バッファをロックし、頂点情報へのポインタを取得
	g_pVtxBuffBattery->Lock(0, 0, (void**)&pVtx, 0);
	for (int nCntPlayer = 0; nCntPlayer < MAX_PLAYER; nCntPlayer++)
	{
		for (int nCntClock = 0; nCntClock < NUM_PLACE; nCntClock++, pVtx += 4)
		{
			// 頂点カラーの設定
			pVtx[0].col = g_aBattery[nCntPlayer].col;
			pVtx[1].col = g_aBattery[nCntPlayer].col;
			pVtx[2].col = g_aBattery[nCntPlayer].col;
			pVtx[3].col = g_aBattery[nCntPlayer].col;

			// テクスチャ座標の設定
			pVtx[0].tex = D3DXVECTOR2(aTexU[nCntClock] * 0.1f, 0.0f);
			pVtx[1].tex = D3DXVECTOR2(aTexU[nCntClock] * 0.1f + 0.1f, 0.0f);
			pVtx[2].tex = D3DXVECTOR2(aTexU[nCntClock] * 0.1f, 1.0f);
			pVtx[3].tex = D3DXVECTOR2(aTexU[nCntClock] * 0.1f + 0.1f, 1.0f);
		}
	}
	// 頂点バッファをアンロック
	g_pVtxBuffBattery->Unlock();
}

//======================================================================================
// バッテリーの描画処理
//======================================================================================
void DrawBattery(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();	// デバイスの取得
	D3DXMATRIX mtxRot, mtxTrans;	// 計算用マトリックス
	D3DXMATRIX mtxView;		// ビューマトリックスの取得

	// アルファテストを有効にする
	pDevice->SetRenderState(D3DRS_ALPHATESTENABLE, TRUE);	// アルファテストを有効にする
	pDevice->SetRenderState(D3DRS_ALPHAFUNC, D3DCMP_GREATER);	// 比較方法を設定(基準値より大きい場合描画)
	pDevice->SetRenderState(D3DRS_ALPHAREF, 0);	// アルファテストの参照値を設定(この場合、0より大きい場合は描画)

	// ライトをオフにする
	pDevice->SetRenderState(D3DRS_LIGHTING, FALSE);

	// カリングをオフにする
	pDevice->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);

	for (int nCntPlayer = 0; nCntPlayer < MAX_PLAYER; nCntPlayer++)
	{
		if (g_aBattery[nCntPlayer].bDisp == false)
		{
			continue;
		}

		for (int nCntClock = 0; nCntClock < NUM_PLACE; nCntClock++)
		{
			// ワールドマトリックスの初期化(デフォルトの値にする)
			D3DXMatrixIdentity(&g_aBattery[nCntPlayer].mtxWorld);

			// ビューマトリックスを取得する
			pDevice->GetTransform(D3DTS_VIEW, &mtxView);

			// 向きを反映
			D3DXMatrixRotationYawPitchRoll(&mtxRot, g_aBattery[nCntPlayer].rot.y, g_aBattery[nCntPlayer].rot.x, g_aBattery[nCntPlayer].rot.z);
			D3DXMatrixMultiply(&g_aBattery[nCntPlayer].mtxWorld, &g_aBattery[nCntPlayer].mtxWorld, &mtxRot);

			// 位置を反映
			D3DXMatrixTranslation(&mtxTrans, g_aBattery[nCntPlayer].pos[nCntClock].x, g_aBattery[nCntPlayer].pos[nCntClock].y, g_aBattery[nCntPlayer].pos[nCntClock].z);
			D3DXMatrixMultiply(&g_aBattery[nCntPlayer].mtxWorld, &g_aBattery[nCntPlayer].mtxWorld, &mtxTrans);

			// ワールドマトリックスの設定
			pDevice->SetTransform(D3DTS_WORLD, &g_aBattery[nCntPlayer].mtxWorld);

			// 頂点バッファをデータストリームに設定
			pDevice->SetStreamSource(0, g_pVtxBuffBattery, 0, sizeof(VERTEX_3D));

			// 頂点フォーマットの設定
			pDevice->SetFVF(FVF_VERTEX_3D);

			// テクスチャの設定
			pDevice->SetTexture(0, g_apTextureBattery[nCntClock]);

			// 時計の描画
			pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, nCntClock * 4 + (nCntPlayer * NUM_PLACE * 4), 2);

		}
	}

	// ライトをオンにする
	pDevice->SetRenderState(D3DRS_LIGHTING, TRUE);

	// カリングを元に戻す
	pDevice->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);

	// アルファテストを無効にする
	pDevice->SetRenderState(D3DRS_ALPHATESTENABLE, FALSE);	// アルファテストを無効にする
	pDevice->SetRenderState(D3DRS_ALPHAFUNC, D3DCMP_ALWAYS);	// 比較方法を設定(条件に関わらず描画)
	pDevice->SetRenderState(D3DRS_ALPHAREF, 0);	// アルファテストの参照値を設定(この場合、0より大きい場合は描画)
}

//======================================================================================
// バッテリーの設置処理
//======================================================================================
void SetBattery(int nIdx)
{
	g_aBattery[nIdx].bDisp = true;
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

	VERTEX_3D* pVtx;
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

//======================================================================================
// バッテリーの残量を渡す
//======================================================================================
int GetBattery(int nIdx)
{
	return g_aBattery[nIdx].nBattery;
}
