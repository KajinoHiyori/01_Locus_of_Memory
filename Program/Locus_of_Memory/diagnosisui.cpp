//======================================================================================
// 
// 診断画面の2DUI処理[diagnosisui.h]
// Author : Kajino Hiyori
//
//======================================================================================
#include "main.h"
#include "fade.h"
#include "resultui.h"
#include "diagnosisui.h"
#include "diagnosis.h"
#include "input.h"
#include "fog.h"

// 診断結果UIの種類
typedef enum
{
	DIAGNOSISTYPE_TITLE = 0,		// 診断結果ロゴ
	DIAGNOSISTYPE_USEDCOMMAND,		// 使われたコマンド
	DIAGNOSISTYPE_R_COMMANDTYPE,	// Rコマンドの種類
	DIAGNOSISTYPE_R_COMMAND100,		// Rコマンド回数[X00]
	DIAGNOSISTYPE_R_COMMAND010,		// Rコマンド回数[0X0]
	DIAGNOSISTYPE_R_COMMAND001,		// Rコマンド回数[00X]
	DIAGNOSISTYPE_R_COMMANDTIMES,	// RXXX回
	DIAGNOSISTYPE_G_COMMANDTYPE,	// Gコマンドの種類
	DIAGNOSISTYPE_G_COMMAND100,		// Gコマンド回数[X00]
	DIAGNOSISTYPE_G_COMMAND010,		// Gコマンド回数[0X0]
	DIAGNOSISTYPE_G_COMMAND001,		// Gコマンド回数[00X]
	DIAGNOSISTYPE_G_COMMANDTIMES,	// GXXX回
	DIAGNOSISTYPE_B_COMMANDTYPE,	// Bコマンドの種類
	DIAGNOSISTYPE_B_COMMAND100,		// Bコマンド回数[X00]
	DIAGNOSISTYPE_B_COMMAND010,		// Bコマンド回数[0X0]
	DIAGNOSISTYPE_B_COMMAND001,		// Bコマンド回数[00X]
	DIAGNOSISTYPE_B_COMMANDTIMES,	// BXXX回
	DIAGNOSISTYPE_Y_COMMANDTYPE,	// Yコマンドの種類
	DIAGNOSISTYPE_Y_COMMAND100,		// Yコマンド回数[X00]
	DIAGNOSISTYPE_Y_COMMAND010,		// Yコマンド回数[0X0]
	DIAGNOSISTYPE_Y_COMMAND001,		// Yコマンド回数[00X]
	DIAGNOSISTYPE_Y_COMMANDTIMES,	// YXXX回
	DIAGNOSISTYPE_MOSTMAGIC,		// 1番使われた魔法
	DIAGNOSISTYPE_MAGICCIRCLE,		// 魔法陣
	DIAGNOSISTYPE_HAPPENDEVENT,		// 発生したイベント数
	DIAGNOSISTYPE_EVENT100,			// イベント数[X00]
	DIAGNOSISTYPE_EVENT010,			// イベント数[0X0]
	DIAGNOSISTYPE_EVENT001,			// イベント数[00X]
	DIAGNOSISTYPE_EVENTTIMES,		// イベントXXX回
	DIAGNOSISTYPE_MAX
}DIAGNOSISTYPE;

// 診断結果UIのテクスチャの種類
typedef enum
{
	DIAGNOSISTEX_TITLE = 0,		// 診断結果ロゴ
	DIAGNOSISTEX_USEDCOMMAND,	// 使われたコマンド数
	DIAGNOSISTEX_R,				// 赤魔法
	DIAGNOSISTEX_G,				// 緑魔法
	DIAGNOSISTEX_B,				// 青魔法
	DIAGNOSISTEX_Y,				// 黄魔法
	DIAGNOSISTEX_MOSTMAGIC,		// 1番使われた魔法
	DIAGNOSISTEX_LEVITATION,	// 浮遊
	DIAGNOSISTEX_COMBUSTION,	// 燃焼
	DIAGNOSISTEX_FLOOD,			// 洪水
	DIAGNOSISTEX_FLASH,			// フラッシュ
	DIAGNOSISTEX_FIREBALL,		// 火球
	DIAGNOSISTEX_SUNSETDELAY,	// 時間停止
	DIAGNOSISTEX_RAINPRAY,		// 雨乞い
	DIAGNOSISTEX_FREEZE,		// 凍結
	DIAGNOSISTEX_GROWTH,		// 成長
	DIAGNOSISTEX_ACCELERATION,	// 加速
	DIAGNOSISTEX_TIMEREVERT,	// 巻き戻し
	DIAGNOSISTEX_HAPPENDEVENT,	// 発生したイベント数
	DIAGNOSISTEX_TIMES,			// 回
	DIAGNOSISTEX_NUMBER,		// 数字
	DIAGNOSISTEX_MAX
}DIAGNOSISTEX;

// 診断結果UIの構造体
typedef struct
{
	DIAGNOSISTYPE	type;	// 種類
	DIAGNOSISTEX	tex;	// 停止
	D3DXVECTOR3		pos;	// 位置
	float	fWidth;			// 幅
	float	fHeight;		// 高さ
	bool	bDisp;			// 表示状態
}DiagnosisUI;

// マクロ定義
#define NUM_DIAGNOSISTYPE	(DIAGNOSISTYPE_MAX)	// 診断結果UIの表示種類
#define DIAGNOSIS_TEX		(DIAGNOSISTEX_MAX)	// テクスチャの種類
#define TITLE_W				(460.0f)	// プレイヤーたちの軌跡[幅]
#define TITLE_H				(60.0f)		// プレイヤーたちの軌跡[高]
#define COMMAND_W			(50.0f)		// コマンドの幅
#define COMMAND_H			(50.0f)		// コマンドの高さ
#define MAGICCIRCLE_SIZE	(60.0f)		// 魔法陣の大きさ
#define TIMES_W				(40.0f)		// 回の幅
#define TIMES_H				(40.0f)		// 回の高さ
#define NUMBER_W			(TIMES_W)	// 数字の幅
#define NUMBER_H			(TIMES_H)	// 数字の高さ
#define TEXT_W				(460.0f * 0.8f)	// タイトル以外の題字
#define TEXT_H				(60.0f * 0.8f)	// タイトル以外の題字

// テクスチャの読み込み
const char* c_apFilenameDiagnosisUI[DIAGNOSIS_TEX] =
{
	"data\\TEXTURE\\diagnosis\\diagnosis000.png",	// 診断結果ロゴ
	"data\\TEXTURE\\diagnosis\\diagnosis001.png",	// 使われたコマンド数
	"data\\TEXTURE\\SpellUI\\01_Red.png",			// 赤魔法
	"data\\TEXTURE\\SpellUI\\02_Green.png",			// 緑魔法
	"data\\TEXTURE\\SpellUI\\03_Blue.png",			// 青魔法
	"data\\TEXTURE\\SpellUI\\04_Yellow.png",		// 黄魔法
	"data\\TEXTURE\\diagnosis\\diagnosis002.png",	// 1番使われた魔法
	"data\\TEXTURE\\SpellUI\\14_Levitation.png",	// 浮遊
	"data\\TEXTURE\\SpellUI\\15_Combustion.png",	// 燃焼
	"data\\TEXTURE\\SpellUI\\16_Flood.png",			// 洪水
	"data\\TEXTURE\\SpellUI\\17_Flash.png",			// フラッシュ
	"data\\TEXTURE\\SpellUI\\18_FireBall.png",		// 火球
	"data\\TEXTURE\\SpellUI\\19_SunsetDelay.png",	// 時間停止
	"data\\TEXTURE\\SpellUI\\20_RainPray.png",		// 雨乞い
	"data\\TEXTURE\\SpellUI\\21_freeze.png",		// 凍結
	"data\\TEXTURE\\SpellUI\\22_Grouth.png",		// 成長
	"data\\TEXTURE\\SpellUI\\23_Acceleration.png",	// 加速
	"data\\TEXTURE\\SpellUI\\24_TimeRevert.png",	// 巻き戻し
	"data\\TEXTURE\\diagnosis\\diagnosis003.png",	// 発生したイベント数
	"data\\TEXTURE\\diagnosis\\diagnosis004.png",	// 回
	"data\\TEXTURE\\number.png",					// 数字
};

// グローバル変数
LPDIRECT3DTEXTURE9 g_apTextureDiagnosisUI[DIAGNOSIS_TEX] = {};	// テクスチャへのポインタ
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffDiagnosisUI = NULL; // 頂点バッファへのポインタ
DiagnosisUI g_aDiagnosis[NUM_DIAGNOSISTYPE];	// 構造体

//========================================================================
// 診断結果UIの初期化処理
//========================================================================
void InitDiagnosisUI(void)
{
	LPDIRECT3DDEVICE9 pDevice;	// デバイスへのポインタ
	// デバイスの取得
	pDevice = GetDevice();

	// テクスチャの読み込み
	for (int nCntUI = 0; nCntUI < DIAGNOSIS_TEX; nCntUI++)
	{
		D3DXCreateTextureFromFile(pDevice, c_apFilenameDiagnosisUI[nCntUI], &g_apTextureDiagnosisUI[nCntUI]);
	}

	// 頂点バッファの生成
	pDevice->CreateVertexBuffer(sizeof(VERTEX_2D) * 4 * NUM_DIAGNOSISTYPE, D3DUSAGE_WRITEONLY, FVF_VERTEX_2D, D3DPOOL_MANAGED, &g_pVtxBuffDiagnosisUI, NULL);

	// UIの配置
	SetDiagnosisUI();

	//for (int nCntDiagnosis = 0; nCntDiagnosis < NUM_DIAGNOSISTYPE; nCntDiagnosis++, pVtx += 4)
	//{
	//	// 頂点座標の設定
	//	pVtx[0].pos = D3DXVECTOR3(g_aDiagnosis[nCntDiagnosis].pos.x - g_aDiagnosis[nCntDiagnosis].fWidth, g_aDiagnosis[nCntDiagnosis].pos.y - g_aDiagnosis[nCntDiagnosis].fHeight, 0.0f);
	//	pVtx[1].pos = D3DXVECTOR3(g_aDiagnosis[nCntDiagnosis].pos.x + g_aDiagnosis[nCntDiagnosis].fWidth, g_aDiagnosis[nCntDiagnosis].pos.y - g_aDiagnosis[nCntDiagnosis].fHeight, 0.0f);
	//	pVtx[2].pos = D3DXVECTOR3(g_aDiagnosis[nCntDiagnosis].pos.x - g_aDiagnosis[nCntDiagnosis].fWidth, g_aDiagnosis[nCntDiagnosis].pos.y + g_aDiagnosis[nCntDiagnosis].fHeight, 0.0f);
	//	pVtx[3].pos = D3DXVECTOR3(g_aDiagnosis[nCntDiagnosis].pos.x + g_aDiagnosis[nCntDiagnosis].fWidth, g_aDiagnosis[nCntDiagnosis].pos.y + g_aDiagnosis[nCntDiagnosis].fHeight, 0.0f);
	//	// 頂点カラーの設定
	//	pVtx[0].col = D3DXCOLOR(1.0f, 0.0f, 1.0f, 1.0f);
	//	pVtx[1].col = D3DXCOLOR(1.0f, 0.0f, 1.0f, 1.0f);
	//	pVtx[2].col = D3DXCOLOR(1.0f, 0.0f, 1.0f, 1.0f);
	//	pVtx[3].col = D3DXCOLOR(1.0f, 0.0f, 1.0f, 1.0f);
	//	// rhwの設定
	//	pVtx[0].rhw = 1.0f;
	//	pVtx[1].rhw = 1.0f;
	//	pVtx[2].rhw = 1.0f;
	//	pVtx[3].rhw = 1.0f;
	//	// テクスチャ座標の設定
	//	pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
	//	pVtx[1].tex = D3DXVECTOR2(1.0f, 0.0f);
	//	pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
	//	pVtx[3].tex = D3DXVECTOR2(1.0f, 1.0f);
	//	g_aDiagnosis[nCntDiagnosis].bDisp = true;
	//}
}

//========================================================================
// 診断結果UIの終了処理
//========================================================================
void UninitDiagnosisUI(void)
{
	// テクスチャの破棄
	for (int nCntDiagnosis = 0; nCntDiagnosis < DIAGNOSIS_TEX; nCntDiagnosis++)
	{
		if (g_apTextureDiagnosisUI[nCntDiagnosis] != NULL)
		{
			g_apTextureDiagnosisUI[nCntDiagnosis]->Release();
			g_apTextureDiagnosisUI[nCntDiagnosis] = NULL;
		}
	}

	// 頂点バッファの破棄
	if (g_pVtxBuffDiagnosisUI != NULL)
	{
		g_pVtxBuffDiagnosisUI->Release();
		g_pVtxBuffDiagnosisUI = NULL;
	}
}

//========================================================================
// 診断結果UIの更新処理
//========================================================================
void UpdateDiagnosisUI(void)
{
	
}

//========================================================================
// 診断結果UIの描画処理
//========================================================================
void DrawDiagnosisUI(void)
{
	LPDIRECT3DDEVICE9 pDevice;	// デバイスへのポインタ
	// デバイスの取得
	pDevice = GetDevice();

	// ライトをオフにする
	pDevice->SetRenderState(D3DRS_LIGHTING, FALSE);
	SetFogEnable(false);
	// 頂点バッファをデータストリームに設定
	pDevice->SetStreamSource(0, g_pVtxBuffDiagnosisUI, 0, sizeof(VERTEX_2D));

	// 頂点フォーマットの設定
	pDevice->SetFVF(FVF_VERTEX_2D);

	for (int nCntDiagnosis = 0; nCntDiagnosis < NUM_DIAGNOSISTYPE; nCntDiagnosis++)
	{
		if (g_aDiagnosis[nCntDiagnosis].bDisp == true)
		{
			// テクスチャの設定
			pDevice->SetTexture(0, g_apTextureDiagnosisUI[g_aDiagnosis[nCntDiagnosis].tex]);
			// ポリゴンの描画
			pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, nCntDiagnosis * 4, 2);
		}
	}
	// ライトをオンにする
	pDevice->SetRenderState(D3DRS_LIGHTING, TRUE);
	SetFogEnable(true);
}

//========================================================================
// 診断結果UIの設定
//========================================================================
void SetDiagnosisUI(void)
{
	TotalData DiagnosisData = LoadTotalData();	// セーブされている情報を格納

	VERTEX_2D* pVtx;
	// 頂点バッファをロックし、頂点情報へのポインタを取得
	g_pVtxBuffDiagnosisUI->Lock(0, 0, (void**)&pVtx, 0);

	for (int nCntUI = 0; nCntUI < NUM_DIAGNOSISTYPE; nCntUI++)
	{
		switch (nCntUI)
		{
		case DIAGNOSISTEX_TITLE:	// 診断結果
			g_aDiagnosis[nCntUI].bDisp = true;
			g_aDiagnosis[nCntUI].fHeight = TITLE_H;
			g_aDiagnosis[nCntUI].fWidth = TITLE_W;
			g_aDiagnosis[nCntUI].pos = INIT_D3DXVEC3;
				break;

		default:
			break;
		}
		// DIAGNOSISTEX_TITLE = 0,		// 診断結果ロゴ
		// DIAGNOSISTEX_USEDCOMMAND,	// 使われたコマンド数
		// DIAGNOSISTEX_R,				// 赤魔法
		// DIAGNOSISTEX_G,				// 緑魔法
		// DIAGNOSISTEX_B,				// 青魔法
		// DIAGNOSISTEX_Y,				// 黄魔法
		// DIAGNOSISTEX_MOSTMAGIC,		// 1番使われた魔法
		// DIAGNOSISTEX_LEVITATION,	// 浮遊
		// DIAGNOSISTEX_COMBUSTION,	// 燃焼
		// DIAGNOSISTEX_FLOOD,			// 洪水
		// DIAGNOSISTEX_FLASH,			// フラッシュ
		// DIAGNOSISTEX_FIREBALL,		// 火球
		// DIAGNOSISTEX_SUNSETDELAY,	// 時間停止
		// DIAGNOSISTEX_RAINPRAY,		// 雨乞い
		// DIAGNOSISTEX_FREEZE,		// 凍結
		// DIAGNOSISTEX_GROWTH,		// 成長
		// DIAGNOSISTEX_ACCELERATION,	// 加速
		// DIAGNOSISTEX_TIMEREVERT,	// 巻き戻し
		// DIAGNOSISTEX_HAPPENDEVENT,	// 発生したイベント数
		// DIAGNOSISTEX_TIMES,			// 回
		// DIAGNOSISTEX_NUMBER,		// 数字
	}

	// 頂点バッファをアンロック
	g_pVtxBuffDiagnosisUI->Unlock();
}