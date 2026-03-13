//======================================================================================
// 
// リザルトの2DUI処理[resultui.h]
// Author : Kajino Hiyori
//
//======================================================================================
#include "main.h"
#include "color.h"
#include "fade.h"
#include "resultui.h"
#include "result.h"
#include "title.h"
#include "input.h"
#include "fog.h"
#include "player.h"
#include "debugproc.h"

#if 0
// リザルトUI演出の管理
typedef enum
{
	RESULTUISTATE_NONDISPLAY = 0,	// 非表示
	RESULTUISTATE_APPEAR,			// 出現
	RESULTUISTATE_DISPLAY,		// 表示
	RESULTUISTATE_DISAPPEAR,		// 収縮
}RESULTUISTATE;

// リザルトUIの構造体
typedef struct
{
	D3DXMATRIX		mtxWorld;	// ワールドマトリックス
	D3DXVECTOR3		pos;		// 位置
	D3DXVECTOR3		rot;		// 向き
	RESULTUITYPE	type;		// 種類
	RESULTUISTATE state;		// 状態
	float	fWidth;			// 幅
	float	fWidthDest;		// 幅の目的値
	float	fHeight;		// 高さ
	float	fHeightDest;	// 高さの目的値
	int		nNumKey;		// 浮遊感をカウントするキー数
	int		nKey;			// 現在のキー数
	bool	bDisp;			// 表示状態
}ResultUI;

// マクロ定義
#define NUM_RESULTUI		(RESULTUITYPE_MAX - 2)						// リザルトUIの表示数
#define MAX_RESULTUI		(RESULTUITYPE_MAX)	// テクスチャの最大数
#define WIDTH				(105.0f)	// 横幅
#define HEIGHT				(70.0f)	// 縦幅
#define UI_KEY				(30)		// UIのキー数
#define CIRCLE_SIZE			(90.0f)	// 魔法陣の大きさ
#define UI_ROT				(D3DXVECTOR3(0.0f, 0.0f, 0.0f))	// 表示方向
#define NORMAL				(D3DXVECTOR3(0.0f, 1.0f, 0.0f))		// 法線ベクトル
#define PLAY_POS			(D3DXVECTOR3(-3480.0f, 120.0f, -3100.0f))	// 操作方法の表示位置
#define MAGIC_POS			(D3DXVECTOR3(-3080.0f, 120.0f, -3100.0f))	// 魔法の使い方の表示位置
#define LIMIT_POS			(D3DXVECTOR3(-2480.0f, 120.0f, -3100.0f))	// 制限時間の表示位置
#define BOOK_POS			(D3DXVECTOR3(-1880.0f, 120.0f, -3100.0f))	// 魔導書の表示位置
#define HOUSE_POS			(D3DXVECTOR3(-2130.0f, 120.0f, -3900.0f))	// 燃える家の表示位置
#define CIRCLE_POS			(D3DXVECTOR3(-1000.0f, 120.0f, -3650.0f))	// 魔法陣の表示位置
#define PLANTS_POS			(D3DXVECTOR3(-3300.0f, 120.0f, -3630.0f))	// 植物の表示位置
#define BRIDGE_POS			(D3DXVECTOR3(-2800.0f, 120.0f, -3100.0f))	// 橋の表示位置
#define CIRCLE_ROT			(D3DXVECTOR3(0.0f, D3DX_PI / 2, 0.0f))		// 魔法陣の表示角度
#define CIRCLE_ROTATE		(0.05f)		// 魔法陣の回転速度
#define APPEAR_SIZE			(250.0f)	// 出現の当たり判定を管理するサイズ

// テクスチャの読み込み
const char* c_apFilenameResultUI[MAX_RESULTUI] =
{
	"data\\TEXTURE\\result\\resultui000.png",	// RESULTUITYPE_PLAYPAD
	"data\\TEXTURE\\result\\resultui001.png",	// RESULTUITYPE_PLAYKEY	
	"data\\TEXTURE\\result\\resultui002.png",	// RESULTUITYPE_MAGICPAD
	"data\\TEXTURE\\result\\resultui003.png",	// RESULTUITYPE_MAGICKEY
	"data\\TEXTURE\\result\\resultui004.png",	// RESULTUITYPE_TIMELIMIT
	"data\\TEXTURE\\result\\resultui005.png",	// RESULTUITYPE_MAGICBOOK
	"data\\TEXTURE\\result\\resultui006.png",	// RESULTUITYPE_FIREHOUSE
	"data\\TEXTURE\\result\\resultui007.png",	// RESULTUITYPE_PLANTS
	"data\\TEXTURE\\result\\resultui008.png",	// RESULTUITYPE_PLANTS
	"data\\TEXTURE\\SpellUI\\19_SunsetDelay.png",	// RESULTUITYPE_MAGICCIRCLE
};

// グローバル変数
LPDIRECT3DTEXTURE9 g_apTextureResultUI[MAX_RESULTUI] = {};	// テクスチャへのポインタ
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffResultUI = NULL; // 頂点バッファへのポインタ
ResultUI g_aResultUI[NUM_RESULTUI];	// 構造体

//======================================================================================
// ResultUIの初期化処理
//======================================================================================
void InitResultUI(void)
{
	// デバイスの取得
	LPDIRECT3DDEVICE9 pDevice = GetDevice();
	// 操作人数の取得
	OPERATIONTYPE operationType = GetOperationType();

	// テクスチャの読み込み
	for (int nCntUI = 0; nCntUI < MAX_RESULTUI; nCntUI++)
	{
		D3DXCreateTextureFromFile(pDevice, c_apFilenameResultUI[nCntUI], &g_apTextureResultUI[nCntUI]);
	}

	for (int nCntUI = 0; nCntUI < NUM_RESULTUI; nCntUI++)
	{
		g_aResultUI[nCntUI].pos = INIT_D3DXVEC3;				// 位置
		g_aResultUI[nCntUI].rot = UI_ROT;						// 向き
		g_aResultUI[nCntUI].type = RESULTUITYPE_PLAYPAD;		// 種類
		g_aResultUI[nCntUI].state = RESULTUISTATE_NONDISPLAY;	// 出現モード
		g_aResultUI[nCntUI].fWidth = WIDTH;						// 幅
		g_aResultUI[nCntUI].fWidthDest = WIDTH;						// 幅の目的値
		g_aResultUI[nCntUI].fHeight = HEIGHT;						// 高さ
		g_aResultUI[nCntUI].fHeightDest = HEIGHT;						// 高さの目的値
		g_aResultUI[nCntUI].nNumKey = UI_KEY;						// 浮遊感をカウントするキー数
		g_aResultUI[nCntUI].nKey = 0;							// 現在のキー数
		g_aResultUI[nCntUI].bDisp = false;						// 表示状態

		SetResultUINonDisp(nCntUI);
	}

	// 頂点バッファの生成
	pDevice->CreateVertexBuffer(sizeof(VERTEX_3D) * 4 * NUM_RESULTUI, D3DUSAGE_WRITEONLY, FVF_VERTEX_3D, D3DPOOL_MANAGED, &g_pVtxBuffResultUI, NULL);

	VERTEX_3D* pVtx;
	// 頂点バッファをロックし、頂点情報へのポインタを取得
	g_pVtxBuffResultUI->Lock(0, 0, (void**)&pVtx, 0);

	for (int nCntUI = 0; nCntUI < NUM_RESULTUI; nCntUI++, pVtx += 4)
	{
		// 頂点座標の設定
		pVtx[0].pos = D3DXVECTOR3(-g_aResultUI[nCntUI].fWidth, g_aResultUI[nCntUI].fHeight, 0.0f);
		pVtx[1].pos = D3DXVECTOR3(g_aResultUI[nCntUI].fWidth, g_aResultUI[nCntUI].fHeight, 0.0f);
		pVtx[2].pos = D3DXVECTOR3(-g_aResultUI[nCntUI].fWidth, -g_aResultUI[nCntUI].fHeight, 0.0f);
		pVtx[3].pos = D3DXVECTOR3(g_aResultUI[nCntUI].fWidth, -g_aResultUI[nCntUI].fHeight, 0.0f);

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
	g_pVtxBuffResultUI->Unlock();

	switch (operationType)
	{
	case OPERATIONTYPE_KEYBOARD:	// キーボード操作
		SetResultUI(RESULTUITYPE_MAGICKEY, MAGIC_POS, UI_ROT);
		SetResultUI(RESULTUITYPE_PLAYKEY, PLAY_POS, UI_ROT);
		break;

	default:	// パッド操作
		SetResultUI(RESULTUITYPE_MAGICPAD, MAGIC_POS, UI_ROT);
		SetResultUI(RESULTUITYPE_PLAYPAD, PLAY_POS, UI_ROT);
		break;
	}

	SetResultUI(RESULTUITYPE_TIMELIMIT, LIMIT_POS, UI_ROT);
	SetResultUI(RESULTUITYPE_MAGICBOOK, BOOK_POS, UI_ROT);
	SetResultUI(RESULTUITYPE_FIREHOUSE, HOUSE_POS, UI_ROT);
	SetResultUI(RESULTUITYPE_PLANTS, PLANTS_POS, UI_ROT);
	SetResultUI(RESULTUITYPE_BRIDGES, BRIDGE_POS, UI_ROT);
	SetResultUI(RESULTUITYPE_MAGICCIRCLE, CIRCLE_POS, CIRCLE_ROT);
}

//======================================================================================
// ResultUIの終了処理
//======================================================================================
void UninitResultUI(void)
{
	// テクスチャの破棄
	for (int nCntUI = 0; nCntUI < MAX_RESULTUI; nCntUI++)
	{
		if (g_apTextureResultUI[nCntUI] != NULL)
		{
			g_apTextureResultUI[nCntUI]->Release();
			g_apTextureResultUI[nCntUI] = NULL;
		}
	}

	// 頂点バッファの破棄
	if (g_pVtxBuffResultUI != NULL)
	{
		g_pVtxBuffResultUI->Release();
		g_pVtxBuffResultUI = NULL;
	}

}

//======================================================================================
// ResultUIの更新処理
//======================================================================================
void UpdateResultUI(void)
{
	Player* pPlayer = GetPlayer();	// プレイヤーの情報を取得
	float fDiffKey, fRateKey = 0.0f;
	bool bDisp[NUM_RESULTUI] = { false, false, false, false };
	OPERATIONTYPE operationType = GetOperationType();

	for (int nCntPlayer = 0; nCntPlayer < MAX_PLAYER; nCntPlayer++, pPlayer++)
	{
		if (pPlayer->bUse == false)
		{
			continue;
		}
		for (int nCntUI = 0; nCntUI < NUM_RESULTUI; nCntUI++)
		{
			if (g_aResultUI[nCntUI].type == RESULTUITYPE_MAGICCIRCLE)
			{
				g_aResultUI[nCntUI].rot.x += CIRCLE_ROTATE;
				//g_aResultUI[nCntUI].rot.y += CIRCLE_ROTATE;
				g_aResultUI[nCntUI].rot.z += CIRCLE_ROTATE;
				g_aResultUI[nCntUI].fWidth = CIRCLE_SIZE;
				g_aResultUI[nCntUI].fHeight = CIRCLE_SIZE;
				g_aResultUI[nCntUI].state = RESULTUISTATE_DISPLAY;
			}
			else
			{
				// プレイヤーが設置位置に近づいた場合展開する
				if (pPlayer->pos.x >= g_aResultUI[nCntUI].pos.x - APPEAR_SIZE &&	// 一定範囲より右にある
					pPlayer->pos.x <= g_aResultUI[nCntUI].pos.x + APPEAR_SIZE &&	// 一定範囲より左にある
					pPlayer->pos.z >= g_aResultUI[nCntUI].pos.z - APPEAR_SIZE &&	// 一定範囲より奥にある
					pPlayer->pos.z <= g_aResultUI[nCntUI].pos.z + APPEAR_SIZE)
				{
					bDisp[nCntUI] = true;
					if (g_aResultUI[nCntUI].state == RESULTUISTATE_NONDISPLAY)
					{
						SetResultUIAppear(nCntUI);
					}
				}
				else if (g_aResultUI[nCntUI].state == RESULTUISTATE_APPEAR || // 出現状態
					g_aResultUI[nCntUI].state == RESULTUISTATE_DISPLAY)	// 表示状態

				{ // 2人プレイの場合の非表示処理

					switch (operationType)
					{
					case OPERATIONTYPE_2P:	// 2人操作
						if (bDisp[nCntUI] == false && nCntPlayer == 0)
						{
							SetResultUIDisappear(nCntUI);
						}
						break;

					default:
						SetResultUIDisappear(nCntUI);
						break;
					}
				}
			}
		}
	}

	for (int nCntUI = 0; nCntUI < NUM_RESULTUI; nCntUI++)
	{
		switch (g_aResultUI[nCntUI].state)
		{
		case RESULTUISTATE_NONDISPLAY:	// 非表示
			g_aResultUI[nCntUI].bDisp = false;
			break;

		case RESULTUISTATE_APPEAR:	// 出現
			// 背景の高度変更
			fRateKey = (float)g_aResultUI[nCntUI].nKey / (float)g_aResultUI[nCntUI].nNumKey;
			fDiffKey = g_aResultUI[nCntUI].fHeightDest - g_aResultUI[nCntUI].fHeight;
			g_aResultUI[nCntUI].fHeight = g_aResultUI[nCntUI].fHeight + fDiffKey * fRateKey;

			// 中心位置からの位置を求める
			g_aResultUI[nCntUI].nKey++;

			if (g_aResultUI[nCntUI].nKey > g_aResultUI[nCntUI].nNumKey)
			{
				SetResultUIDisp(nCntUI);
			}
			break;

		case RESULTUISTATE_DISPLAY:	// 表示
			g_aResultUI[nCntUI].bDisp = true;
			break;

		case RESULTUISTATE_DISAPPEAR:	// 収縮
			// 背景の高度変更
			fRateKey = (float)g_aResultUI[nCntUI].nKey / (float)g_aResultUI[nCntUI].nNumKey;
			fDiffKey = g_aResultUI[nCntUI].fHeightDest - g_aResultUI[nCntUI].fHeight;
			g_aResultUI[nCntUI].fHeight = g_aResultUI[nCntUI].fHeight + fDiffKey * fRateKey;

			// 中心位置からの位置を求める
			g_aResultUI[nCntUI].nKey++;

			if (g_aResultUI[nCntUI].nKey > g_aResultUI[nCntUI].nNumKey)
			{
				SetResultUINonDisp(nCntUI);
			}
			break;
		}
	}

	VERTEX_3D* pVtx;
	// 頂点バッファをロックし、頂点情報へのポインタを取得
	g_pVtxBuffResultUI->Lock(0, 0, (void**)&pVtx, 0);

	for (int nCntUI = 0; nCntUI < NUM_RESULTUI; nCntUI++, pVtx += 4)
	{
		// 頂点座標の設定
		pVtx[0].pos = D3DXVECTOR3(-g_aResultUI[nCntUI].fWidth, g_aResultUI[nCntUI].fHeight, 0.0f);
		pVtx[1].pos = D3DXVECTOR3(g_aResultUI[nCntUI].fWidth, g_aResultUI[nCntUI].fHeight, 0.0f);
		pVtx[2].pos = D3DXVECTOR3(-g_aResultUI[nCntUI].fWidth, -g_aResultUI[nCntUI].fHeight, 0.0f);
		pVtx[3].pos = D3DXVECTOR3(g_aResultUI[nCntUI].fWidth, -g_aResultUI[nCntUI].fHeight, 0.0f);

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
	g_pVtxBuffResultUI->Unlock();
}

//======================================================================================
// ResultUIの描画処理
//======================================================================================
void DrawResultUI(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();	// デバイスの取得
	D3DXMATRIX UIMatrix, mtxRot, mtxView;	// UIのマトリックス情報を取得

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

	for (int nCntUI = 0; nCntUI < NUM_RESULTUI; nCntUI++)
	{
		if (g_aResultUI[nCntUI].bDisp == false)
		{
			continue;
		}

		// ワールドマトリックスの設定

		D3DXMATRIX	mtxRotModel, mtxTransModel;	// 計算用マトリックス
		D3DXMATRIX	mtxParent;					// 親のマトリックス

		// ポリゴンのワールドマトリックスを初期化
		D3DXMatrixIdentity(&g_aResultUI[nCntUI].mtxWorld);

		if (g_aResultUI[nCntUI].type != RESULTUITYPE_MAGICCIRCLE)	// 魔法陣以外はビルボードにする
		{
			// ビューマトリックスを取得する
			pDevice->GetTransform(D3DTS_VIEW, &mtxView);

			// ポリゴンをカメラに対して正面に向ける
			D3DXMatrixInverse(&g_aResultUI[nCntUI].mtxWorld, NULL, &mtxView);	//逆行列を求める

			g_aResultUI[nCntUI].mtxWorld._41 = 0.0f;		//マトリックス(行列)の内容
			g_aResultUI[nCntUI].mtxWorld._42 = 0.0f;
			g_aResultUI[nCntUI].mtxWorld._43 = 0.0f;
		}
		else
		{
			// 向きを反映
			D3DXMatrixRotationYawPitchRoll(&mtxRot, g_aResultUI[nCntUI].rot.y, g_aResultUI[nCntUI].rot.x, g_aResultUI[nCntUI].rot.z);
			D3DXMatrixMultiply(&g_aResultUI[nCntUI].mtxWorld, &g_aResultUI[nCntUI].mtxWorld, &mtxRot);
		}

		// パーツの位置を反映
		D3DXMatrixTranslation(&mtxTransModel, g_aResultUI[nCntUI].pos.x, g_aResultUI[nCntUI].pos.y, g_aResultUI[nCntUI].pos.z);
		D3DXMatrixMultiply(&g_aResultUI[nCntUI].mtxWorld, &g_aResultUI[nCntUI].mtxWorld, &mtxTransModel);

		// パーツのワールドマトリックスを設定
		pDevice->SetTransform(D3DTS_WORLD, &g_aResultUI[nCntUI].mtxWorld);

		// 頂点バッファをデータストリームに設定
		pDevice->SetStreamSource(0, g_pVtxBuffResultUI, 0, sizeof(VERTEX_3D));

		// 頂点フォーマットの設定
		pDevice->SetFVF(FVF_VERTEX_3D);

		// テクスチャの設定
		pDevice->SetTexture(0, g_apTextureResultUI[g_aResultUI[nCntUI].type]);

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
// ResultUIの設置
//======================================================================================
void SetResultUI(RESULTUITYPE type, D3DXVECTOR3 pos, D3DXVECTOR3 rot)
{
	for (int nCntUI = 0; nCntUI < NUM_RESULTUI; nCntUI++)
	{
		if (g_aResultUI[nCntUI].bDisp == false)
		{
			g_aResultUI[nCntUI].type = type;
			g_aResultUI[nCntUI].pos = pos;
			g_aResultUI[nCntUI].rot = rot;
			g_aResultUI[nCntUI].bDisp = true;
			break;
		}
	}
}

//======================================================================================
// ResultUIを出現状態にする
//======================================================================================
void SetResultUIAppear(int nIdx)
{
	g_aResultUI[nIdx].state = RESULTUISTATE_APPEAR;
	g_aResultUI[nIdx].fWidth = WIDTH;		// 幅
	g_aResultUI[nIdx].fWidthDest = WIDTH;	// 幅の目的値
	g_aResultUI[nIdx].fHeightDest = HEIGHT;	// 高さの目的値
	g_aResultUI[nIdx].nKey = 0;		// 現在のキー数
	g_aResultUI[nIdx].bDisp = true;		// 表示状態
}

//======================================================================================
// ResultUIを表示状態にする
//======================================================================================
void SetResultUIDisp(int nIdx)
{
	g_aResultUI[nIdx].state = RESULTUISTATE_DISPLAY;
	g_aResultUI[nIdx].fWidth = WIDTH;	// 幅
	g_aResultUI[nIdx].fWidthDest = WIDTH;	// 幅の目的値
	g_aResultUI[nIdx].fHeight = HEIGHT;	// 高さの目的値
	g_aResultUI[nIdx].fHeightDest = HEIGHT;	// 高さの目的値
	g_aResultUI[nIdx].nKey = 0;		// 現在のキー数
	g_aResultUI[nIdx].bDisp = true;		// 表示状態
}

//======================================================================================
// ResultUIを収縮状態にする
//======================================================================================
void SetResultUIDisappear(int nIdx)
{
	g_aResultUI[nIdx].state = RESULTUISTATE_DISAPPEAR;
	g_aResultUI[nIdx].fWidth = WIDTH;	// 幅
	g_aResultUI[nIdx].fWidthDest = WIDTH;	// 幅の目的値
	g_aResultUI[nIdx].fHeightDest = 0.0f;		// 高さの目的値
	g_aResultUI[nIdx].nKey = 0;		// 現在のキー数
	g_aResultUI[nIdx].bDisp = true;		// 表示状態
}

//======================================================================================
// ResultUIを非表示状態にする
//======================================================================================
void SetResultUINonDisp(int nIdx)
{
	g_aResultUI[nIdx].state = RESULTUISTATE_NONDISPLAY;
	g_aResultUI[nIdx].fWidth = WIDTH;			// 幅
	g_aResultUI[nIdx].fWidthDest = WIDTH;		// 幅の目的値
	g_aResultUI[nIdx].fHeight = 0.0f;			// 高さ
	g_aResultUI[nIdx].fHeightDest = HEIGHT;	// 高さの目的値
	g_aResultUI[nIdx].nKey = 0;				// 現在のキー数
	g_aResultUI[nIdx].bDisp = false;			// 表示状態
}
#endif