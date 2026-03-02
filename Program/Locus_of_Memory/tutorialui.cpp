//======================================================================================
// 
// チュートリアルの2DUI処理[tutorialui.h]
// Author : Kajino Hiyori
//
//======================================================================================
#include "main.h"
#include "color.h"
#include "fade.h"
#include "tutorialui.h"
#include "tutorial.h"
#include "title.h"
#include "input.h"
#include "fog.h"

// チュートリアルUI演出の管理
typedef enum
{
	TUTORIALUISTATE_NONDISPLAY = 0,	// 非表示
	TUTORIALUISTATE_APPEAR,			// 出現
	TUTORIALUISTATE_DISPLAY,		// 表示
	TUTORIALUISTATE_DISAPPEAR,		// 収縮
}TUTORIALUISTATE;


// チュートリアルUIの構造体
typedef struct
{
	D3DXMATRIX		mtxWorld;	// ワールドマトリックス
	D3DXVECTOR3		pos;		// 位置
	D3DXVECTOR3		rot;		// 向き
	TUTORIALUITYPE	type;		// 種類
	TUTORIALUISTATE state;		// 状態
	float	fWidth;			// 幅
	float	fWidthDest;		// 幅の目的値
	float	fHeight;		// 高さ
	float	fHeightDest;	// 高さの目的値
	int		nNumKey;		// 浮遊感をカウントするキー数
	int		nKey;			// 現在のキー数
	bool	bDisp;			// 表示状態
}TutorialUI;

// マクロ定義
#define NUM_TUTORIALUI		(4)						// チュートリアルUIの表示数
#define MAX_TUTORIALUI		(TUTORIALUITYPE_MAX)	// テクスチャの最大数
#define WIDTH				(105.0f)	// 横幅
#define HEIGHT				(70.0f)	// 縦幅
#define UI_KEY				(30)		// UIのキー数
#define UI_ROT				(D3DXVECTOR3(0.0f, 0.0f, 0.0f))	// 表示方向
#define NORMAL				(D3DXVECTOR3(0.0f, 1.0f, 0.0f))		// 法線ベクトル
#define PLAY_POS			(D3DXVECTOR3(-3480.0f, 120.0f, -3000.0f))	// 操作方法の表示位置
#define MAGIC_POS			(D3DXVECTOR3(-3080.0f, 120.0f, -3000.0f))	// 魔法の使い方の表示位置
#define LIMIT_POS			(D3DXVECTOR3(-2680.0f, 120.0f, -3000.0f))	// 制限時間の表示位置
#define BOOK_POS			(D3DXVECTOR3(-2280.0f, 120.0f, -3000.0f))	// 魔導書の表示位置

// テクスチャの読み込み
const char* c_apFilenameTutorialUI[MAX_TUTORIALUI] =
{
	"data\\TEXTURE\\tutorial\\tutorialui000.png",	// TUTORIALUITYPE_PLAYPAD
	"data\\TEXTURE\\tutorial\\tutorialui001.png",	// TUTORIALUITYPE_PLAYKEY	
	"data\\TEXTURE\\tutorial\\tutorialui002.png",	// TUTORIALUITYPE_MAGICPAD
	"data\\TEXTURE\\tutorial\\tutorialui003.png",	// TUTORIALUITYPE_MAGICKEY
	"data\\TEXTURE\\tutorial\\tutorialui004.png",	// TUTORIALUITYPE_TIMELIMIT
	"data\\TEXTURE\\tutorial\\tutorialui005.png",	// TUTORIALUITYPE_MAGICBOOK
};

// グローバル変数
LPDIRECT3DTEXTURE9 g_apTextureTutorialUI[MAX_TUTORIALUI] = {};	// テクスチャへのポインタ
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffTutorialUI = NULL; // 頂点バッファへのポインタ
TutorialUI g_aTutorialUI[NUM_TUTORIALUI];	// 構造体

//======================================================================================
// TutorialUIの初期化処理
//======================================================================================
void InitTutorialUI(void)
{
	// デバイスの取得
	LPDIRECT3DDEVICE9 pDevice = GetDevice();
	// 操作人数の取得
	OPERATIONTYPE operationType = GetOperationType();
	// プレイヤーの情報を取得

	// テクスチャの読み込み
	for (int nCntUI = 0; nCntUI < MAX_TUTORIALUI; nCntUI++)
	{
		D3DXCreateTextureFromFile(pDevice, c_apFilenameTutorialUI[nCntUI], &g_apTextureTutorialUI[nCntUI]);
	}

	for (int nCntUI = 0; nCntUI < NUM_TUTORIALUI; nCntUI++)
	{
		g_aTutorialUI[nCntUI].pos			= INIT_D3DXVEC3;			// 位置
		g_aTutorialUI[nCntUI].rot			= UI_ROT;					// 向き
		g_aTutorialUI[nCntUI].type			= TUTORIALUITYPE_PLAYPAD;	// 種類
		g_aTutorialUI[nCntUI].fWidth		= WIDTH;					// 幅
		g_aTutorialUI[nCntUI].fWidthDest	= WIDTH;					// 幅の目的値
		g_aTutorialUI[nCntUI].fHeight		= HEIGHT;					// 高さ
		g_aTutorialUI[nCntUI].fHeightDest	= HEIGHT;					// 高さの目的値
		g_aTutorialUI[nCntUI].nNumKey		= UI_KEY;					// 浮遊感をカウントするキー数
		g_aTutorialUI[nCntUI].nKey			= 0;						// 現在のキー数
		g_aTutorialUI[nCntUI].bDisp			= false;					// 表示状態
	}

	// 頂点バッファの生成
	pDevice->CreateVertexBuffer(sizeof(VERTEX_3D) * 4 * NUM_TUTORIALUI, D3DUSAGE_WRITEONLY, FVF_VERTEX_3D, D3DPOOL_MANAGED, &g_pVtxBuffTutorialUI, NULL);

	VERTEX_3D* pVtx;
	// 頂点バッファをロックし、頂点情報へのポインタを取得
	g_pVtxBuffTutorialUI->Lock(0, 0, (void**)&pVtx, 0);

	for (int nCntUI = 0; nCntUI < NUM_TUTORIALUI; nCntUI++, pVtx += 4)
	{
		// 頂点座標の設定
		pVtx[0].pos = D3DXVECTOR3(-g_aTutorialUI[nCntUI].fWidth,  g_aTutorialUI[nCntUI].fHeight, 0.0f);
		pVtx[1].pos = D3DXVECTOR3( g_aTutorialUI[nCntUI].fWidth,  g_aTutorialUI[nCntUI].fHeight, 0.0f);
		pVtx[2].pos = D3DXVECTOR3(-g_aTutorialUI[nCntUI].fWidth, -g_aTutorialUI[nCntUI].fHeight, 0.0f);
		pVtx[3].pos = D3DXVECTOR3( g_aTutorialUI[nCntUI].fWidth, -g_aTutorialUI[nCntUI].fHeight, 0.0f);

		// rhwの設定
		pVtx[0].nor = NORMAL;
		pVtx[1].nor = NORMAL;
		pVtx[2].nor = NORMAL;
		pVtx[3].nor = NORMAL;

		// 頂点カラーの設定
		pVtx[0].col = COLOR_UIBUBBLE;
		pVtx[1].col = COLOR_UIBUBBLE;
		pVtx[2].col = COLOR_UIBUBBLE;
		pVtx[3].col = COLOR_UIBUBBLE;

		// テクスチャ座標の設定
		pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
		pVtx[1].tex = D3DXVECTOR2(1.0f, 0.0f);
		pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
		pVtx[3].tex = D3DXVECTOR2(1.0f, 1.0f);

	}
	// 頂点バッファをアンロック
	g_pVtxBuffTutorialUI->Unlock();

	switch (operationType)
	{
	case OPERATIONTYPE_KEYBOARD:	// キーボード操作
		SetTutorialUI(TUTORIALUITYPE_MAGICKEY, MAGIC_POS, UI_ROT);
		SetTutorialUI(TUTORIALUITYPE_PLAYKEY, PLAY_POS, UI_ROT);
		break;

	default:	// パッド操作
		SetTutorialUI(TUTORIALUITYPE_MAGICPAD, MAGIC_POS, UI_ROT);
		SetTutorialUI(TUTORIALUITYPE_PLAYPAD, PLAY_POS, UI_ROT);
		break;
	}

	SetTutorialUI(TUTORIALUITYPE_TIMELIMIT, LIMIT_POS, UI_ROT);
	SetTutorialUI(TUTORIALUITYPE_MAGICBOOK, BOOK_POS, UI_ROT);
}

//======================================================================================
// TutorialUIの終了処理
//======================================================================================
void UninitTutorialUI(void)
{
	// テクスチャの破棄
	for (int nCntUI = 0; nCntUI < MAX_TUTORIALUI; nCntUI++)
	{
		if (g_apTextureTutorialUI[nCntUI] != NULL)
		{
			g_apTextureTutorialUI[nCntUI]->Release();
			g_apTextureTutorialUI[nCntUI] = NULL;
		}
	}

	// 頂点バッファの破棄
	if (g_pVtxBuffTutorialUI != NULL)
	{
		g_pVtxBuffTutorialUI->Release();
		g_pVtxBuffTutorialUI = NULL;
	}

}

//======================================================================================
// TutorialUIの更新処理
//======================================================================================
void UpdateTutorialUI(void)
{
	
}

//======================================================================================
// TutorialUIの描画処理
//======================================================================================
void DrawTutorialUI(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();	// デバイスの取得
	D3DXMATRIX UIMatrix, mtxRot;	// UIのマトリックス情報を取得

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

	for (int nCntUI = 0; nCntUI < NUM_TUTORIALUI; nCntUI++)
	{
		if (g_aTutorialUI[nCntUI].bDisp == false)
		{
			continue;
		}

		// ワールドマトリックスの設定
		pDevice->SetTransform(D3DTS_WORLD, &UIMatrix);

		D3DXMATRIX	mtxRotModel, mtxTransModel;	// 計算用マトリックス
		D3DXMATRIX	mtxParent;					// 親のマトリックス

		// ポリゴンのワールドマトリックスを初期化
		D3DXMatrixIdentity(&g_aTutorialUI[nCntUI].mtxWorld);

		// パーツの位置を反映
		D3DXMatrixTranslation(&mtxTransModel, g_aTutorialUI[nCntUI].pos.x, g_aTutorialUI[nCntUI].pos.y, g_aTutorialUI[nCntUI].pos.z);
		D3DXMatrixMultiply(&g_aTutorialUI[nCntUI].mtxWorld, &g_aTutorialUI[nCntUI].mtxWorld, &mtxTransModel);

		// 向きを反映
		D3DXMatrixRotationYawPitchRoll(&mtxRot, g_aTutorialUI[nCntUI].rot.y, g_aTutorialUI[nCntUI].rot.x, g_aTutorialUI[nCntUI].rot.z);
		D3DXMatrixMultiply(&g_aTutorialUI[nCntUI].mtxWorld, &g_aTutorialUI[nCntUI].mtxWorld, &mtxRot);

		// パーツのワールドマトリックスを設定
		pDevice->SetTransform(D3DTS_WORLD, &g_aTutorialUI[nCntUI].mtxWorld);

		// 頂点バッファをデータストリームに設定
		pDevice->SetStreamSource(0, g_pVtxBuffTutorialUI, 0, sizeof(VERTEX_3D));

		// 頂点フォーマットの設定
		pDevice->SetFVF(FVF_VERTEX_3D);

		// テクスチャの設定
		pDevice->SetTexture(0, g_apTextureTutorialUI[g_aTutorialUI[nCntUI].type]);

		// UIの描画
		pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, nCntUI * 4, 2);

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
// TutorialUIの設置
//======================================================================================
void SetTutorialUI(TUTORIALUITYPE type, D3DXVECTOR3 pos, D3DXVECTOR3 rot)
{
	for (int nCntUI = 0; nCntUI < NUM_TUTORIALUI; nCntUI++)
	{
		if (g_aTutorialUI[nCntUI].bDisp == false)
		{
			g_aTutorialUI[nCntUI].type = type;
			g_aTutorialUI[nCntUI].pos = pos;
			g_aTutorialUI[nCntUI].rot = rot;
			g_aTutorialUI[nCntUI].bDisp = true;
			break;
		}
	}
}
