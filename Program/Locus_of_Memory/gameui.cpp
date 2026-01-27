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
#define TEX_PAUSE		(PAUSE_MENU_MAX)	// ポーズメニューで使うテクスチャ数
#define PAUSE_NUMBER	(3)					// ポーズメニューの数
#define PAUSE_SIZE		(20.0f)				// ポーズのポリゴンサイズ
#define PAUSE_SPACE		(50.0f)				// ポーズの間隔
#define PAUSE_POS		(310.0f)			// ポーズの開始位置X
#define PAUSE_LEFT		(490.0f)			// ポーズの左端
#define PAUSE_RIGHT		(790.0f)			// ポーズの右端
#define PAUSE_MAEGIN	(20.0f)				// ポーズ背景分の余白
#define PRESS_MAEGIN	(-15)				// トリガーとリピートの間隔をあける
#define PRESS_INTERVAL	(20)				// リピートの時の間隔
#define PAUSE_BG_ABOVE	(110.0f)			// ポーズ背景の上端
#define PAUSE_BG_BELOW	(610.0f)			// ポーズ背景の下端
#define VIBRATION_POWER	(500)				// バイブレーションの強度
#define VIBRATION_TIME	(10)				// バイブレーションの継続時間

// グローバル変数
LPDIRECT3DTEXTURE9 g_apTexturePause[TEX_PAUSE] = {};	// テクスチャへのポインタ
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffPause = NULL;			// 頂点バッファへのポインタ
PAUSE_MENU g_pauseMenu;									// ポーズの情報
int g_selectPause = 0;									// 選択されているポーズ状態

const char* c_apFilenamePause[TEX_PAUSE] =
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
void InitPause(void)
{
	LPDIRECT3DDEVICE9 pDevice;	// デバイスへのポインタ
	// デバイスの取得
	pDevice = GetDevice();

	// テクスチャの読み込み
	for (int nCntPause = 0; nCntPause < TEX_PAUSE; nCntPause++)
	{
		D3DXCreateTextureFromFile(pDevice, c_apFilenamePause[nCntPause], &g_apTexturePause[nCntPause]);
	}
	
	// 頂点バッファの生成
	pDevice->CreateVertexBuffer(sizeof(VERTEX_2D) * 4 * TEX_PAUSE,
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_2D,
		D3DPOOL_MANAGED,
		&g_pVtxBuffPause,
		NULL);

	VERTEX_2D* pVtx;
	// 頂点バッファをロックし、頂点情報へのポインタを取得
	g_pVtxBuffPause->Lock(0, 0, (void**)&pVtx, 0);

	for (int nCntPause = 0; nCntPause < PAUSE_NUMBER; nCntPause++)
	{
		// 頂点座標の設定
		pVtx[0].pos = D3DXVECTOR3(PAUSE_LEFT, (PAUSE_POS + nCntPause * PAUSE_SPACE) - PAUSE_SIZE, 0.0f);
		pVtx[1].pos = D3DXVECTOR3(PAUSE_RIGHT, (PAUSE_POS + nCntPause * PAUSE_SPACE) - PAUSE_SIZE, 0.0f);
		pVtx[2].pos = D3DXVECTOR3(PAUSE_LEFT, (PAUSE_POS + nCntPause * PAUSE_SPACE) + PAUSE_SIZE, 0.0f);
		pVtx[3].pos = D3DXVECTOR3(PAUSE_RIGHT, (PAUSE_POS + nCntPause * PAUSE_SPACE) + PAUSE_SIZE, 0.0f);

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
	pVtx[1].pos = D3DXVECTOR3(SCREEN_WIDTH, 0.0f, 0.0f);
	pVtx[2].pos = D3DXVECTOR3(0.0f, SCREEN_HEIGHT, 0.0f);
	pVtx[3].pos = D3DXVECTOR3(SCREEN_WIDTH, SCREEN_HEIGHT, 0.0f);

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
	g_pVtxBuffPause->Unlock();

	// ポーズ状態の初期化
	g_pauseMenu = PAUSE_MENU_CLOCK;
	g_selectPause = 0;
}

//======================================================================================
// ポーズの終了処理
//======================================================================================
void UninitPause(void)
{
	// テクスチャの破棄
	for (int nCntPause = 0; nCntPause < TEX_PAUSE; nCntPause++)
	{
		if (g_apTexturePause[nCntPause] != NULL)
		{
			g_apTexturePause[nCntPause]->Release();
			g_apTexturePause[nCntPause] = NULL;
		}
	}

	// 頂点バッファの破棄
	if (g_pVtxBuffPause != NULL)
	{
		g_pVtxBuffPause->Release();
		g_pVtxBuffPause = NULL;
	}
}

//======================================================================================
// ポーズの更新処理
//======================================================================================
void UpdatePause(void)
{
#if 0
	// 入力情報の保存
	static int nCounterUp = 0;
	static int nCounterDown = 0;

	// 同時押しされた場合、処理を行わない
	if ((GetKeyboardPress(DIK_W) == true && GetKeyboardPress(DIK_S) == true) ||
		(GetJoypadPress(JOYKEY_UP) == true && GetJoypadPress(JOYKEY_DOWN) == true))
	{
		return;
	}

	// WSキーが押された場合、テクスチャの状態を変更する(トリガー)
	else if (GetKeyboardTrigger(DIK_W) == true || GetJoypadTrigger(JOYKEY_UP) == true || GetJoypadLeftStickTrigger(JOYKEY_LSTICK_UP) == true)	// 上
	{
		PlaySound(SE_SELECT);
		g_selectPause--;
		if (g_selectPause < 0)
		{
			g_selectPause = PAUSE_MENU_QUIT;
		}
		nCounterUp = PRESS_MAEGIN;
	}
	else if (GetKeyboardTrigger(DIK_S) == true || GetJoypadTrigger(JOYKEY_DOWN) == true || GetJoypadLeftStickTrigger(JOYKEY_LSTICK_DOWN) == true)	// 下
	{
		nCounterDown = PRESS_MAEGIN;
		PlaySound(SE_SELECT);
		g_selectPause++;
		if (g_selectPause > PAUSE_MENU_QUIT)
		{
			g_selectPause = PAUSE_MENU_CONTINUE;
		}
	}

	// WSキーが押された場合、テクスチャの状態を変更する(リピート)
	else if (GetKeyboardRepeat(DIK_W) == true || GetJoypadRepeat(JOYKEY_UP) == true)	// 上
	{
		PlaySound(SE_SELECT);
		g_selectPause--;
		if (g_selectPause < 0)
		{
			g_selectPause = PAUSE_MENU_QUIT;
		}
	}
	else if (GetKeyboardRepeat(DIK_S) == true || GetJoypadRepeat(JOYKEY_DOWN) == true)	// 下
	{
		PlaySound(SE_SELECT);
		g_selectPause++;
		if (g_selectPause > PAUSE_MENU_QUIT)
		{
			g_selectPause = PAUSE_MENU_CONTINUE;
		}
	}
	else if (GetJoypadLeftStick(JOYKEY_LSTICK_UP) == true)
	{
		nCounterUp++;
		if (nCounterUp >= PRESS_INTERVAL)
		{
			nCounterUp = 0;
			PlaySound(SE_SELECT);
			g_selectPause--;
			if (g_selectPause < 0)
			{
				g_selectPause = PAUSE_MENU_QUIT;
			}
		}
	}
	else if (GetJoypadLeftStick(JOYKEY_LSTICK_DOWN) == true)
	{
		nCounterDown++;
		if (nCounterDown >= PRESS_INTERVAL)
		{
			nCounterDown = 0;
			PlaySound(SE_SELECT);
			g_selectPause++;
			if (g_selectPause > PAUSE_MENU_QUIT)
			{
				g_selectPause = PAUSE_MENU_CONTINUE;
			}
		}
	}

	// 何が選ばれているかの判定
	switch (g_selectPause)
	{
	case PAUSE_MENU_CONTINUE:	// CONTINUE
		g_pauseMenu = PAUSE_MENU_CONTINUE;
		break;
	case PAUSE_MENU_RETRY:		// RETRY
		g_pauseMenu = PAUSE_MENU_RETRY;
		break;
	case PAUSE_MENU_QUIT:		// QUIT
		g_pauseMenu = PAUSE_MENU_QUIT;
		break;
	}

	// 頂点座標の更新
	VERTEX_2D* pVtx;
	// 頂点バッファをロックし、頂点情報へのポインタを取得
	g_pVtxBuffPause->Lock(0, 0, (void**)&pVtx, 0);
	for (int nCntPause = 0; nCntPause < PAUSE_NUMBER; nCntPause++)
	{
		if (nCntPause == g_pauseMenu)	// 選択部だけ明るく表示
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
	g_pVtxBuffPause->Unlock();

	// ENTERキーで確定した場合
	if (GetKeyboardTrigger(DIK_RETURN) == true || GetJoypadTrigger(JOYKEY_A) == true)
	{
		switch (g_pauseMenu)
		{
		case PAUSE_MENU_CONTINUE:	// コンテニュー
			SetEnablePause(false);
			break;
		case PAUSE_MENU_RETRY:		// リトライ
			SetFade(MODE_GAME);
			break;
		case PAUSE_MENU_QUIT:		// タイトルへ戻る
			SetFade(MODE_TITLE);
			break;
		}
	}
#endif
}

//======================================================================================
// ポーズの描画処理
//======================================================================================
void DrawPause(void)
{
	OPERATIONTYPE operationType = GetOperationType();
	LPDIRECT3DDEVICE9 pDevice;	// デバイスへのポインタ
	// デバイスの取得
	pDevice = GetDevice();

	// 頂点バッファをデータストリームに設定
	pDevice->SetStreamSource(0, g_pVtxBuffPause, 0, sizeof(VERTEX_2D));

	// 頂点フォーマットの設定
	pDevice->SetFVF(FVF_VERTEX_2D);

	// テクスチャの設定
	if (operationType == OPERATIONTYPE_2P)
	{
		pDevice->SetTexture(0, g_apTexturePause[PAUSE_MENU_1PALPHA]);
		// ポリゴンの描画
		pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, PAUSE_MENU_1PALPHA * 4, 2);
	}
	else
	{
		pDevice->SetTexture(0, g_apTexturePause[PAUSE_MENU_2PALPHA]);
		pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, PAUSE_MENU_1PALPHA * 4, 2);
	}

	// テクスチャの設定
	pDevice->SetTexture(0, g_apTexturePause[PAUSE_MENU_PHONE]);

	// ポリゴンの描画
	pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, PAUSE_MENU_PHONE * 4, 2);

	for (int nCntPause = 0; nCntPause < PAUSE_NUMBER; nCntPause++)
	{
		// テクスチャの設定
		pDevice->SetTexture(0, g_apTexturePause[nCntPause]);

		// ポリゴンの描画
		pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, nCntPause * 4, 2);
	}
}

//======================================================================================
// ポーズの描画処理
//======================================================================================
void SetPause(int pauseMenu)
{
	g_selectPause = pauseMenu;
}
