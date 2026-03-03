////=============================================================================
////
////	オーラ処理 [aura.cpp]
////	Author : SHUMA AIZU
//// 
////=============================================================================
//
//#include "main.h"
//#include "aura.h"
//#include "input.h"
//
////*****************************************************************************
//// マクロ定義
////*****************************************************************************
//#define AURA_ROT				(D3DX_PI / (AURA_SPLIT_WIDHT - 1))
//
////*****************************************************************************
//// グローバル変数
////*****************************************************************************
//LPDIRECT3DTEXTURE9 g_pTextureAura = NULL;		// テクスチャへのポインタ
//Aura g_aAura[MAX_AURA];							// オーラの情報
//
////=============================================================================
////	オーラの初期化処理
////=============================================================================
//void InitAura(void)
//{
//	// デバイスの取得
//	LPDIRECT3DDEVICE9 pDevice = GetDevice();
//
//	// テクスチャの読み込み
//	D3DXCreateTextureFromFile(pDevice,
//		"data\\TEXTURE\\gradation101.jpg",
//		&g_pTextureAura);
//
//	for (int nCntAura = 0; nCntAura < MAX_AURA; nCntAura++)
//	{
//		g_aAura[nCntAura].pVtxBuff = NULL;
//		g_aAura[nCntAura].pIdxBuff = NULL;
//		g_aAura[nCntAura].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
//		g_aAura[nCntAura].rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
//		g_aAura[nCntAura].fRadius = AURA_RADIUS + (nCntAura * 15.0f);
//		g_aAura[nCntAura].fAngle = ((float)(rand() % 629 - 314) / 100);
//		g_aAura[nCntAura].bUse = false;
//
//		// 頂点バッファの生成
//		pDevice->CreateVertexBuffer(sizeof(VERTEX_3D) * AURA_SPLIT_WIDHT * AURA_SPLIT_HEIGTH,
//			D3DUSAGE_WRITEONLY,
//			FVF_VERTEX_3D,
//			D3DPOOL_MANAGED,
//			&g_aAura[nCntAura].pVtxBuff,
//			NULL);
//	}
//
//	// 初期化
//	VERTEX_3D *pVtx;			// 頂点情報へのポインタ
//
//	for (int nCntAura = 0; nCntAura < MAX_AURA; nCntAura++)
//	{
//		// 頂点バッファをロックし,頂点情報へのポインタを取得
//		g_aAura[nCntAura].pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);
//
//		for (int nCntHeight = 0; nCntHeight < AURA_SPLIT_HEIGTH; nCntHeight++)
//		{
//			float fRot = -D3DX_PI;
//			D3DXVECTOR3 vecDir;
//
//			for (int nCntVtxWidht = 0; nCntVtxWidht < AURA_SPLIT_WIDHT; nCntVtxWidht++)
//			{
//				// 頂点座標の設定
//				pVtx[nCntVtxWidht].pos = D3DXVECTOR3(sinf(fRot) * g_aAura[nCntAura].fRadius, -AURA_HEIGHT * nCntHeight, cosf(fRot) * g_aAura[nCntAura].fRadius);
//
//				// 法線ベクトルの設定
//				vecDir = D3DXVECTOR3(pVtx[nCntVtxWidht].pos.x, 0.0f, pVtx[nCntVtxWidht].pos.z);
//				D3DXVec3Normalize(&vecDir, &vecDir);
//				pVtx[nCntVtxWidht].nor = vecDir;
//
//				// 頂点カラーの設定
//				pVtx[nCntVtxWidht].col = D3DXCOLOR(1.0f, 1.0f, 0.58f, 1.0f);
//
//				// テクスチャ座標の設定
//				pVtx[nCntVtxWidht].tex = D3DXVECTOR2(0.25f * nCntVtxWidht, -0.49f * nCntHeight);
//
//				// 角度を加算
//				fRot -= AURA_ROT * 2;
//			}
//
//			// ポインタを進める
//			pVtx += AURA_SPLIT_WIDHT;
//		}
//		
//		// 頂点バッファをアンロックする
//		g_aAura[nCntAura].pVtxBuff->Unlock();
//	}
//
//	for (int nCntAura = 0; nCntAura < MAX_AURA; nCntAura++)
//	{
//		// インデックスバッファの設定
//		pDevice->CreateIndexBuffer(sizeof(WORD) * (AURA_SPLIT_WIDHT * 2 * (AURA_SPLIT_HEIGTH - 1) + ((AURA_SPLIT_HEIGTH - 2) * 2)),
//			D3DUSAGE_WRITEONLY,
//			D3DFMT_INDEX16,
//			D3DPOOL_MANAGED,
//			&g_aAura[nCntAura].pIdxBuff,
//			NULL);
//
//		WORD* pIdx;		// インデックス情報へのポインタ
//
//		// インデックスバッファをロックし、頂点情報へのポインタを取得
//		g_aAura[nCntAura].pIdxBuff->Lock(0, 0, (void**)&pIdx, 0);
//
//		for (int nCntIdxHeigth = 0; nCntIdxHeigth < AURA_SPLIT_HEIGTH - 1; nCntIdxHeigth++)
//		{
//			for (int nCntIdxWidht = 0; nCntIdxWidht < AURA_SPLIT_WIDHT; nCntIdxWidht++)
//			{
//				if (nCntIdxHeigth != 0 && nCntIdxWidht == 0)
//				{// 高さが最初の行以外かつ幅が最初の行
//					pIdx[0] = (AURA_SPLIT_WIDHT * (nCntIdxHeigth + 1)) + nCntIdxWidht;
//
//					pIdx++;
//				}
//
//				pIdx[0] = (AURA_SPLIT_WIDHT * (nCntIdxHeigth + 1)) + nCntIdxWidht;
//				pIdx[1] = AURA_SPLIT_WIDHT * nCntIdxHeigth + nCntIdxWidht;
//
//				pIdx += 2;
//
//				if (nCntIdxHeigth != AURA_SPLIT_HEIGTH - 2 && nCntIdxWidht == AURA_SPLIT_WIDHT - 1)
//				{// 高さが最後の行以外かつ幅が最後の行
//					pIdx[0] = AURA_SPLIT_WIDHT * nCntIdxHeigth + nCntIdxWidht;
//
//					pIdx++;
//				}
//			}
//		}
//
//		// インデックスバッファをアンロックする
//		g_aAura[nCntAura].pIdxBuff->Unlock();
//	}
//
//	SetAura(D3DXVECTOR3(0.0f, AURA_HEIGHT, 0.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f));
//	//SetAura(D3DXVECTOR3(0.0f, 450.0f, 0.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f));
//	//SetAura(D3DXVECTOR3(0.0f, 450.0f, 0.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f));
//}
//
////=============================================================================
////	オーラの終了処理
////=============================================================================
//void UninitAura(void)
//{
//	// テクスチャの破棄
//	if (g_pTextureAura != NULL)
//	{
//		g_pTextureAura->Release();
//		g_pTextureAura = NULL;
//	}
//	
//	for (int nCntAura = 0; nCntAura < MAX_AURA; nCntAura++)
//	{
//		// 頂点バッファの破棄
//		if (g_aAura[nCntAura].pVtxBuff != NULL)
//		{
//			g_aAura[nCntAura].pVtxBuff->Release();
//			g_aAura[nCntAura].pVtxBuff = NULL;
//		}
//
//		// インデックスバッファの破棄
//		if (g_aAura[nCntAura].pIdxBuff != NULL)
//		{
//			g_aAura[nCntAura].pIdxBuff->Release();
//			g_aAura[nCntAura].pIdxBuff = NULL;
//		}
//	}
//}
//
////=============================================================================
////	オーラの描画処理
////=============================================================================
//void DrawAura(void)
//{
//	LPDIRECT3DDEVICE9 pDevice = GetDevice();	// デバイスの取得
//	D3DXMATRIX mtxRot, mtxTrans;				// 計算用マトリックス
//
//	pDevice->SetRenderState(D3DRS_LIGHTING, FALSE);		// ライトを無効にする
//
//	//// アルファテストを有効にする
//	//pDevice->SetRenderState(D3DRS_ALPHATESTENABLE, TRUE);		// アルファテストを有効にする
//	//pDevice->SetRenderState(D3DRS_ALPHAFUNC, D3DCMP_GREATER);	// 比較方法(基準値より大きければ描画)
//	//pDevice->SetRenderState(D3DRS_ALPHAREF, 150);				// アルファテストの参照値を設定(～以上で描画, intで設定)
//
//	// αブレンディングを加算合成して設定
//	pDevice->SetRenderState(D3DRS_BLENDOP, D3DBLENDOP_ADD);
//	pDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
//	pDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_ONE);
//
//	//pDevice->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);				// カリングの設定
//
//	for (int nCntAura = 0; nCntAura < MAX_AURA; nCntAura++)
//	{
//		// ワールドマトリックスの初期化
//		D3DXMatrixIdentity(&g_aAura[nCntAura].mtxWorld);
//
//		// 向きを反映
//		D3DXMatrixRotationYawPitchRoll(&mtxRot, g_aAura[nCntAura].rot.y, g_aAura[nCntAura].rot.x, g_aAura[nCntAura].rot.z);
//		D3DXMatrixMultiply(&g_aAura[nCntAura].mtxWorld, &g_aAura[nCntAura].mtxWorld, &mtxRot);
//
//		// 位置を反映
//		D3DXMatrixTranslation(&mtxTrans, g_aAura[nCntAura].pos.x, g_aAura[nCntAura].pos.y, g_aAura[nCntAura].pos.z);
//		D3DXMatrixMultiply(&g_aAura[nCntAura].mtxWorld, &g_aAura[nCntAura].mtxWorld, &mtxTrans);
//
//		// ワールドマトリックスの設定
//		pDevice->SetTransform(D3DTS_WORLD, &g_aAura[nCntAura].mtxWorld);
//
//		// 頂点バッファをデータストリームに設定
//		pDevice->SetStreamSource(0, g_aAura[nCntAura].pVtxBuff, 0, sizeof(VERTEX_3D));
//
//		// インデックスバッファをデータストリームに設定
//		pDevice->SetIndices(g_aAura[nCntAura].pIdxBuff);
//
//		// 頂点フォーマットの設定
//		pDevice->SetFVF(FVF_VERTEX_3D);
//
//		// テクスチャの設定
//		pDevice->SetTexture(0, g_pTextureAura);
//
//		if (g_aAura[nCntAura].bUse == true)
//		{
//			// オーラの描画
//			pDevice->DrawIndexedPrimitive(D3DPT_TRIANGLESTRIP,
//				0,
//				0,
//				AURA_SPLIT_WIDHT * AURA_SPLIT_HEIGTH,
//				0,
//				((AURA_SPLIT_WIDHT - 1) * (AURA_SPLIT_HEIGTH - 1) * 2) + ((AURA_SPLIT_HEIGTH - 2) * 4));
//		}
//	}
//
//	// αブレンディングを戻す
//	pDevice->SetRenderState(D3DRS_BLENDOP, D3DBLENDOP_ADD);
//	pDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
//	pDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);
//
//	//// アルファテストを無効にする
//	//pDevice->SetRenderState(D3DRS_ALPHATESTENABLE, FALSE);		// アルファテストを無効化
//	//pDevice->SetRenderState(D3DRS_ALPHAFUNC, D3DCMP_ALWAYS);	// 比較方法(すべて描画)
//	//pDevice->SetRenderState(D3DRS_ALPHAREF, 255);				// 基準値を設定(すべてを描画している)
//
//	pDevice->SetRenderState(D3DRS_LIGHTING, TRUE);		// ライトを有効にする
//}
//
////=============================================================================
////	オーラの更新処理
////=============================================================================
//void UpdateAura(void)
//{
//	Aura* pAura = &g_aAura[0];
//
//	VERTEX_3D* pVtx;			// 頂点情報へのポインタ
//
//	for (int nCntAura = 0; nCntAura < MAX_AURA; nCntAura++, pAura++)
//	{
//		if (pAura->bUse == false)
//		{
//			continue;
//		}
//
//		pAura->fAngle += 0.025f;
//
//		pAura->rot.y += -0.0005f;
//
//		pAura->fAngle = AngleNormalize(pAura->fAngle);
//
//		pAura->rot.y = AngleNormalize(pAura->rot.y);
//
//		// 頂点バッファをロックし,頂点情報へのポインタを取得
//		g_aAura[nCntAura].pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);
//
//		for (int nCntHeight = 0; nCntHeight < AURA_SPLIT_HEIGTH; nCntHeight++)
//		{
//			float fRot = -D3DX_PI;
//			D3DXVECTOR3 vecDir;
//
//			for (int nCntVtxWidht = 0; nCntVtxWidht < AURA_SPLIT_WIDHT; nCntVtxWidht++)
//			{
//				// 頂点座標の設定
//				pVtx[nCntVtxWidht].pos = D3DXVECTOR3(sinf(fRot) * (g_aAura[nCntAura].fRadius - (50.0f * nCntHeight)), -AURA_HEIGHT * nCntHeight, cosf(fRot) * (g_aAura[nCntAura].fRadius - (50.0f * nCntHeight)));
//
//				if (nCntHeight == 0)
//				{
//					pVtx[nCntVtxWidht].pos.y = pVtx[nCntVtxWidht].pos.y + sinf(AngleNormalize(pAura->fAngle + (nCntVtxWidht * (AURA_ROT * 8)))) * 50.0f;
//				}
//
//				// 法線ベクトルの設定
//				vecDir = D3DXVECTOR3(pVtx[nCntVtxWidht].pos.x, 0.0f, pVtx[nCntVtxWidht].pos.z);
//				D3DXVec3Normalize(&vecDir, &vecDir);
//				pVtx[nCntVtxWidht].nor = vecDir;
//
//				// 頂点カラーの設定
//				pVtx[nCntVtxWidht].col = D3DXCOLOR(1.0f, 1.0f, 0.0f, 1.0f);
//
//				// テクスチャ座標の設定
//				pVtx[nCntVtxWidht].tex = D3DXVECTOR2(0.25f * nCntVtxWidht, 0.5f * nCntHeight);
//
//				// 角度を加算
//				fRot += AURA_ROT * 2;
//			}
//
//			// ポインタを進める
//			pVtx += AURA_SPLIT_WIDHT;
//		}
//
//		// 頂点バッファをアンロックする
//		g_aAura[nCntAura].pVtxBuff->Unlock();
//	}
//
//
//}
//
////=============================================================================
////	オーラの設定処理
////=============================================================================
//void SetAura(D3DXVECTOR3 pos, D3DXVECTOR3 rot)
//{
//	for (int nCntCylinder = 0; nCntCylinder < MAX_AURA; nCntCylinder++)
//	{
//		if (g_aAura[nCntCylinder].bUse == false)
//		{
//			// オーラの設定
//			g_aAura[nCntCylinder].pos = pos;
//			g_aAura[nCntCylinder].rot = rot;
//			g_aAura[nCntCylinder].bUse = true;
//			break;
//		}
//	}
//}