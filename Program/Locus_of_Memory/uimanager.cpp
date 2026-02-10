//======================================================================================
// 
// 3D空間用UI処理[uimanager.cpp]
// Author : KajinoHiyori
//
//======================================================================================
#include "uimanager.h"
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
	float		fFlickerX;		// X軸のちらつきを管理
	float		fFlickerZ;		// Z軸のちらつきを管理
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
	int			nSelect;	// 選択しているメニュー
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
//#if 0
	// デバイスの取得
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	// 操作人数の取得
	OPERATIONTYPE operationType = GetOperationType();

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
			g_aUIManager[nCntPlayer].aUITexture[nCntUI].pos			= D3DXVECTOR3(0.0f, GAMEUI_POSY, 0.0f);	// 中心位置
			g_aUIManager[nCntPlayer].aUITexture[nCntUI].col			= COLOR_WHITE;						// 色
			g_aUIManager[nCntPlayer].aUITexture[nCntUI].tex			= UITEX_BG;							// テクスチャの種類
			g_aUIManager[nCntPlayer].aUITexture[nCntUI].fWidth		= PHONE_WIDTH;						// 幅
			g_aUIManager[nCntPlayer].aUITexture[nCntUI].fHeight		= PHONE_HEIGHT;						// 高さ
			g_aUIManager[nCntPlayer].aUITexture[nCntUI].fWidthDest	= 0.0f;								// 幅の目的地
			g_aUIManager[nCntPlayer].aUITexture[nCntUI].fHeightDest	= 0.0f;								// 高さの目的地
			g_aUIManager[nCntPlayer].aUITexture[nCntUI].fFlickerX	= 0.0f;								// ちらつきを管理[X]
			g_aUIManager[nCntPlayer].aUITexture[nCntUI].fFlickerZ	= 0.0f;								// ちらつきを管理[Z]
			g_aUIManager[nCntPlayer].aUITexture[nCntUI].bDisp		= false;							// 表示状態
		}
		g_aUIManager[nCntPlayer].pos = D3DXVECTOR3(0.0f, PHONE_Y, -300.0f);	// 中心位置
		g_aUIManager[nCntPlayer].rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);	// 中心位置
		g_aUIManager[nCntPlayer].type = UITYPE_CLOCK;					// 選択している種類(type)
		g_aUIManager[nCntPlayer].state = UISTATE_SELECT;			// UIの表示状態
		g_aUIManager[nCntPlayer].nSelect = UITYPE_CLOCK;				// 選択している種類(int)
		g_aUIManager[nCntPlayer].bPause = false;						// ポーズ状態(trueでポーズ中)
	}

	// 頂点バッファの生成
	pDevice->CreateVertexBuffer(sizeof(VERTEX_3D) * 4 * MAXUI_TEX * MAX_PLAYER, D3DUSAGE_WRITEONLY, FVF_VERTEX_3D, D3DPOOL_MANAGED, &g_pVtxBuffUIManager, NULL);

	VERTEX_3D* pVtx;
	// 頂点バッファをロックし、頂点情報へのポインタを取得
	g_pVtxBuffUIManager->Lock(0, 0, (void**)&pVtx, 0);
	
	for (int nCntPlayer = 0; nCntPlayer < MAX_PLAYER; nCntPlayer++)
	{
		switch (operationType)
		{
		case OPERATIONTYPE_2P:	// 2人操作
			if (nCntPlayer == 0)
			{
			}
			break;
		default:	// 1人操作
			if (nCntPlayer == 0)
			{
				g_aUIManager[nCntPlayer].bPause = true;
			}
			else
			{
				pVtx += MAXUI_TEX * 4;
				continue;
			}
			break;
		}

		for (int nCntUI = 0; nCntUI < MAXUI_TEX; nCntUI++)
		{
			// 各種情報の設定
			switch (nCntUI)
			{
			case UITEX_BG:	// 背景
				g_aUIManager[nCntPlayer].aUITexture[nCntUI].col			= COLOR_DISALPHA;			// 色
				g_aUIManager[nCntPlayer].aUITexture[nCntUI].tex			= UITEX_BG;				// テクスチャの種類
				//g_aUIManager[nCntPlayer].aUITexture[nCntUI].fWidth		= 0.0f;				// 幅
				//g_aUIManager[nCntPlayer].aUITexture[nCntUI].fHeight		= 0.0f;				// 高さ
				g_aUIManager[nCntPlayer].aUITexture[nCntUI].fWidthDest	= 0.0f;					// 幅の目的地
				g_aUIManager[nCntPlayer].aUITexture[nCntUI].fHeightDest = 0.0f;					// 高さの目的地
				g_aUIManager[nCntPlayer].aUITexture[nCntUI].bDisp		= true;					// 表示状態
				g_aUIManager[nCntPlayer].aUITexture[nCntUI].fFlickerX	= 0.0f;	// ちらつきを軽減
				g_aUIManager[nCntPlayer].aUITexture[nCntUI].fFlickerZ	= 0.0f;	// ちらつきを軽減
				break;

			case UITEX_BATTERY:	// バッテリー
				g_aUIManager[nCntPlayer].aUITexture[nCntUI].col			= COLOR_WHITE;			// 色
				g_aUIManager[nCntPlayer].aUITexture[nCntUI].tex			= UITEX_BATTERY;		// テクスチャの種類
				g_aUIManager[nCntPlayer].aUITexture[nCntUI].bDisp		= true;					// 表示状態
				g_aUIManager[nCntPlayer].aUITexture[nCntUI].fFlickerX = sinf(fRotXCamera[nCntPlayer]) * FLICKER;	// ちらつきを軽減
				g_aUIManager[nCntPlayer].aUITexture[nCntUI].fFlickerZ = cosf(fRotYCamera[nCntPlayer]) * FLICKER;	// ちらつきを軽減
				break;

			case UITEX_BATTERYFRAME:	// バッテリーフレーム
				g_aUIManager[nCntPlayer].aUITexture[nCntUI].col			= COLOR_WHITE;			// 色
				g_aUIManager[nCntPlayer].aUITexture[nCntUI].tex			= UITEX_BATTERYFRAME;	// テクスチャの種類
				g_aUIManager[nCntPlayer].aUITexture[nCntUI].bDisp		= true;					// 表示状態
				g_aUIManager[nCntPlayer].aUITexture[nCntUI].fFlickerX = sinf(fRotXCamera[nCntPlayer]) * FLICKER * 1.5f;	// ちらつきを軽減
				g_aUIManager[nCntPlayer].aUITexture[nCntUI].fFlickerZ = cosf(fRotYCamera[nCntPlayer]) * FLICKER * 1.5f;	// ちらつきを軽減
				break;

			case UITEX_PAUSEMENU:	// ポーズメニュー
				g_aUIManager[nCntPlayer].aUITexture[nCntUI].col			= COLOR_WHITE;			// 色
				g_aUIManager[nCntPlayer].aUITexture[nCntUI].tex			= UITEX_PAUSEMENU;		// テクスチャの種類
				g_aUIManager[nCntPlayer].aUITexture[nCntUI].bDisp		= true;					// 表示状態
				g_aUIManager[nCntPlayer].aUITexture[nCntUI].fFlickerX = sinf(fRotXCamera[nCntPlayer]) * FLICKER * 1.5f;	// ちらつきを軽減
				g_aUIManager[nCntPlayer].aUITexture[nCntUI].fFlickerZ = cosf(fRotYCamera[nCntPlayer]) * FLICKER * 1.5f;	// ちらつきを軽減
				break;

			case UITEX_CLOCKMENU:	// 時計メニュー
				g_aUIManager[nCntPlayer].aUITexture[nCntUI].col			= COLOR_WHITE;			// 色
				g_aUIManager[nCntPlayer].aUITexture[nCntUI].tex			= UITEX_CLOCKMENU;		// テクスチャの種類
				g_aUIManager[nCntPlayer].aUITexture[nCntUI].bDisp		= false;					// 表示状態
				g_aUIManager[nCntPlayer].aUITexture[nCntUI].fFlickerX = sinf(fRotXCamera[nCntPlayer]) * FLICKER * 1.5f;	// ちらつきを軽減
				g_aUIManager[nCntPlayer].aUITexture[nCntUI].fFlickerZ = cosf(fRotYCamera[nCntPlayer]) * FLICKER * 1.5f;	// ちらつきを軽減
				break;

			case UITEX_CLOCK:	// 時計[選択状態]
				g_aUIManager[nCntPlayer].aUITexture[nCntUI].col			= COLOR_YELLOW;			// 色
				g_aUIManager[nCntPlayer].aUITexture[nCntUI].tex			= UITEX_CLOCK;			// テクスチャの種類
				g_aUIManager[nCntPlayer].aUITexture[nCntUI].bDisp		= true;					// 表示状態
				g_aUIManager[nCntPlayer].aUITexture[nCntUI].fFlickerX = sinf(fRotXCamera[nCntPlayer]) * FLICKER * 1.5f;	// ちらつきを軽減
				g_aUIManager[nCntPlayer].aUITexture[nCntUI].fFlickerZ = cosf(fRotYCamera[nCntPlayer]) * FLICKER * 1.5f;	// ちらつきを軽減
				break;

			case UITEX_CONTINUE:	// continue
				g_aUIManager[nCntPlayer].aUITexture[nCntUI].col			= COLOR_WHITE;			// 色
				g_aUIManager[nCntPlayer].aUITexture[nCntUI].tex			= UITEX_CONTINUE;		// テクスチャの種類
				g_aUIManager[nCntPlayer].aUITexture[nCntUI].bDisp		= true;					// 表示状態
				g_aUIManager[nCntPlayer].aUITexture[nCntUI].fFlickerX = sinf(fRotXCamera[nCntPlayer]) * FLICKER * 1.5f;	// ちらつきを軽減
				g_aUIManager[nCntPlayer].aUITexture[nCntUI].fFlickerZ = cosf(fRotYCamera[nCntPlayer]) * FLICKER * 1.5f;	// ちらつきを軽減
				break;

			case UITEX_RETRY:	// retry
				g_aUIManager[nCntPlayer].aUITexture[nCntUI].col			= COLOR_WHITE;			// 色
				g_aUIManager[nCntPlayer].aUITexture[nCntUI].tex			= UITEX_RETRY;			// テクスチャの種類
				g_aUIManager[nCntPlayer].aUITexture[nCntUI].bDisp		= true;					// 表示状態
				g_aUIManager[nCntPlayer].aUITexture[nCntUI].fFlickerX = sinf(fRotXCamera[nCntPlayer]) * FLICKER * 1.5f;	// ちらつきを軽減
				g_aUIManager[nCntPlayer].aUITexture[nCntUI].fFlickerZ = cosf(fRotYCamera[nCntPlayer]) * FLICKER * 1.5f;	// ちらつきを軽減
				break;

			case UITEX_QUIT:	// quit
				g_aUIManager[nCntPlayer].aUITexture[nCntUI].col			= COLOR_WHITE;			// 色
				g_aUIManager[nCntPlayer].aUITexture[nCntUI].tex			= UITEX_QUIT;			// テクスチャの種類
				g_aUIManager[nCntPlayer].aUITexture[nCntUI].bDisp		= true;					// 表示状態
				g_aUIManager[nCntPlayer].aUITexture[nCntUI].fFlickerX = sinf(fRotXCamera[nCntPlayer]) * FLICKER * 1.5f;	// ちらつきを軽減
				g_aUIManager[nCntPlayer].aUITexture[nCntUI].fFlickerZ = cosf(fRotYCamera[nCntPlayer]) * FLICKER * 1.5f;	// ちらつきを軽減
				break;

			case UITEX_FILTER:	// フィルター
				g_aUIManager[nCntPlayer].aUITexture[nCntUI].col			= COLOR_RETROFILTER;	// 色
				g_aUIManager[nCntPlayer].aUITexture[nCntUI].tex			= UITEX_FILTER;			// テクスチャの種類
				g_aUIManager[nCntPlayer].aUITexture[nCntUI].bDisp		= true;					// 表示状態
				g_aUIManager[nCntPlayer].aUITexture[nCntUI].fFlickerX = sinf(fRotXCamera[nCntPlayer]) * FLICKER * 2.0f;	// ちらつきを軽減
				g_aUIManager[nCntPlayer].aUITexture[nCntUI].fFlickerZ = cosf(fRotYCamera[nCntPlayer]) * FLICKER * 2.0f;	// ちらつきを軽減
				break;
			}

			// 中心位置からの位置を求める
			g_aUIManager[nCntPlayer].aUITexture[nCntUI].pos += g_aUIManager[nCntPlayer].pos;

			// 頂点座標の設定
			pVtx[0].pos = D3DXVECTOR3(-g_aUIManager[nCntPlayer].aUITexture[nCntUI].fWidth + g_aUIManager[nCntPlayer].aUITexture[nCntUI].fFlickerX, g_aUIManager[nCntPlayer].aUITexture[nCntUI].fHeight,		g_aUIManager[nCntPlayer].aUITexture[nCntUI].fFlickerZ);
			pVtx[1].pos = D3DXVECTOR3( g_aUIManager[nCntPlayer].aUITexture[nCntUI].fWidth + g_aUIManager[nCntPlayer].aUITexture[nCntUI].fFlickerX, g_aUIManager[nCntPlayer].aUITexture[nCntUI].fHeight,		g_aUIManager[nCntPlayer].aUITexture[nCntUI].fFlickerZ);
			pVtx[2].pos = D3DXVECTOR3(-g_aUIManager[nCntPlayer].aUITexture[nCntUI].fWidth + g_aUIManager[nCntPlayer].aUITexture[nCntUI].fFlickerX, -g_aUIManager[nCntPlayer].aUITexture[nCntUI].fHeight,	g_aUIManager[nCntPlayer].aUITexture[nCntUI].fFlickerZ);
			pVtx[3].pos = D3DXVECTOR3( g_aUIManager[nCntPlayer].aUITexture[nCntUI].fWidth + g_aUIManager[nCntPlayer].aUITexture[nCntUI].fFlickerX, -g_aUIManager[nCntPlayer].aUITexture[nCntUI].fHeight,	g_aUIManager[nCntPlayer].aUITexture[nCntUI].fFlickerZ);

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
			if (g_aUIManager[nCntPlayer].aUITexture[nCntUI].tex == UITEX_FILTER)
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

			pVtx += 4;
		}
	}
	// 頂点バッファをアンロック
	g_pVtxBuffUIManager->Unlock();
//#endif
}

//========================================================================
// UIの終了処理
//========================================================================
void UninitUIManager(void)
{
//#if 0
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
	FADE*	pFade	= GetFade();	// フェードの状態を取得
	D3DXVECTOR3 posOffset = D3DXVECTOR3(0.0f, 0.0f, 0.0f);	// オフセットの情報を初期化

	VERTEX_3D* pVtx;
	// 頂点バッファをロックし、頂点情報へのポインタを取得
	g_pVtxBuffUIManager->Lock(0, 0, (void**)&pVtx, 0);

	for (int nCntPlayer = 0; nCntPlayer < MAX_PLAYER; nCntPlayer++, pPlayer++)
	{
		if (g_aUIManager[nCntPlayer].state == UISTATE_SELECT)	// 選択メニューの場合
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

			for (int nCntSelect = UITEX_CLOCKMENU; nCntSelect < UITEX_FILTER; nCntSelect++)
			{
				if (g_aUIManager[nCntPlayer].nSelect +5 == nCntSelect)
				{
					g_aUIManager[nCntPlayer].aUITexture[nCntSelect].col = COLOR_YELLOW;
				}
				else
				{
					g_aUIManager[nCntPlayer].aUITexture[nCntSelect].col = COLOR_WHITE;
				}

			}

			switch (g_aUIManager[nCntPlayer].type)
			{
			case UITYPE_CLOCK:	// 時計状態を選択

				break;

			case UITYPE_CONTINUE:	// CONTINUEを選択

				break;

			case UITYPE_RETRY:	// RETRYを選択

				break;

			case UITYPE_QUIT:	// QUITを選択

				break;
			}
		}


		// プレイヤーの位置や向きに合わせてUIの表示位置を更新
		g_aUIManager[nCntPlayer].pos.x = -sinf(g_aUIManager[nCntPlayer].rot.y - 0.25f) * 100.0f + pPlayer->pos.x;
		g_aUIManager[nCntPlayer].pos.z = -cosf(g_aUIManager[nCntPlayer].rot.y - 0.25f) * 100.0f + pPlayer->pos.z;

		for (int nCntUI = 0; nCntUI < MAXUI_TEX; nCntUI++, pVtx += 4)
		{
			// オフセット情報の初期化
			g_aUIManager[nCntPlayer].aUITexture[nCntUI].pos = posOffset;

			// 中心位置からの位置を求める
			g_aUIManager[nCntPlayer].aUITexture[nCntUI].pos += g_aUIManager[nCntPlayer].pos;

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
	D3DXMATRIX mtxTrans;	// 計算用マトリックス
	D3DXMATRIX mtxView;		// ビューマトリックスの取得

	//// Zテストを無効にする
	//pDevice->SetRenderState(D3DRS_ZFUNC, D3DCMP_ALWAYS);	// Zテストの比較方法を変更(Zバッファの前後関係に関わらず描画する)
	//pDevice->SetRenderState(D3DRS_ZWRITEENABLE, FALSE);

	// アルファテストを有効にする
	pDevice->SetRenderState(D3DRS_ALPHATESTENABLE, TRUE);	// アルファテストを有効にする
	pDevice->SetRenderState(D3DRS_ALPHAFUNC, D3DCMP_GREATER);	// 比較方法を設定(基準値より大きい場合描画)
	pDevice->SetRenderState(D3DRS_ALPHAREF, 0);	// アルファテストの参照値を設定(この場合、0より大きい場合は描画)

	// ライトをオフにする
	pDevice->SetRenderState(D3DRS_LIGHTING, FALSE);

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
			if (g_aUIManager[nCntPlayer].aUITexture[nCntUI].tex == UITEX_FILTER)
			{
				pDevice->SetRenderState(D3DRS_BLENDOP, D3DBLENDOP_ADD);
				pDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
				pDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_ONE);
			}

			// ワールドマトリックスの初期化(デフォルトの値にする)
			D3DXMatrixIdentity(&g_aUIManager[nCntPlayer].aUITexture[nCntUI].mtxWorld);

			// ビューマトリックスを取得する
			pDevice->GetTransform(D3DTS_VIEW, &mtxView);

			// ポリゴンをカメラに対して正面に向ける
			D3DXMatrixInverse(&g_aUIManager[nCntPlayer].aUITexture[nCntUI].mtxWorld, NULL, &mtxView);	// 逆行列を求める
			g_aUIManager[nCntPlayer].aUITexture[nCntUI].mtxWorld._41 = 0.0f;
			g_aUIManager[nCntPlayer].aUITexture[nCntUI].mtxWorld._42 = 0.0f;
			g_aUIManager[nCntPlayer].aUITexture[nCntUI].mtxWorld._43 = 0.0f;

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
			if (g_aUIManager[nCntPlayer].aUITexture[nCntUI].tex == UITEX_FILTER)
			{
				//αブレンディングを戻す
				pDevice->SetRenderState(D3DRS_BLENDOP, D3DBLENDOP_ADD);
				pDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
				pDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);
			}
		}
	}

	//// Zテストを有効にする
	//pDevice->SetRenderState(D3DRS_ZFUNC, D3DCMP_LESSEQUAL);
	//pDevice->SetRenderState(D3DRS_ZWRITEENABLE, TRUE);

	// ライトをオンにする
	pDevice->SetRenderState(D3DRS_LIGHTING, TRUE);

	// アルファテストを無効にする
	pDevice->SetRenderState(D3DRS_ALPHATESTENABLE, FALSE);	// アルファテストを無効にする
	pDevice->SetRenderState(D3DRS_ALPHAFUNC, D3DCMP_ALWAYS);	// 比較方法を設定(条件に関わらず描画)
	pDevice->SetRenderState(D3DRS_ALPHAREF, 0);	// アルファテストの参照値を設定(この場合、0より大きい場合は描画)
//#endif

#if 0
	LPDIRECT3DDEVICE9 pDevice = GetDevice();	// デバイスの取得
	D3DXMATRIX mtxTrans;	// 計算用マトリックス
	D3DXMATRIX mtxView;		// ビューマトリックスの取得

	// Zテストを無効にする
	pDevice->SetRenderState(D3DRS_ZFUNC, D3DCMP_ALWAYS);	// Zテストの比較方法を変更(Zバッファの前後関係に関わらず描画する)
	pDevice->SetRenderState(D3DRS_ZWRITEENABLE, FALSE);		// Zバッファ更新の有効/無効の設定

	// アルファテストを有効にする
	pDevice->SetRenderState(D3DRS_ALPHATESTENABLE, TRUE);	// アルファテストを有効にする
	pDevice->SetRenderState(D3DRS_ALPHAFUNC, D3DCMP_GREATER);	// 比較方法を設定(基準値より大きい場合描画)
	pDevice->SetRenderState(D3DRS_ALPHAREF, 0);	// アルファテストの参照値を設定(この場合、0より大きい場合は描画)

	// ワールドマトリックスの初期化(デフォルトの値にする)
	D3DXMatrixIdentity(&g_billboard.mtxWorld);

	// ビューマトリックスを取得する
	pDevice->GetTransform(D3DTS_VIEW, &mtxView);

	// ポリゴンをカメラに対して正面に向ける
	D3DXMatrixInverse(&g_billboard.mtxWorld, NULL, &mtxView);	// 逆行列を求める
	g_billboard.mtxWorld._41 = 0.0f;
	g_billboard.mtxWorld._42 = 0.0f;
	g_billboard.mtxWorld._43 = 0.0f;

	// 位置を反映
	D3DXMatrixTranslation(&mtxTrans, g_billboard.pos.x, g_billboard.pos.y, g_billboard.pos.z);
	D3DXMatrixMultiply(&g_billboard.mtxWorld, &g_billboard.mtxWorld, &mtxTrans);

	// ワールドマトリックスの設定
	pDevice->SetTransform(D3DTS_WORLD, &g_billboard.mtxWorld);

	// 頂点バッファをデータストリームに設定
	pDevice->SetStreamSource(0, g_pVtxBuffBillBoard, 0, sizeof(VERTEX_3D));

	// 頂点フォーマットの設定
	pDevice->SetFVF(FVF_VERTEX_3D);

	// テクスチャの設定
	pDevice->SetTexture(0, g_pTextureBillBoard);

	// ビルボードの描画
	pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, 0, 2);

	// Zテストを有効にする
	pDevice->SetRenderState(D3DRS_ZFUNC, D3DCMP_LESSEQUAL);
	pDevice->SetRenderState(D3DRS_ZWRITEENABLE, TRUE);

	// アルファテストを無効にする
	pDevice->SetRenderState(D3DRS_ALPHATESTENABLE, FALSE);	// アルファテストを無効にする
	pDevice->SetRenderState(D3DRS_ALPHAFUNC, D3DCMP_ALWAYS);	// 比較方法を設定(条件に関わらず描画)
	pDevice->SetRenderState(D3DRS_ALPHAREF, 0);	// アルファテストの参照値を設定(この場合、0より大きい場合は描画)
#endif

}

//========================================================================
// ポーズ状態を管理
//========================================================================
bool GetPause(int nIdx)
{
	return g_aUIManager[nIdx].bPause;
}
