//========================================================
// 
// 魔法発動状態のUI表示処理[magicbubble.cpp]
// Author : KajinoHiyori
// 
//========================================================
#include "main.h"
#include "gimmck.h"
#include "player.h"
#include "game.h"
#include "input.h"
#include "title.h"
#include "fog.h"
#include "debugproc.h"
#include "color.h"

// マクロ定義
#define GIMMCK_TYPE			(1)						// テクスチャの最大数
#define GIMMCK_WIDTH		(28.0f)					// 吹き出しの幅
#define GIMMCK_HEIGHT		(10.0f)					// 吹き出しの高さ
#define GIMMCK_X			(-10.0f)				// 吹き出しのX軸
#define GIMMCK_Y			(85.0f)					// 吹き出しのY高度
#define NUM_KEY				(30)					// 処理を行うキー数
#define NORMAL				(D3DXVECTOR3(0.0f, 1.0f, 0.0f))	// 法線ベクトル

// MAGICBUBBLEの構造体
typedef struct
{
	D3DXMATRIX		mtxWorld;	// ワールドマトリックス
	D3DXVECTOR3		pos;		// 位置
	D3DXVECTOR3		rot;		// 向き
	GIMMICKTYPE type;			// 種類
	EVENTOBJECTTYPE	eType;	// 設置するコマンドの種類
	float	fWidth;			// 幅
	float	fWidthDest;		// 幅の目的値
	float	fHeight;		// 高さ
	float	fHeightDest;	// 高さの目的値
	int		nNumKey;		// 浮遊感をカウントするキー数
	int		nKey;			// 現在のキー数
	bool	bDisp;			// 表示状態
}Gimmick;

// グローバル変数
LPDIRECT3DTEXTURE9	g_apTextureGimmick[GIMMCK_TYPE] = {};	// テクスチャへのポインタ
LPDIRECT3DVERTEXBUFFER9	g_pVtxBuffGimmick = NULL;			// 頂点バッファへのポインタ
Gimmick g_aGimmick[MAX_PLAYER];		// MAGICBUBBLEの全体管理

// テクスチャの読み込み
const char* c_apFilenameGimmick[GIMMCK_TYPE] =
{
	"data\\TEXTURE\\Gimmick\\Gimmick_101.png",	// !
};

//======================================================================================
// spellの初期化処理
//======================================================================================
void InitGimmick(void)
{
	// デバイスの取得
	LPDIRECT3DDEVICE9 pDevice = GetDevice();
	// 操作人数の取得
	OPERATIONTYPE operationType = GetOperationType();
	// プレイヤーの情報を取得
	Player* pPlayer = GetPlayer();

	// テクスチャの読み込み
	for (int nCntUI = 0; nCntUI < GIMMCK_TYPE; nCntUI++)
	{
		if (g_apTextureGimmick[nCntUI] == NULL)
		{
			D3DXCreateTextureFromFile(pDevice, c_apFilenameGimmick[nCntUI], &g_apTextureGimmick[nCntUI]);
		}
	}

	// 初期化
	for (int nCntPlayer = 0; nCntPlayer < MAX_PLAYER; nCntPlayer++)
	{
		g_aGimmick[nCntPlayer].pos			= D3DXVECTOR3(GIMMCK_X, GIMMCK_Y, 0.0f);	// 位置
		g_aGimmick[nCntPlayer].rot			= D3DXVECTOR3(0.0f, D3DX_PI, 0.0f);			// 向き
		g_aGimmick[nCntPlayer].type			= GIMMICKTYPE_NONE;		// 種類
		g_aGimmick[nCntPlayer].eType		= EVENTOBJECTTYPE_NONE;	// コマンドの種類
		g_aGimmick[nCntPlayer].fWidth		= GIMMCK_WIDTH;			// 幅
		g_aGimmick[nCntPlayer].fWidthDest	= GIMMCK_WIDTH;			// 幅の目的値
		g_aGimmick[nCntPlayer].fHeight		= GIMMCK_HEIGHT;		// 高さ
		g_aGimmick[nCntPlayer].fHeightDest	= GIMMCK_HEIGHT;		// 高さの目的値
		g_aGimmick[nCntPlayer].nNumKey		= NUM_KEY;				// 浮遊感をカウントするキー数
		g_aGimmick[nCntPlayer].nKey			= 0;					// 現在のキー数
		g_aGimmick[nCntPlayer].bDisp		= false;				// 表示状態
	}

	// 頂点バッファの生成
	pDevice->CreateVertexBuffer(sizeof(VERTEX_3D) * 4 * MAX_PLAYER, D3DUSAGE_WRITEONLY, FVF_VERTEX_3D, D3DPOOL_MANAGED, &g_pVtxBuffGimmick, NULL);

	VERTEX_3D* pVtx;
	// 頂点バッファをロックし、頂点情報へのポインタを取得
	g_pVtxBuffGimmick->Lock(0, 0, (void**)&pVtx, 0);

	for (int nCntPlayer = 0; nCntPlayer < MAX_PLAYER; nCntPlayer++, pVtx += 4)
	{
		// 頂点座標の設定
		pVtx[0].pos = D3DXVECTOR3(-g_aGimmick[nCntPlayer].fWidth,  g_aGimmick[nCntPlayer].fHeight, 0.0f);
		pVtx[1].pos = D3DXVECTOR3( g_aGimmick[nCntPlayer].fWidth,  g_aGimmick[nCntPlayer].fHeight, 0.0f);
		pVtx[2].pos = D3DXVECTOR3(-g_aGimmick[nCntPlayer].fWidth, -g_aGimmick[nCntPlayer].fHeight, 0.0f);
		pVtx[3].pos = D3DXVECTOR3( g_aGimmick[nCntPlayer].fWidth, -g_aGimmick[nCntPlayer].fHeight, 0.0f);

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
	g_pVtxBuffGimmick->Unlock();
}

//======================================================================================
// spellの終了処理
//======================================================================================
void UninitGimmick(void)
{
	// テクスチャの破棄
	for (int nCntUI = 0; nCntUI < GIMMCK_TYPE; nCntUI++)
	{
		if (g_apTextureGimmick[nCntUI] != NULL)
		{
			g_apTextureGimmick[nCntUI]->Release();
			g_apTextureGimmick[nCntUI] = NULL;
		}
	}

	// 頂点バッファの破棄
	if (g_pVtxBuffGimmick != NULL)
	{
		g_pVtxBuffGimmick->Release();
		g_pVtxBuffGimmick = NULL;
	}

}

//======================================================================================
// spellの更新処理
//======================================================================================
void UpdateGimmick(void)
{
	Player* pPlayer = GetPlayer();
	bool bDisp = false;	// 吹き出しの表示を管理

	for (int nCntPlayer = 0; nCntPlayer < MAX_PLAYER; nCntPlayer++, pPlayer++)
	{
		bDisp = false;
		if (pPlayer->state == PLAYERSTATE_PAUSE || pPlayer->state == PLAYERSTATE_SPELL || pPlayer->state == PLAYERSTATE_MAGIC)
		{
			g_aGimmick[nCntPlayer].bDisp = false;
		}
		if (pPlayer->bUse == true)
		{
			for (int nCntMagic = 0; nCntMagic < MAX_OWNCOMMAND; nCntMagic++)
			{
				// もしプレイヤーが同じ魔法を持っている場合は処理を行わない
				if (pPlayer->magicbook.OwnCommand[nCntMagic] == g_aGimmick[nCntPlayer].eType && pPlayer->magicbook.OwnCommand[nCntMagic] != COMMANDOREDER_NONE)
				{
					g_aGimmick[nCntPlayer].bDisp = false;
					break;
				}
			}

			g_aGimmick[nCntPlayer].pos = pPlayer->pos;
			g_aGimmick[nCntPlayer].pos.x += sinf(pPlayer->rot.y) * GIMMCK_X;
			g_aGimmick[nCntPlayer].pos.z += cosf(pPlayer->rot.y) * GIMMCK_X;
			g_aGimmick[nCntPlayer].pos.y += GIMMCK_Y;
		}
	}
}

//======================================================================================
// spellの描画処理
//======================================================================================
void DrawGimmick(void)
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
		if (g_aGimmick[nCntPlayer].bDisp == false)
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
		D3DXMatrixIdentity(&g_aGimmick[nCntPlayer].mtxWorld);

		// ビューマトリックスを取得する
		pDevice->GetTransform(D3DTS_VIEW, &mtxView);

		// ポリゴンをカメラに対して正面に向ける
		D3DXMatrixInverse(&g_aGimmick[nCntPlayer].mtxWorld, NULL, &mtxView);	//逆行列を求める

		g_aGimmick[nCntPlayer].mtxWorld._41 = 0.0f;		//マトリックス(行列)の内容
		g_aGimmick[nCntPlayer].mtxWorld._42 = 0.0f;
		g_aGimmick[nCntPlayer].mtxWorld._43 = 0.0f;

		// パーツの位置を反映
		D3DXMatrixTranslation(&mtxTransModel, g_aGimmick[nCntPlayer].pos.x, g_aGimmick[nCntPlayer].pos.y, g_aGimmick[nCntPlayer].pos.z);
		D3DXMatrixMultiply(&g_aGimmick[nCntPlayer].mtxWorld, &g_aGimmick[nCntPlayer].mtxWorld, &mtxTransModel);

		// パーツのワールドマトリックスを設定
		pDevice->SetTransform(D3DTS_WORLD, &g_aGimmick[nCntPlayer].mtxWorld);

		// 頂点バッファをデータストリームに設定
		pDevice->SetStreamSource(0, g_pVtxBuffGimmick, 0, sizeof(VERTEX_3D));

		// 頂点フォーマットの設定
		pDevice->SetFVF(FVF_VERTEX_3D);

		// テクスチャの設定
		pDevice->SetTexture(0, g_apTextureGimmick[g_aGimmick[nCntPlayer].type]);

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
// 吹き出しの配置
//======================================================================================
void SetGimmick(int nIdx, EVENTOBJECTTYPE eType, int nDistance)
{
	GIMMICKTYPE type = GIMMICKTYPE_NORMAL;	// テクスチャの種類を格納
	g_aGimmick[nIdx].bDisp = true;
	g_aGimmick[nIdx].eType = eType;
	switch (nDistance)
	{
	case -1:	// 魔法の種類を取得してテクスチャを変更(取得可能状態)
		switch (eType)
		{
		case EVENTOBJECTTYPE_NONE:	// コマンドがない場合
			g_aGimmick[nIdx].bDisp = false;
			type = GIMMICKTYPE_NONE;
			break;

		case EVENTOBJECTTYPE_NORMAL:	// GGG
			type = GIMMICKTYPE_NORMAL;
			break;

		case EVENTOBJECTTYPE_PARENT:	// RRR
			type = GIMMICKTYPE_PARENT;
			break;
		}
		break;

	//case 0:	// 近距離に魔法が落ちている
	//	type = GIMMICKTYPE_NEAR;
	//	break;

	//case 1:	// 遠距離に魔法が落ちている
	//	type = GIMMICKTYPE_FAR;
	//	break;

	//default:	// 条件に合わない数字が格納されている場合
	//	type = GIMMICKTYPE_FAR;
	//	g_aGimmick[nIdx].bDisp = false;
	//	break;
	}

	g_aGimmick[nIdx].type = type;
}

//======================================================================================
// 吹き出しの非表示
//======================================================================================
void ResetGimmick(int nIdx)
{
	g_aGimmick[nIdx].bDisp = false;
}
