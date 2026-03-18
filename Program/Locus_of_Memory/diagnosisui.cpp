//======================================================================================
// 
// 診断画面の2DUI処理[diagnosisui.h]
// Author : Kajino Hiyori
//
//======================================================================================
#include "main.h"
#include "fade.h"
#include "diagnosisui.h"
#include "diagnosis.h"
#include "input.h"
#include "fog.h"
#include "color.h"
#include "magic.h"

// 診断結果UIの構造体
typedef struct
{
	DIAGNOSISTYPE	type;	// 種類
	DIAGNOSISTEX	tex;	// テクスチャの種類
	D3DXVECTOR3		pos;	// 位置
	float	fWidth;			// 幅
	float	fHeight;		// 高さ
	bool	bDisp;			// 表示状態
}DiagnosisUI;

// マクロ定義
#define NUM_DIAGNOSISTYPE	(DIAGNOSISTYPE_MAX)	// 診断結果UIの表示種類
#define DIAGNOSIS_TEX		(DIAGNOSISTEX_MAX)	// テクスチャの種類
#define TITLE_W				(460.0f * 0.65f)	// プレイヤーたちの軌跡[幅]
#define TITLE_H				(60.0f * 0.65f)		// プレイヤーたちの軌跡[高]
#define COMMAND_W			(40.0f)		// コマンドの幅
#define COMMAND_H			(40.0f)		// コマンドの高さ
#define MAGICCIRCLE_SIZE	(120.0f)	// 魔法陣の大きさ
#define TIMES_W				(30.0f)		// 回の幅
#define TIMES_H				(30.0f)		// 回の高さ
#define NUMBER_W			(TIMES_W)	// 数字の幅
#define NUMBER_H			(TIMES_H)	// 数字の高さ
#define TEXT_W				(TITLE_W * 0.8f)	// タイトル以外の題字
#define TEXT_H				(TITLE_H * 0.8f)	// タイトル以外の題字
// 配置
#define TITLE_POS		(D3DXVECTOR3(350.0f, 50.0f, 0.0f))									// 診断結果
#define USED_POS		(D3DXVECTOR3(250.0f, 150.0f, 0.0f))									// 使われたコマンド数
#define RED_POS			(D3DXVECTOR3(90.0f, 250.0f, 0.0f))									// Rコマンド
#define RPOS_100		(D3DXVECTOR3(RED_POS.x + 80.0f, RED_POS.y, 0.0f))					// R[X00]
#define RPOS_010		(D3DXVECTOR3(RPOS_100.x + NUMBER_W * 2, RED_POS.y, 0.0f))			// R[0X0]
#define RPOS_001		(D3DXVECTOR3(RPOS_010.x + NUMBER_W * 2, RED_POS.y, 0.0f))			// R[0X0]
#define RPOS_TIMES		(D3DXVECTOR3(RPOS_001.x + NUMBER_W + TIMES_W, RED_POS.y, 0.0f))		// 回
#define GREEN_POS		(D3DXVECTOR3(RPOS_TIMES.x + 120.0f, RED_POS.y, 0.0f))				// Gコマンド
#define GPOS_100		(D3DXVECTOR3(GREEN_POS.x + 80.0f, GREEN_POS.y, 0.0f))				// G[X00]
#define GPOS_010		(D3DXVECTOR3(GPOS_100.x + NUMBER_W * 2, GREEN_POS.y, 0.0f))			// G[0X0]
#define GPOS_001		(D3DXVECTOR3(GPOS_010.x + NUMBER_W * 2, GREEN_POS.y, 0.0f))			// G[0X0]
#define GPOS_TIMES		(D3DXVECTOR3(GPOS_001.x + NUMBER_W + TIMES_W, GREEN_POS.y, 0.0f))	// 回
#define BLUE_POS		(D3DXVECTOR3(90.0f, 340.0f, 0.0f))									// Bコマンド
#define BPOS_100		(D3DXVECTOR3(BLUE_POS.x + 80.0f, BLUE_POS.y, 0.0f))					// B[X00]
#define BPOS_010		(D3DXVECTOR3(BPOS_100.x + NUMBER_W * 2, BLUE_POS.y, 0.0f))			// B[0X0]
#define BPOS_001		(D3DXVECTOR3(BPOS_010.x + NUMBER_W * 2, BLUE_POS.y, 0.0f))			// B[0X0]
#define BPOS_TIMES		(D3DXVECTOR3(BPOS_001.x + NUMBER_W + TIMES_W, BLUE_POS.y, 0.0f))	// 回
#define YELLOW_POS		(D3DXVECTOR3(RPOS_TIMES.x + 120.0f, BLUE_POS.y, 0.0f))				// Gコマンド
#define YPOS_100		(D3DXVECTOR3(YELLOW_POS.x + 80.0f, YELLOW_POS.y, 0.0f))				// G[X00]
#define YPOS_010		(D3DXVECTOR3(YPOS_100.x + NUMBER_W * 2, YELLOW_POS.y, 0.0f))		// G[0X0]
#define YPOS_001		(D3DXVECTOR3(YPOS_010.x + NUMBER_W * 2, YELLOW_POS.y, 0.0f))		// G[0X0]
#define YPOS_TIMES		(D3DXVECTOR3(YPOS_001.x + NUMBER_W + TIMES_W, YELLOW_POS.y, 0.0f))	// 回
#define MAGIC_POS		(D3DXVECTOR3(USED_POS.x - 25.0f, 450.0f, 0.0f))						// 使われた魔法
#define CIRCLE_POS		(D3DXVECTOR3(500.0f, 590.0f, 0.0f))	// 魔法陣の位置
#define EVENT_POS		(D3DXVECTOR3(1020.0f, 500.0f, 0.0f))	// イベントの位置
#define EVENTPOS_100	(D3DXVECTOR3(EVENT_POS.x + NUMBER_W * 2 - 15.0f, EVENT_POS.y + 75.0f, 0.0f))	// Event[X00]
#define EVENTPOS_010	(D3DXVECTOR3(EVENTPOS_100.x + NUMBER_W * 2, EVENTPOS_100.y, 0.0f))	// Event[X00]
#define EVENTPOS_001	(D3DXVECTOR3(EVENTPOS_010.x + NUMBER_W * 2, EVENTPOS_100.y, 0.0f))	// Event[X00]
#define EVENTPOS_TIMES	(D3DXVECTOR3(EVENTPOS_001.x + NUMBER_W + TIMES_W, EVENTPOS_100.y, 0.0f))	// Event[X00]

// テクスチャの読み込み
const char* c_apFilenameDiagnosisUI[DIAGNOSIS_TEX] =
{
	"data\\TEXTURE\\diagnosis\\diagnosis000.png",	// 診断結果
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

	for (int nCntUI = 0; nCntUI < NUM_DIAGNOSISTYPE; nCntUI++)
	{
		g_aDiagnosis[nCntUI].type = DIAGNOSISTYPE_TITLE;	// 種類
		g_aDiagnosis[nCntUI].tex = DIAGNOSISTEX_TITLE;	// テクスチャの種類
		g_aDiagnosis[nCntUI].pos = INIT_D3DXVEC3;	// 位置
		g_aDiagnosis[nCntUI].fWidth = 0.0f;			// 幅
		g_aDiagnosis[nCntUI].fHeight = 0.0f;		// 高さ
		g_aDiagnosis[nCntUI].bDisp = false;			// 表示状態
	}

	// 頂点バッファの生成
	pDevice->CreateVertexBuffer(sizeof(VERTEX_2D) * 4 * NUM_DIAGNOSISTYPE, D3DUSAGE_WRITEONLY, FVF_VERTEX_2D, D3DPOOL_MANAGED, &g_pVtxBuffDiagnosisUI, NULL);

	// UIの配置
	SetDiagnosisUI();
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
	// fogを無効化
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
	// fogを有効化
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

	for (int nCntUI = 0; nCntUI < NUM_DIAGNOSISTYPE; nCntUI++, pVtx += 4)
	{
		int TecU = 0;	// 数字の幅を管理
		switch (nCntUI)
		{
		case DIAGNOSISTYPE_TITLE:	// 診断結果
			g_aDiagnosis[nCntUI].fHeight = TITLE_H;
			g_aDiagnosis[nCntUI].fWidth = TITLE_W;
			g_aDiagnosis[nCntUI].pos = TITLE_POS;
			g_aDiagnosis[nCntUI].tex = DIAGNOSISTEX_TITLE;
			g_aDiagnosis[nCntUI].type = DIAGNOSISTYPE_TITLE;
			// テクスチャ座標の設定
			pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
			pVtx[1].tex = D3DXVECTOR2(1.0f, 0.0f);
			pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
			pVtx[3].tex = D3DXVECTOR2(1.0f, 1.0f);
			break;

		case DIAGNOSISTYPE_USEDCOMMAND:	// 使われたコマンド数
			g_aDiagnosis[nCntUI].fHeight = TEXT_H;
			g_aDiagnosis[nCntUI].fWidth = TEXT_W;
			g_aDiagnosis[nCntUI].pos = USED_POS;
			g_aDiagnosis[nCntUI].tex = DIAGNOSISTEX_USEDCOMMAND;
			g_aDiagnosis[nCntUI].type = DIAGNOSISTYPE_USEDCOMMAND;
			// テクスチャ座標の設定
			pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
			pVtx[1].tex = D3DXVECTOR2(1.0f, 0.0f);
			pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
			pVtx[3].tex = D3DXVECTOR2(1.0f, 1.0f);
			break;

		case DIAGNOSISTYPE_R_COMMANDTYPE:	// Rコマンドの種類
			g_aDiagnosis[nCntUI].fHeight = COMMAND_H;
			g_aDiagnosis[nCntUI].fWidth = COMMAND_W;
			g_aDiagnosis[nCntUI].pos = RED_POS;
			g_aDiagnosis[nCntUI].tex = DIAGNOSISTEX_R;
			g_aDiagnosis[nCntUI].type = DIAGNOSISTYPE_R_COMMANDTYPE;
			// テクスチャ座標の設定
			pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
			pVtx[1].tex = D3DXVECTOR2(1.0f, 0.0f);
			pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
			pVtx[3].tex = D3DXVECTOR2(1.0f, 1.0f);
			break;

		case DIAGNOSISTYPE_R_COMMAND100:	// 回数100
			g_aDiagnosis[nCntUI].fHeight = NUMBER_H;
			g_aDiagnosis[nCntUI].fWidth = NUMBER_W;
			g_aDiagnosis[nCntUI].pos = RPOS_100;
			g_aDiagnosis[nCntUI].tex = DIAGNOSISTEX_NUMBER;
			g_aDiagnosis[nCntUI].type = DIAGNOSISTYPE_R_COMMAND100;
			// 数字の情報を取得
			TecU = DiagnosisData.nRed % 1000 / 100;
			// テクスチャ座標の設定
			pVtx[0].tex = D3DXVECTOR2(TecU * 0.1f, 0.0f);
			pVtx[1].tex = D3DXVECTOR2(TecU * 0.1f + 0.1f, 0.0f);
			pVtx[2].tex = D3DXVECTOR2(TecU * 0.1f, 1.0f);
			pVtx[3].tex = D3DXVECTOR2(TecU * 0.1f + 0.1f, 1.0f);
			break;

		case DIAGNOSISTYPE_R_COMMAND010:	// 回数10
			g_aDiagnosis[nCntUI].fHeight = NUMBER_H;
			g_aDiagnosis[nCntUI].fWidth = NUMBER_W;
			g_aDiagnosis[nCntUI].pos = RPOS_010;
			g_aDiagnosis[nCntUI].tex = DIAGNOSISTEX_NUMBER;
			g_aDiagnosis[nCntUI].type = DIAGNOSISTYPE_R_COMMAND010;
			// 数字の情報を取得
			TecU = DiagnosisData.nRed % 100 / 10;
			// テクスチャ座標の設定
			pVtx[0].tex = D3DXVECTOR2(TecU * 0.1f, 0.0f);
			pVtx[1].tex = D3DXVECTOR2(TecU * 0.1f + 0.1f, 0.0f);
			pVtx[2].tex = D3DXVECTOR2(TecU * 0.1f, 1.0f);
			pVtx[3].tex = D3DXVECTOR2(TecU * 0.1f + 0.1f, 1.0f);
			break;

		case DIAGNOSISTYPE_R_COMMAND001:	// 回数1
			g_aDiagnosis[nCntUI].fHeight = NUMBER_H;
			g_aDiagnosis[nCntUI].fWidth = NUMBER_W;
			g_aDiagnosis[nCntUI].pos = RPOS_001;
			g_aDiagnosis[nCntUI].tex = DIAGNOSISTEX_NUMBER;
			g_aDiagnosis[nCntUI].type = DIAGNOSISTYPE_R_COMMAND001;
			// 数字の情報を取得
			TecU = DiagnosisData.nRed % 10 / 1;
			// テクスチャ座標の設定
			pVtx[0].tex = D3DXVECTOR2(TecU * 0.1f, 0.0f);
			pVtx[1].tex = D3DXVECTOR2(TecU * 0.1f + 0.1f, 0.0f);
			pVtx[2].tex = D3DXVECTOR2(TecU * 0.1f, 1.0f);
			pVtx[3].tex = D3DXVECTOR2(TecU * 0.1f + 0.1f, 1.0f);
			break;

		case DIAGNOSISTYPE_R_COMMANDTIMES:	// 回
			g_aDiagnosis[nCntUI].fHeight = TIMES_H;
			g_aDiagnosis[nCntUI].fWidth = TIMES_W;
			g_aDiagnosis[nCntUI].pos = RPOS_TIMES;
			g_aDiagnosis[nCntUI].tex = DIAGNOSISTEX_TIMES;
			g_aDiagnosis[nCntUI].type = DIAGNOSISTYPE_R_COMMANDTIMES;
			// テクスチャ座標の設定
			pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
			pVtx[1].tex = D3DXVECTOR2(1.0f, 0.0f);
			pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
			pVtx[3].tex = D3DXVECTOR2(1.0f, 1.0f);
			break;

		case DIAGNOSISTYPE_G_COMMANDTYPE:	// Gコマンドの種類
			g_aDiagnosis[nCntUI].fHeight = COMMAND_H;
			g_aDiagnosis[nCntUI].fWidth = COMMAND_W;
			g_aDiagnosis[nCntUI].pos = GREEN_POS;
			g_aDiagnosis[nCntUI].tex = DIAGNOSISTEX_G;
			g_aDiagnosis[nCntUI].type = DIAGNOSISTYPE_G_COMMANDTYPE;
			// テクスチャ座標の設定
			pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
			pVtx[1].tex = D3DXVECTOR2(1.0f, 0.0f);
			pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
			pVtx[3].tex = D3DXVECTOR2(1.0f, 1.0f);
			break;

		case DIAGNOSISTYPE_G_COMMAND100:	// 回数100
			g_aDiagnosis[nCntUI].fHeight = NUMBER_H;
			g_aDiagnosis[nCntUI].fWidth = NUMBER_W;
			g_aDiagnosis[nCntUI].pos = GPOS_100;
			g_aDiagnosis[nCntUI].tex = DIAGNOSISTEX_NUMBER;
			g_aDiagnosis[nCntUI].type = DIAGNOSISTYPE_G_COMMAND100;
			// 数字の情報を取得
			TecU = DiagnosisData.nGreen % 1000 / 100;
			// テクスチャ座標の設定
			pVtx[0].tex = D3DXVECTOR2(TecU * 0.1f, 0.0f);
			pVtx[1].tex = D3DXVECTOR2(TecU * 0.1f + 0.1f, 0.0f);
			pVtx[2].tex = D3DXVECTOR2(TecU * 0.1f, 1.0f);
			pVtx[3].tex = D3DXVECTOR2(TecU * 0.1f + 0.1f, 1.0f);
			break;

		case DIAGNOSISTYPE_G_COMMAND010:	// 回数10
			g_aDiagnosis[nCntUI].fHeight = NUMBER_H;
			g_aDiagnosis[nCntUI].fWidth = NUMBER_W;
			g_aDiagnosis[nCntUI].pos = GPOS_010;
			g_aDiagnosis[nCntUI].tex = DIAGNOSISTEX_NUMBER;
			g_aDiagnosis[nCntUI].type = DIAGNOSISTYPE_G_COMMAND010;
			// 数字の情報を取得
			TecU = DiagnosisData.nGreen % 100 / 10;
			// テクスチャ座標の設定
			pVtx[0].tex = D3DXVECTOR2(TecU * 0.1f, 0.0f);
			pVtx[1].tex = D3DXVECTOR2(TecU * 0.1f + 0.1f, 0.0f);
			pVtx[2].tex = D3DXVECTOR2(TecU * 0.1f, 1.0f);
			pVtx[3].tex = D3DXVECTOR2(TecU * 0.1f + 0.1f, 1.0f);
			break;

		case DIAGNOSISTYPE_G_COMMAND001:	// 回数1
			g_aDiagnosis[nCntUI].fHeight = NUMBER_H;
			g_aDiagnosis[nCntUI].fWidth = NUMBER_W;
			g_aDiagnosis[nCntUI].pos = GPOS_001;
			g_aDiagnosis[nCntUI].tex = DIAGNOSISTEX_NUMBER;
			g_aDiagnosis[nCntUI].type = DIAGNOSISTYPE_G_COMMAND001;
			// 数字の情報を取得
			TecU = DiagnosisData.nGreen % 10 / 1;
			// テクスチャ座標の設定
			pVtx[0].tex = D3DXVECTOR2(TecU * 0.1f, 0.0f);
			pVtx[1].tex = D3DXVECTOR2(TecU * 0.1f + 0.1f, 0.0f);
			pVtx[2].tex = D3DXVECTOR2(TecU * 0.1f, 1.0f);
			pVtx[3].tex = D3DXVECTOR2(TecU * 0.1f + 0.1f, 1.0f);
			break;

		case DIAGNOSISTYPE_G_COMMANDTIMES:	// 回
			g_aDiagnosis[nCntUI].fHeight = TIMES_H;
			g_aDiagnosis[nCntUI].fWidth = TIMES_W;
			g_aDiagnosis[nCntUI].pos = GPOS_TIMES;
			g_aDiagnosis[nCntUI].tex = DIAGNOSISTEX_TIMES;
			g_aDiagnosis[nCntUI].type = DIAGNOSISTYPE_G_COMMANDTIMES;
			// テクスチャ座標の設定
			pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
			pVtx[1].tex = D3DXVECTOR2(1.0f, 0.0f);
			pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
			pVtx[3].tex = D3DXVECTOR2(1.0f, 1.0f);
			break;

		case DIAGNOSISTYPE_B_COMMANDTYPE:	// Bコマンドの種類
			g_aDiagnosis[nCntUI].fHeight = COMMAND_H;
			g_aDiagnosis[nCntUI].fWidth = COMMAND_W;
			g_aDiagnosis[nCntUI].pos = BLUE_POS;
			g_aDiagnosis[nCntUI].tex = DIAGNOSISTEX_B;
			g_aDiagnosis[nCntUI].type = DIAGNOSISTYPE_B_COMMANDTYPE;
			// テクスチャ座標の設定
			pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
			pVtx[1].tex = D3DXVECTOR2(1.0f, 0.0f);
			pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
			pVtx[3].tex = D3DXVECTOR2(1.0f, 1.0f);
			break;

		case DIAGNOSISTYPE_B_COMMAND100:	// 回数100
			g_aDiagnosis[nCntUI].fHeight = NUMBER_H;
			g_aDiagnosis[nCntUI].fWidth = NUMBER_W;
			g_aDiagnosis[nCntUI].pos = BPOS_100;
			g_aDiagnosis[nCntUI].tex = DIAGNOSISTEX_NUMBER;
			g_aDiagnosis[nCntUI].type = DIAGNOSISTYPE_B_COMMAND100;
			// 数字の情報を取得
			TecU = DiagnosisData.nBlue % 1000 / 100;
			// テクスチャ座標の設定
			pVtx[0].tex = D3DXVECTOR2(TecU * 0.1f, 0.0f);
			pVtx[1].tex = D3DXVECTOR2(TecU * 0.1f + 0.1f, 0.0f);
			pVtx[2].tex = D3DXVECTOR2(TecU * 0.1f, 1.0f);
			pVtx[3].tex = D3DXVECTOR2(TecU * 0.1f + 0.1f, 1.0f);
			break;

		case DIAGNOSISTYPE_B_COMMAND010:	// 回数10
			g_aDiagnosis[nCntUI].fHeight = NUMBER_H;
			g_aDiagnosis[nCntUI].fWidth = NUMBER_W;
			g_aDiagnosis[nCntUI].pos = BPOS_010;
			g_aDiagnosis[nCntUI].tex = DIAGNOSISTEX_NUMBER;
			g_aDiagnosis[nCntUI].type = DIAGNOSISTYPE_B_COMMAND010;
			// 数字の情報を取得
			TecU = DiagnosisData.nBlue % 100 / 10;
			// テクスチャ座標の設定
			pVtx[0].tex = D3DXVECTOR2(TecU * 0.1f, 0.0f);
			pVtx[1].tex = D3DXVECTOR2(TecU * 0.1f + 0.1f, 0.0f);
			pVtx[2].tex = D3DXVECTOR2(TecU * 0.1f, 1.0f);
			pVtx[3].tex = D3DXVECTOR2(TecU * 0.1f + 0.1f, 1.0f);
			break;

		case DIAGNOSISTYPE_B_COMMAND001:	// 回数1
			g_aDiagnosis[nCntUI].fHeight = NUMBER_H;
			g_aDiagnosis[nCntUI].fWidth = NUMBER_W;
			g_aDiagnosis[nCntUI].pos = BPOS_001;
			g_aDiagnosis[nCntUI].tex = DIAGNOSISTEX_NUMBER;
			g_aDiagnosis[nCntUI].type = DIAGNOSISTYPE_B_COMMAND001;
			// 数字の情報を取得
			TecU = DiagnosisData.nBlue % 10 / 1;
			// テクスチャ座標の設定
			pVtx[0].tex = D3DXVECTOR2(TecU * 0.1f, 0.0f);
			pVtx[1].tex = D3DXVECTOR2(TecU * 0.1f + 0.1f, 0.0f);
			pVtx[2].tex = D3DXVECTOR2(TecU * 0.1f, 1.0f);
			pVtx[3].tex = D3DXVECTOR2(TecU * 0.1f + 0.1f, 1.0f);
			break;

		case DIAGNOSISTYPE_B_COMMANDTIMES:	// 回
			g_aDiagnosis[nCntUI].fHeight = TIMES_H;
			g_aDiagnosis[nCntUI].fWidth = TIMES_W;
			g_aDiagnosis[nCntUI].pos = BPOS_TIMES;
			g_aDiagnosis[nCntUI].tex = DIAGNOSISTEX_TIMES;
			g_aDiagnosis[nCntUI].type = DIAGNOSISTYPE_B_COMMANDTIMES;
			// テクスチャ座標の設定
			pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
			pVtx[1].tex = D3DXVECTOR2(1.0f, 0.0f);
			pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
			pVtx[3].tex = D3DXVECTOR2(1.0f, 1.0f);
			break;

		case DIAGNOSISTYPE_Y_COMMANDTYPE:	// Yコマンドの種類
			g_aDiagnosis[nCntUI].fHeight = COMMAND_H;
			g_aDiagnosis[nCntUI].fWidth = COMMAND_W;
			g_aDiagnosis[nCntUI].pos = YELLOW_POS;
			g_aDiagnosis[nCntUI].tex = DIAGNOSISTEX_Y;
			g_aDiagnosis[nCntUI].type = DIAGNOSISTYPE_Y_COMMANDTYPE;
			// テクスチャ座標の設定
			pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
			pVtx[1].tex = D3DXVECTOR2(1.0f, 0.0f);
			pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
			pVtx[3].tex = D3DXVECTOR2(1.0f, 1.0f);
			break;

		case DIAGNOSISTYPE_Y_COMMAND100:	// 回数100
			g_aDiagnosis[nCntUI].fHeight = NUMBER_H;
			g_aDiagnosis[nCntUI].fWidth = NUMBER_W;
			g_aDiagnosis[nCntUI].pos = YPOS_100;
			g_aDiagnosis[nCntUI].tex = DIAGNOSISTEX_NUMBER;
			g_aDiagnosis[nCntUI].type = DIAGNOSISTYPE_Y_COMMAND100;
			// 数字の情報を取得
			TecU = DiagnosisData.nYellow % 1000 / 100;
			// テクスチャ座標の設定
			pVtx[0].tex = D3DXVECTOR2(TecU * 0.1f, 0.0f);
			pVtx[1].tex = D3DXVECTOR2(TecU * 0.1f + 0.1f, 0.0f);
			pVtx[2].tex = D3DXVECTOR2(TecU * 0.1f, 1.0f);
			pVtx[3].tex = D3DXVECTOR2(TecU * 0.1f + 0.1f, 1.0f);
			break;

		case DIAGNOSISTYPE_Y_COMMAND010:	// 回数10
			g_aDiagnosis[nCntUI].fHeight = NUMBER_H;
			g_aDiagnosis[nCntUI].fWidth = NUMBER_W;
			g_aDiagnosis[nCntUI].pos = YPOS_010;
			g_aDiagnosis[nCntUI].tex = DIAGNOSISTEX_NUMBER;
			g_aDiagnosis[nCntUI].type = DIAGNOSISTYPE_Y_COMMAND010;
			// 数字の情報を取得
			TecU = DiagnosisData.nYellow % 100 / 10;
			// テクスチャ座標の設定
			pVtx[0].tex = D3DXVECTOR2(TecU * 0.1f, 0.0f);
			pVtx[1].tex = D3DXVECTOR2(TecU * 0.1f + 0.1f, 0.0f);
			pVtx[2].tex = D3DXVECTOR2(TecU * 0.1f, 1.0f);
			pVtx[3].tex = D3DXVECTOR2(TecU * 0.1f + 0.1f, 1.0f);
			break;

		case DIAGNOSISTYPE_Y_COMMAND001:	// 回数1
			g_aDiagnosis[nCntUI].fHeight = NUMBER_H;
			g_aDiagnosis[nCntUI].fWidth = NUMBER_W;
			g_aDiagnosis[nCntUI].pos = YPOS_001;
			g_aDiagnosis[nCntUI].tex = DIAGNOSISTEX_NUMBER;
			g_aDiagnosis[nCntUI].type = DIAGNOSISTYPE_Y_COMMAND001;
			// 数字の情報を取得
			TecU = DiagnosisData.nYellow % 10 / 1;
			// テクスチャ座標の設定
			pVtx[0].tex = D3DXVECTOR2(TecU * 0.1f, 0.0f);
			pVtx[1].tex = D3DXVECTOR2(TecU * 0.1f + 0.1f, 0.0f);
			pVtx[2].tex = D3DXVECTOR2(TecU * 0.1f, 1.0f);
			pVtx[3].tex = D3DXVECTOR2(TecU * 0.1f + 0.1f, 1.0f);
			break;

		case DIAGNOSISTYPE_Y_COMMANDTIMES:	// 回
			g_aDiagnosis[nCntUI].fHeight = TIMES_H;
			g_aDiagnosis[nCntUI].fWidth = TIMES_W;
			g_aDiagnosis[nCntUI].pos = YPOS_TIMES;
			g_aDiagnosis[nCntUI].tex = DIAGNOSISTEX_TIMES;
			g_aDiagnosis[nCntUI].type = DIAGNOSISTYPE_Y_COMMANDTIMES;
			// テクスチャ座標の設定
			pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
			pVtx[1].tex = D3DXVECTOR2(1.0f, 0.0f);
			pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
			pVtx[3].tex = D3DXVECTOR2(1.0f, 1.0f);
			break;

		case DIAGNOSISTYPE_MOSTMAGIC:	// 使われた魔法数
			g_aDiagnosis[nCntUI].fHeight = TEXT_H;
			g_aDiagnosis[nCntUI].fWidth = TEXT_W;
			g_aDiagnosis[nCntUI].pos = MAGIC_POS;
			g_aDiagnosis[nCntUI].tex = DIAGNOSISTEX_MOSTMAGIC;
			g_aDiagnosis[nCntUI].type = DIAGNOSISTYPE_MOSTMAGIC;
			// テクスチャ座標の設定
			pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
			pVtx[1].tex = D3DXVECTOR2(1.0f, 0.0f);
			pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
			pVtx[3].tex = D3DXVECTOR2(1.0f, 1.0f);
			break;

		case DIAGNOSISTYPE_MAGICCIRCLE:	// 魔法陣
			g_aDiagnosis[nCntUI].fHeight = MAGICCIRCLE_SIZE;
			g_aDiagnosis[nCntUI].fWidth = MAGICCIRCLE_SIZE;
			g_aDiagnosis[nCntUI].pos = CIRCLE_POS;
			g_aDiagnosis[nCntUI].tex = SetMostMagic(DiagnosisData);
			g_aDiagnosis[nCntUI].type = DIAGNOSISTYPE_MAGICCIRCLE;
			// テクスチャ座標の設定
			pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
			pVtx[1].tex = D3DXVECTOR2(1.0f, 0.0f);
			pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
			pVtx[3].tex = D3DXVECTOR2(1.0f, 1.0f);
			break;

		case DIAGNOSISTYPE_HAPPENDEVENT:	// イベント
			g_aDiagnosis[nCntUI].fHeight = TEXT_H;
			g_aDiagnosis[nCntUI].fWidth = TEXT_W;
			g_aDiagnosis[nCntUI].pos = EVENT_POS;
			g_aDiagnosis[nCntUI].tex = DIAGNOSISTEX_HAPPENDEVENT;
			g_aDiagnosis[nCntUI].type = DIAGNOSISTYPE_HAPPENDEVENT;
			// テクスチャ座標の設定
			pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
			pVtx[1].tex = D3DXVECTOR2(1.0f, 0.0f);
			pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
			pVtx[3].tex = D3DXVECTOR2(1.0f, 1.0f);
			break;

		case DIAGNOSISTYPE_EVENT100:	// 回数100
			g_aDiagnosis[nCntUI].fHeight = NUMBER_H;
			g_aDiagnosis[nCntUI].fWidth = NUMBER_W;
			g_aDiagnosis[nCntUI].pos = EVENTPOS_100;
			g_aDiagnosis[nCntUI].tex = DIAGNOSISTEX_NUMBER;
			g_aDiagnosis[nCntUI].type = DIAGNOSISTYPE_EVENT100;
			// 数字の情報を取得
			TecU = DiagnosisData.nYellow % 1000 / 100;
			// テクスチャ座標の設定
			pVtx[0].tex = D3DXVECTOR2(TecU * 0.1f, 0.0f);
			pVtx[1].tex = D3DXVECTOR2(TecU * 0.1f + 0.1f, 0.0f);
			pVtx[2].tex = D3DXVECTOR2(TecU * 0.1f, 1.0f);
			pVtx[3].tex = D3DXVECTOR2(TecU * 0.1f + 0.1f, 1.0f);
			break;

		case DIAGNOSISTYPE_EVENT010:	// 回数10
			g_aDiagnosis[nCntUI].fHeight = NUMBER_H;
			g_aDiagnosis[nCntUI].fWidth = NUMBER_W;
			g_aDiagnosis[nCntUI].pos = EVENTPOS_010;
			g_aDiagnosis[nCntUI].tex = DIAGNOSISTEX_NUMBER;
			g_aDiagnosis[nCntUI].type = DIAGNOSISTYPE_EVENT010;
			// 数字の情報を取得
			TecU = DiagnosisData.nYellow % 100 / 10;
			// テクスチャ座標の設定
			pVtx[0].tex = D3DXVECTOR2(TecU * 0.1f, 0.0f);
			pVtx[1].tex = D3DXVECTOR2(TecU * 0.1f + 0.1f, 0.0f);
			pVtx[2].tex = D3DXVECTOR2(TecU * 0.1f, 1.0f);
			pVtx[3].tex = D3DXVECTOR2(TecU * 0.1f + 0.1f, 1.0f);
			break;

		case DIAGNOSISTYPE_EVENT001:	// 回数1
			g_aDiagnosis[nCntUI].fHeight = NUMBER_H;
			g_aDiagnosis[nCntUI].fWidth = NUMBER_W;
			g_aDiagnosis[nCntUI].pos = EVENTPOS_001;
			g_aDiagnosis[nCntUI].tex = DIAGNOSISTEX_NUMBER;
			g_aDiagnosis[nCntUI].type = DIAGNOSISTYPE_EVENT001;
			// 数字の情報を取得
			TecU = DiagnosisData.nYellow % 10 / 1;
			// テクスチャ座標の設定
			pVtx[0].tex = D3DXVECTOR2(TecU * 0.1f, 0.0f);
			pVtx[1].tex = D3DXVECTOR2(TecU * 0.1f + 0.1f, 0.0f);
			pVtx[2].tex = D3DXVECTOR2(TecU * 0.1f, 1.0f);
			pVtx[3].tex = D3DXVECTOR2(TecU * 0.1f + 0.1f, 1.0f);
			break;

		case DIAGNOSISTYPE_EVENTTIMES:	// 回
			g_aDiagnosis[nCntUI].fHeight = TIMES_H;
			g_aDiagnosis[nCntUI].fWidth = TIMES_W;
			g_aDiagnosis[nCntUI].pos = EVENTPOS_TIMES;
			g_aDiagnosis[nCntUI].tex = DIAGNOSISTEX_TIMES;
			g_aDiagnosis[nCntUI].type = DIAGNOSISTYPE_EVENTTIMES;
			// テクスチャ座標の設定
			pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
			pVtx[1].tex = D3DXVECTOR2(1.0f, 0.0f);
			pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
			pVtx[3].tex = D3DXVECTOR2(1.0f, 1.0f);
			break;

		default:

			break;
		}

		// 頂点座標の設定
		pVtx[0].pos = D3DXVECTOR3(g_aDiagnosis[nCntUI].pos.x - g_aDiagnosis[nCntUI].fWidth, g_aDiagnosis[nCntUI].pos.y - g_aDiagnosis[nCntUI].fHeight, 0.0f);
		pVtx[1].pos = D3DXVECTOR3(g_aDiagnosis[nCntUI].pos.x + g_aDiagnosis[nCntUI].fWidth, g_aDiagnosis[nCntUI].pos.y - g_aDiagnosis[nCntUI].fHeight, 0.0f);
		pVtx[2].pos = D3DXVECTOR3(g_aDiagnosis[nCntUI].pos.x - g_aDiagnosis[nCntUI].fWidth, g_aDiagnosis[nCntUI].pos.y + g_aDiagnosis[nCntUI].fHeight, 0.0f);
		pVtx[3].pos = D3DXVECTOR3(g_aDiagnosis[nCntUI].pos.x + g_aDiagnosis[nCntUI].fWidth, g_aDiagnosis[nCntUI].pos.y + g_aDiagnosis[nCntUI].fHeight, 0.0f);
		// 頂点カラーの設定
		pVtx[0].col = COLOR_WHITE;
		pVtx[1].col = COLOR_WHITE;
		pVtx[2].col = COLOR_WHITE;
		pVtx[3].col = COLOR_WHITE;
		// rhwの設定
		pVtx[0].rhw = 1.0f;
		pVtx[1].rhw = 1.0f;
		pVtx[2].rhw = 1.0f;
		pVtx[3].rhw = 1.0f;

		// 表示状態にする
		g_aDiagnosis[nCntUI].bDisp = true;
	}

	// 頂点バッファをアンロック
	g_pVtxBuffDiagnosisUI->Unlock();
}

//========================================================================
// 1番使われた魔法の判定
//========================================================================
DIAGNOSISTEX SetMostMagic(TotalData totalData)
{
	DIAGNOSISTEX tex;
	int nMagic = -10;	// 魔法の比較用使用回数を保存
	MAGICTYPE type;		// 最も使用された魔法の種類を保存

	// forで回せるように情報を格納
	int aMagic[MAGICTYPE_MAX];
	aMagic[0] = totalData.nLevitation;		// 使われた魔法数[浮遊]
	aMagic[1] = totalData.nCombustion;		// 使われた魔法数[燃焼]
	aMagic[2] = totalData.nFlood;			// 使われた魔法数[洪水]
	aMagic[3] = totalData.nFlash;			// 使われた魔法数[フラッシュ]
	aMagic[4] = totalData.nFireBall;		// 使われた魔法数[火球]
	aMagic[5] = totalData.nSunsetDelay;		// 使われた魔法数[時間停止]
	aMagic[6] = totalData.nRainPray;		// 使われた魔法数[雨乞い]
	aMagic[7] = totalData.nFreeze;			// 使われた魔法数[凍結]
	aMagic[8] = totalData.nGrowth;			// 使われた魔法数[成長]
	aMagic[9] = totalData.nAcceleration;	// 使われた魔法数[加速]
	aMagic[10] = totalData.nTimeRevert;		// 使われた魔法数[巻き戻し]

	for (int nCntMagic = 0; nCntMagic < MAGICTYPE_MAX; nCntMagic++)
	{
		if (aMagic[nCntMagic] > nMagic)
		{ // 保存されている値より
			nMagic = aMagic[nCntMagic];
			type = (MAGICTYPE)nCntMagic;
		}
	}

	switch (type)
	{
	case MAGICTYPE_LEVITATION:	// 浮遊
		tex = DIAGNOSISTEX_LEVITATION;
		break;

	case MAGICTYPE_COMBUSTION:	// 燃焼
		tex = DIAGNOSISTEX_COMBUSTION;
		break;

	case MAGICTYPE_FLOOD:	// 洪水
		tex = DIAGNOSISTEX_FLOOD;
		break;

	case MAGICTYPE_FLASH:	// フラッシュ
		tex = DIAGNOSISTEX_FLASH;
		break;

	case MAGICTYPE_FIREBALL:	// 火球
		tex = DIAGNOSISTEX_FIREBALL;
		break;

	case MAGICTYPE_SUNSETDELAY:	// 時間停止
		tex = DIAGNOSISTEX_SUNSETDELAY;
		break;

	case MAGICTYPE_RAINPRAY:	// 雨乞い
		tex = DIAGNOSISTEX_RAINPRAY;
		break;

	case MAGICTYPE_FREEZE:	// 凍結
		tex = DIAGNOSISTEX_FREEZE;
		break;

	case MAGICTYPE_GROWTH:	// 成長
		tex = DIAGNOSISTEX_GROWTH;
		break;

	case MAGICTYPE_ACCELERATION:	// 加速
		tex = DIAGNOSISTEX_ACCELERATION;
		break;

	case MAGICTYPE_TIMEREVERT:	// 巻き戻し
		tex = DIAGNOSISTEX_TIMEREVERT;
		break;
	}

	return tex;
}
