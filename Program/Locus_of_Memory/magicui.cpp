//========================================================
// 
// 魔法発動状態のUI表示処理[magicui.cpp]
// Author : KajinoHiyori
// 
//========================================================
#include "main.h"
#include "magicui.h"
#include "magic.h"
#include "player.h"
#include "game.h"
#include "input.h"
#include "title.h"
#include "fog.h"
#include "debugproc.h"

// マクロ定義
#define MAX_MAGIC			(4)					// 記録できる魔法の最大数
#define MAX_MAGICUI_TEX		(MAGICUI_TEX_MAX)	// テクスチャの最大数
#define MAX_MAGICUI_TYPE	(MAGICUI_TYPE_MAX)	// 表示されるUIの種類
#define INERTIA				(0.1f)				// UI出現の慣性
#define FRAME				(20)				// 出現/退出を管理するフレーム数
#define MAGICUI_POSY		(482.0f)			// 左のUIのX軸
#define PHONE_WIDTH			(108.0f)			// スマホの幅
#define PHONE_HEIGHT		(228.0f)			// スマホの高さ
#define PHONE_HEIGHTUP		(-228.0f)			// スマホの高さ[上]
#define PHONE_HEIGHTDOWN	(228.0f)			// スマホの高さ[下]
#define MAGICBOOK_Y			(-170.0f)			// 魔導書メニューの高度
#define MAGICUI_WIDTH		(PHONE_WIDTH)		// spellメニューの幅
#define MAGICUI_HEIGHT		(24.0f)				// spellメニューの高さ
#define ICON_SIZE			(20.0f)				// 各種アイコンの大きさ
#define MATH_SIZE			(5.0f)				// +=の大きさ
#define MAGIC0_POSY			(-90.0f)			// 1つ目の魔法の高さ
#define MAGIC1_POSY			(-10.0f)			// 2つ目の魔法の高さ
#define MAGIC2_POSY			(80.0f)				// 3つ目の魔法の高さ
#define MAGIC3_POSY			(170.0f)			// 4つ目の魔法の高さ
#define COMMAND1_POSX		(-MAGIC_POSX)		// コマンド1つ目のX
#define ADD01_POSX			(-ECUAL_POSX)		// 01の合成
#define COMMAND2_POSX		(-COMMAND3_POSX)	// コマンド2つ目のX
#define ADD12_POSX			(0.0f)				// 12の合成
#define COMMAND3_POSX		(26.0f)				// コマンド3つ目のX
#define ECUAL_POSX			(53.0f)				// 合成結果
#define MAGIC_POSX			(80.0f)				// 発動魔法のX
#define LEFT_POS			(D3DXVECTOR3(120.0f, MAGICUI_POSY, 0.0f))		// onscreenの左のUI座標
#define RIGHT_POS			(D3DXVECTOR3(1160.0f, MAGICUI_POSY, 0.0f))		// onscreenの右のUI座標

// MAGICUIの構造体
typedef struct
{
	MAGICUI_TYPE	type;			// 表示されるUIの場所を管理
	MAGICUI_TEX		tex;			// 使用されるテクスチャの種類を管理
	D3DXVECTOR3		pos;			// 位置
	float		fWidth;				// 幅
	float		fHeight;			// 高さ
	float		fHeightUp;			// 高さ[上]
	float		fHeightDown;		// 高さ[下]
	float		fHeightDestUp;		// 高さ[上]の目的値
	float		fHeightDestDown;	// 高さ[下]の目的値
	bool		bDisp;				// 表示状態
}MAGICUI;

// MAGICUIの全体管理
typedef struct
{
	MAGICUISTATE state;					// 出現状態
	MAGICUI aMagicUI[MAX_MAGICUI_TYPE];	// UIの種類ごとの表示管理
	D3DXVECTOR3 pos;					// 中心位置
	D3DXVECTOR3 posDest;				// 目的の向き
	int nFrame;							// 現在のフレーム数
	int nNumFrame;						// 移動にかかるフレーム数
	int nCounterUI;						// magicの種類を表示する時間
	bool bDisp;							// 全体の表示管理
}MagicUI;

// グローバル変数
LPDIRECT3DTEXTURE9	g_apTextureMagicUI[MAX_MAGICUI_TEX] = {};	// テクスチャへのポインタ
LPDIRECT3DVERTEXBUFFER9	g_pVtxBuffMagicUI = NULL;			// 頂点バッファへのポインタ
MagicUI g_aMagicUI[MAX_PLAYER];		// MAGICUIの全体管理

// テクスチャの読み込み
const char* c_apFilenameMagicUI[MAX_MAGICUI_TEX] =
{
	"data\\TEXTURE\\SpellUI\\00_MagicNull.png",
	"data\\TEXTURE\\SpellUI\\01_Red.png",
	"data\\TEXTURE\\SpellUI\\02_Green.png",
	"data\\TEXTURE\\SpellUI\\03_Blue.png",
	"data\\TEXTURE\\SpellUI\\04_Yellow.png",
	"data\\TEXTURE\\SpellUI\\13_None.png",
	"data\\TEXTURE\\SpellUI\\14_Levitation.png",
	"data\\TEXTURE\\SpellUI\\15_Combustion.png",
	"data\\TEXTURE\\SpellUI\\16_Flood.png",
	"data\\TEXTURE\\SpellUI\\17_Flash.png",
	"data\\TEXTURE\\SpellUI\\18_FireBall.png",
	"data\\TEXTURE\\SpellUI\\19_SunsetDelay.png",
	"data\\TEXTURE\\SpellUI\\20_RainPray.png",
	"data\\TEXTURE\\SpellUI\\21_freeze.png",
	"data\\TEXTURE\\SpellUI\\22_Grouth.png",
	"data\\TEXTURE\\SpellUI\\23_Acceleration.png",
	"data\\TEXTURE\\SpellUI\\24_TimeRevert.png",
	"data\\TEXTURE\\SpellUI\\27_MagicBook.png",
	"data\\TEXTURE\\SpellUI\\30_Plus.png",
	"data\\TEXTURE\\SpellUI\\31_Equal.png",
};

//======================================================================================
// 魔導書の初期化処理
//======================================================================================
void InitMagicUI(void)
{
	LPDIRECT3DDEVICE9 pDevice;	// デバイスへのポインタ
	// デバイスの取得
	pDevice = GetDevice();

	// UI情報の初期化
	for (int nCntPlayer = 0; nCntPlayer < MAX_PLAYER; nCntPlayer++)
	{
		for (int nCntUI = 0; nCntUI < MAX_MAGICUI_TYPE; nCntUI++)
		{
			g_aMagicUI[nCntPlayer].aMagicUI[nCntUI].tex = MAGICUI_TEX_MAGICNULL;			// テクスチャの初期化
			g_aMagicUI[nCntPlayer].aMagicUI[nCntUI].type = MAGICUI_TYPE_0COMMAND0;			// UIの種類の初期化
			g_aMagicUI[nCntPlayer].aMagicUI[nCntUI].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);	// 位置の初期化
			g_aMagicUI[nCntPlayer].aMagicUI[nCntUI].fWidth				= 0.0f;				// 幅の初期化
			g_aMagicUI[nCntPlayer].aMagicUI[nCntUI].fHeight			= 0.0f;				// 高さの初期化
			g_aMagicUI[nCntPlayer].aMagicUI[nCntUI].fHeightUp			= 0.0f;				// 高さ[上]の初期化
			g_aMagicUI[nCntPlayer].aMagicUI[nCntUI].fHeightDown		= 0.0f;				// 高さ[下]の初期化
			g_aMagicUI[nCntPlayer].aMagicUI[nCntUI].fHeightDestUp		= 0.0f;				// 高さ[上]の目的地の初期化
			g_aMagicUI[nCntPlayer].aMagicUI[nCntUI].fHeightDestDown	= 0.0f;				// 高さ[下]の目的地の初期化
			g_aMagicUI[nCntPlayer].aMagicUI[nCntUI].bDisp = false;							// 表示状態の初期化
		}
		g_aMagicUI[nCntPlayer].state = MAGICUISTATE_NONDISPLAY;		// 画面外にある
		g_aMagicUI[nCntPlayer].bDisp = false;						// 全体の表示状態の初期化
		g_aMagicUI[nCntPlayer].nCounterUI = 0;						// 発動魔法の初期化
		g_aMagicUI[nCntPlayer].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);	// 位置の初期化
		g_aMagicUI[nCntPlayer].nFrame = 0;							// 現在のフレームを初期化
		g_aMagicUI[nCntPlayer].nNumFrame = FRAME;					// 出現の管理を行うフレーム数
	}

	// テクスチャの読み込み
	for (int nCntMagicUI = 0; nCntMagicUI < MAX_MAGICUI_TEX; nCntMagicUI++)
	{
		D3DXCreateTextureFromFile(pDevice, c_apFilenameMagicUI[nCntMagicUI], &g_apTextureMagicUI[nCntMagicUI]);
	}

	// 操作方法の状態を取得
	OPERATIONTYPE operationType = GetOperationType();

	// 頂点バッファの生成
	pDevice->CreateVertexBuffer(sizeof(VERTEX_2D) * 4 * MAX_MAGICUI_TYPE * MAX_PLAYER, D3DUSAGE_WRITEONLY, FVF_VERTEX_2D, D3DPOOL_MANAGED, &g_pVtxBuffMagicUI, NULL);
	
	VERTEX_2D* pVtx;
	// 頂点バッファをロックし、頂点情報へのポインタを取得
	g_pVtxBuffMagicUI->Lock(0, 0, (void**)&pVtx, 0);

	for (int nCntPlayer = 0; nCntPlayer < MAX_PLAYER; nCntPlayer++)
	{
		switch (operationType)	// 操作人数に応じてUIの位置を変更
		{
		case OPERATIONTYPE_2P:	// 2人操作
			switch (nCntPlayer)
			{
			case 0:
				g_aMagicUI[nCntPlayer].pos = LEFT_POS;
				g_aMagicUI[nCntPlayer].posDest = LEFT_POS;
				g_aMagicUI[nCntPlayer].bDisp = true;

				break;
			case 1:
				g_aMagicUI[nCntPlayer].pos = RIGHT_POS;
				g_aMagicUI[nCntPlayer].posDest = RIGHT_POS;
				g_aMagicUI[nCntPlayer].bDisp = true;

				break;
			}
			break;

		default:	// 1人操作
			if (nCntPlayer > 0)
			{
				pVtx += MAX_MAGICUI_TYPE * 4;
				continue;
			}
			g_aMagicUI[nCntPlayer].bDisp = true;
			g_aMagicUI[nCntPlayer].pos = RIGHT_POS;
			g_aMagicUI[nCntPlayer].posDest = RIGHT_POS;
			break;
		}

		ResetMagicUI(nCntPlayer);

		for (int nCntUI = 0; nCntUI < MAX_MAGICUI_TYPE; nCntUI++, pVtx += 4)	// 頂点バッファに数値を代入
		{
			// 頂点座標の設定
			pVtx[0].pos.x = g_aMagicUI[nCntPlayer].pos.x + g_aMagicUI[nCntPlayer].aMagicUI[nCntUI].pos.x - g_aMagicUI[nCntPlayer].aMagicUI[nCntUI].fWidth;
			pVtx[0].pos.y = g_aMagicUI[nCntPlayer].pos.y + g_aMagicUI[nCntPlayer].aMagicUI[nCntUI].pos.y - g_aMagicUI[nCntPlayer].aMagicUI[nCntUI].fHeight;
			pVtx[0].pos.z = 0.0f;
			pVtx[1].pos.x = g_aMagicUI[nCntPlayer].pos.x + g_aMagicUI[nCntPlayer].aMagicUI[nCntUI].pos.x + g_aMagicUI[nCntPlayer].aMagicUI[nCntUI].fWidth;
			pVtx[1].pos.y = g_aMagicUI[nCntPlayer].pos.y + g_aMagicUI[nCntPlayer].aMagicUI[nCntUI].pos.y - g_aMagicUI[nCntPlayer].aMagicUI[nCntUI].fHeight;
			pVtx[1].pos.z = 0.0f;
			pVtx[2].pos.x = g_aMagicUI[nCntPlayer].pos.x + g_aMagicUI[nCntPlayer].aMagicUI[nCntUI].pos.x - g_aMagicUI[nCntPlayer].aMagicUI[nCntUI].fWidth;
			pVtx[2].pos.y = g_aMagicUI[nCntPlayer].pos.y + g_aMagicUI[nCntPlayer].aMagicUI[nCntUI].pos.y + g_aMagicUI[nCntPlayer].aMagicUI[nCntUI].fHeight;
			pVtx[2].pos.z = 0.0f;
			pVtx[3].pos.x = g_aMagicUI[nCntPlayer].pos.x + g_aMagicUI[nCntPlayer].aMagicUI[nCntUI].pos.x + g_aMagicUI[nCntPlayer].aMagicUI[nCntUI].fWidth;
			pVtx[3].pos.y = g_aMagicUI[nCntPlayer].pos.y + g_aMagicUI[nCntPlayer].aMagicUI[nCntUI].pos.y + g_aMagicUI[nCntPlayer].aMagicUI[nCntUI].fHeight;
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
	g_pVtxBuffMagicUI->Unlock();
}

//======================================================================================
// 魔導書の終了処理
//======================================================================================
void UninitMagicUI(void)
{

	// テクスチャの破棄
	for (int nCntMagicUI = 0; nCntMagicUI < MAX_MAGICUI_TEX; nCntMagicUI++)
	{
		if (g_apTextureMagicUI[nCntMagicUI] != NULL)
		{
			g_apTextureMagicUI[nCntMagicUI]->Release();
			g_apTextureMagicUI[nCntMagicUI] = NULL;
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
	// 操作方法の状態を取得
	OPERATIONTYPE operationType = GetOperationType();

	// playerの情報を取得
	Player* pPlayer = GetPlayer();

	if (GetKeyboardTrigger(DIK_8) == true)
	{
		SetMagicUI(0);
		if (operationType == OPERATIONTYPE_2P)
		{
			SetMagicUI(1);
		}
	}
	if (GetKeyboardTrigger(DIK_9) == true)
	{
		DisappearMagicUI(0);
		if (operationType == OPERATIONTYPE_2P)
		{
			DisappearMagicUI(1);
		}
	}
	VERTEX_2D* pVtx;
	// 頂点バッファをロックし、頂点情報へのポインタを取得
	g_pVtxBuffMagicUI->Lock(0, 0, (void**)&pVtx, 0);

	for (int nCntPlayer = 0; nCntPlayer < MAX_PLAYER; nCntPlayer++)
	{
		float fDiffKeyUp = 0.0f;	// 現在の位置と目的の位置の差分を計算[上]
		float fDiffKeyDown = 0.0f;	// 現在の位置と目的の位置の差分を計算[下]
		float fRateKey = (float)g_aMagicUI[nCntPlayer].nFrame / (float)g_aMagicUI[nCntPlayer].nNumFrame;	// フレームの差分を求める

		// プレイヤーの魔法を記録
		SetPlayerMagic(nCntPlayer);

		for (int nCntUI = 0; nCntUI < MAX_MAGICUI_TYPE; nCntUI++, pVtx += 4)	// 頂点バッファに数値を代入
		{
			// 頂点座標の設定
			pVtx[0].pos.x = g_aMagicUI[nCntPlayer].pos.x + g_aMagicUI[nCntPlayer].aMagicUI[nCntUI].pos.x - g_aMagicUI[nCntPlayer].aMagicUI[nCntUI].fWidth;
			pVtx[0].pos.y = g_aMagicUI[nCntPlayer].pos.y + g_aMagicUI[nCntPlayer].aMagicUI[nCntUI].pos.y - g_aMagicUI[nCntPlayer].aMagicUI[nCntUI].fHeight;
			pVtx[0].pos.z = 0.0f;
			pVtx[1].pos.x = g_aMagicUI[nCntPlayer].pos.x + g_aMagicUI[nCntPlayer].aMagicUI[nCntUI].pos.x + g_aMagicUI[nCntPlayer].aMagicUI[nCntUI].fWidth;
			pVtx[1].pos.y = g_aMagicUI[nCntPlayer].pos.y + g_aMagicUI[nCntPlayer].aMagicUI[nCntUI].pos.y - g_aMagicUI[nCntPlayer].aMagicUI[nCntUI].fHeight;
			pVtx[1].pos.z = 0.0f;
			pVtx[2].pos.x = g_aMagicUI[nCntPlayer].pos.x + g_aMagicUI[nCntPlayer].aMagicUI[nCntUI].pos.x - g_aMagicUI[nCntPlayer].aMagicUI[nCntUI].fWidth;
			pVtx[2].pos.y = g_aMagicUI[nCntPlayer].pos.y + g_aMagicUI[nCntPlayer].aMagicUI[nCntUI].pos.y + g_aMagicUI[nCntPlayer].aMagicUI[nCntUI].fHeight;
			pVtx[2].pos.z = 0.0f;
			pVtx[3].pos.x = g_aMagicUI[nCntPlayer].pos.x + g_aMagicUI[nCntPlayer].aMagicUI[nCntUI].pos.x + g_aMagicUI[nCntPlayer].aMagicUI[nCntUI].fWidth;
			pVtx[3].pos.y = g_aMagicUI[nCntPlayer].pos.y + g_aMagicUI[nCntPlayer].aMagicUI[nCntUI].pos.y + g_aMagicUI[nCntPlayer].aMagicUI[nCntUI].fHeight;
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
	g_pVtxBuffMagicUI->Unlock();

}

//======================================================================================
// 魔導書の描画処理
//======================================================================================
void DrawMagicUI(void)
{
	// 一時的にfogを切る
	SetFogEnable(false);

	LPDIRECT3DDEVICE9 pDevice;	// デバイスへのポインタ
	// デバイスの取得
	pDevice = GetDevice();

	// 頂点バッファをデータストリームに設定
	pDevice->SetStreamSource(0, g_pVtxBuffMagicUI, 0, sizeof(VERTEX_2D));

	// 頂点フォーマットの設定
	pDevice->SetFVF(FVF_VERTEX_2D);

	for (int nCntPlayer = 0; nCntPlayer < MAX_PLAYER; nCntPlayer++)
	{
		if (g_aMagicUI[nCntPlayer].bDisp == false)
		{
			continue;
		}

		for (int nCntUI = 0; nCntUI < MAX_MAGICUI_TYPE; nCntUI++)
		{
			if (g_aMagicUI[nCntPlayer].aMagicUI[nCntUI].bDisp == true)
			{
				// テクスチャの設定
				pDevice->SetTexture(0, g_apTextureMagicUI[g_aMagicUI[nCntPlayer].aMagicUI[nCntUI].tex]);

				// ポリゴンの描画
				pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, nCntUI * 4 + (nCntPlayer * MAX_MAGICUI_TYPE * 4), 2);
			}
		}
	}

	// fogを戻す
	SetFogEnable(true);
}

//======================================================================================
// 魔導書の中身の初期化処理
//======================================================================================
void ResetMagicUI(int nIdx)
{
	// 操作方法の状態を取得
	OPERATIONTYPE operationType = GetOperationType();

	for (int nCntUI = 0; nCntUI < MAX_MAGICUI_TYPE; nCntUI++)
	{
		switch (nCntUI)
		{
		case MAGICUI_TYPE_MAGICBOOK:	// 魔導書
			g_aMagicUI[nIdx].aMagicUI[MAGICUI_TYPE_MAGICBOOK].tex		= MAGICUI_TEX_MAGICBOOK;				// 魔導書
			g_aMagicUI[nIdx].aMagicUI[MAGICUI_TYPE_MAGICBOOK].type		= MAGICUI_TYPE_MAGICBOOK;				// UIの種類の初期化
			g_aMagicUI[nIdx].aMagicUI[MAGICUI_TYPE_MAGICBOOK].pos		= D3DXVECTOR3(0.0f, MAGICBOOK_Y, 0.0f);	// 位置の初期化
			g_aMagicUI[nIdx].aMagicUI[MAGICUI_TYPE_MAGICBOOK].fWidth	= MAGICUI_WIDTH;						// 幅の初期化
			g_aMagicUI[nIdx].aMagicUI[MAGICUI_TYPE_MAGICBOOK].fHeight	= MAGICUI_HEIGHT;						// 高さの初期化
			g_aMagicUI[nIdx].aMagicUI[MAGICUI_TYPE_MAGICBOOK].fHeightUp = 0.0f;				// 高さ[上]の初期化
			g_aMagicUI[nIdx].aMagicUI[MAGICUI_TYPE_MAGICBOOK].fHeightDown = 0.0f;				// 高さ[下]の初期化
			g_aMagicUI[nIdx].aMagicUI[MAGICUI_TYPE_MAGICBOOK].fHeightDestUp = 0.0f;				// 高さ[上]の目的地の初期化
			g_aMagicUI[nIdx].aMagicUI[MAGICUI_TYPE_MAGICBOOK].fHeightDestDown = 0.0f;				// 高さ[下]の目的地の初期化
			g_aMagicUI[nIdx].aMagicUI[MAGICUI_TYPE_MAGICBOOK].bDisp	= false;									// テクスチャの初期化
			break;

			// 1つ目の魔法
		case MAGICUI_TYPE_0COMMAND0:	// 1つ目のコマンド
			g_aMagicUI[nIdx].aMagicUI[MAGICUI_TYPE_0COMMAND0].tex		= MAGICUI_TEX_MAGICNULL;							// どの魔法も入力されていない
			g_aMagicUI[nIdx].aMagicUI[MAGICUI_TYPE_0COMMAND0].type		= MAGICUI_TYPE_0COMMAND0;							// UIの種類の初期化
			g_aMagicUI[nIdx].aMagicUI[MAGICUI_TYPE_0COMMAND0].pos		= D3DXVECTOR3(COMMAND1_POSX, MAGIC0_POSY, 0.0f);	// 位置の初期化
			g_aMagicUI[nIdx].aMagicUI[MAGICUI_TYPE_0COMMAND0].fWidth	= ICON_SIZE;										// 幅の初期化
			g_aMagicUI[nIdx].aMagicUI[MAGICUI_TYPE_0COMMAND0].fHeight	= ICON_SIZE;										// 高さの初期化
			g_aMagicUI[nIdx].aMagicUI[MAGICUI_TYPE_0COMMAND0].fHeightUp = 0.0f;				// 高さ[上]の初期化
			g_aMagicUI[nIdx].aMagicUI[MAGICUI_TYPE_0COMMAND0].fHeightDown = 0.0f;				// 高さ[下]の初期化
			g_aMagicUI[nIdx].aMagicUI[MAGICUI_TYPE_0COMMAND0].fHeightDestUp = 0.0f;				// 高さ[上]の目的地の初期化
			g_aMagicUI[nIdx].aMagicUI[MAGICUI_TYPE_0COMMAND0].fHeightDestDown = 0.0f;				// 高さ[下]の目的地の初期化
			g_aMagicUI[nIdx].aMagicUI[MAGICUI_TYPE_0COMMAND0].bDisp	= false;												// テクスチャの初期化
			break;

		case MAGICUI_TYPE_0_01ADD:	// 01の合成
			g_aMagicUI[nIdx].aMagicUI[MAGICUI_TYPE_0_01ADD].tex		= MAGICUI_TEX_ADD;								// どの魔法も入力されていない
			g_aMagicUI[nIdx].aMagicUI[MAGICUI_TYPE_0_01ADD].type		= MAGICUI_TYPE_0_01ADD;				// UIの種類の初期化
			g_aMagicUI[nIdx].aMagicUI[MAGICUI_TYPE_0_01ADD].pos		= D3DXVECTOR3(ADD01_POSX, MAGIC0_POSY, 0.0f);	// 位置の初期化
			g_aMagicUI[nIdx].aMagicUI[MAGICUI_TYPE_0_01ADD].fWidth		= MATH_SIZE;							// 幅の初期化
			g_aMagicUI[nIdx].aMagicUI[MAGICUI_TYPE_0_01ADD].fHeight	= MATH_SIZE;									// 高さの初期化
			g_aMagicUI[nIdx].aMagicUI[MAGICUI_TYPE_0_01ADD].fHeightUp = 0.0f;				// 高さ[上]の初期化
			g_aMagicUI[nIdx].aMagicUI[MAGICUI_TYPE_0_01ADD].fHeightDown = 0.0f;				// 高さ[下]の初期化
			g_aMagicUI[nIdx].aMagicUI[MAGICUI_TYPE_0_01ADD].fHeightDestUp = 0.0f;				// 高さ[上]の目的地の初期化
			g_aMagicUI[nIdx].aMagicUI[MAGICUI_TYPE_0_01ADD].fHeightDestDown = 0.0f;				// 高さ[下]の目的地の初期化
			g_aMagicUI[nIdx].aMagicUI[MAGICUI_TYPE_0_01ADD].bDisp		= false;											// テクスチャの初期化
			break;

		case MAGICUI_TYPE_0COMMAND1:	// 2つ目のコマンド
			g_aMagicUI[nIdx].aMagicUI[MAGICUI_TYPE_0COMMAND1].tex = MAGICUI_TEX_MAGICNULL;					// どの魔法も入力されていない
			g_aMagicUI[nIdx].aMagicUI[MAGICUI_TYPE_0COMMAND1].type = MAGICUI_TYPE_0COMMAND1;				// UIの種類の初期化
			g_aMagicUI[nIdx].aMagicUI[MAGICUI_TYPE_0COMMAND1].pos = D3DXVECTOR3(COMMAND2_POSX, MAGIC0_POSY, 0.0f);	// 位置の初期化
			g_aMagicUI[nIdx].aMagicUI[MAGICUI_TYPE_0COMMAND1].fWidth = ICON_SIZE;						// 幅の初期化
			g_aMagicUI[nIdx].aMagicUI[MAGICUI_TYPE_0COMMAND1].fHeight = ICON_SIZE;						// 高さの初期化
			g_aMagicUI[nIdx].aMagicUI[MAGICUI_TYPE_0COMMAND1].fHeightUp = 0.0f;				// 高さ[上]の初期化
			g_aMagicUI[nIdx].aMagicUI[MAGICUI_TYPE_0COMMAND1].fHeightDown = 0.0f;				// 高さ[下]の初期化
			g_aMagicUI[nIdx].aMagicUI[MAGICUI_TYPE_0COMMAND1].fHeightDestUp = 0.0f;				// 高さ[上]の目的地の初期化
			g_aMagicUI[nIdx].aMagicUI[MAGICUI_TYPE_0COMMAND1].fHeightDestDown = 0.0f;				// 高さ[下]の目的地の初期化
			g_aMagicUI[nIdx].aMagicUI[MAGICUI_TYPE_0COMMAND1].bDisp = false;								// テクスチャの初期化
			break;

		case MAGICUI_TYPE_0_12ADD:	// 12の合成
			g_aMagicUI[nIdx].aMagicUI[MAGICUI_TYPE_0_12ADD].tex = MAGICUI_TEX_ADD;								// どの魔法も入力されていない
			g_aMagicUI[nIdx].aMagicUI[MAGICUI_TYPE_0_12ADD].type = MAGICUI_TYPE_0_12ADD;							// UIの種類の初期化
			g_aMagicUI[nIdx].aMagicUI[MAGICUI_TYPE_0_12ADD].pos = D3DXVECTOR3(ADD12_POSX, MAGIC0_POSY, 0.0f);	// 位置の初期化
			g_aMagicUI[nIdx].aMagicUI[MAGICUI_TYPE_0_12ADD].fWidth = MATH_SIZE;									// 幅の初期化
			g_aMagicUI[nIdx].aMagicUI[MAGICUI_TYPE_0_12ADD].fHeight = MATH_SIZE;									// 高さの初期化
			g_aMagicUI[nIdx].aMagicUI[MAGICUI_TYPE_0_12ADD].fHeightUp = 0.0f;				// 高さ[上]の初期化
			g_aMagicUI[nIdx].aMagicUI[MAGICUI_TYPE_0_12ADD].fHeightDown = 0.0f;				// 高さ[下]の初期化
			g_aMagicUI[nIdx].aMagicUI[MAGICUI_TYPE_0_12ADD].fHeightDestUp = 0.0f;				// 高さ[上]の目的地の初期化
			g_aMagicUI[nIdx].aMagicUI[MAGICUI_TYPE_0_12ADD].fHeightDestDown = 0.0f;				// 高さ[下]の目的地の初期化
			g_aMagicUI[nIdx].aMagicUI[MAGICUI_TYPE_0_12ADD].bDisp = false;											// テクスチャの初期化
			break;

		case MAGICUI_TYPE_0COMMAND2:	// 3つ目のコマンド
			g_aMagicUI[nIdx].aMagicUI[MAGICUI_TYPE_0COMMAND2].tex = MAGICUI_TEX_MAGICNULL;							// どの魔法も入力されていない
			g_aMagicUI[nIdx].aMagicUI[MAGICUI_TYPE_0COMMAND2].type = MAGICUI_TYPE_0COMMAND2;						// UIの種類の初期化
			g_aMagicUI[nIdx].aMagicUI[MAGICUI_TYPE_0COMMAND2].pos = D3DXVECTOR3(COMMAND3_POSX, MAGIC0_POSY, 0.0f);	// 位置の初期化
			g_aMagicUI[nIdx].aMagicUI[MAGICUI_TYPE_0COMMAND2].fWidth = ICON_SIZE;								// 幅の初期化
			g_aMagicUI[nIdx].aMagicUI[MAGICUI_TYPE_0COMMAND2].fHeight = ICON_SIZE;								// 高さの初期化
			g_aMagicUI[nIdx].aMagicUI[MAGICUI_TYPE_0COMMAND2].fHeightUp = 0.0f;				// 高さ[上]の初期化
			g_aMagicUI[nIdx].aMagicUI[MAGICUI_TYPE_0COMMAND2].fHeightDown = 0.0f;				// 高さ[下]の初期化
			g_aMagicUI[nIdx].aMagicUI[MAGICUI_TYPE_0COMMAND2].fHeightDestUp = 0.0f;				// 高さ[上]の目的地の初期化
			g_aMagicUI[nIdx].aMagicUI[MAGICUI_TYPE_0COMMAND2].fHeightDestDown = 0.0f;				// 高さ[下]の目的地の初期化
			g_aMagicUI[nIdx].aMagicUI[MAGICUI_TYPE_0COMMAND2].bDisp = false;										// テクスチャの初期化
			break;

		case MAGICUI_TYPE_0_EQUAL:	// 合成結果
			g_aMagicUI[nIdx].aMagicUI[MAGICUI_TYPE_0_EQUAL].tex = MAGICUI_TEX_EQUAL;							// どの魔法も入力されていない
			g_aMagicUI[nIdx].aMagicUI[MAGICUI_TYPE_0_EQUAL].type = MAGICUI_TYPE_0_EQUAL;						// UIの種類の初期化
			g_aMagicUI[nIdx].aMagicUI[MAGICUI_TYPE_0_EQUAL].pos = D3DXVECTOR3(ECUAL_POSX, MAGIC0_POSY, 0.0f);	// 位置の初期化
			g_aMagicUI[nIdx].aMagicUI[MAGICUI_TYPE_0_EQUAL].fWidth = MATH_SIZE;								// 幅の初期化
			g_aMagicUI[nIdx].aMagicUI[MAGICUI_TYPE_0_EQUAL].fHeight = MATH_SIZE;								// 高さの初期化
			g_aMagicUI[nIdx].aMagicUI[MAGICUI_TYPE_0_EQUAL].fHeightUp = 0.0f;				// 高さ[上]の初期化
			g_aMagicUI[nIdx].aMagicUI[MAGICUI_TYPE_0_EQUAL].fHeightDown = 0.0f;				// 高さ[下]の初期化
			g_aMagicUI[nIdx].aMagicUI[MAGICUI_TYPE_0_EQUAL].fHeightDestUp = 0.0f;				// 高さ[上]の目的地の初期化
			g_aMagicUI[nIdx].aMagicUI[MAGICUI_TYPE_0_EQUAL].fHeightDestDown = 0.0f;				// 高さ[下]の目的地の初期化
			g_aMagicUI[nIdx].aMagicUI[MAGICUI_TYPE_0_EQUAL].bDisp = false;										// テクスチャの初期化
			break;

		case MAGICUI_TYPE_0MAGIC:	// 発動された魔法
			g_aMagicUI[nIdx].aMagicUI[MAGICUI_TYPE_0MAGIC].tex = MAGICUI_TEX_NONE;					// どの魔法も入力されていない
			g_aMagicUI[nIdx].aMagicUI[MAGICUI_TYPE_0MAGIC].type = MAGICUI_TYPE_0MAGIC;				// UIの種類の初期化
			g_aMagicUI[nIdx].aMagicUI[MAGICUI_TYPE_0MAGIC].pos = D3DXVECTOR3(MAGIC_POSX, MAGIC0_POSY, 0.0f);	// 位置の初期化
			g_aMagicUI[nIdx].aMagicUI[MAGICUI_TYPE_0MAGIC].fWidth = ICON_SIZE;					// 幅の初期化
			g_aMagicUI[nIdx].aMagicUI[MAGICUI_TYPE_0MAGIC].fHeight = ICON_SIZE;					// 高さの初期化
			g_aMagicUI[nIdx].aMagicUI[MAGICUI_TYPE_0MAGIC].fHeightUp = 0.0f;				// 高さ[上]の初期化
			g_aMagicUI[nIdx].aMagicUI[MAGICUI_TYPE_0MAGIC].fHeightDown = 0.0f;				// 高さ[下]の初期化
			g_aMagicUI[nIdx].aMagicUI[MAGICUI_TYPE_0MAGIC].fHeightDestUp = 0.0f;				// 高さ[上]の目的地の初期化
			g_aMagicUI[nIdx].aMagicUI[MAGICUI_TYPE_0MAGIC].fHeightDestDown = 0.0f;				// 高さ[下]の目的地の初期化
			g_aMagicUI[nIdx].aMagicUI[MAGICUI_TYPE_0MAGIC].bDisp = false;							// テクスチャの初期化
			break;

			// 2つ目の魔法
		case MAGICUI_TYPE_1COMMAND0:	// 1つ目のコマンド
			g_aMagicUI[nIdx].aMagicUI[MAGICUI_TYPE_1COMMAND0].tex = MAGICUI_TEX_MAGICNULL;								// どの魔法も入力されていない
			g_aMagicUI[nIdx].aMagicUI[MAGICUI_TYPE_1COMMAND0].type = MAGICUI_TYPE_1COMMAND0;							// UIの種類の初期化
			g_aMagicUI[nIdx].aMagicUI[MAGICUI_TYPE_1COMMAND0].pos = D3DXVECTOR3(COMMAND1_POSX, MAGIC1_POSY, 0.0f);	// 位置の初期化
			g_aMagicUI[nIdx].aMagicUI[MAGICUI_TYPE_1COMMAND0].fWidth = ICON_SIZE;									// 幅の初期化
			g_aMagicUI[nIdx].aMagicUI[MAGICUI_TYPE_1COMMAND0].fHeight = ICON_SIZE;									// 高さの初期化
			g_aMagicUI[nIdx].aMagicUI[MAGICUI_TYPE_1COMMAND0].fHeightUp = 0.0f;				// 高さ[上]の初期化
			g_aMagicUI[nIdx].aMagicUI[MAGICUI_TYPE_1COMMAND0].fHeightDown = 0.0f;				// 高さ[下]の初期化
			g_aMagicUI[nIdx].aMagicUI[MAGICUI_TYPE_1COMMAND0].fHeightDestUp = 0.0f;				// 高さ[上]の目的地の初期化
			g_aMagicUI[nIdx].aMagicUI[MAGICUI_TYPE_1COMMAND0].fHeightDestDown = 0.0f;				// 高さ[下]の目的地の初期化
			g_aMagicUI[nIdx].aMagicUI[MAGICUI_TYPE_1COMMAND0].bDisp = false;											// テクスチャの初期化
			break;

		case MAGICUI_TYPE_1_01ADD:	// 01の合成
			g_aMagicUI[nIdx].aMagicUI[MAGICUI_TYPE_1_01ADD].tex = MAGICUI_TEX_ADD;								// どの魔法も入力されていない
			g_aMagicUI[nIdx].aMagicUI[MAGICUI_TYPE_1_01ADD].type = MAGICUI_TYPE_1_01ADD;							// UIの種類の初期化
			g_aMagicUI[nIdx].aMagicUI[MAGICUI_TYPE_1_01ADD].pos = D3DXVECTOR3(ADD01_POSX, MAGIC1_POSY, 0.0f);	// 位置の初期化
			g_aMagicUI[nIdx].aMagicUI[MAGICUI_TYPE_1_01ADD].fWidth = MATH_SIZE;									// 幅の初期化
			g_aMagicUI[nIdx].aMagicUI[MAGICUI_TYPE_1_01ADD].fHeight = MATH_SIZE;									// 高さの初期化
			g_aMagicUI[nIdx].aMagicUI[MAGICUI_TYPE_1_01ADD].fHeightUp = 0.0f;				// 高さ[上]の初期化
			g_aMagicUI[nIdx].aMagicUI[MAGICUI_TYPE_1_01ADD].fHeightDown = 0.0f;				// 高さ[下]の初期化
			g_aMagicUI[nIdx].aMagicUI[MAGICUI_TYPE_1_01ADD].fHeightDestUp = 0.0f;				// 高さ[上]の目的地の初期化
			g_aMagicUI[nIdx].aMagicUI[MAGICUI_TYPE_1_01ADD].fHeightDestDown = 0.0f;				// 高さ[下]の目的地の初期化
			g_aMagicUI[nIdx].aMagicUI[MAGICUI_TYPE_1_01ADD].bDisp = false;											// テクスチャの初期化
			break;

		case MAGICUI_TYPE_1COMMAND1:	// 2つ目のコマンド
			g_aMagicUI[nIdx].aMagicUI[MAGICUI_TYPE_1COMMAND1].tex = MAGICUI_TEX_MAGICNULL;					// どの魔法も入力されていない
			g_aMagicUI[nIdx].aMagicUI[MAGICUI_TYPE_1COMMAND1].type = MAGICUI_TYPE_1COMMAND1;				// UIの種類の初期化
			g_aMagicUI[nIdx].aMagicUI[MAGICUI_TYPE_1COMMAND1].pos = D3DXVECTOR3(COMMAND2_POSX, MAGIC1_POSY, 0.0f);	// 位置の初期化
			g_aMagicUI[nIdx].aMagicUI[MAGICUI_TYPE_1COMMAND1].fWidth = ICON_SIZE;						// 幅の初期化
			g_aMagicUI[nIdx].aMagicUI[MAGICUI_TYPE_1COMMAND1].fHeight = ICON_SIZE;						// 高さの初期化
			g_aMagicUI[nIdx].aMagicUI[MAGICUI_TYPE_1COMMAND1].fHeightUp = 0.0f;				// 高さ[上]の初期化
			g_aMagicUI[nIdx].aMagicUI[MAGICUI_TYPE_1COMMAND1].fHeightDown = 0.0f;				// 高さ[下]の初期化
			g_aMagicUI[nIdx].aMagicUI[MAGICUI_TYPE_1COMMAND1].fHeightDestUp = 0.0f;				// 高さ[上]の目的地の初期化
			g_aMagicUI[nIdx].aMagicUI[MAGICUI_TYPE_1COMMAND1].fHeightDestDown = 0.0f;				// 高さ[下]の目的地の初期化
			g_aMagicUI[nIdx].aMagicUI[MAGICUI_TYPE_1COMMAND1].bDisp = false;								// テクスチャの初期化
			break;

		case MAGICUI_TYPE_1_12ADD:	// 12の合成
			g_aMagicUI[nIdx].aMagicUI[MAGICUI_TYPE_1_12ADD].tex = MAGICUI_TEX_ADD;								// どの魔法も入力されていない
			g_aMagicUI[nIdx].aMagicUI[MAGICUI_TYPE_1_12ADD].type = MAGICUI_TYPE_1_12ADD;							// UIの種類の初期化
			g_aMagicUI[nIdx].aMagicUI[MAGICUI_TYPE_1_12ADD].pos = D3DXVECTOR3(ADD12_POSX, MAGIC1_POSY, 0.0f);	// 位置の初期化
			g_aMagicUI[nIdx].aMagicUI[MAGICUI_TYPE_1_12ADD].fWidth = MATH_SIZE;									// 幅の初期化
			g_aMagicUI[nIdx].aMagicUI[MAGICUI_TYPE_1_12ADD].fHeight = MATH_SIZE;									// 高さの初期化
			g_aMagicUI[nIdx].aMagicUI[MAGICUI_TYPE_1_12ADD].fHeightUp = 0.0f;				// 高さ[上]の初期化
			g_aMagicUI[nIdx].aMagicUI[MAGICUI_TYPE_1_12ADD].fHeightDown = 0.0f;				// 高さ[下]の初期化
			g_aMagicUI[nIdx].aMagicUI[MAGICUI_TYPE_1_12ADD].fHeightDestUp = 0.0f;				// 高さ[上]の目的地の初期化
			g_aMagicUI[nIdx].aMagicUI[MAGICUI_TYPE_1_12ADD].fHeightDestDown = 0.0f;				// 高さ[下]の目的地の初期化
			g_aMagicUI[nIdx].aMagicUI[MAGICUI_TYPE_1_12ADD].bDisp = false;											// テクスチャの初期化
			break;

		case MAGICUI_TYPE_1COMMAND2:	// 3つ目のコマンド
			g_aMagicUI[nIdx].aMagicUI[MAGICUI_TYPE_1COMMAND2].tex = MAGICUI_TEX_MAGICNULL;							// どの魔法も入力されていない
			g_aMagicUI[nIdx].aMagicUI[MAGICUI_TYPE_1COMMAND2].type = MAGICUI_TYPE_1COMMAND2;						// UIの種類の初期化
			g_aMagicUI[nIdx].aMagicUI[MAGICUI_TYPE_1COMMAND2].pos = D3DXVECTOR3(COMMAND3_POSX, MAGIC1_POSY, 0.0f);	// 位置の初期化
			g_aMagicUI[nIdx].aMagicUI[MAGICUI_TYPE_1COMMAND2].fWidth = ICON_SIZE;								// 幅の初期化
			g_aMagicUI[nIdx].aMagicUI[MAGICUI_TYPE_1COMMAND2].fHeight = ICON_SIZE;								// 高さの初期化
			g_aMagicUI[nIdx].aMagicUI[MAGICUI_TYPE_1COMMAND2].fHeightUp = 0.0f;				// 高さ[上]の初期化
			g_aMagicUI[nIdx].aMagicUI[MAGICUI_TYPE_1COMMAND2].fHeightDown = 0.0f;				// 高さ[下]の初期化
			g_aMagicUI[nIdx].aMagicUI[MAGICUI_TYPE_1COMMAND2].fHeightDestUp = 0.0f;				// 高さ[上]の目的地の初期化
			g_aMagicUI[nIdx].aMagicUI[MAGICUI_TYPE_1COMMAND2].fHeightDestDown = 0.0f;				// 高さ[下]の目的地の初期化
			g_aMagicUI[nIdx].aMagicUI[MAGICUI_TYPE_1COMMAND2].bDisp = false;										// テクスチャの初期化
			break;

		case MAGICUI_TYPE_1_EQUAL:	// 合成結果
			g_aMagicUI[nIdx].aMagicUI[MAGICUI_TYPE_1_EQUAL].tex = MAGICUI_TEX_EQUAL;							// どの魔法も入力されていない
			g_aMagicUI[nIdx].aMagicUI[MAGICUI_TYPE_1_EQUAL].type = MAGICUI_TYPE_1_EQUAL;						// UIの種類の初期化
			g_aMagicUI[nIdx].aMagicUI[MAGICUI_TYPE_1_EQUAL].pos = D3DXVECTOR3(ECUAL_POSX, MAGIC1_POSY, 0.0f);	// 位置の初期化
			g_aMagicUI[nIdx].aMagicUI[MAGICUI_TYPE_1_EQUAL].fWidth = MATH_SIZE;								// 幅の初期化
			g_aMagicUI[nIdx].aMagicUI[MAGICUI_TYPE_1_EQUAL].fHeight = MATH_SIZE;								// 高さの初期化
			g_aMagicUI[nIdx].aMagicUI[MAGICUI_TYPE_1_EQUAL].fHeightUp = 0.0f;				// 高さ[上]の初期化
			g_aMagicUI[nIdx].aMagicUI[MAGICUI_TYPE_1_EQUAL].fHeightDown = 0.0f;				// 高さ[下]の初期化
			g_aMagicUI[nIdx].aMagicUI[MAGICUI_TYPE_1_EQUAL].fHeightDestUp = 0.0f;				// 高さ[上]の目的地の初期化
			g_aMagicUI[nIdx].aMagicUI[MAGICUI_TYPE_1_EQUAL].fHeightDestDown = 0.0f;				// 高さ[下]の目的地の初期化
			g_aMagicUI[nIdx].aMagicUI[MAGICUI_TYPE_1_EQUAL].bDisp = false;										// テクスチャの初期化
			break;

		case MAGICUI_TYPE_1MAGIC:	// 発動された魔法
			g_aMagicUI[nIdx].aMagicUI[MAGICUI_TYPE_1MAGIC].tex = MAGICUI_TEX_NONE;					// どの魔法も入力されていない
			g_aMagicUI[nIdx].aMagicUI[MAGICUI_TYPE_1MAGIC].type = MAGICUI_TYPE_1MAGIC;				// UIの種類の初期化
			g_aMagicUI[nIdx].aMagicUI[MAGICUI_TYPE_1MAGIC].pos = D3DXVECTOR3(MAGIC_POSX, MAGIC1_POSY, 0.0f);	// 位置の初期化
			g_aMagicUI[nIdx].aMagicUI[MAGICUI_TYPE_1MAGIC].fWidth = ICON_SIZE;					// 幅の初期化
			g_aMagicUI[nIdx].aMagicUI[MAGICUI_TYPE_1MAGIC].fHeight = ICON_SIZE;					// 高さの初期化
			g_aMagicUI[nIdx].aMagicUI[MAGICUI_TYPE_1MAGIC].fHeightUp = 0.0f;				// 高さ[上]の初期化
			g_aMagicUI[nIdx].aMagicUI[MAGICUI_TYPE_1MAGIC].fHeightDown = 0.0f;				// 高さ[下]の初期化
			g_aMagicUI[nIdx].aMagicUI[MAGICUI_TYPE_1MAGIC].fHeightDestUp = 0.0f;				// 高さ[上]の目的地の初期化
			g_aMagicUI[nIdx].aMagicUI[MAGICUI_TYPE_1MAGIC].fHeightDestDown = 0.0f;				// 高さ[下]の目的地の初期化
			g_aMagicUI[nIdx].aMagicUI[MAGICUI_TYPE_1MAGIC].bDisp = false;							// テクスチャの初期化
			break;

			// 3つ目の魔法
		case MAGICUI_TYPE_2COMMAND0:	// 1つ目のコマンド
			g_aMagicUI[nIdx].aMagicUI[MAGICUI_TYPE_2COMMAND0].tex = MAGICUI_TEX_MAGICNULL;								// どの魔法も入力されていない
			g_aMagicUI[nIdx].aMagicUI[MAGICUI_TYPE_2COMMAND0].type = MAGICUI_TYPE_2COMMAND0;							// UIの種類の初期化
			g_aMagicUI[nIdx].aMagicUI[MAGICUI_TYPE_2COMMAND0].pos = D3DXVECTOR3(COMMAND1_POSX, MAGIC2_POSY, 0.0f);	// 位置の初期化
			g_aMagicUI[nIdx].aMagicUI[MAGICUI_TYPE_2COMMAND0].fWidth = ICON_SIZE;									// 幅の初期化
			g_aMagicUI[nIdx].aMagicUI[MAGICUI_TYPE_2COMMAND0].fHeight = ICON_SIZE;									// 高さの初期化
			g_aMagicUI[nIdx].aMagicUI[MAGICUI_TYPE_2COMMAND0].fHeightUp = 0.0f;				// 高さ[上]の初期化
			g_aMagicUI[nIdx].aMagicUI[MAGICUI_TYPE_2COMMAND0].fHeightDown = 0.0f;				// 高さ[下]の初期化
			g_aMagicUI[nIdx].aMagicUI[MAGICUI_TYPE_2COMMAND0].fHeightDestUp = 0.0f;				// 高さ[上]の目的地の初期化
			g_aMagicUI[nIdx].aMagicUI[MAGICUI_TYPE_2COMMAND0].fHeightDestDown = 0.0f;				// 高さ[下]の目的地の初期化
			g_aMagicUI[nIdx].aMagicUI[MAGICUI_TYPE_2COMMAND0].bDisp = false;											// テクスチャの初期化
			break;

		case MAGICUI_TYPE_2_01ADD:	// 01の合成
			g_aMagicUI[nIdx].aMagicUI[MAGICUI_TYPE_2_01ADD].tex = MAGICUI_TEX_ADD;								// どの魔法も入力されていない
			g_aMagicUI[nIdx].aMagicUI[MAGICUI_TYPE_2_01ADD].type = MAGICUI_TYPE_2_01ADD;							// UIの種類の初期化
			g_aMagicUI[nIdx].aMagicUI[MAGICUI_TYPE_2_01ADD].pos = D3DXVECTOR3(ADD01_POSX, MAGIC2_POSY, 0.0f);	// 位置の初期化
			g_aMagicUI[nIdx].aMagicUI[MAGICUI_TYPE_2_01ADD].fWidth = MATH_SIZE;									// 幅の初期化
			g_aMagicUI[nIdx].aMagicUI[MAGICUI_TYPE_2_01ADD].fHeight = MATH_SIZE;									// 高さの初期化
			g_aMagicUI[nIdx].aMagicUI[MAGICUI_TYPE_2_01ADD].fHeightUp = 0.0f;				// 高さ[上]の初期化
			g_aMagicUI[nIdx].aMagicUI[MAGICUI_TYPE_2_01ADD].fHeightDown = 0.0f;				// 高さ[下]の初期化
			g_aMagicUI[nIdx].aMagicUI[MAGICUI_TYPE_2_01ADD].fHeightDestUp = 0.0f;				// 高さ[上]の目的地の初期化
			g_aMagicUI[nIdx].aMagicUI[MAGICUI_TYPE_2_01ADD].fHeightDestDown = 0.0f;				// 高さ[下]の目的地の初期化
			g_aMagicUI[nIdx].aMagicUI[MAGICUI_TYPE_2_01ADD].bDisp = false;											// テクスチャの初期化
			break;

		case MAGICUI_TYPE_2COMMAND1:	// 2つ目のコマンド
			g_aMagicUI[nIdx].aMagicUI[MAGICUI_TYPE_2COMMAND1].tex = MAGICUI_TEX_MAGICNULL;					// どの魔法も入力されていない
			g_aMagicUI[nIdx].aMagicUI[MAGICUI_TYPE_2COMMAND1].type = MAGICUI_TYPE_2COMMAND1;				// UIの種類の初期化
			g_aMagicUI[nIdx].aMagicUI[MAGICUI_TYPE_2COMMAND1].pos = D3DXVECTOR3(COMMAND2_POSX, MAGIC2_POSY, 0.0f);	// 位置の初期化
			g_aMagicUI[nIdx].aMagicUI[MAGICUI_TYPE_2COMMAND1].fWidth = ICON_SIZE;						// 幅の初期化
			g_aMagicUI[nIdx].aMagicUI[MAGICUI_TYPE_2COMMAND1].fHeight = ICON_SIZE;						// 高さの初期化
			g_aMagicUI[nIdx].aMagicUI[MAGICUI_TYPE_2COMMAND1].fHeightUp = 0.0f;				// 高さ[上]の初期化
			g_aMagicUI[nIdx].aMagicUI[MAGICUI_TYPE_2COMMAND1].fHeightDown = 0.0f;				// 高さ[下]の初期化
			g_aMagicUI[nIdx].aMagicUI[MAGICUI_TYPE_2COMMAND1].fHeightDestUp = 0.0f;				// 高さ[上]の目的地の初期化
			g_aMagicUI[nIdx].aMagicUI[MAGICUI_TYPE_2COMMAND1].fHeightDestDown = 0.0f;				// 高さ[下]の目的地の初期化
			g_aMagicUI[nIdx].aMagicUI[MAGICUI_TYPE_2COMMAND1].bDisp = false;								// テクスチャの初期化
			break;

		case MAGICUI_TYPE_2_12ADD:	// 12の合成
			g_aMagicUI[nIdx].aMagicUI[MAGICUI_TYPE_2_12ADD].tex = MAGICUI_TEX_ADD;								// どの魔法も入力されていない
			g_aMagicUI[nIdx].aMagicUI[MAGICUI_TYPE_2_12ADD].type = MAGICUI_TYPE_2_12ADD;							// UIの種類の初期化
			g_aMagicUI[nIdx].aMagicUI[MAGICUI_TYPE_2_12ADD].pos = D3DXVECTOR3(ADD12_POSX, MAGIC2_POSY, 0.0f);	// 位置の初期化
			g_aMagicUI[nIdx].aMagicUI[MAGICUI_TYPE_2_12ADD].fWidth = MATH_SIZE;									// 幅の初期化
			g_aMagicUI[nIdx].aMagicUI[MAGICUI_TYPE_2_12ADD].fHeight = MATH_SIZE;									// 高さの初期化
			g_aMagicUI[nIdx].aMagicUI[MAGICUI_TYPE_2_12ADD].fHeightUp = 0.0f;				// 高さ[上]の初期化
			g_aMagicUI[nIdx].aMagicUI[MAGICUI_TYPE_2_12ADD].fHeightDown = 0.0f;				// 高さ[下]の初期化
			g_aMagicUI[nIdx].aMagicUI[MAGICUI_TYPE_2_12ADD].fHeightDestUp = 0.0f;				// 高さ[上]の目的地の初期化
			g_aMagicUI[nIdx].aMagicUI[MAGICUI_TYPE_2_12ADD].fHeightDestDown = 0.0f;				// 高さ[下]の目的地の初期化
			g_aMagicUI[nIdx].aMagicUI[MAGICUI_TYPE_2_12ADD].bDisp = false;											// テクスチャの初期化
			break;

		case MAGICUI_TYPE_2COMMAND2:	// 3つ目のコマンド
			g_aMagicUI[nIdx].aMagicUI[MAGICUI_TYPE_2COMMAND2].tex = MAGICUI_TEX_MAGICNULL;							// どの魔法も入力されていない
			g_aMagicUI[nIdx].aMagicUI[MAGICUI_TYPE_2COMMAND2].type = MAGICUI_TYPE_2COMMAND2;						// UIの種類の初期化
			g_aMagicUI[nIdx].aMagicUI[MAGICUI_TYPE_2COMMAND2].pos = D3DXVECTOR3(COMMAND3_POSX, MAGIC2_POSY, 0.0f);	// 位置の初期化
			g_aMagicUI[nIdx].aMagicUI[MAGICUI_TYPE_2COMMAND2].fWidth = ICON_SIZE;								// 幅の初期化
			g_aMagicUI[nIdx].aMagicUI[MAGICUI_TYPE_2COMMAND2].fHeight = ICON_SIZE;								// 高さの初期化
			g_aMagicUI[nIdx].aMagicUI[MAGICUI_TYPE_2COMMAND2].fHeightUp = 0.0f;				// 高さ[上]の初期化
			g_aMagicUI[nIdx].aMagicUI[MAGICUI_TYPE_2COMMAND2].fHeightDown = 0.0f;				// 高さ[下]の初期化
			g_aMagicUI[nIdx].aMagicUI[MAGICUI_TYPE_2COMMAND2].fHeightDestUp = 0.0f;				// 高さ[上]の目的地の初期化
			g_aMagicUI[nIdx].aMagicUI[MAGICUI_TYPE_2COMMAND2].fHeightDestDown = 0.0f;				// 高さ[下]の目的地の初期化
			g_aMagicUI[nIdx].aMagicUI[MAGICUI_TYPE_2COMMAND2].bDisp = false;										// テクスチャの初期化
			break;

		case MAGICUI_TYPE_2_EQUAL:	// 合成結果
			g_aMagicUI[nIdx].aMagicUI[MAGICUI_TYPE_2_EQUAL].tex = MAGICUI_TEX_EQUAL;							// どの魔法も入力されていない
			// ここでバグが発生
			g_aMagicUI[nIdx].aMagicUI[MAGICUI_TYPE_2_EQUAL].type = MAGICUI_TYPE_2_EQUAL;						// UIの種類の初期化
			g_aMagicUI[nIdx].aMagicUI[MAGICUI_TYPE_2_EQUAL].pos = D3DXVECTOR3(ECUAL_POSX, MAGIC2_POSY, 0.0f);	// 位置の初期化
			g_aMagicUI[nIdx].aMagicUI[MAGICUI_TYPE_2_EQUAL].fWidth = MATH_SIZE;								// 幅の初期化
			g_aMagicUI[nIdx].aMagicUI[MAGICUI_TYPE_2_EQUAL].fHeight = MATH_SIZE;								// 高さの初期化
			g_aMagicUI[nIdx].aMagicUI[MAGICUI_TYPE_2_EQUAL].fHeightUp = 0.0f;				// 高さ[上]の初期化
			g_aMagicUI[nIdx].aMagicUI[MAGICUI_TYPE_2_EQUAL].fHeightDown = 0.0f;				// 高さ[下]の初期化
			g_aMagicUI[nIdx].aMagicUI[MAGICUI_TYPE_2_EQUAL].fHeightDestUp = 0.0f;				// 高さ[上]の目的地の初期化
			g_aMagicUI[nIdx].aMagicUI[MAGICUI_TYPE_2_EQUAL].fHeightDestDown = 0.0f;				// 高さ[下]の目的地の初期化
			g_aMagicUI[nIdx].aMagicUI[MAGICUI_TYPE_2_EQUAL].bDisp = false;										// テクスチャの初期化
			break;

		case MAGICUI_TYPE_2MAGIC:	// 発動された魔法
			g_aMagicUI[nIdx].aMagicUI[MAGICUI_TYPE_2MAGIC].tex = MAGICUI_TEX_NONE;					// どの魔法も入力されていない
			g_aMagicUI[nIdx].aMagicUI[MAGICUI_TYPE_2MAGIC].type = MAGICUI_TYPE_2MAGIC;				// UIの種類の初期化
			g_aMagicUI[nIdx].aMagicUI[MAGICUI_TYPE_2MAGIC].pos = D3DXVECTOR3(MAGIC_POSX, MAGIC2_POSY, 0.0f);	// 位置の初期化
			g_aMagicUI[nIdx].aMagicUI[MAGICUI_TYPE_2MAGIC].fWidth = ICON_SIZE;					// 幅の初期化
			g_aMagicUI[nIdx].aMagicUI[MAGICUI_TYPE_2MAGIC].fHeight = ICON_SIZE;					// 高さの初期化
			g_aMagicUI[nIdx].aMagicUI[MAGICUI_TYPE_2MAGIC].fHeightUp = 0.0f;				// 高さ[上]の初期化
			g_aMagicUI[nIdx].aMagicUI[MAGICUI_TYPE_2MAGIC].fHeightDown = 0.0f;				// 高さ[下]の初期化
			g_aMagicUI[nIdx].aMagicUI[MAGICUI_TYPE_2MAGIC].fHeightDestUp = 0.0f;				// 高さ[上]の目的地の初期化
			g_aMagicUI[nIdx].aMagicUI[MAGICUI_TYPE_2MAGIC].fHeightDestDown = 0.0f;				// 高さ[下]の目的地の初期化
			g_aMagicUI[nIdx].aMagicUI[MAGICUI_TYPE_2MAGIC].bDisp = false;							// テクスチャの初期化
			break;

			// 4つ目の魔法
		case MAGICUI_TYPE_3COMMAND0:	// 1つ目のコマンド
			g_aMagicUI[nIdx].aMagicUI[MAGICUI_TYPE_3COMMAND0].tex = MAGICUI_TEX_MAGICNULL;								// どの魔法も入力されていない
			g_aMagicUI[nIdx].aMagicUI[MAGICUI_TYPE_3COMMAND0].type = MAGICUI_TYPE_3COMMAND0;							// UIの種類の初期化
			g_aMagicUI[nIdx].aMagicUI[MAGICUI_TYPE_3COMMAND0].pos = D3DXVECTOR3(COMMAND1_POSX, MAGIC3_POSY, 0.0f);	// 位置の初期化
			g_aMagicUI[nIdx].aMagicUI[MAGICUI_TYPE_3COMMAND0].fWidth = ICON_SIZE;									// 幅の初期化
			g_aMagicUI[nIdx].aMagicUI[MAGICUI_TYPE_3COMMAND0].fHeight = ICON_SIZE;									// 高さの初期化
			g_aMagicUI[nIdx].aMagicUI[MAGICUI_TYPE_3COMMAND0].fHeightUp = 0.0f;				// 高さ[上]の初期化
			g_aMagicUI[nIdx].aMagicUI[MAGICUI_TYPE_3COMMAND0].fHeightDown = 0.0f;				// 高さ[下]の初期化
			g_aMagicUI[nIdx].aMagicUI[MAGICUI_TYPE_3COMMAND0].fHeightDestUp = 0.0f;				// 高さ[上]の目的地の初期化
			g_aMagicUI[nIdx].aMagicUI[MAGICUI_TYPE_3COMMAND0].fHeightDestDown = 0.0f;				// 高さ[下]の目的地の初期化
			g_aMagicUI[nIdx].aMagicUI[MAGICUI_TYPE_3COMMAND0].bDisp = false;											// テクスチャの初期化
			break;

		case MAGICUI_TYPE_3_01ADD:	// 01の合成
			g_aMagicUI[nIdx].aMagicUI[MAGICUI_TYPE_3_01ADD].tex = MAGICUI_TEX_ADD;								// どの魔法も入力されていない
			g_aMagicUI[nIdx].aMagicUI[MAGICUI_TYPE_3_01ADD].type = MAGICUI_TYPE_3_01ADD;							// UIの種類の初期化
			g_aMagicUI[nIdx].aMagicUI[MAGICUI_TYPE_3_01ADD].pos = D3DXVECTOR3(ADD01_POSX, MAGIC3_POSY, 0.0f);	// 位置の初期化
			g_aMagicUI[nIdx].aMagicUI[MAGICUI_TYPE_3_01ADD].fWidth = MATH_SIZE;									// 幅の初期化
			g_aMagicUI[nIdx].aMagicUI[MAGICUI_TYPE_3_01ADD].fHeight = MATH_SIZE;									// 高さの初期化
			g_aMagicUI[nIdx].aMagicUI[MAGICUI_TYPE_3_01ADD].fHeightUp = 0.0f;				// 高さ[上]の初期化
			g_aMagicUI[nIdx].aMagicUI[MAGICUI_TYPE_3_01ADD].fHeightDown = 0.0f;				// 高さ[下]の初期化
			g_aMagicUI[nIdx].aMagicUI[MAGICUI_TYPE_3_01ADD].fHeightDestUp = 0.0f;				// 高さ[上]の目的地の初期化
			g_aMagicUI[nIdx].aMagicUI[MAGICUI_TYPE_3_01ADD].fHeightDestDown = 0.0f;				// 高さ[下]の目的地の初期化
			g_aMagicUI[nIdx].aMagicUI[MAGICUI_TYPE_3_01ADD].bDisp = false;											// テクスチャの初期化
			break;

		case MAGICUI_TYPE_3COMMAND1:	// 2つ目のコマンド
			g_aMagicUI[nIdx].aMagicUI[MAGICUI_TYPE_3COMMAND1].tex = MAGICUI_TEX_MAGICNULL;					// どの魔法も入力されていない
			g_aMagicUI[nIdx].aMagicUI[MAGICUI_TYPE_3COMMAND1].type = MAGICUI_TYPE_3COMMAND1;				// UIの種類の初期化
			g_aMagicUI[nIdx].aMagicUI[MAGICUI_TYPE_3COMMAND1].pos = D3DXVECTOR3(COMMAND2_POSX, MAGIC3_POSY, 0.0f);	// 位置の初期化
			g_aMagicUI[nIdx].aMagicUI[MAGICUI_TYPE_3COMMAND1].fWidth = ICON_SIZE;						// 幅の初期化
			g_aMagicUI[nIdx].aMagicUI[MAGICUI_TYPE_3COMMAND1].fHeight = ICON_SIZE;						// 高さの初期化
			g_aMagicUI[nIdx].aMagicUI[MAGICUI_TYPE_3COMMAND1].fHeightUp = 0.0f;				// 高さ[上]の初期化
			g_aMagicUI[nIdx].aMagicUI[MAGICUI_TYPE_3COMMAND1].fHeightDown = 0.0f;				// 高さ[下]の初期化
			g_aMagicUI[nIdx].aMagicUI[MAGICUI_TYPE_3COMMAND1].fHeightDestUp = 0.0f;				// 高さ[上]の目的地の初期化
			g_aMagicUI[nIdx].aMagicUI[MAGICUI_TYPE_3COMMAND1].fHeightDestDown = 0.0f;				// 高さ[下]の目的地の初期化
			g_aMagicUI[nIdx].aMagicUI[MAGICUI_TYPE_3COMMAND1].bDisp = false;								// テクスチャの初期化
			break;

		case MAGICUI_TYPE_3_12ADD:	// 12の合成
			g_aMagicUI[nIdx].aMagicUI[MAGICUI_TYPE_3_12ADD].tex = MAGICUI_TEX_ADD;								// どの魔法も入力されていない
			g_aMagicUI[nIdx].aMagicUI[MAGICUI_TYPE_3_12ADD].type = MAGICUI_TYPE_3_12ADD;							// UIの種類の初期化
			g_aMagicUI[nIdx].aMagicUI[MAGICUI_TYPE_3_12ADD].pos = D3DXVECTOR3(ADD12_POSX, MAGIC3_POSY, 0.0f);	// 位置の初期化
			g_aMagicUI[nIdx].aMagicUI[MAGICUI_TYPE_3_12ADD].fWidth = MATH_SIZE;									// 幅の初期化
			g_aMagicUI[nIdx].aMagicUI[MAGICUI_TYPE_3_12ADD].fHeight = MATH_SIZE;									// 高さの初期化
			g_aMagicUI[nIdx].aMagicUI[MAGICUI_TYPE_3_12ADD].fHeightUp = 0.0f;				// 高さ[上]の初期化
			g_aMagicUI[nIdx].aMagicUI[MAGICUI_TYPE_3_12ADD].fHeightDown = 0.0f;				// 高さ[下]の初期化
			g_aMagicUI[nIdx].aMagicUI[MAGICUI_TYPE_3_12ADD].fHeightDestUp = 0.0f;				// 高さ[上]の目的地の初期化
			g_aMagicUI[nIdx].aMagicUI[MAGICUI_TYPE_3_12ADD].fHeightDestDown = 0.0f;				// 高さ[下]の目的地の初期化
			g_aMagicUI[nIdx].aMagicUI[MAGICUI_TYPE_3_12ADD].bDisp = false;											// テクスチャの初期化
			break;

		case MAGICUI_TYPE_3COMMAND2:	// 3つ目のコマンド
			g_aMagicUI[nIdx].aMagicUI[MAGICUI_TYPE_3COMMAND2].tex = MAGICUI_TEX_MAGICNULL;							// どの魔法も入力されていない
			g_aMagicUI[nIdx].aMagicUI[MAGICUI_TYPE_3COMMAND2].type = MAGICUI_TYPE_3COMMAND2;						// UIの種類の初期化
			g_aMagicUI[nIdx].aMagicUI[MAGICUI_TYPE_3COMMAND2].pos = D3DXVECTOR3(COMMAND3_POSX, MAGIC3_POSY, 0.0f);	// 位置の初期化
			g_aMagicUI[nIdx].aMagicUI[MAGICUI_TYPE_3COMMAND2].fWidth = ICON_SIZE;								// 幅の初期化
			g_aMagicUI[nIdx].aMagicUI[MAGICUI_TYPE_3COMMAND2].fHeight = ICON_SIZE;								// 高さの初期化
			g_aMagicUI[nIdx].aMagicUI[MAGICUI_TYPE_3COMMAND2].fHeightUp = 0.0f;				// 高さ[上]の初期化
			g_aMagicUI[nIdx].aMagicUI[MAGICUI_TYPE_3COMMAND2].fHeightDown = 0.0f;				// 高さ[下]の初期化
			g_aMagicUI[nIdx].aMagicUI[MAGICUI_TYPE_3COMMAND2].fHeightDestUp = 0.0f;				// 高さ[上]の目的地の初期化
			g_aMagicUI[nIdx].aMagicUI[MAGICUI_TYPE_3COMMAND2].fHeightDestDown = 0.0f;				// 高さ[下]の目的地の初期化
			g_aMagicUI[nIdx].aMagicUI[MAGICUI_TYPE_3COMMAND2].bDisp = false;										// テクスチャの初期化
			break;

		case MAGICUI_TYPE_3_EQUAL:	// 合成結果
			g_aMagicUI[nIdx].aMagicUI[MAGICUI_TYPE_3_EQUAL].tex = MAGICUI_TEX_EQUAL;							// どの魔法も入力されていない
			g_aMagicUI[nIdx].aMagicUI[MAGICUI_TYPE_3_EQUAL].type = MAGICUI_TYPE_3_EQUAL;						// UIの種類の初期化
			g_aMagicUI[nIdx].aMagicUI[MAGICUI_TYPE_3_EQUAL].pos = D3DXVECTOR3(ECUAL_POSX, MAGIC3_POSY, 0.0f);	// 位置の初期化
			g_aMagicUI[nIdx].aMagicUI[MAGICUI_TYPE_3_EQUAL].fWidth = MATH_SIZE;								// 幅の初期化
			g_aMagicUI[nIdx].aMagicUI[MAGICUI_TYPE_3_EQUAL].fHeight = MATH_SIZE;								// 高さの初期化
			g_aMagicUI[nIdx].aMagicUI[MAGICUI_TYPE_3_EQUAL].fHeightUp = 0.0f;				// 高さ[上]の初期化
			g_aMagicUI[nIdx].aMagicUI[MAGICUI_TYPE_3_EQUAL].fHeightDown = 0.0f;				// 高さ[下]の初期化
			g_aMagicUI[nIdx].aMagicUI[MAGICUI_TYPE_3_EQUAL].fHeightDestUp = 0.0f;				// 高さ[上]の目的地の初期化
			g_aMagicUI[nIdx].aMagicUI[MAGICUI_TYPE_3_EQUAL].fHeightDestDown = 0.0f;				// 高さ[下]の目的地の初期化
			g_aMagicUI[nIdx].aMagicUI[MAGICUI_TYPE_3_EQUAL].bDisp = false;										// テクスチャの初期化
			break;

		case MAGICUI_TYPE_3MAGIC:	// 発動された魔法
			g_aMagicUI[nIdx].aMagicUI[MAGICUI_TYPE_3MAGIC].tex = MAGICUI_TEX_NONE;					// どの魔法も入力されていない
			g_aMagicUI[nIdx].aMagicUI[MAGICUI_TYPE_3MAGIC].type = MAGICUI_TYPE_3MAGIC;				// UIの種類の初期化
			g_aMagicUI[nIdx].aMagicUI[MAGICUI_TYPE_3MAGIC].pos = D3DXVECTOR3(MAGIC_POSX, MAGIC3_POSY, 0.0f);	// 位置の初期化
			g_aMagicUI[nIdx].aMagicUI[MAGICUI_TYPE_3MAGIC].fWidth = ICON_SIZE;					// 幅の初期化
			g_aMagicUI[nIdx].aMagicUI[MAGICUI_TYPE_3MAGIC].fHeight = ICON_SIZE;					// 高さの初期化
			g_aMagicUI[nIdx].aMagicUI[MAGICUI_TYPE_3MAGIC].fHeightUp = 0.0f;				// 高さ[上]の初期化
			g_aMagicUI[nIdx].aMagicUI[MAGICUI_TYPE_3MAGIC].fHeightDown = 0.0f;				// 高さ[下]の初期化
			g_aMagicUI[nIdx].aMagicUI[MAGICUI_TYPE_3MAGIC].fHeightDestUp = 0.0f;				// 高さ[上]の目的地の初期化
			g_aMagicUI[nIdx].aMagicUI[MAGICUI_TYPE_3MAGIC].fHeightDestDown = 0.0f;				// 高さ[下]の目的地の初期化
			g_aMagicUI[nIdx].aMagicUI[MAGICUI_TYPE_3MAGIC].bDisp = false;							// テクスチャの初期化
			break;	
		}
	}
}

//======================================================================================
// UIを配置
//======================================================================================
void SetMagicUI(int nIdx)
{
	g_aMagicUI[nIdx].bDisp = true;
	g_aMagicUI[nIdx].nFrame = 0;
	g_aMagicUI[nIdx].state = MAGICUISTATE_APPEAR;
}

//======================================================================================
// UIを非表示にする
//======================================================================================
void DisappearMagicUI(int nIdx)
{
	for (int nCntUI = 0; nCntUI < MAX_MAGICUI_TYPE; nCntUI++)
	{
		g_aMagicUI[nIdx].aMagicUI[nCntUI].bDisp = false;
	}
	g_aMagicUI[nIdx].nFrame = 0;
	g_aMagicUI[nIdx].state = MAGICUISTATE_DISAPPERA;
}

//======================================================================================
// プレイヤーの魔導書の情報を記録する
//======================================================================================
void SetPlayerMagic(int nIdx)
{
	Player* pPlayer = GetPlayer();	// プレイヤーの情報を取得

	for (int nCntCommand = 0; nCntCommand < MAX_MAGIC; nCntCommand++)
	{
		MAGICUI_TEX magicUI[MAX_MAGIC];

		// 1つ目の魔法
		switch (pPlayer[nIdx].magicbook.OwnCommand[nCntCommand])
		{
		case COMMANDOREDER_NONE:	// 魔法を取得していない場合
			magicUI[0] = MAGICUI_TEX_MAGICNULL;
			magicUI[1] = MAGICUI_TEX_MAGICNULL;
			magicUI[2] = MAGICUI_TEX_MAGICNULL;
			magicUI[3] = MAGICUI_TEX_NONE;
			break;

		// 単色魔法=================================
		case COMMANDOREDER_RRR:	// RRRの場合
			magicUI[0] = MAGICUI_TEX_RED;
			magicUI[1] = MAGICUI_TEX_RED;
			magicUI[2] = MAGICUI_TEX_RED;
			magicUI[3] = MAGICUI_TEX_COMBUSTION;
			break;

		case COMMANDOREDER_GGG:	// GGGの場合
			magicUI[0] = MAGICUI_TEX_GREEN;
			magicUI[1] = MAGICUI_TEX_GREEN;
			magicUI[2] = MAGICUI_TEX_GREEN;
			magicUI[3] = MAGICUI_TEX_LEVITATION;
			break;

		case COMMANDOREDER_BBB:	// BBBの場合
			magicUI[0] = MAGICUI_TEX_BLUE;
			magicUI[1] = MAGICUI_TEX_BLUE;
			magicUI[2] = MAGICUI_TEX_BLUE;
			magicUI[3] = MAGICUI_TEX_FLOOD;
			break;

		case COMMANDOREDER_YYY:	// YYYの場合
			magicUI[0] = MAGICUI_TEX_YELLOW;
			magicUI[1] = MAGICUI_TEX_YELLOW;
			magicUI[2] = MAGICUI_TEX_YELLOW;
			magicUI[3] = MAGICUI_TEX_FLASH;
			break;
		
		// 火球魔法=================================
		case COMMANDOREDER_RRG:	// RRGの場合
			magicUI[0] = MAGICUI_TEX_RED;
			magicUI[1] = MAGICUI_TEX_RED;
			magicUI[2] = MAGICUI_TEX_GREEN;
			magicUI[3] = MAGICUI_TEX_FIREBALL;
			break;

		case COMMANDOREDER_RGR:	// RGRの場合
			magicUI[0] = MAGICUI_TEX_RED;
			magicUI[1] = MAGICUI_TEX_GREEN;
			magicUI[2] = MAGICUI_TEX_RED;
			magicUI[3] = MAGICUI_TEX_FIREBALL;
			break;

		case COMMANDOREDER_GRR:	// GRRの場合
			magicUI[0] = MAGICUI_TEX_GREEN;
			magicUI[1] = MAGICUI_TEX_RED;
			magicUI[2] = MAGICUI_TEX_RED;
			magicUI[3] = MAGICUI_TEX_FIREBALL;
			break;

		// 太陽魔法=================================
		case COMMANDOREDER_RYY:	// RYYの場合
			magicUI[0] = MAGICUI_TEX_RED;
			magicUI[1] = MAGICUI_TEX_YELLOW;
			magicUI[2] = MAGICUI_TEX_YELLOW;
			magicUI[3] = MAGICUI_TEX_SUNSETDELAY;
			break;

		case COMMANDOREDER_YRY:	// YRYの場合
			magicUI[0] = MAGICUI_TEX_YELLOW;
			magicUI[1] = MAGICUI_TEX_RED;
			magicUI[2] = MAGICUI_TEX_YELLOW;
			magicUI[3] = MAGICUI_TEX_SUNSETDELAY;
			break;

		case COMMANDOREDER_YYR:	// YYRの場合
			magicUI[0] = MAGICUI_TEX_YELLOW;
			magicUI[1] = MAGICUI_TEX_YELLOW;
			magicUI[2] = MAGICUI_TEX_RED;
			magicUI[3] = MAGICUI_TEX_SUNSETDELAY;
			break;
		
		// 雨乞魔法=================================
		case COMMANDOREDER_BBG:	// BBGの場合
			magicUI[0] = MAGICUI_TEX_BLUE;
			magicUI[1] = MAGICUI_TEX_BLUE;
			magicUI[2] = MAGICUI_TEX_GREEN;
			magicUI[3] = MAGICUI_TEX_RAINPRAY;
			break;

		case COMMANDOREDER_BGB:	// BGBの場合
			magicUI[0] = MAGICUI_TEX_BLUE;
			magicUI[1] = MAGICUI_TEX_GREEN;
			magicUI[2] = MAGICUI_TEX_BLUE;
			magicUI[3] = MAGICUI_TEX_RAINPRAY;
			break;

		case COMMANDOREDER_GBB:	//GBBの場合
			magicUI[0] = MAGICUI_TEX_GREEN;
			magicUI[1] = MAGICUI_TEX_BLUE;
			magicUI[2] = MAGICUI_TEX_BLUE;
			magicUI[3] = MAGICUI_TEX_RAINPRAY;
			break;

		// 氷結魔法=================================
		case COMMANDOREDER_BGG:	//GBBの場合
			magicUI[0] = MAGICUI_TEX_BLUE;
			magicUI[1] = MAGICUI_TEX_GREEN;
			magicUI[2] = MAGICUI_TEX_GREEN;
			magicUI[3] = MAGICUI_TEX_FREEZE;
			break;

		case COMMANDOREDER_GBG:	// GBGの場合
			magicUI[0] = MAGICUI_TEX_GREEN;
			magicUI[1] = MAGICUI_TEX_BLUE;
			magicUI[2] = MAGICUI_TEX_GREEN;
			magicUI[3] = MAGICUI_TEX_FREEZE;
			break;

		case COMMANDOREDER_GGB:	// GGBの場合
			magicUI[0] = MAGICUI_TEX_GREEN;
			magicUI[1] = MAGICUI_TEX_GREEN;
			magicUI[2] = MAGICUI_TEX_BLUE;
			magicUI[3] = MAGICUI_TEX_FREEZE;
			break;

		// 成長魔法=================================
		case COMMANDOREDER_BYY:	// BYYの場合
			magicUI[0] = MAGICUI_TEX_BLUE;
			magicUI[1] = MAGICUI_TEX_YELLOW;
			magicUI[2] = MAGICUI_TEX_YELLOW;
			magicUI[3] = MAGICUI_TEX_GROWTH;
			break;

		case COMMANDOREDER_YBY:	// YBYの場合
			magicUI[0] = MAGICUI_TEX_YELLOW;
			magicUI[1] = MAGICUI_TEX_BLUE;
			magicUI[2] = MAGICUI_TEX_YELLOW;
			magicUI[3] = MAGICUI_TEX_GROWTH;
			break;

		case COMMANDOREDER_YYB:	// YYBの場合
			magicUI[0] = MAGICUI_TEX_YELLOW;
			magicUI[1] = MAGICUI_TEX_YELLOW;
			magicUI[2] = MAGICUI_TEX_BLUE;
			magicUI[3] = MAGICUI_TEX_GROWTH;
			break;

		// 加速魔法=================================
		case COMMANDOREDER_GGY:	// GGYの場合
			magicUI[0] = MAGICUI_TEX_GREEN;
			magicUI[1] = MAGICUI_TEX_GREEN;
			magicUI[2] = MAGICUI_TEX_YELLOW;
			magicUI[3] = MAGICUI_TEX_ACCELERATION;
			break;

		case COMMANDOREDER_GYG:	// GYGの場合
			magicUI[0] = MAGICUI_TEX_GREEN;
			magicUI[1] = MAGICUI_TEX_YELLOW;
			magicUI[2] = MAGICUI_TEX_GREEN;
			magicUI[3] = MAGICUI_TEX_ACCELERATION;
			break;

		case COMMANDOREDER_YGG:	// YGGの場合
			magicUI[0] = MAGICUI_TEX_YELLOW;
			magicUI[1] = MAGICUI_TEX_GREEN;
			magicUI[2] = MAGICUI_TEX_GREEN;
			magicUI[3] = MAGICUI_TEX_ACCELERATION;
			break;

		// 時間魔法=================================
		case COMMANDOREDER_RGB:	// RGBの場合
			magicUI[0] = MAGICUI_TEX_RED;
			magicUI[1] = MAGICUI_TEX_GREEN;
			magicUI[2] = MAGICUI_TEX_BLUE;
			magicUI[3] = MAGICUI_TEX_ACCELERATION;
			break;
		}

		switch (nCntCommand)
		{
		case 0:	// 1つ目の魔法
			g_aMagicUI[nIdx].aMagicUI[MAGICUI_TYPE_0COMMAND0].tex	= magicUI[0];
			g_aMagicUI[nIdx].aMagicUI[MAGICUI_TYPE_0COMMAND1].tex	= magicUI[1];
			g_aMagicUI[nIdx].aMagicUI[MAGICUI_TYPE_0COMMAND2].tex	= magicUI[2];
			g_aMagicUI[nIdx].aMagicUI[MAGICUI_TYPE_0MAGIC].tex		= magicUI[3];
			break;

		case 1:	// 2つ目の魔法
			g_aMagicUI[nIdx].aMagicUI[MAGICUI_TYPE_1COMMAND0].tex	= magicUI[0];
			g_aMagicUI[nIdx].aMagicUI[MAGICUI_TYPE_1COMMAND1].tex	= magicUI[1];
			g_aMagicUI[nIdx].aMagicUI[MAGICUI_TYPE_1COMMAND2].tex	= magicUI[2];
			g_aMagicUI[nIdx].aMagicUI[MAGICUI_TYPE_1MAGIC].tex		= magicUI[3];
			break;

		case 2:	// 3つ目の魔法
			g_aMagicUI[nIdx].aMagicUI[MAGICUI_TYPE_2COMMAND0].tex	= magicUI[0];
			g_aMagicUI[nIdx].aMagicUI[MAGICUI_TYPE_2COMMAND1].tex	= magicUI[1];
			g_aMagicUI[nIdx].aMagicUI[MAGICUI_TYPE_2COMMAND2].tex	= magicUI[2];
			g_aMagicUI[nIdx].aMagicUI[MAGICUI_TYPE_2MAGIC].tex		= magicUI[3];
			break;

		case 3:	// 4つ目の魔法
			g_aMagicUI[nIdx].aMagicUI[MAGICUI_TYPE_3COMMAND0].tex	= magicUI[0];
			g_aMagicUI[nIdx].aMagicUI[MAGICUI_TYPE_3COMMAND1].tex	= magicUI[1];
			g_aMagicUI[nIdx].aMagicUI[MAGICUI_TYPE_3COMMAND2].tex	= magicUI[2];
			g_aMagicUI[nIdx].aMagicUI[MAGICUI_TYPE_3MAGIC].tex		= magicUI[3];
			break;
		}
	}
}