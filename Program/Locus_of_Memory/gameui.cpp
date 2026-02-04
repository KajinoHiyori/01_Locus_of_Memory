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
#include "fog.h"
#include "player.h"
#include "clock.h"
#include "magicui.h"
#include "spellui.h"
#include "menubg.h"

// マクロ定義
#define MAX_GAMEUI		(2)					// UIの最大数
#define NUM_SELECT		(5)					// 選択数
#define FRAME			(20)				// 出現/退出を管理するフレーム数
#define NUM_GAMEUI		(GAMEUI_TYPE_MAX)	// ポーズメニューで使うテクスチャ数
#define WIDTH			(135.0f)			// UIの基本幅
#define LEFT_POSX		(1100.0f)			// 左のUIのX軸
#define LEFTPHONE_POS	(D3DXVECTOR3(LEFT_POSX, 360.0f, 0.0f))	// 左のスマホの位置
#define PHONE_HEIGHT	(285.0f)			// スマホの縦幅
#define PAUSE_POS		(D3DXVECTOR3(LEFT_POSX, 100.0f, 0.0f))	// PAUSEタイトルの位置
#define PAUSE_HEIGHT	(25.0f)			// PAUSEタイトルの縦幅
#define PHONE_WIDTH		(108.0f)		// スマホの幅
#define PHONE_HEIGHT	(228.0f)		// スマホの高さ
#define GAMEUI_POSY		(482.0f)		// 左のUIのY軸
#define PAUSE_WIDTH		(PHONE_WIDTH)	// spellメニューの幅
#define PAUSE_HEIGHT	(24.0f)			// spellメニューの高さ
#define MENU_HEIGHT		(20.0f)			// メニューの高さ
#define PAUSE_Y			(-170.0f)		// spellメニューの高度
#define CLOCK_Y			(-100.0f)		// 時計の高度
#define MAGICBOOK_Y		(-30.0f)		// 魔導書の高度
#define CONTINUE_Y		(40.0f)			// CONTINUEの高度
#define RETRY_Y			(110.0f)		// RETRYの高度
#define QUIT_Y			(180.0f)		// QUITの高度
#define LEFT_OUTPOS		(D3DXVECTOR3(-PHONE_WIDTH, GAMEUI_POSY, 0.0f))					// offscreenの左のUI座標
#define RIGHT_OUTPOS	(D3DXVECTOR3(SCREEN_WIDTH + PHONE_WIDTH, GAMEUI_POSY, 0.0f))	// offscreenの右のUI座標
#define LEFT_POS		(D3DXVECTOR3(120.0f, GAMEUI_POSY, 0.0f))						// onscreenの左のUI座標
#define RIGHT_POS		(D3DXVECTOR3(1160.0f, GAMEUI_POSY, 0.0f))						// onscreenの右のUI座標

#define COLOR_WHITE		D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f)	// 白
#define COLOR_CYAN		D3DXCOLOR(0.0f, 1.0f, 1.0f, 1.0f)	// シアン

// ゲームUIの構造体
typedef struct
{
	GAMEUI_TYPE type;		// テクスチャの種類
	D3DXVECTOR3 pos;		// 中心座標
	D3DXCOLOR	col;		// 色
	float fWidth;			// 幅
	float fHeight;			// 高さ
}GAMEUI;

// ゲームUIの表示位置を管理
typedef struct
{
	GAMEUI gameUI[NUM_GAMEUI];	// テクスチャの種類
	D3DXVECTOR3 pos;			// 現在の位置
	D3DXVECTOR3 posDest;		// 目的値
	GAMEUI_STATE state;			// 状態を管理
	int nFrame;					// 現在のフレーム数
	int nNumFrame;				// 移動にかかるフレーム数
	int nSelect;				// 現在選択されているメニュー
	bool bMenu;					// メニュー状態を管理
	bool bDisp;					// 表示状態
	bool bPause;				// ポーズ状態を管理[trueでポーズ中]
}GameUI;

// グローバル変数
LPDIRECT3DTEXTURE9	g_apTextureGameUI[NUM_GAMEUI] = {};	// テクスチャへのポインタ
LPDIRECT3DVERTEXBUFFER9	g_pVtxBuffGameUI = NULL;		// 頂点バッファへのポインタ
GameUI g_aGameUI[MAX_GAMEUI];							// UIの管理
bool g_aisPause[MAX_PLAYER];									// ポーズしているかどうか
bool g_bPauseDisp = true;										// ポーズ画面表示状態

const char* c_apFilenameGameUI[NUM_GAMEUI] =
{
	"data\\TEXTURE\\Pause\\pause_100.png",
	"data\\TEXTURE\\Pause\\pause_101.png",
	"data\\TEXTURE\\Pause\\pause_000.png",
	"data\\TEXTURE\\Pause\\pause_001.png",
	"data\\TEXTURE\\Pause\\pause_002.png",
	"data\\TEXTURE\\Pause\\pause_003.png",
	"data\\TEXTURE\\Pause\\pause_004.png",
};

//======================================================================================
// ポーズの初期化処理
//======================================================================================
void InitGameUI(void)
{
	LPDIRECT3DDEVICE9 pDevice;	// デバイスへのポインタ
	// デバイスの取得
	pDevice = GetDevice();

	// UI情報の初期化
	for (int nCntPlayer = 0; nCntPlayer < MAX_PLAYER; nCntPlayer++)
	{
		for (int nCntUI = 0; nCntUI < NUM_GAMEUI; nCntUI++)
		{
			g_aGameUI[nCntPlayer].gameUI[nCntUI].type	= GAMEUI_TYPE_CLOCK;					// UIの種類の初期化
			g_aGameUI[nCntPlayer].gameUI[nCntUI].pos	= D3DXVECTOR3(0.0f, 0.0f, 0.0f);	// 位置の初期化
			g_aGameUI[nCntPlayer].gameUI[nCntUI].col	= COLOR_WHITE;	// 色の初期化
			g_aGameUI[nCntPlayer].gameUI[nCntUI].fWidth	= 0.0f;									// 幅の初期化
			g_aGameUI[nCntPlayer].gameUI[nCntUI].fHeight = 0.0f;								// 高さの初期化
		}
		g_aGameUI[nCntPlayer].state		= GAMEUI_STATE_OFFSCREEN;			// 画面外にある
		g_aGameUI[nCntPlayer].bDisp		= false;							// 全体の表示状態の初期化
		g_aGameUI[nCntPlayer].posDest	= D3DXVECTOR3(0.0f, 0.0f, 0.0f);	// 位置の初期化
		g_aGameUI[nCntPlayer].pos		= D3DXVECTOR3(0.0f, 0.0f, 0.0f);	// 位置の初期化
		g_aGameUI[nCntPlayer].nFrame	= 0;								// 現在のフレームを初期化
		g_aGameUI[nCntPlayer].nNumFrame	= FRAME;							// 出現の管理を行うフレーム数
		g_aGameUI[nCntPlayer].nSelect	= GAMEUI_TYPE_CLOCK;				// 選択状態の管理
		g_aGameUI[nCntPlayer].bMenu		= false;							// メニュー状態の管理
		g_aGameUI[nCntPlayer].bPause	= false;							// ポーズ状態の管理
	}

	// テクスチャの読み込み
	for (int nCntGameUI = 0; nCntGameUI < NUM_GAMEUI; nCntGameUI++)
	{
		D3DXCreateTextureFromFile(pDevice, c_apFilenameGameUI[nCntGameUI], &g_apTextureGameUI[nCntGameUI]);
	}

	// 操作方法の状態を取得
	OPERATIONTYPE operationType = GetOperationType();

	// 頂点バッファの生成
	pDevice->CreateVertexBuffer(sizeof(VERTEX_2D) * 4 * NUM_GAMEUI * MAX_PLAYER, D3DUSAGE_WRITEONLY, FVF_VERTEX_2D, D3DPOOL_MANAGED, &g_pVtxBuffGameUI, NULL);

	VERTEX_2D* pVtx;
	// 頂点バッファをロックし、頂点情報へのポインタを取得
	g_pVtxBuffGameUI->Lock(0, 0, (void**)&pVtx, 0);

	for (int nCntPlayer = 0; nCntPlayer < MAX_PLAYER; nCntPlayer++)
	{
		switch (operationType)	// 操作人数に応じてUIの位置を変更
		{
		case OPERATIONTYPE_2P:	// 2人操作
			switch (nCntPlayer)
			{
			case 0:
				g_aGameUI[nCntPlayer].pos = LEFT_OUTPOS;
				g_aGameUI[nCntPlayer].posDest = LEFT_OUTPOS;
				g_aGameUI[nCntPlayer].bDisp = false;

				break;
			case 1:
				g_aGameUI[nCntPlayer].pos = RIGHT_OUTPOS;
				g_aGameUI[nCntPlayer].pos = RIGHT_OUTPOS;
				g_aGameUI[nCntPlayer].bDisp = false;

				break;
			}
			break;

		default:	// 1人操作
			if (nCntPlayer > 0)
			{
				pVtx += NUM_GAMEUI * 4;
				continue;
			}
			g_aGameUI[nCntPlayer].bDisp = false;
			g_aGameUI[nCntPlayer].pos = RIGHT_OUTPOS;
			g_aGameUI[nCntPlayer].pos = RIGHT_OUTPOS;
			break;
		}

		for (int nCntUI = 0; nCntUI < NUM_GAMEUI; nCntUI++)
		{
			switch (nCntUI)
			{
			case GAMEUI_TYPE_PHONE:	// スマホ
				g_aGameUI[nCntPlayer].gameUI[nCntUI].type		= GAMEUI_TYPE_PHONE;				// UIの種類の初期化
				g_aGameUI[nCntPlayer].gameUI[nCntUI].pos		= D3DXVECTOR3(0.0f, 0.0f, 0.0f);	// 位置の初期化
				g_aGameUI[nCntPlayer].gameUI[nCntUI].col		= COLOR_WHITE;						// 色の初期化
				g_aGameUI[nCntPlayer].gameUI[nCntUI].fWidth		= PHONE_WIDTH;						// 幅の初期化
				g_aGameUI[nCntPlayer].gameUI[nCntUI].fHeight	= PHONE_HEIGHT;						// 高さの初期化
				break;

			case GAMEUI_TYPE_PAUSE:	// ポーズタイトル
				g_aGameUI[nCntPlayer].gameUI[nCntUI].type		= GAMEUI_TYPE_PAUSE;				// UIの種類の初期化
				g_aGameUI[nCntPlayer].gameUI[nCntUI].pos		= D3DXVECTOR3(0.0f, PAUSE_Y, 0.0f);	// 位置の初期化
				g_aGameUI[nCntPlayer].gameUI[nCntUI].col		= COLOR_WHITE;						// 色の初期化
				g_aGameUI[nCntPlayer].gameUI[nCntUI].fWidth		= PAUSE_WIDTH;						// 幅の初期化
				g_aGameUI[nCntPlayer].gameUI[nCntUI].fHeight	= PAUSE_HEIGHT;						// 高さの初期化
				break;

			case GAMEUI_TYPE_CLOCK:	// 時計
				g_aGameUI[nCntPlayer].gameUI[nCntUI].type		= GAMEUI_TYPE_CLOCK;				// UIの種類の初期化
				g_aGameUI[nCntPlayer].gameUI[nCntUI].pos		= D3DXVECTOR3(0.0f, CLOCK_Y, 0.0f);	// 位置の初期化
				g_aGameUI[nCntPlayer].gameUI[nCntUI].col		= COLOR_CYAN;						// 色の初期化
				g_aGameUI[nCntPlayer].gameUI[nCntUI].fWidth		= PAUSE_WIDTH;						// 幅の初期化
				g_aGameUI[nCntPlayer].gameUI[nCntUI].fHeight	= MENU_HEIGHT;						// 高さの初期化
				break;

			case GAMEUI_TYPE_MAGICBOOK:	// 魔導書
				g_aGameUI[nCntPlayer].gameUI[nCntUI].type		= GAMEUI_TYPE_MAGICBOOK;				// UIの種類の初期化
				g_aGameUI[nCntPlayer].gameUI[nCntUI].pos		= D3DXVECTOR3(0.0f, MAGICBOOK_Y, 0.0f);	// 位置の初期化
				g_aGameUI[nCntPlayer].gameUI[nCntUI].col		= COLOR_WHITE;						// 色の初期化
				g_aGameUI[nCntPlayer].gameUI[nCntUI].fWidth		= PAUSE_WIDTH;						// 幅の初期化
				g_aGameUI[nCntPlayer].gameUI[nCntUI].fHeight	= MENU_HEIGHT;						// 高さの初期化
				break;

			case GAMEUI_TYPE_CONTINUE:	// CONTINUE
				g_aGameUI[nCntPlayer].gameUI[nCntUI].type		= GAMEUI_TYPE_CONTINUE;				// UIの種類の初期化
				g_aGameUI[nCntPlayer].gameUI[nCntUI].pos		= D3DXVECTOR3(0.0f, CONTINUE_Y, 0.0f);	// 位置の初期化
				g_aGameUI[nCntPlayer].gameUI[nCntUI].col		= COLOR_WHITE;						// 色の初期化
				g_aGameUI[nCntPlayer].gameUI[nCntUI].fWidth		= PAUSE_WIDTH;						// 幅の初期化
				g_aGameUI[nCntPlayer].gameUI[nCntUI].fHeight	= MENU_HEIGHT;						// 高さの初期化
				break;

			case GAMEUI_TYPE_RETRY:	// RETRY
				g_aGameUI[nCntPlayer].gameUI[nCntUI].type		= GAMEUI_TYPE_RETRY;				// UIの種類の初期化
				g_aGameUI[nCntPlayer].gameUI[nCntUI].pos		= D3DXVECTOR3(0.0f, RETRY_Y, 0.0f);	// 位置の初期化
				g_aGameUI[nCntPlayer].gameUI[nCntUI].col		= COLOR_WHITE;						// 色の初期化
				g_aGameUI[nCntPlayer].gameUI[nCntUI].fWidth		= PAUSE_WIDTH;						// 幅の初期化
				g_aGameUI[nCntPlayer].gameUI[nCntUI].fHeight	= MENU_HEIGHT;						// 高さの初期化
				break;

			case GAMEUI_TYPE_QUIT:	// QUIT
				g_aGameUI[nCntPlayer].gameUI[nCntUI].type		= GAMEUI_TYPE_QUIT;					// UIの種類の初期化
				g_aGameUI[nCntPlayer].gameUI[nCntUI].pos		= D3DXVECTOR3(0.0f, QUIT_Y, 0.0f);	// 位置の初期化
				g_aGameUI[nCntPlayer].gameUI[nCntUI].col		= COLOR_WHITE;						// 色の初期化
				g_aGameUI[nCntPlayer].gameUI[nCntUI].fWidth		= PAUSE_WIDTH;						// 幅の初期化
				g_aGameUI[nCntPlayer].gameUI[nCntUI].fHeight	= MENU_HEIGHT;						// 高さの初期化
				break;
			}
		}

		for (int nCntUI = 0; nCntUI < NUM_GAMEUI; nCntUI++, pVtx += 4)	// 頂点バッファに数値を代入
		{
			// 頂点座標の設定
			pVtx[0].pos.x = g_aGameUI[nCntPlayer].pos.x + g_aGameUI[nCntPlayer].gameUI[nCntUI].pos.x - g_aGameUI[nCntPlayer].gameUI[nCntUI].fWidth;
			pVtx[0].pos.y = g_aGameUI[nCntPlayer].pos.y + g_aGameUI[nCntPlayer].gameUI[nCntUI].pos.y - g_aGameUI[nCntPlayer].gameUI[nCntUI].fHeight;
			pVtx[0].pos.z = 0.0f;
			pVtx[1].pos.x = g_aGameUI[nCntPlayer].pos.x + g_aGameUI[nCntPlayer].gameUI[nCntUI].pos.x + g_aGameUI[nCntPlayer].gameUI[nCntUI].fWidth;
			pVtx[1].pos.y = g_aGameUI[nCntPlayer].pos.y + g_aGameUI[nCntPlayer].gameUI[nCntUI].pos.y - g_aGameUI[nCntPlayer].gameUI[nCntUI].fHeight;
			pVtx[1].pos.z = 0.0f;
			pVtx[2].pos.x = g_aGameUI[nCntPlayer].pos.x + g_aGameUI[nCntPlayer].gameUI[nCntUI].pos.x - g_aGameUI[nCntPlayer].gameUI[nCntUI].fWidth;
			pVtx[2].pos.y = g_aGameUI[nCntPlayer].pos.y + g_aGameUI[nCntPlayer].gameUI[nCntUI].pos.y + g_aGameUI[nCntPlayer].gameUI[nCntUI].fHeight;
			pVtx[2].pos.z = 0.0f;
			pVtx[3].pos.x = g_aGameUI[nCntPlayer].pos.x + g_aGameUI[nCntPlayer].gameUI[nCntUI].pos.x + g_aGameUI[nCntPlayer].gameUI[nCntUI].fWidth;
			pVtx[3].pos.y = g_aGameUI[nCntPlayer].pos.y + g_aGameUI[nCntPlayer].gameUI[nCntUI].pos.y + g_aGameUI[nCntPlayer].gameUI[nCntUI].fHeight;
			pVtx[3].pos.z = 0.0f;

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
}

//======================================================================================
// ポーズの終了処理
//======================================================================================
void UninitGameUI(void)
{
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
}

//======================================================================================
// ポーズの更新処理
//======================================================================================
void UpdateGameUI(void)
{
	// フェードの状態を取得
	FADE* pFade = GetFade();

	VERTEX_2D* pVtx;
	// 頂点バッファをロックし、頂点情報へのポインタを取得
	g_pVtxBuffGameUI->Lock(0, 0, (void**)&pVtx, 0);

	for (int nCntPlayer = 0; nCntPlayer < MAX_PLAYER; nCntPlayer++)
	{
		OPERATIONTYPE operationType = GetOperationType();

		switch (operationType)
		{
		case OPERATIONTYPE_2P:	// 2人操作
			if (GetJoypadTrigger(JOYKEY_START, nCntPlayer) == true)	// 2Pでのポーズ切り替え管理
			{
				if (g_aGameUI[nCntPlayer].bPause == false && g_aGameUI[nCntPlayer].state == GAMEUI_STATE_OFFSCREEN)
				{
					if (nCntPlayer == 0)
					{
						g_aGameUI[nCntPlayer].posDest = LEFT_POS;
					}
					else if (nCntPlayer == 1)
					{
						g_aGameUI[nCntPlayer].posDest = RIGHT_POS;
					}
					g_aGameUI[nCntPlayer].bDisp = true;
					g_aGameUI[nCntPlayer].bPause = true;
					g_aGameUI[nCntPlayer].state = GAMEUI_STATE_APPEAR;
					g_aGameUI[nCntPlayer].nFrame = 0;
				}
				else if (g_aGameUI[nCntPlayer].bPause == true && g_aGameUI[nCntPlayer].state == GAMEUI_STATE_ONSCREEN)
				{
					if (nCntPlayer == 0)
					{
						g_aGameUI[nCntPlayer].posDest = LEFT_OUTPOS;
					}
					else if (nCntPlayer == 1)
					{
						g_aGameUI[nCntPlayer].posDest = RIGHT_OUTPOS;
					}
					g_aGameUI[nCntPlayer].bDisp = true;
					g_aGameUI[nCntPlayer].bPause = false;
					g_aGameUI[nCntPlayer].state = GAMEUI_STATE_DISAPPEAR;
					g_aGameUI[nCntPlayer].nFrame = 0;
				}
			}
			break;

		default:	// 1人操作
			if (nCntPlayer > 0)
			{
				continue;
			}
			if ((GetKeyboardTrigger(DIK_P) == true && nCntPlayer == 0) || GetJoypadTrigger(JOYKEY_START, nCntPlayer) == true)
			{
				if (g_aGameUI[nCntPlayer].bPause == false && g_aGameUI[nCntPlayer].state == GAMEUI_STATE_OFFSCREEN)
				{
					g_aGameUI[nCntPlayer].posDest = RIGHT_POS;
					g_aGameUI[nCntPlayer].bDisp = true;
					g_aGameUI[nCntPlayer].bPause = true;
					g_aGameUI[nCntPlayer].state = GAMEUI_STATE_APPEAR;
					g_aGameUI[nCntPlayer].nFrame = 0;
				}
				else if (g_aGameUI[nCntPlayer].bPause == true && g_aGameUI[nCntPlayer].state == GAMEUI_STATE_ONSCREEN)
				{
					g_aGameUI[nCntPlayer].posDest = RIGHT_OUTPOS;
					g_aGameUI[nCntPlayer].bDisp = true;
					g_aGameUI[nCntPlayer].bPause = false;
					g_aGameUI[nCntPlayer].state = GAMEUI_STATE_DISAPPEAR;
					g_aGameUI[nCntPlayer].nFrame = 0;
					if (g_aGameUI[nCntPlayer].bMenu == true)
					{
						DisappearMenuBG(nCntPlayer);
						g_aGameUI[nCntPlayer].bMenu = false;
					}
				}
			}
			break;
		}

		// 全体の移動を管理
		float fDiffKeyX = 0.0f;	// 現在の位置と目的の位置の差分を計算
		float fRateKey = (float)g_aGameUI[nCntPlayer].nFrame / (float)g_aGameUI[nCntPlayer].nNumFrame;
		if (g_aGameUI[nCntPlayer].bDisp == true)
		{
			switch (g_aGameUI[nCntPlayer].state)
			{
			case GAMEUI_STATE_NONE:	// 何もしていない状態

				break;

			case GAMEUI_STATE_ONSCREEN:	// スクリーン上にある
				g_aGameUI[nCntPlayer].bDisp = true;

				// 選択状態
				if ((GetKeyboardRepeat(DIK_W) == true && nCntPlayer == 0) || GetJoypadRepeat(JOYKEY_UP, nCntPlayer) == true || GetJoypadStickRepeatL(JOYSTICK_UP, nCntPlayer) == true)
				{
					g_aGameUI[nCntPlayer].nSelect--;
					if (g_aGameUI[nCntPlayer].nSelect < GAMEUI_TYPE_CLOCK)
					{
						g_aGameUI[nCntPlayer].nSelect = GAMEUI_TYPE_QUIT;
					}
				}
				else if ((GetKeyboardRepeat(DIK_S) == true && nCntPlayer == 0) || GetJoypadRepeat(JOYKEY_DOWN, nCntPlayer) == true || GetJoypadStickRepeatL(JOYSTICK_DOWN, nCntPlayer) == true)
				{
					g_aGameUI[nCntPlayer].nSelect++;
					if (g_aGameUI[nCntPlayer].nSelect > GAMEUI_TYPE_QUIT)
					{
						g_aGameUI[nCntPlayer].nSelect = GAMEUI_TYPE_CLOCK;
					}
				}

				// 選択したメニューを呼び出す
				if (*pFade == FADE_NONE && ((GetKeyboardTrigger(DIK_RETURN) == true && nCntPlayer == 0) || GetJoypadTrigger(JOYKEY_A, nCntPlayer) == true))
				{
					switch (g_aGameUI[nCntPlayer].nSelect)
					{
					case GAMEUI_TYPE_CLOCK:	// 時計
						if (g_aGameUI[nCntPlayer].bMenu == false)
						{
							if (operationType == OPERATIONTYPE_2P)	// 2P操作の場合
							{
								if (nCntPlayer == 0)
								{
									SetMenuBG(nCntPlayer, CLOCK_Y, MENUBG_TEX_CLOCK);
								}
								else if (nCntPlayer == 1)
								{
									SetMenuBG(nCntPlayer, CLOCK_Y, MENUBG_TEX_CLOCK);
								}
							}
							else
							{
								SetMenuBG(nCntPlayer, CLOCK_Y, MENUBG_TEX_CLOCK);
							}
							g_aGameUI[nCntPlayer].bMenu = true;
						}
						else if (g_aGameUI[nCntPlayer].bMenu == true)
						{
							DisappearMenuBG(nCntPlayer);
							g_aGameUI[nCntPlayer].bMenu = false;
						}
						break;

					case GAMEUI_TYPE_MAGICBOOK:	// 魔導書
						if (g_aGameUI[nCntPlayer].bMenu == false)
						{
							SetMenuBG(nCntPlayer, MAGICBOOK_Y, MENUBG_TEX_MAGICBOOK);
							g_aGameUI[nCntPlayer].bMenu = true;
						}
						else
						{
							DisappearMenuBG(nCntPlayer);
							g_aGameUI[nCntPlayer].bMenu = false;
						}
						break;

					case GAMEUI_TYPE_CONTINUE:	// CONTINUE
						if (g_aGameUI[nCntPlayer].state != GAMEUI_STATE_DISAPPEAR)
						{
							g_aGameUI[nCntPlayer].bDisp = true;
							g_aGameUI[nCntPlayer].bPause = false;
							if (operationType == OPERATIONTYPE_2P)	// 2P操作の場合
							{
								if (nCntPlayer == 0)
								{
									g_aGameUI[nCntPlayer].posDest = LEFT_OUTPOS;
								}
								else if (nCntPlayer == 1)
								{
									g_aGameUI[nCntPlayer].posDest = RIGHT_OUTPOS;
								}
							}
							else
							{
								g_aGameUI[nCntPlayer].posDest = RIGHT_OUTPOS;
							}
							g_aGameUI[nCntPlayer].state = GAMEUI_STATE_DISAPPEAR;
							g_aGameUI[nCntPlayer].nFrame = 0;
						}
						if (g_aGameUI[nCntPlayer].bMenu == true)
						{
							DisappearMenuBG(nCntPlayer);
						}
						break;

					case GAMEUI_TYPE_RETRY:	// RETRY
						SetFade(MODE_GAME);
						break;

					case GAMEUI_TYPE_QUIT:	// QUIT
						SetFade(MODE_TITLE);
						break;
					}
				}
				break;

			case GAMEUI_STATE_APPEAR:	// 出現状態
				fDiffKeyX = g_aGameUI[nCntPlayer].posDest.x - g_aGameUI[nCntPlayer].pos.x;
				g_aGameUI[nCntPlayer].pos.x = g_aGameUI[nCntPlayer].pos.x + fDiffKeyX * fRateKey;
				g_aGameUI[nCntPlayer].nFrame++;
				if (g_aGameUI[nCntPlayer].nFrame == g_aGameUI[nCntPlayer].nNumFrame)
				{
					g_aGameUI[nCntPlayer].state = GAMEUI_STATE_ONSCREEN;
					switch (operationType)
					{
					case OPERATIONTYPE_2P:	// 2人操作
						if (nCntPlayer == 0)
						{
							g_aGameUI[nCntPlayer].pos = LEFT_POS;
							g_aGameUI[nCntPlayer].posDest = LEFT_POS;
						}
						else if (nCntPlayer == 1)
						{
							g_aGameUI[nCntPlayer].pos = RIGHT_POS;
							g_aGameUI[nCntPlayer].posDest = RIGHT_POS;
						}
						break;

					default:	// 1人操作
						if (nCntPlayer == 0)
						{
							g_aGameUI[nCntPlayer].pos = RIGHT_POS;
							g_aGameUI[nCntPlayer].posDest = RIGHT_POS;
						}
						break;
					}

				}
				break;

			case GAMEUI_STATE_DISAPPEAR:	// 退出状態
				fDiffKeyX = g_aGameUI[nCntPlayer].posDest.x - g_aGameUI[nCntPlayer].pos.x;
				g_aGameUI[nCntPlayer].pos.x = g_aGameUI[nCntPlayer].pos.x + fDiffKeyX * fRateKey;
				g_aGameUI[nCntPlayer].nFrame++;
				if (g_aGameUI[nCntPlayer].nFrame == g_aGameUI[nCntPlayer].nNumFrame)
				{
					g_aGameUI[nCntPlayer].state = GAMEUI_STATE_OFFSCREEN;
					g_aGameUI[nCntPlayer].bDisp = false;
					g_aGameUI[nCntPlayer].nSelect = GAMEUI_TYPE_CLOCK;
					switch (operationType)
					{
					case OPERATIONTYPE_2P:	// 2人操作
						if (nCntPlayer == 0)
						{
							g_aGameUI[nCntPlayer].pos = LEFT_OUTPOS;
							g_aGameUI[nCntPlayer].posDest = LEFT_OUTPOS;
						}
						else if (nCntPlayer == 1)
						{
							g_aGameUI[nCntPlayer].pos = RIGHT_OUTPOS;
							g_aGameUI[nCntPlayer].posDest = RIGHT_OUTPOS;
						}
						break;

					default:	// 1人操作
						if (nCntPlayer == 0)
						{
							g_aGameUI[nCntPlayer].pos = RIGHT_OUTPOS;
							g_aGameUI[nCntPlayer].posDest = RIGHT_OUTPOS;
						}
						break;
					}
				}
				break;

			case GAMEUI_STATE_OFFSCREEN:	// スクリーン外にある
				g_aGameUI[nCntPlayer].bDisp = false;
				break;
			}
		}
		for (int nCntUI = 0; nCntUI < NUM_GAMEUI; nCntUI++, pVtx += 4)	// 頂点バッファに数値を代入
		{
			// 頂点座標の設定
			pVtx[0].pos.x = g_aGameUI[nCntPlayer].pos.x + g_aGameUI[nCntPlayer].gameUI[nCntUI].pos.x - g_aGameUI[nCntPlayer].gameUI[nCntUI].fWidth;
			pVtx[0].pos.y = g_aGameUI[nCntPlayer].pos.y + g_aGameUI[nCntPlayer].gameUI[nCntUI].pos.y - g_aGameUI[nCntPlayer].gameUI[nCntUI].fHeight;
			pVtx[0].pos.z = g_aGameUI[nCntPlayer].pos.z + g_aGameUI[nCntPlayer].gameUI[nCntUI].pos.z;
			pVtx[1].pos.x = g_aGameUI[nCntPlayer].pos.x + g_aGameUI[nCntPlayer].gameUI[nCntUI].pos.x + g_aGameUI[nCntPlayer].gameUI[nCntUI].fWidth;
			pVtx[1].pos.y = g_aGameUI[nCntPlayer].pos.y + g_aGameUI[nCntPlayer].gameUI[nCntUI].pos.y - g_aGameUI[nCntPlayer].gameUI[nCntUI].fHeight;
			pVtx[1].pos.z = g_aGameUI[nCntPlayer].pos.z + g_aGameUI[nCntPlayer].gameUI[nCntUI].pos.z;
			pVtx[2].pos.x = g_aGameUI[nCntPlayer].pos.x + g_aGameUI[nCntPlayer].gameUI[nCntUI].pos.x - g_aGameUI[nCntPlayer].gameUI[nCntUI].fWidth;
			pVtx[2].pos.y = g_aGameUI[nCntPlayer].pos.y + g_aGameUI[nCntPlayer].gameUI[nCntUI].pos.y + g_aGameUI[nCntPlayer].gameUI[nCntUI].fHeight;
			pVtx[2].pos.z = g_aGameUI[nCntPlayer].pos.z + g_aGameUI[nCntPlayer].gameUI[nCntUI].pos.z;
			pVtx[3].pos.x = g_aGameUI[nCntPlayer].pos.x + g_aGameUI[nCntPlayer].gameUI[nCntUI].pos.x + g_aGameUI[nCntPlayer].gameUI[nCntUI].fWidth;
			pVtx[3].pos.y = g_aGameUI[nCntPlayer].pos.y + g_aGameUI[nCntPlayer].gameUI[nCntUI].pos.y + g_aGameUI[nCntPlayer].gameUI[nCntUI].fHeight;
			pVtx[3].pos.z = g_aGameUI[nCntPlayer].pos.z + g_aGameUI[nCntPlayer].gameUI[nCntUI].pos.z;

			// rhwの設定
			pVtx[0].rhw = 1.0f;
			pVtx[1].rhw = 1.0f;
			pVtx[2].rhw = 1.0f;
			pVtx[3].rhw = 1.0f;

			if (g_aGameUI[nCntPlayer].nSelect == nCntUI)
			{
				// 頂点カラーの設定
				pVtx[0].col = COLOR_CYAN;
				pVtx[1].col = COLOR_CYAN;
				pVtx[2].col = COLOR_CYAN;
				pVtx[3].col = COLOR_CYAN;
			}
			else
			{
				// 頂点カラーの設定
				pVtx[0].col = COLOR_WHITE;
				pVtx[1].col = COLOR_WHITE;
				pVtx[2].col = COLOR_WHITE;
				pVtx[3].col = COLOR_WHITE;
			}

			// テクスチャ座標の設定
			pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
			pVtx[1].tex = D3DXVECTOR2(1.0f, 0.0f);
			pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
			pVtx[3].tex = D3DXVECTOR2(1.0f, 1.0f);
		}
	}

	// 頂点バッファをアンロック
	g_pVtxBuffGameUI->Unlock();
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

	// 一時的にfogを切る
	SetFogEnable(false);

	for (int nCntPlayer = 0; nCntPlayer < MAX_PLAYER; nCntPlayer++)
	{
		if (g_aGameUI[nCntPlayer].bDisp == true)
		{
			for (int nCntGameUI = 0; nCntGameUI < NUM_GAMEUI; nCntGameUI++)
			{
				// テクスチャの設定
				pDevice->SetTexture(0, g_apTextureGameUI[g_aGameUI[nCntPlayer].gameUI[nCntGameUI].type]);

				// ポリゴンの描画
				pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, nCntGameUI * 4 + (nCntPlayer * NUM_GAMEUI * 4), 2);
			}
		}
	}

	// fogの再設置
	SetFogEnable(true);
}
