//========================================================
// 
// クエストのUI表示処理[questui.cpp]
// Author : KajinoHiyori
// 
//========================================================
#include "questui.h"
#include "color.h"
#include "main.h"
#include "debugproc.h"
#include "input.h"
#include "sound.h"

// テクスチャの種類
typedef enum
{
	QUESTUI_TEX_MAINQUEST = 0,	// メインクエスト
	QUESTUI_TEX_SUBQUEST,		// サブクエスト
	QUESTUI_TEX_TEMPLE,			// 神殿
	QUESTUI_TEX_SHIRINE,		// 祠
	QUESTUI_TEX_DRAGON,			// ドラゴン
	QUESTUI_TEX_FLOWER,			// 花
	QUESTUI_TEX_BLACKSMITH,		// 鍛冶場
	QUESTUI_TEX_STREETLIGHT,	// 街灯
	QUESTUI_TEX_LINE,			// クリア時の横棒
	QUESTUI_TEX_MAX
}QUESTUI_TEX;

// UIのステータスを管理
typedef enum
{
	QUESTSTATE_DISP = 0,	// 表示状態[未クリア]
	QUESTSTATE_CLEAR,		// クリア状態[線が引かれている]
	QUESTSTATE_ADDALPHA,	// 透明度を上げる[祠クエスト完了時に表示]
	QUESTSTATE_MOVESIDE,	// 端にはける[同時に透明度も上げていく]
	QUESTSTATE_MOVEUP,		// 上に移動
	QUESTSTATE_NONDISP,		// 非表示状態
	QUESTSTATE_MAX
}QUESTSTATE;

// クエストの構造体
typedef struct
{
	QUESTTYPE	type;		// クエストの種類
	QUESTSTATE	state;		// クエストの状態
	bool		bClear;		// クリア状態
}Quest;

// クエストUIの構造体
typedef struct
{
	QUESTUI_TEX		tex;		// テクスチャの種類
	D3DXVECTOR3		pos;		// 現在の中心位置
	D3DXVECTOR3		posDest;	// 目的の表示位置
	D3DXVECTOR3		posOffset;	// 元々の位置
	D3DXCOLOR		col;		// 色
	float	fHeight;		// 文字部分の高さ
	float	fWidth;			// 文字部分の幅
	float	fWidthDest;		// 完了線の目的の幅
	int		nKey;			// 現在処理しているフレーム数
	int		nNumKey;		// 処理を行うフレーム数
	bool	bDisp;			// 表示状態
}QuestUI;

// マクロ定義
#define NUM_QUEST			(QUESTTYPE_MAX)	// クエストの総数
#define NUM_UI				(NUM_QUEST * 2 + 2)		// 必要なポリゴン数
#define MAX_QUESTUI			(NUM_QUEST + 2)			// 必要な構造体数
#define MAX_QUESTUITEX		(QUESTUI_TEX_MAX)		// クエストUI用テクスチャの最大数
#define MAX_MAINQUEST		(2)			// メインクエストの総数
#define MAX_SUBQUEST		(4)			// サブクエストの総数
#define NUM_KEY				(60)		// 処理を行うキー数
#define MAINQUEST_HEIGHT	(56.0f)		// メインクエストの高さ[0.8倍]
#define MAIN_HEIGHT			(42.0f)		// メインの高さ[0.6倍]
#define SUBQUEST_HEIGHT		(56.0f)		// サブクエストの高さ[0.8倍]
#define SUB_HEIGHT			(35.0f)		// サブの高さ[0.5倍]
#define MAINQUEST_WIDTH		(360.0f)	// メインクエストの幅
#define TEMPLE_WIDTH		(300.0f)	// 神殿クエストの幅
#define SHIRINE_WIDTH		(480.0f)	// 祠クエストの幅
#define SUBQUEST_WIDTH		(320.0f)	// サブクエストの幅
#define DRAGON_WIDTH		(350.0f)	// ドラゴンの幅
#define FLOWER_WIDTH		(250.0f)	// 花クエストの幅
#define SMITH_WIDTH			(350.0f)	// 鍛冶場クエストの幅
#define LIGHT_WIDTH			(350.0f)	// 街灯クエストの幅
#define MAIN_POSX			(10.0f)		// メインクエストのX座標
#define LEFTSIDE_POSX		(-490.0f)	// 左にはけた際のX座標
#define RIGHTSIDE_POSX		(1290.0f)	// 右にはけた際のX座標
#define SUB_POSX			(920.0f)	// サブクエストのX座標
#define MAINQUEST_POS		(D3DXVECTOR3(MAIN_POSX, 10.0f, 0.0f))	// メインクエストの位置
#define TEMPLE_POS			(D3DXVECTOR3(MAIN_POSX, 80.0f, 0.0f))	// 神殿クエストの位置
#define SHIRINE_POS			(D3DXVECTOR3(MAIN_POSX, 140.0f, 0.0f))	// 祠クエストの位置
#define SUBQUEST_POS		(D3DXVECTOR3(SUB_POSX, 10.0f, 0.0f))	// サブクエストの位置
#define DRAGON_POS			(D3DXVECTOR3(SUB_POSX, 80.0f, 0.0f))	// ドラゴンの位置
#define FLOWER_POS			(D3DXVECTOR3(SUB_POSX, 120.0f, 0.0f))	// 花クエストの位置
#define SMITH_POS			(D3DXVECTOR3(SUB_POSX, 160.0f, 0.0f))	// 鍛冶場クエストの位置
#define LIGHT_POS			(D3DXVECTOR3(SUB_POSX, 200.0f, 0.0f))	// 街灯クエストの位置

// テクスチャの読み込み
const char* c_apFilenameQuestUI[MAX_QUESTUITEX] =
{
	"data\\TEXTURE\\quest\\quest000.png",	// メインクエスト
	"data\\TEXTURE\\quest\\quest001.png",	// サブクエスト
	"data\\TEXTURE\\quest\\quest002.png",	// 神殿
	"data\\TEXTURE\\quest\\quest003.png",	// 祠
	"data\\TEXTURE\\quest\\quest004.png",	// ドラゴン
	"data\\TEXTURE\\quest\\quest005.png",	// 花
	"data\\TEXTURE\\quest\\quest006.png",	// 鍛冶場
	"data\\TEXTURE\\quest\\quest007.png",	// 街灯
	"data\\TEXTURE\\quest\\quest100.png",	// クリア時の横棒
};

// グローバル変数
QuestUI	g_aQuestUI[NUM_UI];	// クエストUIの構造体
Quest	g_aQuest[NUM_QUEST];		// クエストの構造体
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffQuestUI = NULL;
LPDIRECT3DTEXTURE9		g_apTextureQuestUI[MAX_QUESTUITEX] = {};

//======================================================================================
// クエストUIの初期化処理
//======================================================================================
void InitQuestUI(void)
{
	// デバイスの取得
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	// テクスチャの読み込み
	for (int nCntUI = 0; nCntUI < MAX_QUESTUITEX; nCntUI++)
	{
		D3DXCreateTextureFromFile(pDevice, c_apFilenameQuestUI[nCntUI], &g_apTextureQuestUI[nCntUI]);
	}

	// クエストの構造体
	for (int nCntQuest = 0; nCntQuest < NUM_QUEST; nCntQuest++)
	{
		switch (nCntQuest)
		{
		case QUESTTYPE_TEMPLE:	// 神殿
			g_aQuest[nCntQuest].state = QUESTSTATE_DISP;
			g_aQuest[nCntQuest].type = QUESTTYPE_TEMPLE;
			break;

		case QUESTTYPE_SHIRINE:	// 祠
			g_aQuest[nCntQuest].state = QUESTSTATE_DISP;
			g_aQuest[nCntQuest].type = QUESTTYPE_SHIRINE;
			break;

		case QUESTTYPE_DRAGON:	// ドラゴン
			g_aQuest[nCntQuest].state = QUESTSTATE_DISP;
			g_aQuest[nCntQuest].type = QUESTTYPE_DRAGON;
			break;

		case QUESTTYPE_FLOWER:	// 花
			g_aQuest[nCntQuest].state = QUESTSTATE_DISP;
			g_aQuest[nCntQuest].type = QUESTTYPE_FLOWER;
			break;

		case QUESTTYPE_BLACKSMITH:	// 鍛冶場
			g_aQuest[nCntQuest].state = QUESTSTATE_DISP;
			g_aQuest[nCntQuest].type = QUESTTYPE_BLACKSMITH;
			break;

		case QUESTTYPE_STREETLIGHT:	// 街灯
			g_aQuest[nCntQuest].state = QUESTSTATE_DISP;
			g_aQuest[nCntQuest].type = QUESTTYPE_STREETLIGHT;
			break;
		}
		g_aQuest[nCntQuest].bClear = false;	// クリア状態じゃなくする
	}

	// UIの構造体
	for (int nCntUI = 0; nCntUI < NUM_UI; nCntUI++)
	{
		g_aQuestUI[nCntUI].bDisp = true;		// 表示状態をtrueにする
		g_aQuestUI[nCntUI].nKey = 0;			// キー数の初期化
		g_aQuestUI[nCntUI].nNumKey = NUM_KEY;	// キー総数を決定
	
		switch (nCntUI)
		{
		case 0:	// メインクエスト
			g_aQuestUI[nCntUI].col			= COLOR_ORANGE;
			g_aQuestUI[nCntUI].fHeight		= MAINQUEST_HEIGHT;
			g_aQuestUI[nCntUI].fWidth		= MAINQUEST_WIDTH;
			g_aQuestUI[nCntUI].fWidthDest	= MAINQUEST_WIDTH;
			g_aQuestUI[nCntUI].pos			= MAINQUEST_POS;
			g_aQuestUI[nCntUI].posDest		= MAINQUEST_POS;
			g_aQuestUI[nCntUI].posOffset	= MAINQUEST_POS;
			g_aQuestUI[nCntUI].tex			= QUESTUI_TEX_MAINQUEST;
			break;

		case 1:	// 神殿クエスト
			g_aQuestUI[nCntUI].col			= COLOR_WHITE;
			g_aQuestUI[nCntUI].fHeight		= MAIN_HEIGHT;
			g_aQuestUI[nCntUI].fWidth		= TEMPLE_WIDTH;
			g_aQuestUI[nCntUI].fWidthDest	= TEMPLE_WIDTH;
			g_aQuestUI[nCntUI].pos			= TEMPLE_POS;
			g_aQuestUI[nCntUI].posDest		= TEMPLE_POS;
			g_aQuestUI[nCntUI].posOffset	= TEMPLE_POS;
			g_aQuestUI[nCntUI].tex			= QUESTUI_TEX_TEMPLE;
			break;

		case 2:	// 神殿クエストの横線
			g_aQuestUI[nCntUI].col			= COLOR_WHITE;
			g_aQuestUI[nCntUI].fHeight		= MAIN_HEIGHT;
			g_aQuestUI[nCntUI].fWidth		= 0.0f;
			g_aQuestUI[nCntUI].fWidthDest	= TEMPLE_WIDTH;
			g_aQuestUI[nCntUI].pos			= TEMPLE_POS;
			g_aQuestUI[nCntUI].posDest		= TEMPLE_POS;
			g_aQuestUI[nCntUI].posOffset	= TEMPLE_POS;
			g_aQuestUI[nCntUI].tex			= QUESTUI_TEX_LINE;
			break;

		case 3:	// 祠クエスト
			g_aQuestUI[nCntUI].col			= COLOR_WHITE;
			g_aQuestUI[nCntUI].col.a		= 0.0f;			// 透明度を上げて見えないように対応
			g_aQuestUI[nCntUI].fHeight		= MAIN_HEIGHT;
			g_aQuestUI[nCntUI].fWidth		= SHIRINE_WIDTH;
			g_aQuestUI[nCntUI].fWidthDest	= SHIRINE_WIDTH;
			g_aQuestUI[nCntUI].pos			= SHIRINE_POS;
			g_aQuestUI[nCntUI].posDest		= SHIRINE_POS;
			g_aQuestUI[nCntUI].posOffset	= SHIRINE_POS;
			g_aQuestUI[nCntUI].tex			= QUESTUI_TEX_SHIRINE;
			break;

		case 4:	// 祠クエストの横線
			g_aQuestUI[nCntUI].col			= COLOR_WHITE;
			g_aQuestUI[nCntUI].fHeight		= MAIN_HEIGHT;
			g_aQuestUI[nCntUI].fWidth		= 0.0f;
			g_aQuestUI[nCntUI].fWidthDest	= SHIRINE_WIDTH;
			g_aQuestUI[nCntUI].pos			= SHIRINE_POS;
			g_aQuestUI[nCntUI].posDest		= SHIRINE_POS;
			g_aQuestUI[nCntUI].posOffset	= SHIRINE_POS;
			g_aQuestUI[nCntUI].tex			= QUESTUI_TEX_LINE;
			break;

		case 5:	// サブクエスト
			g_aQuestUI[nCntUI].col			= COLOR_CYAN;
			g_aQuestUI[nCntUI].fHeight		= SUBQUEST_HEIGHT;
			g_aQuestUI[nCntUI].fWidth		= SUBQUEST_WIDTH;
			g_aQuestUI[nCntUI].fWidthDest	= SUBQUEST_WIDTH;
			g_aQuestUI[nCntUI].pos			= SUBQUEST_POS;
			g_aQuestUI[nCntUI].posDest		= SUBQUEST_POS;
			g_aQuestUI[nCntUI].posOffset	= SUBQUEST_POS;
			g_aQuestUI[nCntUI].tex			= QUESTUI_TEX_SUBQUEST;
			break;

		case 6:	// ドラゴン
			g_aQuestUI[nCntUI].col			= COLOR_WHITE;
			g_aQuestUI[nCntUI].fHeight		= SUB_HEIGHT;
			g_aQuestUI[nCntUI].fWidth		= DRAGON_WIDTH;
			g_aQuestUI[nCntUI].fWidthDest	= DRAGON_WIDTH;
			g_aQuestUI[nCntUI].pos			= DRAGON_POS;
			g_aQuestUI[nCntUI].posDest		= DRAGON_POS;
			g_aQuestUI[nCntUI].posOffset	= DRAGON_POS;
			g_aQuestUI[nCntUI].tex			= QUESTUI_TEX_DRAGON;
			break;

		case 7:	// ドラゴンの横線
			g_aQuestUI[nCntUI].col			= COLOR_WHITE;
			g_aQuestUI[nCntUI].fHeight		= SUB_HEIGHT;
			g_aQuestUI[nCntUI].fWidth		= 0.0f;
			g_aQuestUI[nCntUI].fWidthDest	= DRAGON_WIDTH;
			g_aQuestUI[nCntUI].pos			= DRAGON_POS;
			g_aQuestUI[nCntUI].posDest		= DRAGON_POS;
			g_aQuestUI[nCntUI].posOffset	= DRAGON_POS;
			g_aQuestUI[nCntUI].tex			= QUESTUI_TEX_LINE;
			break;

		case 8:	// 花クエスト
			g_aQuestUI[nCntUI].col			= COLOR_WHITE;
			g_aQuestUI[nCntUI].fHeight		= SUB_HEIGHT;
			g_aQuestUI[nCntUI].fWidth		= FLOWER_WIDTH;
			g_aQuestUI[nCntUI].fWidthDest	= FLOWER_WIDTH;
			g_aQuestUI[nCntUI].pos			= FLOWER_POS;
			g_aQuestUI[nCntUI].posDest		= FLOWER_POS;
			g_aQuestUI[nCntUI].posOffset	= FLOWER_POS;
			g_aQuestUI[nCntUI].tex			= QUESTUI_TEX_FLOWER;
			break;

		case 9:	// 花クエストの横線
			g_aQuestUI[nCntUI].col			= COLOR_WHITE;
			g_aQuestUI[nCntUI].fHeight		= SUB_HEIGHT;
			g_aQuestUI[nCntUI].fWidth		= 0.0f;
			g_aQuestUI[nCntUI].fWidthDest	= FLOWER_WIDTH;
			g_aQuestUI[nCntUI].pos			= FLOWER_POS;
			g_aQuestUI[nCntUI].posDest		= FLOWER_POS;
			g_aQuestUI[nCntUI].posOffset	= FLOWER_POS;
			g_aQuestUI[nCntUI].tex			= QUESTUI_TEX_LINE;
			break;

		case 10:	// 鍛冶場クエスト
			g_aQuestUI[nCntUI].col			= COLOR_WHITE;
			g_aQuestUI[nCntUI].fHeight		= SUB_HEIGHT;
			g_aQuestUI[nCntUI].fWidth		= SMITH_WIDTH;
			g_aQuestUI[nCntUI].fWidthDest	= SMITH_WIDTH;
			g_aQuestUI[nCntUI].pos			= SMITH_POS;
			g_aQuestUI[nCntUI].posDest		= SMITH_POS;
			g_aQuestUI[nCntUI].posOffset	= SMITH_POS;
			g_aQuestUI[nCntUI].tex			= QUESTUI_TEX_BLACKSMITH;
			break;

		case 11:	// 鍛冶場クエストの横線
			g_aQuestUI[nCntUI].col			= COLOR_WHITE;
			g_aQuestUI[nCntUI].fHeight		= SUB_HEIGHT;
			g_aQuestUI[nCntUI].fWidth		= 0.0f;
			g_aQuestUI[nCntUI].fWidthDest	= SMITH_WIDTH;
			g_aQuestUI[nCntUI].pos			= SMITH_POS;
			g_aQuestUI[nCntUI].posDest		= SMITH_POS;
			g_aQuestUI[nCntUI].posOffset	= SMITH_POS;
			g_aQuestUI[nCntUI].tex			= QUESTUI_TEX_LINE;
			break;

		case 12:	// 街灯クエスト
			g_aQuestUI[nCntUI].col			= COLOR_WHITE;
			g_aQuestUI[nCntUI].fHeight		= SUB_HEIGHT;
			g_aQuestUI[nCntUI].fWidth		= LIGHT_WIDTH;
			g_aQuestUI[nCntUI].fWidthDest	= LIGHT_WIDTH;
			g_aQuestUI[nCntUI].pos			= LIGHT_POS;
			g_aQuestUI[nCntUI].posDest		= LIGHT_POS;
			g_aQuestUI[nCntUI].posOffset	= LIGHT_POS;
			g_aQuestUI[nCntUI].tex			= QUESTUI_TEX_STREETLIGHT;
			break;

		case 13:	// 街灯クエストの横線
			g_aQuestUI[nCntUI].col			= COLOR_WHITE;
			g_aQuestUI[nCntUI].fHeight		= SUB_HEIGHT;
			g_aQuestUI[nCntUI].fWidth		= 0.0f;
			g_aQuestUI[nCntUI].fWidthDest	= LIGHT_WIDTH;
			g_aQuestUI[nCntUI].pos			= LIGHT_POS;
			g_aQuestUI[nCntUI].posDest		= LIGHT_POS;
			g_aQuestUI[nCntUI].posOffset	= LIGHT_POS;
			g_aQuestUI[nCntUI].tex			= QUESTUI_TEX_LINE;
			break;
		}
	}

	// 頂点バッファの生成
	pDevice->CreateVertexBuffer(sizeof(VERTEX_2D) * 4 * NUM_UI, D3DUSAGE_WRITEONLY, FVF_VERTEX_2D, D3DPOOL_MANAGED, &g_pVtxBuffQuestUI, NULL);

	VERTEX_2D* pVtx;
	// 頂点バッファをロックし、頂点情報へのポインタを取得
	g_pVtxBuffQuestUI->Lock(0, 0, (void**)&pVtx, 0);

	for (int nCntUI = 0; nCntUI < NUM_UI; nCntUI++, pVtx += 4)
	{
		// 位置の設定
		pVtx[0].pos = D3DXVECTOR3(g_aQuestUI[nCntUI].pos.x,								g_aQuestUI[nCntUI].pos.y,								0.0f);
		pVtx[1].pos = D3DXVECTOR3(g_aQuestUI[nCntUI].pos.x + g_aQuestUI[nCntUI].fWidth, g_aQuestUI[nCntUI].pos.y,								0.0f);
		pVtx[2].pos = D3DXVECTOR3(g_aQuestUI[nCntUI].pos.x,								g_aQuestUI[nCntUI].pos.y + g_aQuestUI[nCntUI].fHeight,	0.0f);
		pVtx[3].pos = D3DXVECTOR3(g_aQuestUI[nCntUI].pos.x + g_aQuestUI[nCntUI].fWidth, g_aQuestUI[nCntUI].pos.y + g_aQuestUI[nCntUI].fHeight,	0.0f);

		// rhwの設定
		pVtx[0].rhw = 1.0f;
		pVtx[1].rhw = 1.0f;
		pVtx[2].rhw = 1.0f;
		pVtx[3].rhw = 1.0f;

		// 色の設定
		pVtx[0].col = g_aQuestUI[nCntUI].col;
		pVtx[1].col = g_aQuestUI[nCntUI].col;
		pVtx[2].col = g_aQuestUI[nCntUI].col;
		pVtx[3].col = g_aQuestUI[nCntUI].col;

		// テクスチャ座標の設定
		pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
		pVtx[1].tex = D3DXVECTOR2(1.0f, 0.0f);
		pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
		pVtx[3].tex = D3DXVECTOR2(1.0f, 1.0f);
	}

	// 頂点バッファをアンロック
	g_pVtxBuffQuestUI->Unlock();
}

//======================================================================================
// クエストUIの初期化処理
//======================================================================================
void UninitQuestUI(void)
{
	// テクスチャの破棄
	for (int nCntUI = 0; nCntUI < MAX_QUESTUITEX; nCntUI++)
	{
		if (g_apTextureQuestUI[nCntUI] != NULL)
		{
			g_apTextureQuestUI[nCntUI]->Release();
			g_apTextureQuestUI[nCntUI] = NULL;
		}
	}

	// 頂点バッファの破棄
	if (g_pVtxBuffQuestUI != NULL)
	{
		g_pVtxBuffQuestUI->Release();
		g_pVtxBuffQuestUI = NULL;
	}
}

//======================================================================================
// クエストUIの初期化処理
//======================================================================================
void UpdateQuestUI(void)
{
	// クエストの位置更新
	UpdateQuestPos();

	// UIの更新
	for (int nCntUI = 0; nCntUI < NUM_UI; nCntUI++)
	{
		if (g_aQuestUI[nCntUI].bDisp == false)
		{
			continue;
		}

		switch (nCntUI)
		{
		case 0:	// メインクエスト
			
			break;

		case 1:	// 神殿クエスト
			UpdateQuest(nCntUI, QUESTTYPE_TEMPLE);
			break;

		case 2:	// 神殿クエストの横線
			UpdateLine(nCntUI, QUESTTYPE_TEMPLE);
			break;

		case 3:	// 祠クエスト
			UpdateQuest(nCntUI, QUESTTYPE_SHIRINE);
			break;

		case 4:	// 祠クエストの横線
			UpdateLine(nCntUI, QUESTTYPE_SHIRINE);
			break;

		case 5:	// サブクエスト
			
			break;

		case 6:	// ドラゴン
			UpdateQuest(nCntUI, QUESTTYPE_DRAGON);
			break;

		case 7:	// ドラゴンの横線
			UpdateLine(nCntUI, QUESTTYPE_DRAGON);
			break;

		case 8:	// 花クエスト
			UpdateQuest(nCntUI, QUESTTYPE_FLOWER);
			break;

		case 9:	// 花クエストの横線
			UpdateLine(nCntUI, QUESTTYPE_FLOWER);
			break;

		case 10:	// 鍛冶場クエスト
			UpdateQuest(nCntUI, QUESTTYPE_BLACKSMITH);
			break;

		case 11:	// 鍛冶場クエストの横線
			UpdateLine(nCntUI, QUESTTYPE_BLACKSMITH);
			break;

		case 12:	// 街灯クエスト
			UpdateQuest(nCntUI, QUESTTYPE_STREETLIGHT);
			break;

		case 13:	// 街灯クエストの横線
			UpdateLine(nCntUI, QUESTTYPE_STREETLIGHT);
			break;
		}
	}

	VERTEX_2D* pVtx;
	// 頂点バッファをロックし、頂点情報へのポインタを取得
	g_pVtxBuffQuestUI->Lock(0, 0, (void**)&pVtx, 0);

	for (int nCntUI = 0; nCntUI < NUM_UI; nCntUI++, pVtx += 4)
	{
		// 位置の設定
		pVtx[0].pos = D3DXVECTOR3(g_aQuestUI[nCntUI].pos.x, g_aQuestUI[nCntUI].pos.y, 0.0f);
		pVtx[1].pos = D3DXVECTOR3(g_aQuestUI[nCntUI].pos.x + g_aQuestUI[nCntUI].fWidth, g_aQuestUI[nCntUI].pos.y, 0.0f);
		pVtx[2].pos = D3DXVECTOR3(g_aQuestUI[nCntUI].pos.x, g_aQuestUI[nCntUI].pos.y + g_aQuestUI[nCntUI].fHeight, 0.0f);
		pVtx[3].pos = D3DXVECTOR3(g_aQuestUI[nCntUI].pos.x + g_aQuestUI[nCntUI].fWidth, g_aQuestUI[nCntUI].pos.y + g_aQuestUI[nCntUI].fHeight, 0.0f);

		// rhwの設定
		pVtx[0].rhw = 1.0f;
		pVtx[1].rhw = 1.0f;
		pVtx[2].rhw = 1.0f;
		pVtx[3].rhw = 1.0f;

		// 色の設定
		pVtx[0].col = g_aQuestUI[nCntUI].col;
		pVtx[1].col = g_aQuestUI[nCntUI].col;
		pVtx[2].col = g_aQuestUI[nCntUI].col;
		pVtx[3].col = g_aQuestUI[nCntUI].col;

		// テクスチャ座標の設定
		pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
		pVtx[1].tex = D3DXVECTOR2(1.0f, 0.0f);
		pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
		pVtx[3].tex = D3DXVECTOR2(1.0f, 1.0f);
	}

	// 頂点バッファをアンロック
	g_pVtxBuffQuestUI->Unlock();
}

//======================================================================================
// クエストUIの初期化処理
//======================================================================================
void DrawQuestUI(void)
{
	LPDIRECT3DDEVICE9 pDevice;	// デバイスへのポインタ
	// デバイスの取得
	pDevice = GetDevice();

	// 頂点バッファをデータストリームに設定
	pDevice->SetStreamSource(0, g_pVtxBuffQuestUI, 0, sizeof(VERTEX_2D));

	// 頂点フォーマットの設定
	pDevice->SetFVF(FVF_VERTEX_2D);

	for (int nCntUI = 0; nCntUI < NUM_UI; nCntUI++)
	{ // UIが使用されている場合
		if (g_aQuestUI[nCntUI].bDisp == true)
		{
			// テクスチャの設定
			pDevice->SetTexture(0, g_apTextureQuestUI[g_aQuestUI[nCntUI].tex]);
			// ポリゴンの描画
			pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, nCntUI * 4, 2);
		}
	}
}

//======================================================================================
// クエストのクリア状態を設定
//======================================================================================
void ClearQuest(QUESTTYPE questType)
{
	if (g_aQuest[questType].bClear == true)
	{ // そのクエストがクリア状態であればリターンする
		return;
	}

	switch (questType)
	{
	case QUESTTYPE_TEMPLE:	// 神殿
		g_aQuest[QUESTTYPE_TEMPLE].bClear = true;
		g_aQuest[QUESTTYPE_TEMPLE].state = QUESTSTATE_CLEAR;
		break;

	case QUESTTYPE_SHIRINE:	// 祠
		g_aQuest[QUESTTYPE_SHIRINE].bClear = true;
		g_aQuest[QUESTTYPE_SHIRINE].state = QUESTSTATE_ADDALPHA;
		break;

	case QUESTTYPE_DRAGON:	// ドラゴン
		g_aQuest[QUESTTYPE_DRAGON].bClear = true;
		g_aQuest[QUESTTYPE_DRAGON].state = QUESTSTATE_CLEAR;
		break;

	case QUESTTYPE_FLOWER:	// 花
		g_aQuest[QUESTTYPE_FLOWER].bClear = true;
		g_aQuest[QUESTTYPE_FLOWER].state = QUESTSTATE_CLEAR;
		break;

	case QUESTTYPE_BLACKSMITH:	// 鍛冶場
		g_aQuest[QUESTTYPE_BLACKSMITH].bClear = true;
		g_aQuest[QUESTTYPE_BLACKSMITH].state = QUESTSTATE_CLEAR;
		break;

	case QUESTTYPE_STREETLIGHT:	// 街灯
		g_aQuest[QUESTTYPE_STREETLIGHT].bClear = true;
		g_aQuest[QUESTTYPE_STREETLIGHT].state = QUESTSTATE_CLEAR;
		break;
	}
	MODE mode = GetMode();
	if (mode == MODE_GAME)
	{
		PlaySound(SOUND_LABEL_QUESTCLEAR);
	}
}

//======================================================================================
// クエストの位置更新を管理
//======================================================================================
void UpdateQuestPos(void)
{
	// メインクエストの位置管理は行わない

	// サブクエストの位置更新
	// 3つクリア済の場合===========
	if (g_aQuest[QUESTTYPE_DRAGON].state == QUESTSTATE_NONDISP && g_aQuest[QUESTTYPE_FLOWER].state == QUESTSTATE_NONDISP && g_aQuest[QUESTTYPE_BLACKSMITH].state == QUESTSTATE_NONDISP 
		&& g_aQuest[QUESTTYPE_STREETLIGHT].state == QUESTSTATE_DISP)
	{ // ドラゴン / 花 / 鍛冶場 がクリア状態 && 街灯が未クリア
		g_aQuestUI[12].posDest = DRAGON_POS;	// 街灯クエスト
		g_aQuestUI[13].posDest = DRAGON_POS;	// 街灯クエストの横線
		g_aQuest[QUESTTYPE_STREETLIGHT].state = QUESTSTATE_MOVEUP;
	}
	else if (g_aQuest[QUESTTYPE_DRAGON].state == QUESTSTATE_NONDISP && g_aQuest[QUESTTYPE_FLOWER].state == QUESTSTATE_NONDISP && g_aQuest[QUESTTYPE_STREETLIGHT].state == QUESTSTATE_NONDISP 
		&& g_aQuest[QUESTTYPE_BLACKSMITH].state == QUESTSTATE_DISP)
	{ // ドラゴン / 花 / 街灯 がクリア状態 && 鍛冶場が未クリア
		g_aQuestUI[10].posDest = DRAGON_POS;	// 鍛冶場クエスト
		g_aQuestUI[11].posDest = DRAGON_POS;	// 鍛冶場クエストの横線
		g_aQuest[QUESTTYPE_BLACKSMITH].state = QUESTSTATE_MOVEUP;
	}
	else if (g_aQuest[QUESTTYPE_DRAGON].state == QUESTSTATE_NONDISP && g_aQuest[QUESTTYPE_BLACKSMITH].state == QUESTSTATE_NONDISP && g_aQuest[QUESTTYPE_STREETLIGHT].state == QUESTSTATE_NONDISP
		&& g_aQuest[QUESTTYPE_FLOWER].state == QUESTSTATE_DISP)
	{ // ドラゴン / 鍛冶場 / 街灯 がクリア状態 && 花が未クリア
		g_aQuestUI[8].posDest = DRAGON_POS;	// 花クエスト
		g_aQuestUI[9].posDest = DRAGON_POS;	// 花クエストの横線
		g_aQuest[QUESTTYPE_FLOWER].state = QUESTSTATE_MOVEUP;
	}
	else if (g_aQuest[QUESTTYPE_FLOWER].state == QUESTSTATE_NONDISP && g_aQuest[QUESTTYPE_BLACKSMITH].state == QUESTSTATE_NONDISP && g_aQuest[QUESTTYPE_STREETLIGHT].state == QUESTSTATE_NONDISP
		&& g_aQuest[QUESTTYPE_DRAGON].state == QUESTSTATE_DISP)
	{ // 花 / 鍛冶場 / 街灯 がクリア状態 && ドラゴンが未クリア
		// ドラゴンは1番上にあるクエストのため、処理を行わない
	}

	// 2つクリア済の場合===========
	else if (g_aQuest[QUESTTYPE_DRAGON].state == QUESTSTATE_NONDISP && g_aQuest[QUESTTYPE_FLOWER].state == QUESTSTATE_NONDISP
		&& g_aQuest[QUESTTYPE_BLACKSMITH].state == QUESTSTATE_DISP && g_aQuest[QUESTTYPE_STREETLIGHT].state == QUESTSTATE_DISP)
	{ // ドラゴン / 花 がクリア状態 && 鍛冶場 / 街灯が未クリア
		g_aQuestUI[10].posDest = DRAGON_POS;	// 鍛冶場クエスト
		g_aQuestUI[11].posDest = DRAGON_POS;	// 鍛冶場クエストの横線
		g_aQuest[QUESTTYPE_BLACKSMITH].state = QUESTSTATE_MOVEUP;
		g_aQuestUI[12].posDest = FLOWER_POS;	// 街灯クエスト
		g_aQuestUI[13].posDest = FLOWER_POS;	// 街灯クエストの横線
		g_aQuest[QUESTTYPE_STREETLIGHT].state = QUESTSTATE_MOVEUP;
	}
	else if (g_aQuest[QUESTTYPE_DRAGON].state == QUESTSTATE_NONDISP && g_aQuest[QUESTTYPE_BLACKSMITH].state == QUESTSTATE_NONDISP
		&& g_aQuest[QUESTTYPE_FLOWER].state == QUESTSTATE_DISP && g_aQuest[QUESTTYPE_STREETLIGHT].state == QUESTSTATE_DISP)
	{ // ドラゴン / 鍛冶場 がクリア状態 && 花 / 街灯が未クリア状態
		g_aQuestUI[8].posDest = DRAGON_POS;	// 花クエスト
		g_aQuestUI[9].posDest = DRAGON_POS;	// 花クエストの横線
		g_aQuest[QUESTTYPE_FLOWER].state = QUESTSTATE_MOVEUP;
		g_aQuestUI[12].posDest = FLOWER_POS;	// 街灯クエスト
		g_aQuestUI[13].posDest = FLOWER_POS;	// 街灯クエストの横線
		g_aQuest[QUESTTYPE_STREETLIGHT].state = QUESTSTATE_MOVEUP;
	}
	else if (g_aQuest[QUESTTYPE_DRAGON].state == QUESTSTATE_NONDISP && g_aQuest[QUESTTYPE_STREETLIGHT].state == QUESTSTATE_NONDISP
		&& g_aQuest[QUESTTYPE_FLOWER].state == QUESTSTATE_DISP && g_aQuest[QUESTTYPE_BLACKSMITH].state == QUESTSTATE_DISP)
	{ // ドラゴン / 街灯 がクリア状態 && 花 / 鍛冶場が未クリア状態
		g_aQuestUI[8].posDest = DRAGON_POS;	// 花クエスト
		g_aQuestUI[9].posDest = DRAGON_POS;	// 花クエストの横線
		g_aQuest[QUESTTYPE_FLOWER].state = QUESTSTATE_MOVEUP;
		g_aQuestUI[10].posDest = FLOWER_POS;	// 鍛冶場クエスト
		g_aQuestUI[11].posDest = FLOWER_POS;	// 鍛冶場クエストの横線
		g_aQuest[QUESTTYPE_BLACKSMITH].state = QUESTSTATE_MOVEUP;
	}
	else if (g_aQuest[QUESTTYPE_FLOWER].state == QUESTSTATE_NONDISP && g_aQuest[QUESTTYPE_BLACKSMITH].state == QUESTSTATE_NONDISP
		&& g_aQuest[QUESTTYPE_DRAGON].state == QUESTSTATE_DISP && g_aQuest[QUESTTYPE_STREETLIGHT].state == QUESTSTATE_DISP)
	{ // 花 / 鍛冶場 がクリア状態 && ドラゴン / 街灯が未クリア状態
		g_aQuestUI[12].posDest = FLOWER_POS;	// 街灯クエスト
		g_aQuestUI[13].posDest = FLOWER_POS;	// 街灯クエストの横線
		g_aQuest[QUESTTYPE_STREETLIGHT].state = QUESTSTATE_MOVEUP;
	}
	else if (g_aQuest[QUESTTYPE_FLOWER].state == QUESTSTATE_NONDISP && g_aQuest[QUESTTYPE_STREETLIGHT].state == QUESTSTATE_NONDISP
		&& g_aQuest[QUESTTYPE_DRAGON].state == QUESTSTATE_DISP && g_aQuest[QUESTTYPE_BLACKSMITH].state == QUESTSTATE_DISP)
	{ // 花 / 街灯 がクリア状態 && ドラゴン / 鍛冶場が未クリア状態
		g_aQuestUI[10].posDest = FLOWER_POS;	// 鍛冶場クエスト
		g_aQuestUI[11].posDest = FLOWER_POS;	// 鍛冶場クエストの横線
		g_aQuest[QUESTTYPE_BLACKSMITH].state = QUESTSTATE_MOVEUP;
	}
	else if (g_aQuest[QUESTTYPE_BLACKSMITH].state == QUESTSTATE_NONDISP && g_aQuest[QUESTTYPE_STREETLIGHT].state == QUESTSTATE_NONDISP
		&& g_aQuest[QUESTTYPE_DRAGON].state == QUESTSTATE_DISP && g_aQuest[QUESTTYPE_FLOWER].state == QUESTSTATE_DISP)
	{ // 鍛冶場 / 街灯 がクリア状態 && ドラゴン / 花が未クリア状態
		// どちらも上にあるため処理を行わない
	}

	// 1つクリア済の場合===========
	else if (g_aQuest[QUESTTYPE_DRAGON].state == QUESTSTATE_NONDISP
		&& g_aQuest[QUESTTYPE_FLOWER].state == QUESTSTATE_DISP && g_aQuest[QUESTTYPE_BLACKSMITH].state == QUESTSTATE_DISP && g_aQuest[QUESTTYPE_STREETLIGHT].state == QUESTSTATE_DISP)
	{ // ドラゴン がクリア状態 && 花 / 鍛冶場 / 街灯が未クリア状態
		g_aQuestUI[8].posDest = DRAGON_POS;		// 花クエスト
		g_aQuestUI[9].posDest = DRAGON_POS;		// 花クエストの横線
		g_aQuest[QUESTTYPE_FLOWER].state = QUESTSTATE_MOVEUP;
		g_aQuestUI[10].posDest = FLOWER_POS;	// 鍛冶場クエスト
		g_aQuestUI[11].posDest = FLOWER_POS;	// 鍛冶場クエストの横線
		g_aQuest[QUESTTYPE_BLACKSMITH].state = QUESTSTATE_MOVEUP;
		g_aQuestUI[12].posDest = SMITH_POS;		// 街灯クエスト
		g_aQuestUI[13].posDest = SMITH_POS;		// 街灯クエストの横線
		g_aQuest[QUESTTYPE_STREETLIGHT].state = QUESTSTATE_MOVEUP;
	}
	else if (g_aQuest[QUESTTYPE_FLOWER].state == QUESTSTATE_NONDISP
		&& g_aQuest[QUESTTYPE_DRAGON].state == QUESTSTATE_DISP && g_aQuest[QUESTTYPE_BLACKSMITH].state == QUESTSTATE_DISP && g_aQuest[QUESTTYPE_STREETLIGHT].state == QUESTSTATE_DISP)
	{ // 花 がクリア状態 && ドラゴン / 鍛冶場 / 街灯が未クリア状態
		g_aQuestUI[10].posDest = FLOWER_POS;	// 鍛冶場クエスト
		g_aQuestUI[11].posDest = FLOWER_POS;	// 鍛冶場クエストの横線
		g_aQuest[QUESTTYPE_BLACKSMITH].state = QUESTSTATE_MOVEUP;
		g_aQuestUI[12].posDest = SMITH_POS;		// 街灯クエスト
		g_aQuestUI[13].posDest = SMITH_POS;		// 街灯クエストの横線
		g_aQuest[QUESTTYPE_STREETLIGHT].state = QUESTSTATE_MOVEUP;
	}
	else if (g_aQuest[QUESTTYPE_BLACKSMITH].state == QUESTSTATE_NONDISP
		&& g_aQuest[QUESTTYPE_DRAGON].state == QUESTSTATE_DISP && g_aQuest[QUESTTYPE_FLOWER].state == QUESTSTATE_DISP && g_aQuest[QUESTTYPE_STREETLIGHT].state == QUESTSTATE_DISP)
	{ // 鍛冶場 がクリア状態 && ドラゴン / 花 / 街灯が未クリア状態
		g_aQuestUI[12].posDest = SMITH_POS;	// 街灯クエスト
		g_aQuestUI[13].posDest = SMITH_POS;	// 街灯クエストの横線
		g_aQuest[QUESTTYPE_STREETLIGHT].state = QUESTSTATE_MOVEUP;
	}
	else if (g_aQuest[QUESTTYPE_STREETLIGHT].state == QUESTSTATE_NONDISP
		&& g_aQuest[QUESTTYPE_DRAGON].state == QUESTSTATE_DISP && g_aQuest[QUESTTYPE_FLOWER].state == QUESTSTATE_DISP && g_aQuest[QUESTTYPE_STREETLIGHT].state == QUESTSTATE_DISP)
	{ // 街灯 がクリア状態 && ドラゴン / 花 / 鍛冶場が未クリア状態
		// どちらも上にあるため処理を行わない
	}
}

//======================================================================================
// 線を端まで延ばす
//======================================================================================
void ExtendLine(int nIdx, QUESTTYPE questType)
{
	float fDiffKey = 0.0f;	// キーの差分を計算
	float fRateKey = (float)g_aQuestUI[nIdx].nKey / (float)g_aQuestUI[nIdx].nNumKey;

	// 幅変更
	fDiffKey = g_aQuestUI[nIdx].fWidthDest - g_aQuestUI[nIdx].fWidth;
	g_aQuestUI[nIdx].fWidth = g_aQuestUI[nIdx].fWidth + fDiffKey * fRateKey;
	
	g_aQuestUI[nIdx].nKey++;

	// 一定のキーに達したら次のステートに移行
	if (g_aQuestUI[nIdx].nKey > g_aQuestUI[nIdx].nNumKey)
	{
		g_aQuestUI[nIdx].fWidth = g_aQuestUI[nIdx].fWidthDest;
		g_aQuestUI[nIdx].nKey = 0;
		g_aQuest[questType].state = QUESTSTATE_MOVESIDE;

		switch (questType)
		{
		case QUESTTYPE_TEMPLE:		// 神殿
			g_aQuestUI[nIdx - 1].posDest.x = LEFTSIDE_POSX;
			g_aQuestUI[nIdx].posDest.x = LEFTSIDE_POSX;
			break;

		case QUESTTYPE_SHIRINE:		// 祠
			g_aQuestUI[nIdx - 1].posDest.x = LEFTSIDE_POSX;
			g_aQuestUI[nIdx].posDest.x = LEFTSIDE_POSX;
			break;

		case QUESTTYPE_DRAGON:		// ドラゴン
			g_aQuestUI[nIdx - 1].posDest.x = RIGHTSIDE_POSX;
			g_aQuestUI[nIdx].posDest.x = RIGHTSIDE_POSX;
			break;

		case QUESTTYPE_FLOWER:		// 花
			g_aQuestUI[nIdx - 1].posDest.x = RIGHTSIDE_POSX;
			g_aQuestUI[nIdx].posDest.x = RIGHTSIDE_POSX;
			break;

		case QUESTTYPE_BLACKSMITH:	// 鍛冶場
			g_aQuestUI[nIdx - 1].posDest.x = RIGHTSIDE_POSX;
			g_aQuestUI[nIdx].posDest.x = RIGHTSIDE_POSX;
			break;

		case QUESTTYPE_STREETLIGHT:	// 街灯
			g_aQuestUI[nIdx - 1].posDest.x = RIGHTSIDE_POSX;
			g_aQuestUI[nIdx].posDest.x = RIGHTSIDE_POSX;
			break;
		}
	}
}

//======================================================================================
// 画面外にはけながら透明度を上げる
//======================================================================================
void MoveSide(int nIdx, QUESTTYPE questType)
{
	float fDiffKey, fDiffAlpha = 0.0f;	// キーの差分を計算
	float fRateKey = (float)g_aQuestUI[nIdx].nKey / (float)g_aQuestUI[nIdx].nNumKey;

	// X座標変更
	fDiffKey = g_aQuestUI[nIdx].posDest.x - g_aQuestUI[nIdx].pos.x;
	g_aQuestUI[nIdx].pos.x = g_aQuestUI[nIdx].pos.x + fDiffKey * fRateKey;
	// α値変更
	fDiffAlpha = 0.0f - g_aQuestUI[nIdx].col.a;
	g_aQuestUI[nIdx].col.a = g_aQuestUI[nIdx].col.a + fDiffAlpha * fRateKey;

	g_aQuestUI[nIdx].nKey++;

	// 一定のキーに達したら次のステートに移行
	if (g_aQuestUI[nIdx].nKey > g_aQuestUI[nIdx].nNumKey)
	{
		g_aQuestUI[nIdx].nKey = 0;
		g_aQuestUI[nIdx].fWidth = g_aQuestUI[nIdx].fWidthDest;
		g_aQuest[questType].state = QUESTSTATE_NONDISP;
		g_aQuestUI[nIdx].bDisp = false;
	}
}

//======================================================================================
// 上方向に移動する
//======================================================================================
void MoveUp(int nIdx, QUESTTYPE questType)
{
	float fDiffKey = 0.0f;	// キーの差分を計算
	float fRateKey = (float)g_aQuestUI[nIdx].nKey / (float)g_aQuestUI[nIdx].nNumKey;

	// X座標変更
	fDiffKey = g_aQuestUI[nIdx].posDest.y - g_aQuestUI[nIdx].pos.y;
	g_aQuestUI[nIdx].pos.y = g_aQuestUI[nIdx].pos.y + fDiffKey * fRateKey;

	g_aQuestUI[nIdx].nKey++;

	// 一定のキーに達したら次のステートに移行
	if (g_aQuestUI[nIdx].nKey > g_aQuestUI[nIdx].nNumKey)
	{
		g_aQuestUI[nIdx].nKey = 0;
		g_aQuest[questType].state = QUESTSTATE_DISP;
	}
}

//======================================================================================
// 透明度を下げる
//======================================================================================
void AddAlpha(int nIdx, QUESTTYPE questType)
{
	float fDiffAlpha = 0.0f;	// キーの差分を計算
	float fRateKey = (float)g_aQuestUI[nIdx].nKey / (float)g_aQuestUI[nIdx].nNumKey;

	// α値変更
	fDiffAlpha = 1.0f - g_aQuestUI[nIdx].col.a;
	g_aQuestUI[nIdx].col.a = g_aQuestUI[nIdx].col.a + fDiffAlpha * fRateKey;

	g_aQuestUI[nIdx].nKey++;

	// 一定のキーに達したら次のステートに移行
	if (g_aQuestUI[nIdx].nKey > g_aQuestUI[nIdx].nNumKey)
	{
		g_aQuestUI[nIdx].nKey = 0;
		g_aQuestUI[nIdx].col.a = 1.0f;
		g_aQuest[questType].state = QUESTSTATE_CLEAR;
	}
}

//======================================================================================
// クエスト部分の更新を行う
//======================================================================================
void UpdateQuest(int nIdx, QUESTTYPE questType)
{
	switch (g_aQuest[questType].state)
	{
	case QUESTSTATE_DISP:	// 表示状態

		break;

	case QUESTSTATE_CLEAR:	// クリア状態

		break;

	case QUESTSTATE_ADDALPHA:	// 透明度を上げる
		AddAlpha(nIdx, questType);
		break;

	case QUESTSTATE_MOVESIDE:	// 端にはける
		MoveSide(nIdx, questType);
		break;

	case QUESTSTATE_MOVEUP:	// 上に移動する
		MoveUp(nIdx, questType);
		break;

	case QUESTSTATE_NONDISP:	// 非表示状態
		g_aQuestUI[nIdx].bDisp = false;
		break;
	}
}

//======================================================================================
// 横線の更新を行う
//======================================================================================
void UpdateLine(int nIdx, QUESTTYPE questType)
{
	switch (g_aQuest[questType].state)
	{
	case QUESTSTATE_DISP:	// 表示状態

		break;

	case QUESTSTATE_CLEAR:	// クリア状態
		ExtendLine(nIdx, questType);
		break;

	case QUESTSTATE_ADDALPHA:	// 透明度を上げる

		break;

	case QUESTSTATE_MOVESIDE:	// 端にはける
		MoveSide(nIdx, questType);
		break;

	case QUESTSTATE_MOVEUP:	// 上に移動する
		MoveUp(nIdx, questType);
		break;

	case QUESTSTATE_NONDISP:	// 非表示状態
		g_aQuestUI[nIdx].bDisp = false;
		break;
	}
}