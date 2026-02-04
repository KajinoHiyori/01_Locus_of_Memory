//========================================================
// 
// メニューの背景処理[menubg.cpp]
// Author : KajinoHiyori
// 
//========================================================
#include "main.h"
#include "menubg.h"
#include "player.h"
#include "title.h"
#include "fog.h"
#include "debugproc.h"
#include "input.h"

// マクロ定義
#define MAX_MAGIC			(4)					// 記録できる魔法の最大数
#define MAX_MENUBG_TEX		(MENUBG_TEX_MAX)	// テクスチャの最大数
#define INERTIA				(0.1f)				// UI出現の慣性
#define FRAME				(20)				// 出現/退出を管理するフレーム数
#define MENUBG_POSY			(482.0f)			// 左のUIのY軸
#define LEFT_POS			(D3DXVECTOR3(120.0f, MENUBG_POSY, 0.0f))		// onscreenの左のUI座標
#define RIGHT_POS			(D3DXVECTOR3(1160.0f, MENUBG_POSY, 0.0f))		// onscreenの右のUI座標

// 

// MENUBGの構造体
typedef struct
{
	MENUBG_STATE	state;			// 出現状態
	MENUBG_TEX		tex;			// 使用されるテクスチャの種類を管理
	D3DXVECTOR3		pos;			// 位置
	float			fWidth;			// 幅
	float			fPosY;			// Y軸の高さ
	float			fHeightUp;		// 高さ[上]
	float			fHeightDown;	// 高さ[下]
	float			fHeightDestUp;	// 高さ[上]の目的値
	float			fHeightDestDown;// 高さ[下]の目的値
	bool			bDisp;			// 表示状態
	int nFrame;							// 現在のフレーム数
	int nNumFrame;						// 移動にかかるフレーム数
	int nCounterUI;						// magicの種類を表示する時間
}MenuBG;

// グローバル変数
LPDIRECT3DTEXTURE9	g_apTextureMenuBG[MAX_MENUBG_TEX] = {};	// テクスチャへのポインタ
LPDIRECT3DVERTEXBUFFER9	g_pVtxBuffMenuBG = NULL;			// 頂点バッファへのポインタ
MenuBG g_aMenuBG[MAX_PLAYER];		// MENUBGの全体管理

// テクスチャの読み込み
const char* c_apFilenameMenuBG[MAX_MENUBG_TEX] =
{
	"data\\TEXTURE\\SpellUI\\28_ClockBG.png",
	"data\\TEXTURE\\SpellUI\\29_MagicBookBG.png",
};

//======================================================================================
// 魔導書の初期化処理
//======================================================================================
void InitMenuBG(void)
{
	LPDIRECT3DDEVICE9 pDevice;	// デバイスへのポインタ
	// デバイスの取得
	pDevice = GetDevice();

	// UI情報の初期化
	for (int nCntPlayer = 0; nCntPlayer < MAX_PLAYER; nCntPlayer++)
	{
		g_aMenuBG[nCntPlayer].tex				= MENUBG_TEX_CLOCK;					// テクスチャの初期化
		g_aMenuBG[nCntPlayer].pos				= D3DXVECTOR3(0.0f, 0.0f, 0.0f);	// 位置の初期化
		g_aMenuBG[nCntPlayer].fWidth			= 0.0f;								// 幅の初期化
		g_aMenuBG[nCntPlayer].fPosY				= 0.0f;								// 開始地点Y
		g_aMenuBG[nCntPlayer].fHeightUp			= 0.0f;								// 高さ[上]の初期化
		g_aMenuBG[nCntPlayer].fHeightDown		= 0.0f;								// 高さ[下]の初期化
		g_aMenuBG[nCntPlayer].fHeightDestUp		= 0.0f;								// 高さ[上]の目的地の初期化
		g_aMenuBG[nCntPlayer].fHeightDestDown	= 0.0f;								// 高さ[下]の目的地の初期化
		g_aMenuBG[nCntPlayer].bDisp				= false;							// 表示状態の初期化
		g_aMenuBG[nCntPlayer].state				= MENUBG_STATE_NONDISPLAY;			// 画面外にある
		g_aMenuBG[nCntPlayer].nCounterUI		= 0;								// 発動魔法の初期化
		g_aMenuBG[nCntPlayer].nFrame			= 0;								// 現在のフレームを初期化
		g_aMenuBG[nCntPlayer].nNumFrame			= FRAME;							// 出現の管理を行うフレーム数
	}

	// テクスチャの読み込み
	for (int nCntMenuBG = 0; nCntMenuBG < MAX_MENUBG_TEX; nCntMenuBG++)
	{
		D3DXCreateTextureFromFile(pDevice, c_apFilenameMenuBG[nCntMenuBG], &g_apTextureMenuBG[nCntMenuBG]);
	}

	// 操作方法の状態を取得
	OPERATIONTYPE operationType = GetOperationType();

	// 頂点バッファの生成
	pDevice->CreateVertexBuffer(sizeof(VERTEX_2D) * 4 * MAX_PLAYER, D3DUSAGE_WRITEONLY, FVF_VERTEX_2D, D3DPOOL_MANAGED, &g_pVtxBuffMenuBG, NULL);

	VERTEX_2D* pVtx;
	// 頂点バッファをロックし、頂点情報へのポインタを取得
	g_pVtxBuffMenuBG->Lock(0, 0, (void**)&pVtx, 0);

	for (int nCntPlayer = 0; nCntPlayer < MAX_PLAYER; nCntPlayer++, pVtx += 4)
	{
		switch (operationType)	// 操作人数に応じてUIの位置を変更
		{
		case OPERATIONTYPE_2P:	// 2人操作
			switch (nCntPlayer)
			{
			case 0:
				g_aMenuBG[nCntPlayer].pos = LEFT_POS;
				g_aMenuBG[nCntPlayer].bDisp = false;

				break;
			case 1:
				g_aMenuBG[nCntPlayer].pos = RIGHT_POS;
				g_aMenuBG[nCntPlayer].bDisp = false;

				break;
			}
			break;

		default:	// 1人操作
			if (nCntPlayer > 0)
			{
				pVtx += 4;
				continue;
			}
			g_aMenuBG[nCntPlayer].bDisp = true;
			g_aMenuBG[nCntPlayer].pos = RIGHT_POS;
			break;
		}

		// 頂点座標の設定
		pVtx[0].pos.x = g_aMenuBG[nCntPlayer].pos.x - g_aMenuBG[nCntPlayer].fWidth;
		pVtx[0].pos.y = g_aMenuBG[nCntPlayer].pos.y + g_aMenuBG[nCntPlayer].fHeightUp;
		pVtx[0].pos.z = 0.0f;					    
		pVtx[1].pos.x = g_aMenuBG[nCntPlayer].pos.x + g_aMenuBG[nCntPlayer].fWidth;
		pVtx[1].pos.y = g_aMenuBG[nCntPlayer].pos.y + g_aMenuBG[nCntPlayer].fHeightUp;
		pVtx[1].pos.z = 0.0f;					    
		pVtx[2].pos.x = g_aMenuBG[nCntPlayer].pos.x - g_aMenuBG[nCntPlayer].fWidth;
		pVtx[2].pos.y = g_aMenuBG[nCntPlayer].pos.y + g_aMenuBG[nCntPlayer].fHeightDown;
		pVtx[2].pos.z = 0.0f;					    
		pVtx[3].pos.x = g_aMenuBG[nCntPlayer].pos.x + g_aMenuBG[nCntPlayer].fWidth;
		pVtx[3].pos.y = g_aMenuBG[nCntPlayer].pos.y + g_aMenuBG[nCntPlayer].fHeightDown;
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

	// 頂点バッファをアンロック
	g_pVtxBuffMenuBG->Unlock();
}

//======================================================================================
// 魔導書の終了処理
//======================================================================================
void UninitMenuBG(void)
{

	// テクスチャの破棄
	for (int nCntMenuBG = 0; nCntMenuBG < MAX_MENUBG_TEX; nCntMenuBG++)
	{
		if (g_apTextureMenuBG[nCntMenuBG] != NULL)
		{
			g_apTextureMenuBG[nCntMenuBG]->Release();
			g_apTextureMenuBG[nCntMenuBG] = NULL;
		}
	}

	// 頂点バッファの破棄
	if (g_pVtxBuffMenuBG != NULL)
	{
		g_pVtxBuffMenuBG->Release();
		g_pVtxBuffMenuBG = NULL;
	}

}

//======================================================================================
// 魔導書の更新処理
//======================================================================================
void UpdateMenuBG(void)
{
	// 操作方法の状態を取得
	OPERATIONTYPE operationType = GetOperationType();

	// playerの情報を取得
	Player* pPlayer = GetPlayer();

	if (GetKeyboardTrigger(DIK_5) == true)
		SetMenuBG(0, RIGHT_POS.y);

	if (GetKeyboardTrigger(DIK_6) == true)
		DisappearMenuBG(0, RIGHT_POS.y);

	VERTEX_2D* pVtx;
	// 頂点バッファをロックし、頂点情報へのポインタを取得
	g_pVtxBuffMenuBG->Lock(0, 0, (void**)&pVtx, 0);

	for (int nCntPlayer = 0; nCntPlayer < MAX_PLAYER; nCntPlayer++, pVtx += 4)
	{
		float fDiffKeyUp = 0.0f;	// 現在の位置と目的の位置の差分を計算[上]
		float fDiffKeyDown = 0.0f;	// 現在の位置と目的の位置の差分を計算[下]
		float fRateKey = (float)g_aMenuBG[nCntPlayer].nFrame / (float)g_aMenuBG[nCntPlayer].nNumFrame;	// フレームの差分を求める

		switch (g_aMenuBG[nCntPlayer].state)
		{
		case MENUBG_STATE_NONDISPLAY:	// 非表示
			g_aMenuBG[nCntPlayer].bDisp = false;
			break;

		case MENUBG_STATE_APPEAR:	// 出現
			// MENUBGの出現処理
			fDiffKeyUp = g_aMenuBG[nCntPlayer].fHeightDestUp - g_aMenuBG[nCntPlayer].fHeightUp;
			fDiffKeyDown = g_aMenuBG[nCntPlayer].fHeightDestDown - g_aMenuBG[nCntPlayer].fHeightDown;

			g_aMenuBG[nCntPlayer].fHeightUp = g_aMenuBG[nCntPlayer].fHeightUp + fDiffKeyUp * fRateKey;
			g_aMenuBG[nCntPlayer].fHeightDown = g_aMenuBG[nCntPlayer].fHeightDown + fDiffKeyDown * fRateKey;

			g_aMenuBG[nCntPlayer].nFrame++;
			if (g_aMenuBG[nCntPlayer].nFrame == g_aMenuBG[nCntPlayer].nNumFrame)
			{
				g_aMenuBG[nCntPlayer].fHeightUp = HEIGHT_POSUP;
				g_aMenuBG[nCntPlayer].fHeightDown = HEIGHT_POSDOWN;
				g_aMenuBG[nCntPlayer].state = MENUBG_STATE_DISPLAY;	// 表示状態に切り替え
			}
			break;

		case MENUBG_STATE_DISPLAY:	// 非表示

			break;

		case MENUBG_STATE_DISAPPERA:	// 収縮
			// MENUBGの収縮処理
			fDiffKeyUp = g_aMenuBG[nCntPlayer].fHeightDestUp - g_aMenuBG[nCntPlayer].fHeightUp;
			fDiffKeyDown = g_aMenuBG[nCntPlayer].fHeightDestDown - g_aMenuBG[nCntPlayer].fHeightDown;

			g_aMenuBG[nCntPlayer].fHeightUp = g_aMenuBG[nCntPlayer].fHeightUp + fDiffKeyUp * fRateKey;
			g_aMenuBG[nCntPlayer].fHeightDown = g_aMenuBG[nCntPlayer].fHeightDown + fDiffKeyDown * fRateKey;

			g_aMenuBG[nCntPlayer].nFrame++;
			if (g_aMenuBG[nCntPlayer].nFrame == g_aMenuBG[nCntPlayer].nNumFrame)
			{
				g_aMenuBG[nCntPlayer].fHeightUp = 0.0f;
				g_aMenuBG[nCntPlayer].fHeightDown = 0.0f;
				g_aMenuBG[nCntPlayer].state = MENUBG_STATE_NONDISPLAY;	// 非表示状態に切り替え
			}

			break;
		}
		// 頂点座標の設定
		pVtx[0].pos.x = g_aMenuBG[nCntPlayer].pos.x - g_aMenuBG[nCntPlayer].fWidth;
		pVtx[0].pos.y = g_aMenuBG[nCntPlayer].pos.y + g_aMenuBG[nCntPlayer].fHeightUp;
		pVtx[0].pos.z = 0.0f;
		pVtx[1].pos.x = g_aMenuBG[nCntPlayer].pos.x + g_aMenuBG[nCntPlayer].fWidth;
		pVtx[1].pos.y = g_aMenuBG[nCntPlayer].pos.y + g_aMenuBG[nCntPlayer].fHeightUp;
		pVtx[1].pos.z = 0.0f;
		pVtx[2].pos.x = g_aMenuBG[nCntPlayer].pos.x - g_aMenuBG[nCntPlayer].fWidth;
		pVtx[2].pos.y = g_aMenuBG[nCntPlayer].pos.y + g_aMenuBG[nCntPlayer].fHeightDown;
		pVtx[2].pos.z = 0.0f;
		pVtx[3].pos.x = g_aMenuBG[nCntPlayer].pos.x + g_aMenuBG[nCntPlayer].fWidth;
		pVtx[3].pos.y = g_aMenuBG[nCntPlayer].pos.y + g_aMenuBG[nCntPlayer].fHeightDown;
		pVtx[3].pos.z = 0.0f;
	}
	// 頂点バッファをアンロック
	g_pVtxBuffMenuBG->Unlock();
}

//======================================================================================
// 魔導書の描画処理
//======================================================================================
void DrawMenuBG(void)
{
	// 一時的にfogを切る
	SetFogEnable(false);

	LPDIRECT3DDEVICE9 pDevice;	// デバイスへのポインタ
	// デバイスの取得
	pDevice = GetDevice();

	// 頂点バッファをデータストリームに設定
	pDevice->SetStreamSource(0, g_pVtxBuffMenuBG, 0, sizeof(VERTEX_2D));

	// 頂点フォーマットの設定
	pDevice->SetFVF(FVF_VERTEX_2D);

	for (int nCntPlayer = 0; nCntPlayer < MAX_PLAYER; nCntPlayer++)
	{
		if (g_aMenuBG[nCntPlayer].bDisp == false)
		{
			continue;
		}
		// テクスチャの設定
		pDevice->SetTexture(0, g_apTextureMenuBG[g_aMenuBG[nCntPlayer].tex]);

		// ポリゴンの描画
		pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, nCntPlayer * 4, 2);
	}

	// fogを戻す
	SetFogEnable(true);
}

//======================================================================================
// UIを配置
//======================================================================================
void SetMenuBG(int nIdx, float fPosY)
{
	g_aMenuBG[nIdx].bDisp = true;
	g_aMenuBG[nIdx].nFrame = 0;
	g_aMenuBG[nIdx].state = MENUBG_STATE_APPEAR;
	g_aMenuBG[nIdx].bDisp = true;			// 幅の初期化
	g_aMenuBG[nIdx].fPosY = fPosY;			// 開始地点Y
	g_aMenuBG[nIdx].fHeightUp = fPosY;			// 高さ[上]の初期化
	g_aMenuBG[nIdx].fHeightDown = fPosY;		// 高さ[下]の初期化
	g_aMenuBG[nIdx].fHeightDestUp = HEIGHT_POSUP;		// 高さ[上]の目的地の初期化
	g_aMenuBG[nIdx].fHeightDestDown = HEIGHT_POSDOWN;	// 高さ[下]の目的地の初期化
}

//======================================================================================
// UIを非表示にする
//======================================================================================
void DisappearMenuBG(int nIdx, float fPosY)
{
	g_aMenuBG[nIdx].nFrame = 0;
	g_aMenuBG[nIdx].state = MENUBG_STATE_DISAPPERA;
	g_aMenuBG[nIdx].fHeightDestUp = fPosY;				// 高さ[上]の目的地の初期化
	g_aMenuBG[nIdx].fHeightDestDown = fPosY;			// 高さ[下]の目的地の初期化
}
