//========================================================
// 
// 準備完了状態のUI表示処理[readyui.cpp]
// Author : KajinoHiyori
// 
//========================================================
#include "main.h"
#include "readyui.h"
#include "player.h"
#include "game.h"
#include "input.h"
#include "title.h"
#include "fog.h"
#include "debugproc.h"
#include "color.h"

// マクロ定義
#define MAXREADYUI_TYPE		(READYUITYPE_MAX)	// テクスチャの最大数
#define READY_WIDTH		(30.0f)				// 準備完了状態の幅
#define READY_HEIGHT		(READY_WIDTH / 2)	// 準備完了状態の高さ
#define READY_X			(-10.0f)			// 準備完了状態のX軸
#define READY_Y			(100.0f)				// 準備完了状態のY高度
#define NUM_KEY				(30)				// 処理を行うキー数
#define NORMAL				(D3DXVECTOR3(0.0f, 1.0f, 0.0f))	// 法線ベクトル

// READYUIの構造体
typedef struct
{
	D3DXMATRIX		mtxWorld;	// ワールドマトリックス
	D3DXVECTOR3		pos;		// 位置
	D3DXVECTOR3		rot;		// 向き
	READYUITYPE type;		// 種類
	float	fWidth;			// 幅
	float	fHeight;		// 高さ
	bool	bDisp;			// 表示状態
}ReadyUI;

// グローバル変数
LPDIRECT3DTEXTURE9	g_apTextureReadyUI[MAXREADYUI_TYPE] = {};	// テクスチャへのポインタ
LPDIRECT3DVERTEXBUFFER9	g_pVtxBuffReadyUI = NULL;			// 頂点バッファへのポインタ
ReadyUI g_aReadyUI[MAX_PLAYER];		// READYUIの全体管理

// テクスチャの読み込み
const char* c_apFilenameReadyUI[MAXREADYUI_TYPE] =
{
	"data\\TEXTURE\\ready\\ready000.png",	// チュートリアル遷移[X]
	"data\\TEXTURE\\ready\\ready001.png",	// チュートリアル遷移[ENTER]
	"data\\TEXTURE\\ready\\ready002.png",	// チュートリアル遷移完了状態
	"data\\TEXTURE\\ready\\ready003.png",	// ゲーム遷移[X]
	"data\\TEXTURE\\ready\\ready004.png",	// ゲーム遷移[ENTER]	
	"data\\TEXTURE\\ready\\ready005.png",	// ゲーム遷移完了状態
	"data\\TEXTURE\\ready\\ready006.png",	// 他プレイヤーの待機状態
};

//======================================================================================
// spellの初期化処理
//======================================================================================
void InitReadyUI(void)
{
	// デバイスの取得
	LPDIRECT3DDEVICE9 pDevice = GetDevice();
	// 操作人数の取得
	OPERATIONTYPE operationType = GetOperationType();
	// プレイヤーの情報を取得
	Player* pPlayer = GetPlayer();

	// テクスチャの読み込み
	for (int nCntUI = 0; nCntUI < MAXREADYUI_TYPE; nCntUI++)
	{
		if (g_apTextureReadyUI[nCntUI] == NULL)
		{
			D3DXCreateTextureFromFile(pDevice, c_apFilenameReadyUI[nCntUI], &g_apTextureReadyUI[nCntUI]);
		}
	}

	// 初期化
	for (int nCntPlayer = 0; nCntPlayer < MAX_PLAYER; nCntPlayer++)
	{
		g_aReadyUI[nCntPlayer].pos			= D3DXVECTOR3(0.0f, READY_Y, 0.0f);	// 位置
		g_aReadyUI[nCntPlayer].rot			= D3DXVECTOR3(0.0f, D3DX_PI, 0.0f);			// 向き
		g_aReadyUI[nCntPlayer].type			= READYUITYPE_TUTORIALX;	// 種類
		g_aReadyUI[nCntPlayer].fWidth		= READY_WIDTH;			// 幅
		g_aReadyUI[nCntPlayer].fHeight		= READY_HEIGHT;		// 高さ
		g_aReadyUI[nCntPlayer].bDisp		= false;				// 表示状態
	}

	// 頂点バッファの生成
	pDevice->CreateVertexBuffer(sizeof(VERTEX_3D) * 4 * MAX_PLAYER, D3DUSAGE_WRITEONLY, FVF_VERTEX_3D, D3DPOOL_MANAGED, &g_pVtxBuffReadyUI, NULL);

	VERTEX_3D* pVtx;
	// 頂点バッファをロックし、頂点情報へのポインタを取得
	g_pVtxBuffReadyUI->Lock(0, 0, (void**)&pVtx, 0);

	for (int nCntPlayer = 0; nCntPlayer < MAX_PLAYER; nCntPlayer++, pVtx += 4)
	{
		// 頂点座標の設定
		pVtx[0].pos = D3DXVECTOR3(-g_aReadyUI[nCntPlayer].fWidth,  g_aReadyUI[nCntPlayer].fHeight, 0.0f);
		pVtx[1].pos = D3DXVECTOR3( g_aReadyUI[nCntPlayer].fWidth,  g_aReadyUI[nCntPlayer].fHeight, 0.0f);
		pVtx[2].pos = D3DXVECTOR3(-g_aReadyUI[nCntPlayer].fWidth, -g_aReadyUI[nCntPlayer].fHeight, 0.0f);
		pVtx[3].pos = D3DXVECTOR3( g_aReadyUI[nCntPlayer].fWidth, -g_aReadyUI[nCntPlayer].fHeight, 0.0f);

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
	// 頂点バッファをアンロック
	g_pVtxBuffReadyUI->Unlock();
}

//======================================================================================
// spellの終了処理
//======================================================================================
void UninitReadyUI(void)
{
	// テクスチャの破棄
	for (int nCntUI = 0; nCntUI < MAXREADYUI_TYPE; nCntUI++)
	{
		if (g_apTextureReadyUI[nCntUI] != NULL)
		{
			g_apTextureReadyUI[nCntUI]->Release();
			g_apTextureReadyUI[nCntUI] = NULL;
		}
	}

	// 頂点バッファの破棄
	if (g_pVtxBuffReadyUI != NULL)
	{
		g_pVtxBuffReadyUI->Release();
		g_pVtxBuffReadyUI = NULL;
	}

}

//======================================================================================
// spellの更新処理
//======================================================================================
void UpdateReadyUI(void)
{
	Player* pPlayer = GetPlayer();
	bool bDisp = false;	// 準備完了状態の表示を管理

	for (int nCntPlayer = 0; nCntPlayer < MAX_PLAYER; nCntPlayer++, pPlayer++)
	{
		
		if (pPlayer->bUse == true)
		{
			g_aReadyUI[nCntPlayer].pos = pPlayer->pos;
			g_aReadyUI[nCntPlayer].pos.x += sinf(pPlayer->rot.y) * READY_X;
			g_aReadyUI[nCntPlayer].pos.z += cosf(pPlayer->rot.y) * READY_X;
			g_aReadyUI[nCntPlayer].pos.y += READY_Y;
		}
	}
}

//======================================================================================
// spellの描画処理
//======================================================================================
void DrawReadyUI(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();	// デバイスの取得
	D3DXMATRIX UIMatrix, mtxRot, mtxView;	// UIのマトリックス情報を取得
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
		if (g_aReadyUI[nCntPlayer].bDisp == false)
		{
			continue;
		}
		// UIのマトリックス情報を取得
		UIMatrix = pPlayer->mtxWorld;

		// ワールドマトリックスの設定
		pDevice->SetTransform(D3DTS_WORLD, &UIMatrix);

		D3DXMATRIX	mtxRotModel, mtxTransModel;	// 計算用マトリックス
		D3DXMATRIX	mtxParent;					// 親のマトリックス

		// 親となるマトリックスを設定
		// ポリゴンのワールドマトリックスを初期化
		D3DXMatrixIdentity(&g_aReadyUI[nCntPlayer].mtxWorld);

		// ビューマトリックスを取得する
		pDevice->GetTransform(D3DTS_VIEW, &mtxView);

		// ポリゴンをカメラに対して正面に向ける
		D3DXMatrixInverse(&g_aReadyUI[nCntPlayer].mtxWorld, NULL, &mtxView);	//逆行列を求める

		g_aReadyUI[nCntPlayer].mtxWorld._41 = 0.0f;		//マトリックス(行列)の内容
		g_aReadyUI[nCntPlayer].mtxWorld._42 = 0.0f;
		g_aReadyUI[nCntPlayer].mtxWorld._43 = 0.0f;

		// パーツの位置を反映
		D3DXMatrixTranslation(&mtxTransModel, g_aReadyUI[nCntPlayer].pos.x, g_aReadyUI[nCntPlayer].pos.y, g_aReadyUI[nCntPlayer].pos.z);
		D3DXMatrixMultiply(&g_aReadyUI[nCntPlayer].mtxWorld, &g_aReadyUI[nCntPlayer].mtxWorld, &mtxTransModel);

		// パーツのワールドマトリックスを設定
		pDevice->SetTransform(D3DTS_WORLD, &g_aReadyUI[nCntPlayer].mtxWorld);

		// 頂点バッファをデータストリームに設定
		pDevice->SetStreamSource(0, g_pVtxBuffReadyUI, 0, sizeof(VERTEX_3D));

		// 頂点フォーマットの設定
		pDevice->SetFVF(FVF_VERTEX_3D);

		// テクスチャの設定
		pDevice->SetTexture(0, g_apTextureReadyUI[g_aReadyUI[nCntPlayer].type]);

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

//======================================================================================
// 準備完了状態の配置
//======================================================================================
void SetReadyUI(int nIdx, READYUITYPE type)
{
	g_aReadyUI[nIdx].bDisp = true;
	g_aReadyUI[nIdx].type = type;
}

//======================================================================================
// 準備完了状態の非表示
//======================================================================================
void ResetReadyUI(int nIdx)
{
	g_aReadyUI[nIdx].bDisp = false;
}
