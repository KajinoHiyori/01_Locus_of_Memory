//======================================================================================
// 
// ゲーム画面でのUI表示処理[gameui.cpp]
// Author : KajinoHiyori
//
//======================================================================================
#include "main.h"
#include "gameui.h"
#include "fade.h"
#include "game.h"
#include "input.h"
#include "title.h"

// マクロ定義
#define MAX_GAMEUI		(2)					// UIの最大数
#define NUM_GAMEUI		(GAMEUI_TYPE_MAX)	// ポーズメニューで使うテクスチャ数
#define NUM_SELECT		(5)					// 選択数
#define WIDTH			(135.0f)			// UIの基本幅
#define LEFT_POSX		(1100.0f)			// 左のUIのX軸
#define LEFTPHONE_POS	(D3DXVECTOR3(LEFT_POSX, 360.0f, 0.0f))	// 左のスマホの位置
#define PHONE_HEIGHT	(285.0f)			// スマホの縦幅
#define PAUSE_POS		(D3DXVECTOR3(LEFT_POSX, 100.0f, 0.0f))	// PAUSEタイトルの位置
#define PAUSE_HEIGHT	(25.0f)				// PAUSEタイトルの縦幅


#define COLOR_WHITE		D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f)	// 白
#define COLOR_CYAN		D3DXCOLOR(0.0f, 1.0f, 1.0f, 1.0f)	// シアン

// ゲームUIの構造体
typedef struct
{
	GAMEUI_TYPE type;		// テクスチャの種類
	D3DXVECTOR3 pos;		// 中心座標
	D3DXVECTOR3 posDest;	// 目的の向き
	D3DXCOLOR	col;		// 色
	float fWidth;			// 幅
	float fHeight;			// 高さ
	bool bDisp;				// 表示状態
}GameUI;

// グローバル変数
LPDIRECT3DTEXTURE9	g_apTextureGameUI[NUM_GAMEUI] = {};	// テクスチャへのポインタ
LPDIRECT3DVERTEXBUFFER9	g_pVtxBuffGameUI = NULL;		// 頂点バッファへのポインタ
GameUI g_GameUI[MAX_GAMEUI][NUM_GAMEUI];			// UIの表示処理
int g_nOperationType;									// UI表示数の管理

const char* c_apFilenameGameUI[NUM_GAMEUI] =
{
	"data\\TEXTURE\\pause000.png",
	"data\\TEXTURE\\pause001.png",
	"data\\TEXTURE\\pause002.png",
	"data\\TEXTURE\\pause100.png",
	"data\\TEXTURE\\pause101.png",
	"data\\TEXTURE\\pause101.png",
	"data\\TEXTURE\\pause101.png",
};

//======================================================================================
// ポーズの初期化処理
//======================================================================================
void InitGameUI(void)
{
	LPDIRECT3DDEVICE9 pDevice;	// デバイスへのポインタ
	// デバイスの取得
	pDevice = GetDevice();

	// テクスチャの読み込み
	for (int nCntGameUI = 0; nCntGameUI < NUM_GAMEUI; nCntGameUI++)
	{
		D3DXCreateTextureFromFile(pDevice, c_apFilenameGameUI[nCntGameUI], &g_apTextureGameUI[nCntGameUI]);
	}

	// 操作方法の状態を取得
	OPERATIONTYPE operationType = GetOperationType();

	// 頂点バッファの生成
	pDevice->CreateVertexBuffer(sizeof(VERTEX_2D) * 4 * NUM_GAMEUI * MAX_GAMEUI, D3DUSAGE_WRITEONLY, FVF_VERTEX_2D, D3DPOOL_MANAGED, &g_pVtxBuffGameUI, NULL);
	VERTEX_2D* pVtx;
	// 頂点バッファをロックし、頂点情報へのポインタを取得
	g_pVtxBuffGameUI->Lock(0, 0, (void**)&pVtx, 0);

	// 左のUIを配置
	for (int nCntUI = 0; nCntUI < NUM_GAMEUI; nCntUI++, pVtx += 4)
	{
		switch (nCntUI)
		{
		case GAMEUI_TYPE_CLOCK:	// 時計
			g_GameUI[0][nCntUI].type	= GAMEUI_TYPE_CLOCK;
			g_GameUI[0][nCntUI].pos		= LEFTPHONE_POS;
			g_GameUI[0][nCntUI].posDest = LEFTPHONE_POS;
			g_GameUI[0][nCntUI].col		= COLOR_CYAN;
			g_GameUI[0][nCntUI].fWidth	= WIDTH;
			g_GameUI[0][nCntUI].fHeight = PHONE_HEIGHT;
			g_GameUI[0][nCntUI].bDisp	= true;
			break;

		case GAMEUI_TYPE_MAGICBOOK:	// 魔導書
			g_GameUI[0][nCntUI].type = GAMEUI_TYPE_MAGICBOOK;
			g_GameUI[0][nCntUI].pos = LEFTPHONE_POS;
			g_GameUI[0][nCntUI].fWidth = WIDTH;
			g_GameUI[0][nCntUI].fHeight = PHONE_HEIGHT;
			g_GameUI[0][nCntUI].bDisp = true;
			break;

		case GAMEUI_TYPE_CONTINUE:	// CONTINUE
			g_GameUI[0][nCntUI].type = GAMEUI_TYPE_CONTINUE;
			g_GameUI[0][nCntUI].pos = LEFTPHONE_POS;
			g_GameUI[0][nCntUI].fWidth = WIDTH;
			g_GameUI[0][nCntUI].fHeight = PHONE_HEIGHT;
			g_GameUI[0][nCntUI].bDisp = true;
			break;

		case GAMEUI_TYPE_RETRY:	// RETRY
			g_GameUI[0][nCntUI].type = GAMEUI_TYPE_RETRY;
			g_GameUI[0][nCntUI].pos = LEFTPHONE_POS;
			g_GameUI[0][nCntUI].fWidth = WIDTH;
			g_GameUI[0][nCntUI].fHeight = PHONE_HEIGHT;
			g_GameUI[0][nCntUI].bDisp = true;
			break;

		case GAMEUI_TYPE_QUIT:	// QUIT
			g_GameUI[0][nCntUI].type = GAMEUI_TYPE_QUIT;
			g_GameUI[0][nCntUI].pos = LEFTPHONE_POS;
			g_GameUI[0][nCntUI].fWidth = WIDTH;
			g_GameUI[0][nCntUI].fHeight = PHONE_HEIGHT;
			g_GameUI[0][nCntUI].bDisp = true;
			break;

		case GAMEUI_TYPE_PAUSE:	// pauseタイトル
			g_GameUI[0][nCntUI].type	= GAMEUI_TYPE_PAUSE;
			g_GameUI[0][nCntUI].pos		= PAUSE_POS;
			g_GameUI[0][nCntUI].posDest = PAUSE_POS;
			g_GameUI[0][nCntUI].col		= COLOR_CYAN;
			g_GameUI[0][nCntUI].fWidth	= WIDTH;
			g_GameUI[0][nCntUI].fHeight = PAUSE_HEIGHT;
			g_GameUI[0][nCntUI].bDisp	= true;
			break;

		case GAMEUI_TYPE_PHONE:	// スマホ
			g_GameUI[0][nCntUI].type	= GAMEUI_TYPE_PHONE;
			g_GameUI[0][nCntUI].pos		= LEFTPHONE_POS;
			g_GameUI[0][nCntUI].posDest = PAUSE_POS;
			g_GameUI[0][nCntUI].col		= COLOR_CYAN;
			g_GameUI[0][nCntUI].fWidth	= WIDTH;
			g_GameUI[0][nCntUI].fHeight	= PHONE_HEIGHT;
			g_GameUI[0][nCntUI].bDisp	= true;
			break;
		}




	}

	// 頂点バッファをアンロック
	g_pVtxBuffGameUI->Unlock();
#if 0
	

	OPERATIONTYPE operationType = GetOperationType();	// 操作方法の取得
	switch (operationType)
	{
	case OPERATIONTYPE_2P:	// 2Pプレイ
		g_nOperationType = 1;
		break;

	default:	// 1P/LEYBOARDプレイ
		g_nOperationType = 0;
		break;
	}

	

	// 頂点バッファの生成
	pDevice->CreateVertexBuffer(sizeof(VERTEX_2D) * 4 * NUM_GAMEUI * MAX_GAMEUI, D3DUSAGE_WRITEONLY, FVF_VERTEX_2D, D3DPOOL_MANAGED, &g_pVtxBuffGameUI, NULL);
	VERTEX_2D* pVtx;
	// 頂点バッファをロックし、頂点情報へのポインタを取得
	g_pVtxBuffGameUI->Lock(0, 0, (void**)&pVtx, 0);
	switch (g_nOperationType)
	{
	case 1:	// 2PPLAY
		for (int nCntOP = 0; nCntOP < MAX_GAMEUI; nCntOP++)
		{
			for (int nCntUI = 0; nCntUI < NUM_GAMEUI; nCntUI++, pVtx += 4)
			{
				switch (nCntUI)
				{
				case GAMEUI_TYPE_PHONE:	// スマホ
					g_GameUI[nCntOP][nCntUI].type = GAMEUI_TYPE_PHONE;
					g_GameUI[nCntOP][nCntUI].pos = LEFTPHONE_POS;
					g_GameUI[nCntOP][nCntUI].fWidth = PHONE_WIDTH;
					g_GameUI[nCntOP][nCntUI].fHeight = PHONE_HEIGHT;
					g_GameUI[nCntOP][nCntUI].bDisp = true;
					break;

				case GAMEUI_TYPE_1PALPHA:	// 背景暗転

					break;
				}

				//GAMEUI_TYPE_CLOCK = 0,	// 時計
				//GAMEUI_TYPE_MAGICBOOK,	// 魔導書
				//GAMEUI_TYPE_CONTINUE,	// CONTINUE
				//GAMEUI_TYPE_RETRY,		// RETRY
				//GAMEUI_TYPE_QUIT,		// QUIT
				//GAMEUI_TYPE_PAUSE,		// PAUSEタイトル
				//GAMEUI_TYPE_PHONE,		// スマホ
				//GAMEUI_TYPE_1PALPHA,		// ポーズ背景を暗くする[1P]
				//GAMEUI_TYPE_2PALPHA,		// ポーズ背景を暗くする[2P]

				// 頂点座標の設定
				pVtx[0].pos = D3DXVECTOR3(g_gameUI[nCntOPTYPE][nCntUI].pos.x - g_gameUI[nCntOPTYPE][nCntUI].fWidth, g_gameUI[nCntOPTYPE][nCntUI].pos.y - g_gameUI[nCntOPTYPE][nCntUI].fHeight, g_gameUI[nCntOPTYPE][nCntUI].pos.z);
				pVtx[1].pos = D3DXVECTOR3(g_gameUI[nCntOPTYPE][nCntUI].pos.x + g_gameUI[nCntOPTYPE][nCntUI].fWidth, g_gameUI[nCntOPTYPE][nCntUI].pos.y - g_gameUI[nCntOPTYPE][nCntUI].fHeight, g_gameUI[nCntOPTYPE][nCntUI].pos.z);
				pVtx[2].pos = D3DXVECTOR3(g_gameUI[nCntOPTYPE][nCntUI].pos.x - g_gameUI[nCntOPTYPE][nCntUI].fWidth, g_gameUI[nCntOPTYPE][nCntUI].pos.y + g_gameUI[nCntOPTYPE][nCntUI].fHeight, g_gameUI[nCntOPTYPE][nCntUI].pos.z);
				pVtx[3].pos = D3DXVECTOR3(g_gameUI[nCntOPTYPE][nCntUI].pos.x + g_gameUI[nCntOPTYPE][nCntUI].fWidth, g_gameUI[nCntOPTYPE][nCntUI].pos.y + g_gameUI[nCntOPTYPE][nCntUI].fHeight, g_gameUI[nCntOPTYPE][nCntUI].pos.z);

				// rhwの設定
				pVtx[0].rhw = 1.0f;
				pVtx[1].rhw = 1.0f;
				pVtx[2].rhw = 1.0f;
				pVtx[3].rhw = 1.0f;

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
			}
		}

	
	}
	// 頂点バッファをアンロック
	g_pVtxBuffGameUI->Unlock();

	VERTEX_2D* pVtx;
	// 頂点バッファをロックし、頂点情報へのポインタを取得
	g_pVtxBuffGameUI->Lock(0, 0, (void**)&pVtx, 0);

	for (int nCntOPTYPE = 0; nCntOPTYPE < MAX_GAMEUI; nCntOPTYPE++)
	{
		for (int nCntUI = 0; nCntUI < NUM_GAMEUI; nCntUI++, pVtx += 4)
		{
			switch (nCntUI)
			{
			case GAMEUI_TYPE_PHONE:	// スマホ
				g_gameUI[nCntOPTYPE][nCntUI].type = GAMEUI_TYPE_PHONE;
				g_gameUI[nCntOPTYPE][nCntUI].pos = LEFTPHONE_POS;
				g_gameUI[nCntOPTYPE][nCntUI].fWidth = PHONE_WIDTH;
				g_gameUI[nCntOPTYPE][nCntUI].fHeight = PHONE_HEIGHT;
				g_gameUI[nCntOPTYPE][nCntUI].bDisp = true;
				break;

			case GAMEUI_TYPE_1PALPHA:	// 背景暗転
				
				break;
			}

			//GAMEUI_TYPE_CLOCK = 0,	// 時計
			//GAMEUI_TYPE_MAGICBOOK,	// 魔導書
			//GAMEUI_TYPE_CONTINUE,	// CONTINUE
			//GAMEUI_TYPE_RETRY,		// RETRY
			//GAMEUI_TYPE_QUIT,		// QUIT
			//GAMEUI_TYPE_PAUSE,		// PAUSEタイトル
			//GAMEUI_TYPE_PHONE,		// スマホ
			//GAMEUI_TYPE_1PALPHA,		// ポーズ背景を暗くする[1P]
			//GAMEUI_TYPE_2PALPHA,		// ポーズ背景を暗くする[2P]

			// 頂点座標の設定
			pVtx[0].pos = D3DXVECTOR3(g_gameUI[nCntOPTYPE][nCntUI].pos.x - g_gameUI[nCntOPTYPE][nCntUI].fWidth, g_gameUI[nCntOPTYPE][nCntUI].pos.y - g_gameUI[nCntOPTYPE][nCntUI].fHeight, g_gameUI[nCntOPTYPE][nCntUI].pos.z);
			pVtx[1].pos = D3DXVECTOR3(g_gameUI[nCntOPTYPE][nCntUI].pos.x + g_gameUI[nCntOPTYPE][nCntUI].fWidth, g_gameUI[nCntOPTYPE][nCntUI].pos.y - g_gameUI[nCntOPTYPE][nCntUI].fHeight, g_gameUI[nCntOPTYPE][nCntUI].pos.z);
			pVtx[2].pos = D3DXVECTOR3(g_gameUI[nCntOPTYPE][nCntUI].pos.x - g_gameUI[nCntOPTYPE][nCntUI].fWidth, g_gameUI[nCntOPTYPE][nCntUI].pos.y + g_gameUI[nCntOPTYPE][nCntUI].fHeight, g_gameUI[nCntOPTYPE][nCntUI].pos.z);
			pVtx[3].pos = D3DXVECTOR3(g_gameUI[nCntOPTYPE][nCntUI].pos.x + g_gameUI[nCntOPTYPE][nCntUI].fWidth, g_gameUI[nCntOPTYPE][nCntUI].pos.y + g_gameUI[nCntOPTYPE][nCntUI].fHeight, g_gameUI[nCntOPTYPE][nCntUI].pos.z);

			// rhwの設定
			pVtx[0].rhw = 1.0f;
			pVtx[1].rhw = 1.0f;
			pVtx[2].rhw = 1.0f;
			pVtx[3].rhw = 1.0f;

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
		}
	}
	// 頂点バッファをアンロック
	g_pVtxBuffGameUI->Unlock();
#endif
}

//======================================================================================
// ポーズの終了処理
//======================================================================================
void UninitGameUI(void)
{
#if 0
	// テクスチャの破棄
	for (int nCntGameUI = 0; nCntGameUI < NUM_GAMEUI; nCntGameUI++)
	{
		if (g_apTextureGameUI[nCntGameUI] != NULL)
		{
			g_apTextureGameUI[nCntGameUI]->Release();
			g_apTextureGameUI[nCntGameUI] = NULL;
		}
	}

	// 頂点バッファの破棄
	if (g_pVtxBuffGameUI != NULL)
	{
		g_pVtxBuffGameUI->Release();
		g_pVtxBuffGameUI = NULL;
	}
#endif
}

//======================================================================================
// ポーズの更新処理
//======================================================================================
void UpdateGameUI(void)
{
#if 0
	// 入力情報の保存
	static int nCounterUp = 0;
	static int nCounterDown = 0;

	// 同時押しされた場合、処理を行わない
	if ((GetKeyboardPress(DIK_W) == true && GetKeyboardPress(DIK_S) == true) ||
		(GetJoypadPress(JOYKEY_UP, 0) == true && GetJoypadPress(JOYKEY_DOWN, 0) == true))
	{
		return;
	}

	// WSキーが押された場合、テクスチャの状態を変更する(リピート)
	else if (GetKeyboardRepeat(DIK_W) == true || GetJoypadRepeat(JOYKEY_UP, 0) == true || GetJoypadStickRepeatL(JOYSTICK_UP, 0) == true)	// 上
	{
		g_nSelectGameUI[nCntOP]--;
		if (g_selectGameUI < 0)
		{
			g_selectGameUI = GAMEUI_TYPE_QUIT;
		}
	}
	else if (GetKeyboardRepeat(DIK_S) == true || GetJoypadRepeat(JOYKEY_DOWN, 0) == true || GetJoypadStickRepeatL(JOYSTICK_DOWN, 0) == true)	// 下
	{
		g_selectGameUI++;
		if (g_selectGameUI > GAMEUI_TYPE_QUIT)
		{
			g_selectGameUI = GAMEUI_TYPE_CLOCK;
		}
	}

	// 何が選ばれているかの判定
	switch (g_selectGameUI)
	{
	case GAMEUI_TYPE_CLOCK:	// 時計
		g_pauseMenu = GAMEUI_TYPE_CONTINUE;
		break;

	case GAMEUI_TYPE_MAGICBOOK:	// 魔導書
		g_pauseMenu = GAMEUI_TYPE_MAGICBOOK;
		break;

	case GAMEUI_TYPE_CONTINUE:	// CONTINUE
		g_pauseMenu = GAMEUI_TYPE_CONTINUE;
		break;

	case GAMEUI_TYPE_RETRY:		// RETRY
		g_pauseMenu = GAMEUI_TYPE_RETRY;
		break;
	
	case GAMEUI_TYPE_QUIT:		// QUIT
		g_pauseMenu = GAMEUI_TYPE_QUIT;
		break;
	}

	// 頂点座標の更新
	VERTEX_2D* pVtx;
	// 頂点バッファをロックし、頂点情報へのポインタを取得
	g_pVtxBuffGameUI->Lock(0, 0, (void**)&pVtx, 0);
	for (int nCntGameUI = 0; nCntGameUI < NUM_SELECT; nCntGameUI++)
	{
		if (nCntGameUI == g_pauseMenu)	// 選択部だけ明るく表示
		{
			pVtx[0].col = D3DXCOLOR(1.0f, 1.0f, 0.0f, 1.0f);
			pVtx[1].col = D3DXCOLOR(1.0f, 1.0f, 0.0f, 1.0f);
			pVtx[2].col = D3DXCOLOR(1.0f, 1.0f, 0.0f, 1.0f);
			pVtx[3].col = D3DXCOLOR(1.0f, 1.0f, 0.0f, 1.0f);
		}
		else
		{
			pVtx[0].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.5f);
			pVtx[1].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.5f);
			pVtx[2].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.5f);
			pVtx[3].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.5f);
		}
		pVtx += 4;
	}
	// アンロック
	g_pVtxBuffGameUI->Unlock();

	// ENTERキーで確定した場合
	if (GetKeyboardTrigger(DIK_RETURN) == true || GetJoypadTrigger(JOYKEY_A, 0) == true)
	{
		switch (g_pauseMenu)
		{
		case GAMEUI_TYPE_CONTINUE:	// コンテニュー

			break;
		case GAMEUI_TYPE_RETRY:		// リトライ
			SetFade(MODE_GAME);
			break;
		case GAMEUI_TYPE_QUIT:		// タイトルへ戻る
			SetFade(MODE_TITLE);
			break;
		}
	}
#endif
}

//======================================================================================
// ポーズの描画処理
//======================================================================================
void DrawGameUI(void)
{
	LPDIRECT3DDEVICE9 pDevice;	// デバイスへのポインタ
	// デバイスの取得
	pDevice = GetDevice();

	// 頂点バッファをデータストリームに設定
	pDevice->SetStreamSource(0, g_pVtxBuffGameUI, 0, sizeof(VERTEX_2D));

	// 頂点フォーマットの設定
	pDevice->SetFVF(FVF_VERTEX_2D);

	// テクスチャの設定
	pDevice->SetTexture(0, g_apTextureGameUI[GAMEUI_TYPE_PHONE]);

	// ポリゴンの描画
	pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, GAMEUI_TYPE_PHONE * 4, 2);

	// テクスチャの設定
	pDevice->SetTexture(0, g_apTextureGameUI[GAMEUI_TYPE_PAUSE]);

	// ポリゴンの描画
	pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, GAMEUI_TYPE_PAUSE * 4, 2);

	for (int nCntGameUI = 0; nCntGameUI < NUM_SELECT; nCntGameUI++)
	{
		// テクスチャの設定
		pDevice->SetTexture(0, g_apTextureGameUI[nCntGameUI]);

		// ポリゴンの描画
		pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, nCntGameUI * 4, 2);
	}
}
