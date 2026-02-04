////=============================================================================
////
////	ポーズ処理 [pause.cpp]
////	Author : SHUMA AIZU
//// 
////=============================================================================
//
//#include "main.h"
//#include "pause.h"
//#include "input.h"
//#include "game.h"
//#include "time.h"
//#include "fade.h"
//
//#include "player.h"
//#include "gameui.h"
//#include "magicui.h"
//#include "spellui.h"
//#include "clock.h"
//
////*****************************************************************************
//// マクロ定義
////*****************************************************************************
//#define PAUSEMENU_WIDTH		(120.0f)			// ポーズメニューの幅
//#define PAUSEMENU_HEIGHT	(30.0f)				// ポーズメニューの高さ
//#define PAUSEMENU_POSX		(1100.0f)			// ポーズメニューの位置
//#define PAUSEMENU_POSY		(320.0f)			// ポーズメニューの位置
//
////*****************************************************************************
//// グローバル変数
////*****************************************************************************
//LPDIRECT3DTEXTURE9 g_apTexturePause[PAUSE_MENU_MAX] = {};		// テクスチャへのポインタ
//LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffPause = NULL;					// 頂点バッファへのポインタ
//PAUSE_MENU g_apauseMenu[MAX_PLAYER] = {};						// ポーズメニューの状態
//PAUSE_MODE g_apauseMode[MAX_PLAYER] = {};						// ポーズモードの状態
//bool g_aisPause[MAX_PLAYER];									// ポーズしているかどうか
//bool g_bPauseDisp = true;										// ポーズ画面表示状態
//
//PauseMenuInfo g_aPauseMenuInfo[PAUSE_MENU_MAX] =
//{
//	{{ PAUSEMENU_POSX, PAUSEMENU_POSY, 0.0f }, { 0.0f, 0.0f, 1.0f * D3DX_PI }, PAUSEMENU_WIDTH, PAUSEMENU_HEIGHT, 0.0f, 0.0f },
//	{{ PAUSEMENU_POSX, PAUSEMENU_POSY + PAUSEMENU_HEIGHT * 3, 0.0f }, { 0.0f, 0.0f, 1.0f * D3DX_PI }, PAUSEMENU_WIDTH, PAUSEMENU_HEIGHT, 0.0f, 0.0f },
//	{{ PAUSEMENU_POSX, PAUSEMENU_POSY + PAUSEMENU_HEIGHT * 6, 0.0f}, {0.0f, 0.0f, 1.0f * D3DX_PI}, PAUSEMENU_WIDTH, PAUSEMENU_HEIGHT, 0.0f, 0.0f},
//	{{ PAUSEMENU_POSX, PAUSEMENU_POSY + PAUSEMENU_HEIGHT * 9, 0.0f }, { 0.0f, 0.0f, 1.0f * D3DX_PI }, PAUSEMENU_WIDTH, PAUSEMENU_HEIGHT, 0.0f, 0.0f },
//	{{ PAUSEMENU_POSX, PAUSEMENU_POSY + PAUSEMENU_HEIGHT * 12, 0.0f }, { 0.0f, 0.0f, 1.0f * D3DX_PI }, PAUSEMENU_WIDTH, PAUSEMENU_HEIGHT, 0.0f, 0.0f },
//};
//
////=============================================================================
////	ポーズメニューの初期化処理
////=============================================================================
//void InitPause(void)
//{
//	const char* pPauseMenuFileName[] =
//	{
//		"data\\TEXTURE\\continue000.png",
//		"data\\TEXTURE\\continue000.png",
//		"data\\TEXTURE\\continue000.png",
//		"data\\TEXTURE\\restart000.png",
//		"data\\TEXTURE\\quit000.png"
//	};
//
//	// デバイスの取得
//	LPDIRECT3DDEVICE9 pDevice = GetDevice();
//
//	for (int nCntPause = 0; nCntPause < PAUSE_MENU_MAX; nCntPause++)
//	{
//		// テクスチャの読み込み
//		D3DXCreateTextureFromFile(pDevice,
//			pPauseMenuFileName[nCntPause],
//			&g_apTexturePause[nCntPause]);
//	}
//
//
//	// 頂点バッファの生成
//	pDevice->CreateVertexBuffer(sizeof(VERTEX_2D) * 4 * PAUSE_MENU_MAX * MAX_PLAYER,
//		D3DUSAGE_WRITEONLY,
//		FVF_VERTEX_2D,
//		D3DPOOL_MANAGED,
//		&g_pVtxBuffPause,
//		NULL);
//
//	// 初期化
//	for (int nCntPlayer = 0; nCntPlayer < MAX_PLAYER; nCntPlayer++)
//	{
//		g_apauseMenu[nCntPlayer] = PAUSE_MENU_CLOCK;
//		g_apauseMode[nCntPlayer] = PAUSE_MODE_NEUTRAL;
//		g_aisPause[nCntPlayer] = false;
//	}
//	g_bPauseDisp = true;
//
//	PauseMenuInfo* pPauseMenuInfo = &g_aPauseMenuInfo[0];
//
//	VERTEX_2D *pVtx;			// 頂点情報へのポインタ
//
//	// 頂点バッファをロックし,頂点情報へのポインタを取得
//	g_pVtxBuffPause->Lock(0, 0, (void * *)&pVtx, 0);
//
//	for (int nCntPause = 0; nCntPause < PAUSE_MENU_MAX; nCntPause++, pPauseMenuInfo++)
//	{
//		pPauseMenuInfo->fLength = SQRTF(pPauseMenuInfo->fWidth, pPauseMenuInfo->fHeight);
//		pPauseMenuInfo->fAngle = atan2f(pPauseMenuInfo->fWidth, pPauseMenuInfo->fHeight);
//
//		// 頂点座標の設定
//		pVtx[0].pos = D3DXVECTOR3(pPauseMenuInfo->pos.x - pPauseMenuInfo->fWidth, pPauseMenuInfo->pos.y - pPauseMenuInfo->fHeight, 0.0f);
//		pVtx[1].pos = D3DXVECTOR3(pPauseMenuInfo->pos.x + pPauseMenuInfo->fWidth, pPauseMenuInfo->pos.y - pPauseMenuInfo->fHeight, 0.0f);
//		pVtx[2].pos = D3DXVECTOR3(pPauseMenuInfo->pos.x - pPauseMenuInfo->fWidth, pPauseMenuInfo->pos.y + pPauseMenuInfo->fHeight, 0.0f);
//		pVtx[3].pos = D3DXVECTOR3(pPauseMenuInfo->pos.x + pPauseMenuInfo->fWidth, pPauseMenuInfo->pos.y + pPauseMenuInfo->fHeight, 0.0f);
//
//		// rhwの設定
//		pVtx[0].rhw = 1.0f;
//		pVtx[1].rhw = 1.0f;
//		pVtx[2].rhw = 1.0f;
//		pVtx[3].rhw = 1.0f;
//
//		if (nCntPause == g_apauseMenu[0])
//		{ // 選択されていれば不透明度を戻す
//			// 頂点カラーの設定
//			pVtx[0].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
//			pVtx[1].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
//			pVtx[2].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
//			pVtx[3].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
//		}
//		else
//		{ // 選択されていなければ不透明度を下げる
//			// 頂点カラーの設定
//			pVtx[0].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.5f);
//			pVtx[1].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.5f);
//			pVtx[2].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.5f);
//			pVtx[3].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.5f);
//		}
//
//		// テクスチャ座標の設定
//		pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
//		pVtx[1].tex = D3DXVECTOR2(1.0f, 0.0f);
//		pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
//		pVtx[3].tex = D3DXVECTOR2(1.0f, 1.0f);
//
//		pVtx += 4;
//	}
//
//	// 頂点バッファをアンロックする
//	g_pVtxBuffPause->Unlock();
//}
//
////=============================================================================
////	ポーズメニューの終了処理
////=============================================================================
//void UninitPause(void)
//{
//	// テクスチャの破棄
//	for (int nCntPause = 0; nCntPause < PAUSE_MENU_MAX; nCntPause++)
//	{
//		if (g_apTexturePause[nCntPause] != NULL)
//		{
//			g_apTexturePause[nCntPause]->Release();
//			g_apTexturePause[nCntPause] = NULL;
//		}
//	}
//
//	// 頂点バッファの破棄
//	if (g_pVtxBuffPause != NULL)
//	{
//		g_pVtxBuffPause->Release();
//		g_pVtxBuffPause = NULL;
//	}
//}
//
////=============================================================================
////	ポーズメニューの描画処理
////=============================================================================
//void DrawPause(void)
//{
//	LPDIRECT3DDEVICE9 pDevice;				// デバイスへのポインタ
//
//	for (int nCntPlayer = 0; nCntPlayer < MAX_PLAYER; nCntPlayer++)
//	{
//		if (g_aisPause[nCntPlayer] == false)
//		{
//			continue;
//		}
//
//		if (g_apauseMode[nCntPlayer] != PAUSE_MODE_NEUTRAL)
//		{
//			continue;
//		}
//
//		// デバイスの取得
//		pDevice = GetDevice();
//
//		// 頂点バッファをデータストリームに設定
//		pDevice->SetStreamSource(0, g_pVtxBuffPause, 0, sizeof(VERTEX_2D));
//
//		// 頂点フォーマットの設定
//		pDevice->SetFVF(FVF_VERTEX_2D);
//
//		if (g_bPauseDisp == true)
//		{
//			for (int nCntPause = 0; nCntPause < PAUSE_MENU_MAX; nCntPause++)
//			{
//				// テクスチャの設定
//				pDevice->SetTexture(0, g_apTexturePause[nCntPause]);
//
//				// ポリゴンの描画
//				pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, nCntPause * 4, 2);
//			}
//		}
//	}
//}
//
////=============================================================================
////	ポーズメニューの更新処理
////=============================================================================
//void UpdatePause(void)
//{
//	for (int nCntPlayer = 0; nCntPlayer < MAX_PLAYER; nCntPlayer++)
//	{
//		if (g_apauseMode[nCntPlayer] != PAUSE_MODE_NEUTRAL)
//		{
//			if (GetJoypadTrigger(JOYKEY_B, 0) == true)
//			{
//				g_apauseMode[nCntPlayer] = PAUSE_MODE_NEUTRAL;
//				DisappearMagicUI(0);
//				DisappearClock(nCntPlayer);
//			}
//
//			continue;
//		}
//
//		if (GetJoypadTrigger(JOYKEY_START, nCntPlayer) == true)
//		{
//			g_aisPause[nCntPlayer] = g_aisPause[nCntPlayer] ? false : true;
//		}
//
//		if (g_aisPause[nCntPlayer] == false)
//		{
//			continue;
//		}
//
//		if (GetJoypadRepeat(JOYKEY_UP, 0) == true || GetKeyboardRepeat(DIK_W) == true || GetJoypadStickRepeatL(JOYSTICK_UP, 0) == true)
//		{ // 上方向キーが押されたら
//			// 現在のモードに合わせて変更
//			switch (g_apauseMenu[nCntPlayer])
//			{
//			case PAUSE_MENU_CLOCK:
//				g_apauseMenu[nCntPlayer] = PAUSE_MENU_QUIT;
//				break;
//
//			case PAUSE_MENU_MAGICBOOK:
//				g_apauseMenu[nCntPlayer] = PAUSE_MENU_CLOCK;
//				break;
//
//			case PAUSE_MENU_CONTINUE:
//				g_apauseMenu[nCntPlayer] = PAUSE_MENU_MAGICBOOK;
//				break;
//
//			case PAUSE_MENU_RESTART:
//				g_apauseMenu[nCntPlayer] = PAUSE_MENU_CONTINUE;
//				break;
//
//			case PAUSE_MENU_QUIT:
//				g_apauseMenu[nCntPlayer] = PAUSE_MENU_RESTART;
//				break;
//			}
//		}
//
//		if (GetJoypadRepeat(JOYKEY_DOWN, 0) == true || GetKeyboardRepeat(DIK_S) == true || GetJoypadStickRepeatL(JOYSTICK_DOWN, 0) == true)
//		{ // 下方向キーが押されたら
//			// 現在のモードに合わせて変更
//			switch (g_apauseMenu[nCntPlayer])
//			{
//			case PAUSE_MENU_CLOCK:
//				g_apauseMenu[nCntPlayer] = PAUSE_MENU_MAGICBOOK;
//				break;
//
//			case PAUSE_MENU_MAGICBOOK:
//				g_apauseMenu[nCntPlayer] = PAUSE_MENU_CONTINUE;
//				break;
//
//			case PAUSE_MENU_CONTINUE:
//				g_apauseMenu[nCntPlayer] = PAUSE_MENU_RESTART;
//				break;
//
//			case PAUSE_MENU_RESTART:
//				g_apauseMenu[nCntPlayer] = PAUSE_MENU_QUIT;
//				break;
//
//			case PAUSE_MENU_QUIT:
//				g_apauseMenu[nCntPlayer] = PAUSE_MENU_CLOCK;
//				break;
//			}
//		}
//
//		if (GetJoypadTrigger(JOYKEY_A, 0) == true || GetKeyboardTrigger(DIK_RETURN) == true)
//		{ // 決定キーが押されたら
//			// 現在のモードに合わせて変更
//			switch (g_apauseMenu[nCntPlayer])
//			{
//			case PAUSE_MENU_CLOCK:
//				g_apauseMode[nCntPlayer] = PAUSE_MODE_CLOCK;
//				SetClock(nCntPlayer, g_aPauseMenuInfo[PAUSE_MENU_CLOCK].pos);
//				break;
//
//			case PAUSE_MENU_MAGICBOOK:
//				g_apauseMode[nCntPlayer] = PAUSE_MODE_MAGICBOOK;
//				SetMagicUI(0);
//				break;
//
//			case PAUSE_MENU_CONTINUE:
//				g_aisPause[nCntPlayer] = false;
//				break;
//
//			case PAUSE_MENU_RESTART:
//				SetFade(MODE_GAME);
//				break;
//
//			case PAUSE_MENU_QUIT:
//				SetFade(MODE_TITLE);
//				break;
//			}
//		}
//
//		if (GetKeyboardTrigger(DIK_F5) == true)
//		{
//			g_bPauseDisp = g_bPauseDisp ? false : true;
//		}
//	}
//}
//
////=============================================================================
////	ポーズメニューの設定処理
////=============================================================================
//void SetPauseMenu(PAUSE_MENU pause_menu)
//{
//	g_apauseMenu[0] = pause_menu;
//}
//
////=============================================================================
////	ポーズメニューの表示状態取得処理
////=============================================================================
//bool GetPauseDisp(void)
//{
//	return g_bPauseDisp;
//}