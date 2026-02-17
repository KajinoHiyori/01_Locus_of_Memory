//========================================================
// 
// 魔導書のUI表示処理[magicui.cpp]
// Author : KajinoHiyori
// 
//========================================================
#include "main.h"
#include "magicui.h"
#include "player.h"
#include "game.h"
#include "input.h"
#include "title.h"
#include "fog.h"
#include "debugproc.h"
#include "camera.h"

// マクロ定義
#define MAX_COMMAND			(4)					// 所持可能な魔法の最大数
#define NUM_KEY				(30)				// 処理を行うキー数
#define MAGICUI_WIDTH		(20.0f)			// MAGICUIの幅
#define MAGICUI_HEIGHT		(5.0f)			// MAGICUIの高さ
#define MAGICUI_X			(23.0f)				// MAGICUIのX軸
#define MAGICUI_YUP			(100.0f)			// MAGICUIのY高度
#define MAGICUI_YDOWN		(80.0f)				// MAGICUIのY高度
#define MAGICUI_Z			(40.0f)				// 魔導書の配置場所Z
#define MAXSPELL_TEX		(MAGICUI_TEX_MAX)	// テクスチャの最大数
#define MAXSPELL_TYPE		(MAGICUI_TYPE_MAX)	// 配置の最大数
#define NORMAL				(D3DXVECTOR3(0.0f, 1.0f, 0.0f))	// 法線ベクトル

// MAGICUIの状態管理
typedef enum
{
	MAGICUISTATE_NONDISPLAY = 0,	// 非表示
	MAGICUISTATE_APPEAR,			// 出現状態
	MAGICUISTATE_DISPLAY,			// 表示状態
	MAGICUISTATE_DISAPPEAR,		// 収縮状態
	MAGICUISTATE_MAX
}MAGICUISTATE;

// MAGICUIの構造体
typedef struct
{
	D3DXMATRIX		mtxWorld;	// ワールドマトリックス
	D3DXVECTOR3		pos;		// 位置
	MAGICUI_TEX		tex;		// 使用されるテクスチャの種類を管理
	MAGICUI_TYPE	type;		// 表示されるUIの場所を管理
	float	fWidth;			// 幅
	float	fWidthDest;		// 幅の目的値
	float	fHeight;		// 高さ
	float	fHeightDest;	// 高さの目的値
	bool	bDisp;			// 表示状態
}MAGICUI;

// MAGICUIの全体管理
typedef struct
{
	MAGICUI aMagicUI[MAXSPELL_TYPE];	// UIの種類ごとの表示管理
	D3DXVECTOR3		pos;		// 中心位置
	D3DXVECTOR3		rot;		// 向き
	MAGICUISTATE	state;		// 出現状態
	int		nNumKey;			// 移動にかかるフレーム数
	int		nKey;				// 現在のフレーム数
	bool	bDisp;				// 全体の表示管理
}MagicUI;

// グローバル変数
LPDIRECT3DTEXTURE9	g_apTextureMagicUI[MAXSPELL_TEX] = {};	// テクスチャへのポインタ
LPDIRECT3DVERTEXBUFFER9	g_pVtxBuffMagicUI = NULL;			// 頂点バッファへのポインタ
MagicUI g_aMagicUI[MAX_PLAYER];		// MAGICUIの全体管理

// テクスチャの読み込み
const char* c_apFilenameMagicUI[MAXSPELL_TEX] =
{
	"data\\TEXTURE\\MagicUI\\MagicUI_100.png",
	"data\\TEXTURE\\MagicUI\\MagicUI_000.png",
	"data\\TEXTURE\\MagicUI\\MagicUI_001.png",
	"data\\TEXTURE\\MagicUI\\MagicUI_002.png",
	"data\\TEXTURE\\MagicUI\\MagicUI_003.png",
	"data\\TEXTURE\\MagicUI\\MagicUI_004.png",
	"data\\TEXTURE\\MagicUI\\MagicUI_005.png",
	"data\\TEXTURE\\MagicUI\\MagicUI_006.png",
	"data\\TEXTURE\\MagicUI\\MagicUI_007.png",
	"data\\TEXTURE\\MagicUI\\MagicUI_008.png",
	"data\\TEXTURE\\MagicUI\\MagicUI_009.png",
	"data\\TEXTURE\\MagicUI\\MagicUI_010.png",
	"data\\TEXTURE\\MagicUI\\MagicUI_011.png",
	"data\\TEXTURE\\MagicUI\\MagicUI_012.png",
	"data\\TEXTURE\\MagicUI\\MagicUI_013.png",
	"data\\TEXTURE\\MagicUI\\MagicUI_014.png",
	"data\\TEXTURE\\MagicUI\\MagicUI_015.png",
	"data\\TEXTURE\\MagicUI\\MagicUI_016.png",
	"data\\TEXTURE\\MagicUI\\MagicUI_017.png",
	"data\\TEXTURE\\MagicUI\\MagicUI_018.png",
	"data\\TEXTURE\\MagicUI\\MagicUI_019.png",
	"data\\TEXTURE\\MagicUI\\MagicUI_020.png",
	"data\\TEXTURE\\MagicUI\\MagicUI_021.png",
	"data\\TEXTURE\\MagicUI\\MagicUI_022.png",
};

//======================================================================================
// 魔導書の初期化処理
//======================================================================================
void InitMagicUI(void)
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
	for (int nCntUI = 0; nCntUI < MAXSPELL_TEX; nCntUI++)
	{
		D3DXCreateTextureFromFile(pDevice, c_apFilenameMagicUI[nCntUI], &g_apTextureMagicUI[nCntUI]);
	}

	// 初期化
	for (int nCntPlayer = 0; nCntPlayer < MAX_PLAYER; nCntPlayer++)
	{
		for (int nCntUI = 0; nCntUI < MAXSPELL_TYPE; nCntUI++)
		{
			g_aMagicUI[nCntPlayer].aMagicUI[nCntUI].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);	// 中心位置
			g_aMagicUI[nCntPlayer].aMagicUI[nCntUI].tex = MAGICUI_TEX_NULL;					// テクスチャの種類
			g_aMagicUI[nCntPlayer].aMagicUI[nCntUI].type = MAGICUI_TYPE_MAGIC0;				// テクスチャの配置
			g_aMagicUI[nCntPlayer].aMagicUI[nCntUI].fWidth = MAGICUI_WIDTH;					// 幅
			g_aMagicUI[nCntPlayer].aMagicUI[nCntUI].fHeight = MAGICUI_HEIGHT;				// 高さ
			g_aMagicUI[nCntPlayer].aMagicUI[nCntUI].fWidthDest = 0.0f;						// 幅の目的地
			g_aMagicUI[nCntPlayer].aMagicUI[nCntUI].fHeightDest = 0.0f;						// 高さの目的地
			g_aMagicUI[nCntPlayer].aMagicUI[nCntUI].bDisp = false;							// 表示状態
		}
		g_aMagicUI[nCntPlayer].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);		// 中心位置
		g_aMagicUI[nCntPlayer].rot = D3DXVECTOR3(0.0f, 3.14f, 0.0f);	// 中心位置
		g_aMagicUI[nCntPlayer].state = MAGICUISTATE_NONDISPLAY;			// UIの表示状態
		g_aMagicUI[nCntPlayer].nNumKey = NUM_KEY;						// 処理を行うキー数
		g_aMagicUI[nCntPlayer].nKey = 0;							// 現在のキー
		g_aMagicUI[nCntPlayer].bDisp = false;			// UI表示状態(trueでポーズ中)
	}

	// 頂点バッファの生成
	pDevice->CreateVertexBuffer(sizeof(VERTEX_3D) * 4 * MAXSPELL_TYPE * MAX_PLAYER, D3DUSAGE_WRITEONLY, FVF_VERTEX_3D, D3DPOOL_MANAGED, &g_pVtxBuffMagicUI, NULL);

	VERTEX_3D* pVtx;
	// 頂点バッファをロックし、頂点情報へのポインタを取得
	g_pVtxBuffMagicUI->Lock(0, 0, (void**)&pVtx, 0);

	for (int nCntPlayer = 0; nCntPlayer < MAX_PLAYER; nCntPlayer++)
	{
		for (int nCntUI = 0; nCntUI < MAXSPELL_TYPE; nCntUI++, pVtx += 4)
		{
			// 各種情報の設定
			switch (nCntUI)
			{
			case MAGICUI_TYPE_MAGIC0:	// 1つ目の魔法
				g_aMagicUI[nCntPlayer].aMagicUI[nCntUI].pos		= D3DXVECTOR3(-MAGICUI_X, MAGICUI_YUP, 0.0f);	// 中心位置
				g_aMagicUI[nCntPlayer].aMagicUI[nCntUI].tex		= MAGICUI_TEX_NULL;		// テクスチャの種類
				g_aMagicUI[nCntPlayer].aMagicUI[nCntUI].type	= MAGICUI_TYPE_MAGIC0;	// テクスチャの配置
				g_aMagicUI[nCntPlayer].aMagicUI[nCntUI].fWidth	= MAGICUI_WIDTH;		// 幅
				g_aMagicUI[nCntPlayer].aMagicUI[nCntUI].fHeight = MAGICUI_HEIGHT;		// 高さ
				g_aMagicUI[nCntPlayer].aMagicUI[nCntUI].fWidthDest	= 0.0f;				// 幅の目的地
				g_aMagicUI[nCntPlayer].aMagicUI[nCntUI].fHeightDest = 0.0f;				// 高さの目的地
				g_aMagicUI[nCntPlayer].aMagicUI[nCntUI].bDisp		= false;			// 表示状態
				break;

			case MAGICUI_TYPE_MAGIC1:	// 2つ目の魔法
				g_aMagicUI[nCntPlayer].aMagicUI[nCntUI].pos		= D3DXVECTOR3(MAGICUI_X, MAGICUI_YUP, 0.0f);	// 中心位置
				g_aMagicUI[nCntPlayer].aMagicUI[nCntUI].tex		= MAGICUI_TEX_NULL;		// テクスチャの種類
				g_aMagicUI[nCntPlayer].aMagicUI[nCntUI].type	= MAGICUI_TYPE_MAGIC1;	// テクスチャの配置
				g_aMagicUI[nCntPlayer].aMagicUI[nCntUI].fWidth	= MAGICUI_WIDTH;		// 幅
				g_aMagicUI[nCntPlayer].aMagicUI[nCntUI].fHeight = MAGICUI_HEIGHT;		// 高さ
				g_aMagicUI[nCntPlayer].aMagicUI[nCntUI].fWidthDest	= 0.0f;				// 幅の目的地
				g_aMagicUI[nCntPlayer].aMagicUI[nCntUI].fHeightDest = 0.0f;				// 高さの目的地
				g_aMagicUI[nCntPlayer].aMagicUI[nCntUI].bDisp		= false;			// 表示状態
				break;

			case MAGICUI_TYPE_MAGIC2:	// 3つ目の魔法
				g_aMagicUI[nCntPlayer].aMagicUI[nCntUI].pos		= D3DXVECTOR3(-MAGICUI_X, MAGICUI_YDOWN, 0.0f);	// 中心位置
				g_aMagicUI[nCntPlayer].aMagicUI[nCntUI].tex		= MAGICUI_TEX_NULL;		// テクスチャの種類
				g_aMagicUI[nCntPlayer].aMagicUI[nCntUI].type	= MAGICUI_TYPE_MAGIC2;	// テクスチャの配置
				g_aMagicUI[nCntPlayer].aMagicUI[nCntUI].fWidth	= MAGICUI_WIDTH;		// 幅
				g_aMagicUI[nCntPlayer].aMagicUI[nCntUI].fHeight = MAGICUI_HEIGHT;		// 高さ
				g_aMagicUI[nCntPlayer].aMagicUI[nCntUI].fWidthDest	= 0.0f;				// 幅の目的地
				g_aMagicUI[nCntPlayer].aMagicUI[nCntUI].fHeightDest = 0.0f;				// 高さの目的地
				g_aMagicUI[nCntPlayer].aMagicUI[nCntUI].bDisp		= false;			// 表示状態
				break;

			case MAGICUI_TYPE_MAGIC3:	// 4つ目の魔法
				g_aMagicUI[nCntPlayer].aMagicUI[nCntUI].pos		= D3DXVECTOR3(MAGICUI_X, MAGICUI_YDOWN, 0.0f);	// 中心位置
				g_aMagicUI[nCntPlayer].aMagicUI[nCntUI].tex		= MAGICUI_TEX_NULL;		// テクスチャの種類
				g_aMagicUI[nCntPlayer].aMagicUI[nCntUI].type	= MAGICUI_TYPE_MAGIC3;	// テクスチャの配置
				g_aMagicUI[nCntPlayer].aMagicUI[nCntUI].fWidth	= MAGICUI_WIDTH;		// 幅
				g_aMagicUI[nCntPlayer].aMagicUI[nCntUI].fHeight = MAGICUI_HEIGHT;		// 高さ
				g_aMagicUI[nCntPlayer].aMagicUI[nCntUI].fWidthDest	= 0.0f;				// 幅の目的地
				g_aMagicUI[nCntPlayer].aMagicUI[nCntUI].fHeightDest = 0.0f;				// 高さの目的地
				g_aMagicUI[nCntPlayer].aMagicUI[nCntUI].bDisp		= false;			// 表示状態
				break;
			}

			// 中心位置からの位置を求める
			g_aMagicUI[nCntPlayer].aMagicUI[nCntUI].pos += g_aMagicUI[nCntPlayer].pos;

			// 頂点座標の設定
			pVtx[0].pos = D3DXVECTOR3(-g_aMagicUI[nCntPlayer].aMagicUI[nCntUI].fWidth, g_aMagicUI[nCntPlayer].aMagicUI[nCntUI].fHeight, MAGICUI_Z);
			pVtx[1].pos = D3DXVECTOR3(g_aMagicUI[nCntPlayer].aMagicUI[nCntUI].fWidth, g_aMagicUI[nCntPlayer].aMagicUI[nCntUI].fHeight, MAGICUI_Z);
			pVtx[2].pos = D3DXVECTOR3(-g_aMagicUI[nCntPlayer].aMagicUI[nCntUI].fWidth, -g_aMagicUI[nCntPlayer].aMagicUI[nCntUI].fHeight, MAGICUI_Z);
			pVtx[3].pos = D3DXVECTOR3(g_aMagicUI[nCntPlayer].aMagicUI[nCntUI].fWidth, -g_aMagicUI[nCntPlayer].aMagicUI[nCntUI].fHeight, MAGICUI_Z);

			// rhwの設定
			pVtx[0].nor = NORMAL;
			pVtx[1].nor = NORMAL;
			pVtx[2].nor = NORMAL;
			pVtx[3].nor = NORMAL;

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

		}
	}
	// 頂点バッファをアンロック
	g_pVtxBuffMagicUI->Unlock();
}

//======================================================================================
// 魔導書の終了処理
//======================================================================================
void UninitMagicUI(void)
{
	// テクスチャの破棄
	for (int nCntUI = 0; nCntUI < MAXSPELL_TEX; nCntUI++)
	{
		if (g_apTextureMagicUI[nCntUI] != NULL)
		{
			g_apTextureMagicUI[nCntUI]->Release();
			g_apTextureMagicUI[nCntUI] = NULL;
		}
	}

	// 頂点バッファの破棄
	if (g_pVtxBuffMagicUI != NULL)
	{
		g_pVtxBuffMagicUI->Release();
		g_pVtxBuffMagicUI = NULL;
	}

}

//======================================================================================
// 魔導書の更新処理
//======================================================================================
void UpdateMagicUI(void)
{
	VERTEX_3D* pVtx;
	// 頂点バッファをロックし、頂点情報へのポインタを取得
	g_pVtxBuffMagicUI->Lock(0, 0, (void**)&pVtx, 0);

	Player* pPlayer = GetPlayer();
	for (int nCntPlayer = 0; nCntPlayer < MAX_PLAYER; nCntPlayer++, pPlayer++)
	{
		// 魔導書の中身を更新
		SetMagicTexture(nCntPlayer);

		// SPELLメニューを開いているかのフラグを立てる
		if ((GetKeyboardPress(DIK_TAB) == true && nCntPlayer == 0) || GetJoypadRightTriggePress(nCntPlayer) == true || GetJoypadLeftTriggePress(nCntPlayer) == true)
		{
			if ((GetKeyboardTrigger(DIK_TAB) == true && nCntPlayer == 0) || GetJoypadTrigger(JOYKEY_LEFT_TRIGGER, nCntPlayer) == true || GetJoypadTrigger(JOYKEY_RIGHT_TRIGGER, nCntPlayer) == true)
			{
				g_aMagicUI[nCntPlayer].state = MAGICUISTATE_APPEAR;
				SetMagicUIAppear(nCntPlayer);
			}
			g_aMagicUI[nCntPlayer].bDisp = true;
		}
		else
		{
			if ((GetKeyboardRelease(DIK_TAB) == true && nCntPlayer == 0) || GetJoypadRelease(JOYKEY_LEFT_TRIGGER, nCntPlayer) == true || GetJoypadRelease(JOYKEY_RIGHT_TRIGGER, nCntPlayer) == true)
			{
				g_aMagicUI[nCntPlayer].state = MAGICUISTATE_DISAPPEAR;
				SetMagicUIDisappear(nCntPlayer);
			}
		}

		if (g_aMagicUI[nCntPlayer].bDisp == false)
		{
			continue;
		}

		// 全体の演出処理======================================================================================
		float fDiffKey = 0.0f;	// キーの差分を計算
		float fRateKey = (float)g_aMagicUI[nCntPlayer].nKey / (float)g_aMagicUI[nCntPlayer].nNumKey;
		switch (g_aMagicUI[nCntPlayer].state)
		{
		case MAGICUISTATE_NONDISPLAY:	// 非表示状態
			g_aMagicUI[nCntPlayer].bDisp = false;
			break;
		
		case MAGICUISTATE_APPEAR:	// 出現状態
			for (int nCntUI = 0; nCntUI < MAXSPELL_TYPE; nCntUI++)
			{
				// 背景の高度変更
				fDiffKey = g_aMagicUI[nCntPlayer].aMagicUI[nCntUI].fHeightDest - g_aMagicUI[nCntPlayer].aMagicUI[nCntUI].fHeight;
				g_aMagicUI[nCntPlayer].aMagicUI[nCntUI].fHeight = g_aMagicUI[nCntPlayer].aMagicUI[nCntUI].fHeight + fDiffKey * fRateKey;
				// 中心位置からの位置を求める
				g_aMagicUI[nCntPlayer].aMagicUI[nCntUI].pos += g_aMagicUI[nCntPlayer].pos;
			}
			g_aMagicUI[nCntPlayer].nKey++;
		
			if (g_aMagicUI[nCntPlayer].nKey > g_aMagicUI[nCntPlayer].nNumKey)
			{
				g_aMagicUI[nCntPlayer].state = MAGICUISTATE_DISPLAY;
				SetMagicUIDisplay(nCntPlayer);
			}
			break;
		
		case MAGICUISTATE_DISPLAY:	// 表示状態
		
			break;
		
		case MAGICUISTATE_DISAPPEAR:	// 収縮状態
			for (int nCntUI = 0; nCntUI < MAXSPELL_TYPE; nCntUI++)
			{
				// 背景の高度変更
				fDiffKey = g_aMagicUI[nCntPlayer].aMagicUI[nCntUI].fHeightDest - g_aMagicUI[nCntPlayer].aMagicUI[nCntUI].fHeight;
				g_aMagicUI[nCntPlayer].aMagicUI[nCntUI].fHeight = g_aMagicUI[nCntPlayer].aMagicUI[nCntUI].fHeight + fDiffKey * fRateKey;
				// 中心位置からの位置を求める
				g_aMagicUI[nCntPlayer].aMagicUI[nCntUI].pos += g_aMagicUI[nCntPlayer].pos;
			}
			g_aMagicUI[nCntPlayer].nKey++;
		
			if (g_aMagicUI[nCntPlayer].nKey > g_aMagicUI[nCntPlayer].nNumKey)
			{
				g_aMagicUI[nCntPlayer].state = MAGICUISTATE_NONDISPLAY;
				SetMagicUINonDisplay(nCntPlayer);
			}
			break;
		}

		for (int nCntUI = 0; nCntUI < MAXSPELL_TYPE; nCntUI++, pVtx+= 4)
		{
			// 頂点座標の設定
			pVtx[0].pos = D3DXVECTOR3(-g_aMagicUI[nCntPlayer].aMagicUI[nCntUI].fWidth, g_aMagicUI[nCntPlayer].aMagicUI[nCntUI].fHeight, MAGICUI_Z);
			pVtx[1].pos = D3DXVECTOR3(g_aMagicUI[nCntPlayer].aMagicUI[nCntUI].fWidth, g_aMagicUI[nCntPlayer].aMagicUI[nCntUI].fHeight, MAGICUI_Z);
			pVtx[2].pos = D3DXVECTOR3(-g_aMagicUI[nCntPlayer].aMagicUI[nCntUI].fWidth, -g_aMagicUI[nCntPlayer].aMagicUI[nCntUI].fHeight, MAGICUI_Z);
			pVtx[3].pos = D3DXVECTOR3(g_aMagicUI[nCntPlayer].aMagicUI[nCntUI].fWidth, -g_aMagicUI[nCntPlayer].aMagicUI[nCntUI].fHeight, MAGICUI_Z);
		}
	}

	// 頂点バッファをアンロック
	g_pVtxBuffMagicUI->Unlock();
}

//======================================================================================
// 魔導書の描画処理
//======================================================================================
void DrawMagicUI(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();	// デバイスの取得
	D3DXMATRIX UIMatrix, mtxRot;	// UIのマトリックス情報を取得
	Player* pPlayer = GetPlayer();

	// ワールドマトリックスの初期化(デフォルトの値にする)
	D3DXMatrixIdentity(&UIMatrix);

	// アルファテストを有効にする
	pDevice->SetRenderState(D3DRS_ALPHATESTENABLE, TRUE);	// アルファテストを有効にする
	pDevice->SetRenderState(D3DRS_ALPHAFUNC, D3DCMP_GREATER);	// 比較方法を設定(基準値より大きい場合描画)
	pDevice->SetRenderState(D3DRS_ALPHAREF, 0);	// アルファテストの参照値を設定(この場合、0より大きい場合は描画)

	// ライトをオフにする
	pDevice->SetRenderState(D3DRS_LIGHTING, FALSE);

	// カリングをオフにする
	pDevice->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);

	for (int nCntPlayer = 0; nCntPlayer < MAX_PLAYER; nCntPlayer++, pPlayer++)
	{
		if (g_aMagicUI[nCntPlayer].bDisp == false)
		{
			continue;
		}
		// UIのマトリックス情報を取得
		UIMatrix = pPlayer->mtxWorld;

		// ワールドマトリックスの設定
		pDevice->SetTransform(D3DTS_WORLD, &UIMatrix);

		for (int nCntUI = 0; nCntUI < MAXSPELL_TYPE; nCntUI++)
		{
			D3DXMATRIX	mtxRotModel, mtxTransModel;	// 計算用マトリックス
			D3DXMATRIX	mtxParent;					// 親のマトリックス

			if (g_aMagicUI[nCntPlayer].aMagicUI[nCntUI].bDisp == false)
			{
				continue;
			}

			// ポリゴンのワールドマトリックスを初期化
			D3DXMatrixIdentity(&g_aMagicUI[nCntPlayer].aMagicUI[nCntUI].mtxWorld);

			// パーツの位置を反映
			D3DXMatrixTranslation(&mtxTransModel, g_aMagicUI[nCntPlayer].aMagicUI[nCntUI].pos.x, g_aMagicUI[nCntPlayer].aMagicUI[nCntUI].pos.y, g_aMagicUI[nCntPlayer].aMagicUI[nCntUI].pos.z);
			D3DXMatrixMultiply(&g_aMagicUI[nCntPlayer].aMagicUI[nCntUI].mtxWorld, &g_aMagicUI[nCntPlayer].aMagicUI[nCntUI].mtxWorld, &mtxTransModel);

			// 向きを反映
			D3DXMatrixRotationYawPitchRoll(&mtxRot, g_aMagicUI[nCntPlayer].rot.y, g_aMagicUI[nCntPlayer].rot.x, g_aMagicUI[nCntPlayer].rot.z);
			D3DXMatrixMultiply(&g_aMagicUI[nCntPlayer].aMagicUI[nCntUI].mtxWorld, &g_aMagicUI[nCntPlayer].aMagicUI[nCntUI].mtxWorld, &mtxRot);

			// 親マトリックスを設定
			mtxParent = UIMatrix;

			// 算出したパーツのワールドマトリックスと親モデルのマトリックスを掛け合わせる
			D3DXMatrixMultiply(&g_aMagicUI[nCntPlayer].aMagicUI[nCntUI].mtxWorld, &g_aMagicUI[nCntPlayer].aMagicUI[nCntUI].mtxWorld, &mtxParent);

			// パーツのワールドマトリックスを設定
			pDevice->SetTransform(D3DTS_WORLD, &g_aMagicUI[nCntPlayer].aMagicUI[nCntUI].mtxWorld);

			// 頂点バッファをデータストリームに設定
			pDevice->SetStreamSource(0, g_pVtxBuffMagicUI, 0, sizeof(VERTEX_3D));

			// 頂点フォーマットの設定
			pDevice->SetFVF(FVF_VERTEX_3D);

			// テクスチャの設定
			pDevice->SetTexture(0, g_apTextureMagicUI[g_aMagicUI[nCntPlayer].aMagicUI[nCntUI].tex]);

			// UIの描画
			pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, nCntUI * 4 + (nCntPlayer * MAXSPELL_TYPE * 4), 2);
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

//======================================================================================
// プレイヤーの魔導書の情報を移す
//======================================================================================
void SetMagicTexture(int nIdx)
{
	Player* pPlayer = GetPlayer();

	// プレイヤーの所持魔法を取得してローカルで格納
	COMMANDOREDER commandOrder[MAX_COMMAND];
	// テクスチャ情報をローカルで保存
	MAGICUI_TEX	tex = MAGICUI_TEX_NULL;

	for (int nCntMagic = 0; nCntMagic < MAX_COMMAND; nCntMagic++)
	{
		commandOrder[nCntMagic] = pPlayer[nIdx].magicbook.OwnCommand[nCntMagic];

		// コマンドごとにテクスチャを配置
		switch (commandOrder[nCntMagic])
		{
		case COMMANDOREDER_NONE:	// コマンドがない場合
			g_aMagicUI[nIdx].aMagicUI[nCntMagic].bDisp = false;
			tex = MAGICUI_TEX_NULL;
			break;

		case COMMANDOREDER_GGG:	// GGG
			tex = MAGICUI_TEX_GGG;
			break;

		case COMMANDOREDER_RRR:	// RRR
			tex = MAGICUI_TEX_RRR;
			break;

		case COMMANDOREDER_BBB:	// BBB
			tex = MAGICUI_TEX_BBB;
			break;

		case COMMANDOREDER_YYY:	// YYY
			tex = MAGICUI_TEX_YYY;
			break;

		case COMMANDOREDER_RRG:	// RRG
			tex = MAGICUI_TEX_RRG;
			break;

		case COMMANDOREDER_RGR:	// RGR
			tex = MAGICUI_TEX_RGR;
			break;

		case COMMANDOREDER_GRR:	// GRR
			tex = MAGICUI_TEX_GRR;
			break;

		case COMMANDOREDER_RYY:	// RYY
			tex = MAGICUI_TEX_RYY;
			break;

		case COMMANDOREDER_YRY:	// YRY
			tex = MAGICUI_TEX_YRY;
			break;

		case COMMANDOREDER_YYR:	// YYR
			tex = MAGICUI_TEX_YYR;
			break;

		case COMMANDOREDER_BBG:	// BBG
			tex = MAGICUI_TEX_BBG;
			break;

		case COMMANDOREDER_BGB:	// BGB
			tex = MAGICUI_TEX_BGB;
			break;

		case COMMANDOREDER_GBB:	// GBB
			tex = MAGICUI_TEX_GBB;
			break;

		case COMMANDOREDER_BGG:	// BGG
			tex = MAGICUI_TEX_BGG;
			break;

		case COMMANDOREDER_GBG:	// GBG
			tex = MAGICUI_TEX_GBG;
			break;

		case COMMANDOREDER_GGB:	// GGB
			tex = MAGICUI_TEX_GGB;
			break;

		case COMMANDOREDER_BYY:	// BYY
			tex = MAGICUI_TEX_BYY;
			break;

		case COMMANDOREDER_YBY:	// YBY
			tex = MAGICUI_TEX_YBY;
			break;

		case COMMANDOREDER_YYB:	// YYB
			tex = MAGICUI_TEX_YYB;
			break;

		case COMMANDOREDER_GGY:	// GGY
			tex = MAGICUI_TEX_GGY;
			break;

		case COMMANDOREDER_GYG:	// GYG
			tex = MAGICUI_TEX_GYG;
			break;

		case COMMANDOREDER_YGG:	// YGG
			tex = MAGICUI_TEX_YGG;
			break;

		case COMMANDOREDER_RGB:	// RGB
			tex = MAGICUI_TEX_RGB;
			break;
		}

		g_aMagicUI[nIdx].aMagicUI[nCntMagic].tex = tex;
	}
}

//======================================================================================
// 魔導書を出現状態にする
//======================================================================================
void SetMagicUIAppear(int nIdx)
{
	g_aMagicUI[nIdx].nKey = 0;
	for (int nCntUI = 0; nCntUI < MAXSPELL_TYPE; nCntUI++)
	{
		// 非表示のものは処理を行わない
		if (g_aMagicUI[nIdx].aMagicUI[nCntUI].tex == MAGICUI_TEX_NULL)
		{
			continue;
		}

		// 各種情報の設定
		switch (nCntUI)
		{
		case MAGICUI_TYPE_MAGIC0:	// 1つ目の魔法
			g_aMagicUI[nIdx].aMagicUI[nCntUI].pos = D3DXVECTOR3(-MAGICUI_X, MAGICUI_YUP, 0.0f);	// 中心位置
			g_aMagicUI[nIdx].aMagicUI[nCntUI].fWidth = MAGICUI_WIDTH;		// 幅
			g_aMagicUI[nIdx].aMagicUI[nCntUI].fHeight = 0.0f;		// 高さ
			g_aMagicUI[nIdx].aMagicUI[nCntUI].fWidthDest = 0.0f;				// 幅の目的地
			g_aMagicUI[nIdx].aMagicUI[nCntUI].fHeightDest = MAGICUI_HEIGHT;				// 高さの目的地
			g_aMagicUI[nIdx].aMagicUI[nCntUI].bDisp = true;			// 表示状態
			break;

		case MAGICUI_TYPE_MAGIC1:	// 2つ目の魔法
			g_aMagicUI[nIdx].aMagicUI[nCntUI].pos = D3DXVECTOR3(MAGICUI_X, MAGICUI_YUP, 0.0f);	// 中心位置
			g_aMagicUI[nIdx].aMagicUI[nCntUI].fWidth = MAGICUI_WIDTH;		// 幅
			g_aMagicUI[nIdx].aMagicUI[nCntUI].fHeight = 0.0f;		// 高さ
			g_aMagicUI[nIdx].aMagicUI[nCntUI].fWidthDest = 0.0f;				// 幅の目的地
			g_aMagicUI[nIdx].aMagicUI[nCntUI].fHeightDest = MAGICUI_HEIGHT;				// 高さの目的地
			g_aMagicUI[nIdx].aMagicUI[nCntUI].bDisp = true;			// 表示状態
			break;

		case MAGICUI_TYPE_MAGIC2:	// 3つ目の魔法
			g_aMagicUI[nIdx].aMagicUI[nCntUI].pos = D3DXVECTOR3(-MAGICUI_X, MAGICUI_YDOWN, 0.0f);	// 中心位置
			g_aMagicUI[nIdx].aMagicUI[nCntUI].fWidth = MAGICUI_WIDTH;		// 幅
			g_aMagicUI[nIdx].aMagicUI[nCntUI].fHeight = 0.0f;		// 高さ
			g_aMagicUI[nIdx].aMagicUI[nCntUI].fWidthDest = 0.0f;				// 幅の目的地
			g_aMagicUI[nIdx].aMagicUI[nCntUI].fHeightDest = MAGICUI_HEIGHT;				// 高さの目的地
			g_aMagicUI[nIdx].aMagicUI[nCntUI].bDisp = true;			// 表示状態
			break;

		case MAGICUI_TYPE_MAGIC3:	// 4つ目の魔法
			g_aMagicUI[nIdx].aMagicUI[nCntUI].pos = D3DXVECTOR3(MAGICUI_X, MAGICUI_YDOWN, 0.0f);	// 中心位置
			g_aMagicUI[nIdx].aMagicUI[nCntUI].fWidth = MAGICUI_WIDTH;		// 幅
			g_aMagicUI[nIdx].aMagicUI[nCntUI].fHeight = 0.0f;		// 高さ
			g_aMagicUI[nIdx].aMagicUI[nCntUI].fWidthDest = 0.0f;				// 幅の目的地
			g_aMagicUI[nIdx].aMagicUI[nCntUI].fHeightDest = MAGICUI_HEIGHT;				// 高さの目的地
			g_aMagicUI[nIdx].aMagicUI[nCntUI].bDisp = true;			// 表示状態
			break;
		}
	}
}

//======================================================================================
// 魔導書を表示状態にする
//======================================================================================
void SetMagicUIDisplay(int nIdx)
{
	g_aMagicUI[nIdx].nKey = 0;
	for (int nCntUI = 0; nCntUI < MAXSPELL_TYPE; nCntUI++)
	{
		// 非表示のものは処理を行わない
		if (g_aMagicUI[nIdx].aMagicUI[nCntUI].tex == MAGICUI_TEX_NULL)
		{
			continue;
		}

		switch (nCntUI)
		{
		case MAGICUI_TYPE_MAGIC0:	// 1つ目の魔法
			g_aMagicUI[nIdx].aMagicUI[nCntUI].pos = D3DXVECTOR3(-MAGICUI_X, MAGICUI_YUP, 0.0f);	// 中心位置
			g_aMagicUI[nIdx].aMagicUI[nCntUI].fWidth = MAGICUI_WIDTH;		// 幅
			g_aMagicUI[nIdx].aMagicUI[nCntUI].fHeight = MAGICUI_HEIGHT;		// 高さ
			g_aMagicUI[nIdx].aMagicUI[nCntUI].fWidthDest = 0.0f;				// 幅の目的地
			g_aMagicUI[nIdx].aMagicUI[nCntUI].fHeightDest = MAGICUI_HEIGHT;				// 高さの目的地
			g_aMagicUI[nIdx].aMagicUI[nCntUI].bDisp = true;			// 表示状態
			break;

		case MAGICUI_TYPE_MAGIC1:	// 2つ目の魔法
			g_aMagicUI[nIdx].aMagicUI[nCntUI].pos = D3DXVECTOR3(MAGICUI_X, MAGICUI_YUP, 0.0f);	// 中心位置
			g_aMagicUI[nIdx].aMagicUI[nCntUI].fWidth = MAGICUI_WIDTH;		// 幅
			g_aMagicUI[nIdx].aMagicUI[nCntUI].fHeight = MAGICUI_HEIGHT;		// 高さ
			g_aMagicUI[nIdx].aMagicUI[nCntUI].fWidthDest = 0.0f;				// 幅の目的地
			g_aMagicUI[nIdx].aMagicUI[nCntUI].fHeightDest = MAGICUI_HEIGHT;				// 高さの目的地
			g_aMagicUI[nIdx].aMagicUI[nCntUI].bDisp = true;			// 表示状態
			break;

		case MAGICUI_TYPE_MAGIC2:	// 3つ目の魔法
			g_aMagicUI[nIdx].aMagicUI[nCntUI].pos = D3DXVECTOR3(-MAGICUI_X, MAGICUI_YDOWN, 0.0f);	// 中心位置
			g_aMagicUI[nIdx].aMagicUI[nCntUI].fWidth = MAGICUI_WIDTH;		// 幅
			g_aMagicUI[nIdx].aMagicUI[nCntUI].fHeight = MAGICUI_HEIGHT;		// 高さ
			g_aMagicUI[nIdx].aMagicUI[nCntUI].fWidthDest = 0.0f;				// 幅の目的地
			g_aMagicUI[nIdx].aMagicUI[nCntUI].fHeightDest = MAGICUI_HEIGHT;				// 高さの目的地
			g_aMagicUI[nIdx].aMagicUI[nCntUI].bDisp = true;			// 表示状態
			break;

		case MAGICUI_TYPE_MAGIC3:	// 4つ目の魔法
			g_aMagicUI[nIdx].aMagicUI[nCntUI].pos = D3DXVECTOR3(MAGICUI_X, MAGICUI_YDOWN, 0.0f);	// 中心位置
			g_aMagicUI[nIdx].aMagicUI[nCntUI].fWidth = MAGICUI_WIDTH;		// 幅
			g_aMagicUI[nIdx].aMagicUI[nCntUI].fHeight = MAGICUI_HEIGHT;		// 高さ
			g_aMagicUI[nIdx].aMagicUI[nCntUI].fWidthDest = 0.0f;				// 幅の目的地
			g_aMagicUI[nIdx].aMagicUI[nCntUI].fHeightDest = MAGICUI_HEIGHT;				// 高さの目的地
			g_aMagicUI[nIdx].aMagicUI[nCntUI].bDisp = true;			// 表示状態
			break;
		}
	}
}

//======================================================================================
// 魔導書を収縮状態にする
//======================================================================================
void SetMagicUIDisappear(int nIdx)
{
	g_aMagicUI[nIdx].nKey = 0;
	for (int nCntUI = 0; nCntUI < MAXSPELL_TYPE; nCntUI++)
	{
		// 非表示のものは処理を行わない
		if (g_aMagicUI[nIdx].aMagicUI[nCntUI].bDisp == false)
		{
			continue;
		}

		switch (nCntUI)
		{
		case MAGICUI_TYPE_MAGIC0:	// 1つ目の魔法
			g_aMagicUI[nIdx].aMagicUI[nCntUI].pos = D3DXVECTOR3(-MAGICUI_X, MAGICUI_YUP, 0.0f);	// 中心位置
			g_aMagicUI[nIdx].aMagicUI[nCntUI].fWidth = MAGICUI_WIDTH;		// 幅
			g_aMagicUI[nIdx].aMagicUI[nCntUI].fHeight = MAGICUI_HEIGHT;		// 高さ
			g_aMagicUI[nIdx].aMagicUI[nCntUI].fWidthDest = 0.0f;				// 幅の目的地
			g_aMagicUI[nIdx].aMagicUI[nCntUI].fHeightDest = 0.0f;				// 高さの目的地
			g_aMagicUI[nIdx].aMagicUI[nCntUI].bDisp = true;			// 表示状態
			break;

		case MAGICUI_TYPE_MAGIC1:	// 2つ目の魔法
			g_aMagicUI[nIdx].aMagicUI[nCntUI].pos = D3DXVECTOR3(MAGICUI_X, MAGICUI_YUP, 0.0f);	// 中心位置
			g_aMagicUI[nIdx].aMagicUI[nCntUI].fWidth = MAGICUI_WIDTH;		// 幅
			g_aMagicUI[nIdx].aMagicUI[nCntUI].fHeight = MAGICUI_HEIGHT;		// 高さ
			g_aMagicUI[nIdx].aMagicUI[nCntUI].fWidthDest = 0.0f;				// 幅の目的地
			g_aMagicUI[nIdx].aMagicUI[nCntUI].fHeightDest = 0.0f;				// 高さの目的地
			g_aMagicUI[nIdx].aMagicUI[nCntUI].bDisp = true;			// 表示状態
			break;

		case MAGICUI_TYPE_MAGIC2:	// 3つ目の魔法
			g_aMagicUI[nIdx].aMagicUI[nCntUI].pos = D3DXVECTOR3(-MAGICUI_X, MAGICUI_YDOWN, 0.0f);	// 中心位置
			g_aMagicUI[nIdx].aMagicUI[nCntUI].fWidth = MAGICUI_WIDTH;		// 幅
			g_aMagicUI[nIdx].aMagicUI[nCntUI].fHeight = MAGICUI_HEIGHT;		// 高さ
			g_aMagicUI[nIdx].aMagicUI[nCntUI].fWidthDest = 0.0f;				// 幅の目的地
			g_aMagicUI[nIdx].aMagicUI[nCntUI].fHeightDest = 0.0f;				// 高さの目的地
			g_aMagicUI[nIdx].aMagicUI[nCntUI].bDisp = true;			// 表示状態
			break;

		case MAGICUI_TYPE_MAGIC3:	// 4つ目の魔法
			g_aMagicUI[nIdx].aMagicUI[nCntUI].pos = D3DXVECTOR3(MAGICUI_X, MAGICUI_YDOWN, 0.0f);	// 中心位置
			g_aMagicUI[nIdx].aMagicUI[nCntUI].fWidth = MAGICUI_WIDTH;		// 幅
			g_aMagicUI[nIdx].aMagicUI[nCntUI].fHeight = MAGICUI_HEIGHT;		// 高さ
			g_aMagicUI[nIdx].aMagicUI[nCntUI].fWidthDest = 0.0f;				// 幅の目的地
			g_aMagicUI[nIdx].aMagicUI[nCntUI].fHeightDest = 0.0f;				// 高さの目的地
			g_aMagicUI[nIdx].aMagicUI[nCntUI].bDisp = true;			// 表示状態
			break;
		}
	}
}

//======================================================================================
// 魔導書を非表示状態にする
//======================================================================================
void SetMagicUINonDisplay(int nIdx)
{
	g_aMagicUI[nIdx].nKey = 0;
	for (int nCntUI = 0; nCntUI < MAXSPELL_TYPE; nCntUI++)
	{
		// 各種情報の設定
		switch (nCntUI)
		{
		case MAGICUI_TYPE_MAGIC0:	// 1つ目の魔法
			g_aMagicUI[nIdx].aMagicUI[nCntUI].pos = D3DXVECTOR3(-MAGICUI_X, MAGICUI_YUP, 0.0f);	// 中心位置
			g_aMagicUI[nIdx].aMagicUI[nCntUI].fWidth = MAGICUI_WIDTH;		// 幅
			g_aMagicUI[nIdx].aMagicUI[nCntUI].fHeight = 0.0f;		// 高さ
			g_aMagicUI[nIdx].aMagicUI[nCntUI].fWidthDest = 0.0f;				// 幅の目的地
			g_aMagicUI[nIdx].aMagicUI[nCntUI].fHeightDest = MAGICUI_HEIGHT;				// 高さの目的地
			g_aMagicUI[nIdx].aMagicUI[nCntUI].bDisp = false;			// 表示状態
			break;

		case MAGICUI_TYPE_MAGIC1:	// 2つ目の魔法
			g_aMagicUI[nIdx].aMagicUI[nCntUI].pos = D3DXVECTOR3(MAGICUI_X, MAGICUI_YUP, 0.0f);	// 中心位置
			g_aMagicUI[nIdx].aMagicUI[nCntUI].fWidth = MAGICUI_WIDTH;		// 幅
			g_aMagicUI[nIdx].aMagicUI[nCntUI].fHeight = 0.0f;		// 高さ
			g_aMagicUI[nIdx].aMagicUI[nCntUI].fWidthDest = 0.0f;				// 幅の目的地
			g_aMagicUI[nIdx].aMagicUI[nCntUI].fHeightDest = MAGICUI_HEIGHT;				// 高さの目的地
			g_aMagicUI[nIdx].aMagicUI[nCntUI].bDisp = false;			// 表示状態
			break;

		case MAGICUI_TYPE_MAGIC2:	// 3つ目の魔法
			g_aMagicUI[nIdx].aMagicUI[nCntUI].pos = D3DXVECTOR3(-MAGICUI_X, MAGICUI_YDOWN, 0.0f);	// 中心位置
			g_aMagicUI[nIdx].aMagicUI[nCntUI].fWidth = MAGICUI_WIDTH;		// 幅
			g_aMagicUI[nIdx].aMagicUI[nCntUI].fHeight = 0.0f;		// 高さ
			g_aMagicUI[nIdx].aMagicUI[nCntUI].fWidthDest = 0.0f;				// 幅の目的地
			g_aMagicUI[nIdx].aMagicUI[nCntUI].fHeightDest = MAGICUI_HEIGHT;				// 高さの目的地
			g_aMagicUI[nIdx].aMagicUI[nCntUI].bDisp = false;			// 表示状態
			break;

		case MAGICUI_TYPE_MAGIC3:	// 4つ目の魔法
			g_aMagicUI[nIdx].aMagicUI[nCntUI].pos = D3DXVECTOR3(MAGICUI_X, MAGICUI_YDOWN, 0.0f);	// 中心位置
			g_aMagicUI[nIdx].aMagicUI[nCntUI].fWidth = MAGICUI_WIDTH;		// 幅
			g_aMagicUI[nIdx].aMagicUI[nCntUI].fHeight = 0.0f;		// 高さ
			g_aMagicUI[nIdx].aMagicUI[nCntUI].fWidthDest = 0.0f;				// 幅の目的地
			g_aMagicUI[nIdx].aMagicUI[nCntUI].fHeightDest = MAGICUI_HEIGHT;				// 高さの目的地
			g_aMagicUI[nIdx].aMagicUI[nCntUI].bDisp = false;			// 表示状態
			break;
		}
	}
}