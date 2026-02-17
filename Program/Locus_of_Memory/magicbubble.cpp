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
#include "color.h"

// マクロ定義
#define MAXMAGICBUBBLE_TYPE	(MAGICBUBBLETYPE_MAX)	// テクスチャの最大数
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
	MAGICBUBBLETYPE type;		// 種類
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
	"data\\TEXTURE\\MagicBubble\\MagicBubble_100.png",	// ?
	"data\\TEXTURE\\MagicBubble\\MagicBubble_101.png",	// !
	"data\\TEXTURE\\MagicBubble\\MagicBubble_000.png",	// GGG
	"data\\TEXTURE\\MagicBubble\\MagicBubble_001.png",	// RRR
	"data\\TEXTURE\\MagicBubble\\MagicBubble_002.png",	// BBB
	"data\\TEXTURE\\MagicBubble\\MagicBubble_003.png",	// YYY
	"data\\TEXTURE\\MagicBubble\\MagicBubble_004.png",	// RRG
	"data\\TEXTURE\\MagicBubble\\MagicBubble_005.png",	// RGR
	"data\\TEXTURE\\MagicBubble\\MagicBubble_006.png",	// GRR
	"data\\TEXTURE\\MagicBubble\\MagicBubble_007.png",	// RYY
	"data\\TEXTURE\\MagicBubble\\MagicBubble_008.png",	// YRY
	"data\\TEXTURE\\MagicBubble\\MagicBubble_009.png",	// YYR
	"data\\TEXTURE\\MagicBubble\\MagicBubble_010.png",	// BBG
	"data\\TEXTURE\\MagicBubble\\MagicBubble_011.png",	// BGB
	"data\\TEXTURE\\MagicBubble\\MagicBubble_012.png",	// GBB
	"data\\TEXTURE\\MagicBubble\\MagicBubble_013.png",	// BGG
	"data\\TEXTURE\\MagicBubble\\MagicBubble_014.png",	// GBG
	"data\\TEXTURE\\MagicBubble\\MagicBubble_015.png",	// GGB
	"data\\TEXTURE\\MagicBubble\\MagicBubble_016.png",	// BYY
	"data\\TEXTURE\\MagicBubble\\MagicBubble_017.png",	// YBY
	"data\\TEXTURE\\MagicBubble\\MagicBubble_018.png",	// YYB
	"data\\TEXTURE\\MagicBubble\\MagicBubble_019.png",	// GGY
	"data\\TEXTURE\\MagicBubble\\MagicBubble_020.png",	// GYG
	"data\\TEXTURE\\MagicBubble\\MagicBubble_021.png",	// YGG
	"data\\TEXTURE\\MagicBubble\\MagicBubble_022.png",	// RGB
};

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

	// テクスチャの読み込み
	for (int nCntUI = 0; nCntUI < MAXMAGICBUBBLE_TYPE; nCntUI++)
	{
		D3DXCreateTextureFromFile(pDevice, c_apFilenameMagicBubble[nCntUI], &g_apTextureMagicBubble[nCntUI]);
	}

	// 初期化
	for (int nCntPlayer = 0; nCntPlayer < MAX_PLAYER; nCntPlayer++)
	{
		g_aMagicBubble[nCntPlayer].pos			= D3DXVECTOR3(BUBBLE_X, BUBBLE_Y, 0.0f);	// 位置
		g_aMagicBubble[nCntPlayer].rot			= D3DXVECTOR3(0.0f, D3DX_PI, 0.0f);			// 向き
		g_aMagicBubble[nCntPlayer].type			= MAGICBUBBLETYPE_FAR;	// 種類
		g_aMagicBubble[nCntPlayer].fWidth		= BUBBLE_WIDTH;			// 幅
		g_aMagicBubble[nCntPlayer].fWidthDest	= BUBBLE_WIDTH;			// 幅の目的値
		g_aMagicBubble[nCntPlayer].fHeight		= BUBBLE_HEIGHT;		// 高さ
		g_aMagicBubble[nCntPlayer].fHeightDest	= BUBBLE_HEIGHT;		// 高さの目的値
		g_aMagicBubble[nCntPlayer].nNumKey		= NUM_KEY;				// 浮遊感をカウントするキー数
		g_aMagicBubble[nCntPlayer].nKey			= 0;					// 現在のキー数
		g_aMagicBubble[nCntPlayer].bDisp		= false;				// 表示状態
	}

	// 頂点バッファの生成
	pDevice->CreateVertexBuffer(sizeof(VERTEX_3D) * 4 * MAX_PLAYER, D3DUSAGE_WRITEONLY, FVF_VERTEX_3D, D3DPOOL_MANAGED, &g_pVtxBuffMagicBubble, NULL);

	VERTEX_3D* pVtx;
	// 頂点バッファをロックし、頂点情報へのポインタを取得
	g_pVtxBuffMagicBubble->Lock(0, 0, (void**)&pVtx, 0);

	for (int nCntPlayer = 0; nCntPlayer < MAX_PLAYER; nCntPlayer++, pVtx += 4)
	{
		// 頂点座標の設定
		pVtx[0].pos = D3DXVECTOR3(-g_aMagicBubble[nCntPlayer].fWidth,  g_aMagicBubble[nCntPlayer].fHeight, 0.0f);
		pVtx[1].pos = D3DXVECTOR3( g_aMagicBubble[nCntPlayer].fWidth,  g_aMagicBubble[nCntPlayer].fHeight, 0.0f);
		pVtx[2].pos = D3DXVECTOR3(-g_aMagicBubble[nCntPlayer].fWidth, -g_aMagicBubble[nCntPlayer].fHeight, 0.0f);
		pVtx[3].pos = D3DXVECTOR3( g_aMagicBubble[nCntPlayer].fWidth, -g_aMagicBubble[nCntPlayer].fHeight, 0.0f);

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
	g_pVtxBuffMagicBubble->Unlock();
}

//======================================================================================
// spellの終了処理
//======================================================================================
void UninitMagicBubble(void)
{
	// テクスチャの破棄
	for (int nCntUI = 0; nCntUI < MAXMAGICBUBBLE_TYPE; nCntUI++)
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
	for (int nCntPlayer = 0; nCntPlayer < MAX_PLAYER; nCntPlayer++)
	{
		if ((GetKeyboardPress(DIK_TAB) == true && nCntPlayer == 0) || GetJoypadRightTriggePress(nCntPlayer) == true || GetJoypadLeftTriggePress(nCntPlayer) == true)
		{
			g_aMagicBubble[nCntPlayer].bDisp = false;
		}
	}
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
		if (g_aMagicBubble[nCntPlayer].bDisp == false)
		{
			continue;
		}
		// UIのマトリックス情報を取得
		UIMatrix = pPlayer->mtxWorld;

		// ワールドマトリックスの設定
		pDevice->SetTransform(D3DTS_WORLD, &UIMatrix);

		D3DXMATRIX	mtxRotModel, mtxTransModel;	// 計算用マトリックス
		D3DXMATRIX	mtxParent;					// 親のマトリックス

		// ポリゴンのワールドマトリックスを初期化
		D3DXMatrixIdentity(&g_aMagicBubble[nCntPlayer].mtxWorld);

		// パーツの位置を反映
		D3DXMatrixTranslation(&mtxTransModel, g_aMagicBubble[nCntPlayer].pos.x, g_aMagicBubble[nCntPlayer].pos.y, g_aMagicBubble[nCntPlayer].pos.z);
		D3DXMatrixMultiply(&g_aMagicBubble[nCntPlayer].mtxWorld, &g_aMagicBubble[nCntPlayer].mtxWorld, &mtxTransModel);

		// 向きを反映
		D3DXMatrixRotationYawPitchRoll(&mtxRot, g_aMagicBubble[nCntPlayer].rot.y, g_aMagicBubble[nCntPlayer].rot.x, g_aMagicBubble[nCntPlayer].rot.z);
		D3DXMatrixMultiply(&g_aMagicBubble[nCntPlayer].mtxWorld, &g_aMagicBubble[nCntPlayer].mtxWorld, &mtxRot);

		// 親マトリックスを設定
		mtxParent = UIMatrix;

		// 算出したパーツのワールドマトリックスと親モデルのマトリックスを掛け合わせる
		D3DXMatrixMultiply(&g_aMagicBubble[nCntPlayer].mtxWorld, &g_aMagicBubble[nCntPlayer].mtxWorld, &mtxParent);

		// パーツのワールドマトリックスを設定
		pDevice->SetTransform(D3DTS_WORLD, &g_aMagicBubble[nCntPlayer].mtxWorld);

		// 頂点バッファをデータストリームに設定
		pDevice->SetStreamSource(0, g_pVtxBuffMagicBubble, 0, sizeof(VERTEX_3D));

		// 頂点フォーマットの設定
		pDevice->SetFVF(FVF_VERTEX_3D);

		// テクスチャの設定
		pDevice->SetTexture(0, g_apTextureMagicBubble[g_aMagicBubble[nCntPlayer].type]);

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
void SetMagicBubble(int nIdx, COMMANDOREDER command, int nDistance)
{
	MAGICBUBBLETYPE type = MAGICBUBBLETYPE_FAR;	// テクスチャの種類を格納
	g_aMagicBubble[nIdx].bDisp = true;

	switch (nDistance)
	{
	case -1:	// 魔法の種類を取得してテクスチャを変更(取得可能状態)
		switch (command)
		{
		case COMMANDOREDER_NONE:	// コマンドがない場合
			g_aMagicBubble[nIdx].bDisp = false;
			type = MAGICBUBBLETYPE_FAR; 
			break;

		case COMMANDOREDER_GGG:	// GGG
			type = MAGICBUBBLETYPE_GGG;
			break;

		case COMMANDOREDER_RRR:	// RRR
			type = MAGICBUBBLETYPE_RRR;
			break;

		case COMMANDOREDER_BBB:	// BBB
			type = MAGICBUBBLETYPE_BBB;
			break;

		case COMMANDOREDER_YYY:	// YYY
			type = MAGICBUBBLETYPE_YYY;
			break;

		case COMMANDOREDER_RRG:	// RRG
			type = MAGICBUBBLETYPE_RRG;
			break;

		case COMMANDOREDER_RGR:	// RGR
			type = MAGICBUBBLETYPE_RGR;
			break;

		case COMMANDOREDER_GRR:	// GRR
			type = MAGICBUBBLETYPE_GRR;
			break;
		case COMMANDOREDER_RYY:	// RYY
			type = MAGICBUBBLETYPE_RYY;
			break;
		case COMMANDOREDER_YRY:	// YRY
			type = MAGICBUBBLETYPE_YRY;
			break;
		case COMMANDOREDER_YYR:	// YYR
			type = MAGICBUBBLETYPE_YYR;
			break;
		case COMMANDOREDER_BBG:	// BBG
			type = MAGICBUBBLETYPE_BBG;
			break;
		case COMMANDOREDER_BGB:	// BGB
			type = MAGICBUBBLETYPE_BGB;
			break;
		case COMMANDOREDER_GBB:	// GBB
			type = MAGICBUBBLETYPE_GBB;
			break;
		case COMMANDOREDER_BGG:	// BGG
			type = MAGICBUBBLETYPE_BGG;
			break;
		case COMMANDOREDER_GBG:	// GBG
			type = MAGICBUBBLETYPE_GBG;
			break;
		case COMMANDOREDER_GGB:	// GGB
			type = MAGICBUBBLETYPE_GGB;
			break;
		case COMMANDOREDER_BYY:	// BYY
			type = MAGICBUBBLETYPE_BYY;
			break;
		case COMMANDOREDER_YBY:	// YBY
			type = MAGICBUBBLETYPE_YBY;
			break;
		case COMMANDOREDER_YYB:	// YYB
			type = MAGICBUBBLETYPE_YYB;
			break;
		case COMMANDOREDER_GGY:	// GGY
			type = MAGICBUBBLETYPE_GGY;
			break;
		case COMMANDOREDER_GYG:	// GYG
			type = MAGICBUBBLETYPE_GYG;
			break;
		case COMMANDOREDER_YGG:	// YGG
			type = MAGICBUBBLETYPE_YGG;
			break;
		case COMMANDOREDER_RGB:	// RGB
			type = MAGICBUBBLETYPE_RGB;
			break;
		}
		break;

	case 0:	// 近距離に魔法が落ちている
		type = MAGICBUBBLETYPE_NEAR;
		break;

	case 1:	// 遠距離に魔法が落ちている
		type = MAGICBUBBLETYPE_FAR;
		break;

	default:	// 条件に合わない数字が格納されている場合
		type = MAGICBUBBLETYPE_FAR;
		g_aMagicBubble[nIdx].bDisp = false;
		break;
	}

	g_aMagicBubble[nIdx].type = type;
}

//======================================================================================
// 吹き出しの非表示
//======================================================================================
void ResetMagicBubble(int nIdx)
{
	g_aMagicBubble[nIdx].bDisp = false;
}
