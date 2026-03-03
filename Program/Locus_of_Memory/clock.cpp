//========================================================================
//
// 時計処理[clock.cpp]
// Author : KajinoHiyori
//
//========================================================================
#include "clock.h"
#include "object.h"
#include "uimanager.h"
#include "player.h"
#include "main.h"
#include "title.h"
#include "debugproc.h"
#include "color.h"
#include "game.h"

// マクロ定義
#define NUM_PLACE		(4)				// 時計の最大数
#define NORMAL			(D3DXVECTOR3(0.0f, 1.0f, 0.0f))	// 法線ベクトル
#define PHONE_WIDTH		(28.125f)			// スマホの幅
#define PHONE_HEIGHT	(50.0f)				// スマホの高さ
#define PHONE_Y			(50.0f)				// スマホの高度
#define NUM_WIDTH		(PHONE_WIDTH / 2)	// 数字のサイズ
#define SPELLCLOCK_POSY	(482.0f)			// 左の時計のX軸
#define LEFT_POS		(D3DXVECTOR3(120.0f, SPELLCLOCK_POSY, 0.0f))		// onscreenの左の時計座標
#define RIGHT_POS		(D3DXVECTOR3(1160.0f, SPELLCLOCK_POSY, 0.0f))		// onscreenの右の時計座標
#define START_HOUR		(8)		// 開始時刻[時]
#define START_MIN		(0)		// 開始時刻[分]
#define END_HOUR		(20)	// 終了時刻[時]
#define END_MIN			(0)		// 終了時刻[分]
#define MAX_MIN			(60)	// 分の最大値
#define MAX_HOUR		(24)	// 時間の最大値
#define HALF_HOUR		(MAX_HOUR / 2)	// 12時間
#define INTERVAL_TIME	(15)	// どのくらいの間隔で1分進むのか
#define END_TIME		(2000)	// ゲーム終了時間
#define MAGIC_TIME		(840)	// 時間停止魔法継続時間
#define MAX_ANGLE		(D3DX_PI * 2)	// 角度の最大数
#define NUM_TOWER		(4)				// 時計塔の側面にある時計数

// 時計の構造体定義
typedef struct
{
	D3DXMATRIX	mtxWorld;				// ワールドマトリックス
	D3DXVECTOR3 pos[NUM_PLACE];			// 位置
	D3DXCOLOR	col;					// 色
	bool bDisp;							// 表示状態
}Clock;

typedef struct
{
	int nCounter;		// 経過時間の間隔をカウント
	int nMagicCounter;	// 魔法の継続時間をカウント
	int nHour;			// 時
	int nMinute;		// 分
	int nTime;			// 時間[4桁表示]
	CLOCKSTATE state;	// 稼働状態
}Time;

const char* c_pFilenameclock[1] =
{
	"data\\TEXTURE\\number.png",
};

// グローバル変数
LPDIRECT3DTEXTURE9 g_apTextureClock[NUM_PLACE] = {};
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffClock = NULL;
Clock g_aClock[MAX_PLAYER];
Time g_time;
int g_nIdxClockTower;

//======================================================================================
// 時計の初期化処理
//======================================================================================
void InitClock(void)
{
	// デバイスの取得
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	// 操作人数の取得
	OPERATIONTYPE operationType = GetOperationType();

	// プレイヤーの情報を取得
	Player* pPlayer = GetPlayer();

	// テクスチャの読み込み
	for (int nCntClock = 0; nCntClock < NUM_PLACE; nCntClock++)
	{
		D3DXCreateTextureFromFile(pDevice, c_pFilenameclock[nCntClock], &g_apTextureClock[nCntClock]);
	}
	// 初期化
	for (int nCntPlayer = 0; nCntPlayer < MAX_PLAYER; nCntPlayer++)
	{
		for (int nCntClock = 0; nCntClock < NUM_PLACE; nCntClock++)
		{
			g_aClock[nCntPlayer].pos[nCntClock] = D3DXVECTOR3(nCntClock * NUM_WIDTH - NUM_WIDTH * 2, 0.0f, 0.0f);
		}

		g_aClock[nCntPlayer].col = COLOR_WHITE;								// 色
		g_aClock[nCntPlayer].bDisp = false;									// 表示状態
	}

	// 時間構造体
	g_time.nCounter = 0;
	g_time.nMagicCounter = 0;
	g_time.nHour = START_HOUR;
	g_time.nMinute = START_MIN;
	g_time.nTime = g_time.nHour * 100 + g_time.nMinute;
	g_time.state = CLOCKSTATE_OPERATION;

	// 頂点バッファの生成
	pDevice->CreateVertexBuffer(sizeof(VERTEX_3D) * 4 * NUM_PLACE * MAX_PLAYER, D3DUSAGE_WRITEONLY, FVF_VERTEX_3D, D3DPOOL_MANAGED, &g_pVtxBuffClock, NULL);

	VERTEX_3D* pVtx;
	// 頂点バッファをロックし、頂点情報へのポインタを取得
	g_pVtxBuffClock->Lock(0, 0, (void**)&pVtx, 0);

	for (int nCntPlayer = 0; nCntPlayer < MAX_PLAYER; nCntPlayer++)
	{
		for (int nCntClock = 0; nCntClock < NUM_PLACE; nCntClock++, pVtx += 4)
		{
			// 頂点座標の設定
			pVtx[0].pos = D3DXVECTOR3(0.0f, NUM_WIDTH, 0.0f);
			pVtx[1].pos = D3DXVECTOR3(NUM_WIDTH, NUM_WIDTH, 0.0f);
			pVtx[2].pos = D3DXVECTOR3(0.0f, -NUM_WIDTH, 0.0f);
			pVtx[3].pos = D3DXVECTOR3(NUM_WIDTH, -NUM_WIDTH, 0.0f);

			// rhwの設定
			pVtx[0].nor = NORMAL;
			pVtx[1].nor = NORMAL;
			pVtx[2].nor = NORMAL;
			pVtx[3].nor = NORMAL;

			// 頂点カラーの設定
			pVtx[0].col = g_aClock[nCntPlayer].col;
			pVtx[1].col = g_aClock[nCntPlayer].col;
			pVtx[2].col = g_aClock[nCntPlayer].col;
			pVtx[3].col = g_aClock[nCntPlayer].col;

			pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
			pVtx[1].tex = D3DXVECTOR2(0.1f, 0.0f);
			pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
			pVtx[3].tex = D3DXVECTOR2(0.1f, 1.0f);
		}
	}
	// 頂点バッファをアンロック
	g_pVtxBuffClock->Unlock();
	//#endif
}

//======================================================================================
// 時計の終了処理
//======================================================================================
void UninitClock(void)
{
	// テクスチャの破棄
	for (int nCntClock = 0; nCntClock < NUM_PLACE; nCntClock++)
	{
		if (g_apTextureClock[nCntClock] != NULL)
		{
			g_apTextureClock[nCntClock]->Release();
			g_apTextureClock[nCntClock] = NULL;
		}
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
	MODE mode = GetMode();
	if (mode == MODE_TUTORIAL)
	{
		g_time.state = CLOCKSTATE_STOP;
	}

	switch (g_time.state)
	{
	case CLOCKSTATE_NONE:	// 何もしていない状態

		break;

	case CLOCKSTATE_OPERATION:	// 稼働中
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
		break;

	case CLOCKSTATE_STOP:	// 停止中

		break;

	case CLOCKSTATE_MAGIC:	// 魔法による停止中
		g_time.nMagicCounter++;	// カウンターを加算
		if (g_time.nMagicCounter >= MAGIC_TIME)
		{
			g_time.nMagicCounter = 0;
			SetClockState(CLOCKSTATE_OPERATION);
		}
		break;
	}


	// 時間管理
	g_time.nTime = g_time.nHour * 100 + g_time.nMinute;

	PrintDebugProc("現在時刻 %d\n", g_time.nTime);

	if (g_time.nTime >= END_TIME)
	{// 一定時間を超えたらゲーム終了
		//SetGameState(GAMESTATE_TIMEOVER, 0);
	}

	int aTexU[NUM_PLACE];	// 各桁の数値を格納
	aTexU[0] = g_time.nTime % 10000 / 1000;
	aTexU[1] = g_time.nTime % 1000 / 100;
	aTexU[2] = g_time.nTime % 100 / 10;
	aTexU[3] = g_time.nTime % 10 / 1;

	VERTEX_3D* pVtx;
	// 頂点バッファをロックし、頂点情報へのポインタを取得
	g_pVtxBuffClock->Lock(0, 0, (void**)&pVtx, 0);
	for (int nCntPlayer = 0; nCntPlayer < MAX_PLAYER; nCntPlayer++)
	{
		for (int nCntClock = 0; nCntClock < NUM_PLACE; nCntClock++, pVtx += 4)
		{
			// 頂点カラーの設定
			pVtx[0].col = g_aClock[nCntPlayer].col;
			pVtx[1].col = g_aClock[nCntPlayer].col;
			pVtx[2].col = g_aClock[nCntPlayer].col;
			pVtx[3].col = g_aClock[nCntPlayer].col;

			// テクスチャ座標の設定
			pVtx[0].tex = D3DXVECTOR2(aTexU[nCntClock] * 0.1f, 0.0f);
			pVtx[1].tex = D3DXVECTOR2(aTexU[nCntClock] * 0.1f + 0.1f, 0.0f);
			pVtx[2].tex = D3DXVECTOR2(aTexU[nCntClock] * 0.1f, 1.0f);
			pVtx[3].tex = D3DXVECTOR2(aTexU[nCntClock] * 0.1f + 0.1f, 1.0f);
		}
	}
	// 頂点バッファをアンロック
	g_pVtxBuffClock->Unlock();

	// 時計塔のモデルを動かす========================================================
	ParentObject* pParentObject = GetParentObjectInfo(g_nIdxClockTower);
	// 分針を更新する
	float fMin = ((float)g_time.nMinute / MAX_MIN) * (-MAX_ANGLE);
	for (int nMin = 0; nMin < NUM_TOWER; nMin++)
	{
		pParentObject->OffSetData.rot[nMin * 2 + 2].z = fMin;
	}
	// 時針を更新する
	float fHour = (float)g_time.nHour;
	//if (fHour >= (float)HALF_HOUR)	// 12時を越えたら整理する
	//{
	//	fHour -= (float)HALF_HOUR;
	//}
	fHour = fHour / HALF_HOUR * (-MAX_ANGLE);
	fMin = ((float)g_time.nMinute / MAX_MIN) * (-MAX_ANGLE / HALF_HOUR);
	for (int nHour = 0; nHour < NUM_TOWER; nHour++)
	{
		pParentObject->OffSetData.rot[nHour * 2 + 1].z = fHour + fMin;
	}

	for (int nCntModel = 1; nCntModel < pParentObject->pModelData->nNumParts; nCntModel++)
	{
		pParentObject->OffSetData.rot[nCntModel].z = AngleNormalize(pParentObject->OffSetData.rot[nCntModel].z);
	}
}

//======================================================================================
// 時計の描画処理
//======================================================================================
void DrawClock(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();	// デバイスの取得
	// UIのマトリックス情報を取得
	D3DXMATRIX UIMatrix;

	// ワールドマトリックスの初期化(デフォルトの値にする)
	D3DXMatrixIdentity(&UIMatrix);

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
		if (g_aClock[nCntPlayer].bDisp == false)
		{
			continue;
		}
		// UIのマトリックス情報を取得
		UIMatrix = GetUIMatrix(nCntPlayer);

		// ワールドマトリックスの設定
		pDevice->SetTransform(D3DTS_WORLD, &UIMatrix);

		for (int nCntClock = 0; nCntClock < NUM_PLACE; nCntClock++)
		{
			D3DXMATRIX	mtxRotModel, mtxTransModel;	// 計算用マトリックス
			D3DXMATRIX	mtxParent;					// 親のマトリックス

			// ポリゴンのワールドマトリックスを初期化
			D3DXMatrixIdentity(&g_aClock[nCntPlayer].mtxWorld);

			// パーツの位置を反映
			D3DXMatrixTranslation(&mtxTransModel, g_aClock[nCntPlayer].pos[nCntClock].x, g_aClock[nCntPlayer].pos[nCntClock].y, g_aClock[nCntPlayer].pos[nCntClock].z);
			D3DXMatrixMultiply(&g_aClock[nCntPlayer].mtxWorld, &g_aClock[nCntPlayer].mtxWorld, &mtxTransModel);

			// 親マトリックスを設定
			mtxParent = UIMatrix;

			// 算出したパーツのワールドマトリックスと親モデルのマトリックスを掛け合わせる
			D3DXMatrixMultiply(&g_aClock[nCntPlayer].mtxWorld, &g_aClock[nCntPlayer].mtxWorld, &mtxParent);

			// パーツのワールドマトリックスを設定
			pDevice->SetTransform(D3DTS_WORLD, &g_aClock[nCntPlayer].mtxWorld);

			// 頂点バッファをデータストリームに設定
			pDevice->SetStreamSource(0, g_pVtxBuffClock, 0, sizeof(VERTEX_3D));

			// 頂点フォーマットの設定
			pDevice->SetFVF(FVF_VERTEX_3D);

			// テクスチャの設定
			pDevice->SetTexture(0, g_apTextureClock[0]);

			// UIの描画
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
// 時計の設置処理
//======================================================================================
void SetClock(int nIdx)
{
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

//======================================================================================
// 時間を返す
//======================================================================================
int GetTime(void)
{
	return g_time.nTime;
}

//======================================================================================
// 分を返す
//======================================================================================
int GetMinute(void)
{
	return g_time.nMinute;
}

//======================================================================================
// 時計塔のインデックスを取得
//======================================================================================
void SetClockTowerIdx(int nIdx)
{
	g_nIdxClockTower = nIdx;
}