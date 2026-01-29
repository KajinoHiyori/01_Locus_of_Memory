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

// マクロ定義
#define MAX_SPELLTEX	(SPELLUI_TEX_MAX)	// テクスチャの最大数
#define MAX_SPELLTYPE	(SPELLUI_TYPE_MAX)	// 表示されるUIの種類
#define LEFT_POSX		(1100.0f)			// 左のUIのX軸
#define LEFT_POS		(D3DXVECTOR3(LEFT_POSX, 360.0f, 0.0f))
#define COMMMAND_SIZE	(30.0f)		// コマンドボタンの大きさ
#define BUTTON_SIZE		(15.0f)		// 操作キー表示の大きさ
#define SPELL_WIDTH		(135.0f)	// spellメニューの幅
#define SPELL_HEIGHT	(30.0f)		// spellメニューの高さ
#define PHONE_WIDTH		(135.0f)	// スマホの幅
#define PHONE_HEIGHT	(285.0f)	// スマホの幅

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

// グローバル変数
LPDIRECT3DTEXTURE9	g_apTextureSpellUI[MAX_SPELLTEX] = {};	// テクスチャへのポインタ
LPDIRECT3DVERTEXBUFFER9	g_pVtxBuffSpellUI = NULL;			// 頂点バッファへのポインタ
SPELLUI g_SpellUI[MAX_PLAYER][MAX_SPELLTEX];				// UIの表示処理
bool g_bAllDisp[MAX_PLAYER];								// UIの全体表示管理
D3DXVECTOR3 MainPos[MAX_PLAYER];							// UIの全体管理位置

const char* c_apFilenameSpellUI[MAX_SPELLTEX] =
{
	"data\\TEXTURE\\pause000.png",
	"data\\TEXTURE\\pause000.png",
	"data\\TEXTURE\\pause001.png",
	"data\\TEXTURE\\pause002.png",
	"data\\TEXTURE\\pause100.png",
	"data\\TEXTURE\\pause101.png",
	"data\\TEXTURE\\pause101.png",
	"data\\TEXTURE\\pause101.png",
	"data\\TEXTURE\\pause101.png",
	"data\\TEXTURE\\pause101.png",
	"data\\TEXTURE\\pause101.png",
	"data\\TEXTURE\\pause101.png",
	"data\\TEXTURE\\pause101.png",
	"data\\TEXTURE\\pause101.png",
	"data\\TEXTURE\\pause101.png",
	"data\\TEXTURE\\pause101.png",
	"data\\TEXTURE\\pause101.png",
	"data\\TEXTURE\\pause101.png",
	"data\\TEXTURE\\pause101.png",
	"data\\TEXTURE\\pause101.png",
	"data\\TEXTURE\\pause101.png",
	"data\\TEXTURE\\pause101.png",
	"data\\TEXTURE\\pause101.png",
	"data\\TEXTURE\\pause101.png",
	"data\\TEXTURE\\pause101.png",
	"data\\TEXTURE\\pause101.png",
	"data\\TEXTURE\\pause101.png",
};

//======================================================================================
// ポーズの初期化処理
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
			g_SpellUI[nCntPlayer][nCntUI].tex		= SPELLUI_TEX_MAGICNULL;			// テクスチャの初期化
			g_SpellUI[nCntPlayer][nCntUI].type		= SPELLUI_TYPE_COMMAND0;			// UIの種類の初期化
			g_SpellUI[nCntPlayer][nCntUI].pos		= D3DXVECTOR3(0.0f, 0.0f, 0.0f);	// 位置の初期化
			g_SpellUI[nCntPlayer][nCntUI].fWidth	= 0.0f;								// 幅の初期化
			g_SpellUI[nCntPlayer][nCntUI].fHeight	= 0.0f;								// 高さの初期化
			g_SpellUI[nCntPlayer][nCntUI].bDisp		= false;							// テクスチャの初期化
		}
		g_bAllDisp[nCntPlayer] = true;
		MainPos[nCntPlayer] = D3DXVECTOR3(0.0f, 0.0f, 0.0f);	// 位置の初期化
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
		case OPERATIONTYPE_2P:	// 2P操作

			break;

		default:	// 1人操作
			if (nCntPlayer > 0)
			{
				continue;
			}
			for (int nCntUI = 0; nCntUI < MAX_SPELLTYPE; nCntUI++)
			{
				switch (nCntUI)
				{
				case SPELLUI_TYPE_COMMAND0:	// 1つ目のコマンド
					g_SpellUI[nCntPlayer][SPELLUI_TYPE_COMMAND0].tex = SPELLUI_TEX_MAGICNULL;			// どの魔法も入力されていない
					g_SpellUI[nCntPlayer][SPELLUI_TYPE_COMMAND0].type = SPELLUI_TYPE_COMMAND0;			// UIの種類の初期化
					g_SpellUI[nCntPlayer][SPELLUI_TYPE_COMMAND0].pos = D3DXVECTOR3(-30.0f, 50.0f, 0.0f);// 位置の初期化
					g_SpellUI[nCntPlayer][SPELLUI_TYPE_COMMAND0].fWidth = COMMMAND_SIZE;						// 幅の初期化
					g_SpellUI[nCntPlayer][SPELLUI_TYPE_COMMAND0].fHeight = COMMMAND_SIZE;						// 高さの初期化
					g_SpellUI[nCntPlayer][SPELLUI_TYPE_COMMAND0].bDisp = true;							// テクスチャの初期化
					break;

				case SPELLUI_TYPE_COMMAND1:	// 2つ目のコマンド
					g_SpellUI[nCntPlayer][SPELLUI_TYPE_COMMAND1].tex = SPELLUI_TEX_MAGICNULL;			// どの魔法も入力されていない
					g_SpellUI[nCntPlayer][SPELLUI_TYPE_COMMAND1].type = SPELLUI_TYPE_COMMAND1;			// UIの種類の初期化
					g_SpellUI[nCntPlayer][SPELLUI_TYPE_COMMAND1].pos = D3DXVECTOR3(0.0f, 50.0f, 0.0f);	// 位置の初期化
					g_SpellUI[nCntPlayer][SPELLUI_TYPE_COMMAND1].fWidth = COMMMAND_SIZE;							// 幅の初期化
					g_SpellUI[nCntPlayer][SPELLUI_TYPE_COMMAND1].fHeight = COMMMAND_SIZE;						// 高さの初期化
					g_SpellUI[nCntPlayer][SPELLUI_TYPE_COMMAND1].bDisp = true;							// テクスチャの初期化
					break;

				case SPELLUI_TYPE_COMMAND2:	// 3つ目のコマンド
					g_SpellUI[nCntPlayer][SPELLUI_TYPE_COMMAND2].tex = SPELLUI_TEX_MAGICNULL;			// どの魔法も入力されていない
					g_SpellUI[nCntPlayer][SPELLUI_TYPE_COMMAND2].type = SPELLUI_TYPE_COMMAND2;			// UIの種類の初期化
					g_SpellUI[nCntPlayer][SPELLUI_TYPE_COMMAND2].pos = D3DXVECTOR3(30.0f, 50.0f, 0.0f);	// 位置の初期化
					g_SpellUI[nCntPlayer][SPELLUI_TYPE_COMMAND2].fWidth = COMMMAND_SIZE;							// 幅の初期化
					g_SpellUI[nCntPlayer][SPELLUI_TYPE_COMMAND2].fHeight = COMMMAND_SIZE;						// 高さの初期化
					g_SpellUI[nCntPlayer][SPELLUI_TYPE_COMMAND2].bDisp = true;							// テクスチャの初期化
					break;

				case SPELLUI_TYPE_MAGIC:	// 発動された魔法
					g_SpellUI[nCntPlayer][SPELLUI_TYPE_MAGIC].tex = SPELLUI_TEX_MAGICNULL;			// どの魔法も入力されていない
					g_SpellUI[nCntPlayer][SPELLUI_TYPE_MAGIC].type = SPELLUI_TYPE_MAGIC;			// UIの種類の初期化
					g_SpellUI[nCntPlayer][SPELLUI_TYPE_MAGIC].pos = D3DXVECTOR3(0.0f, 25.0f, 0.0f);	// 位置の初期化
					g_SpellUI[nCntPlayer][SPELLUI_TYPE_MAGIC].fWidth = COMMMAND_SIZE;						// 幅の初期化
					g_SpellUI[nCntPlayer][SPELLUI_TYPE_MAGIC].fHeight = COMMMAND_SIZE;						// 高さの初期化
					g_SpellUI[nCntPlayer][SPELLUI_TYPE_MAGIC].bDisp = false;						// テクスチャの初期化
					break;

				case SPELLUI_TYPE_RED:	// 赤魔法
					g_SpellUI[nCntPlayer][SPELLUI_TYPE_RED].tex = SPELLUI_TEX_RED;					// 赤魔法
					g_SpellUI[nCntPlayer][SPELLUI_TYPE_RED].type = SPELLUI_TYPE_RED;				// UIの種類の初期化
					g_SpellUI[nCntPlayer][SPELLUI_TYPE_RED].pos = D3DXVECTOR3(50.0f, 150.0f, 0.0f);	// 位置の初期化
					g_SpellUI[nCntPlayer][SPELLUI_TYPE_RED].fWidth = COMMMAND_SIZE;							// 幅の初期化
					g_SpellUI[nCntPlayer][SPELLUI_TYPE_RED].fHeight = COMMMAND_SIZE;							// 高さの初期化
					g_SpellUI[nCntPlayer][SPELLUI_TYPE_RED].bDisp = true;							// テクスチャの初期化
					break;

				case SPELLUI_TYPE_GREEN:	// 緑魔法
					g_SpellUI[nCntPlayer][SPELLUI_TYPE_GREEN].tex = SPELLUI_TEX_GREEN;				// 緑魔法
					g_SpellUI[nCntPlayer][SPELLUI_TYPE_GREEN].type = SPELLUI_TYPE_GREEN;			// UIの種類の初期化
					g_SpellUI[nCntPlayer][SPELLUI_TYPE_GREEN].pos = D3DXVECTOR3(0.0f, 200.0f, 0.0f);	// 位置の初期化
					g_SpellUI[nCntPlayer][SPELLUI_TYPE_GREEN].fWidth = COMMMAND_SIZE;						// 幅の初期化
					g_SpellUI[nCntPlayer][SPELLUI_TYPE_GREEN].fHeight = COMMMAND_SIZE;						// 高さの初期化
					g_SpellUI[nCntPlayer][SPELLUI_TYPE_GREEN].bDisp = true;						// テクスチャの初期化
					break;

				case SPELLUI_TYPE_BLUE:	// 青魔法
					g_SpellUI[nCntPlayer][SPELLUI_TYPE_BLUE].tex = SPELLUI_TEX_BLUE;				// 青魔法
					g_SpellUI[nCntPlayer][SPELLUI_TYPE_BLUE].type = SPELLUI_TYPE_BLUE;				// UIの種類の初期化
					g_SpellUI[nCntPlayer][SPELLUI_TYPE_BLUE].pos = D3DXVECTOR3(-50.0f, 150.0f, 0.0f);	// 位置の初期化
					g_SpellUI[nCntPlayer][SPELLUI_TYPE_BLUE].fWidth = COMMMAND_SIZE;							// 幅の初期化
					g_SpellUI[nCntPlayer][SPELLUI_TYPE_BLUE].fHeight = COMMMAND_SIZE;						// 高さの初期化
					g_SpellUI[nCntPlayer][SPELLUI_TYPE_BLUE].bDisp = true;							// テクスチャの初期化
					break;

				case SPELLUI_TYPE_YELLOW:	// 黄魔法
					g_SpellUI[nCntPlayer][SPELLUI_TYPE_YELLOW].tex = SPELLUI_TEX_YELLOW;				// 黄魔法
					g_SpellUI[nCntPlayer][SPELLUI_TYPE_YELLOW].type = SPELLUI_TYPE_YELLOW;				// UIの種類の初期化
					g_SpellUI[nCntPlayer][SPELLUI_TYPE_YELLOW].pos = D3DXVECTOR3(0.0f, 100.0f, 0.0f);	// 位置の初期化
					g_SpellUI[nCntPlayer][SPELLUI_TYPE_YELLOW].fWidth = COMMMAND_SIZE;							// 幅の初期化
					g_SpellUI[nCntPlayer][SPELLUI_TYPE_YELLOW].fHeight = COMMMAND_SIZE;						// 高さの初期化
					g_SpellUI[nCntPlayer][SPELLUI_TYPE_YELLOW].bDisp = true;							// テクスチャの初期化
					break;

				case SPELLUI_TYPE_OP_R:	// 赤魔法発動ボタン
					if (operationType == OPERATIONTYPE_1P)	// ジョイパッド1P操作
					{
						g_SpellUI[nCntPlayer][SPELLUI_TYPE_OP_R].tex = SPELLUI_TEX_B;				// Bボタン
					}
					else if (OPERATIONTYPE_KEYBOARD)	// キーボード1P操作
					{
						g_SpellUI[nCntPlayer][SPELLUI_TYPE_OP_R].tex = SPELLUI_TEX_1;				// 1キー
					}
					g_SpellUI[nCntPlayer][SPELLUI_TYPE_OP_R].type = SPELLUI_TYPE_OP_R;				// UIの種類の初期化
					g_SpellUI[nCntPlayer][SPELLUI_TYPE_OP_R].pos = D3DXVECTOR3(50.0f, 170.0f, 0.0f);	// 位置の初期化
					g_SpellUI[nCntPlayer][SPELLUI_TYPE_OP_R].fWidth = BUTTON_SIZE;							// 幅の初期化
					g_SpellUI[nCntPlayer][SPELLUI_TYPE_OP_R].fHeight = BUTTON_SIZE;						// 高さの初期化
					g_SpellUI[nCntPlayer][SPELLUI_TYPE_OP_R].bDisp = true;							// テクスチャの初期化
					break;

				case SPELLUI_TYPE_OP_G:	// 緑魔法発動ボタン
					if (operationType == OPERATIONTYPE_1P)	// ジョイパッド1P操作
					{
						g_SpellUI[nCntPlayer][SPELLUI_TYPE_OP_G].tex = SPELLUI_TEX_A;				// Aボタン
					}
					else if (OPERATIONTYPE_KEYBOARD)	// キーボード1P操作
					{
						g_SpellUI[nCntPlayer][SPELLUI_TYPE_OP_G].tex = SPELLUI_TEX_2;				// 2キー
					}
					g_SpellUI[nCntPlayer][SPELLUI_TYPE_OP_G].type = SPELLUI_TYPE_OP_G;				// UIの種類の初期化
					g_SpellUI[nCntPlayer][SPELLUI_TYPE_OP_G].pos = D3DXVECTOR3(0.0f, 220.0f, 0.0f);	// 位置の初期化
					g_SpellUI[nCntPlayer][SPELLUI_TYPE_OP_G].fWidth = BUTTON_SIZE;							// 幅の初期化
					g_SpellUI[nCntPlayer][SPELLUI_TYPE_OP_G].fHeight = BUTTON_SIZE;						// 高さの初期化
					g_SpellUI[nCntPlayer][SPELLUI_TYPE_OP_G].bDisp = true;							// テクスチャの初期化
					break;

				case SPELLUI_TYPE_OP_B:	// 青魔法発動ボタン
					if (operationType == OPERATIONTYPE_1P)	// ジョイパッド1P操作
					{
						g_SpellUI[nCntPlayer][SPELLUI_TYPE_OP_B].tex = SPELLUI_TEX_X;				// Xボタン
					}
					else if (OPERATIONTYPE_KEYBOARD)	// キーボード1P操作
					{
						g_SpellUI[nCntPlayer][SPELLUI_TYPE_OP_B].tex = SPELLUI_TEX_3;				// 3キー
					}
					g_SpellUI[nCntPlayer][SPELLUI_TYPE_OP_B].type = SPELLUI_TYPE_OP_B;				// UIの種類の初期化
					g_SpellUI[nCntPlayer][SPELLUI_TYPE_OP_B].pos = D3DXVECTOR3(-50.0f, 170.0f, 0.0f);	// 位置の初期化
					g_SpellUI[nCntPlayer][SPELLUI_TYPE_OP_B].fWidth = BUTTON_SIZE;							// 幅の初期化
					g_SpellUI[nCntPlayer][SPELLUI_TYPE_OP_B].fHeight = BUTTON_SIZE;						// 高さの初期化
					g_SpellUI[nCntPlayer][SPELLUI_TYPE_OP_B].bDisp = true;							// テクスチャの初期化
					break;

				case SPELLUI_TYPE_OP_Y:	// 黄魔法発動ボタン
					if (operationType == OPERATIONTYPE_1P)	// ジョイパッド1P操作
					{
						g_SpellUI[nCntPlayer][SPELLUI_TYPE_OP_Y].tex = SPELLUI_TEX_Y;				// Yボタン
					}
					else if (OPERATIONTYPE_KEYBOARD)	// キーボード1P操作
					{
						g_SpellUI[nCntPlayer][SPELLUI_TYPE_OP_Y].tex = SPELLUI_TEX_4;				// 4キー
					}
					g_SpellUI[nCntPlayer][SPELLUI_TYPE_OP_Y].type = SPELLUI_TYPE_OP_Y;				// UIの種類の初期化
					g_SpellUI[nCntPlayer][SPELLUI_TYPE_OP_Y].pos = D3DXVECTOR3(0.0f, 120.0f, 0.0f);	// 位置の初期化
					g_SpellUI[nCntPlayer][SPELLUI_TYPE_OP_Y].fWidth = BUTTON_SIZE;							// 幅の初期化
					g_SpellUI[nCntPlayer][SPELLUI_TYPE_OP_Y].fHeight = BUTTON_SIZE;						// 高さの初期化
					g_SpellUI[nCntPlayer][SPELLUI_TYPE_OP_Y].bDisp = true;							// テクスチャの初期化
					break;

				case SPELLUI_TYPE_SPELL:	// spell
					g_SpellUI[nCntPlayer][SPELLUI_TYPE_SPELL].tex = SPELLUI_TEX_SPELL;				// spell
					g_SpellUI[nCntPlayer][SPELLUI_TYPE_SPELL].type = SPELLUI_TYPE_SPELL;				// UIの種類の初期化
					g_SpellUI[nCntPlayer][SPELLUI_TYPE_SPELL].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);	// 位置の初期化
					g_SpellUI[nCntPlayer][SPELLUI_TYPE_SPELL].fWidth = SPELL_WIDTH;							// 幅の初期化
					g_SpellUI[nCntPlayer][SPELLUI_TYPE_SPELL].fHeight = SPELL_HEIGHT;						// 高さの初期化
					g_SpellUI[nCntPlayer][SPELLUI_TYPE_SPELL].bDisp = true;							// テクスチャの初期化
					break;

				case SPELLUI_TYPE_PHONE:	// phone
					g_SpellUI[nCntPlayer][SPELLUI_TYPE_PHONE].tex = SPELLUI_TEX_PHONE;				// spell
					g_SpellUI[nCntPlayer][SPELLUI_TYPE_PHONE].type = SPELLUI_TYPE_PHONE;			// UIの種類の初期化
					g_SpellUI[nCntPlayer][SPELLUI_TYPE_PHONE].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);	// 位置の初期化
					g_SpellUI[nCntPlayer][SPELLUI_TYPE_PHONE].fWidth = PHONE_WIDTH;					// 幅の初期化
					g_SpellUI[nCntPlayer][SPELLUI_TYPE_PHONE].fHeight = PHONE_HEIGHT;				// 高さの初期化
					g_SpellUI[nCntPlayer][SPELLUI_TYPE_PHONE].bDisp = true;							// テクスチャの初期化
					break;
				}

				g_bAllDisp[nCntPlayer] = true;
				MainPos[nCntPlayer] = LEFT_POS;
			}


			break;
		}

		for (int nCntUI = 0; nCntUI < MAX_SPELLTYPE; nCntUI++, pVtx += 4)	// 頂点バッファに数値を代入
		{
			// 頂点座標の設定
			pVtx[0].pos = D3DXVECTOR3(MainPos[nCntPlayer].x + g_SpellUI[nCntPlayer][nCntUI].pos.x - g_SpellUI[nCntPlayer][nCntUI].fWidth, MainPos[nCntPlayer].y + g_SpellUI[nCntPlayer][nCntUI].pos.y - g_SpellUI[nCntPlayer][nCntUI].fHeight, MainPos[nCntPlayer].z + g_SpellUI[nCntPlayer][nCntUI].pos.z);
			pVtx[1].pos = D3DXVECTOR3(MainPos[nCntPlayer].x + g_SpellUI[nCntPlayer][nCntUI].pos.x + g_SpellUI[nCntPlayer][nCntUI].fWidth, MainPos[nCntPlayer].y + g_SpellUI[nCntPlayer][nCntUI].pos.y - g_SpellUI[nCntPlayer][nCntUI].fHeight, MainPos[nCntPlayer].z + g_SpellUI[nCntPlayer][nCntUI].pos.z);
			pVtx[2].pos = D3DXVECTOR3(MainPos[nCntPlayer].x + g_SpellUI[nCntPlayer][nCntUI].pos.x - g_SpellUI[nCntPlayer][nCntUI].fWidth, MainPos[nCntPlayer].y + g_SpellUI[nCntPlayer][nCntUI].pos.y + g_SpellUI[nCntPlayer][nCntUI].fHeight, MainPos[nCntPlayer].z + g_SpellUI[nCntPlayer][nCntUI].pos.z);
			pVtx[3].pos = D3DXVECTOR3(MainPos[nCntPlayer].x + g_SpellUI[nCntPlayer][nCntUI].pos.x + g_SpellUI[nCntPlayer][nCntUI].fWidth, MainPos[nCntPlayer].y + g_SpellUI[nCntPlayer][nCntUI].pos.y + g_SpellUI[nCntPlayer][nCntUI].fHeight, MainPos[nCntPlayer].z + g_SpellUI[nCntPlayer][nCntUI].pos.z);

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
// ポーズの終了処理
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
// ポーズの更新処理
//======================================================================================
void UpdateSpellUI(void)
{
#if 0
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
		g_nSelectSpellUI[nCntOP]--;
		if (g_selectSpellUI < 0)
		{
			g_selectSpellUI = GAMEUI_TYPE_QUIT;
		}
	}
	else if (GetKeyboardRepeat(DIK_S) == true || GetJoypadRepeat(JOYKEY_DOWN, 0) == true || GetJoypadStickRepeatL(JOYSTICK_DOWN, 0) == true)	// 下
	{
		g_selectSpellUI++;
		if (g_selectSpellUI > GAMEUI_TYPE_QUIT)
		{
			g_selectSpellUI = GAMEUI_TYPE_CLOCK;
		}
	}

	// 何が選ばれているかの判定
	switch (g_selectSpellUI)
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
	g_pVtxBuffSpellUI->Lock(0, 0, (void**)&pVtx, 0);
	for (int nCntSpellUI = 0; nCntSpellUI < NUM_SELECT; nCntSpellUI++)
	{
		if (nCntSpellUI == g_pauseMenu)	// 選択部だけ明るく表示
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
	g_pVtxBuffSpellUI->Unlock();

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
#endif
}

//======================================================================================
// ポーズの描画処理
//======================================================================================
void DrawSpellUI(void)
{
	LPDIRECT3DDEVICE9 pDevice;	// デバイスへのポインタ
	// デバイスの取得
	pDevice = GetDevice();

	// 頂点バッファをデータストリームに設定
	pDevice->SetStreamSource(0, g_pVtxBuffSpellUI, 0, sizeof(VERTEX_2D));

	// 頂点フォーマットの設定
	pDevice->SetFVF(FVF_VERTEX_2D);

	for (int nCntPlayer = 0; nCntPlayer < MAX_PLAYER; nCntPlayer++)
	{
		if (g_bAllDisp[nCntPlayer] == false)
		{
			continue;
		}

		for (int nCntUI = 0; nCntUI < MAX_SPELLTYPE; nCntUI++)
		{
			// テクスチャの設定
			pDevice->SetTexture(0, g_apTextureSpellUI[g_SpellUI[nCntPlayer][nCntUI].tex]);

			// ポリゴンの描画
			pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, nCntUI * 4 + nCntPlayer * MAX_SPELLTYPE, 2);
		}
	}
}