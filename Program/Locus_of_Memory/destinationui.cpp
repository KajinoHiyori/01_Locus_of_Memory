//========================================================
// 
// 魔法の位置に合わせたアイコン表示処理[questionmark.cpp]
// Author : KajinoHiyori
// 
//========================================================
#include "main.h"
#include "destinationui.h"
#include "debugproc.h"
#include "player.h"
#include "game.h"
#include "input.h"
#include "title.h"
#include "tutorial.h"
#include "fog.h"
#include "debugproc.h"
#include "color.h"
#include "magiceffect.h"
#include "goal.h"

// テクスチャの種類
typedef enum
{
	ARROWTYPE_TUTORIAL = 0,	// チュートリアル
	ARROWTYPE_GAME,			// ゲーム
	ARROWTYPE_MAX
}ARROWTYPE;

// マクロ定義
#define DESTINATION_TYPE	(ARROWTYPE_MAX)		// テクスチャの最大数
#define MARK_WIDTH		(15.0f)		// 吹き出しの幅
#define MARK_HEIGHT		(30.0f)		// 吹き出しの高さ
#define MARK_X			(30.0f)		// 吹き出しのX軸
#define MARK_Y			(95.0f)		// 吹き出しのY高度
#define DISTANCE		(70.0f)		// 処理を行うキー数
#define NONDISP			(500.0f)	// これ以上離れていたら表示しない
#define DISP			(100.0f)	// アルファ値1.0fで表示
#define MAGICEF_SIZE	(75.0f)		// マジックエフェクトの配置サイズ
#define NORMAL			(D3DXVECTOR3(0.0f, 1.0f, 0.0f))	// 法線ベクトル

// MAGICMARKの構造体
typedef struct
{
	D3DXMATRIX		mtxWorld;		// ワールドマトリックス
	D3DXVECTOR3		pos;			// 位置
	D3DXVECTOR3		posDest;		// 目的位置
	D3DXVECTOR3		rot;			// 位置
	D3DXVECTOR3		Offset;			// 位置
	ARROWTYPE		type;			// 位置
	float			fWidth;			// 幅
	float			fHeight;		// 高さ
	bool			bDisp;			// 表示状態
}Destination;

// グローバル変数
LPDIRECT3DTEXTURE9	g_apTextureDestination[DESTINATION_TYPE] = {};	// テクスチャへのポインタ
LPDIRECT3DVERTEXBUFFER9	g_pVtxBuffDestination = NULL;			// 頂点バッファへのポインタ
Destination g_aDestination[MAX_PLAYER];		// MAGICMARKの全体管理

// テクスチャの読み込み
const char* c_apFilenameDestination[DESTINATION_TYPE] =
{
	"data\\TEXTURE\\arrow000.png",	// tutorial
	"data\\TEXTURE\\arrow001.png",	// game
};

//======================================================================================
// questionmarkの初期化処理
//======================================================================================
void InitDestinationUI(void)
{
	// デバイスの取得
	LPDIRECT3DDEVICE9 pDevice = GetDevice();
	// 操作人数の取得
	OPERATIONTYPE operationType = GetOperationType();
	// プレイヤーの情報を取得
	Player* pPlayer = GetPlayer();
	// 現在のモードを取得
	MODE mode = GetMode();

	// テクスチャの読み込み
	for (int nCntUI = 0; nCntUI < DESTINATION_TYPE; nCntUI++)
	{
		if (g_apTextureDestination[nCntUI] == NULL)
		{
			D3DXCreateTextureFromFile(pDevice, c_apFilenameDestination[nCntUI], &g_apTextureDestination[nCntUI]);
		}
	}

	// 初期化
	for (int nCntPlayer = 0; nCntPlayer < MAX_PLAYER; nCntPlayer++)
	{
		g_aDestination[nCntPlayer].pos	= D3DXVECTOR3(50.0f, MARK_Y, 50.0f);	// 位置
		g_aDestination[nCntPlayer].rot = D3DXVECTOR3(D3DX_PI / 2, 0.0f, 0.0f);	// 位置
		g_aDestination[nCntPlayer].Offset = INIT_D3DXVEC3;	// 位置
		g_aDestination[nCntPlayer].fWidth = MARK_WIDTH;	// 幅
		g_aDestination[nCntPlayer].fHeight = MARK_HEIGHT;	// 高さ
		g_aDestination[nCntPlayer].bDisp = true;			// 表示状態

		switch (mode)
		{
		case MODE_TITLE:
			g_aDestination[nCntPlayer].posDest = GATE_POS;	// 位置
			g_aDestination[nCntPlayer].type = ARROWTYPE_TUTORIAL;
			break;

		case MODE_TUTORIAL:
			g_aDestination[nCntPlayer].posDest = GOAL_POS;	// 位置
			g_aDestination[nCntPlayer].type = ARROWTYPE_GAME;
			break;
		}
	}

	// 頂点バッファの生成
	pDevice->CreateVertexBuffer(sizeof(VERTEX_3D) * 4 * MAX_PLAYER, D3DUSAGE_WRITEONLY, FVF_VERTEX_3D, D3DPOOL_MANAGED, &g_pVtxBuffDestination, NULL);

	VERTEX_3D* pVtx;
	// 頂点バッファをロックし、頂点情報へのポインタを取得
	g_pVtxBuffDestination->Lock(0, 0, (void**)&pVtx, 0);

	for (int nCntPlayer = 0; nCntPlayer < MAX_PLAYER; nCntPlayer++, pVtx += 4)
	{
		// 頂点座標の設定
		pVtx[0].pos = D3DXVECTOR3(-g_aDestination[nCntPlayer].fWidth, g_aDestination[nCntPlayer].fHeight, 0.0f);
		pVtx[1].pos = D3DXVECTOR3(g_aDestination[nCntPlayer].fWidth, g_aDestination[nCntPlayer].fHeight, 0.0f);
		pVtx[2].pos = D3DXVECTOR3(-g_aDestination[nCntPlayer].fWidth, -g_aDestination[nCntPlayer].fHeight, 0.0f);
		pVtx[3].pos = D3DXVECTOR3(g_aDestination[nCntPlayer].fWidth, -g_aDestination[nCntPlayer].fHeight, 0.0f);

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
	g_pVtxBuffDestination->Unlock();
}

//======================================================================================
// questionmarkの終了処理
//======================================================================================
void UninitDestinationUI(void)
{
	// テクスチャの破棄
	for (int nCntUI = 0; nCntUI < DESTINATION_TYPE; nCntUI++)
	{
		if (g_apTextureDestination[nCntUI] != NULL)
		{
			g_apTextureDestination[nCntUI]->Release();
			g_apTextureDestination[nCntUI] = NULL;
		}
	}

	// 頂点バッファの破棄
	if (g_pVtxBuffDestination != NULL)
	{
		g_pVtxBuffDestination->Release();
		g_pVtxBuffDestination = NULL;
	}

}

//======================================================================================
// questionmarkの更新処理
//======================================================================================
void UpdateDestinationUI(void)
{
	Player* pPlayer = GetPlayer();
	DropMagic* pDropMagic = GetDropMagic();	// 落ちている魔法を取得

	for (int nCntPlayer = 0; nCntPlayer < MAX_PLAYER; nCntPlayer++, pPlayer++)
	{
		if (pPlayer->bUse == false)
		{
			g_aDestination[nCntPlayer].bDisp = false;
			continue;
		}
		else
		{
			g_aDestination[nCntPlayer].bDisp = true;
		}

		// 位置の更新
		g_aDestination[nCntPlayer].Offset = pPlayer->pos;
		g_aDestination[nCntPlayer].Offset.y -= 80.0f;

		// 距離を取得
		float fDistance = DistanceDestination(nCntPlayer);

		// 目的の向きに合わせて表示位置を変更
		UpdateArrowPos(nCntPlayer);

		g_aDestination[0].pos.x = sinf(g_aDestination[nCntPlayer].rot.y) * DISTANCE;
		g_aDestination[0].pos.z = cosf(g_aDestination[nCntPlayer].rot.y) * DISTANCE;

		PrintDebugProc("目的地の位置 : (%f, %f, %f)\n", g_aDestination[nCntPlayer].pos.x, g_aDestination[nCntPlayer].pos.y, g_aDestination[nCntPlayer].pos.z);
		PrintDebugProc("目的地の向き : (%f, %f, %f)\n", g_aDestination[nCntPlayer].rot.x, g_aDestination[nCntPlayer].rot.y, g_aDestination[nCntPlayer].rot.z);
	}
}

//======================================================================================
// questionmarkの描画処理
//======================================================================================
void DrawDestinationUI(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();	// デバイスの取得
	// UIのマトリックス情報を取得
	D3DXMATRIX UIMatrix, mtxView, mtxParentTrans, mtxRot;
	D3DXMATRIX	mtxParent;					// 親のマトリックス

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
		if (g_aDestination[nCntPlayer].bDisp == false)
		{
			continue;
		}

		// ポリゴンのワールドマトリックスを初期化
		D3DXMatrixIdentity(&UIMatrix);

		// パーツの位置を反映
		D3DXMatrixTranslation(&mtxParentTrans, g_aDestination[nCntPlayer].Offset.x, g_aDestination[nCntPlayer].Offset.y, g_aDestination[nCntPlayer].Offset.z);
		D3DXMatrixMultiply(&UIMatrix, &UIMatrix, &mtxParentTrans);

		// ワールドマトリックスの設定
		pDevice->SetTransform(D3DTS_WORLD, &UIMatrix);

		// UIのマトリックス情報を取得
		mtxParent = UIMatrix;

		// ワールドマトリックスの設定
		pDevice->SetTransform(D3DTS_WORLD, &mtxParent);

		D3DXMATRIX	mtxRotModel, mtxTransModel;	// 計算用マトリックス

		// ポリゴンのワールドマトリックスを初期化
		D3DXMatrixIdentity(&g_aDestination[nCntPlayer].mtxWorld);

		// ビューマトリックスを取得する
		// 向きを反映
		D3DXMatrixRotationYawPitchRoll(&mtxRot, g_aDestination[nCntPlayer].rot.y, g_aDestination[nCntPlayer].rot.x, g_aDestination[nCntPlayer].rot.z);
		D3DXMatrixMultiply(&g_aDestination[nCntPlayer].mtxWorld, &g_aDestination[nCntPlayer].mtxWorld, &mtxRot);

		// パーツの位置を反映
		D3DXMatrixTranslation(&mtxTransModel, g_aDestination[nCntPlayer].pos.x, g_aDestination[nCntPlayer].pos.y, g_aDestination[nCntPlayer].pos.z);
		D3DXMatrixMultiply(&g_aDestination[nCntPlayer].mtxWorld, &g_aDestination[nCntPlayer].mtxWorld, &mtxTransModel);

		// 算出したパーツのワールドマトリックスと親モデルのマトリックスを掛け合わせる
		D3DXMatrixMultiply(&g_aDestination[nCntPlayer].mtxWorld, &g_aDestination[nCntPlayer].mtxWorld, &mtxParent);

		// パーツのワールドマトリックスを設定
		pDevice->SetTransform(D3DTS_WORLD, &g_aDestination[nCntPlayer].mtxWorld);

		// 頂点バッファをデータストリームに設定
		pDevice->SetStreamSource(0, g_pVtxBuffDestination, 0, sizeof(VERTEX_3D));

		// 頂点フォーマットの設定
		pDevice->SetFVF(FVF_VERTEX_3D);

		// テクスチャの設定
		pDevice->SetTexture(0, g_apTextureDestination[g_aDestination[nCntPlayer].type]);

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
// 1番近くに落ちている魔法との距離を返す
//======================================================================================
float DistanceDestination(int nIdx)
{
	float fLength = 1000000.0f;	// 魔法との距離を保存
	float fCompare = 0.0f;	// 比較用の値を代入
	Player* pPlayer = GetPlayer();


	fCompare = sqrtf(((pPlayer[nIdx].pos.x - g_aDestination[nIdx].posDest.x) * (pPlayer[nIdx].pos.x - g_aDestination[nIdx].posDest.x)) + ((pPlayer[nIdx].pos.z - g_aDestination[nIdx].posDest.z) * (pPlayer[nIdx].pos.z - g_aDestination[nIdx].posDest.z))) * 0.5f;

	fLength = fCompare;

	return fLength;
}

//======================================================================================
// 魔法の位置に合わせて?の位置を変更する
//======================================================================================
void UpdateArrowPos(int nIdx)
{
	DropMagic* pDropMagic = GetDropMagic();	// 落ちている魔法を取得

	float fRot = atan2f(g_aDestination[nIdx].posDest.x - g_aDestination[nIdx].Offset.x, g_aDestination[nIdx].posDest.z - g_aDestination[nIdx].Offset.z);
	g_aDestination[nIdx].rot.y = AngleNormalize(fRot);

	// 表示位置の変更
	g_aDestination[nIdx].pos.x = sinf(g_aDestination[nIdx].rot.y) * DISTANCE;
	g_aDestination[nIdx].pos.z = cosf(g_aDestination[nIdx].rot.y) * DISTANCE;
}
