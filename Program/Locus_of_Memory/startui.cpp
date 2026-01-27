#if 0

#endif // 0

//======================================================================================
// 
// タイトルの2DUI処理[startui.h]
//
//======================================================================================
#include "main.h"
#include "fade.h"
#include "startui.h"
#include "start.h"
#include "input.h"

// タイトルUIの種類
typedef enum
{
	StartUITYPE_LOGO = 0,	// タイトルロゴ
	StartUITYPE_1PPLAY,		// 1PPALY
	StartUITYPE_2PPLAY,		// 2PPALY
	StartUITYPE_KEYBOARD,	// KEYBOARD
	StartUITYPE_MAX
}STARTUITYPE;

// タイトルUI演出の管理
typedef enum
{
	StartTEXT_MODE_NONE = 0,	// 通常
	StartTEXT_MODE_WIDTH,		// 横長
	StartTEXT_MODE_HEIGHT,		// 縦長
	StartTEXT_MODE_BLINKING,	// 点滅
	StartTEXT_MODE_BLINKING1,	// 高速点滅
	StartTEXT_MODE_MAX
}STARTTEXT_MODE;

// タイトルUIの状態
typedef enum
{
	StartTEXTSTATE_APPEAR = 0,	// ロゴ出現状態
	StartTEXTSTATE_NORMAL,		// 通常状態
	StartTEXTSTATE_START,		// チュートリアルへの遷移待機
	StartTEXTSTATE_MAX
}STARTTEXTSTATE;

// タイトルUIの構造体
typedef struct
{
	STARTUITYPE		type;	// 種類
	STARTTEXT_MODE	mode;	// 演出
	D3DXVECTOR3		pos;	// 位置
	int nCounterState;		// 拡縮を管理
	float	fWidth;			// 幅
	float	fHeight;		// 高さ
	bool	bDisp;			// 表示状態
}StartText;

// マクロ定義
#define NUM_StartUI			(StartUITYPE_MAX)	// タイトルUIのレイヤー数
#define SELECT_NUM			(3)			// 操作方法選択数
#define LOGO_POS			(D3DXVECTOR3(350.0f, 200.0f, 0.0f))	// ロゴの位置
#define LOGO_WIDTH			(300.0f)	// ロゴの横幅
#define LOGO_HEIGHT			(150.0f)	// ロゴの縦幅
#define SELECT_POS			(D3DXVECTOR3(1000.0f, 400.0f, 0.0f))	// セレクトボタンの開始位置
#define SELECT_WIDTH		(150.0f)	// セレクトボタンの横幅
#define SELECT_HEIGHT		(75.0f)		// セレクトボタンの縦幅
#define SELECT_MARGIN		(80.0f)		// セレクトボタンの余白

// テクスチャの読み込み
const char* c_apFilenameStartUI[StartUITYPE_MAX] =
{
	"data\\TEXTURE\\start_000.png",	// [StartTEXT_TYPE_BG]
	"data\\TEXTURE\\start_001.png",	// [StartTEXT_TYPE_ENTER]
	"data\\TEXTURE\\start_001.png",	// [StartTEXT_TYPE_ENTER]
	"data\\TEXTURE\\start_001.png",	// [StartTEXT_TYPE_ENTER]
};

// グローバル変数
LPDIRECT3DTEXTURE9 g_apTextureStartUI[NUM_StartUI] = {};	// テクスチャへのポインタ
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffStartUI = NULL; // 頂点バッファへのポインタ
StartText g_aStartUI[NUM_StartUI];	// 構造体

//========================================================================
// タイトルUIの初期化処理
//========================================================================
void InitStartUI(void)
{
	LPDIRECT3DDEVICE9 pDevice;	// デバイスへのポインタ
	// デバイスの取得
	pDevice = GetDevice();

	// テクスチャの読み込み
	for (int nCntUI = 0; nCntUI < NUM_StartUI; nCntUI++)
	{
		D3DXCreateTextureFromFile(pDevice, c_apFilenameStartUI[nCntUI], &g_apTextureStartUI[nCntUI]);
	}

	// 頂点バッファの生成
	pDevice->CreateVertexBuffer(sizeof(VERTEX_2D) * 4 * NUM_StartUI, D3DUSAGE_WRITEONLY, FVF_VERTEX_2D, D3DPOOL_MANAGED, &g_pVtxBuffStartUI, NULL);

	VERTEX_2D* pVtx;
	// 頂点バッファをロックし、頂点情報へのポインタを取得
	g_pVtxBuffStartUI->Lock(0, 0, (void**)&pVtx, 0);

	for (int nCntStart = 0; nCntStart < NUM_StartUI; nCntStart++, pVtx += 4)
	{
		switch (nCntStart)
		{
		case StartUITYPE_LOGO:	// タイトルロゴ
			// 頂点座標の設定
			pVtx[0].pos = D3DXVECTOR3(LOGO_POS.x - LOGO_WIDTH, LOGO_POS.y - LOGO_HEIGHT, 0.0f);
			pVtx[1].pos = D3DXVECTOR3(LOGO_POS.x + LOGO_WIDTH, LOGO_POS.y - LOGO_HEIGHT, 0.0f);
			pVtx[2].pos = D3DXVECTOR3(LOGO_POS.x - LOGO_WIDTH, LOGO_POS.y + LOGO_HEIGHT, 0.0f);
			pVtx[3].pos = D3DXVECTOR3(LOGO_POS.x + LOGO_WIDTH, LOGO_POS.y + LOGO_HEIGHT, 0.0f);
			// 頂点カラーの設定
			pVtx[0].col = D3DXCOLOR(1.0f, 1.0f, 0.0f, 1.0f);
			pVtx[1].col = D3DXCOLOR(1.0f, 1.0f, 0.0f, 1.0f);
			pVtx[2].col = D3DXCOLOR(1.0f, 1.0f, 0.0f, 1.0f);
			pVtx[3].col = D3DXCOLOR(1.0f, 1.0f, 0.0f, 1.0f);
			break;

		case StartUITYPE_1PPLAY:	// 1PPLAY
		// 頂点座標の設定
			pVtx[0].pos = D3DXVECTOR3(SELECT_POS.x - SELECT_WIDTH, SELECT_POS.y + SELECT_HEIGHT + (nCntStart - 1) * SELECT_MARGIN, 0.0f);
			pVtx[1].pos = D3DXVECTOR3(SELECT_POS.x + SELECT_WIDTH, SELECT_POS.y + SELECT_HEIGHT + (nCntStart - 1) * SELECT_MARGIN, 0.0f);
			pVtx[2].pos = D3DXVECTOR3(SELECT_POS.x - SELECT_WIDTH, SELECT_POS.y + SELECT_HEIGHT * 2 + (nCntStart - 1) * SELECT_MARGIN, 0.0f);
			pVtx[3].pos = D3DXVECTOR3(SELECT_POS.x + SELECT_WIDTH, SELECT_POS.y + SELECT_HEIGHT * 2 + (nCntStart - 1) * SELECT_MARGIN, 0.0f);
			// 頂点カラーの設定
			pVtx[0].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
			pVtx[1].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
			pVtx[2].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
			pVtx[3].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
			break;

		case StartUITYPE_2PPLAY:	// 2PPLAY
		// 頂点座標の設定
			pVtx[0].pos = D3DXVECTOR3(SELECT_POS.x - SELECT_WIDTH, SELECT_POS.y + SELECT_HEIGHT + (nCntStart - 1) * SELECT_MARGIN, 0.0f);
			pVtx[1].pos = D3DXVECTOR3(SELECT_POS.x + SELECT_WIDTH, SELECT_POS.y + SELECT_HEIGHT + (nCntStart - 1) * SELECT_MARGIN, 0.0f);
			pVtx[2].pos = D3DXVECTOR3(SELECT_POS.x - SELECT_WIDTH, SELECT_POS.y + SELECT_HEIGHT * 2 + (nCntStart - 1) * SELECT_MARGIN, 0.0f);
			pVtx[3].pos = D3DXVECTOR3(SELECT_POS.x + SELECT_WIDTH, SELECT_POS.y + SELECT_HEIGHT * 2 + (nCntStart - 1) * SELECT_MARGIN, 0.0f);
			// 頂点カラーの設定
			pVtx[0].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.5f);
			pVtx[1].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.5f);
			pVtx[2].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.5f);
			pVtx[3].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.5f);
			break;

		case StartUITYPE_KEYBOARD:	// KEYBOARD
			// 頂点座標の設定
			pVtx[0].pos = D3DXVECTOR3(SELECT_POS.x - SELECT_WIDTH, SELECT_POS.y + SELECT_HEIGHT + (nCntStart - 1) * SELECT_MARGIN, 0.0f);
			pVtx[1].pos = D3DXVECTOR3(SELECT_POS.x + SELECT_WIDTH, SELECT_POS.y + SELECT_HEIGHT + (nCntStart - 1) * SELECT_MARGIN, 0.0f);
			pVtx[2].pos = D3DXVECTOR3(SELECT_POS.x - SELECT_WIDTH, SELECT_POS.y + SELECT_HEIGHT * 2 + (nCntStart - 1) * SELECT_MARGIN, 0.0f);
			pVtx[3].pos = D3DXVECTOR3(SELECT_POS.x + SELECT_WIDTH, SELECT_POS.y + SELECT_HEIGHT * 2 + (nCntStart - 1) * SELECT_MARGIN, 0.0f);
			// 頂点カラーの設定
			pVtx[0].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.5f);
			pVtx[1].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.5f);
			pVtx[2].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.5f);
			pVtx[3].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.5f);
			break;
		}

		// rhwの設定
		pVtx[0].rhw = 1.0f;
		pVtx[1].rhw = 1.0f;
		pVtx[2].rhw = 1.0f;
		pVtx[3].rhw = 1.0f;

		// テクスチャ座標の設定
		pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
		pVtx[1].tex = D3DXVECTOR2(1.0f, 0.0f);
		pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
		pVtx[3].tex = D3DXVECTOR2(1.0f, 1.0f);

		g_aStartUI[nCntStart].bDisp = true;
	}
	// 頂点バッファをアンロック
	g_pVtxBuffStartUI->Unlock();
}

//========================================================================
// タイトルUIの終了処理
//========================================================================
void UninitStartUI(void)
{
	// テクスチャの破棄
	for (int nCntStart = 0; nCntStart < NUM_StartUI; nCntStart++)
	{
		if (g_apTextureStartUI[nCntStart] != NULL)
		{
			g_apTextureStartUI[nCntStart]->Release();
			g_apTextureStartUI[nCntStart] = NULL;
		}
	}

	// 頂点バッファの破棄
	if (g_pVtxBuffStartUI != NULL)
	{
		g_pVtxBuffStartUI->Release();
		g_pVtxBuffStartUI = NULL;
	}
}

//========================================================================
// タイトルUIの更新処理
//========================================================================
void UpdateStartUI(void)
{
	//// 現在のフェードの状態を管理
	//FADE *pfade = GetFade();

	//VERTEX_2D* pVtx;
	//// 頂点バッファをロックし、頂点情報へのポインタを取得
	//g_pVtxBuffStartUI->Lock(0, 0, (void**)&pVtx, 0);

	//pVtx += 4;	// ロゴの分だけポインタを進める

	//for (int nCntUI = 0; nCntUI < SELECT_NUM; nCntUI++, pVtx += 4)
	//{
	//	if (nCntUI + 1 == g_nSelectOperation)	// 選択部だけ明るく表示
	//	{
	//		pVtx[0].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
	//		pVtx[1].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
	//		pVtx[2].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
	//		pVtx[3].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
	//	}
	//	else
	//	{
	//		pVtx[0].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.5f);
	//		pVtx[1].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.5f);
	//		pVtx[2].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.5f);
	//		pVtx[3].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.5f);
	//	}
	//}
	//
	//// 頂点バッファをアンロック
	//g_pVtxBuffStartUI->Unlock();
}

//========================================================================
// タイトルUIの描画処理
//========================================================================
void DrawStartUI(void)
{
	LPDIRECT3DDEVICE9 pDevice;	// デバイスへのポインタ
	// デバイスの取得
	pDevice = GetDevice();

	// 頂点バッファをデータストリームに設定
	pDevice->SetStreamSource(0, g_pVtxBuffStartUI, 0, sizeof(VERTEX_2D));

	// 頂点フォーマットの設定
	pDevice->SetFVF(FVF_VERTEX_2D);

	for (int nCntStart = 0; nCntStart < NUM_StartUI; nCntStart++)
	{
		if (g_aStartUI[nCntStart].bDisp == true)
		{
			// テクスチャの設定
			pDevice->SetTexture(0, g_apTextureStartUI[nCntStart]);
			// ポリゴンの描画
			pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, nCntStart * 4, 2);
		}
	}
}
