//========================================================================
// 
// 3D塀の描画[BrickWall.cpp]
//
//========================================================================
#include "brickwall.h"
#include "input.h"
#include "color.h"

// マクロ定義
#define DIVISION_W	(16)				// 横の分割数
#define DIVISION_H	(1)					// 高さの分割数
#define WIDTH_SIZE	(DIVISION_W + 1)	// 横幅の大きさ
#define HEIGHT_SIZE	(DIVISION_H + 1)	// 奥行きの大きさ
#define PRIMITIVE	(DIVISION_W * DIVISION_H * 2 + (DIVISION_H - 1) * 4)	// プリミティブ数
#define INDEX		(PRIMITIVE + 2)					// インデックス数
#define VERTEX		(WIDTH_SIZE * HEIGHT_SIZE)		// 頂点数

#define SIZE		(500.0f)	// 高さ
#define MOVE		(1.5f)		// 移動量
#define ROTATE		(0.05f)		// 回転量
#define NORMAL		(D3DXVECTOR3(0.0f, 0.0f, -1.0f))	// 法線
#define DEFAULT		(D3DXVECTOR3(0.0f, 0.0f, 0.0f))		// デフォルト
#define POS			(D3DXVECTOR3(0.0f, -250.0f, 0.0f))	// 位置

// グローバル変数
LPDIRECT3DTEXTURE9 g_pTextureBrickWall = NULL;	// テクスチャへのポインタ
BrickWall g_BrickWall;

//========================================================================
// 塀の初期化処理
//========================================================================
void InitBrickWall(void)
{
	LPDIRECT3DDEVICE9 pDevice;	// デバイスへのポインタ
	// デバイスの取得
	pDevice = GetDevice();

	// テクスチャの読み込み
	D3DXCreateTextureFromFile(pDevice, "data\\TEXTURE\\sky001.jpg", &g_pTextureBrickWall);

	// 初期化
	g_BrickWall.pos = DEFAULT;
	g_BrickWall.rot = DEFAULT;
	g_BrickWall.pVtxBuff = NULL;
	g_BrickWall.pIdxBuff = NULL;
	g_BrickWall.bUse = false;

	// 頂点バッファの生成
	pDevice->CreateVertexBuffer(sizeof(VERTEX_3D) * VERTEX,	// 確保するバッファのサイズ
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_3D,									// 頂点フォーマット
		D3DPOOL_MANAGED,
		&g_BrickWall.pVtxBuff,
		NULL);

	VERTEX_3D* pVtx;
	// 頂点バッファをロックし、頂点情報へのポインタを取得
	g_BrickWall.pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	for (int nCntHeight = 0; nCntHeight < HEIGHT_SIZE; nCntHeight++, pVtx += WIDTH_SIZE)
	{
		for (int nCntWidth = 0; nCntWidth < WIDTH_SIZE; nCntWidth++)
		{
			// 頂点座標の設定
			pVtx[nCntWidth].pos.x = sinf(-D3DX_PI / (DIVISION_W / 2) * (nCntWidth)) * SIZE;
			pVtx[nCntWidth].pos.y = nCntHeight * SIZE;
			pVtx[nCntWidth].pos.z = cosf(-D3DX_PI / (DIVISION_W / 2) * (nCntWidth)) * SIZE;
			
			// テクスチャ座標の設定
			pVtx[nCntWidth].tex.x = (float)nCntWidth / DIVISION_W;
			pVtx[nCntWidth].tex.y = (float)nCntHeight / (DIVISION_H);

			// 法線の設定
			pVtx[nCntWidth].nor = NORMAL;

			// 頂点カラーの設定
			pVtx[nCntWidth].col = COLOR_WHITE;
		}
	}

	// 頂点バッファをアンロック
	g_BrickWall.pVtxBuff->Unlock();

	// インデックスバッファの生成
	pDevice->CreateIndexBuffer(sizeof(WORD) * INDEX,	// 確保するバッファのサイズ
		D3DUSAGE_WRITEONLY,
		D3DFMT_INDEX16,
		D3DPOOL_MANAGED,
		&g_BrickWall.pIdxBuff,
		NULL);

	WORD* pIdx;	// インデックス情報へのポインタ	
	// インデックスバッファをロックし、頂点番号へのポインタを取得
	g_BrickWall.pIdxBuff->Lock(0, 0, (void**)&pIdx, 0);

	// 頂点番号データを設定
	int nCntDepth = 0;	// 段ずれの回数をカウント

	for (int nCntVtx = 0; nCntVtx < INDEX / 2; nCntVtx++)
	{
		if (nCntVtx % (DIVISION_W + 2) == WIDTH_SIZE)
		{
			nCntDepth++;	// 段ずれの回数を追加
			pIdx[0] = nCntVtx - nCntDepth;
			pIdx[1] = nCntVtx - nCntDepth + (DIVISION_W + 2);
		}
		else
		{
			pIdx[0] = nCntVtx - nCntDepth + WIDTH_SIZE;
			pIdx[1] = nCntVtx - nCntDepth;
		}
		pIdx += 2;
	}

	// インデックスバッファをアンロック
	g_BrickWall.pIdxBuff->Unlock();

	// 空の配置
	SetBrickWall(POS, DEFAULT);
}

//========================================================================
// 塀の終了処理
//========================================================================
void UninitBrickWall(void)
{
	// テクスチャの破棄
	if (g_pTextureBrickWall != NULL)
	{
		g_pTextureBrickWall->Release();
		g_pTextureBrickWall = NULL;
	}
	
	// 頂点バッファの破棄
	if (g_BrickWall.pVtxBuff != NULL)
	{
		g_BrickWall.pVtxBuff->Release();
		g_BrickWall.pVtxBuff = NULL;
	}

	// インデックスバッファの破棄
	if (g_BrickWall.pIdxBuff != NULL)
	{
		g_BrickWall.pIdxBuff->Release();
		g_BrickWall.pIdxBuff = NULL;
	}
}

//========================================================================
// 塀の更新処理
//========================================================================
void UpdateBrickWall(void)
{
	
}

//========================================================================
// 塀の描画処理
//========================================================================
void DrawBrickWall(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();	// デバイスの取得
	D3DXMATRIX mtxRot, mtxTrans;	// 計算用マトリックス

	if (g_BrickWall.bUse == true)
	{
		// ワールドマトリックスの初期化(デフォルトの値にする)
		D3DXMatrixIdentity(&g_BrickWall.mtxWorld);

		// 向きを反映
		D3DXMatrixRotationYawPitchRoll(&mtxRot, g_BrickWall.rot.y, g_BrickWall.rot.x, g_BrickWall.rot.z);
		D3DXMatrixMultiply(&g_BrickWall.mtxWorld, &g_BrickWall.mtxWorld, &mtxRot);

		// 位置を反映
		D3DXMatrixTranslation(&mtxTrans, g_BrickWall.pos.x, g_BrickWall.pos.y, g_BrickWall.pos.z);
		D3DXMatrixMultiply(&g_BrickWall.mtxWorld, &g_BrickWall.mtxWorld, &mtxTrans);

		// ワールドマトリックスの設定
		pDevice->SetTransform(D3DTS_WORLD, &g_BrickWall.mtxWorld);

		// 頂点バッファをデータストリームに設定
		pDevice->SetStreamSource(0, g_BrickWall.pVtxBuff, 0, sizeof(VERTEX_3D));

		// インデックスバッファをデータストリームに設定
		pDevice->SetIndices(g_BrickWall.pIdxBuff);

		// 頂点フォーマットの設定
		pDevice->SetFVF(FVF_VERTEX_3D);

		// テクスチャの設定
		pDevice->SetTexture(0, g_pTextureBrickWall);

		// 塀の描画
		pDevice->DrawIndexedPrimitive(D3DPT_TRIANGLESTRIP,
			0,
			0,
			VERTEX,			// 必要な頂点数
			0,
			PRIMITIVE);		// 描画するプリミティブの数
	}
}

//========================================================================
// 塀の座標を渡す
//========================================================================
void SetBrickWall(D3DXVECTOR3 pos, D3DXVECTOR3 rot)
{
	if (g_BrickWall.bUse == false)
	{
		g_BrickWall.pos = pos;
		g_BrickWall.rot = rot;
		g_BrickWall.bUse = true;
	}
}

//========================================================================
// 塀の座標を渡す
//========================================================================
D3DXVECTOR3 GetBrickWallPos(void)
{
	return g_BrickWall.pos;
}