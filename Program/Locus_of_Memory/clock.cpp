//========================================================================
//
// 時計処理[clock.cpp]
// Author : KajinoHiyori
//
//========================================================================
#include "clock.h"
#include "player.h"
#include "main.h"
#include "title.h"

// マクロ定義
#define NUM_PLACE		(4)				// 時計の最大数
#define NUM_SIZE		(40)			// 数字のサイズ
#define SPELLUI_POSY	(482.0f)		// 左のUIのX軸
#define LEFT_POS		(D3DXVECTOR3(120.0f, SPELLUI_POSY, 0.0f))		// onscreenの左のUI座標
#define RIGHT_POS		(D3DXVECTOR3(1160.0f, SPELLUI_POSY, 0.0f))		// onscreenの右のUI座標
#define PHONE_WIDTH		(108.0f)		// スマホの幅
#define PHONE_HEIGHT	(228.0f)		// スマホの高さ
#define START_HOUR		(8)		// 開始時刻[時]
#define START_MIN		(0)		// 開始時刻[分]
#define END_HOUR		(20)	// 終了時刻[時]
#define END_MIN			(0)		// 終了時刻[分]
#define MAX_MIN			(60)	// 分の最大値
#define INTERVAL_TIME	(15)	// どのくらいの間隔で1分進むのか

// 時計の構造体定義
typedef struct
{
	D3DXVECTOR3 Pos;	// 位置
	bool bDisp;			// 表示状態
}Clock;

typedef struct
{
	int nCounter;		// 経過時間の間隔をカウント
	int nHour;			// 時
	int nMinute;		// 分
	int nTime;			// 時間[4桁表示]
	CLOCKSTATE state;	// 稼働状態
}Time;

// グローバル変数
LPDIRECT3DTEXTURE9 g_pTextureClock = NULL;
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffClock = NULL;
Clock g_aClock[MAX_PLAYER];
Time g_time;

//======================================================================================
// 時計の初期化処理
//======================================================================================
void InitClock(void)
{
	LPDIRECT3DDEVICE9 pDevice;
	// デバイスの取得
	pDevice = GetDevice();

	// 操作方法の取得
	OPERATIONTYPE operationType = GetOperationType();

	// テクスチャの読み込み
	D3DXCreateTextureFromFile(pDevice, "data\\TEXTURE\\number.png", &g_pTextureClock);

	for (int nCntPlayer = 0; nCntPlayer < MAX_PLAYER; nCntPlayer++)
	{
		switch (operationType)
		{
		case OPERATIONTYPE_2P:	// 2人操作
			if (nCntPlayer == 0)
			{
				g_aClock[nCntPlayer].Pos = LEFT_POS;
			}
			else if (nCntPlayer == 1)
			{
				g_aClock[nCntPlayer].Pos = RIGHT_POS;
			}
			break;

		default:
			if (nCntPlayer > 0)
			{
				continue;
			}
			g_aClock[nCntPlayer].Pos = RIGHT_POS;
			break;
		}
		g_aClock[nCntPlayer].bDisp = false;
	}

	g_time.nCounter = 0;
	g_time.nHour = START_HOUR;
	g_time.nMinute = START_MIN;
	g_time.nTime = g_time.nHour * 100 + g_time.nMinute;
	g_time.state = CLOCKSTATE_OPERATION;

	// 頂点バッファの生成
	pDevice->CreateVertexBuffer(sizeof(VERTEX_2D) * 4 * NUM_PLACE * MAX_PLAYER, D3DUSAGE_WRITEONLY, FVF_VERTEX_2D, D3DPOOL_MANAGED, &g_pVtxBuffClock, NULL);

	VERTEX_2D* pVtx;
	// 頂点バッファをロックし、頂点情報へのポインタを取得
	g_pVtxBuffClock->Lock(0, 0, (void**)&pVtx, 0);
	for (int nCntPlayer = 0; nCntPlayer < MAX_PLAYER; nCntPlayer++)
	{
		for (int nCntClock = 0; nCntClock < NUM_PLACE; nCntClock++)
		{
			// 位置の設定
			pVtx[0].pos.x = g_aClock[nCntClock].Pos.x + nCntClock * NUM_SIZE;
			pVtx[0].pos.y = g_aClock[nCntClock].Pos.y;
			pVtx[0].pos.z = g_aClock[nCntClock].Pos.z;
			pVtx[1].pos.x = g_aClock[nCntClock].Pos.x + nCntClock * NUM_SIZE + NUM_SIZE;
			pVtx[1].pos.y = g_aClock[nCntClock].Pos.y;
			pVtx[1].pos.z = g_aClock[nCntClock].Pos.z;
			pVtx[2].pos.x = g_aClock[nCntClock].Pos.x + nCntClock * NUM_SIZE;
			pVtx[2].pos.y = g_aClock[nCntClock].Pos.y + NUM_SIZE;
			pVtx[2].pos.z = g_aClock[nCntClock].Pos.z;
			pVtx[3].pos.x = g_aClock[nCntClock].Pos.x + nCntClock * NUM_SIZE + NUM_SIZE;
			pVtx[3].pos.y = g_aClock[nCntClock].Pos.y + NUM_SIZE;
			pVtx[3].pos.z = g_aClock[nCntClock].Pos.z;

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
	g_pVtxBuffClock->Unlock();

	//SetClock(0, D3DXVECTOR3(10.0f, 10.0f, 0.0f));
}

//======================================================================================
// 時計の終了処理
//======================================================================================
void UninitClock(void)
{
	// テクスチャの破棄
	if (g_pTextureClock != NULL)
	{
		g_pTextureClock->Release();
		g_pTextureClock = NULL;
	}

	// 頂点バッファの破棄
	if (g_pVtxBuffClock != NULL)
	{
		g_pVtxBuffClock->Release();
		g_pVtxBuffClock = NULL;
	}
}

//======================================================================================
// 時計の更新処理
//======================================================================================
void UpdateClock(void)
{
	if (g_time.state == CLOCKSTATE_OPERATION)	// タイマーが稼働中の場合
	{
		g_time.nCounter++;
		// 一定フレーム経過すると1分進む
		if (g_time.nCounter >= INTERVAL_TIME)
		{
			g_time.nMinute++;
			g_time.nCounter = 0;
		}
		// 60分を越えると1時間追加される
		if (g_time.nMinute >= MAX_MIN)
		{
			g_time.nHour++;
			g_time.nMinute = 0;
		}
		if (g_time.nHour >= 24)
		{
			g_time.nHour = 0;
		}
	}
	g_time.nTime = g_time.nHour * 100 + g_time.nMinute;

	int aTexU[NUM_PLACE];	// 各桁の数値を格納

	aTexU[0] = g_time.nTime % 10000 / 1000;
	aTexU[1] = g_time.nTime % 1000 / 100;
	aTexU[2] = g_time.nTime % 100 / 10;
	aTexU[3] = g_time.nTime % 10 / 1;

	VERTEX_2D* pVtx;
	// 頂点バッファをロックし、頂点情報へのポインタを取得
	g_pVtxBuffClock->Lock(0, 0, (void**)&pVtx, 0);
	for (int nCntPlayer = 0; nCntPlayer < MAX_PLAYER; nCntPlayer++)
	{
		for (int nCntClock = 0; nCntClock < NUM_PLACE; nCntClock++)
		{
			// テクスチャ座標の設定
			pVtx[0].tex = D3DXVECTOR2(aTexU[nCntClock] * 0.1f, 0.0f);
			pVtx[1].tex = D3DXVECTOR2(aTexU[nCntClock] * 0.1f + 0.1f, 0.0f);
			pVtx[2].tex = D3DXVECTOR2(aTexU[nCntClock] * 0.1f, 1.0f);
			pVtx[3].tex = D3DXVECTOR2(aTexU[nCntClock] * 0.1f + 0.1f, 1.0f);

			pVtx += 4;
		}
	}
	// 頂点バッファをアンロック
	g_pVtxBuffClock->Unlock();
}

//======================================================================================
// 時計の描画処理
//======================================================================================
void DrawClock(void)
{
	LPDIRECT3DDEVICE9 pDevice;	// デバイスへのポインタ
	// デバイスの取得
	pDevice = GetDevice();

	// 頂点バッファをデータストリームに設定
	pDevice->SetStreamSource(0, g_pVtxBuffClock, 0, sizeof(VERTEX_2D));

	// 頂点フォーマットの設定
	pDevice->SetFVF(FVF_VERTEX_2D);
	// テクスチャの設定
	pDevice->SetTexture(0, g_pTextureClock);

	for (int nCntPlayer = 0; nCntPlayer < MAX_PLAYER; nCntPlayer++)
	{
		if (g_aClock[nCntPlayer].bDisp == true)
		{
			for (int nCntClock = 0; nCntClock < NUM_PLACE; nCntClock++)
			{
				// ポリゴンの描画
				pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, nCntClock * 4, 2);
			}
		}
	}
}

//======================================================================================
// 時計の設置処理
//======================================================================================
void SetClock(int nIdx, D3DXVECTOR3 pos)
{
	int aTexU[NUM_PLACE];	// 各桁の数値を格納

	aTexU[0] = g_time.nTime % 10000 / 1000;
	aTexU[1] = g_time.nTime % 1000 / 100;
	aTexU[2] = g_time.nTime % 100 / 10;
	aTexU[3] = g_time.nTime % 10 / 1;

	VERTEX_2D* pVtx;
	// 頂点バッファをロックし、頂点情報へのポインタを取得
	g_pVtxBuffClock->Lock(0, 0, (void**)&pVtx, 0);

	g_aClock[nIdx].Pos = pos;

	for (int nCntClock = 0; nCntClock < NUM_PLACE; nCntClock++)
	{
		// 位置の設定
		pVtx[0].pos.x = g_aClock[nIdx].Pos.x + nCntClock * NUM_SIZE;
		pVtx[0].pos.y = g_aClock[nIdx].Pos.y;
		pVtx[0].pos.z = g_aClock[nIdx].Pos.z;
		pVtx[1].pos.x = g_aClock[nIdx].Pos.x + nCntClock * NUM_SIZE + NUM_SIZE;
		pVtx[1].pos.y = g_aClock[nIdx].Pos.y;
		pVtx[1].pos.z = g_aClock[nIdx].Pos.z;
		pVtx[2].pos.x = g_aClock[nIdx].Pos.x + nCntClock * NUM_SIZE;
		pVtx[2].pos.y = g_aClock[nIdx].Pos.y + NUM_SIZE;
		pVtx[2].pos.z = g_aClock[nIdx].Pos.z;
		pVtx[3].pos.x = g_aClock[nIdx].Pos.x + nCntClock * NUM_SIZE + NUM_SIZE;
		pVtx[3].pos.y = g_aClock[nIdx].Pos.y + NUM_SIZE;
		pVtx[3].pos.z = g_aClock[nIdx].Pos.z;

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
		pVtx[0].tex = D3DXVECTOR2(aTexU[nCntClock] * 0.1f, 0.0f);
		pVtx[1].tex = D3DXVECTOR2(aTexU[nCntClock] * 0.1f + 0.1f, 0.0f);
		pVtx[2].tex = D3DXVECTOR2(aTexU[nCntClock] * 0.1f, 1.0f);
		pVtx[3].tex = D3DXVECTOR2(aTexU[nCntClock] * 0.1f + 0.1f, 1.0f);

		pVtx += 4;
	}
	
	// 頂点バッファをアンロック
	g_pVtxBuffClock->Unlock();

	g_aClock[nIdx].bDisp = true;
}

//======================================================================================
// 時計の稼働状態更新処理
//======================================================================================
void SetClockState(CLOCKSTATE clockState)
{
	g_time.state = clockState;
}

//======================================================================================
// 時計の稼働状態を渡す
//======================================================================================
CLOCKSTATE GetClockState(void)
{
	return g_time.state;
}

//======================================================================================
// 時計を非表示にする
//======================================================================================
void DisappearClock(int nIdx)
{
	g_aClock[nIdx].bDisp = false;
}