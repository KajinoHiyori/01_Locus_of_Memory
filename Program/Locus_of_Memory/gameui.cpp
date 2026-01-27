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
#define TEX_PAUSE		(GAMEUI_TYPE_MAX)	// ポーズメニューで使うテクスチャ数
#define PAUSE_NUMBER	(5)					// ポーズメニューの数
#define PAUSE_SIZE		(20.0f)				// ポーズのポリゴンサイズ
#define PAUSE_SPACE		(50.0f)				// ポーズの間隔
#define PAUSE_POS		(100.0f)			// ポーズの開始位置X
#define PAUSE_LEFT		(100.0f)			// ポーズの左端
#define PAUSE_RIGHT		(200.0f)			// ポーズの右端
#define PAUSE_MAEGIN	(20.0f)				// ポーズ背景分の余白
#define PRESS_MAEGIN	(-15)				// トリガーとリピートの間隔をあける
#define PRESS_INTERVAL	(20)				// リピートの時の間隔
#define PAUSE_BG_ABOVE	(110.0f)			// ポーズ背景の上端
#define PAUSE_BG_BELOW	(610.0f)			// ポーズ背景の下端
#define VIBRATION_POWER	(500)				// バイブレーションの強度
#define VIBRATION_TIME	(10)				// バイブレーションの継続時間

// グローバル変数
LPDIRECT3DTEXTURE9 g_apTextureGameUI[TEX_PAUSE] = {};	// テクスチャへのポインタ
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffGameUI = NULL;			// 頂点バッファへのポインタ
GAMEUI_TYPE g_pauseMenu;									// ポーズの情報
int g_selectGameUI = 0;									// 選択されているポーズ状態

const char* c_apFilenameGameUI[TEX_PAUSE] =
{
	"data\\TEXTURE\\pause000.png",
	"data\\TEXTURE\\pause001.png",
	"data\\TEXTURE\\pause002.png",
	"data\\TEXTURE\\pause100.png",
	"data\\TEXTURE\\pause101.png",
	"data\\TEXTURE\\pause101.png",
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
	for (int nCntGameUI = 0; nCntGameUI < TEX_PAUSE; nCntGameUI++)
	{
		D3DXCreateTextureFromFile(pDevice, c_apFilenameGameUI[nCntGameUI], &g_apTextureGameUI[nCntGameUI]);
	}
	
	// 頂点バッファの生成
	pDevice->CreateVertexBuffer(sizeof(VERTEX_2D) * 4 * TEX_PAUSE,
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_2D,
		D3DPOOL_MANAGED,
		&g_pVtxBuffGameUI,
		NULL);

	VERTEX_2D* pVtx;
	// 頂点バッファをロックし、頂点情報へのポインタを取得
	g_pVtxBuffGameUI->Lock(0, 0, (void**)&pVtx, 0);

	for (int nCntGameUI = 0; nCntGameUI < PAUSE_NUMBER; nCntGameUI++)
	{
		// 頂点座標の設定
		pVtx[0].pos = D3DXVECTOR3(PAUSE_LEFT, (PAUSE_POS + nCntGameUI * PAUSE_SPACE) - PAUSE_SIZE, 0.0f);
		pVtx[1].pos = D3DXVECTOR3(PAUSE_RIGHT, (PAUSE_POS + nCntGameUI * PAUSE_SPACE) - PAUSE_SIZE, 0.0f);
		pVtx[2].pos = D3DXVECTOR3(PAUSE_LEFT, (PAUSE_POS + nCntGameUI * PAUSE_SPACE) + PAUSE_SIZE, 0.0f);
		pVtx[3].pos = D3DXVECTOR3(PAUSE_RIGHT, (PAUSE_POS + nCntGameUI * PAUSE_SPACE) + PAUSE_SIZE, 0.0f);

		// rhwの設定
		pVtx[0].rhw = 1.0f;
		pVtx[1].rhw = 1.0f;
		pVtx[2].rhw = 1.0f;
		pVtx[3].rhw = 1.0f;

		// 頂点カラーの設定
		pVtx[0].col = COLOR_WHITE;
		pVtx[1].col = COLOR_WHITE;
		pVtx[2].col = COLOR_WHITE;
		pVtx[3].col = COLOR_WHITE;

		// テクスチャ座標の設定
		pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
		pVtx[1].tex = D3DXVECTOR2(1.0f, 0.0f);
		pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
		pVtx[3].tex = D3DXVECTOR2(1.0f, 1.0f);

		pVtx += 4;
	}

	// ポーズ背景の設定
	// 頂点座標の設定
	pVtx[0].pos = D3DXVECTOR3(PAUSE_LEFT - PAUSE_MAEGIN, PAUSE_BG_ABOVE, 0.0f);
	pVtx[1].pos = D3DXVECTOR3(PAUSE_RIGHT + PAUSE_MAEGIN, PAUSE_BG_ABOVE, 0.0f);
	pVtx[2].pos = D3DXVECTOR3(PAUSE_LEFT - PAUSE_MAEGIN, PAUSE_BG_BELOW, 0.0f);
	pVtx[3].pos = D3DXVECTOR3(PAUSE_RIGHT + PAUSE_MAEGIN, PAUSE_BG_BELOW, 0.0f);

	// rhwの設定
	pVtx[0].rhw = 1.0f;
	pVtx[1].rhw = 1.0f;
	pVtx[2].rhw = 1.0f;
	pVtx[3].rhw = 1.0f;

	// 頂点カラーの設定
	pVtx[0].col = COLOR_WHITE;
	pVtx[1].col = COLOR_WHITE;
	pVtx[2].col = COLOR_WHITE;
	pVtx[3].col = COLOR_WHITE;

	// テクスチャ座標の設定
	pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
	pVtx[1].tex = D3DXVECTOR2(1.0f, 0.0f);
	pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
	pVtx[3].tex = D3DXVECTOR2(1.0f, 1.0f);

	pVtx += 4;

	// 背景を暗くする
	// 頂点座標の設定
	pVtx[0].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	pVtx[1].pos = D3DXVECTOR3(100.0f, 0.0f, 0.0f);
	pVtx[2].pos = D3DXVECTOR3(0.0f, 100.0f, 0.0f);
	pVtx[3].pos = D3DXVECTOR3(100.0f, 100.0f, 0.0f);

	// rhwの設定
	pVtx[0].rhw = 1.0f;
	pVtx[1].rhw = 1.0f;
	pVtx[2].rhw = 1.0f;
	pVtx[3].rhw = 1.0f;

	// 頂点カラーの設定
	pVtx[0].col = COLOR_WHITE;
	pVtx[1].col = COLOR_WHITE;
	pVtx[2].col = COLOR_WHITE;
	pVtx[3].col = COLOR_WHITE;

	// テクスチャ座標の設定
	pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
	pVtx[1].tex = D3DXVECTOR2(1.0f, 0.0f);
	pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
	pVtx[3].tex = D3DXVECTOR2(1.0f, 1.0f);

	// 頂点バッファをアンロック
	g_pVtxBuffGameUI->Unlock();

	// ポーズ状態の初期化
	g_pauseMenu = GAMEUI_TYPE_CLOCK;
	g_selectGameUI = 0;
}

//======================================================================================
// ポーズの終了処理
//======================================================================================
void UninitGameUI(void)
{
	// テクスチャの破棄
	for (int nCntGameUI = 0; nCntGameUI < TEX_PAUSE; nCntGameUI++)
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
		g_selectGameUI--;
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
	for (int nCntGameUI = 0; nCntGameUI < PAUSE_NUMBER; nCntGameUI++)
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
}

//======================================================================================
// ポーズの描画処理
//======================================================================================
void DrawGameUI(void)
{
	OPERATIONTYPE operationType = GetOperationType();
	LPDIRECT3DDEVICE9 pDevice;	// デバイスへのポインタ
	// デバイスの取得
	pDevice = GetDevice();

	// 頂点バッファをデータストリームに設定
	pDevice->SetStreamSource(0, g_pVtxBuffGameUI, 0, sizeof(VERTEX_2D));

	// 頂点フォーマットの設定
	pDevice->SetFVF(FVF_VERTEX_2D);

	// テクスチャの設定
	if (operationType == OPERATIONTYPE_2P)
	{
		pDevice->SetTexture(0, g_apTextureGameUI[GAMEUI_TYPE_1PALPHA]);
		// ポリゴンの描画
		pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, GAMEUI_TYPE_1PALPHA * 4, 2);
	}
	else
	{
		pDevice->SetTexture(0, g_apTextureGameUI[GAMEUI_TYPE_2PALPHA]);
		pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, GAMEUI_TYPE_1PALPHA * 4, 2);
	}

	// テクスチャの設定
	pDevice->SetTexture(0, g_apTextureGameUI[GAMEUI_TYPE_PHONE]);

	// ポリゴンの描画
	pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, GAMEUI_TYPE_PHONE * 4, 2);

	for (int nCntGameUI = 0; nCntGameUI < PAUSE_NUMBER; nCntGameUI++)
	{
		// テクスチャの設定
		pDevice->SetTexture(0, g_apTextureGameUI[nCntGameUI]);

		// ポリゴンの描画
		pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, nCntGameUI * 4, 2);
	}
}

//======================================================================================
// ポーズの描画処理
//======================================================================================
void SetGameUI(int pauseMenu)
{
	g_selectGameUI = pauseMenu;
}
