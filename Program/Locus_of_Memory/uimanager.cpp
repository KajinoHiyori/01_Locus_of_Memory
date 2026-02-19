//======================================================================================
// 
// 3D空間用UI処理[uimanager.cpp]
// Author : KajinoHiyori
//
//======================================================================================
#include "uimanager.h"
#include "spellui.h"
#include "clock.h"
#include "battery.h"
#include "player.h"
#include "camera.h"
#include "main.h"
#include "input.h"
#include "color.h"
#include "title.h"
#include "debugproc.h"
#include "fade.h"

// マクロ定義
#define MAXUI_TEX		(UITEX_MAX)		// テクスチャの最大数
#define MAX_SELECT		(UITYPE_MAX)	// 選択できる最大数
#define NORMAL			(D3DXVECTOR3(0.0f, 1.0f, 0.0f))	// 法線ベクトル
#define GAMEUI_POSY		(110.0f)		// 左のUIのY軸
#define PHONE_WIDTH		(28.125f)		// スマホの幅
#define PHONE_HEIGHT	(50.0f)			// スマホの高さ
#define FLICKER			(0.5f)			// UI画面のちらつきを軽減
#define PHONE_Y			(50.0f)			// スマホの高度
#define UI_DISTANCEX	(-60.0f)		// UIとplayerの距離X
#define UI_DISTANCEZ	(-60.0f)		// UIとplayerの距離Z
#define NUM_KEY			(30)			// 処理を行うキー数

// UIのテクスチャの状態
typedef struct
{
	D3DXMATRIX	mtxWorld;		// ワールドマトリックス
	D3DXVECTOR3 pos;			// 中心位置
	D3DXCOLOR	col;			// 色
	UITEX		tex;			// テクスチャの種類
	float		fWidth;			// 幅
	float		fHeight;		// 高さ
	float		fWidthDest;		// 幅の目的地
	float		fHeightDest;	// 高さの目的地
	bool		bDisp;			// 表示状態
}UI_TEXTURE;

// UIの構造体定義
typedef struct
{
	UI_TEXTURE	aUITexture[MAXUI_TEX];
	D3DXVECTOR3 pos;		// 位置
	D3DXVECTOR3 rot;		// 向き
	UITYPE		type;		// 表示中のUIの種類
	UISTATE		state;		// UIの表示状態
	UISTATE		stateNext;	// UIの表示状態
	int			nSelect;	// 選択しているメニュー
	int			nNumKey;	// 処理を行うキー数
	int			nKey;		// 現在のキー
	bool		bPause;		// ポーズ状態
}UIManager;

// テクスチャの読み込み
const char* c_apFilenameUIManager[MAXUI_TEX] =
{
	"data\\TEXTURE\\Pause\\pause_100.png",	// 背景
	"data\\TEXTURE\\Pause\\pause_101.png",	// バッテリー[残量によって変化]
	"data\\TEXTURE\\Pause\\pause_102.png",	// バッテリーのフレーム
	"data\\TEXTURE\\Pause\\pause_103.png",	// ポーズメニュー
	"data\\TEXTURE\\Pause\\pause_104.png",	// 時計[メニュー]
	"data\\TEXTURE\\Pause\\pause_000.png",	// 時計[選択状態]
	"data\\TEXTURE\\Pause\\pause_001.png",	// continue
	"data\\TEXTURE\\Pause\\pause_002.png",	// retry
	"data\\TEXTURE\\Pause\\pause_003.png",	// quit
	"data\\TEXTURE\\Pause\\retrofilter.jpg",	// フィルター
	"data\\TEXTURE\\Pause\\retrofilter.jpg",	// フィルター
};

// グローバル変数
LPDIRECT3DTEXTURE9 g_apTextureUIManager[MAXUI_TEX] = {};		// テクスチャへのポインタ
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffUIManager = NULL;	// 頂点バッファへのポインタ
UIManager g_aUIManager[MAX_PLAYER];

//========================================================================
// UIの初期化処理
//========================================================================
void InitUIManager(void)
{
	// デバイスの取得
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	// 操作人数の取得
	OPERATIONTYPE operationType = GetOperationType();

	// プレイヤーの情報を取得
	Player* pPlayer = GetPlayer();

	// カメラ状態の取得
	Camera* pCamera = GetCamera();
	float fRotXCamera[MAX_CAMERA] = { 0.0f, 0.0f };
	float fRotYCamera[MAX_CAMERA] = { 0.0f, 0.0f };
	for (int nCntCamera = 0; nCntCamera < MAX_CAMERA; nCntCamera++)
	{
		fRotXCamera[nCntCamera] = pCamera[nCntCamera].rot.x;
		fRotYCamera[nCntCamera] = pCamera[nCntCamera].rot.y;
	}
	// テクスチャの読み込み
	for (int nCntUI = 0; nCntUI < MAXUI_TEX; nCntUI++)
	{
		D3DXCreateTextureFromFile(pDevice, c_apFilenameUIManager[nCntUI], &g_apTextureUIManager[nCntUI]);
	}

	// 初期化
	for (int nCntPlayer = 0; nCntPlayer < MAX_PLAYER; nCntPlayer++)
	{
		for (int nCntUI = 0; nCntUI < MAXUI_TEX; nCntUI++)
		{
			g_aUIManager[nCntPlayer].aUITexture[nCntUI].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);	// 中心位置
			g_aUIManager[nCntPlayer].aUITexture[nCntUI].col = COLOR_WHITE;						// 色
			g_aUIManager[nCntPlayer].aUITexture[nCntUI].tex = UITEX_BG;							// テクスチャの種類
			g_aUIManager[nCntPlayer].aUITexture[nCntUI].fWidth = PHONE_WIDTH;						// 幅
			g_aUIManager[nCntPlayer].aUITexture[nCntUI].fHeight = PHONE_HEIGHT;						// 高さ
			g_aUIManager[nCntPlayer].aUITexture[nCntUI].fWidthDest = 0.0f;								// 幅の目的地
			g_aUIManager[nCntPlayer].aUITexture[nCntUI].fHeightDest = 0.0f;								// 高さの目的地
			g_aUIManager[nCntPlayer].aUITexture[nCntUI].bDisp = false;							// 表示状態
		}
		g_aUIManager[nCntPlayer].pos = D3DXVECTOR3(0.0f, PHONE_Y, 0.0f);	// 中心位置
		g_aUIManager[nCntPlayer].rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);	// 中心位置
		g_aUIManager[nCntPlayer].type = UITYPE_CLOCK;						// 選択している種類(type)
		g_aUIManager[nCntPlayer].state = UISTATE_NONDISPLAY;				// UIの表示状態
		g_aUIManager[nCntPlayer].stateNext = UISTATE_NONDISPLAY;				// UIの表示状態
		g_aUIManager[nCntPlayer].nSelect = UITYPE_CLOCK;						// 選択している種類(int)
		g_aUIManager[nCntPlayer].nNumKey = NUM_KEY;							// 処理を行うキー数
		g_aUIManager[nCntPlayer].nKey = 0;								// 現在のキー
		g_aUIManager[nCntPlayer].bPause = false;							// ポーズ状態(trueでポーズ中)
	}

	// 頂点バッファの生成
	pDevice->CreateVertexBuffer(sizeof(VERTEX_3D) * 4 * MAXUI_TEX * MAX_PLAYER, D3DUSAGE_WRITEONLY, FVF_VERTEX_3D, D3DPOOL_MANAGED, &g_pVtxBuffUIManager, NULL);

	VERTEX_3D* pVtx;
	// 頂点バッファをロックし、頂点情報へのポインタを取得
	g_pVtxBuffUIManager->Lock(0, 0, (void**)&pVtx, 0);

	for (int nCntPlayer = 0; nCntPlayer < MAX_PLAYER; nCntPlayer++)
	{
		for (int nCntUI = 0; nCntUI < MAXUI_TEX; nCntUI++, pVtx += 4)
		{
			// 各種情報の設定
			switch (nCntUI)
			{
			case UITEX_BG:	// 背景
				g_aUIManager[nCntPlayer].aUITexture[nCntUI].col = COLOR_DISALPHA;		// 色
				g_aUIManager[nCntPlayer].aUITexture[nCntUI].tex = UITEX_BG;				// テクスチャの種類
				g_aUIManager[nCntPlayer].aUITexture[nCntUI].fWidth = PHONE_WIDTH;			// 幅
				g_aUIManager[nCntPlayer].aUITexture[nCntUI].fHeight = 0.0f;					// 高さ
				g_aUIManager[nCntPlayer].aUITexture[nCntUI].fWidthDest = PHONE_WIDTH;			// 幅の目的地
				g_aUIManager[nCntPlayer].aUITexture[nCntUI].fHeightDest = PHONE_HEIGHT;			// 高さの目的地
				break;

			case UITEX_BATTERY:	// バッテリー
				g_aUIManager[nCntPlayer].aUITexture[nCntUI].col = COLOR_WHITE;			// 色
				g_aUIManager[nCntPlayer].aUITexture[nCntUI].tex = UITEX_BATTERY;		// テクスチャの種類
				break;

			case UITEX_BATTERYFRAME:	// バッテリーフレーム
				g_aUIManager[nCntPlayer].aUITexture[nCntUI].col = COLOR_WHITE;			// 色
				g_aUIManager[nCntPlayer].aUITexture[nCntUI].tex = UITEX_BATTERYFRAME;	// テクスチャの種類
				break;

			case UITEX_PAUSEMENU:	// ポーズメニュー
				g_aUIManager[nCntPlayer].aUITexture[nCntUI].col = COLOR_WHITE;			// 色
				g_aUIManager[nCntPlayer].aUITexture[nCntUI].tex = UITEX_PAUSEMENU;		// テクスチャの種類
				break;

			case UITEX_CLOCKMENU:	// 時計メニュー
				g_aUIManager[nCntPlayer].aUITexture[nCntUI].col = COLOR_WHITE;			// 色
				g_aUIManager[nCntPlayer].aUITexture[nCntUI].tex = UITEX_CLOCKMENU;		// テクスチャの種類
				break;

			case UITEX_CLOCK:	// 時計[選択状態]
				g_aUIManager[nCntPlayer].aUITexture[nCntUI].col = COLOR_YELLOW;			// 色
				g_aUIManager[nCntPlayer].aUITexture[nCntUI].tex = UITEX_CLOCK;			// テクスチャの種類
				break;

			case UITEX_CONTINUE:	// continue
				g_aUIManager[nCntPlayer].aUITexture[nCntUI].col = COLOR_WHITE;			// 色
				g_aUIManager[nCntPlayer].aUITexture[nCntUI].tex = UITEX_CONTINUE;		// テクスチャの種類
				break;

			case UITEX_RETRY:	// retry
				g_aUIManager[nCntPlayer].aUITexture[nCntUI].col = COLOR_WHITE;			// 色
				g_aUIManager[nCntPlayer].aUITexture[nCntUI].tex = UITEX_RETRY;			// テクスチャの種類
				break;

			case UITEX_QUIT:	// quit
				g_aUIManager[nCntPlayer].aUITexture[nCntUI].col = COLOR_WHITE;			// 色
				g_aUIManager[nCntPlayer].aUITexture[nCntUI].tex = UITEX_QUIT;			// テクスチャの種類
				break;

			case UITEX_FILTER:	// フィルター
				g_aUIManager[nCntPlayer].aUITexture[nCntUI].col = COLOR_RETROFILTER;	// 色
				g_aUIManager[nCntPlayer].aUITexture[nCntUI].tex = UITEX_FILTER;			// テクスチャの種類
				g_aUIManager[nCntPlayer].aUITexture[nCntUI].fWidth = PHONE_WIDTH;			// 幅
				g_aUIManager[nCntPlayer].aUITexture[nCntUI].fHeight = 0.0f;					// 高さ
				g_aUIManager[nCntPlayer].aUITexture[nCntUI].fWidthDest = PHONE_WIDTH;			// 幅の目的地
				g_aUIManager[nCntPlayer].aUITexture[nCntUI].fHeightDest = PHONE_HEIGHT;			// 高さの目的地
				break;

			case UITEX_BGFILTER:	// 背景フィルター
				g_aUIManager[nCntPlayer].aUITexture[nCntUI].col = COLOR_RETROFILTER;		// 色
				g_aUIManager[nCntPlayer].aUITexture[nCntUI].tex = UITEX_BGFILTER;			// テクスチャの種類
				g_aUIManager[nCntPlayer].aUITexture[nCntUI].fWidth = PHONE_WIDTH;			// 幅
				g_aUIManager[nCntPlayer].aUITexture[nCntUI].fHeight = 0.0f;					// 高さ
				g_aUIManager[nCntPlayer].aUITexture[nCntUI].fWidthDest = PHONE_WIDTH;			// 幅の目的地
				g_aUIManager[nCntPlayer].aUITexture[nCntUI].fHeightDest = PHONE_HEIGHT;			// 高さの目的地
				break;
			}

			// 中心位置からの位置を求める
			g_aUIManager[nCntPlayer].aUITexture[nCntUI].pos += g_aUIManager[nCntPlayer].pos;

			// 頂点座標の設定
			pVtx[0].pos = D3DXVECTOR3(-g_aUIManager[nCntPlayer].aUITexture[nCntUI].fWidth, g_aUIManager[nCntPlayer].aUITexture[nCntUI].fHeight, 0.0f);
			pVtx[1].pos = D3DXVECTOR3(g_aUIManager[nCntPlayer].aUITexture[nCntUI].fWidth, g_aUIManager[nCntPlayer].aUITexture[nCntUI].fHeight, 0.0f);
			pVtx[2].pos = D3DXVECTOR3(-g_aUIManager[nCntPlayer].aUITexture[nCntUI].fWidth, -g_aUIManager[nCntPlayer].aUITexture[nCntUI].fHeight, 0.0f);
			pVtx[3].pos = D3DXVECTOR3(g_aUIManager[nCntPlayer].aUITexture[nCntUI].fWidth, -g_aUIManager[nCntPlayer].aUITexture[nCntUI].fHeight, 0.0f);

			// rhwの設定
			pVtx[0].nor = NORMAL;
			pVtx[1].nor = NORMAL;
			pVtx[2].nor = NORMAL;
			pVtx[3].nor = NORMAL;

			// 頂点カラーの設定
			pVtx[0].col = g_aUIManager[nCntPlayer].aUITexture[nCntUI].col;
			pVtx[1].col = g_aUIManager[nCntPlayer].aUITexture[nCntUI].col;
			pVtx[2].col = g_aUIManager[nCntPlayer].aUITexture[nCntUI].col;
			pVtx[3].col = g_aUIManager[nCntPlayer].aUITexture[nCntUI].col;

			// テクスチャ座標の設定
			if (g_aUIManager[nCntPlayer].aUITexture[nCntUI].tex == UITEX_FILTER || g_aUIManager[nCntPlayer].aUITexture[nCntUI].tex == UITEX_FILTER)
			{
				pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
				pVtx[1].tex = D3DXVECTOR2(1.0f, 0.0f);
				pVtx[2].tex = D3DXVECTOR2(0.0f, 1.5f);
				pVtx[3].tex = D3DXVECTOR2(1.0f, 1.5f);
			}
			else
			{
				pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
				pVtx[1].tex = D3DXVECTOR2(1.0f, 0.0f);
				pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
				pVtx[3].tex = D3DXVECTOR2(1.0f, 1.0f);
			}
		}
	}
	// 頂点バッファをアンロック
	g_pVtxBuffUIManager->Unlock();
}

//========================================================================
// UIの終了処理
//========================================================================
void UninitUIManager(void)
{
	// テクスチャの破棄
	for (int nCntUI = 0; nCntUI < MAXUI_TEX; nCntUI++)
	{
		if (g_apTextureUIManager[nCntUI] != NULL)
		{
			g_apTextureUIManager[nCntUI]->Release();
			g_apTextureUIManager[nCntUI] = NULL;
		}
	}

	// 頂点バッファの破棄
	if (g_pVtxBuffUIManager != NULL)
	{
		g_pVtxBuffUIManager->Release();
		g_pVtxBuffUIManager = NULL;
	}
	//#endif
}

//========================================================================
// UIの更新処理
//========================================================================
void UpdateUIManager(void)
{
	Player* pPlayer = GetPlayer();	// プレイヤーの情報を取得
	FADE* pFade = GetFade();	// フェードの状態を取得
	D3DXVECTOR3 posOffset = D3DXVECTOR3(0.0f, 0.0f, 0.0f);	// オフセットの情報を初期化

	VERTEX_3D* pVtx;
	// 頂点バッファをロックし、頂点情報へのポインタを取得
	g_pVtxBuffUIManager->Lock(0, 0, (void**)&pVtx, 0);

	for (int nCntPlayer = 0; nCntPlayer < MAX_PLAYER; nCntPlayer++, pPlayer++)
	{
		switch (g_aUIManager[nCntPlayer].state)
		{
		case UISTATE_NONDISPLAY:	// 非表示状態
			g_aUIManager[nCntPlayer].nSelect = UITYPE_CLOCK;	// 時計を選択状態にする
			g_aUIManager[nCntPlayer].type = UITYPE_CLOCK;	// 時計を選択状態にする
			g_aUIManager[nCntPlayer].bPause = false;		//ポーズ状態でなくする
			for (int nCntUI = 0; nCntUI < MAXUI_TEX; nCntUI++)
			{
				g_aUIManager[nCntPlayer].aUITexture[nCntUI].bDisp = false;
			}
			if ((GetKeyboardTrigger(DIK_P) == true && nCntPlayer == 0) || GetJoypadTrigger(JOYKEY_START, nCntPlayer) == true)
			{
				SetUIAppear(nCntPlayer);	// UIを出現状態にする
			}
			break;

		case UISTATE_APPEAR:	// 出現状態
			UpdateUIBG(nCntPlayer);
			if (g_aUIManager[nCntPlayer].nKey > g_aUIManager[nCntPlayer].nNumKey)
			{
				SetUISelect(nCntPlayer);
				g_aUIManager[nCntPlayer].stateNext = UISTATE_NONDISPLAY;
			}
			break;

		case UISTATE_SELECT:	// セレクトメニュー
			if (((GetKeyboardTrigger(DIK_P) == true && nCntPlayer == 0) || GetJoypadTrigger(JOYKEY_START, nCntPlayer) == true) && g_aUIManager[nCntPlayer].bPause == true)
			{
				SetUIDissapear(nCntPlayer);	// UIの表示状態を消滅状態にする
			}
			else
			{
				// 選択に合わせてメニューを切り替え
				if ((GetKeyboardRepeat(DIK_W) == true && nCntPlayer == 0) || GetJoypadRepeat(JOYKEY_UP, nCntPlayer) == true || GetJoypadStickRepeatL(JOYSTICK_UP, nCntPlayer) == true)
				{
					g_aUIManager[nCntPlayer].nSelect--;
					if (g_aUIManager[nCntPlayer].nSelect < UITYPE_CLOCK)
					{
						g_aUIManager[nCntPlayer].nSelect = UITYPE_QUIT;
					}
				}
				else if ((GetKeyboardRepeat(DIK_S) == true && nCntPlayer == 0) || GetJoypadRepeat(JOYKEY_DOWN, nCntPlayer) == true || GetJoypadStickRepeatL(JOYSTICK_DOWN, nCntPlayer) == true)
				{
					g_aUIManager[nCntPlayer].nSelect++;
					if (g_aUIManager[nCntPlayer].nSelect > UITYPE_QUIT)
					{
						g_aUIManager[nCntPlayer].nSelect = UITYPE_CLOCK;
					}
				}

				// テクスチャの色を変更
				for (int nCntSelect = UITEX_CLOCKMENU; nCntSelect < UITEX_FILTER; nCntSelect++)
				{
					if (g_aUIManager[nCntPlayer].nSelect + 5 == nCntSelect)
					{
						g_aUIManager[nCntPlayer].aUITexture[nCntSelect].col = COLOR_YELLOW;
					}
					else
					{
						g_aUIManager[nCntPlayer].aUITexture[nCntSelect].col = COLOR_WHITE;
					}

				}

				if (*pFade == FADE_NONE && ((GetKeyboardTrigger(DIK_RETURN) == true && nCntPlayer == 0) || GetJoypadTrigger(JOYKEY_A, nCntPlayer) == true))
				{
					switch (g_aUIManager[nCntPlayer].nSelect)
					{
					case UITYPE_CLOCK:	// 時計状態を選択
						SetUIDissapear(nCntPlayer);
						g_aUIManager[nCntPlayer].stateNext = UISTATE_CLOCKAPPEAR;
						break;

					case UITYPE_CONTINUE:	// CONTINUEを選択
						SetUIDissapear(nCntPlayer);
						break;

					case UITYPE_RETRY:	// RETRYを選択
						SetFade(MODE_GAME, COLOR_WHITE);
						break;

					case UITYPE_QUIT:	// QUITを選択
						SetFade(MODE_TITLE, COLOR_WHITE);
						break;
					}
				}
			}
			break;

		case UISTATE_DISAPPEAR:	// 消滅
			UpdateUIBG(nCntPlayer);
			if (g_aUIManager[nCntPlayer].nKey > g_aUIManager[nCntPlayer].nNumKey)
			{
				if (g_aUIManager[nCntPlayer].stateNext == UISTATE_CLOCKAPPEAR)
				{
					SetClockAppear(nCntPlayer);
				}
				else
				{
					SetUINonDisp(nCntPlayer);
				}
			}
			break;

		case UISTATE_CLOCKAPPEAR:	// 時計の出現
			UpdateUIBG(nCntPlayer);
			if (g_aUIManager[nCntPlayer].nKey > g_aUIManager[nCntPlayer].nNumKey)
			{
				SetClockMenu(nCntPlayer);
			}
			break;

		case UISTATE_CLOCK:	// 時計
			if ((GetKeyboardTrigger(DIK_RETURN) == true && nCntPlayer == 0) || GetJoypadTrigger(JOYKEY_A, nCntPlayer) == true)
			{
				SetClockDissapear(nCntPlayer);
				g_aUIManager[nCntPlayer].stateNext = UISTATE_APPEAR;
			}
			else if (((GetKeyboardTrigger(DIK_P) == true && nCntPlayer == 0) || GetJoypadTrigger(JOYKEY_START, nCntPlayer) == true) && g_aUIManager[nCntPlayer].bPause == true)
			{
				SetClockDissapear(nCntPlayer);
				g_aUIManager[nCntPlayer].stateNext = UISTATE_NONDISPLAY;
			}
			break;

		case UISTATE_CLOCKDISAPPEAR:	// 時計の消滅
			UpdateUIBG(nCntPlayer);
			if (g_aUIManager[nCntPlayer].nKey > g_aUIManager[nCntPlayer].nNumKey)
			{
				if (g_aUIManager[nCntPlayer].stateNext == UISTATE_APPEAR)
				{
					SetUIAppear(nCntPlayer);
				}
				else
				{
					SetUINonDisp(nCntPlayer);
				}
			}
			break;
		}

		if (g_aUIManager[nCntPlayer].bPause == false)
		{
			continue;
		}
		
		pPlayer->state = PLAYERSTATE_PAUSE;
		// UIの表示角度を設定
		g_aUIManager[nCntPlayer].rot.y = pPlayer->rot.y + D3DX_PI;
		g_aUIManager[nCntPlayer].rot.y = AngleNormalize(g_aUIManager[nCntPlayer].rot.y);

		// プレイヤーの位置や向きに合わせてUIの表示位置を更新
		g_aUIManager[nCntPlayer].pos.x = pPlayer->pos.x + sinf(pPlayer->rot.y - 0.875f) * UI_DISTANCEX;
		g_aUIManager[nCntPlayer].pos.z = pPlayer->pos.z + cosf(pPlayer->rot.y - 0.875f) * UI_DISTANCEZ;

		for (int nCntUI = 0; nCntUI < MAXUI_TEX; nCntUI++, pVtx += 4)
		{
			// オフセット情報の初期化
			g_aUIManager[nCntPlayer].aUITexture[nCntUI].pos = posOffset;

			// 中心位置からの位置を求める
			g_aUIManager[nCntPlayer].aUITexture[nCntUI].pos = g_aUIManager[nCntPlayer].pos;

			// 頂点座標の設定
			pVtx[0].pos = D3DXVECTOR3(-g_aUIManager[nCntPlayer].aUITexture[nCntUI].fWidth, g_aUIManager[nCntPlayer].aUITexture[nCntUI].fHeight, 0.0f);
			pVtx[1].pos = D3DXVECTOR3(g_aUIManager[nCntPlayer].aUITexture[nCntUI].fWidth, g_aUIManager[nCntPlayer].aUITexture[nCntUI].fHeight, 0.0f);
			pVtx[2].pos = D3DXVECTOR3(-g_aUIManager[nCntPlayer].aUITexture[nCntUI].fWidth, -g_aUIManager[nCntPlayer].aUITexture[nCntUI].fHeight, 0.0f);
			pVtx[3].pos = D3DXVECTOR3(g_aUIManager[nCntPlayer].aUITexture[nCntUI].fWidth, -g_aUIManager[nCntPlayer].aUITexture[nCntUI].fHeight, 0.0f);

			// 頂点カラーの設定
			pVtx[0].col = g_aUIManager[nCntPlayer].aUITexture[nCntUI].col;
			pVtx[1].col = g_aUIManager[nCntPlayer].aUITexture[nCntUI].col;
			pVtx[2].col = g_aUIManager[nCntPlayer].aUITexture[nCntUI].col;
			pVtx[3].col = g_aUIManager[nCntPlayer].aUITexture[nCntUI].col;
		}
	}

	// 頂点バッファをアンロック
	g_pVtxBuffUIManager->Unlock();
}

//========================================================================
// UIの描画処理
//========================================================================
void DrawUIManager(void)
{
	//#if 0
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
		if (g_aUIManager[nCntPlayer].bPause == false)
		{
			continue;
		}

		for (int nCntUI = 0; nCntUI < MAXUI_TEX; nCntUI++)
		{
			if (g_aUIManager[nCntPlayer].aUITexture[nCntUI].bDisp == false)
			{
				continue;
			}

			// フィルターのみ加算合成にする
			if (g_aUIManager[nCntPlayer].aUITexture[nCntUI].tex == UITEX_FILTER || g_aUIManager[nCntPlayer].aUITexture[nCntUI].tex == UITEX_FILTER)
			{
				pDevice->SetRenderState(D3DRS_BLENDOP, D3DBLENDOP_ADD);
				pDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
				pDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_ONE);
			}

			// ワールドマトリックスの初期化(デフォルトの値にする)
			D3DXMatrixIdentity(&g_aUIManager[nCntPlayer].aUITexture[nCntUI].mtxWorld);

			// ビューマトリックスを取得する
			pDevice->GetTransform(D3DTS_VIEW, &mtxView);

			// 向きを反映
			D3DXMatrixRotationYawPitchRoll(&mtxRot, g_aUIManager[nCntPlayer].rot.y, g_aUIManager[nCntPlayer].rot.x, g_aUIManager[nCntPlayer].rot.z);
			D3DXMatrixMultiply(&g_aUIManager[nCntPlayer].aUITexture[nCntUI].mtxWorld, &g_aUIManager[nCntPlayer].aUITexture[nCntUI].mtxWorld, &mtxRot);

			// 位置を反映
			D3DXMatrixTranslation(&mtxTrans, g_aUIManager[nCntPlayer].aUITexture[nCntUI].pos.x, g_aUIManager[nCntPlayer].aUITexture[nCntUI].pos.y, g_aUIManager[nCntPlayer].aUITexture[nCntUI].pos.z);
			D3DXMatrixMultiply(&g_aUIManager[nCntPlayer].aUITexture[nCntUI].mtxWorld, &g_aUIManager[nCntPlayer].aUITexture[nCntUI].mtxWorld, &mtxTrans);

			// ワールドマトリックスの設定
			pDevice->SetTransform(D3DTS_WORLD, &g_aUIManager[nCntPlayer].aUITexture[nCntUI].mtxWorld);

			// 頂点バッファをデータストリームに設定
			pDevice->SetStreamSource(0, g_pVtxBuffUIManager, 0, sizeof(VERTEX_3D));

			// 頂点フォーマットの設定
			pDevice->SetFVF(FVF_VERTEX_3D);

			// テクスチャの設定
			pDevice->SetTexture(0, g_apTextureUIManager[g_aUIManager[nCntPlayer].aUITexture[nCntUI].tex]);

			// UIの描画
			pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, nCntUI * 4 + (nCntPlayer * MAXUI_TEX * 4), 2);

			// フィルターを加算合成から戻す
			if (g_aUIManager[nCntPlayer].aUITexture[nCntUI].tex == UITEX_FILTER || g_aUIManager[nCntPlayer].aUITexture[nCntUI].tex == UITEX_FILTER)
			{
				//αブレンディングを戻す
				pDevice->SetRenderState(D3DRS_BLENDOP, D3DBLENDOP_ADD);
				pDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
				pDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);
			}
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

//========================================================================
// ポーズ状態の初期化
//========================================================================
void SetPauseFalse(void)
{
	for (int nCntPlayer = 0; nCntPlayer < MAX_PLAYER; nCntPlayer++)
	{
		g_aUIManager[nCntPlayer].bPause = false;
		SetUINonDisp(nCntPlayer);
	}
}

//========================================================================
// ポーズ状態を管理
//========================================================================
bool GetPause(int nIdx)
{
	return g_aUIManager[nIdx].bPause;
}

//========================================================================
// UIのマトリックス情報を取得
//========================================================================
D3DXMATRIX GetUIMatrix(int nIdx)
{
	return g_aUIManager[nIdx].aUITexture[UITEX_BG].mtxWorld;
}

//========================================================================
// UIの位置を取得
//========================================================================
D3DXVECTOR3 GetUIPos(int nIdx)
{
	return g_aUIManager[nIdx].pos;
}

//========================================================================
// UIの角度を取得
//========================================================================
D3DXVECTOR3 GetUIRot(int nIdx)
{
	return g_aUIManager[nIdx].rot;
}

//========================================================================
// 背景の拡縮を管理
//========================================================================
void UpdateUIBG(int nIdx)
{
	float fDiffKey = 0.0f;	// キーの差分を計算
	float fRateKey = (float)g_aUIManager[nIdx].nKey / (float)g_aUIManager[nIdx].nNumKey;

	// 背景の高度変更
	fDiffKey = g_aUIManager[nIdx].aUITexture[UITEX_BG].fHeightDest - g_aUIManager[nIdx].aUITexture[UITEX_BG].fHeight;
	g_aUIManager[nIdx].aUITexture[UITEX_BG].fHeight = g_aUIManager[nIdx].aUITexture[UITEX_BG].fHeight + fDiffKey * fRateKey;
	// 背景フィルターの高度変更
	fDiffKey = g_aUIManager[nIdx].aUITexture[UITEX_FILTER].fHeightDest - g_aUIManager[nIdx].aUITexture[UITEX_FILTER].fHeight;
	g_aUIManager[nIdx].aUITexture[UITEX_FILTER].fHeight = g_aUIManager[nIdx].aUITexture[UITEX_FILTER].fHeight + fDiffKey * fRateKey;
	// フィルターの高度変更
	fDiffKey = g_aUIManager[nIdx].aUITexture[UITEX_BGFILTER].fHeightDest - g_aUIManager[nIdx].aUITexture[UITEX_BGFILTER].fHeight;
	g_aUIManager[nIdx].aUITexture[UITEX_BGFILTER].fHeight = g_aUIManager[nIdx].aUITexture[UITEX_BGFILTER].fHeight + fDiffKey * fRateKey;
	g_aUIManager[nIdx].nKey++;
}

//========================================================================
// UIを出現状態にする
//========================================================================
void SetUIAppear(int nIdx)
{
	Player* pPlayer = GetPlayer();
	pPlayer[nIdx].state = PLAYERSTATE_PAUSE;
	g_aUIManager[nIdx].bPause = true;
	g_aUIManager[nIdx].state = UISTATE_APPEAR;
	g_aUIManager[nIdx].nKey = 0;
	g_aUIManager[nIdx].aUITexture[UITEX_BG].fHeight = 0.0f;			// 高さを管理
	g_aUIManager[nIdx].aUITexture[UITEX_BG].fHeightDest = PHONE_HEIGHT;	// 高さの目的値を設定
	g_aUIManager[nIdx].aUITexture[UITEX_BG].bDisp = true;			// 表示状態の管理
	g_aUIManager[nIdx].aUITexture[UITEX_FILTER].fHeight = 0.0f;			// 高さを管理
	g_aUIManager[nIdx].aUITexture[UITEX_FILTER].fHeightDest = PHONE_HEIGHT;	// 高さの目的値を設定
	g_aUIManager[nIdx].aUITexture[UITEX_FILTER].bDisp = true;			// 表示状態の管理
	g_aUIManager[nIdx].aUITexture[UITEX_BGFILTER].fHeight = 0.0f;			// 高さを管理
	g_aUIManager[nIdx].aUITexture[UITEX_BGFILTER].fHeightDest = PHONE_HEIGHT;	// 高さの目的値を設定
	g_aUIManager[nIdx].aUITexture[UITEX_BGFILTER].bDisp = true;			// 表示状態の管理
}

//========================================================================
// UIを消滅状態にする
//========================================================================
void SetUIDissapear(int nIdx)
{
	for (int nCntUI = 0; nCntUI < MAXUI_TEX; nCntUI++)
	{
		g_aUIManager[nIdx].aUITexture[nCntUI].bDisp = false;
	}
	DissapearBattery(nIdx);
	g_aUIManager[nIdx].state = UISTATE_DISAPPEAR;
	g_aUIManager[nIdx].nKey = 0;
	g_aUIManager[nIdx].aUITexture[UITEX_BG].bDisp = true;			// 表示状態の管理
	g_aUIManager[nIdx].aUITexture[UITEX_FILTER].bDisp = true;			// 表示状態の管理
	g_aUIManager[nIdx].aUITexture[UITEX_BGFILTER].bDisp = true;			// 表示状態の管理
	g_aUIManager[nIdx].aUITexture[UITEX_BG].fHeight = PHONE_HEIGHT;			// 高さを管理
	g_aUIManager[nIdx].aUITexture[UITEX_FILTER].fHeight = PHONE_HEIGHT;			// 高さを管理
	g_aUIManager[nIdx].aUITexture[UITEX_BGFILTER].fHeight = PHONE_HEIGHT;			// 高さを管理
	g_aUIManager[nIdx].aUITexture[UITEX_BG].fHeightDest = 0.0f;	// 高さの目的値を設定
	g_aUIManager[nIdx].aUITexture[UITEX_FILTER].fHeightDest = 0.0f;	// 高さの目的値を設定
	g_aUIManager[nIdx].aUITexture[UITEX_BGFILTER].fHeightDest = 0.0f;	// 高さの目的値を設定

}

//========================================================================
// UIを選択状態にする
//========================================================================
void SetUISelect(int nIdx)
{
	for (int nCntUI = 0; nCntUI < MAXUI_TEX; nCntUI++)
	{
		g_aUIManager[nIdx].nKey = 0;
		g_aUIManager[nIdx].aUITexture[nCntUI].fWidth = PHONE_WIDTH;	// 幅
		g_aUIManager[nIdx].aUITexture[nCntUI].fHeight = PHONE_HEIGHT;	// 高さ
		g_aUIManager[nIdx].aUITexture[nCntUI].fWidthDest = 0.0f;		// 幅の目的地
		g_aUIManager[nIdx].aUITexture[nCntUI].fHeightDest = 0.0f;		// 高さの目的地
		g_aUIManager[nIdx].aUITexture[nCntUI].bDisp = true;		// 表示状態
		g_aUIManager[nIdx].state = UISTATE_SELECT;					// 選択状態
	}
	SetBattery(nIdx);
	g_aUIManager[nIdx].aUITexture[UITEX_CLOCKMENU].bDisp = false;
}

//========================================================================
// UIを非表示状態にする
//========================================================================
void SetUINonDisp(int nIdx)
{
	for (int nCntUI = 0; nCntUI < MAXUI_TEX; nCntUI++)
	{
		g_aUIManager[nIdx].aUITexture[nCntUI].fWidth = PHONE_WIDTH;	// 幅
		g_aUIManager[nIdx].aUITexture[nCntUI].fHeight = 0.0f;	// 高さ
		g_aUIManager[nIdx].aUITexture[nCntUI].fWidthDest = PHONE_WIDTH;		// 幅の目的地
		g_aUIManager[nIdx].aUITexture[nCntUI].fHeightDest = PHONE_HEIGHT;		// 高さの目的地
		g_aUIManager[nIdx].aUITexture[nCntUI].bDisp = false;		// 表示状態
		g_aUIManager[nIdx].state = UISTATE_NONDISPLAY;					// 選択状態
	}
	g_aUIManager[nIdx].nKey = 0;
	g_aUIManager[nIdx].bPause = false;
	g_aUIManager[nIdx].state = UISTATE_NONDISPLAY;				// UIの表示状態
	g_aUIManager[nIdx].stateNext = UISTATE_NONDISPLAY;				// UIの表示状態
	g_aUIManager[nIdx].nSelect = UITYPE_CLOCK;						// 選択している種類(int)
	DissapearBattery(nIdx);
	Player* pPlayer = GetPlayer();
	pPlayer[nIdx].state = PLAYERSTATE_NORMAL;
	if (((GetKeyboardPress(DIK_TAB) == true && nIdx == 0) || GetJoypadRightTriggePress(nIdx) == true || GetJoypadLeftTriggePress(nIdx) == true))
	{
		SetSpellUIAppear(nIdx);
	}
}

//========================================================================
// 時計を出現状態にする
//========================================================================
void SetClockAppear(int nIdx)
{
	g_aUIManager[nIdx].bPause = true;
	g_aUIManager[nIdx].state = UISTATE_CLOCKAPPEAR;
	g_aUIManager[nIdx].nKey = 0;
	g_aUIManager[nIdx].aUITexture[UITEX_BG].fHeight = 0.0f;			// 高さを管理
	g_aUIManager[nIdx].aUITexture[UITEX_BG].fHeightDest = PHONE_HEIGHT;	// 高さの目的値を設定
	g_aUIManager[nIdx].aUITexture[UITEX_BG].bDisp = true;			// 表示状態の管理
	g_aUIManager[nIdx].aUITexture[UITEX_FILTER].fHeight = 0.0f;			// 高さを管理
	g_aUIManager[nIdx].aUITexture[UITEX_FILTER].fHeightDest = PHONE_HEIGHT;	// 高さの目的値を設定
	g_aUIManager[nIdx].aUITexture[UITEX_FILTER].bDisp = true;			// 表示状態の管理
	g_aUIManager[nIdx].aUITexture[UITEX_BGFILTER].fHeight = 0.0f;			// 高さを管理
	g_aUIManager[nIdx].aUITexture[UITEX_BGFILTER].fHeightDest = PHONE_HEIGHT;	// 高さの目的値を設定
	g_aUIManager[nIdx].aUITexture[UITEX_BGFILTER].bDisp = true;			// 表示状態の管理
}

//========================================================================
// 時計を表示状態にする
//========================================================================
void SetClockMenu(int nIdx)
{
	SetBattery(nIdx);
	SetClock(nIdx);
	g_aUIManager[nIdx].nKey = 0;
	g_aUIManager[nIdx].state = UISTATE_CLOCK;
	g_aUIManager[nIdx].aUITexture[UITEX_CLOCKMENU].bDisp = true;
	g_aUIManager[nIdx].aUITexture[UITEX_BG].bDisp = true;				// 表示状態の管理
	g_aUIManager[nIdx].aUITexture[UITEX_FILTER].bDisp = true;			// 表示状態の管理
	g_aUIManager[nIdx].aUITexture[UITEX_BGFILTER].bDisp = true;			// 表示状態の管理
	g_aUIManager[nIdx].aUITexture[UITEX_BATTERY].bDisp = true;			// 表示状態の管理
	g_aUIManager[nIdx].aUITexture[UITEX_BATTERYFRAME].bDisp = true;			// 表示状態の管理
}

//========================================================================
// 時計を消滅状態にする
//========================================================================
void SetClockDissapear(int nIdx)
{
	DissapearBattery(nIdx);
	DisappearClock(nIdx);
	g_aUIManager[nIdx].aUITexture[UITEX_BATTERY].bDisp = false;			// 表示状態の管理
	g_aUIManager[nIdx].aUITexture[UITEX_BATTERYFRAME].bDisp = false;			// 表示状態の管理
	g_aUIManager[nIdx].aUITexture[UITEX_CLOCKMENU].bDisp = false;
	g_aUIManager[nIdx].state = UISTATE_CLOCKDISAPPEAR;
	g_aUIManager[nIdx].nKey = 0;
	g_aUIManager[nIdx].aUITexture[UITEX_BG].bDisp = true;			// 表示状態の管理
	g_aUIManager[nIdx].aUITexture[UITEX_FILTER].bDisp = true;			// 表示状態の管理
	g_aUIManager[nIdx].aUITexture[UITEX_BGFILTER].bDisp = true;			// 表示状態の管理
	g_aUIManager[nIdx].aUITexture[UITEX_BG].fHeight = PHONE_HEIGHT;			// 高さを管理
	g_aUIManager[nIdx].aUITexture[UITEX_FILTER].fHeight = PHONE_HEIGHT;			// 高さを管理
	g_aUIManager[nIdx].aUITexture[UITEX_BGFILTER].fHeight = PHONE_HEIGHT;			// 高さを管理
	g_aUIManager[nIdx].aUITexture[UITEX_BG].fHeightDest = 0.0f;	// 高さの目的値を設定
	g_aUIManager[nIdx].aUITexture[UITEX_FILTER].fHeightDest = 0.0f;	// 高さの目的値を設定
	g_aUIManager[nIdx].aUITexture[UITEX_BGFILTER].fHeightDest = 0.0f;	// 高さの目的値を設定
}

//========================================================================
// 時計を非表示状態にする
//========================================================================
void SetClockNonDisp(int nIdx)
{
	for (int nCntUI = 0; nCntUI < MAXUI_TEX; nCntUI++)
	{
		g_aUIManager[nIdx].aUITexture[nCntUI].fWidth = PHONE_WIDTH;	// 幅
		g_aUIManager[nIdx].aUITexture[nCntUI].fHeight = 0.0f;	// 高さ
		g_aUIManager[nIdx].aUITexture[nCntUI].fWidthDest = PHONE_WIDTH;		// 幅の目的地
		g_aUIManager[nIdx].aUITexture[nCntUI].fHeightDest = PHONE_HEIGHT;		// 高さの目的地
		g_aUIManager[nIdx].aUITexture[nCntUI].bDisp = false;		// 表示状態
		g_aUIManager[nIdx].state = UISTATE_NONDISPLAY;					// 選択状態
	}
	g_aUIManager[nIdx].nKey = 0;
	g_aUIManager[nIdx].bPause = false;
}
