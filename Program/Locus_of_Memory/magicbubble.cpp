//========================================================
// 
// 魔法発動状態のUI表示処理[magicbubble.cpp]
// Author : KajinoHiyori
// 
//========================================================
#include "main.h"
#include "magicbubble.h"
#include "player.h"
#include "game.h"
#include "input.h"
#include "title.h"
#include "fog.h"
#include "debugproc.h"
#include "camera.h"

// マクロ定義
#define MAX_COMMAND			(3)					// 受け付けるコマンドの最大数
#define NUM_KEY				(30)				// 処理を行うキー数
#define DISP_MAGIC			(30)				// 魔法を表示している時間
#define COMMAND_SIZE		(8.0f)				// コマンドのサイズ
#define COMMAND_X			(15.0f)				// コマンドのX軸
#define COMMAND_Y			(55.0f)				// コマンドの高度
#define MAXMAGICBUBBLE_TYPE		(MAGICBUBBLETYPE_MAX)	// 配置の最大数
#define MAGIC_SIZE			(6.0f)				// 魔法の大きさ
#define MAGICY_Y			(40.0f)			// 黄魔法の高度
#define MAGICG_Y			(20.0f)			// 緑魔法の高度
#define MAGICRB_Y			(30.0f)			// 赤青魔法の高度
#define MAGICBUBBLE_Z			(-40.0f)		// MAGICBUBBLEの表示位置
#define NORMAL				(D3DXVECTOR3(0.0f, 1.0f, 0.0f))	// 法線ベクトル

// MAGICBUBBLEの構造体
typedef struct
{
	D3DXMATRIX		mtxWorld;	// ワールドマトリックス
	D3DXVECTOR3		pos;		// 位置
	MAGICBUBBLETYPE type;
	float	fWidth;			// 幅
	float	fWidthDest;		// 幅の目的値
	float	fHeight;		// 高さ
	float	fHeightDest;	// 高さの目的値
	int		nNumKey;		// 浮遊感をカウントするキー数
	int		nKey;			// 現在のキー数
	bool	bDisp;			// 表示状態
}MagicBubble;

// グローバル変数
LPDIRECT3DTEXTURE9	g_apTextureMagicBubble[MAXMAGICBUBBLE_TYPE] = {};	// テクスチャへのポインタ
LPDIRECT3DVERTEXBUFFER9	g_pVtxBuffMagicBubble = NULL;			// 頂点バッファへのポインタ
MagicBubble g_aMagicBubble[MAX_PLAYER];		// MAGICBUBBLEの全体管理

// テクスチャの読み込み
const char* c_apFilenameMagicBubble[MAXMAGICBUBBLE_TYPE] =
{
	"data\\TEXTURE\\MagicBubble\\00_MagicNull.png",
	"data\\TEXTURE\\MagicBubble\\01_Red.png",
	"data\\TEXTURE\\MagicBubble\\02_Green.png",
	"data\\TEXTURE\\MagicBubble\\03_Blue.png",
	"data\\TEXTURE\\MagicBubble\\04_Yellow.png",
	"data\\TEXTURE\\MagicBubble\\05_K.png",
	"data\\TEXTURE\\MagicBubble\\06_L.png",
	"data\\TEXTURE\\MagicBubble\\07_J.png",
	"data\\TEXTURE\\MagicBubble\\08_I.png",
	"data\\TEXTURE\\MagicBubble\\09_A.png",
	"data\\TEXTURE\\MagicBubble\\10_B.png",
	"data\\TEXTURE\\MagicBubble\\11_X.png",
	"data\\TEXTURE\\MagicBubble\\12_Y.png",
	"data\\TEXTURE\\MagicBubble\\13_None.png",
	"data\\TEXTURE\\MagicBubble\\14_Levitation.png",
	"data\\TEXTURE\\MagicBubble\\15_Combustion.png",
	"data\\TEXTURE\\MagicBubble\\16_Flood.png",
	"data\\TEXTURE\\MagicBubble\\17_Flash.png",
	"data\\TEXTURE\\MagicBubble\\18_FireBall.png",
	"data\\TEXTURE\\MagicBubble\\19_SunsetDelay.png",
	"data\\TEXTURE\\MagicBubble\\20_RainPray.png",
	"data\\TEXTURE\\MagicBubble\\21_freeze.png",
	"data\\TEXTURE\\MagicBubble\\22_Grouth.png",
	"data\\TEXTURE\\MagicBubble\\23_Acceleration.png",
	"data\\TEXTURE\\MagicBubble\\24_TimeRevert.png",
};

#if 0
//======================================================================================
// spellの初期化処理
//======================================================================================
void InitMagicBubble(void)
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
	for (int nCntUI = 0; nCntUI < MAXMAGICBUBBLE_TYPE; nCntUI++)
	{
		D3DXCreateTextureFromFile(pDevice, c_apFilenameMagicBubble[nCntUI], &g_apTextureMagicBubble[nCntUI]);
	}

	// 初期化
	for (int nCntPlayer = 0; nCntPlayer < MAX_PLAYER; nCntPlayer++)
	{
		for (int nCntUI = 0; nCntUI < MAXMAGICBUBBLE_TYPE; nCntUI++)
		{
			g_aMagicBubble[nCntPlayer].aMagicBubble[nCntUI].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);	// 中心位置
			g_aMagicBubble[nCntPlayer].aMagicBubble[nCntUI].tex = MAGICBUBBLE_TEX_MAGICNULL;			// テクスチャの種類
			g_aMagicBubble[nCntPlayer].aMagicBubble[nCntUI].type = MAGICBUBBLE_TYPE_COMMAND0;			// テクスチャの配置
			g_aMagicBubble[nCntPlayer].aMagicBubble[nCntUI].move = MAGICBUBBLE_MOVE_NONDISP;				// UIの浮遊感
			g_aMagicBubble[nCntPlayer].aMagicBubble[nCntUI].fWidth = 0.0f;		// 幅
			g_aMagicBubble[nCntPlayer].aMagicBubble[nCntUI].fHeight = 0.0f;		// 高さ
			g_aMagicBubble[nCntPlayer].aMagicBubble[nCntUI].fWidthDest = 0.0f;		// 幅の目的地
			g_aMagicBubble[nCntPlayer].aMagicBubble[nCntUI].fHeightDest = 0.0f;		// 高さの目的地
			g_aMagicBubble[nCntPlayer].aMagicBubble[nCntUI].nKey = 0;		// 浮遊感を演出
			g_aMagicBubble[nCntPlayer].aMagicBubble[nCntUI].nNumKey = NUM_KEY;	// 浮遊感を演出
			g_aMagicBubble[nCntPlayer].aMagicBubble[nCntUI].bDisp = false;		// 表示状態
		}
		g_aMagicBubble[nCntPlayer].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);	// 中心位置
		g_aMagicBubble[nCntPlayer].rot = D3DXVECTOR3(0.0f, 3.14f, 0.0f);	// 中心位置
		g_aMagicBubble[nCntPlayer].magicType = MAGICBUBBLE_TEX_NONE;		// 発動中の魔法を格納
		g_aMagicBubble[nCntPlayer].state = MAGICBUBBLESTATE_NONDISPLAY;	// UIの表示状態
		g_aMagicBubble[nCntPlayer].nNumKey = NUM_KEY;			// 処理を行うキー数
		g_aMagicBubble[nCntPlayer].nKey = 0;				// 現在のキー
		g_aMagicBubble[nCntPlayer].nCounterUI = DISP_MAGIC;		// 魔法の表示時間
		g_aMagicBubble[nCntPlayer].bSpell = false;			// UI表示状態(trueでポーズ中)
	}

	// 頂点バッファの生成
	pDevice->CreateVertexBuffer(sizeof(VERTEX_3D) * 4 * MAXMAGICBUBBLE_TYPE * MAX_PLAYER, D3DUSAGE_WRITEONLY, FVF_VERTEX_3D, D3DPOOL_MANAGED, &g_pVtxBuffMagicBubble, NULL);

	VERTEX_3D* pVtx;
	// 頂点バッファをロックし、頂点情報へのポインタを取得
	g_pVtxBuffMagicBubble->Lock(0, 0, (void**)&pVtx, 0);

	for (int nCntPlayer = 0; nCntPlayer < MAX_PLAYER; nCntPlayer++)
	{
		for (int nCntUI = 0; nCntUI < MAXMAGICBUBBLE_TYPE; nCntUI++, pVtx += 4)
		{
			// 各種情報の設定
			switch (nCntUI)
			{
			case MAGICBUBBLE_TYPE_COMMAND0:	// 1つ目のコマンド
				g_aMagicBubble[nCntPlayer].aMagicBubble[nCntUI].pos = D3DXVECTOR3(-COMMAND_X, COMMAND_Y, 0.0f);	// 中心位置
				g_aMagicBubble[nCntPlayer].aMagicBubble[nCntUI].tex = MAGICBUBBLE_TEX_MAGICNULL;	// テクスチャの種類
				g_aMagicBubble[nCntPlayer].aMagicBubble[nCntUI].type = MAGICBUBBLE_TYPE_COMMAND0;	// テクスチャの配置
				g_aMagicBubble[nCntPlayer].aMagicBubble[nCntUI].move = MAGICBUBBLE_MOVE_NONDISP;		// UIの浮遊感
				g_aMagicBubble[nCntPlayer].aMagicBubble[nCntUI].fWidth = COMMAND_SIZE;				// 幅
				g_aMagicBubble[nCntPlayer].aMagicBubble[nCntUI].fHeight = COMMAND_SIZE;				// 高さ
				g_aMagicBubble[nCntPlayer].aMagicBubble[nCntUI].fWidthDest = 0.0f;					// 幅の目的地
				g_aMagicBubble[nCntPlayer].aMagicBubble[nCntUI].fHeightDest = 0.0f;					// 高さの目的地
				g_aMagicBubble[nCntPlayer].aMagicBubble[nCntUI].nKey = 0;						// 浮遊感を演出
				g_aMagicBubble[nCntPlayer].aMagicBubble[nCntUI].nNumKey = NUM_KEY;					// 浮遊感を演出
				g_aMagicBubble[nCntPlayer].aMagicBubble[nCntUI].bDisp = false;					// 表示状態
				break;

			case MAGICBUBBLE_TYPE_COMMAND1:	// 2つ目のコマンド
				g_aMagicBubble[nCntPlayer].aMagicBubble[nCntUI].pos = D3DXVECTOR3(0.0f, COMMAND_Y, 0.0f);	// 中心位置
				g_aMagicBubble[nCntPlayer].aMagicBubble[nCntUI].tex = MAGICBUBBLE_TEX_MAGICNULL;	// テクスチャの種類
				g_aMagicBubble[nCntPlayer].aMagicBubble[nCntUI].type = MAGICBUBBLE_TYPE_COMMAND1;	// テクスチャの配置
				g_aMagicBubble[nCntPlayer].aMagicBubble[nCntUI].move = MAGICBUBBLE_MOVE_NONDISP;		// UIの浮遊感
				g_aMagicBubble[nCntPlayer].aMagicBubble[nCntUI].fWidth = COMMAND_SIZE;				// 幅
				g_aMagicBubble[nCntPlayer].aMagicBubble[nCntUI].fHeight = COMMAND_SIZE;				// 高さ
				g_aMagicBubble[nCntPlayer].aMagicBubble[nCntUI].fWidthDest = 0.0f;					// 幅の目的地
				g_aMagicBubble[nCntPlayer].aMagicBubble[nCntUI].fHeightDest = 0.0f;					// 高さの目的地
				g_aMagicBubble[nCntPlayer].aMagicBubble[nCntUI].nKey = 0;						// 浮遊感を演出
				g_aMagicBubble[nCntPlayer].aMagicBubble[nCntUI].nNumKey = NUM_KEY;					// 浮遊感を演出
				g_aMagicBubble[nCntPlayer].aMagicBubble[nCntUI].bDisp = false;					// 表示状態
				break;

			case MAGICBUBBLE_TYPE_COMMAND2:	// 3つ目のコマンド
				g_aMagicBubble[nCntPlayer].aMagicBubble[nCntUI].pos = D3DXVECTOR3(COMMAND_X, COMMAND_Y, 0.0f);	// 中心位置
				g_aMagicBubble[nCntPlayer].aMagicBubble[nCntUI].tex = MAGICBUBBLE_TEX_MAGICNULL;	// テクスチャの種類
				g_aMagicBubble[nCntPlayer].aMagicBubble[nCntUI].type = MAGICBUBBLE_TYPE_COMMAND2;	// テクスチャの配置
				g_aMagicBubble[nCntPlayer].aMagicBubble[nCntUI].move = MAGICBUBBLE_MOVE_NONDISP;		// UIの浮遊感
				g_aMagicBubble[nCntPlayer].aMagicBubble[nCntUI].fWidth = COMMAND_SIZE;				// 幅
				g_aMagicBubble[nCntPlayer].aMagicBubble[nCntUI].fHeight = COMMAND_SIZE;				// 高さ
				g_aMagicBubble[nCntPlayer].aMagicBubble[nCntUI].fWidthDest = 0.0f;					// 幅の目的地
				g_aMagicBubble[nCntPlayer].aMagicBubble[nCntUI].fHeightDest = 0.0f;					// 高さの目的地
				g_aMagicBubble[nCntPlayer].aMagicBubble[nCntUI].nKey = 0;							// 浮遊感を演出
				g_aMagicBubble[nCntPlayer].aMagicBubble[nCntUI].nNumKey = NUM_KEY;					// 浮遊感を演出
				g_aMagicBubble[nCntPlayer].aMagicBubble[nCntUI].bDisp = false;					// 表示状態
				break;

			case MAGICBUBBLE_TYPE_RED:	// 赤魔法
				g_aMagicBubble[nCntPlayer].aMagicBubble[nCntUI].pos = D3DXVECTOR3(COMMAND_X, MAGICRB_Y, 0.0f);	// 中心位置
				// テクスチャの種類
				if (operationType == OPERATIONTYPE_KEYBOARD)
				{ // キーボード操作
					g_aMagicBubble[nCntPlayer].aMagicBubble[nCntUI].tex = MAGICBUBBLE_TEX_L;
				}
				else
				{ // ジョイパッド操作
					g_aMagicBubble[nCntPlayer].aMagicBubble[nCntUI].tex = MAGICBUBBLE_TEX_B;
				}
				g_aMagicBubble[nCntPlayer].aMagicBubble[nCntUI].type = MAGICBUBBLE_TYPE_RED;	// テクスチャの配置
				g_aMagicBubble[nCntPlayer].aMagicBubble[nCntUI].move = MAGICBUBBLE_MOVE_STOP;		// UIの浮遊感
				g_aMagicBubble[nCntPlayer].aMagicBubble[nCntUI].fWidth = MAGIC_SIZE;				// 幅
				g_aMagicBubble[nCntPlayer].aMagicBubble[nCntUI].fHeight = MAGIC_SIZE;				// 高さ
				g_aMagicBubble[nCntPlayer].aMagicBubble[nCntUI].fWidthDest = 0.0f;					// 幅の目的地
				g_aMagicBubble[nCntPlayer].aMagicBubble[nCntUI].fHeightDest = 0.0f;					// 高さの目的地
				g_aMagicBubble[nCntPlayer].aMagicBubble[nCntUI].nKey = 0;							// 浮遊感を演出
				g_aMagicBubble[nCntPlayer].aMagicBubble[nCntUI].nNumKey = NUM_KEY;					// 浮遊感を演出
				g_aMagicBubble[nCntPlayer].aMagicBubble[nCntUI].bDisp = true;						// 表示状態
				break;

			case MAGICBUBBLE_TYPE_GREEN:	// 緑魔法
				g_aMagicBubble[nCntPlayer].aMagicBubble[nCntUI].pos = D3DXVECTOR3(0.0f, MAGICG_Y, 0.0f);	// 中心位置
				// テクスチャの種類
				if (operationType == OPERATIONTYPE_KEYBOARD)
				{ // キーボード操作
					g_aMagicBubble[nCntPlayer].aMagicBubble[nCntUI].tex = MAGICBUBBLE_TEX_K;
				}
				else
				{ // ジョイパッド操作
					g_aMagicBubble[nCntPlayer].aMagicBubble[nCntUI].tex = MAGICBUBBLE_TEX_A;
				}
				g_aMagicBubble[nCntPlayer].aMagicBubble[nCntUI].type = MAGICBUBBLE_TYPE_GREEN;	// テクスチャの配置
				g_aMagicBubble[nCntPlayer].aMagicBubble[nCntUI].move = MAGICBUBBLE_MOVE_STOP;		// UIの浮遊感
				g_aMagicBubble[nCntPlayer].aMagicBubble[nCntUI].fWidth = MAGIC_SIZE;				// 幅
				g_aMagicBubble[nCntPlayer].aMagicBubble[nCntUI].fHeight = MAGIC_SIZE;				// 高さ
				g_aMagicBubble[nCntPlayer].aMagicBubble[nCntUI].fWidthDest = 0.0f;					// 幅の目的地
				g_aMagicBubble[nCntPlayer].aMagicBubble[nCntUI].fHeightDest = 0.0f;					// 高さの目的地
				g_aMagicBubble[nCntPlayer].aMagicBubble[nCntUI].nKey = 0;							// 浮遊感を演出
				g_aMagicBubble[nCntPlayer].aMagicBubble[nCntUI].nNumKey = NUM_KEY;					// 浮遊感を演出
				g_aMagicBubble[nCntPlayer].aMagicBubble[nCntUI].bDisp = true;						// 表示状態
				break;

			case MAGICBUBBLE_TYPE_BLUE:	// 青魔法
				g_aMagicBubble[nCntPlayer].aMagicBubble[nCntUI].pos = D3DXVECTOR3(-COMMAND_X, MAGICRB_Y, 0.0f);	// 中心位置
				// テクスチャの種類
				if (operationType == OPERATIONTYPE_KEYBOARD)
				{ // キーボード操作
					g_aMagicBubble[nCntPlayer].aMagicBubble[nCntUI].tex = MAGICBUBBLE_TEX_J;
				}
				else
				{ // ジョイパッド操作
					g_aMagicBubble[nCntPlayer].aMagicBubble[nCntUI].tex = MAGICBUBBLE_TEX_X;
				}
				g_aMagicBubble[nCntPlayer].aMagicBubble[nCntUI].type = MAGICBUBBLE_TYPE_BLUE;	// テクスチャの配置
				g_aMagicBubble[nCntPlayer].aMagicBubble[nCntUI].move = MAGICBUBBLE_MOVE_STOP;	// UIの浮遊感
				g_aMagicBubble[nCntPlayer].aMagicBubble[nCntUI].fWidth = MAGIC_SIZE;				// 幅
				g_aMagicBubble[nCntPlayer].aMagicBubble[nCntUI].fHeight = MAGIC_SIZE;				// 高さ
				g_aMagicBubble[nCntPlayer].aMagicBubble[nCntUI].fWidthDest = 0.0f;					// 幅の目的地
				g_aMagicBubble[nCntPlayer].aMagicBubble[nCntUI].fHeightDest = 0.0f;					// 高さの目的地
				g_aMagicBubble[nCntPlayer].aMagicBubble[nCntUI].nKey = 0;							// 浮遊感を演出
				g_aMagicBubble[nCntPlayer].aMagicBubble[nCntUI].nNumKey = NUM_KEY;					// 浮遊感を演出
				g_aMagicBubble[nCntPlayer].aMagicBubble[nCntUI].bDisp = true;						// 表示状態
				break;

			case MAGICBUBBLE_TYPE_YELLOW:	// 黄魔法
				g_aMagicBubble[nCntPlayer].aMagicBubble[nCntUI].pos = D3DXVECTOR3(0.0f, MAGICY_Y, 0.0f);	// 中心位置
				// テクスチャの種類
				if (operationType == OPERATIONTYPE_KEYBOARD)
				{ // キーボード操作
					g_aMagicBubble[nCntPlayer].aMagicBubble[nCntUI].tex = MAGICBUBBLE_TEX_I;
				}
				else
				{ // ジョイパッド操作
					g_aMagicBubble[nCntPlayer].aMagicBubble[nCntUI].tex = MAGICBUBBLE_TEX_Y;
				}
				g_aMagicBubble[nCntPlayer].aMagicBubble[nCntUI].type = MAGICBUBBLE_TYPE_YELLOW;	// テクスチャの配置
				g_aMagicBubble[nCntPlayer].aMagicBubble[nCntUI].move = MAGICBUBBLE_MOVE_STOP;	// UIの浮遊感
				g_aMagicBubble[nCntPlayer].aMagicBubble[nCntUI].fWidth = MAGIC_SIZE;		// 幅
				g_aMagicBubble[nCntPlayer].aMagicBubble[nCntUI].fHeight = MAGIC_SIZE;		// 高さ
				g_aMagicBubble[nCntPlayer].aMagicBubble[nCntUI].fWidthDest = 0.0f;			// 幅の目的地
				g_aMagicBubble[nCntPlayer].aMagicBubble[nCntUI].fHeightDest = 0.0f;			// 高さの目的地
				g_aMagicBubble[nCntPlayer].aMagicBubble[nCntUI].nKey = 0;					// 浮遊感を演出
				g_aMagicBubble[nCntPlayer].aMagicBubble[nCntUI].nNumKey = NUM_KEY;			// 浮遊感を演出
				g_aMagicBubble[nCntPlayer].aMagicBubble[nCntUI].bDisp = true;				// 表示状態
				break;
			}

			// 中心位置からの位置を求める
			g_aMagicBubble[nCntPlayer].aMagicBubble[nCntUI].pos += g_aMagicBubble[nCntPlayer].pos;

			// 頂点座標の設定
			pVtx[0].pos = D3DXVECTOR3(-g_aMagicBubble[nCntPlayer].aMagicBubble[nCntUI].fWidth, g_aMagicBubble[nCntPlayer].aMagicBubble[nCntUI].fHeight, MAGICBUBBLE_Z);
			pVtx[1].pos = D3DXVECTOR3(g_aMagicBubble[nCntPlayer].aMagicBubble[nCntUI].fWidth, g_aMagicBubble[nCntPlayer].aMagicBubble[nCntUI].fHeight, MAGICBUBBLE_Z);
			pVtx[2].pos = D3DXVECTOR3(-g_aMagicBubble[nCntPlayer].aMagicBubble[nCntUI].fWidth, -g_aMagicBubble[nCntPlayer].aMagicBubble[nCntUI].fHeight, MAGICBUBBLE_Z);
			pVtx[3].pos = D3DXVECTOR3(g_aMagicBubble[nCntPlayer].aMagicBubble[nCntUI].fWidth, -g_aMagicBubble[nCntPlayer].aMagicBubble[nCntUI].fHeight, MAGICBUBBLE_Z);

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

		g_aMagicBubble[0].bSpell = true;
		g_aMagicBubble[1].bSpell = true;
	}
	// 頂点バッファをアンロック
	g_pVtxBuffMagicBubble->Unlock();
}

//======================================================================================
// spellの終了処理
//======================================================================================
void UninitMagicBubble(void)
{
	// テクスチャの破棄
	for (int nCntUI = 0; nCntUI < MAXMAGICBUBBLE_TEX; nCntUI++)
	{
		if (g_apTextureMagicBubble[nCntUI] != NULL)
		{
			g_apTextureMagicBubble[nCntUI]->Release();
			g_apTextureMagicBubble[nCntUI] = NULL;
		}
	}

	// 頂点バッファの破棄
	if (g_pVtxBuffMagicBubble != NULL)
	{
		g_pVtxBuffMagicBubble->Release();
		g_pVtxBuffMagicBubble = NULL;
	}

}

//======================================================================================
// spellの更新処理
//======================================================================================
void UpdateMagicBubble(void)
{
	VERTEX_3D* pVtx;
	// 頂点バッファをロックし、頂点情報へのポインタを取得
	g_pVtxBuffMagicBubble->Lock(0, 0, (void**)&pVtx, 0);

	Player* pPlayer = GetPlayer();
	for (int nCntPlayer = 0; nCntPlayer < MAX_PLAYER; nCntPlayer++, pPlayer++)
	{
		// MAGICBUBBLEメニューを開いているかのフラグを立てる
		if ((GetKeyboardPress(DIK_TAB) == true && nCntPlayer == 0) || GetJoypadRightTriggePress(nCntPlayer) == true || GetJoypadLeftTriggePress(nCntPlayer) == true)
		{
			if ((GetKeyboardTrigger(DIK_TAB) == true && nCntPlayer == 0) || GetJoypadTrigger(JOYKEY_LEFT_TRIGGER, nCntPlayer) == true || GetJoypadTrigger(JOYKEY_RIGHT_TRIGGER, nCntPlayer) == true)
			{
				g_aMagicBubble[nCntPlayer].state = MAGICBUBBLESTATE_APPEAR;
				SetMagicBubbleAppear(nCntPlayer);
			}
			g_aMagicBubble[nCntPlayer].bSpell = true;
			pPlayer->state = PLAYERSTATE_MAGICBUBBLE;
		}
		else
		{
			if ((GetKeyboardRelease(DIK_TAB) == true && nCntPlayer == 0) || GetJoypadRelease(JOYKEY_LEFT_TRIGGER, nCntPlayer) == true || GetJoypadRelease(JOYKEY_RIGHT_TRIGGER, nCntPlayer) == true)
			{
				g_aMagicBubble[nCntPlayer].state = MAGICBUBBLESTATE_DISAPPEAR;
				SetMagicBubbleDisappear(nCntPlayer);
				ResetCommand(nCntPlayer);
				pPlayer->state = PLAYERSTATE_NORMAL;
			}
		}

		if (g_aMagicBubble[nCntPlayer].aMagicBubble[MAGICBUBBLE_TYPE_COMMAND0].bDisp == false &&
			g_aMagicBubble[nCntPlayer].aMagicBubble[MAGICBUBBLE_TYPE_COMMAND1].bDisp == false &&
			g_aMagicBubble[nCntPlayer].aMagicBubble[MAGICBUBBLE_TYPE_COMMAND2].bDisp == false)
		{ // コマンド入力が行われていない場合、セーブ内容を初期化
			ResetCommdSave(nCntPlayer);
		}


		if (g_aMagicBubble[nCntPlayer].bSpell == false)
		{
			continue;
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
				g_aMagicBubble[nCntPlayer].aMagicBubble[nCntUI].tex = MAGICBUBBLE_TEX_MAGICNULL;
				break;
			case COMMANDTYPE_R:	// 赤魔法
				g_aMagicBubble[nCntPlayer].aMagicBubble[nCntUI].tex = MAGICBUBBLE_TEX_RED;
				break;
			case COMMANDTYPE_G:	// 緑魔法
				g_aMagicBubble[nCntPlayer].aMagicBubble[nCntUI].tex = MAGICBUBBLE_TEX_GREEN;
				break;
			case COMMANDTYPE_B:	// 青魔法
				g_aMagicBubble[nCntPlayer].aMagicBubble[nCntUI].tex = MAGICBUBBLE_TEX_BLUE;
				break;
			case COMMANDTYPE_Y:	// 黄魔法
				g_aMagicBubble[nCntPlayer].aMagicBubble[nCntUI].tex = MAGICBUBBLE_TEX_YELLOW;
				break;
			}
		}

		// 全体の演出処理======================================================================================
		float fDiffKey = 0.0f;	// キーの差分を計算
		float fRateKey = (float)g_aMagicBubble[nCntPlayer].nKey / (float)g_aMagicBubble[nCntPlayer].nNumKey;
		switch (g_aMagicBubble[nCntPlayer].state)
		{
		case MAGICBUBBLESTATE_NONDISPLAY:	// 非表示状態
			g_aMagicBubble[nCntPlayer].bSpell = false;
			break;

		case MAGICBUBBLESTATE_APPEAR:	// 出現状態
			for (int nCntUI = 0; nCntUI < MAXMAGICBUBBLE_TYPE; nCntUI++)
			{
				// 背景の高度変更
				fDiffKey = g_aMagicBubble[nCntPlayer].aMagicBubble[nCntUI].fHeightDest - g_aMagicBubble[nCntPlayer].aMagicBubble[nCntUI].fHeight;
				g_aMagicBubble[nCntPlayer].aMagicBubble[nCntUI].fHeight = g_aMagicBubble[nCntPlayer].aMagicBubble[nCntUI].fHeight + fDiffKey * fRateKey;
				// 中心位置からの位置を求める
				g_aMagicBubble[nCntPlayer].aMagicBubble[nCntUI].pos += g_aMagicBubble[nCntPlayer].pos;
			}
			g_aMagicBubble[nCntPlayer].nKey++;

			if (g_aMagicBubble[nCntPlayer].nKey > g_aMagicBubble[nCntPlayer].nNumKey)
			{
				g_aMagicBubble[nCntPlayer].state = MAGICBUBBLESTATE_DISPLAY;
				SetMagicBubbleDisplay(nCntPlayer);
			}
			break;

		case MAGICBUBBLESTATE_DISPLAY:	// 表示状態

			break;

		case MAGICBUBBLESTATE_SETMAGIC:	// 魔法発動状態
			g_aMagicBubble[nCntPlayer].nCounterUI--;
			if (g_aMagicBubble[nCntPlayer].nCounterUI < 0)
			{
				g_aMagicBubble[nCntPlayer].nCounterUI = DISP_MAGIC;
				SetCommandDisappear(nCntPlayer, MAGICBUBBLE_TYPE_COMMAND0);
				SetCommandDisappear(nCntPlayer, MAGICBUBBLE_TYPE_COMMAND1);
				SetCommandDisappear(nCntPlayer, MAGICBUBBLE_TYPE_COMMAND2);
				//ResetCommdSave(nCntPlayer);
				g_aMagicBubble[nCntPlayer].state = MAGICBUBBLESTATE_DISPLAY;
			}
			break;

		case MAGICBUBBLESTATE_DISAPPEAR:	// 収縮状態
			for (int nCntUI = 0; nCntUI < MAXMAGICBUBBLE_TYPE; nCntUI++)
			{
				// 背景の高度変更
				fDiffKey = g_aMagicBubble[nCntPlayer].aMagicBubble[nCntUI].fHeightDest - g_aMagicBubble[nCntPlayer].aMagicBubble[nCntUI].fHeight;
				g_aMagicBubble[nCntPlayer].aMagicBubble[nCntUI].fHeight = g_aMagicBubble[nCntPlayer].aMagicBubble[nCntUI].fHeight + fDiffKey * fRateKey;
				// 中心位置からの位置を求める
				g_aMagicBubble[nCntPlayer].aMagicBubble[nCntUI].pos += g_aMagicBubble[nCntPlayer].pos;
			}
			g_aMagicBubble[nCntPlayer].nKey++;

			if (g_aMagicBubble[nCntPlayer].nKey > g_aMagicBubble[nCntPlayer].nNumKey)
			{
				g_aMagicBubble[nCntPlayer].state = MAGICBUBBLESTATE_DISPLAY;
				SetMagicBubbleNonDisplay(nCntPlayer);
				ResetCommdSave(nCntPlayer);
			}
			break;
		}

		// 個々の演出処理======================================================================================
		for (int nCntUI = 0; nCntUI < MAXMAGICBUBBLE_TYPE; nCntUI++, pVtx += 4)
		{
			switch (g_aMagicBubble[nCntPlayer].aMagicBubble[nCntUI].move)
			{
			case MAGICBUBBLE_MOVE_STOP:	// 停止
				g_aMagicBubble[nCntPlayer].aMagicBubble[nCntUI].bDisp = true;
				break;

			case MAGICBUBBLE_MOVE_APPEAR:	// 出現
				g_aMagicBubble[nCntPlayer].aMagicBubble[nCntUI].bDisp = true;

				fRateKey = (float)g_aMagicBubble[nCntPlayer].aMagicBubble[nCntUI].nKey / (float)g_aMagicBubble[nCntPlayer].aMagicBubble[nCntUI].nNumKey;
				// 背景の高度変更
				fDiffKey = g_aMagicBubble[nCntPlayer].aMagicBubble[nCntUI].fHeightDest - g_aMagicBubble[nCntPlayer].aMagicBubble[nCntUI].fHeight;
				g_aMagicBubble[nCntPlayer].aMagicBubble[nCntUI].fHeight = g_aMagicBubble[nCntPlayer].aMagicBubble[nCntUI].fHeight + fDiffKey * fRateKey;
				if (g_aMagicBubble[nCntPlayer].aMagicBubble[nCntUI].nKey > g_aMagicBubble[nCntPlayer].aMagicBubble[nCntUI].nNumKey)
				{
					g_aMagicBubble[nCntPlayer].aMagicBubble[nCntUI].move = MAGICBUBBLE_MOVE_STOP;
				}
				g_aMagicBubble[nCntPlayer].aMagicBubble[nCntUI].nKey++;
				break;

			case MAGICBUBBLE_MOVE_DISAPPEAR:	// 収縮
				fRateKey = (float)g_aMagicBubble[nCntPlayer].aMagicBubble[nCntUI].nKey / (float)g_aMagicBubble[nCntPlayer].aMagicBubble[nCntUI].nNumKey;
				// 背景の高度変更
				fDiffKey = g_aMagicBubble[nCntPlayer].aMagicBubble[nCntUI].fHeightDest - g_aMagicBubble[nCntPlayer].aMagicBubble[nCntUI].fHeight;
				g_aMagicBubble[nCntPlayer].aMagicBubble[nCntUI].fHeight = g_aMagicBubble[nCntPlayer].aMagicBubble[nCntUI].fHeight + fDiffKey * fRateKey;
				if (g_aMagicBubble[nCntPlayer].aMagicBubble[nCntUI].nKey > g_aMagicBubble[nCntPlayer].aMagicBubble[nCntUI].nNumKey)
				{
					g_aMagicBubble[nCntPlayer].aMagicBubble[nCntUI].move = MAGICBUBBLE_MOVE_NONDISP;
				}
				g_aMagicBubble[nCntPlayer].aMagicBubble[nCntUI].nKey++;
				break;

			case MAGICBUBBLE_MOVE_NONDISP:	// 非表示
				g_aMagicBubble[nCntPlayer].aMagicBubble[nCntUI].bDisp = false;
				break;
			}

			// 頂点座標の設定
			pVtx[0].pos = D3DXVECTOR3(-g_aMagicBubble[nCntPlayer].aMagicBubble[nCntUI].fWidth, g_aMagicBubble[nCntPlayer].aMagicBubble[nCntUI].fHeight, MAGICBUBBLE_Z);
			pVtx[1].pos = D3DXVECTOR3(g_aMagicBubble[nCntPlayer].aMagicBubble[nCntUI].fWidth, g_aMagicBubble[nCntPlayer].aMagicBubble[nCntUI].fHeight, MAGICBUBBLE_Z);
			pVtx[2].pos = D3DXVECTOR3(-g_aMagicBubble[nCntPlayer].aMagicBubble[nCntUI].fWidth, -g_aMagicBubble[nCntPlayer].aMagicBubble[nCntUI].fHeight, MAGICBUBBLE_Z);
			pVtx[3].pos = D3DXVECTOR3(g_aMagicBubble[nCntPlayer].aMagicBubble[nCntUI].fWidth, -g_aMagicBubble[nCntPlayer].aMagicBubble[nCntUI].fHeight, MAGICBUBBLE_Z);
		}
	}

	// 頂点バッファをアンロック
	g_pVtxBuffMagicBubble->Unlock();
}

//======================================================================================
// spellの描画処理
//======================================================================================
void DrawMagicBubble(void)
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
		if (g_aMagicBubble[nCntPlayer].bSpell == false)
		{
			continue;
		}
		// UIのマトリックス情報を取得
		UIMatrix = pPlayer->mtxWorld;

		// ワールドマトリックスの設定
		pDevice->SetTransform(D3DTS_WORLD, &UIMatrix);

		for (int nCntUI = 0; nCntUI < MAXMAGICBUBBLE_TYPE; nCntUI++)
		{
			D3DXMATRIX	mtxRotModel, mtxTransModel;	// 計算用マトリックス
			D3DXMATRIX	mtxParent;					// 親のマトリックス

			if (g_aMagicBubble[nCntPlayer].aMagicBubble[nCntUI].bDisp == false)
			{
				continue;
			}

			// ポリゴンのワールドマトリックスを初期化
			D3DXMatrixIdentity(&g_aMagicBubble[nCntPlayer].aMagicBubble[nCntUI].mtxWorld);

			// パーツの位置を反映
			D3DXMatrixTranslation(&mtxTransModel, g_aMagicBubble[nCntPlayer].aMagicBubble[nCntUI].pos.x, g_aMagicBubble[nCntPlayer].aMagicBubble[nCntUI].pos.y, g_aMagicBubble[nCntPlayer].aMagicBubble[nCntUI].pos.z);
			D3DXMatrixMultiply(&g_aMagicBubble[nCntPlayer].aMagicBubble[nCntUI].mtxWorld, &g_aMagicBubble[nCntPlayer].aMagicBubble[nCntUI].mtxWorld, &mtxTransModel);

			// 向きを反映
			D3DXMatrixRotationYawPitchRoll(&mtxRot, g_aMagicBubble[nCntPlayer].rot.y, g_aMagicBubble[nCntPlayer].rot.x, g_aMagicBubble[nCntPlayer].rot.z);
			D3DXMatrixMultiply(&g_aMagicBubble[nCntPlayer].aMagicBubble[nCntUI].mtxWorld, &g_aMagicBubble[nCntPlayer].aMagicBubble[nCntUI].mtxWorld, &mtxRot);

			// 親マトリックスを設定
			mtxParent = UIMatrix;

			// 算出したパーツのワールドマトリックスと親モデルのマトリックスを掛け合わせる
			D3DXMatrixMultiply(&g_aMagicBubble[nCntPlayer].aMagicBubble[nCntUI].mtxWorld, &g_aMagicBubble[nCntPlayer].aMagicBubble[nCntUI].mtxWorld, &mtxParent);

			// パーツのワールドマトリックスを設定
			pDevice->SetTransform(D3DTS_WORLD, &g_aMagicBubble[nCntPlayer].aMagicBubble[nCntUI].mtxWorld);

			// 頂点バッファをデータストリームに設定
			pDevice->SetStreamSource(0, g_pVtxBuffMagicBubble, 0, sizeof(VERTEX_3D));

			// 頂点フォーマットの設定
			pDevice->SetFVF(FVF_VERTEX_3D);

			// テクスチャの設定
			pDevice->SetTexture(0, g_apTextureMagicBubble[g_aMagicBubble[nCntPlayer].aMagicBubble[nCntUI].tex]);

			// UIの描画
			pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, nCntUI * 4 + (nCntPlayer * MAXMAGICBUBBLE_TYPE * 4), 2);
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

#endif