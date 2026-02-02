//========================================================
// 
// 魔法発動状態のUI表示処理[spellui.cpp]
// Author : KajinoHiyori
// 
//========================================================
#include "main.h"
#include "spellui.h"
#include "player.h"
#include "game.h"
#include "input.h"
#include "title.h"
#include "fog.h"
#include "debugproc.h"

// マクロ定義
#define MAX_COMMAND			(3)					// 受け付けるコマンドの最大数
#define MAX_SPELLTEX		(SPELLUI_TEX_MAX)	// テクスチャの最大数
#define MAX_SPELLTYPE		(SPELLUI_TYPE_MAX)	// 表示されるUIの種類
#define INERTIA				(0.1f)				// UI出現の慣性
#define FRAME				(20)				// 出現/退出を管理するフレーム数
#define SPELLUI_POSY		(482.0f)			// 左のUIのX軸
#define LEFT_OUTPOS			(D3DXVECTOR3(-PHONE_WIDTH, SPELLUI_POSY, 0.0f))		// offscreenの左のUI座標
#define RIGHT_OUTPOS		(D3DXVECTOR3(SCREEN_WIDTH + PHONE_WIDTH, SPELLUI_POSY, 0.0f))	// offscreenの右のUI座標
#define LEFT_POS			(D3DXVECTOR3(120.0f, SPELLUI_POSY, 0.0f))		// onscreenの左のUI座標
#define RIGHT_POS			(D3DXVECTOR3(1160.0f, SPELLUI_POSY, 0.0f))					// onscreenの右のUI座標
#define COMMMAND_Y			(-60.0f)		// コマンド結果の高度
#define COMMMAND_SIZE		(25.0f)			// コマンドボタンの大きさ
#define MAGIC_SIZE			(60.0f)			// 発動魔法の大きさ
#define BUTTON_SIZE			(15.0f)			// 操作キー表示の大きさ
#define SPELL_WIDTH			(PHONE_WIDTH)	// spellメニューの幅
#define SPELL_HEIGHT		(24.0f)			// spellメニューの高さ
#define SPELL_Y				(-170.0f)		// spellメニューの高度
#define MAGIC_Y				(-65.0f)		// magicの種類の高度
#define PHONE_WIDTH			(108.0f)		// スマホの幅
#define PHONE_HEIGHT		(228.0f)		// スマホの高さ
#define COMMMAND_DIGIT		(75.0f)			// コマンドの表示幅
#define COMMMAND_OPDIGIT	(85.0f)			// コマンドの表示幅
#define RBCOMMAND_Y			(110.0f)		// RBコマンドの高度
#define YCOMMAND_Y			(RBCOMMAND_Y - COMMMAND_OPDIGIT)	// Yコマンドの高度
#define GCOMMAND_Y			(RBCOMMAND_Y + COMMMAND_OPDIGIT)	// Gコマンドの高度
#define RB_OPTYPE_X			(COMMMAND_OPDIGIT - 55.0f)		// RB操作コマンドの高度
#define Y_OPTYPE_Y			(YCOMMAND_Y + 55.0f)			// Y操作コマンドの高度
#define G_OPTYPE_Y			(GCOMMAND_Y - 55.0f)			// G操作コマンドの高度

// SPELLUIの構造体
typedef struct
{
	SPELLUI_TYPE	type;		// 表示されるUIの場所を管理
	SPELLUI_TEX		tex;		// 使用されるテクスチャの種類を管理
	D3DXVECTOR3		pos;		// 位置
	float			fWidth;		// 幅
	float			fHeight;	// 高さ
	bool			bDisp;		// 表示状態
}SPELLUI;

// SPELLUIの出現状態を管理
typedef enum
{
	SPELLUISTATE_NONE = -1,	// 何もしていない状態
	SPELLUISTATE_ONSCREEN,	// 画面内
	SPELLUISTATE_APPEAR,	// 出現状態
	SPELLUISTATE_DISAPPEAR,	// はける状態
	SPELLUISTATE_OFFSCREEN,	// 画面外
}SPELLUISTATE;

// SPELLUIの全体管理
typedef struct
{
	SPELLUISTATE state;					// 出現状態
	SPELLUI g_SpellUI[MAX_SPELLTEX];	// UIの種類ごとの表示管理
	D3DXVECTOR3 pos;					// 中心位置
	D3DXVECTOR3 posDest;				// 目的の向き
	int nFrame;							// 現在のフレーム数
	int nNumFrame;						// 移動にかかるフレーム数
	int nCounterUI;						// magicの種類を表示する時間
	bool bDisp;							// 全体の表示管理
}SpellUI;

// グローバル変数
LPDIRECT3DTEXTURE9	g_apTextureSpellUI[MAX_SPELLTEX] = {};	// テクスチャへのポインタ
LPDIRECT3DVERTEXBUFFER9	g_pVtxBuffSpellUI = NULL;			// 頂点バッファへのポインタ
SpellUI g_aSpellUI[MAX_PLAYER];		// SPELLUIの全体管理

// テクスチャの読み込み
const char* c_apFilenameSpellUI[MAX_SPELLTEX] =
{
	"data\\TEXTURE\\SpellUI\\00_MagicNull.png",
	"data\\TEXTURE\\SpellUI\\01_Red.png",
	"data\\TEXTURE\\SpellUI\\02_Green.png",
	"data\\TEXTURE\\SpellUI\\03_Blue.png",
	"data\\TEXTURE\\SpellUI\\04_Yellow.png",
	"data\\TEXTURE\\SpellUI\\05_1.png",
	"data\\TEXTURE\\SpellUI\\06_2.png",
	"data\\TEXTURE\\SpellUI\\07_3.png",
	"data\\TEXTURE\\SpellUI\\08_4.png",
	"data\\TEXTURE\\SpellUI\\09_A.png",
	"data\\TEXTURE\\SpellUI\\10_B.png",
	"data\\TEXTURE\\SpellUI\\11_X.png",
	"data\\TEXTURE\\SpellUI\\12_Y.png",
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
	"data\\TEXTURE\\SpellUI\\25_Spell.png",
	"data\\TEXTURE\\SpellUI\\26_Phone.png",
};

//======================================================================================
// spellの初期化処理
//======================================================================================
void InitSpellUI(void)
{
	LPDIRECT3DDEVICE9 pDevice;	// デバイスへのポインタ
	// デバイスの取得
	pDevice = GetDevice();

	// UI情報の初期化
	for (int nCntPlayer = 0; nCntPlayer < MAX_PLAYER; nCntPlayer++)
	{
		for (int nCntUI = 0; nCntUI < MAX_SPELLTEX; nCntUI++)
		{
			g_aSpellUI[nCntPlayer].g_SpellUI[nCntUI].tex		= SPELLUI_TEX_MAGICNULL;			// テクスチャの初期化
			g_aSpellUI[nCntPlayer].g_SpellUI[nCntUI].type		= SPELLUI_TYPE_COMMAND0;			// UIの種類の初期化
			g_aSpellUI[nCntPlayer].g_SpellUI[nCntUI].pos		= D3DXVECTOR3(0.0f, 0.0f, 0.0f);	// 位置の初期化
			g_aSpellUI[nCntPlayer].g_SpellUI[nCntUI].fWidth	= 0.0f;									// 幅の初期化
			g_aSpellUI[nCntPlayer].g_SpellUI[nCntUI].fHeight	= 0.0f;								// 高さの初期化
			g_aSpellUI[nCntPlayer].g_SpellUI[nCntUI].bDisp		= false;							// 表示状態の初期化
		}
		g_aSpellUI[nCntPlayer].state = SPELLUISTATE_OFFSCREEN;		// 画面外にある
		g_aSpellUI[nCntPlayer].bDisp = false;						// 全体の表示状態の初期化
		g_aSpellUI[nCntPlayer].nCounterUI = 0;						// 発動魔法の初期化
		g_aSpellUI[nCntPlayer].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);	// 位置の初期化
		g_aSpellUI[nCntPlayer].nFrame = 0;							// 現在のフレームを初期化
		g_aSpellUI[nCntPlayer].nNumFrame = FRAME;					// 出現の管理を行うフレーム数
	}

	// テクスチャの読み込み
	for (int nCntSpellUI = 0; nCntSpellUI < MAX_SPELLTEX; nCntSpellUI++)
	{
		D3DXCreateTextureFromFile(pDevice, c_apFilenameSpellUI[nCntSpellUI], &g_apTextureSpellUI[nCntSpellUI]);
	}

	// 操作方法の状態を取得
	OPERATIONTYPE operationType = GetOperationType();

	// 頂点バッファの生成
	pDevice->CreateVertexBuffer(sizeof(VERTEX_2D) * 4 * MAX_SPELLTYPE * MAX_PLAYER, D3DUSAGE_WRITEONLY, FVF_VERTEX_2D, D3DPOOL_MANAGED, &g_pVtxBuffSpellUI, NULL);
	VERTEX_2D* pVtx;
	// 頂点バッファをロックし、頂点情報へのポインタを取得
	g_pVtxBuffSpellUI->Lock(0, 0, (void**)&pVtx, 0);

	for (int nCntPlayer = 0; nCntPlayer < MAX_PLAYER; nCntPlayer++)
	{
		switch (operationType)	// 操作人数に応じてUIの位置を変更
		{
		case OPERATIONTYPE_2P:	// 2人操作
			switch (nCntPlayer)
			{
			case 0:
				g_aSpellUI[nCntPlayer].pos		= LEFT_OUTPOS;
				g_aSpellUI[nCntPlayer].posDest	= LEFT_OUTPOS;
				g_aSpellUI[nCntPlayer].bDisp	= true;

				break;
			case 1:
				g_aSpellUI[nCntPlayer].pos		= RIGHT_OUTPOS;
				g_aSpellUI[nCntPlayer].pos		= RIGHT_OUTPOS;
				g_aSpellUI[nCntPlayer].bDisp	= true;

				break;
			}	
			break;

		default:	// 1人操作
			if (nCntPlayer > 0)
			{
				pVtx += MAX_SPELLTYPE * 4;
				continue;
			}
			g_aSpellUI[nCntPlayer].bDisp	= true;
			g_aSpellUI[nCntPlayer].pos		= RIGHT_OUTPOS;
			g_aSpellUI[nCntPlayer].pos		= RIGHT_OUTPOS;
			break;
		}
			
		ResetSpellUI(nCntPlayer);

		for (int nCntUI = 0; nCntUI < MAX_SPELLTYPE; nCntUI++, pVtx += 4)	// 頂点バッファに数値を代入
		{
			// 頂点座標の設定
			pVtx[0].pos.x = g_aSpellUI[nCntPlayer].pos.x + g_aSpellUI[nCntPlayer].g_SpellUI[nCntUI].pos.x - g_aSpellUI[nCntPlayer].g_SpellUI[nCntUI].fWidth;
			pVtx[0].pos.y = g_aSpellUI[nCntPlayer].pos.y + g_aSpellUI[nCntPlayer].g_SpellUI[nCntUI].pos.y - g_aSpellUI[nCntPlayer].g_SpellUI[nCntUI].fHeight;
			pVtx[0].pos.z = g_aSpellUI[nCntPlayer].pos.z + g_aSpellUI[nCntPlayer].g_SpellUI[nCntUI].pos.z;
			pVtx[1].pos.x = g_aSpellUI[nCntPlayer].pos.x + g_aSpellUI[nCntPlayer].g_SpellUI[nCntUI].pos.x + g_aSpellUI[nCntPlayer].g_SpellUI[nCntUI].fWidth;
			pVtx[1].pos.y = g_aSpellUI[nCntPlayer].pos.y + g_aSpellUI[nCntPlayer].g_SpellUI[nCntUI].pos.y - g_aSpellUI[nCntPlayer].g_SpellUI[nCntUI].fHeight;
			pVtx[1].pos.z = g_aSpellUI[nCntPlayer].pos.z + g_aSpellUI[nCntPlayer].g_SpellUI[nCntUI].pos.z;
			pVtx[2].pos.x = g_aSpellUI[nCntPlayer].pos.x + g_aSpellUI[nCntPlayer].g_SpellUI[nCntUI].pos.x - g_aSpellUI[nCntPlayer].g_SpellUI[nCntUI].fWidth;
			pVtx[2].pos.y = g_aSpellUI[nCntPlayer].pos.y + g_aSpellUI[nCntPlayer].g_SpellUI[nCntUI].pos.y + g_aSpellUI[nCntPlayer].g_SpellUI[nCntUI].fHeight;
			pVtx[2].pos.z = g_aSpellUI[nCntPlayer].pos.z + g_aSpellUI[nCntPlayer].g_SpellUI[nCntUI].pos.z;
			pVtx[3].pos.x = g_aSpellUI[nCntPlayer].pos.x + g_aSpellUI[nCntPlayer].g_SpellUI[nCntUI].pos.x + g_aSpellUI[nCntPlayer].g_SpellUI[nCntUI].fWidth;
			pVtx[3].pos.y = g_aSpellUI[nCntPlayer].pos.y + g_aSpellUI[nCntPlayer].g_SpellUI[nCntUI].pos.y + g_aSpellUI[nCntPlayer].g_SpellUI[nCntUI].fHeight;
			pVtx[3].pos.z = g_aSpellUI[nCntPlayer].pos.z + g_aSpellUI[nCntPlayer].g_SpellUI[nCntUI].pos.z;

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
	g_pVtxBuffSpellUI->Unlock();
}

//======================================================================================
// spellの終了処理
//======================================================================================
void UninitSpellUI(void)
{

	// テクスチャの破棄
	for (int nCntSpellUI = 0; nCntSpellUI < MAX_SPELLTEX; nCntSpellUI++)
	{
		if (g_apTextureSpellUI[nCntSpellUI] != NULL)
		{
			g_apTextureSpellUI[nCntSpellUI]->Release();
			g_apTextureSpellUI[nCntSpellUI] = NULL;
		}
	}

	// 頂点バッファの破棄
	if (g_pVtxBuffSpellUI != NULL)
	{
		g_pVtxBuffSpellUI->Release();
		g_pVtxBuffSpellUI = NULL;
	}

}

//======================================================================================
// spellの更新処理
//======================================================================================
void UpdateSpellUI(void)
{
	// 操作方法の状態を取得
	OPERATIONTYPE operationType = GetOperationType();

	VERTEX_2D* pVtx;
	// 頂点バッファをロックし、頂点情報へのポインタを取得
	g_pVtxBuffSpellUI->Lock(0, 0, (void**)&pVtx, 0);

	for (int nCntPlayer = 0; nCntPlayer < MAX_PLAYER; nCntPlayer++)
	{
		switch (operationType)
		{
		case OPERATIONTYPE_2P:	// 2人操作
			if ((GetKeyboardPress(DIK_TAB) == true && nCntPlayer == 0) || (GetKeyboardPress(DIK_RSHIFT) == true && nCntPlayer == 1) || GetJoypadRightTriggePress(nCntPlayer) == true)
			{
				if (g_aSpellUI[nCntPlayer].state == SPELLUISTATE_OFFSCREEN && nCntPlayer == 0)
				{
					g_aSpellUI[nCntPlayer].posDest = LEFT_POS;
					g_aSpellUI[nCntPlayer].state = SPELLUISTATE_APPEAR;
					g_aSpellUI[nCntPlayer].nFrame = 0;
				}
				else if (g_aSpellUI[nCntPlayer].state == SPELLUISTATE_OFFSCREEN && nCntPlayer == 1)
				{
					g_aSpellUI[nCntPlayer].state = SPELLUISTATE_APPEAR;
					g_aSpellUI[nCntPlayer].posDest = RIGHT_POS;
					g_aSpellUI[nCntPlayer].nFrame = 0;
				}
				g_aSpellUI[nCntPlayer].bDisp = true;
			}
			else
			{
				ResetSpellUI(nCntPlayer);
				if (g_aSpellUI[nCntPlayer].state == SPELLUISTATE_ONSCREEN && nCntPlayer == 0)
				{
					g_aSpellUI[nCntPlayer].state = SPELLUISTATE_DISAPPEAR;
					g_aSpellUI[nCntPlayer].posDest = LEFT_OUTPOS;
					g_aSpellUI[nCntPlayer].nFrame = 0;
				}
				else if (g_aSpellUI[nCntPlayer].state == SPELLUISTATE_ONSCREEN && nCntPlayer == 1)
				{
					g_aSpellUI[nCntPlayer].state = SPELLUISTATE_DISAPPEAR;
					g_aSpellUI[nCntPlayer].posDest = RIGHT_OUTPOS;
					g_aSpellUI[nCntPlayer].nFrame = 0;
				}
			}
			break;

		default:	// 1人操作
			if (nCntPlayer > 0)
			{
				continue;
			}
			if ((GetKeyboardPress(DIK_TAB) == true && nCntPlayer == 0) || GetJoypadRightTriggePress(nCntPlayer) == true || GetJoypadRightTriggePress(nCntPlayer) == true)
			{
				g_aSpellUI[nCntPlayer].bDisp = true;
				if (g_aSpellUI[nCntPlayer].state == SPELLUISTATE_OFFSCREEN && nCntPlayer == 0)
				{
					g_aSpellUI[nCntPlayer].state = SPELLUISTATE_APPEAR;
					g_aSpellUI[nCntPlayer].posDest = RIGHT_POS;
					g_aSpellUI[nCntPlayer].nFrame = 0;
				}
			}
			else
			{
				ResetSpellUI(nCntPlayer);
				if (g_aSpellUI[nCntPlayer].state == SPELLUISTATE_ONSCREEN && nCntPlayer == 0)
				{
					g_aSpellUI[nCntPlayer].state = SPELLUISTATE_DISAPPEAR;
					g_aSpellUI[nCntPlayer].posDest = RIGHT_OUTPOS;
					g_aSpellUI[nCntPlayer].nFrame = 0;
				}
			}
			break;
		}
		if (g_aSpellUI[nCntPlayer].bDisp == true)
		{
			COMMANDTYPE* commandType = GetCommandType(nCntPlayer);
			COMMANDTYPE CommandUI[MAX_COMMAND];

			for (int nCntUI = 0; nCntUI < MAX_COMMAND; nCntUI++)
			{
				CommandUI[nCntUI] = commandType[nCntUI];
				switch (CommandUI[nCntUI])
				{
				case COMMANDTYPE_NONE:	// 入力なし
					g_aSpellUI[nCntPlayer].g_SpellUI[nCntUI + 2].tex = SPELLUI_TEX_NONE;
					break;
				case COMMANDTYPE_R:	// 赤魔法
					g_aSpellUI[nCntPlayer].g_SpellUI[nCntUI + 2].tex = SPELLUI_TEX_RED;
					break;
				case COMMANDTYPE_G:	// 緑魔法
					g_aSpellUI[nCntPlayer].g_SpellUI[nCntUI + 2].tex = SPELLUI_TEX_GREEN;
					break;
				case COMMANDTYPE_B:	// 青魔法
					g_aSpellUI[nCntPlayer].g_SpellUI[nCntUI + 2].tex = SPELLUI_TEX_BLUE;
					break;
				case COMMANDTYPE_Y:	// 黄魔法
					g_aSpellUI[nCntPlayer].g_SpellUI[nCntUI + 2].tex = SPELLUI_TEX_YELLOW;
					break;
				}
			}
		}
		if (g_aSpellUI[nCntPlayer].nCounterUI > 0)
		{
			g_aSpellUI[nCntPlayer].g_SpellUI[SPELLUI_TYPE_COMMAND0].bDisp = false;	// テクスチャの初期化
			g_aSpellUI[nCntPlayer].g_SpellUI[SPELLUI_TYPE_COMMAND1].bDisp = false;	// テクスチャの初期化
			g_aSpellUI[nCntPlayer].g_SpellUI[SPELLUI_TYPE_COMMAND2].bDisp = false;	// テクスチャの初期化	
			g_aSpellUI[nCntPlayer].g_SpellUI[SPELLUI_TYPE_MAGIC].bDisp = true;		// テクスチャの初期化
			g_aSpellUI[nCntPlayer].nCounterUI--;
		}
		else if (g_aSpellUI[nCntPlayer].nCounterUI <= 0)
		{
			g_aSpellUI[nCntPlayer].g_SpellUI[SPELLUI_TYPE_COMMAND0].bDisp = true;	// テクスチャの初期化
			g_aSpellUI[nCntPlayer].g_SpellUI[SPELLUI_TYPE_COMMAND1].bDisp = true;	// テクスチャの初期化
			g_aSpellUI[nCntPlayer].g_SpellUI[SPELLUI_TYPE_COMMAND2].bDisp = true;	// テクスチャの初期化
			g_aSpellUI[nCntPlayer].g_SpellUI[SPELLUI_TYPE_MAGIC].bDisp = false;		// テクスチャの初期化
		}

		// SPELLUIの移動処理
		float fDiffKeyX = 0.0f;	// 現在の位置と目的の位置の差分を計算
		float fRateKey = (float)g_aSpellUI[nCntPlayer].nFrame/  (float)g_aSpellUI[nCntPlayer].nNumFrame;
		switch (g_aSpellUI[nCntPlayer].state)
		{
		case SPELLUISTATE_NONE:	// 何もしていない状態

			break;

		case SPELLUISTATE_ONSCREEN:	// スクリーン上にある

			break;

		case SPELLUISTATE_APPEAR:	// 出現状態
			fDiffKeyX = g_aSpellUI[nCntPlayer].posDest.x - g_aSpellUI[nCntPlayer].pos.x;
			g_aSpellUI[nCntPlayer].pos.x = g_aSpellUI[nCntPlayer].pos.x + fDiffKeyX * fRateKey;
			g_aSpellUI[nCntPlayer].nFrame++;
			if (g_aSpellUI[nCntPlayer].nFrame == g_aSpellUI[nCntPlayer].nNumFrame)
			{
				g_aSpellUI[nCntPlayer].state = SPELLUISTATE_ONSCREEN;
				switch (operationType)
				{
				case OPERATIONTYPE_2P:	// 2人操作
					if (nCntPlayer == 0)
					{
						g_aSpellUI[nCntPlayer].pos = LEFT_POS;
						g_aSpellUI[nCntPlayer].posDest = LEFT_POS;
					}
					else if (nCntPlayer == 1)
					{
						g_aSpellUI[nCntPlayer].pos = RIGHT_POS;
						g_aSpellUI[nCntPlayer].posDest = RIGHT_POS;
					}
					break;

				default:	// 1人操作
					if (nCntPlayer == 0)
					{
						g_aSpellUI[nCntPlayer].pos = RIGHT_POS;
						g_aSpellUI[nCntPlayer].posDest = RIGHT_POS;
					}
					break;
				}
				
			}
			break;

		case SPELLUISTATE_DISAPPEAR:	// 退出状態
			fDiffKeyX = g_aSpellUI[nCntPlayer].posDest.x - g_aSpellUI[nCntPlayer].pos.x;
			g_aSpellUI[nCntPlayer].pos.x = g_aSpellUI[nCntPlayer].pos.x + fDiffKeyX * fRateKey;
			g_aSpellUI[nCntPlayer].nFrame++;
			if (g_aSpellUI[nCntPlayer].nFrame == g_aSpellUI[nCntPlayer].nNumFrame)
			{
				g_aSpellUI[nCntPlayer].state = SPELLUISTATE_OFFSCREEN;
				g_aSpellUI[nCntPlayer].bDisp = false;
				switch (operationType)
				{
				case OPERATIONTYPE_2P:	// 2人操作
					if (nCntPlayer == 0)
					{
						g_aSpellUI[nCntPlayer].pos = LEFT_OUTPOS;
						g_aSpellUI[nCntPlayer].posDest = LEFT_OUTPOS;
					}
					else if (nCntPlayer == 1)
					{
						g_aSpellUI[nCntPlayer].pos = RIGHT_OUTPOS;
						g_aSpellUI[nCntPlayer].posDest = RIGHT_OUTPOS;
					}
					break;

				default:	// 1人操作
					if (nCntPlayer == 0)
					{
						g_aSpellUI[nCntPlayer].pos = RIGHT_OUTPOS;
						g_aSpellUI[nCntPlayer].posDest = RIGHT_OUTPOS;
					}
					break;
				}

			}
			break;

		case SPELLUISTATE_OFFSCREEN:	// スクリーン外にある

			break;
		}
		
		for (int nCntUI = 0; nCntUI < MAX_SPELLTYPE; nCntUI++, pVtx += 4)	// 頂点バッファに数値を代入
		{
			// 頂点座標の設定
			pVtx[0].pos.x = g_aSpellUI[nCntPlayer].pos.x + g_aSpellUI[nCntPlayer].g_SpellUI[nCntUI].pos.x - g_aSpellUI[nCntPlayer].g_SpellUI[nCntUI].fWidth;
			pVtx[0].pos.y = g_aSpellUI[nCntPlayer].pos.y + g_aSpellUI[nCntPlayer].g_SpellUI[nCntUI].pos.y - g_aSpellUI[nCntPlayer].g_SpellUI[nCntUI].fHeight;
			pVtx[0].pos.z = g_aSpellUI[nCntPlayer].pos.z + g_aSpellUI[nCntPlayer].g_SpellUI[nCntUI].pos.z;
			pVtx[1].pos.x = g_aSpellUI[nCntPlayer].pos.x + g_aSpellUI[nCntPlayer].g_SpellUI[nCntUI].pos.x + g_aSpellUI[nCntPlayer].g_SpellUI[nCntUI].fWidth;
			pVtx[1].pos.y = g_aSpellUI[nCntPlayer].pos.y + g_aSpellUI[nCntPlayer].g_SpellUI[nCntUI].pos.y - g_aSpellUI[nCntPlayer].g_SpellUI[nCntUI].fHeight;
			pVtx[1].pos.z = g_aSpellUI[nCntPlayer].pos.z + g_aSpellUI[nCntPlayer].g_SpellUI[nCntUI].pos.z;
			pVtx[2].pos.x = g_aSpellUI[nCntPlayer].pos.x + g_aSpellUI[nCntPlayer].g_SpellUI[nCntUI].pos.x - g_aSpellUI[nCntPlayer].g_SpellUI[nCntUI].fWidth;
			pVtx[2].pos.y = g_aSpellUI[nCntPlayer].pos.y + g_aSpellUI[nCntPlayer].g_SpellUI[nCntUI].pos.y + g_aSpellUI[nCntPlayer].g_SpellUI[nCntUI].fHeight;
			pVtx[2].pos.z = g_aSpellUI[nCntPlayer].pos.z + g_aSpellUI[nCntPlayer].g_SpellUI[nCntUI].pos.z;
			pVtx[3].pos.x = g_aSpellUI[nCntPlayer].pos.x + g_aSpellUI[nCntPlayer].g_SpellUI[nCntUI].pos.x + g_aSpellUI[nCntPlayer].g_SpellUI[nCntUI].fWidth;
			pVtx[3].pos.y = g_aSpellUI[nCntPlayer].pos.y + g_aSpellUI[nCntPlayer].g_SpellUI[nCntUI].pos.y + g_aSpellUI[nCntPlayer].g_SpellUI[nCntUI].fHeight;
			pVtx[3].pos.z = g_aSpellUI[nCntPlayer].pos.z + g_aSpellUI[nCntPlayer].g_SpellUI[nCntUI].pos.z;

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
	g_pVtxBuffSpellUI->Unlock();
}

//======================================================================================
// spellの描画処理
//======================================================================================
void DrawSpellUI(void)
{
	// 一時的にfogを切る
	SetFogEnable(false);

	LPDIRECT3DDEVICE9 pDevice;	// デバイスへのポインタ
	// デバイスの取得
	pDevice = GetDevice();

	// 頂点バッファをデータストリームに設定
	pDevice->SetStreamSource(0, g_pVtxBuffSpellUI, 0, sizeof(VERTEX_2D));

	// 頂点フォーマットの設定
	pDevice->SetFVF(FVF_VERTEX_2D);

	for (int nCntPlayer = 0; nCntPlayer < MAX_PLAYER; nCntPlayer++)
	{
		if (g_aSpellUI[nCntPlayer].bDisp == false)
		{
			continue;
		}

		for (int nCntUI = 0; nCntUI < MAX_SPELLTYPE; nCntUI++)
		{
			if (g_aSpellUI[nCntPlayer].g_SpellUI[nCntUI].bDisp == true)
			{
				// テクスチャの設定
				pDevice->SetTexture(0, g_apTextureSpellUI[g_aSpellUI[nCntPlayer].g_SpellUI[nCntUI].tex]);

				// ポリゴンの描画
				pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, nCntUI * 4 + (nCntPlayer * MAX_SPELLTYPE * 4), 2);
			}
		}
	}

	// fogを戻す
	SetFogEnable(true);
}

//======================================================================================
// spellの中身の初期化処理
//======================================================================================
void ResetSpellUI(int nIdx)
{
	// 操作方法の状態を取得
	OPERATIONTYPE operationType = GetOperationType();

	for (int nCntUI = 0; nCntUI < MAX_SPELLTEX; nCntUI++)
	{
		for (int nCntUI = 0; nCntUI < MAX_SPELLTYPE; nCntUI++)
		{
			switch (nCntUI)
			{
			case SPELLUI_TYPE_PHONE:	// phone
				g_aSpellUI[nIdx].g_SpellUI[SPELLUI_TYPE_PHONE].tex = SPELLUI_TEX_PHONE;				// phone
				g_aSpellUI[nIdx].g_SpellUI[SPELLUI_TYPE_PHONE].type = SPELLUI_TYPE_PHONE;				// UIの種類の初期化
				g_aSpellUI[nIdx].g_SpellUI[SPELLUI_TYPE_PHONE].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);	// 位置の初期化
				g_aSpellUI[nIdx].g_SpellUI[SPELLUI_TYPE_PHONE].fWidth = PHONE_WIDTH;						// 幅の初期化
				g_aSpellUI[nIdx].g_SpellUI[SPELLUI_TYPE_PHONE].fHeight = PHONE_HEIGHT;						// 高さの初期化
				g_aSpellUI[nIdx].g_SpellUI[SPELLUI_TYPE_PHONE].bDisp = true;								// テクスチャの初期化
				break;

			case SPELLUI_TYPE_SPELL:	// spell
				g_aSpellUI[nIdx].g_SpellUI[SPELLUI_TYPE_SPELL].tex = SPELLUI_TEX_SPELL;				// spell
				g_aSpellUI[nIdx].g_SpellUI[SPELLUI_TYPE_SPELL].type = SPELLUI_TYPE_SPELL;				// UIの種類の初期化
				g_aSpellUI[nIdx].g_SpellUI[SPELLUI_TYPE_SPELL].pos = D3DXVECTOR3(0.0f, SPELL_Y, 0.0f);	// 位置の初期化
				g_aSpellUI[nIdx].g_SpellUI[SPELLUI_TYPE_SPELL].fWidth = SPELL_WIDTH;						// 幅の初期化
				g_aSpellUI[nIdx].g_SpellUI[SPELLUI_TYPE_SPELL].fHeight = SPELL_HEIGHT;						// 高さの初期化
				g_aSpellUI[nIdx].g_SpellUI[SPELLUI_TYPE_SPELL].bDisp = true;								// テクスチャの初期化
				break;

			case SPELLUI_TYPE_COMMAND0:	// 1つ目のコマンド
				g_aSpellUI[nIdx].g_SpellUI[SPELLUI_TYPE_COMMAND0].tex = SPELLUI_TEX_MAGICNULL;							// どの魔法も入力されていない
				g_aSpellUI[nIdx].g_SpellUI[SPELLUI_TYPE_COMMAND0].type = SPELLUI_TYPE_COMMAND0;							// UIの種類の初期化
				g_aSpellUI[nIdx].g_SpellUI[SPELLUI_TYPE_COMMAND0].pos = D3DXVECTOR3(-COMMMAND_DIGIT, COMMMAND_Y, 0.0f);	// 位置の初期化
				g_aSpellUI[nIdx].g_SpellUI[SPELLUI_TYPE_COMMAND0].fWidth = COMMMAND_SIZE;									// 幅の初期化
				g_aSpellUI[nIdx].g_SpellUI[SPELLUI_TYPE_COMMAND0].fHeight = COMMMAND_SIZE;									// 高さの初期化
				g_aSpellUI[nIdx].g_SpellUI[SPELLUI_TYPE_COMMAND0].bDisp = true;												// テクスチャの初期化
				break;

			case SPELLUI_TYPE_COMMAND1:	// 2つ目のコマンド
				g_aSpellUI[nIdx].g_SpellUI[SPELLUI_TYPE_COMMAND1].tex = SPELLUI_TEX_MAGICNULL;				// どの魔法も入力されていない
				g_aSpellUI[nIdx].g_SpellUI[SPELLUI_TYPE_COMMAND1].type = SPELLUI_TYPE_COMMAND1;				// UIの種類の初期化
				g_aSpellUI[nIdx].g_SpellUI[SPELLUI_TYPE_COMMAND1].pos = D3DXVECTOR3(0.0f, COMMMAND_Y, 0.0f);	// 位置の初期化
				g_aSpellUI[nIdx].g_SpellUI[SPELLUI_TYPE_COMMAND1].fWidth = COMMMAND_SIZE;						// 幅の初期化
				g_aSpellUI[nIdx].g_SpellUI[SPELLUI_TYPE_COMMAND1].fHeight = COMMMAND_SIZE;						// 高さの初期化
				g_aSpellUI[nIdx].g_SpellUI[SPELLUI_TYPE_COMMAND1].bDisp = true;									// テクスチャの初期化
				break;

			case SPELLUI_TYPE_COMMAND2:	// 3つ目のコマンド
				g_aSpellUI[nIdx].g_SpellUI[SPELLUI_TYPE_COMMAND2].tex = SPELLUI_TEX_MAGICNULL;							// どの魔法も入力されていない
				g_aSpellUI[nIdx].g_SpellUI[SPELLUI_TYPE_COMMAND2].type = SPELLUI_TYPE_COMMAND2;							// UIの種類の初期化
				g_aSpellUI[nIdx].g_SpellUI[SPELLUI_TYPE_COMMAND2].pos = D3DXVECTOR3(COMMMAND_DIGIT, COMMMAND_Y, 0.0f);	// 位置の初期化
				g_aSpellUI[nIdx].g_SpellUI[SPELLUI_TYPE_COMMAND2].fWidth = COMMMAND_SIZE;									// 幅の初期化
				g_aSpellUI[nIdx].g_SpellUI[SPELLUI_TYPE_COMMAND2].fHeight = COMMMAND_SIZE;									// 高さの初期化
				g_aSpellUI[nIdx].g_SpellUI[SPELLUI_TYPE_COMMAND2].bDisp = true;												// テクスチャの初期化
				break;

			case SPELLUI_TYPE_MAGIC:	// 発動された魔法
				g_aSpellUI[nIdx].g_SpellUI[SPELLUI_TYPE_MAGIC].tex = SPELLUI_TEX_NONE;					// どの魔法も入力されていない
				g_aSpellUI[nIdx].g_SpellUI[SPELLUI_TYPE_MAGIC].type = SPELLUI_TYPE_MAGIC;				// UIの種類の初期化
				g_aSpellUI[nIdx].g_SpellUI[SPELLUI_TYPE_MAGIC].pos = D3DXVECTOR3(0.0f, MAGIC_Y, 0.0f);	// 位置の初期化
				g_aSpellUI[nIdx].g_SpellUI[SPELLUI_TYPE_MAGIC].fWidth = MAGIC_SIZE;						// 幅の初期化
				g_aSpellUI[nIdx].g_SpellUI[SPELLUI_TYPE_MAGIC].fHeight = MAGIC_SIZE;						// 高さの初期化
				g_aSpellUI[nIdx].g_SpellUI[SPELLUI_TYPE_MAGIC].bDisp = false;							// テクスチャの初期化
				break;

			case SPELLUI_TYPE_RED:	// 赤魔法
				g_aSpellUI[nIdx].g_SpellUI[SPELLUI_TYPE_RED].tex = SPELLUI_TEX_RED;									// 赤魔法
				g_aSpellUI[nIdx].g_SpellUI[SPELLUI_TYPE_RED].type = SPELLUI_TYPE_RED;									// UIの種類の初期化
				g_aSpellUI[nIdx].g_SpellUI[SPELLUI_TYPE_RED].pos = D3DXVECTOR3(COMMMAND_DIGIT, RBCOMMAND_Y, 0.0f);	// 位置の初期化
				g_aSpellUI[nIdx].g_SpellUI[SPELLUI_TYPE_RED].fWidth = COMMMAND_SIZE;									// 幅の初期化
				g_aSpellUI[nIdx].g_SpellUI[SPELLUI_TYPE_RED].fHeight = COMMMAND_SIZE;									// 高さの初期化
				g_aSpellUI[nIdx].g_SpellUI[SPELLUI_TYPE_RED].bDisp = true;												// テクスチャの初期化
				break;

			case SPELLUI_TYPE_GREEN:	// 緑魔法
				g_aSpellUI[nIdx].g_SpellUI[SPELLUI_TYPE_GREEN].tex = SPELLUI_TEX_GREEN;					// 緑魔法
				g_aSpellUI[nIdx].g_SpellUI[SPELLUI_TYPE_GREEN].type = SPELLUI_TYPE_GREEN;					// UIの種類の初期化
				g_aSpellUI[nIdx].g_SpellUI[SPELLUI_TYPE_GREEN].pos = D3DXVECTOR3(0.0f, GCOMMAND_Y, 0.0f);	// 位置の初期化
				g_aSpellUI[nIdx].g_SpellUI[SPELLUI_TYPE_GREEN].fWidth = COMMMAND_SIZE;						// 幅の初期化
				g_aSpellUI[nIdx].g_SpellUI[SPELLUI_TYPE_GREEN].fHeight = COMMMAND_SIZE;						// 高さの初期化
				g_aSpellUI[nIdx].g_SpellUI[SPELLUI_TYPE_GREEN].bDisp = true;									// テクスチャの初期化
				break;

			case SPELLUI_TYPE_BLUE:	// 青魔法
				g_aSpellUI[nIdx].g_SpellUI[SPELLUI_TYPE_BLUE].tex = SPELLUI_TEX_BLUE;									// 青魔法
				g_aSpellUI[nIdx].g_SpellUI[SPELLUI_TYPE_BLUE].type = SPELLUI_TYPE_BLUE;								// UIの種類の初期化
				g_aSpellUI[nIdx].g_SpellUI[SPELLUI_TYPE_BLUE].pos = D3DXVECTOR3(-COMMMAND_DIGIT, RBCOMMAND_Y, 0.0f);	// 位置の初期化
				g_aSpellUI[nIdx].g_SpellUI[SPELLUI_TYPE_BLUE].fWidth = COMMMAND_SIZE;									// 幅の初期化
				g_aSpellUI[nIdx].g_SpellUI[SPELLUI_TYPE_BLUE].fHeight = COMMMAND_SIZE;									// 高さの初期化
				g_aSpellUI[nIdx].g_SpellUI[SPELLUI_TYPE_BLUE].bDisp = true;												// テクスチャの初期化
				break;

			case SPELLUI_TYPE_YELLOW:	// 黄魔法
				g_aSpellUI[nIdx].g_SpellUI[SPELLUI_TYPE_YELLOW].tex = SPELLUI_TEX_YELLOW;					// 黄魔法
				g_aSpellUI[nIdx].g_SpellUI[SPELLUI_TYPE_YELLOW].type = SPELLUI_TYPE_YELLOW;					// UIの種類の初期化
				g_aSpellUI[nIdx].g_SpellUI[SPELLUI_TYPE_YELLOW].pos = D3DXVECTOR3(0.0f, YCOMMAND_Y, 0.0f);	// 位置の初期化
				g_aSpellUI[nIdx].g_SpellUI[SPELLUI_TYPE_YELLOW].fWidth = COMMMAND_SIZE;						// 幅の初期化
				g_aSpellUI[nIdx].g_SpellUI[SPELLUI_TYPE_YELLOW].fHeight = COMMMAND_SIZE;						// 高さの初期化
				g_aSpellUI[nIdx].g_SpellUI[SPELLUI_TYPE_YELLOW].bDisp = true;									// テクスチャの初期化
				break;

			case SPELLUI_TYPE_OP_R:	// 赤魔法発動ボタン
				if (operationType == OPERATIONTYPE_1P || operationType == OPERATIONTYPE_2P)	// ジョイパッド操作
				{
					g_aSpellUI[nIdx].g_SpellUI[SPELLUI_TYPE_OP_R].tex = SPELLUI_TEX_B;				// Bボタン
				}
				else if (OPERATIONTYPE_KEYBOARD)	// キーボード1P操作
				{
					g_aSpellUI[nIdx].g_SpellUI[SPELLUI_TYPE_OP_R].tex = SPELLUI_TEX_1;				// 1キー
				}
				g_aSpellUI[nIdx].g_SpellUI[SPELLUI_TYPE_OP_R].type = SPELLUI_TYPE_OP_R;					// UIの種類の初期化
				g_aSpellUI[nIdx].g_SpellUI[SPELLUI_TYPE_OP_R].pos = D3DXVECTOR3(RB_OPTYPE_X, RBCOMMAND_Y, 0.0f);	// 位置の初期化
				g_aSpellUI[nIdx].g_SpellUI[SPELLUI_TYPE_OP_R].fWidth = BUTTON_SIZE;						// 幅の初期化
				g_aSpellUI[nIdx].g_SpellUI[SPELLUI_TYPE_OP_R].fHeight = BUTTON_SIZE;						// 高さの初期化
				g_aSpellUI[nIdx].g_SpellUI[SPELLUI_TYPE_OP_R].bDisp = true;								// テクスチャの初期化
				break;

			case SPELLUI_TYPE_OP_G:	// 緑魔法発動ボタン
				if (operationType == OPERATIONTYPE_1P || operationType == OPERATIONTYPE_2P)	// ジョイパッド操作
				{
					g_aSpellUI[nIdx].g_SpellUI[SPELLUI_TYPE_OP_G].tex = SPELLUI_TEX_A;				// Aボタン
				}
				else if (OPERATIONTYPE_KEYBOARD)	// キーボード1P操作
				{
					g_aSpellUI[nIdx].g_SpellUI[SPELLUI_TYPE_OP_G].tex = SPELLUI_TEX_2;				// 2キー
				}
				g_aSpellUI[nIdx].g_SpellUI[SPELLUI_TYPE_OP_G].type = SPELLUI_TYPE_OP_G;				// UIの種類の初期化
				g_aSpellUI[nIdx].g_SpellUI[SPELLUI_TYPE_OP_G].pos = D3DXVECTOR3(0.0f, G_OPTYPE_Y, 0.0f);	// 位置の初期化
				g_aSpellUI[nIdx].g_SpellUI[SPELLUI_TYPE_OP_G].fWidth = BUTTON_SIZE;							// 幅の初期化
				g_aSpellUI[nIdx].g_SpellUI[SPELLUI_TYPE_OP_G].fHeight = BUTTON_SIZE;						// 高さの初期化
				g_aSpellUI[nIdx].g_SpellUI[SPELLUI_TYPE_OP_G].bDisp = true;							// テクスチャの初期化
				break;

			case SPELLUI_TYPE_OP_B:	// 青魔法発動ボタン
				if (operationType == OPERATIONTYPE_1P || operationType == OPERATIONTYPE_2P)	// ジョイパッド操作
				{
					g_aSpellUI[nIdx].g_SpellUI[SPELLUI_TYPE_OP_B].tex = SPELLUI_TEX_X;				// Xボタン
				}
				else if (OPERATIONTYPE_KEYBOARD)	// キーボード1P操作
				{
					g_aSpellUI[nIdx].g_SpellUI[SPELLUI_TYPE_OP_B].tex = SPELLUI_TEX_3;				// 3キー
				}
				g_aSpellUI[nIdx].g_SpellUI[SPELLUI_TYPE_OP_B].type = SPELLUI_TYPE_OP_B;				// UIの種類の初期化
				g_aSpellUI[nIdx].g_SpellUI[SPELLUI_TYPE_OP_B].pos = D3DXVECTOR3(-RB_OPTYPE_X, RBCOMMAND_Y, 0.0f);	// 位置の初期化
				g_aSpellUI[nIdx].g_SpellUI[SPELLUI_TYPE_OP_B].fWidth = BUTTON_SIZE;							// 幅の初期化
				g_aSpellUI[nIdx].g_SpellUI[SPELLUI_TYPE_OP_B].fHeight = BUTTON_SIZE;						// 高さの初期化
				g_aSpellUI[nIdx].g_SpellUI[SPELLUI_TYPE_OP_B].bDisp = true;							// テクスチャの初期化
				break;

			case SPELLUI_TYPE_OP_Y:	// 黄魔法発動ボタン
				if (operationType == OPERATIONTYPE_1P || operationType == OPERATIONTYPE_2P)	// ジョイパッド操作
				{
					g_aSpellUI[nIdx].g_SpellUI[SPELLUI_TYPE_OP_Y].tex = SPELLUI_TEX_Y;				// Yボタン
				}
				else if (OPERATIONTYPE_KEYBOARD)	// キーボード1P操作
				{
					g_aSpellUI[nIdx].g_SpellUI[SPELLUI_TYPE_OP_Y].tex = SPELLUI_TEX_4;				// 4キー
				}
				g_aSpellUI[nIdx].g_SpellUI[SPELLUI_TYPE_OP_Y].type = SPELLUI_TYPE_OP_Y;				// UIの種類の初期化
				g_aSpellUI[nIdx].g_SpellUI[SPELLUI_TYPE_OP_Y].pos = D3DXVECTOR3(0.0f, Y_OPTYPE_Y, 0.0f);	// 位置の初期化
				g_aSpellUI[nIdx].g_SpellUI[SPELLUI_TYPE_OP_Y].fWidth = BUTTON_SIZE;							// 幅の初期化
				g_aSpellUI[nIdx].g_SpellUI[SPELLUI_TYPE_OP_Y].fHeight = BUTTON_SIZE;						// 高さの初期化
				g_aSpellUI[nIdx].g_SpellUI[SPELLUI_TYPE_OP_Y].bDisp = true;							// テクスチャの初期化
				break;
			}
		}

	}
}

//======================================================================================
// UIを発動中の魔法に変更
//======================================================================================
void SetSpellUI(MAGICTYPE magicType, int nIdx, int nDispTime)
{
	switch (magicType)
	{
	case MAGICTYPE_NONE:	// 何もない場合
		g_aSpellUI[nIdx].g_SpellUI[SPELLUI_TYPE_MAGIC].tex = SPELLUI_TEX_NONE;			// 詠唱失敗のテクスチャに切り替え
		break;

	case MAGICTYPE_LEVITATION:	// 浮遊
		g_aSpellUI[nIdx].g_SpellUI[SPELLUI_TYPE_MAGIC].tex = SPELLUI_TEX_LEVITATION;	// 浮遊のテクスチャに切り替え
		break;

	case MAGICTYPE_COMBUSTION:	// 燃焼
		g_aSpellUI[nIdx].g_SpellUI[SPELLUI_TYPE_MAGIC].tex = SPELLUI_TEX_COMBUSTION;	// 燃焼のテクスチャに切り替え
		break;

	case MAGICTYPE_FLOOD:	// 洪水、氾濫
		g_aSpellUI[nIdx].g_SpellUI[SPELLUI_TYPE_MAGIC].tex = SPELLUI_TEX_FLOOD;	// 洪水、氾濫のテクスチャに切り替え
		break;

	case MAGICTYPE_FLASH:	// フラッシュ
		g_aSpellUI[nIdx].g_SpellUI[SPELLUI_TYPE_MAGIC].tex = SPELLUI_TEX_FLASH;	// フラッシュのテクスチャに切り替え
		break;

	case MAGICTYPE_FIREBALL:	// 火球
		g_aSpellUI[nIdx].g_SpellUI[SPELLUI_TYPE_MAGIC].tex = SPELLUI_TEX_FIREBALL;	// 火球のテクスチャに切り替え
		break;

	case MAGICTYPE_SUNSETDELAY:	// 太陽の動きを止める
		g_aSpellUI[nIdx].g_SpellUI[SPELLUI_TYPE_MAGIC].tex = SPELLUI_TEX_SUNSETDELAY;	// 太陽の動きを止めるテクスチャに切り替え
		break;

	case MAGICTYPE_RAINPRAY:	// 雨乞い
		g_aSpellUI[nIdx].g_SpellUI[SPELLUI_TYPE_MAGIC].tex = SPELLUI_TEX_RAINPRAY;	// 雨乞いのテクスチャに切り替え
		break;

	case MAGICTYPE_FREEZE:	// 凍結
		g_aSpellUI[nIdx].g_SpellUI[SPELLUI_TYPE_MAGIC].tex = SPELLUI_TEX_FREEZE;	// 凍結のテクスチャに切り替え
		break;

	case MAGICTYPE_GROWTH:	// 成長
		g_aSpellUI[nIdx].g_SpellUI[SPELLUI_TYPE_MAGIC].tex = SPELLUI_TEX_GROWTH;	// 成長のテクスチャに切り替え
		break;

	case MAGICTYPE_ACCELERATION:	// 加速
		g_aSpellUI[nIdx].g_SpellUI[SPELLUI_TYPE_MAGIC].tex = SPELLUI_TEX_ACCELERATION;	// 加速のテクスチャに切り替え
		break;

	case MAGICTYPE_TIMEREVERT:	// 巻き戻し
		g_aSpellUI[nIdx].g_SpellUI[SPELLUI_TYPE_MAGIC].tex = SPELLUI_TEX_TIMEREVERT;	// 巻き戻しのテクスチャに切り替え
		break;
	}
	g_aSpellUI[nIdx].nCounterUI = nDispTime;
}
