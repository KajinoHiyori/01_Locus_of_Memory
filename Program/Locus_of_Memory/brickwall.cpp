//========================================================================
// 
// 3Dメッシュスカイの描画[MeshSky.cpp]
//
//========================================================================
#include "meshsky.h"
#include "input.h"
#include "color.h"

// マクロ定義
#define DIVISION_W	(8)								// 横の分割数
#define DIVISION_H	(3)								// 高さの分割数
#define WIDTH_SIZE	(DIVISION_W + 1)				// 横幅の大きさ
#define HEIGHT_SIZE	(DIVISION_H + 1)					// 奥行きの大きさ
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
LPDIRECT3DTEXTURE9 g_pTextureMeshSky = NULL;	// テクスチャへのポインタ
MeshSky g_meshSky;

//========================================================================
// メッシュスカイの初期化処理
//========================================================================
void InitMeshSky(void)
{
	LPDIRECT3DDEVICE9 pDevice;	// デバイスへのポインタ
	// デバイスの取得
	pDevice = GetDevice();

	// テクスチャの読み込み
	D3DXCreateTextureFromFile(pDevice, "data\\TEXTURE\\sky001.jpg", &g_pTextureMeshSky);

	// 初期化
	g_meshSky.pos = DEFAULT;
	g_meshSky.rot = DEFAULT;
	g_meshSky.pVtxBuff = NULL;
	g_meshSky.pIdxBuff = NULL;
	g_meshSky.bUse = false;

	// 頂点バッファの生成
	pDevice->CreateVertexBuffer(sizeof(VERTEX_3D) * VERTEX,	// 確保するバッファのサイズ
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_3D,									// 頂点フォーマット
		D3DPOOL_MANAGED,
		&g_meshSky.pVtxBuff,
		NULL);

	VERTEX_3D* pVtx;
	// 頂点バッファをロックし、頂点情報へのポインタを取得
	g_meshSky.pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	for (int nCntHeight = RESET_DATA; nCntHeight < HEIGHT_SIZE; nCntHeight++, pVtx += WIDTH_SIZE)
	{
		for (int nCntWidth = RESET_DATA; nCntWidth < WIDTH_SIZE; nCntWidth++)
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
	g_meshSky.pVtxBuff->Unlock();

	// インデックスバッファの生成
	pDevice->CreateIndexBuffer(sizeof(WORD) * INDEX,	// 確保するバッファのサイズ
		D3DUSAGE_WRITEONLY,
		D3DFMT_INDEX16,
		D3DPOOL_MANAGED,
		&g_meshSky.pIdxBuff,
		NULL);

	WORD* pIdx;	// インデックス情報へのポインタ	
	// インデックスバッファをロックし、頂点番号へのポインタを取得
	g_meshSky.pIdxBuff->Lock(0, 0, (void**)&pIdx, 0);

	// 頂点番号データを設定
	int nCntDepth = RESET_DATA;	// 段ずれの回数をカウント

	for (int nCntVtx = RESET_DATA; nCntVtx < INDEX / 2; nCntVtx++)
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
	g_meshSky.pIdxBuff->Unlock();

	// 空の配置
	SetMeshSky(POS, DEFAULT);
}

//========================================================================
// メッシュスカイの終了処理
//========================================================================
void UninitMeshSky(void)
{
	// テクスチャの破棄
	if (g_pTextureMeshSky != NULL)
	{
		g_pTextureMeshSky->Release();
		g_pTextureMeshSky = NULL;
	}
	
	// 頂点バッファの破棄
	if (g_meshSky.pVtxBuff != NULL)
	{
		g_meshSky.pVtxBuff->Release();
		g_meshSky.pVtxBuff = NULL;
	}

	// インデックスバッファの破棄
	if (g_meshSky.pIdxBuff != NULL)
	{
		g_meshSky.pIdxBuff->Release();
		g_meshSky.pIdxBuff = NULL;
	}
}

//========================================================================
// メッシュスカイの更新処理
//========================================================================
void UpdateMeshSky(void)
{
	
}

//========================================================================
// メッシュスカイの描画処理
//========================================================================
void DrawMeshSky(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();	// デバイスの取得
	D3DXMATRIX mtxRot, mtxTrans;	// 計算用マトリックス

	if (g_meshSky.bUse == true)
	{
		// ワールドマトリックスの初期化(デフォルトの値にする)
		D3DXMatrixIdentity(&g_meshSky.mtxWorld);

		// 向きを反映
		D3DXMatrixRotationYawPitchRoll(&mtxRot, g_meshSky.rot.y, g_meshSky.rot.x, g_meshSky.rot.z);
		D3DXMatrixMultiply(&g_meshSky.mtxWorld, &g_meshSky.mtxWorld, &mtxRot);

		// 位置を反映
		D3DXMatrixTranslation(&mtxTrans, g_meshSky.pos.x, g_meshSky.pos.y, g_meshSky.pos.z);
		D3DXMatrixMultiply(&g_meshSky.mtxWorld, &g_meshSky.mtxWorld, &mtxTrans);

		// ワールドマトリックスの設定
		pDevice->SetTransform(D3DTS_WORLD, &g_meshSky.mtxWorld);

		// 頂点バッファをデータストリームに設定
		pDevice->SetStreamSource(0, g_meshSky.pVtxBuff, 0, sizeof(VERTEX_3D));

		// インデックスバッファをデータストリームに設定
		pDevice->SetIndices(g_meshSky.pIdxBuff);

		// 頂点フォーマットの設定
		pDevice->SetFVF(FVF_VERTEX_3D);

		// テクスチャの設定
		pDevice->SetTexture(0, g_pTextureMeshSky);

		// メッシュスカイの描画
		pDevice->DrawIndexedPrimitive(D3DPT_TRIANGLESTRIP,
			0,
			0,
			VERTEX,			// 必要な頂点数
			0,
			PRIMITIVE);		// 描画するプリミティブの数
	}
}

//========================================================================
// メッシュスカイの座標を渡す
//========================================================================
void SetMeshSky(D3DXVECTOR3 pos, D3DXVECTOR3 rot)
{
	if (g_meshSky.bUse == false)
	{
		g_meshSky.pos = pos;
		g_meshSky.rot = rot;
		g_meshSky.bUse = true;
	}
}

//========================================================================
// メッシュスカイの座標を渡す
//========================================================================
D3DXVECTOR3 GetMeshSkyPos(void)
{
	return g_meshSky.pos;
}