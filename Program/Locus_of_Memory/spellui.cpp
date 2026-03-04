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
#include "camera.h"
#include "color.h"

// マクロ定義
#define MAX_COMMAND			(3)					// 受け付けるコマンドの最大数
#define NUM_KEY				(30)				// 処理を行うキー数
#define DISP_MAGIC			(30)				// 魔法を表示している時間
#define COMMAND_SIZE		(8.0f)				// コマンドのサイズ
#define COMMAND_X			(15.0f)				// コマンドのX軸
#define COMMAND_Y			(55.0f)				// コマンドの高度
#define MAXSPELL_TEX		(SPELLUI_TEX_MAX)	// テクスチャの最大数
#define MAXSPELL_TYPE		(SPELLUI_TYPE_MAX)	// 配置の最大数
#define MAGIC_SIZE			(6.0f)				// 魔法の大きさ
#define MAGICY_Y			(40.0f)			// 黄魔法の高度
#define MAGICG_Y			(20.0f)			// 緑魔法の高度
#define MAGICRB_Y			(30.0f)			// 赤青魔法の高度
#define SPELLUI_Z			(-40.0f)		// SPELLUIの表示位置
#define NORMAL				(D3DXVECTOR3(0.0f, 1.0f, 0.0f))	// 法線ベクトル

// SPELLUIの浮遊感を演出
typedef enum
{ 
	SPELLUI_MOVE_STOP = 0,	// 静止
	SPELLUI_MOVE_APPEAR,	// 出現
	SPELLUI_MOVE_DISAPPEAR,	// 収縮
	SPELLUI_MOVE_NONDISP,	// 非表示
	SPELLUI_MOVE_UP,		// 上昇
	SPELLUI_MOVE_DOWN,		// 下降
	SPELLUI_MOVE_MAX
}SPELLUI_MOVE;

// SPELLUIの構造体
typedef struct
{
	D3DXMATRIX		mtxWorld;	// ワールドマトリックス
	D3DXVECTOR3		pos;		// 位置
	SPELLUI_TEX		tex;		// 使用されるテクスチャの種類を管理
	SPELLUI_TYPE	type;		// 表示されるUIの場所を管理
	SPELLUI_MOVE	move;		// 浮遊感
	float	fWidth;			// 幅
	float	fWidthDest;		// 幅の目的値
	float	fHeight;		// 高さ
	float	fHeightDest;	// 高さの目的値
	int		nNumKey;		// 浮遊感をカウントするキー数
	int		nKey;			// 現在のキー数
	bool	bDisp;			// 表示状態
}SPELLUI;

// SPELLUIの全体管理
typedef struct
{
	SPELLUI aSpellUI[MAXSPELL_TYPE];	// UIの種類ごとの表示管理
	D3DXVECTOR3		pos;		// 中心位置
	D3DXVECTOR3		rot;		// 向き
	SPELLUISTATE	state;		// 出現状態
	SPELLUI_TEX		magicType;	// 魔法の種類
	int		nNumKey;			// 移動にかかるフレーム数
	int		nKey;				// 現在のフレーム数
	int		nCounterUI;			// magicの種類を表示する時間
	bool	bSpell;				// 全体の表示管理
}SpellUI;

// グローバル変数
LPDIRECT3DTEXTURE9	g_apTextureSpellUI[MAXSPELL_TEX] = {};	// テクスチャへのポインタ
LPDIRECT3DVERTEXBUFFER9	g_pVtxBuffSpellUI = NULL;			// 頂点バッファへのポインタ
SpellUI g_aSpellUI[MAX_PLAYER];		// SPELLUIの全体管理

// テクスチャの読み込み
const char* c_apFilenameSpellUI[MAXSPELL_TEX] =
{
	"data\\TEXTURE\\SpellUI\\00_MagicNull.png",
	"data\\TEXTURE\\SpellUI\\01_Red.png",
	"data\\TEXTURE\\SpellUI\\02_Green.png",
	"data\\TEXTURE\\SpellUI\\03_Blue.png",
	"data\\TEXTURE\\SpellUI\\04_Yellow.png",
	"data\\TEXTURE\\SpellUI\\05_K.png",
	"data\\TEXTURE\\SpellUI\\06_L.png",
	"data\\TEXTURE\\SpellUI\\07_J.png",
	"data\\TEXTURE\\SpellUI\\08_I.png",
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
};

//======================================================================================
// spellの初期化処理
//======================================================================================
void InitSpellUI(void)
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
		if (g_apTextureSpellUI[nCntUI] == NULL)
		{
			D3DXCreateTextureFromFile(pDevice, c_apFilenameSpellUI[nCntUI], &g_apTextureSpellUI[nCntUI]);
		}
	}

	// 初期化
	for (int nCntPlayer = 0; nCntPlayer < MAX_PLAYER; nCntPlayer++)
	{
		for (int nCntUI = 0; nCntUI < MAXSPELL_TYPE; nCntUI++)
		{
			g_aSpellUI[nCntPlayer].aSpellUI[nCntUI].pos		= D3DXVECTOR3(0.0f, 0.0f, 0.0f);	// 中心位置
			g_aSpellUI[nCntPlayer].aSpellUI[nCntUI].tex		= SPELLUI_TEX_MAGICNULL;			// テクスチャの種類
			g_aSpellUI[nCntPlayer].aSpellUI[nCntUI].type	= SPELLUI_TYPE_COMMAND0;			// テクスチャの配置
			g_aSpellUI[nCntPlayer].aSpellUI[nCntUI].move	= SPELLUI_MOVE_NONDISP;				// UIの浮遊感
			g_aSpellUI[nCntPlayer].aSpellUI[nCntUI].fWidth		= 0.0f;		// 幅
			g_aSpellUI[nCntPlayer].aSpellUI[nCntUI].fHeight		= 0.0f;		// 高さ
			g_aSpellUI[nCntPlayer].aSpellUI[nCntUI].fWidthDest	= 0.0f;		// 幅の目的地
			g_aSpellUI[nCntPlayer].aSpellUI[nCntUI].fHeightDest = 0.0f;		// 高さの目的地
			g_aSpellUI[nCntPlayer].aSpellUI[nCntUI].nKey		= 0;		// 浮遊感を演出
			g_aSpellUI[nCntPlayer].aSpellUI[nCntUI].nNumKey		= NUM_KEY;	// 浮遊感を演出
			g_aSpellUI[nCntPlayer].aSpellUI[nCntUI].bDisp		= false;		// 表示状態
		}
		g_aSpellUI[nCntPlayer].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);	// 中心位置
		g_aSpellUI[nCntPlayer].rot = D3DXVECTOR3(0.0f, 3.14f, 0.0f);	// 中心位置
		g_aSpellUI[nCntPlayer].magicType = SPELLUI_TEX_NONE;		// 発動中の魔法を格納
		g_aSpellUI[nCntPlayer].state = SPELLUISTATE_NONDISPLAY;	// UIの表示状態
		g_aSpellUI[nCntPlayer].nNumKey		= NUM_KEY;			// 処理を行うキー数
		g_aSpellUI[nCntPlayer].nKey			= 0;				// 現在のキー
		g_aSpellUI[nCntPlayer].nCounterUI	= DISP_MAGIC;		// 魔法の表示時間
		g_aSpellUI[nCntPlayer].bSpell		= false;			// UI表示状態(trueでポーズ中)
	}

	// 頂点バッファの生成
	pDevice->CreateVertexBuffer(sizeof(VERTEX_3D) * 4 * MAXSPELL_TYPE * MAX_PLAYER, D3DUSAGE_WRITEONLY, FVF_VERTEX_3D, D3DPOOL_MANAGED, &g_pVtxBuffSpellUI, NULL);

	VERTEX_3D* pVtx;
	// 頂点バッファをロックし、頂点情報へのポインタを取得
	g_pVtxBuffSpellUI->Lock(0, 0, (void**)&pVtx, 0);

	for (int nCntPlayer = 0; nCntPlayer < MAX_PLAYER; nCntPlayer++)
	{
		for (int nCntUI = 0; nCntUI < MAXSPELL_TYPE; nCntUI++, pVtx += 4)
		{
			// 各種情報の設定
			switch (nCntUI)
			{
			case SPELLUI_TYPE_COMMAND0:	// 1つ目のコマンド
				g_aSpellUI[nCntPlayer].aSpellUI[nCntUI].pos		= D3DXVECTOR3(-COMMAND_X, COMMAND_Y, 0.0f);	// 中心位置
				g_aSpellUI[nCntPlayer].aSpellUI[nCntUI].tex		= SPELLUI_TEX_MAGICNULL;	// テクスチャの種類
				g_aSpellUI[nCntPlayer].aSpellUI[nCntUI].type	= SPELLUI_TYPE_COMMAND0;	// テクスチャの配置
				g_aSpellUI[nCntPlayer].aSpellUI[nCntUI].move	= SPELLUI_MOVE_NONDISP;		// UIの浮遊感
				g_aSpellUI[nCntPlayer].aSpellUI[nCntUI].fWidth	= COMMAND_SIZE;				// 幅
				g_aSpellUI[nCntPlayer].aSpellUI[nCntUI].fHeight = COMMAND_SIZE;				// 高さ
				g_aSpellUI[nCntPlayer].aSpellUI[nCntUI].fWidthDest	= 0.0f;					// 幅の目的地
				g_aSpellUI[nCntPlayer].aSpellUI[nCntUI].fHeightDest = 0.0f;					// 高さの目的地
				g_aSpellUI[nCntPlayer].aSpellUI[nCntUI].nKey	= 0;						// 浮遊感を演出
				g_aSpellUI[nCntPlayer].aSpellUI[nCntUI].nNumKey = NUM_KEY;					// 浮遊感を演出
				g_aSpellUI[nCntPlayer].aSpellUI[nCntUI].bDisp	= false;					// 表示状態
				break;

			case SPELLUI_TYPE_COMMAND1:	// 2つ目のコマンド
				g_aSpellUI[nCntPlayer].aSpellUI[nCntUI].pos		= D3DXVECTOR3(0.0f, COMMAND_Y, 0.0f);	// 中心位置
				g_aSpellUI[nCntPlayer].aSpellUI[nCntUI].tex		= SPELLUI_TEX_MAGICNULL;	// テクスチャの種類
				g_aSpellUI[nCntPlayer].aSpellUI[nCntUI].type	= SPELLUI_TYPE_COMMAND1;	// テクスチャの配置
				g_aSpellUI[nCntPlayer].aSpellUI[nCntUI].move	= SPELLUI_MOVE_NONDISP;		// UIの浮遊感
				g_aSpellUI[nCntPlayer].aSpellUI[nCntUI].fWidth	= COMMAND_SIZE;				// 幅
				g_aSpellUI[nCntPlayer].aSpellUI[nCntUI].fHeight = COMMAND_SIZE;				// 高さ
				g_aSpellUI[nCntPlayer].aSpellUI[nCntUI].fWidthDest	= 0.0f;					// 幅の目的地
				g_aSpellUI[nCntPlayer].aSpellUI[nCntUI].fHeightDest = 0.0f;					// 高さの目的地
				g_aSpellUI[nCntPlayer].aSpellUI[nCntUI].nKey	= 0;						// 浮遊感を演出
				g_aSpellUI[nCntPlayer].aSpellUI[nCntUI].nNumKey = NUM_KEY;					// 浮遊感を演出
				g_aSpellUI[nCntPlayer].aSpellUI[nCntUI].bDisp	= false;					// 表示状態
				break;

			case SPELLUI_TYPE_COMMAND2:	// 3つ目のコマンド
				g_aSpellUI[nCntPlayer].aSpellUI[nCntUI].pos		= D3DXVECTOR3(COMMAND_X, COMMAND_Y, 0.0f);	// 中心位置
				g_aSpellUI[nCntPlayer].aSpellUI[nCntUI].tex		= SPELLUI_TEX_MAGICNULL;	// テクスチャの種類
				g_aSpellUI[nCntPlayer].aSpellUI[nCntUI].type	= SPELLUI_TYPE_COMMAND2;	// テクスチャの配置
				g_aSpellUI[nCntPlayer].aSpellUI[nCntUI].move	= SPELLUI_MOVE_NONDISP;		// UIの浮遊感
				g_aSpellUI[nCntPlayer].aSpellUI[nCntUI].fWidth	= COMMAND_SIZE;				// 幅
				g_aSpellUI[nCntPlayer].aSpellUI[nCntUI].fHeight = COMMAND_SIZE;				// 高さ
				g_aSpellUI[nCntPlayer].aSpellUI[nCntUI].fWidthDest	= 0.0f;					// 幅の目的地
				g_aSpellUI[nCntPlayer].aSpellUI[nCntUI].fHeightDest = 0.0f;					// 高さの目的地
				g_aSpellUI[nCntPlayer].aSpellUI[nCntUI].nKey = 0;							// 浮遊感を演出
				g_aSpellUI[nCntPlayer].aSpellUI[nCntUI].nNumKey = NUM_KEY;					// 浮遊感を演出
				g_aSpellUI[nCntPlayer].aSpellUI[nCntUI].bDisp	= false;					// 表示状態
				break;

			case SPELLUI_TYPE_RED:	// 赤魔法
				g_aSpellUI[nCntPlayer].aSpellUI[nCntUI].pos = D3DXVECTOR3(COMMAND_X, MAGICRB_Y, 0.0f);	// 中心位置
				// テクスチャの種類
				if (operationType == OPERATIONTYPE_KEYBOARD)
				{ // キーボード操作
					g_aSpellUI[nCntPlayer].aSpellUI[nCntUI].tex = SPELLUI_TEX_L;
				}
				else
				{ // ジョイパッド操作
					g_aSpellUI[nCntPlayer].aSpellUI[nCntUI].tex = SPELLUI_TEX_B;
				}
				g_aSpellUI[nCntPlayer].aSpellUI[nCntUI].type	= SPELLUI_TYPE_RED;	// テクスチャの配置
				g_aSpellUI[nCntPlayer].aSpellUI[nCntUI].move	= SPELLUI_MOVE_STOP;		// UIの浮遊感
				g_aSpellUI[nCntPlayer].aSpellUI[nCntUI].fWidth	= MAGIC_SIZE;				// 幅
				g_aSpellUI[nCntPlayer].aSpellUI[nCntUI].fHeight = MAGIC_SIZE;				// 高さ
				g_aSpellUI[nCntPlayer].aSpellUI[nCntUI].fWidthDest = 0.0f;					// 幅の目的地
				g_aSpellUI[nCntPlayer].aSpellUI[nCntUI].fHeightDest = 0.0f;					// 高さの目的地
				g_aSpellUI[nCntPlayer].aSpellUI[nCntUI].nKey = 0;							// 浮遊感を演出
				g_aSpellUI[nCntPlayer].aSpellUI[nCntUI].nNumKey = NUM_KEY;					// 浮遊感を演出
				g_aSpellUI[nCntPlayer].aSpellUI[nCntUI].bDisp = true;						// 表示状態
				break;

			case SPELLUI_TYPE_GREEN:	// 緑魔法
				g_aSpellUI[nCntPlayer].aSpellUI[nCntUI].pos = D3DXVECTOR3(0.0f, MAGICG_Y, 0.0f);	// 中心位置
				// テクスチャの種類
				if (operationType == OPERATIONTYPE_KEYBOARD)
				{ // キーボード操作
					g_aSpellUI[nCntPlayer].aSpellUI[nCntUI].tex = SPELLUI_TEX_K;
				}
				else
				{ // ジョイパッド操作
					g_aSpellUI[nCntPlayer].aSpellUI[nCntUI].tex = SPELLUI_TEX_A;
				}
				g_aSpellUI[nCntPlayer].aSpellUI[nCntUI].type	= SPELLUI_TYPE_GREEN;	// テクスチャの配置
				g_aSpellUI[nCntPlayer].aSpellUI[nCntUI].move	= SPELLUI_MOVE_STOP;		// UIの浮遊感
				g_aSpellUI[nCntPlayer].aSpellUI[nCntUI].fWidth	= MAGIC_SIZE;				// 幅
				g_aSpellUI[nCntPlayer].aSpellUI[nCntUI].fHeight = MAGIC_SIZE;				// 高さ
				g_aSpellUI[nCntPlayer].aSpellUI[nCntUI].fWidthDest	= 0.0f;					// 幅の目的地
				g_aSpellUI[nCntPlayer].aSpellUI[nCntUI].fHeightDest = 0.0f;					// 高さの目的地
				g_aSpellUI[nCntPlayer].aSpellUI[nCntUI].nKey = 0;							// 浮遊感を演出
				g_aSpellUI[nCntPlayer].aSpellUI[nCntUI].nNumKey = NUM_KEY;					// 浮遊感を演出
				g_aSpellUI[nCntPlayer].aSpellUI[nCntUI].bDisp	= true;						// 表示状態
				break;

			case SPELLUI_TYPE_BLUE:	// 青魔法
				g_aSpellUI[nCntPlayer].aSpellUI[nCntUI].pos = D3DXVECTOR3(-COMMAND_X, MAGICRB_Y, 0.0f);	// 中心位置
				// テクスチャの種類
				if (operationType == OPERATIONTYPE_KEYBOARD)
				{ // キーボード操作
					g_aSpellUI[nCntPlayer].aSpellUI[nCntUI].tex = SPELLUI_TEX_J;
				}
				else
				{ // ジョイパッド操作
					g_aSpellUI[nCntPlayer].aSpellUI[nCntUI].tex = SPELLUI_TEX_X;
				}
				g_aSpellUI[nCntPlayer].aSpellUI[nCntUI].type = SPELLUI_TYPE_BLUE;	// テクスチャの配置
				g_aSpellUI[nCntPlayer].aSpellUI[nCntUI].move = SPELLUI_MOVE_STOP;	// UIの浮遊感
				g_aSpellUI[nCntPlayer].aSpellUI[nCntUI].fWidth	= MAGIC_SIZE;				// 幅
				g_aSpellUI[nCntPlayer].aSpellUI[nCntUI].fHeight = MAGIC_SIZE;				// 高さ
				g_aSpellUI[nCntPlayer].aSpellUI[nCntUI].fWidthDest	= 0.0f;					// 幅の目的地
				g_aSpellUI[nCntPlayer].aSpellUI[nCntUI].fHeightDest = 0.0f;					// 高さの目的地
				g_aSpellUI[nCntPlayer].aSpellUI[nCntUI].nKey = 0;							// 浮遊感を演出
				g_aSpellUI[nCntPlayer].aSpellUI[nCntUI].nNumKey = NUM_KEY;					// 浮遊感を演出
				g_aSpellUI[nCntPlayer].aSpellUI[nCntUI].bDisp	= true;						// 表示状態
				break;

			case SPELLUI_TYPE_YELLOW:	// 黄魔法
				g_aSpellUI[nCntPlayer].aSpellUI[nCntUI].pos = D3DXVECTOR3(0.0f, MAGICY_Y, 0.0f);	// 中心位置
				// テクスチャの種類
				if (operationType == OPERATIONTYPE_KEYBOARD)
				{ // キーボード操作
					g_aSpellUI[nCntPlayer].aSpellUI[nCntUI].tex = SPELLUI_TEX_I;
				}
				else
				{ // ジョイパッド操作
					g_aSpellUI[nCntPlayer].aSpellUI[nCntUI].tex = SPELLUI_TEX_Y;
				}
				g_aSpellUI[nCntPlayer].aSpellUI[nCntUI].type = SPELLUI_TYPE_YELLOW;	// テクスチャの配置
				g_aSpellUI[nCntPlayer].aSpellUI[nCntUI].move = SPELLUI_MOVE_STOP;	// UIの浮遊感
				g_aSpellUI[nCntPlayer].aSpellUI[nCntUI].fWidth = MAGIC_SIZE;		// 幅
				g_aSpellUI[nCntPlayer].aSpellUI[nCntUI].fHeight = MAGIC_SIZE;		// 高さ
				g_aSpellUI[nCntPlayer].aSpellUI[nCntUI].fWidthDest = 0.0f;			// 幅の目的地
				g_aSpellUI[nCntPlayer].aSpellUI[nCntUI].fHeightDest = 0.0f;			// 高さの目的地
				g_aSpellUI[nCntPlayer].aSpellUI[nCntUI].nKey = 0;					// 浮遊感を演出
				g_aSpellUI[nCntPlayer].aSpellUI[nCntUI].nNumKey = NUM_KEY;			// 浮遊感を演出
				g_aSpellUI[nCntPlayer].aSpellUI[nCntUI].bDisp = true;				// 表示状態
				break;
			}

			// 中心位置からの位置を求める
			g_aSpellUI[nCntPlayer].aSpellUI[nCntUI].pos += g_aSpellUI[nCntPlayer].pos;

			// 頂点座標の設定
			pVtx[0].pos = D3DXVECTOR3(-g_aSpellUI[nCntPlayer].aSpellUI[nCntUI].fWidth, g_aSpellUI[nCntPlayer].aSpellUI[nCntUI].fHeight, SPELLUI_Z);
			pVtx[1].pos = D3DXVECTOR3(g_aSpellUI[nCntPlayer].aSpellUI[nCntUI].fWidth,  g_aSpellUI[nCntPlayer].aSpellUI[nCntUI].fHeight, SPELLUI_Z);
			pVtx[2].pos = D3DXVECTOR3(-g_aSpellUI[nCntPlayer].aSpellUI[nCntUI].fWidth, -g_aSpellUI[nCntPlayer].aSpellUI[nCntUI].fHeight, SPELLUI_Z);
			pVtx[3].pos = D3DXVECTOR3(g_aSpellUI[nCntPlayer].aSpellUI[nCntUI].fWidth,  -g_aSpellUI[nCntPlayer].aSpellUI[nCntUI].fHeight, SPELLUI_Z);

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

		g_aSpellUI[0].bSpell = true;
		g_aSpellUI[1].bSpell = true;
	}
	// 頂点バッファをアンロック
	g_pVtxBuffSpellUI->Unlock();

	SetSpellUINonDisplay(0);
	SetSpellUINonDisplay(1);
}

//======================================================================================
// spellの終了処理
//======================================================================================
void UninitSpellUI(void)
{
	// テクスチャの破棄
	for (int nCntUI = 0; nCntUI < MAXSPELL_TEX; nCntUI++)
	{
		if (g_apTextureSpellUI[nCntUI] != NULL)
		{
			g_apTextureSpellUI[nCntUI]->Release();
			g_apTextureSpellUI[nCntUI] = NULL;
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
	VERTEX_3D* pVtx;
	// 頂点バッファをロックし、頂点情報へのポインタを取得
	g_pVtxBuffSpellUI->Lock(0, 0, (void**)&pVtx, 0);

	bool bSpell = false;	// ポーズ状態との兼ね合いを管理

	Player* pPlayer = GetPlayer();
	for (int nCntPlayer = 0; nCntPlayer < MAX_PLAYER; nCntPlayer++, pPlayer++)
	{
		//// ポーズ状態になったら非表示にする
		//if (((GetKeyboardTrigger(DIK_P) == true && nCntPlayer == 0) || GetJoypadTrigger(JOYKEY_START, nCntPlayer) == true) && g_aSpellUI[nCntPlayer].bSpell == true)
		//{
		//	SetSpellUIDisappear(nCntPlayer);
		//	bSpell = true;
		//}
		//else if (g_aSpellUI[nCntPlayer].bSpell == false)
		//{
		//	SetSpellUINonDisplay(nCntPlayer);
		//}
		//
		//// SPELLメニューを開いているかのフラグを立てる
		//if (((GetKeyboardPress(DIK_TAB) == true && nCntPlayer == 0) || GetJoypadRightTriggePress(nCntPlayer) == true || GetJoypadLeftTriggePress(nCntPlayer) == true))
		//{
		//	if (bSpell == false && ((GetKeyboardTrigger(DIK_TAB) == true && nCntPlayer == 0) || GetJoypadTrigger(JOYKEY_LEFT_TRIGGER, nCntPlayer) == true || GetJoypadTrigger(JOYKEY_RIGHT_TRIGGER, nCntPlayer) == true)
		//		&& (g_aSpellUI[nCntPlayer].state == SPELLUISTATE_NONDISPLAY || g_aSpellUI[nCntPlayer].state == SPELLUISTATE_DISAPPEAR)
		//		&& pPlayer->bJump == false)
		//	{
		//		g_aSpellUI[nCntPlayer].state = SPELLUISTATE_APPEAR;
		//		SetSpellUINonDisplay(nCntPlayer);
		//		SetSpellUIAppear(nCntPlayer);
		//		g_aSpellUI[nCntPlayer].bSpell = true;
		//		SetMotion(&pPlayer->motion, pPlayer->pModelData, &pPlayer->OffSetData, (MOTIONTYPE)PLAYERMOTIONTYPE_COMMAND, true, true, BLENDFRAME);
		//	}
		//	pPlayer->state = PLAYERSTATE_SPELL;
		//}
		//else
		//{
		//	if (bSpell == false && ((GetKeyboardRelease(DIK_TAB) == true && nCntPlayer == 0) || GetJoypadRelease(JOYKEY_LEFT_TRIGGER, nCntPlayer) == true || GetJoypadRelease(JOYKEY_RIGHT_TRIGGER, nCntPlayer) == true)
		//		&& (g_aSpellUI[nCntPlayer].state == SPELLUISTATE_DISPLAY || g_aSpellUI[nCntPlayer].state == SPELLUISTATE_APPEAR || g_aSpellUI[nCntPlayer].state == SPELLUISTATE_SETMAGIC)
		//		&& pPlayer->bJump == false)
		//	{
		//		g_aSpellUI[nCntPlayer].state = SPELLUISTATE_DISAPPEAR;
		//		SetSpellUIDisplay(nCntPlayer);
		//		SetSpellUIDisappear(nCntPlayer);
		//		ResetCommand(nCntPlayer);
		//		pPlayer->state = PLAYERSTATE_NORMAL;
		//		SetMotion(&pPlayer->motion, pPlayer->pModelData, &pPlayer->OffSetData, (MOTIONTYPE)PLAYERMOTIONTYPE_NEUTRAL, true, true, BLENDFRAME);
		//	}
		//}

		if (g_aSpellUI[nCntPlayer].bSpell == false)
		{
			continue;
		}

		if (g_aSpellUI[nCntPlayer].aSpellUI[SPELLUI_TYPE_COMMAND0].bDisp == false &&
			g_aSpellUI[nCntPlayer].aSpellUI[SPELLUI_TYPE_COMMAND1].bDisp == false &&
			g_aSpellUI[nCntPlayer].aSpellUI[SPELLUI_TYPE_COMMAND2].bDisp == false)
		{ // コマンド入力が行われていない場合、セーブ内容を初期化
			ResetCommdSave(nCntPlayer);
		}

		// コマンドの入力情報を取得
		COMMANDTYPE* commandType = GetCommandSaveType(nCntPlayer);
		COMMANDTYPE CommandUI[MAX_COMMAND];
		for (int nCntUI = 0; nCntUI < MAX_COMMAND; nCntUI++)
		{
			CommandUI[nCntUI] = commandType[nCntUI];
			switch (CommandUI[nCntUI])
			{
			case COMMANDTYPE_NONE:	// 入力なし
				g_aSpellUI[nCntPlayer].aSpellUI[nCntUI].tex = SPELLUI_TEX_MAGICNULL;
				break;
			case COMMANDTYPE_R:	// 赤魔法
				g_aSpellUI[nCntPlayer].aSpellUI[nCntUI].tex = SPELLUI_TEX_RED;
				break;
			case COMMANDTYPE_G:	// 緑魔法
				g_aSpellUI[nCntPlayer].aSpellUI[nCntUI].tex = SPELLUI_TEX_GREEN;
				break;
			case COMMANDTYPE_B:	// 青魔法
				g_aSpellUI[nCntPlayer].aSpellUI[nCntUI].tex = SPELLUI_TEX_BLUE;
				break;
			case COMMANDTYPE_Y:	// 黄魔法
				g_aSpellUI[nCntPlayer].aSpellUI[nCntUI].tex = SPELLUI_TEX_YELLOW;
				break;
			}
		}

		// 全体の演出処理======================================================================================
		float fDiffKey = 0.0f;	// キーの差分を計算
		float fRateKey = (float)g_aSpellUI[nCntPlayer].nKey / (float)g_aSpellUI[nCntPlayer].nNumKey;
		switch (g_aSpellUI[nCntPlayer].state)
		{
		case SPELLUISTATE_NONDISPLAY:	// 非表示状態
			g_aSpellUI[nCntPlayer].bSpell = false;
			break;

		case SPELLUISTATE_APPEAR:	// 出現状態
			for (int nCntUI = 0; nCntUI < MAXSPELL_TYPE; nCntUI++)
			{
				// 背景の高度変更
				fDiffKey = g_aSpellUI[nCntPlayer].aSpellUI[nCntUI].fHeightDest - g_aSpellUI[nCntPlayer].aSpellUI[nCntUI].fHeight;
				g_aSpellUI[nCntPlayer].aSpellUI[nCntUI].fHeight = g_aSpellUI[nCntPlayer].aSpellUI[nCntUI].fHeight + fDiffKey * fRateKey;
				// 中心位置からの位置を求める
				g_aSpellUI[nCntPlayer].aSpellUI[nCntUI].pos += g_aSpellUI[nCntPlayer].pos;
			}
			g_aSpellUI[nCntPlayer].nKey++;

			if (g_aSpellUI[nCntPlayer].nKey > g_aSpellUI[nCntPlayer].nNumKey)
			{
				g_aSpellUI[nCntPlayer].state = SPELLUISTATE_DISPLAY;
				SetSpellUIDisplay(nCntPlayer);
			}
			break;

		case SPELLUISTATE_DISPLAY:	// 表示状態

			break;

		case SPELLUISTATE_SETMAGIC:	// 魔法発動状態
			g_aSpellUI[nCntPlayer].nCounterUI--;
			if (g_aSpellUI[nCntPlayer].nCounterUI < 0)
			{
				g_aSpellUI[nCntPlayer].nCounterUI = DISP_MAGIC;
				SetCommandDisappear(nCntPlayer, SPELLUI_TYPE_COMMAND0);
				SetCommandDisappear(nCntPlayer, SPELLUI_TYPE_COMMAND1);
				SetCommandDisappear(nCntPlayer, SPELLUI_TYPE_COMMAND2);
				//ResetCommdSave(nCntPlayer);
				g_aSpellUI[nCntPlayer].state = SPELLUISTATE_DISPLAY;
			}
			break;

		case SPELLUISTATE_DISAPPEAR:	// 収縮状態
			for (int nCntUI = 0; nCntUI < MAXSPELL_TYPE; nCntUI++)
			{
				// 背景の高度変更
				fDiffKey = g_aSpellUI[nCntPlayer].aSpellUI[nCntUI].fHeightDest - g_aSpellUI[nCntPlayer].aSpellUI[nCntUI].fHeight;
				g_aSpellUI[nCntPlayer].aSpellUI[nCntUI].fHeight = g_aSpellUI[nCntPlayer].aSpellUI[nCntUI].fHeight + fDiffKey * fRateKey;
				// 中心位置からの位置を求める
				g_aSpellUI[nCntPlayer].aSpellUI[nCntUI].pos += g_aSpellUI[nCntPlayer].pos;
			}
			g_aSpellUI[nCntPlayer].nKey++;

			if (g_aSpellUI[nCntPlayer].nKey > g_aSpellUI[nCntPlayer].nNumKey)
			{
				g_aSpellUI[nCntPlayer].state = SPELLUISTATE_DISPLAY;
				SetSpellUINonDisplay(nCntPlayer);
				ResetCommdSave(nCntPlayer);
			}
			break;
		}

		// 個々の演出処理======================================================================================
		for (int nCntUI = 0; nCntUI < MAXSPELL_TYPE; nCntUI++, pVtx += 4)
		{
			switch (g_aSpellUI[nCntPlayer].aSpellUI[nCntUI].move)
			{
			case SPELLUI_MOVE_STOP:	// 停止
				g_aSpellUI[nCntPlayer].aSpellUI[nCntUI].bDisp = true;
				break;

			case SPELLUI_MOVE_APPEAR:	// 出現
				g_aSpellUI[nCntPlayer].aSpellUI[nCntUI].bDisp = true;

				fRateKey = (float)g_aSpellUI[nCntPlayer].aSpellUI[nCntUI].nKey / (float)g_aSpellUI[nCntPlayer].aSpellUI[nCntUI].nNumKey;
				// 背景の高度変更
				fDiffKey = g_aSpellUI[nCntPlayer].aSpellUI[nCntUI].fHeightDest - g_aSpellUI[nCntPlayer].aSpellUI[nCntUI].fHeight;
				g_aSpellUI[nCntPlayer].aSpellUI[nCntUI].fHeight = g_aSpellUI[nCntPlayer].aSpellUI[nCntUI].fHeight + fDiffKey * fRateKey;
				if (g_aSpellUI[nCntPlayer].aSpellUI[nCntUI].nKey > g_aSpellUI[nCntPlayer].aSpellUI[nCntUI].nNumKey)
				{
					g_aSpellUI[nCntPlayer].aSpellUI[nCntUI].move = SPELLUI_MOVE_STOP;
				}
				g_aSpellUI[nCntPlayer].aSpellUI[nCntUI].nKey++;
				break;

			case SPELLUI_MOVE_DISAPPEAR:	// 収縮
				fRateKey = (float)g_aSpellUI[nCntPlayer].aSpellUI[nCntUI].nKey / (float)g_aSpellUI[nCntPlayer].aSpellUI[nCntUI].nNumKey;
				// 背景の高度変更
				fDiffKey = g_aSpellUI[nCntPlayer].aSpellUI[nCntUI].fHeightDest - g_aSpellUI[nCntPlayer].aSpellUI[nCntUI].fHeight;
				g_aSpellUI[nCntPlayer].aSpellUI[nCntUI].fHeight = g_aSpellUI[nCntPlayer].aSpellUI[nCntUI].fHeight + fDiffKey * fRateKey;
				if (g_aSpellUI[nCntPlayer].aSpellUI[nCntUI].nKey > g_aSpellUI[nCntPlayer].aSpellUI[nCntUI].nNumKey)
				{
					g_aSpellUI[nCntPlayer].aSpellUI[nCntUI].move = SPELLUI_MOVE_NONDISP;
				}
				g_aSpellUI[nCntPlayer].aSpellUI[nCntUI].nKey++;
				break;

			case SPELLUI_MOVE_NONDISP:	// 非表示
				g_aSpellUI[nCntPlayer].aSpellUI[nCntUI].bDisp = false;
				break;
			}

			// 頂点座標の設定
			pVtx[0].pos = D3DXVECTOR3(-g_aSpellUI[nCntPlayer].aSpellUI[nCntUI].fWidth, g_aSpellUI[nCntPlayer].aSpellUI[nCntUI].fHeight, SPELLUI_Z);
			pVtx[1].pos = D3DXVECTOR3(g_aSpellUI[nCntPlayer].aSpellUI[nCntUI].fWidth, g_aSpellUI[nCntPlayer].aSpellUI[nCntUI].fHeight, SPELLUI_Z);
			pVtx[2].pos = D3DXVECTOR3(-g_aSpellUI[nCntPlayer].aSpellUI[nCntUI].fWidth, -g_aSpellUI[nCntPlayer].aSpellUI[nCntUI].fHeight, SPELLUI_Z);
			pVtx[3].pos = D3DXVECTOR3(g_aSpellUI[nCntPlayer].aSpellUI[nCntUI].fWidth, -g_aSpellUI[nCntPlayer].aSpellUI[nCntUI].fHeight, SPELLUI_Z);
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
		if (g_aSpellUI[nCntPlayer].bSpell == false)
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

			if (g_aSpellUI[nCntPlayer].aSpellUI[nCntUI].bDisp == false)
			{
				continue;
			}

			// ポリゴンのワールドマトリックスを初期化
			D3DXMatrixIdentity(&g_aSpellUI[nCntPlayer].aSpellUI[nCntUI].mtxWorld);

			// パーツの位置を反映
			D3DXMatrixTranslation(&mtxTransModel, g_aSpellUI[nCntPlayer].aSpellUI[nCntUI].pos.x, g_aSpellUI[nCntPlayer].aSpellUI[nCntUI].pos.y, g_aSpellUI[nCntPlayer].aSpellUI[nCntUI].pos.z);
			D3DXMatrixMultiply(&g_aSpellUI[nCntPlayer].aSpellUI[nCntUI].mtxWorld, &g_aSpellUI[nCntPlayer].aSpellUI[nCntUI].mtxWorld, &mtxTransModel);

			// 向きを反映
			D3DXMatrixRotationYawPitchRoll(&mtxRot, g_aSpellUI[nCntPlayer].rot.y, g_aSpellUI[nCntPlayer].rot.x, g_aSpellUI[nCntPlayer].rot.z);
			D3DXMatrixMultiply(&g_aSpellUI[nCntPlayer].aSpellUI[nCntUI].mtxWorld, &g_aSpellUI[nCntPlayer].aSpellUI[nCntUI].mtxWorld, &mtxRot);

			// 親マトリックスを設定
			mtxParent = UIMatrix;

			// 算出したパーツのワールドマトリックスと親モデルのマトリックスを掛け合わせる
			D3DXMatrixMultiply(&g_aSpellUI[nCntPlayer].aSpellUI[nCntUI].mtxWorld, &g_aSpellUI[nCntPlayer].aSpellUI[nCntUI].mtxWorld, &mtxParent);

			// パーツのワールドマトリックスを設定
			pDevice->SetTransform(D3DTS_WORLD, &g_aSpellUI[nCntPlayer].aSpellUI[nCntUI].mtxWorld);

			// 頂点バッファをデータストリームに設定
			pDevice->SetStreamSource(0, g_pVtxBuffSpellUI, 0, sizeof(VERTEX_3D));

			// 頂点フォーマットの設定
			pDevice->SetFVF(FVF_VERTEX_3D);

			// テクスチャの設定
			pDevice->SetTexture(0, g_apTextureSpellUI[g_aSpellUI[nCntPlayer].aSpellUI[nCntUI].tex]);

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
// UIを発動中の魔法に変更
//======================================================================================
void SetSpellUI(MAGICTYPE magicType, int nIdx, int nDispTime)
{
	switch (magicType)
	{
	case MAGICTYPE_NONE:	// 何もない場合
		g_aSpellUI[nIdx].magicType = SPELLUI_TEX_NONE;			// 詠唱失敗のテクスチャに切り替え
		break;

	case MAGICTYPE_LEVITATION:	// 浮遊
		g_aSpellUI[nIdx].magicType = SPELLUI_TEX_LEVITATION;	// 浮遊のテクスチャに切り替え
		break;

	case MAGICTYPE_COMBUSTION:	// 燃焼
		g_aSpellUI[nIdx].magicType = SPELLUI_TEX_COMBUSTION;	// 燃焼のテクスチャに切り替え
		break;

	case MAGICTYPE_FLOOD:	// 洪水、氾濫
		g_aSpellUI[nIdx].magicType = SPELLUI_TEX_FLOOD;	// 洪水、氾濫のテクスチャに切り替え
		break;

	case MAGICTYPE_FLASH:	// フラッシュ
		g_aSpellUI[nIdx].magicType = SPELLUI_TEX_FLASH;	// フラッシュのテクスチャに切り替え
		break;

	case MAGICTYPE_FIREBALL:	// 火球
		g_aSpellUI[nIdx].magicType = SPELLUI_TEX_FIREBALL;	// 火球のテクスチャに切り替え
		break;

	case MAGICTYPE_SUNSETDELAY:	// 太陽の動きを止める
		g_aSpellUI[nIdx].magicType = SPELLUI_TEX_SUNSETDELAY;	// 太陽の動きを止めるテクスチャに切り替え
		break;

	case MAGICTYPE_RAINPRAY:	// 雨乞い
		g_aSpellUI[nIdx].magicType = SPELLUI_TEX_RAINPRAY;	// 雨乞いのテクスチャに切り替え
		break;

	case MAGICTYPE_FREEZE:	// 凍結
		g_aSpellUI[nIdx].magicType = SPELLUI_TEX_FREEZE;	// 凍結のテクスチャに切り替え
		break;

	case MAGICTYPE_GROWTH:	// 成長
		g_aSpellUI[nIdx].magicType = SPELLUI_TEX_GROWTH;	// 成長のテクスチャに切り替え
		break;

	case MAGICTYPE_ACCELERATION:	// 加速
		g_aSpellUI[nIdx].magicType = SPELLUI_TEX_ACCELERATION;	// 加速のテクスチャに切り替え
		break;

	case MAGICTYPE_TIMEREVERT:	// 巻き戻し
		g_aSpellUI[nIdx].magicType = SPELLUI_TEX_TIMEREVERT;	// 巻き戻しのテクスチャに切り替え
		break;
	}
	g_aSpellUI[nIdx].nCounterUI = nDispTime;
}

//======================================================================================
// UIのステータスを変更
//======================================================================================
void SetSpellUIState(int nIdx, SPELLUISTATE state)
{
	g_aSpellUI[nIdx].state = state;
}

//======================================================================================
// SpellUIを出現状態にする
//======================================================================================
void SetSpellUIAppear(int nIdx)
{
	// 操作人数の取得
	OPERATIONTYPE operationType = GetOperationType();
	Player* pPlayer = GetPlayer();
	if (pPlayer->state != PLAYERSTATE_PAUSE)
	{
		for (int nCntUI = 0; nCntUI < MAXSPELL_TYPE; nCntUI++)
		{
			// 各種情報の設定
			switch (nCntUI)
			{
			case SPELLUI_TYPE_COMMAND0:	// 1つ目のコマンド
				g_aSpellUI[nIdx].aSpellUI[nCntUI].pos = D3DXVECTOR3(-COMMAND_X, COMMAND_Y, 0.0f);	// 中心位置
				g_aSpellUI[nIdx].aSpellUI[nCntUI].tex = SPELLUI_TEX_MAGICNULL;	// テクスチャの種類
				g_aSpellUI[nIdx].aSpellUI[nCntUI].type = SPELLUI_TYPE_COMMAND0;	// テクスチャの配置
				g_aSpellUI[nIdx].aSpellUI[nCntUI].move = SPELLUI_MOVE_NONDISP;		// UIの浮遊感
				g_aSpellUI[nIdx].aSpellUI[nCntUI].fWidth = COMMAND_SIZE;		// 幅
				g_aSpellUI[nIdx].aSpellUI[nCntUI].fWidthDest = COMMAND_SIZE;		// 幅の目的地
				g_aSpellUI[nIdx].aSpellUI[nCntUI].fHeightDest = COMMAND_SIZE;		// 高さの目的地
				g_aSpellUI[nIdx].aSpellUI[nCntUI].nKey = 0;				// 浮遊感を演出
				g_aSpellUI[nIdx].aSpellUI[nCntUI].nNumKey = NUM_KEY;			// 浮遊感を演出
				g_aSpellUI[nIdx].aSpellUI[nCntUI].bDisp = false;				// 表示状態
				break;

			case SPELLUI_TYPE_COMMAND1:	// 2つ目のコマンド
				g_aSpellUI[nIdx].aSpellUI[nCntUI].pos = D3DXVECTOR3(0.0f, COMMAND_Y, 0.0f);	// 中心位置
				g_aSpellUI[nIdx].aSpellUI[nCntUI].tex = SPELLUI_TEX_MAGICNULL;	// テクスチャの種類
				g_aSpellUI[nIdx].aSpellUI[nCntUI].type = SPELLUI_TYPE_COMMAND1;	// テクスチャの配置
				g_aSpellUI[nIdx].aSpellUI[nCntUI].move = SPELLUI_MOVE_NONDISP;		// UIの浮遊感
				g_aSpellUI[nIdx].aSpellUI[nCntUI].fWidth = COMMAND_SIZE;		// 幅
				g_aSpellUI[nIdx].aSpellUI[nCntUI].fWidthDest = COMMAND_SIZE;		// 幅の目的地
				g_aSpellUI[nIdx].aSpellUI[nCntUI].fHeightDest = COMMAND_SIZE;		// 高さの目的地
				g_aSpellUI[nIdx].aSpellUI[nCntUI].nKey = 0;				// 浮遊感を演出
				g_aSpellUI[nIdx].aSpellUI[nCntUI].nNumKey = NUM_KEY;			// 浮遊感を演出
				g_aSpellUI[nIdx].aSpellUI[nCntUI].bDisp = false;				// 表示状態
				break;

			case SPELLUI_TYPE_COMMAND2:	// 3つ目のコマンド
				g_aSpellUI[nIdx].aSpellUI[nCntUI].pos = D3DXVECTOR3(COMMAND_X, COMMAND_Y, 0.0f);	// 中心位置
				g_aSpellUI[nIdx].aSpellUI[nCntUI].tex = SPELLUI_TEX_MAGICNULL;	// テクスチャの種類
				g_aSpellUI[nIdx].aSpellUI[nCntUI].type = SPELLUI_TYPE_COMMAND2;	// テクスチャの配置
				g_aSpellUI[nIdx].aSpellUI[nCntUI].move = SPELLUI_MOVE_NONDISP;		// UIの浮遊感
				g_aSpellUI[nIdx].aSpellUI[nCntUI].fWidth = COMMAND_SIZE;		// 幅
				g_aSpellUI[nIdx].aSpellUI[nCntUI].fWidthDest = COMMAND_SIZE;		// 幅の目的地
				g_aSpellUI[nIdx].aSpellUI[nCntUI].fHeightDest = COMMAND_SIZE;		// 高さの目的地
				g_aSpellUI[nIdx].aSpellUI[nCntUI].nKey = 0;				// 浮遊感を演出
				g_aSpellUI[nIdx].aSpellUI[nCntUI].nNumKey = NUM_KEY;			// 浮遊感を演出
				g_aSpellUI[nIdx].aSpellUI[nCntUI].bDisp = false;				// 表示状態
				break;

			case SPELLUI_TYPE_RED:	// 赤魔法
				g_aSpellUI[nIdx].aSpellUI[nCntUI].pos = D3DXVECTOR3(COMMAND_X, MAGICRB_Y, 0.0f);	// 中心位置
				// テクスチャの種類
				if (operationType == OPERATIONTYPE_KEYBOARD)
				{ // キーボード操作
					g_aSpellUI[nIdx].aSpellUI[nCntUI].tex = SPELLUI_TEX_L;
				}
				else
				{ // ジョイパッド操作
					g_aSpellUI[nIdx].aSpellUI[nCntUI].tex = SPELLUI_TEX_B;
				}
				g_aSpellUI[nIdx].aSpellUI[nCntUI].type = SPELLUI_TYPE_RED;		// テクスチャの配置
				g_aSpellUI[nIdx].aSpellUI[nCntUI].move = SPELLUI_MOVE_STOP;	// UIの浮遊感
				g_aSpellUI[nIdx].aSpellUI[nCntUI].fWidth = MAGIC_SIZE;	// 幅
				g_aSpellUI[nIdx].aSpellUI[nCntUI].fWidthDest = MAGIC_SIZE;	// 幅の目的地
				g_aSpellUI[nIdx].aSpellUI[nCntUI].fHeightDest = MAGIC_SIZE;	// 高さの目的地
				g_aSpellUI[nIdx].aSpellUI[nCntUI].nKey = 0;			// 浮遊感を演出
				g_aSpellUI[nIdx].aSpellUI[nCntUI].nNumKey = NUM_KEY;		// 浮遊感を演出
				g_aSpellUI[nIdx].aSpellUI[nCntUI].bDisp = true;			// 表示状態
				break;

			case SPELLUI_TYPE_GREEN:	// 緑魔法
				g_aSpellUI[nIdx].aSpellUI[nCntUI].pos = D3DXVECTOR3(0.0f, MAGICG_Y, 0.0f);	// 中心位置
				// テクスチャの種類
				if (operationType == OPERATIONTYPE_KEYBOARD)
				{ // キーボード操作
					g_aSpellUI[nIdx].aSpellUI[nCntUI].tex = SPELLUI_TEX_K;
				}
				else
				{ // ジョイパッド操作
					g_aSpellUI[nIdx].aSpellUI[nCntUI].tex = SPELLUI_TEX_A;
				}
				g_aSpellUI[nIdx].aSpellUI[nCntUI].type = SPELLUI_TYPE_GREEN;	// テクスチャの配置
				g_aSpellUI[nIdx].aSpellUI[nCntUI].move = SPELLUI_MOVE_STOP;		// UIの浮遊感
				g_aSpellUI[nIdx].aSpellUI[nCntUI].fWidth = MAGIC_SIZE;	// 幅
				g_aSpellUI[nIdx].aSpellUI[nCntUI].fWidthDest = MAGIC_SIZE;	// 幅の目的地
				g_aSpellUI[nIdx].aSpellUI[nCntUI].fHeightDest = MAGIC_SIZE;	// 高さの目的地
				g_aSpellUI[nIdx].aSpellUI[nCntUI].nKey = 0;			// 浮遊感を演出
				g_aSpellUI[nIdx].aSpellUI[nCntUI].nNumKey = NUM_KEY;		// 浮遊感を演出
				g_aSpellUI[nIdx].aSpellUI[nCntUI].bDisp = true;			// 表示状態
				break;

			case SPELLUI_TYPE_BLUE:	// 青魔法
				g_aSpellUI[nIdx].aSpellUI[nCntUI].pos = D3DXVECTOR3(-COMMAND_X, MAGICRB_Y, 0.0f);	// 中心位置
				// テクスチャの種類
				if (operationType == OPERATIONTYPE_KEYBOARD)
				{ // キーボード操作
					g_aSpellUI[nIdx].aSpellUI[nCntUI].tex = SPELLUI_TEX_J;
				}
				else
				{ // ジョイパッド操作
					g_aSpellUI[nIdx].aSpellUI[nCntUI].tex = SPELLUI_TEX_X;
				}
				g_aSpellUI[nIdx].aSpellUI[nCntUI].type = SPELLUI_TYPE_BLUE;	// テクスチャの配置
				g_aSpellUI[nIdx].aSpellUI[nCntUI].move = SPELLUI_MOVE_STOP;	// UIの浮遊感
				g_aSpellUI[nIdx].aSpellUI[nCntUI].fWidth = MAGIC_SIZE;	// 幅
				g_aSpellUI[nIdx].aSpellUI[nCntUI].fWidthDest = MAGIC_SIZE;	// 幅の目的地
				g_aSpellUI[nIdx].aSpellUI[nCntUI].fHeightDest = MAGIC_SIZE;	// 高さの目的地
				g_aSpellUI[nIdx].aSpellUI[nCntUI].nKey = 0;			// 浮遊感を演出
				g_aSpellUI[nIdx].aSpellUI[nCntUI].nNumKey = NUM_KEY;		// 浮遊感を演出
				g_aSpellUI[nIdx].aSpellUI[nCntUI].bDisp = true;			// 表示状態
				break;

			case SPELLUI_TYPE_YELLOW:	// 黄魔法
				g_aSpellUI[nIdx].aSpellUI[nCntUI].pos = D3DXVECTOR3(0.0f, MAGICY_Y, 0.0f);	// 中心位置
				// テクスチャの種類
				if (operationType == OPERATIONTYPE_KEYBOARD)
				{ // キーボード操作
					g_aSpellUI[nIdx].aSpellUI[nCntUI].tex = SPELLUI_TEX_I;
				}
				else
				{ // ジョイパッド操作
					g_aSpellUI[nIdx].aSpellUI[nCntUI].tex = SPELLUI_TEX_Y;
				}
				g_aSpellUI[nIdx].aSpellUI[nCntUI].type = SPELLUI_TYPE_YELLOW;	// テクスチャの配置
				g_aSpellUI[nIdx].aSpellUI[nCntUI].move = SPELLUI_MOVE_STOP;	// UIの浮遊感
				g_aSpellUI[nIdx].aSpellUI[nCntUI].fWidth = MAGIC_SIZE;	// 幅
				g_aSpellUI[nIdx].aSpellUI[nCntUI].fWidthDest = MAGIC_SIZE;	// 幅の目的地
				g_aSpellUI[nIdx].aSpellUI[nCntUI].fHeightDest = MAGIC_SIZE;	// 高さの目的地
				g_aSpellUI[nIdx].aSpellUI[nCntUI].nKey = 0;			// 浮遊感を演出
				g_aSpellUI[nIdx].aSpellUI[nCntUI].nNumKey = NUM_KEY;		// 浮遊感を演出
				g_aSpellUI[nIdx].aSpellUI[nCntUI].bDisp = true;			// 表示状態
				break;
			}
		}
		g_aSpellUI[nIdx].bSpell = true;
		g_aSpellUI[nIdx].state = SPELLUISTATE_APPEAR;
		g_aSpellUI[nIdx].rot = D3DXVECTOR3(0.0f, 3.14f, 0.0f);	// 中心位置
		g_aSpellUI[nIdx].magicType = SPELLUI_TEX_NONE;		// 発動中の魔法を格納
		g_aSpellUI[nIdx].nNumKey = NUM_KEY;			// 処理を行うキー数
		g_aSpellUI[nIdx].nKey = 0;				// 現在のキー
	}
}

//======================================================================================
// SpellUIを表示状態にする
//======================================================================================
void SetSpellUIDisplay(int nIdx)
{
	// 操作人数の取得
	OPERATIONTYPE operationType = GetOperationType();
	Player* pPlayer = GetPlayer();
	if (pPlayer->state != PLAYERSTATE_PAUSE)
	{
		for (int nCntUI = 0; nCntUI < MAXSPELL_TYPE; nCntUI++)
		{
			// 各種情報の設定
			switch (nCntUI)
			{
			case SPELLUI_TYPE_RED:	// 赤魔法
				g_aSpellUI[nIdx].aSpellUI[nCntUI].pos = D3DXVECTOR3(COMMAND_X, MAGICRB_Y, 0.0f);	// 中心位置
				// テクスチャの種類
				if (operationType == OPERATIONTYPE_KEYBOARD)
				{ // キーボード操作
					g_aSpellUI[nIdx].aSpellUI[nCntUI].tex = SPELLUI_TEX_L;
				}
				else
				{ // ジョイパッド操作
					g_aSpellUI[nIdx].aSpellUI[nCntUI].tex = SPELLUI_TEX_B;
				}
				g_aSpellUI[nIdx].aSpellUI[nCntUI].type = SPELLUI_TYPE_RED;	// テクスチャの配置
				g_aSpellUI[nIdx].aSpellUI[nCntUI].move = SPELLUI_MOVE_STOP;	// UIの浮遊感
				g_aSpellUI[nIdx].aSpellUI[nCntUI].fWidth = MAGIC_SIZE;	// 幅
				g_aSpellUI[nIdx].aSpellUI[nCntUI].fHeight = MAGIC_SIZE;	// 高さ
				g_aSpellUI[nIdx].aSpellUI[nCntUI].fWidthDest = 0.0f;			// 幅の目的地
				g_aSpellUI[nIdx].aSpellUI[nCntUI].fHeightDest = 0.0f;			// 高さの目的地
				g_aSpellUI[nIdx].aSpellUI[nCntUI].nKey = 0;			// 浮遊感を演出
				g_aSpellUI[nIdx].aSpellUI[nCntUI].nNumKey = NUM_KEY;		// 浮遊感を演出
				g_aSpellUI[nIdx].aSpellUI[nCntUI].bDisp = true;			// 表示状態
				break;

			case SPELLUI_TYPE_GREEN:	// 緑魔法
				g_aSpellUI[nIdx].aSpellUI[nCntUI].pos = D3DXVECTOR3(0.0f, MAGICG_Y, 0.0f);	// 中心位置
				// テクスチャの種類
				if (operationType == OPERATIONTYPE_KEYBOARD)
				{ // キーボード操作
					g_aSpellUI[nIdx].aSpellUI[nCntUI].tex = SPELLUI_TEX_K;
				}
				else
				{ // ジョイパッド操作
					g_aSpellUI[nIdx].aSpellUI[nCntUI].tex = SPELLUI_TEX_A;
				}
				g_aSpellUI[nIdx].aSpellUI[nCntUI].type = SPELLUI_TYPE_GREEN;	// テクスチャの配置
				g_aSpellUI[nIdx].aSpellUI[nCntUI].move = SPELLUI_MOVE_STOP;		// UIの浮遊感
				g_aSpellUI[nIdx].aSpellUI[nCntUI].fWidth = MAGIC_SIZE;	// 幅
				g_aSpellUI[nIdx].aSpellUI[nCntUI].fHeight = MAGIC_SIZE;	// 高さ
				g_aSpellUI[nIdx].aSpellUI[nCntUI].fWidthDest = 0.0f;			// 幅の目的地
				g_aSpellUI[nIdx].aSpellUI[nCntUI].fHeightDest = 0.0f;			// 高さの目的地
				g_aSpellUI[nIdx].aSpellUI[nCntUI].nKey = 0;			// 浮遊感を演出
				g_aSpellUI[nIdx].aSpellUI[nCntUI].nNumKey = NUM_KEY;		// 浮遊感を演出
				g_aSpellUI[nIdx].aSpellUI[nCntUI].bDisp = true;			// 表示状態
				break;

			case SPELLUI_TYPE_BLUE:	// 青魔法
				g_aSpellUI[nIdx].aSpellUI[nCntUI].pos = D3DXVECTOR3(-COMMAND_X, MAGICRB_Y, 0.0f);	// 中心位置
				// テクスチャの種類
				if (operationType == OPERATIONTYPE_KEYBOARD)
				{ // キーボード操作
					g_aSpellUI[nIdx].aSpellUI[nCntUI].tex = SPELLUI_TEX_J;
				}
				else
				{ // ジョイパッド操作
					g_aSpellUI[nIdx].aSpellUI[nCntUI].tex = SPELLUI_TEX_X;
				}
				g_aSpellUI[nIdx].aSpellUI[nCntUI].type = SPELLUI_TYPE_BLUE;	// テクスチャの配置
				g_aSpellUI[nIdx].aSpellUI[nCntUI].move = SPELLUI_MOVE_STOP;	// UIの浮遊感
				g_aSpellUI[nIdx].aSpellUI[nCntUI].fWidth = MAGIC_SIZE;	// 幅
				g_aSpellUI[nIdx].aSpellUI[nCntUI].fHeight = MAGIC_SIZE;	// 高さ
				g_aSpellUI[nIdx].aSpellUI[nCntUI].fWidthDest = 0.0f;			// 幅の目的地
				g_aSpellUI[nIdx].aSpellUI[nCntUI].fHeightDest = 0.0f;			// 高さの目的地
				g_aSpellUI[nIdx].aSpellUI[nCntUI].nKey = 0;			// 浮遊感を演出
				g_aSpellUI[nIdx].aSpellUI[nCntUI].nNumKey = NUM_KEY;		// 浮遊感を演出
				g_aSpellUI[nIdx].aSpellUI[nCntUI].bDisp = true;			// 表示状態
				break;

			case SPELLUI_TYPE_YELLOW:	// 黄魔法
				g_aSpellUI[nIdx].aSpellUI[nCntUI].pos = D3DXVECTOR3(0.0f, MAGICY_Y, 0.0f);	// 中心位置
				// テクスチャの種類
				if (operationType == OPERATIONTYPE_KEYBOARD)
				{ // キーボード操作
					g_aSpellUI[nIdx].aSpellUI[nCntUI].tex = SPELLUI_TEX_I;
				}
				else
				{ // ジョイパッド操作
					g_aSpellUI[nIdx].aSpellUI[nCntUI].tex = SPELLUI_TEX_Y;
				}
				g_aSpellUI[nIdx].aSpellUI[nCntUI].type = SPELLUI_TYPE_YELLOW;	// テクスチャの配置
				g_aSpellUI[nIdx].aSpellUI[nCntUI].move = SPELLUI_MOVE_STOP;		// UIの浮遊感
				g_aSpellUI[nIdx].aSpellUI[nCntUI].fWidth = MAGIC_SIZE;	// 幅
				g_aSpellUI[nIdx].aSpellUI[nCntUI].fHeight = MAGIC_SIZE;	// 高さ
				g_aSpellUI[nIdx].aSpellUI[nCntUI].fWidthDest = 0.0f;			// 幅の目的地
				g_aSpellUI[nIdx].aSpellUI[nCntUI].fHeightDest = 0.0f;			// 高さの目的地
				g_aSpellUI[nIdx].aSpellUI[nCntUI].nKey = 0;			// 浮遊感を演出
				g_aSpellUI[nIdx].aSpellUI[nCntUI].nNumKey = NUM_KEY;		// 浮遊感を演出
				g_aSpellUI[nIdx].aSpellUI[nCntUI].bDisp = true;			// 表示状態
				break;
			}
		}
		g_aSpellUI[nIdx].state = SPELLUISTATE_DISPLAY;
		g_aSpellUI[nIdx].rot = D3DXVECTOR3(0.0f, 3.14f, 0.0f);	// 中心位置
		g_aSpellUI[nIdx].magicType = SPELLUI_TEX_NONE;		// 発動中の魔法を格納
		g_aSpellUI[nIdx].nNumKey = NUM_KEY;			// 処理を行うキー数
		g_aSpellUI[nIdx].nKey = 0;				// 現在のキー
	}
}

//======================================================================================
// SpellUIを収縮状態にする
//======================================================================================
void SetSpellUIDisappear(int nIdx)
{
	// 操作人数の取得
	OPERATIONTYPE operationType = GetOperationType();
	Player* pPlayer = GetPlayer();
	if (pPlayer->state != PLAYERSTATE_PAUSE)
	{
		for (int nCntUI = 0; nCntUI < MAXSPELL_TYPE; nCntUI++)
		{
			// 各種情報の設定
			switch (nCntUI)
			{
			case SPELLUI_TYPE_COMMAND0:	// 1つ目のコマンド
				if (g_aSpellUI[nIdx].aSpellUI[nCntUI].bDisp == true)
				{
					SetCommandDisappear(nIdx, nCntUI);
				}
				break;

			case SPELLUI_TYPE_COMMAND1:	// 2つ目のコマンド
				if (g_aSpellUI[nIdx].aSpellUI[nCntUI].bDisp == true)
				{
					SetCommandDisappear(nIdx, nCntUI);
				}
				break;

			case SPELLUI_TYPE_COMMAND2:	// 2つ目のコマンド
				if (g_aSpellUI[nIdx].aSpellUI[nCntUI].bDisp == true)
				{
					SetCommandDisappear(nIdx, nCntUI);
				}
				break;

			case SPELLUI_TYPE_RED:	// 赤魔法
				g_aSpellUI[nIdx].aSpellUI[nCntUI].pos = D3DXVECTOR3(COMMAND_X, MAGICRB_Y, 0.0f);	// 中心位置
				// テクスチャの種類
				if (operationType == OPERATIONTYPE_KEYBOARD)
				{ // キーボード操作
					g_aSpellUI[nIdx].aSpellUI[nCntUI].tex = SPELLUI_TEX_L;
				}
				else
				{ // ジョイパッド操作
					g_aSpellUI[nIdx].aSpellUI[nCntUI].tex = SPELLUI_TEX_B;
				}
				g_aSpellUI[nIdx].aSpellUI[nCntUI].type = SPELLUI_TYPE_RED;	// テクスチャの配置
				g_aSpellUI[nIdx].aSpellUI[nCntUI].move = SPELLUI_MOVE_STOP;	// UIの浮遊感
				g_aSpellUI[nIdx].aSpellUI[nCntUI].fWidth = MAGIC_SIZE;	// 幅
				g_aSpellUI[nIdx].aSpellUI[nCntUI].fWidthDest = 0.0f;			// 幅の目的地
				g_aSpellUI[nIdx].aSpellUI[nCntUI].fHeightDest = 0.0f;			// 高さの目的地
				g_aSpellUI[nIdx].aSpellUI[nCntUI].nKey = 0;			// 浮遊感を演出
				g_aSpellUI[nIdx].aSpellUI[nCntUI].nNumKey = NUM_KEY;		// 浮遊感を演出
				g_aSpellUI[nIdx].aSpellUI[nCntUI].bDisp = true;			// 表示状態
				break;

			case SPELLUI_TYPE_GREEN:	// 緑魔法
				g_aSpellUI[nIdx].aSpellUI[nCntUI].pos = D3DXVECTOR3(0.0f, MAGICG_Y, 0.0f);	// 中心位置
				// テクスチャの種類
				if (operationType == OPERATIONTYPE_KEYBOARD)
				{ // キーボード操作
					g_aSpellUI[nIdx].aSpellUI[nCntUI].tex = SPELLUI_TEX_K;
				}
				else
				{ // ジョイパッド操作
					g_aSpellUI[nIdx].aSpellUI[nCntUI].tex = SPELLUI_TEX_A;
				}
				g_aSpellUI[nIdx].aSpellUI[nCntUI].type = SPELLUI_TYPE_GREEN;	// テクスチャの配置
				g_aSpellUI[nIdx].aSpellUI[nCntUI].move = SPELLUI_MOVE_STOP;	// UIの浮遊感
				g_aSpellUI[nIdx].aSpellUI[nCntUI].fWidth = MAGIC_SIZE;	// 幅
				g_aSpellUI[nIdx].aSpellUI[nCntUI].fWidthDest = 0.0f;			// 幅の目的地
				g_aSpellUI[nIdx].aSpellUI[nCntUI].fHeightDest = 0.0f;			// 高さの目的地
				g_aSpellUI[nIdx].aSpellUI[nCntUI].nKey = 0;			// 浮遊感を演出
				g_aSpellUI[nIdx].aSpellUI[nCntUI].nNumKey = NUM_KEY;		// 浮遊感を演出
				g_aSpellUI[nIdx].aSpellUI[nCntUI].bDisp = true;			// 表示状態
				break;

			case SPELLUI_TYPE_BLUE:	// 青魔法
				g_aSpellUI[nIdx].aSpellUI[nCntUI].pos = D3DXVECTOR3(-COMMAND_X, MAGICRB_Y, 0.0f);	// 中心位置
				// テクスチャの種類
				if (operationType == OPERATIONTYPE_KEYBOARD)
				{ // キーボード操作
					g_aSpellUI[nIdx].aSpellUI[nCntUI].tex = SPELLUI_TEX_J;
				}
				else
				{ // ジョイパッド操作
					g_aSpellUI[nIdx].aSpellUI[nCntUI].tex = SPELLUI_TEX_X;
				}
				g_aSpellUI[nIdx].aSpellUI[nCntUI].type = SPELLUI_TYPE_BLUE;	// テクスチャの配置
				g_aSpellUI[nIdx].aSpellUI[nCntUI].move = SPELLUI_MOVE_STOP;	// UIの浮遊感
				g_aSpellUI[nIdx].aSpellUI[nCntUI].fWidth = MAGIC_SIZE;	// 幅
				g_aSpellUI[nIdx].aSpellUI[nCntUI].fWidthDest = 0.0f;			// 幅の目的地
				g_aSpellUI[nIdx].aSpellUI[nCntUI].fHeightDest = 0.0f;			// 高さの目的地
				g_aSpellUI[nIdx].aSpellUI[nCntUI].nKey = 0;			// 浮遊感を演出
				g_aSpellUI[nIdx].aSpellUI[nCntUI].nNumKey = NUM_KEY;		// 浮遊感を演出
				g_aSpellUI[nIdx].aSpellUI[nCntUI].bDisp = true;			// 表示状態
				break;

			case SPELLUI_TYPE_YELLOW:	// 黄魔法
				g_aSpellUI[nIdx].aSpellUI[nCntUI].pos = D3DXVECTOR3(0.0f, MAGICY_Y, 0.0f);	// 中心位置
				// テクスチャの種類
				if (operationType == OPERATIONTYPE_KEYBOARD)
				{ // キーボード操作
					g_aSpellUI[nIdx].aSpellUI[nCntUI].tex = SPELLUI_TEX_I;
				}
				else
				{ // ジョイパッド操作
					g_aSpellUI[nIdx].aSpellUI[nCntUI].tex = SPELLUI_TEX_Y;
				}
				g_aSpellUI[nIdx].aSpellUI[nCntUI].type = SPELLUI_TYPE_YELLOW;	// テクスチャの配置
				g_aSpellUI[nIdx].aSpellUI[nCntUI].move = SPELLUI_MOVE_STOP;	// UIの浮遊感
				g_aSpellUI[nIdx].aSpellUI[nCntUI].fWidth = MAGIC_SIZE;	// 幅
				g_aSpellUI[nIdx].aSpellUI[nCntUI].fWidthDest = 0.0f;			// 幅の目的地
				g_aSpellUI[nIdx].aSpellUI[nCntUI].fHeightDest = 0.0f;			// 高さの目的地
				g_aSpellUI[nIdx].aSpellUI[nCntUI].nKey = 0;			// 浮遊感を演出
				g_aSpellUI[nIdx].aSpellUI[nCntUI].nNumKey = NUM_KEY;		// 浮遊感を演出
				g_aSpellUI[nIdx].aSpellUI[nCntUI].bDisp = true;			// 表示状態
				break;
			}
		}
		g_aSpellUI[nIdx].state = SPELLUISTATE_DISAPPEAR;
		g_aSpellUI[nIdx].rot = D3DXVECTOR3(0.0f, 3.14f, 0.0f);	// 中心位置
		g_aSpellUI[nIdx].magicType = SPELLUI_TEX_NONE;		// 発動中の魔法を格納
		g_aSpellUI[nIdx].nNumKey = NUM_KEY;			// 処理を行うキー数
		g_aSpellUI[nIdx].nKey = 0;				// 現在のキー
	}
}

//======================================================================================
// SpellUIを非表示状態にする
//======================================================================================
void SetSpellUINonDisplay(int nIdx)
{
	// 操作人数の取得
	OPERATIONTYPE operationType = GetOperationType();

	for (int nCntUI = 0; nCntUI < MAXSPELL_TYPE; nCntUI++)
	{
		// 各種情報の設定
		switch (nCntUI)
		{
		case SPELLUI_TYPE_RED:	// 赤魔法
			g_aSpellUI[nIdx].aSpellUI[nCntUI].pos = D3DXVECTOR3(COMMAND_X, MAGICRB_Y, 0.0f);	// 中心位置
			// テクスチャの種類
			if (operationType == OPERATIONTYPE_KEYBOARD)
			{ // キーボード操作
				g_aSpellUI[nIdx].aSpellUI[nCntUI].tex = SPELLUI_TEX_L;
			}
			else
			{ // ジョイパッド操作
				g_aSpellUI[nIdx].aSpellUI[nCntUI].tex = SPELLUI_TEX_B;
			}
			g_aSpellUI[nIdx].aSpellUI[nCntUI].type	= SPELLUI_TYPE_RED;	// テクスチャの配置
			g_aSpellUI[nIdx].aSpellUI[nCntUI].move	= SPELLUI_MOVE_STOP;	// UIの浮遊感
			g_aSpellUI[nIdx].aSpellUI[nCntUI].fWidth		= 0.0f;		// 幅
			g_aSpellUI[nIdx].aSpellUI[nCntUI].fHeight		= 0.0f;		// 高さ
			g_aSpellUI[nIdx].aSpellUI[nCntUI].fWidthDest	= 0.0f;		// 幅の目的地
			g_aSpellUI[nIdx].aSpellUI[nCntUI].fHeightDest	= 0.0f;		// 高さの目的地
			g_aSpellUI[nIdx].aSpellUI[nCntUI].nKey			= 0;		// 浮遊感を演出
			g_aSpellUI[nIdx].aSpellUI[nCntUI].nNumKey		= NUM_KEY;	// 浮遊感を演出
			g_aSpellUI[nIdx].aSpellUI[nCntUI].bDisp			= true;		// 表示状態
			break;

		case SPELLUI_TYPE_GREEN:	// 緑魔法
			g_aSpellUI[nIdx].aSpellUI[nCntUI].pos = D3DXVECTOR3(0.0f, MAGICG_Y, 0.0f);	// 中心位置
			// テクスチャの種類
			if (operationType == OPERATIONTYPE_KEYBOARD)
			{ // キーボード操作
				g_aSpellUI[nIdx].aSpellUI[nCntUI].tex = SPELLUI_TEX_K;
			}
			else
			{ // ジョイパッド操作
				g_aSpellUI[nIdx].aSpellUI[nCntUI].tex = SPELLUI_TEX_A;
			}
			g_aSpellUI[nIdx].aSpellUI[nCntUI].type	= SPELLUI_TYPE_GREEN;	// テクスチャの配置
			g_aSpellUI[nIdx].aSpellUI[nCntUI].move	= SPELLUI_MOVE_STOP;	// UIの浮遊感
			g_aSpellUI[nIdx].aSpellUI[nCntUI].fWidth		= 0.0f;		// 幅
			g_aSpellUI[nIdx].aSpellUI[nCntUI].fHeight		= 0.0f;		// 高さ
			g_aSpellUI[nIdx].aSpellUI[nCntUI].fWidthDest	= 0.0f;		// 幅の目的地
			g_aSpellUI[nIdx].aSpellUI[nCntUI].fHeightDest	= 0.0f;		// 高さの目的地
			g_aSpellUI[nIdx].aSpellUI[nCntUI].nKey			= 0;		// 浮遊感を演出
			g_aSpellUI[nIdx].aSpellUI[nCntUI].nNumKey		= NUM_KEY;	// 浮遊感を演出
			g_aSpellUI[nIdx].aSpellUI[nCntUI].bDisp			= true;		// 表示状態
			break;

		case SPELLUI_TYPE_BLUE:	// 青魔法
			g_aSpellUI[nIdx].aSpellUI[nCntUI].pos = D3DXVECTOR3(-COMMAND_X, MAGICRB_Y, 0.0f);	// 中心位置
			// テクスチャの種類
			if (operationType == OPERATIONTYPE_KEYBOARD)
			{ // キーボード操作
				g_aSpellUI[nIdx].aSpellUI[nCntUI].tex = SPELLUI_TEX_J;
			}
			else
			{ // ジョイパッド操作
				g_aSpellUI[nIdx].aSpellUI[nCntUI].tex = SPELLUI_TEX_X;
			}
			g_aSpellUI[nIdx].aSpellUI[nCntUI].type	= SPELLUI_TYPE_BLUE;	// テクスチャの配置
			g_aSpellUI[nIdx].aSpellUI[nCntUI].move	= SPELLUI_MOVE_STOP;	// UIの浮遊感
			g_aSpellUI[nIdx].aSpellUI[nCntUI].fWidth		= 0.0f;		// 幅
			g_aSpellUI[nIdx].aSpellUI[nCntUI].fHeight		= 0.0f;		// 高さ
			g_aSpellUI[nIdx].aSpellUI[nCntUI].fWidthDest	= 0.0f;		// 幅の目的地
			g_aSpellUI[nIdx].aSpellUI[nCntUI].fHeightDest	= 0.0f;		// 高さの目的地
			g_aSpellUI[nIdx].aSpellUI[nCntUI].nKey			= 0;		// 浮遊感を演出
			g_aSpellUI[nIdx].aSpellUI[nCntUI].nNumKey		= NUM_KEY;	// 浮遊感を演出
			g_aSpellUI[nIdx].aSpellUI[nCntUI].bDisp			= true;		// 表示状態
			break;

		case SPELLUI_TYPE_YELLOW:	// 黄魔法
			g_aSpellUI[nIdx].aSpellUI[nCntUI].pos = D3DXVECTOR3(0.0f, MAGICY_Y, 0.0f);	// 中心位置
			// テクスチャの種類
			if (operationType == OPERATIONTYPE_KEYBOARD)
			{ // キーボード操作
				g_aSpellUI[nIdx].aSpellUI[nCntUI].tex = SPELLUI_TEX_I;
			}
			else
			{ // ジョイパッド操作
				g_aSpellUI[nIdx].aSpellUI[nCntUI].tex = SPELLUI_TEX_Y;
			}
			g_aSpellUI[nIdx].aSpellUI[nCntUI].type	= SPELLUI_TYPE_YELLOW;	// テクスチャの配置
			g_aSpellUI[nIdx].aSpellUI[nCntUI].move	= SPELLUI_MOVE_STOP;		// UIの浮遊感
			g_aSpellUI[nIdx].aSpellUI[nCntUI].fWidth		= 0.0f;		// 幅
			g_aSpellUI[nIdx].aSpellUI[nCntUI].fHeight		= 0.0f;		// 高さ
			g_aSpellUI[nIdx].aSpellUI[nCntUI].fWidthDest	= 0.0f;		// 幅の目的地
			g_aSpellUI[nIdx].aSpellUI[nCntUI].fHeightDest	= 0.0f;		// 高さの目的地
			g_aSpellUI[nIdx].aSpellUI[nCntUI].nKey			= 0;		// 浮遊感を演出
			g_aSpellUI[nIdx].aSpellUI[nCntUI].nNumKey		= NUM_KEY;	// 浮遊感を演出
			g_aSpellUI[nIdx].aSpellUI[nCntUI].bDisp			= true;		// 表示状態
			break;
		}
	}
	g_aSpellUI[nIdx].state = SPELLUISTATE_NONDISPLAY;
	g_aSpellUI[nIdx].bSpell = false;
	g_aSpellUI[nIdx].rot = D3DXVECTOR3(0.0f, 3.14f, 0.0f);	// 中心位置
	g_aSpellUI[nIdx].magicType = SPELLUI_TEX_NONE;		// 発動中の魔法を格納
	g_aSpellUI[nIdx].nNumKey = NUM_KEY;			// 処理を行うキー数
	g_aSpellUI[nIdx].nKey = 0;				// 現在のキー
}

//======================================================================================
// コマンドを出現状態にする
//======================================================================================
void SetCommandTex(int nIdx, int nNumCommand)
{
	g_aSpellUI[nIdx].aSpellUI[nNumCommand].move = SPELLUI_MOVE_APPEAR;
	g_aSpellUI[nIdx].aSpellUI[nNumCommand].fWidth = COMMAND_SIZE;		// 幅
	g_aSpellUI[nIdx].aSpellUI[nNumCommand].fHeight = 0.0f;				// 高さ
	g_aSpellUI[nIdx].aSpellUI[nNumCommand].fWidthDest = 0.0f;			// 幅の目的地
	g_aSpellUI[nIdx].aSpellUI[nNumCommand].fHeightDest = COMMAND_SIZE;	// 高さの目的地
	g_aSpellUI[nIdx].aSpellUI[nNumCommand].nKey = 0;					// キーの初期化
	g_aSpellUI[nIdx].aSpellUI[nNumCommand].nNumKey = NUM_KEY;			// 浮遊感を演出
	g_aSpellUI[nIdx].aSpellUI[nNumCommand].bDisp = true;				// 表示状態
}

//======================================================================================
// コマンドを表示状態にする
//======================================================================================
void SetCommandDisplay(int nIdx, int nNumCommand)
{
	g_aSpellUI[nIdx].aSpellUI[nNumCommand].move = SPELLUI_MOVE_STOP;
	g_aSpellUI[nIdx].aSpellUI[nNumCommand].fWidth = COMMAND_SIZE;		// 幅
	g_aSpellUI[nIdx].aSpellUI[nNumCommand].fHeight = COMMAND_SIZE;		// 高さ
	g_aSpellUI[nIdx].aSpellUI[nNumCommand].fWidthDest = 0.0f;			// 幅の目的地
	g_aSpellUI[nIdx].aSpellUI[nNumCommand].fHeightDest = COMMAND_SIZE;	// 高さの目的地
	g_aSpellUI[nIdx].aSpellUI[nNumCommand].nKey = 0;					// キーの初期化
	g_aSpellUI[nIdx].aSpellUI[nNumCommand].nNumKey = NUM_KEY;			// 浮遊感を演出
}

//======================================================================================
// コマンドを収縮状態にする
//======================================================================================
void SetCommandDisappear(int nIdx, int nNumCommand)
{
	g_aSpellUI[nIdx].aSpellUI[nNumCommand].move = SPELLUI_MOVE_DISAPPEAR;
	g_aSpellUI[nIdx].aSpellUI[nNumCommand].fWidth = COMMAND_SIZE;		// 幅
	g_aSpellUI[nIdx].aSpellUI[nNumCommand].fHeight = COMMAND_SIZE;		// 高さ
	g_aSpellUI[nIdx].aSpellUI[nNumCommand].fWidthDest = 0.0f;			// 幅の目的地
	g_aSpellUI[nIdx].aSpellUI[nNumCommand].fHeightDest = 0.0f;	// 高さの目的地
	g_aSpellUI[nIdx].aSpellUI[nNumCommand].nKey = 0;					// キーの初期化
	g_aSpellUI[nIdx].aSpellUI[nNumCommand].nNumKey = NUM_KEY;			// 浮遊感を演出
}

//======================================================================================
// UIの表示状態を取得
//======================================================================================
bool GetSpellUIDisp(int nIdx)
{
	return g_aSpellUI[nIdx].bSpell;
}

//======================================================================================
// UIのステータスを変更
//======================================================================================
SPELLUISTATE GetSpellUIState(int nIdx)
{
	return g_aSpellUI[nIdx].state;
}

//======================================================================================
// spellのテクスチャ取得処理
//======================================================================================
LPDIRECT3DTEXTURE9 GetSpellTexture(SPELLUI_TEX SpellTex)
{
	return g_apTextureSpellUI[SpellTex + SPELLUI_TEX_LEVITATION];
}