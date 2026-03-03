//========================================================
// 
// 魔法の位置に合わせたアイコン表示処理[questionmark.cpp]
// Author : KajinoHiyori
// 
//========================================================
#include "main.h"
#include "questionmark.h"
#include "magic.h"
#include "player.h"
#include "game.h"
#include "input.h"
#include "title.h"
#include "fog.h"
#include "debugproc.h"
#include "color.h"

// マクロ定義
#define QUESTIONMARK_TYPE	(1)	// テクスチャの最大数
#define BUBBLE_WIDTH		(28.0f)					// 吹き出しの幅
#define BUBBLE_HEIGHT		(10.0f)					// 吹き出しの高さ
#define BUBBLE_X			(30.0f)					// 吹き出しのX軸
#define BUBBLE_Y			(85.0f)					// 吹き出しのY高度
#define NUM_KEY				(30)					// 処理を行うキー数
#define NORMAL				(D3DXVECTOR3(0.0f, 1.0f, 0.0f))	// 法線ベクトル

// MAGICBUBBLEの構造体
typedef struct
{
	D3DXMATRIX		mtxWorld;	// ワールドマトリックス
	D3DXVECTOR3		pos;		// 位置
	D3DXVECTOR3		rot;		// 向き
	D3DXCOLOR		col;		// 色
	float	fWidth;			// 幅
	float	fWidthDest;		// 幅の目的値
	float	fHeight;		// 高さ
	float	fHeightDest;	// 高さの目的値
	int		nNumKey;		// 浮遊感をカウントするキー数
	int		nKey;			// 現在のキー数
	bool	bDisp;			// 表示状態
}QuestionMark;

// グローバル変数
LPDIRECT3DTEXTURE9	g_apTextureQuestionMark[QUESTIONMARK_TYPE] = {};	// テクスチャへのポインタ
LPDIRECT3DVERTEXBUFFER9	g_pVtxBuffQuestionMark = NULL;			// 頂点バッファへのポインタ
QuestionMark g_aQuestionMark[MAX_PLAYER];		// MAGICBUBBLEの全体管理

// テクスチャの読み込み
const char* c_apFilenameQuestionMark[QUESTIONMARK_TYPE] =
{
	"data\\TEXTURE\\questionmark.png",	// ?
};

//======================================================================================
// questionmarkの初期化処理
//======================================================================================
void InitQuestionMark(void)
{
	// デバイスの取得
	LPDIRECT3DDEVICE9 pDevice = GetDevice();
	// 操作人数の取得
	OPERATIONTYPE operationType = GetOperationType();
	// プレイヤーの情報を取得
	Player* pPlayer = GetPlayer();

	// テクスチャの読み込み
	for (int nCntUI = 0; nCntUI < QUESTIONMARK_TYPE; nCntUI++)
	{
		if (g_apTextureQuestionMark[nCntUI] == NULL)
		{
			D3DXCreateTextureFromFile(pDevice, c_apFilenameQuestionMark[nCntUI], &g_apTextureQuestionMark[nCntUI]);
		}
	}

	// 初期化
	for (int nCntPlayer = 0; nCntPlayer < MAX_PLAYER; nCntPlayer++)
	{
		g_aQuestionMark[nCntPlayer].pos = D3DXVECTOR3(BUBBLE_X, BUBBLE_Y, BUBBLE_X);	// 位置
		g_aQuestionMark[nCntPlayer].rot = D3DXVECTOR3(0.0f, D3DX_PI, 0.0f);			// 向き
		g_aQuestionMark[nCntPlayer].col = COLOR_WHITE;	// 種類
		g_aQuestionMark[nCntPlayer].fWidth = BUBBLE_WIDTH;			// 幅
		g_aQuestionMark[nCntPlayer].fWidthDest = BUBBLE_WIDTH;			// 幅の目的値
		g_aQuestionMark[nCntPlayer].fHeight = BUBBLE_HEIGHT;		// 高さ
		g_aQuestionMark[nCntPlayer].fHeightDest = BUBBLE_HEIGHT;		// 高さの目的値
		g_aQuestionMark[nCntPlayer].nNumKey = NUM_KEY;				// 浮遊感をカウントするキー数
		g_aQuestionMark[nCntPlayer].nKey = 0;					// 現在のキー数
		g_aQuestionMark[nCntPlayer].bDisp = false;				// 表示状態
	}

	// 頂点バッファの生成
	pDevice->CreateVertexBuffer(sizeof(VERTEX_3D) * 4 * MAX_PLAYER, D3DUSAGE_WRITEONLY, FVF_VERTEX_3D, D3DPOOL_MANAGED, &g_pVtxBuffQuestionMark, NULL);

	VERTEX_3D* pVtx;
	// 頂点バッファをロックし、頂点情報へのポインタを取得
	g_pVtxBuffQuestionMark->Lock(0, 0, (void**)&pVtx, 0);

	for (int nCntPlayer = 0; nCntPlayer < MAX_PLAYER; nCntPlayer++, pVtx += 4)
	{
		// 頂点座標の設定
		pVtx[0].pos = D3DXVECTOR3(-g_aQuestionMark[nCntPlayer].fWidth, g_aQuestionMark[nCntPlayer].fHeight, 0.0f);
		pVtx[1].pos = D3DXVECTOR3(g_aQuestionMark[nCntPlayer].fWidth, g_aQuestionMark[nCntPlayer].fHeight, 0.0f);
		pVtx[2].pos = D3DXVECTOR3(-g_aQuestionMark[nCntPlayer].fWidth, -g_aQuestionMark[nCntPlayer].fHeight, 0.0f);
		pVtx[3].pos = D3DXVECTOR3(g_aQuestionMark[nCntPlayer].fWidth, -g_aQuestionMark[nCntPlayer].fHeight, 0.0f);

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
	g_pVtxBuffQuestionMark->Unlock();
}

//======================================================================================
// questionmarkの終了処理
//======================================================================================
void UninitQuestionMark(void)
{
	// テクスチャの破棄
	for (int nCntUI = 0; nCntUI < QUESTIONMARK_TYPE; nCntUI++)
	{
		if (g_apTextureQuestionMark[nCntUI] != NULL)
		{
			g_apTextureQuestionMark[nCntUI]->Release();
			g_apTextureQuestionMark[nCntUI] = NULL;
		}
	}

	// 頂点バッファの破棄
	if (g_pVtxBuffQuestionMark != NULL)
	{
		g_pVtxBuffQuestionMark->Release();
		g_pVtxBuffQuestionMark = NULL;
	}

}

//======================================================================================
// questionmarkの更新処理
//======================================================================================
void UpdateQuestionMark(void)
{

	
}

//======================================================================================
// questionmarkの描画処理
//======================================================================================
void DrawQuestionMark(void)
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
		if (g_aQuestionMark[nCntPlayer].bDisp == false)
		{
			//continue;
		}
		// UIのマトリックス情報を取得
		UIMatrix = pPlayer->mtxWorld;

		// ワールドマトリックスの設定
		pDevice->SetTransform(D3DTS_WORLD, &UIMatrix);

		D3DXMATRIX	mtxRotModel, mtxTransModel;	// 計算用マトリックス
		D3DXMATRIX	mtxParent;					// 親のマトリックス

		// ポリゴンのワールドマトリックスを初期化
		D3DXMatrixIdentity(&g_aQuestionMark[nCntPlayer].mtxWorld);

		// パーツの位置を反映
		D3DXMatrixTranslation(&mtxTransModel, g_aQuestionMark[nCntPlayer].pos.x, g_aQuestionMark[nCntPlayer].pos.y, g_aQuestionMark[nCntPlayer].pos.z);
		D3DXMatrixMultiply(&g_aQuestionMark[nCntPlayer].mtxWorld, &g_aQuestionMark[nCntPlayer].mtxWorld, &mtxTransModel);

		// 向きを反映
		D3DXMatrixRotationYawPitchRoll(&mtxRot, g_aQuestionMark[nCntPlayer].rot.y, g_aQuestionMark[nCntPlayer].rot.x, g_aQuestionMark[nCntPlayer].rot.z);
		D3DXMatrixMultiply(&g_aQuestionMark[nCntPlayer].mtxWorld, &g_aQuestionMark[nCntPlayer].mtxWorld, &mtxRot);

		// 親マトリックスを設定
		mtxParent = UIMatrix;

		// 算出したパーツのワールドマトリックスと親モデルのマトリックスを掛け合わせる
		D3DXMatrixMultiply(&g_aQuestionMark[nCntPlayer].mtxWorld, &g_aQuestionMark[nCntPlayer].mtxWorld, &mtxParent);

		// パーツのワールドマトリックスを設定
		pDevice->SetTransform(D3DTS_WORLD, &g_aQuestionMark[nCntPlayer].mtxWorld);

		// 頂点バッファをデータストリームに設定
		pDevice->SetStreamSource(0, g_pVtxBuffQuestionMark, 0, sizeof(VERTEX_3D));

		// 頂点フォーマットの設定
		pDevice->SetFVF(FVF_VERTEX_3D);

		// テクスチャの設定
		pDevice->SetTexture(0, g_apTextureQuestionMark[0]);

		// UIの描画
		pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, nCntPlayer * 4, 2);

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
