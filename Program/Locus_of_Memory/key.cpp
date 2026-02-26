//======================================================================================
// 
// 3Dアクションゲーム_MASTER KEY[Key.cpp]
//
//======================================================================================
#if 0
#include "main.h"
#include "key.h"
#include "player.h"
#include "meshring.h"
#include "meshfield.h"
#include "particle.h"
#include "block.h"
#include "shadow.h"
#include "color.h"
#include "sound.h"
#include "input.h"

// マクロ定義
#define NUM_KEY			(KEYTYPE_MAX)	// モデルの数
#define SHADOW			(10.0f)			// 影の大きさ
#define BLANK			(0.001f)		// 空白
#define PARTICLE		(30)			// パーティクルの寿命
#define GRAVITY			(0.75f)			// 重力
#define ANGLE_DOUBLE	(629)			// 角度の最大数(*2)
#define MAX_ANGLE		(314)			// 角度の最大数
#define ANGLE_ADJUST	(100.0f)		// 角度の値を指定
#define DEFALT			(D3DXVECTOR3(0.0f, 0.0f, 0.0f))					// xyzが0.0fの場合
#define NORMAL			(D3DXVECTOR3(0.0f, 1.0f, 0.0f))					// 基本の法線
#define VTX_MIN			(D3DXVECTOR3(10000.0f, 10000.0f, 10000.0f))		// カギの大きさの初期化値(最小)
#define VTX_MAX			(D3DXVECTOR3(-10000.0f, -10000.0f, -10000.0f))	// カギの大きさの初期化値(最大)

// モデルの読み込み
const char* c_apFilenameKey[NUM_KEY] =
{
	"data\\MODEL\\key\\key_000.x",	// カギ[KEYTYPE_RED]
	"data\\MODEL\\key\\key_001.x",	// カギ[KEYTYPE_GREEN]
	"data\\MODEL\\key\\key_002.x",	// カギ[KEYTYPE_CYAN]
};

// グローバル変数
KeyModel g_aKeyModel[NUM_KEY];		// モデルの種類を管理
KeyItem g_aKey[MAX_KEYITEM];	// カギの情報を格納

//======================================================================================
// カギの初期化処理
//======================================================================================
void InitKey(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();	// デバイスの取得
	D3DXMATERIAL* pMat;
	int nNumVtx = RESET_DATA;	// 頂点数
	DWORD dwSizeFVF;			// 頂点フォーマットのサイズ
	BYTE* pVtxBuff;				// 頂点バッファへのポインタ
	D3DXVECTOR3 vtxMin = VTX_MIN;	// カギの大きさをローカルで保存
	D3DXVECTOR3 vtxMax = VTX_MAX;	// カギの大きさをローカルで保存

	// モデル情報の初期化
	for (int nCntModel = RESET_DATA; nCntModel < NUM_KEY; nCntModel++)
	{
		g_aKeyModel[nCntModel].pMesh = NULL;
		g_aKeyModel[nCntModel].pBuffMat = NULL;
		g_aKeyModel[nCntModel].dwNumMat = RESET_DATA;
		g_aKeyModel[nCntModel].vtxMax = VTX_MAX;
		g_aKeyModel[nCntModel].vtxMin = VTX_MIN;
	}

	// Key情報の初期化
	for (int nCntKey = RESET_DATA; nCntKey < MAX_KEYITEM; nCntKey++)
	{
		g_aKey[nCntKey].pos = DEFALT;
		g_aKey[nCntKey].posOld = DEFALT;
		g_aKey[nCntKey].rot = DEFALT;
		g_aKey[nCntKey].move = DEFALT;
		g_aKey[nCntKey].type = KEYTYPE_RED;
		g_aKey[nCntKey].nIdxShadow = -1;
		g_aKey[nCntKey].bUse = false;
		g_aKey[nCntKey].bUseOld = false;
	}

	// Xファイルの読み込み
	for (int nCntModel = RESET_DATA; nCntModel < NUM_KEY; nCntModel++)
	{
		D3DXLoadMeshFromX(c_apFilenameKey[nCntModel], D3DXMESH_SYSTEMMEM, pDevice, NULL, &g_aKeyModel[nCntModel].pBuffMat, NULL, &g_aKeyModel[nCntModel].dwNumMat, &g_aKeyModel[nCntModel].pMesh);

		// 頂点数を取得
		nNumVtx = g_aKeyModel[nCntModel].pMesh->GetNumVertices();

		// 頂点フォーマットのサイズを取得
		dwSizeFVF = D3DXGetFVFVertexSize(g_aKeyModel[nCntModel].pMesh->GetFVF());

		// 頂点バッファをロック
		g_aKeyModel[nCntModel].pMesh->LockVertexBuffer(D3DLOCK_READONLY, (void**)&pVtxBuff);

		// カギの大きさを取得
		for (int nCntVtx = RESET_DATA; nCntVtx < nNumVtx; nCntVtx++, pVtxBuff += dwSizeFVF)
		{
			D3DXVECTOR3 vtx = *(D3DXVECTOR3*)pVtxBuff;	// 頂点座標の代入
			// X座標の比較
			if (vtxMin.x >= vtx.x)			// 保存されている最小のX座標より小さい場合
			{
				vtxMin.x = vtx.x;
			}
			else if (vtxMax.x <= vtx.x)	// 保存されている最大のX座標より大きい場合
			{
				vtxMax.x = vtx.x;
			}

			// Y座標の比較
			if (vtxMin.y >= vtx.y)			// 保存されている最小のY座標より小さい場合
			{
				vtxMin.y = vtx.y;
			}
			else if (vtxMax.y <= vtx.y)	// 保存されている最大のY座標より大きい場合
			{
				vtxMax.y = vtx.y;
			}

			// Z座標の比較
			if (vtxMin.z >= vtx.z)			// 保存されている最小のZ座標より小さい場合
			{
				vtxMin.z = vtx.z;
			}
			else if (vtxMax.z <= vtx.z)	// 保存されている最大のZ座標より大きい場合
			{
				vtxMax.z = vtx.z;
			}
		}

		// 頂点バッファをアンロック
		g_aKeyModel[nCntModel].pMesh->UnlockVertexBuffer();

		// カギの当たり判定を各配列に代入
		g_aKeyModel[nCntModel].vtxMin = vtxMin;
		g_aKeyModel[nCntModel].vtxMax = vtxMax;

		// マテリアルデータへのポインタを取得
		pMat = (D3DXMATERIAL*)g_aKeyModel[nCntModel].pBuffMat->GetBufferPointer();

		// テクスチャの読み込み
		for (int nCntMat = RESET_DATA; nCntMat < (int)g_aKeyModel[nCntModel].dwNumMat; nCntMat++)
		{
			if (pMat[nCntMat].pTextureFilename != NULL)	// テクスチャファイルが存在する場合
			{
				D3DXCreateTextureFromFile(pDevice, pMat[nCntMat].pTextureFilename, &g_aKeyModel[nCntModel].apTexture[nCntMat]);
			}
		}
	}
}

//======================================================================================
// カギの終了処理
//======================================================================================
void UninitKey(void)
{
	// メッシュの破棄
	for (int nCntModel = RESET_DATA; nCntModel < NUM_KEY; nCntModel++)
	{
		if (g_aKeyModel[nCntModel].pMesh != NULL)
		{
			g_aKeyModel[nCntModel].pMesh->Release();
			g_aKeyModel[nCntModel].pMesh = NULL;
		}
	}

	// テクスチャの破棄
	for (int nCntModel = RESET_DATA; nCntModel < NUM_KEY; nCntModel++)
	{
		for (int nCntMat = RESET_DATA; nCntMat < (int)g_aKeyModel[nCntModel].dwNumMat; nCntMat++)
		{
			if (g_aKeyModel[nCntModel].apTexture[nCntMat] != NULL)
			{
				g_aKeyModel[nCntModel].apTexture[nCntMat]->Release();
				g_aKeyModel[nCntModel].apTexture[nCntMat] = NULL;
			}
		}
	}

	// マテリアルの破棄
	for (int nCntModel = RESET_DATA; nCntModel < NUM_KEY; nCntModel++)
	{
		if (g_aKeyModel[nCntModel].pBuffMat != NULL)
		{
			g_aKeyModel[nCntModel].pBuffMat->Release();
			g_aKeyModel[nCntModel].pBuffMat = NULL;
		}
	}
}

//======================================================================================
// カギの更新処理
//======================================================================================
void UpdateKey(void)
{
	MeshField* pMeshField = GetMeshField();
	Player* pPlayer = GetPlayer();

	for (int nCntKey = RESET_DATA; nCntKey < MAX_KEYITEM; nCntKey++)
	{
		if (g_aKey[nCntKey].bUse == true)
		{
			// 重力
			g_aKey[nCntKey].move.y -= GRAVITY;

			// 位置の更新
			g_aKey[nCntKey].pos += g_aKey[nCntKey].move;

			// 重力を加算
			g_aKey[nCntKey].pos.y += (0.0f - g_aKey[nCntKey].move.y) * 0.1f;

			// 最低高度に到達したとき
			if (g_aKey[nCntKey].pos.y < pMeshField->pos.y)
			{
				g_aKey[nCntKey].pos.y = pMeshField->pos.y;
				g_aKey[nCntKey].move.y = RESET_DATA;
			}

			// ブロックとの当たり判定
			if (CollisionBlock(&g_aKey[nCntKey].pos, &g_aKey[nCntKey].posOld, &g_aKey[nCntKey].move, g_aKeyModel[g_aKey[nCntKey].type].vtxMin, g_aKeyModel[g_aKey[nCntKey].type].vtxMax) == true)
			{
			}

			// オブジェクトとの距離を指定
			float fHeight = DistanceShadow(&g_aKey[nCntKey].pos, g_aKeyModel[g_aKey[nCntKey].type].vtxMin, g_aKeyModel[g_aKey[nCntKey].type].vtxMax);
			
			// 影の色を指定
			SetShadowColorData(g_aKey[nCntKey].nIdxShadow, g_aKey[nCntKey].pos.y, fHeight);
			
			// 影の位置を更新
			SetPositionShadow(g_aKey[nCntKey].nIdxShadow, D3DXVECTOR3(g_aKey[nCntKey].pos.x, fHeight, g_aKey[nCntKey].pos.z));
		
			// カギを自動回転
			g_aKey[nCntKey].rot.y += 0.05f;

			if (g_aKey[nCntKey].bUse == true && g_aKey[nCntKey].bUseOld == false && pPlayer->nKey == 2 && pPlayer->nCounterMotion == RESET_DATA)
			{
				g_aKey[nCntKey].bUse = false;
				ResetShadow(g_aKey[nCntKey].nIdxShadow);
				// カギの取得フラグを立てる
				switch (g_aKey[nCntKey].type)
				{
				case KEYTYPE_RED:
					pPlayer->bRedKey = true;	// カギを取得済みにする
					SetParticle(PARTICLETYPE_GRABITY, g_aKey[nCntKey].pos, COLOR_RED, PARTICLE);
					SetParticle(PARTICLETYPE_GRABITY, g_aKey[nCntKey].pos, COLOR_RED, PARTICLE);
					SetParticle(PARTICLETYPE_GRABITY, g_aKey[nCntKey].pos, COLOR_GREEN, PARTICLE);
					SetParticle(PARTICLETYPE_GRABITY, g_aKey[nCntKey].pos, COLOR_CYAN, PARTICLE);
					break;

				case KEYTYPE_GREEN:
					pPlayer->bGreenKey = true;	// カギを取得済みにする
					SetParticle(PARTICLETYPE_GRABITY, g_aKey[nCntKey].pos, COLOR_RED, PARTICLE);
					SetParticle(PARTICLETYPE_GRABITY, g_aKey[nCntKey].pos, COLOR_GREEN, PARTICLE);
					SetParticle(PARTICLETYPE_GRABITY, g_aKey[nCntKey].pos, COLOR_GREEN, PARTICLE);
					SetParticle(PARTICLETYPE_GRABITY, g_aKey[nCntKey].pos, COLOR_CYAN, PARTICLE);

					break;

				case KEYTYPE_CYAN:
					pPlayer->bCyanKey = true;	// カギを取得済みにする
					SetParticle(PARTICLETYPE_GRABITY, g_aKey[nCntKey].pos, COLOR_RED, PARTICLE);
					SetParticle(PARTICLETYPE_GRABITY, g_aKey[nCntKey].pos, COLOR_GREEN, PARTICLE);
					SetParticle(PARTICLETYPE_GRABITY, g_aKey[nCntKey].pos, COLOR_CYAN, PARTICLE);
					SetParticle(PARTICLETYPE_GRABITY, g_aKey[nCntKey].pos, COLOR_CYAN, PARTICLE);
					break;
				}
			}
		}
	}
}

//======================================================================================
// カギの描画処理
//======================================================================================
void DrawKey(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();	// デバイスの取得
	D3DXMATRIX mtxRot, mtxTrans;	// 計算用マトリックス
	D3DMATERIAL9 matDef;	// 現在のマテリアルを保存
	D3DXMATERIAL* pMat;		// マテリアルデータへのポインタ

	for (int nCntKey = RESET_DATA; nCntKey < MAX_KEYITEM; nCntKey++)
	{
		if (g_aKey[nCntKey].bUse == true)
		{
			// ワールドマトリックスの初期化(デフォルトの値にする)
			D3DXMatrixIdentity(&g_aKey[nCntKey].mtxWorld);

			// 向きを反映
			D3DXMatrixRotationYawPitchRoll(&mtxRot, g_aKey[nCntKey].rot.y, g_aKey[nCntKey].rot.x, g_aKey[nCntKey].rot.z);
			D3DXMatrixMultiply(&g_aKey[nCntKey].mtxWorld, &g_aKey[nCntKey].mtxWorld, &mtxRot);

			// 位置を反映
			D3DXMatrixTranslation(&mtxTrans, g_aKey[nCntKey].pos.x, g_aKey[nCntKey].pos.y, g_aKey[nCntKey].pos.z);
			D3DXMatrixMultiply(&g_aKey[nCntKey].mtxWorld, &g_aKey[nCntKey].mtxWorld, &mtxTrans);

			// ワールドマトリックスの設定
			pDevice->SetTransform(D3DTS_WORLD, &g_aKey[nCntKey].mtxWorld);

			// 現在のマテリアルを取得
			pDevice->GetMaterial(&matDef);

			// マテリアルデータへのポインタを取得
			pMat = (D3DXMATERIAL*)g_aKeyModel[g_aKey[nCntKey].type].pBuffMat->GetBufferPointer();

			// マテリアルごとにカギを描画
			for (int nCntMat = RESET_DATA; nCntMat < (int)g_aKeyModel[g_aKey[nCntKey].type].dwNumMat; nCntMat++)
			{
				// マテリアルの設定
				pDevice->SetMaterial(&pMat[nCntMat].MatD3D);

				// テクスチャの設定
				pDevice->SetTexture(0, g_aKeyModel[g_aKey[nCntKey].type].apTexture[nCntMat]);

				// カギパーツの描画
				g_aKeyModel[g_aKey[nCntKey].type].pMesh->DrawSubset(nCntMat);	// ここでモデルの形を指定しているため、g_aKeyModelの中身を設定する必要がある
			}

			// 保存していたマテリアルに戻す
			pDevice->SetMaterial(&matDef);
		}
	}
}

//======================================================================================
// カギを配置
//======================================================================================
void SetKey(D3DXVECTOR3 pos, D3DXVECTOR3 rot, KEYTYPE type)
{
	for (int nCntKey = RESET_DATA; nCntKey < MAX_KEYITEM; nCntKey++)
	{
		if (g_aKey[nCntKey].bUse == false)
		{
			g_aKey[nCntKey].pos = pos;
			g_aKey[nCntKey].posOld = pos;
			g_aKey[nCntKey].rot = rot;
			g_aKey[nCntKey].rot.y = (float)((rand() % ANGLE_DOUBLE - MAX_ANGLE) / ANGLE_ADJUST);	// 回転の開始位置をランダムにする
			g_aKey[nCntKey].move = DEFALT;
			g_aKey[nCntKey].type = type;
			g_aKey[nCntKey].bUse = true;
			g_aKey[nCntKey].bUseOld = true;
			// 影のIDを設定
			g_aKey[nCntKey].nIdxShadow = SetShadow(SHADOWTYPE_CIRCLE, SHADOW, SHADOW);
		
			break;
		}
	}
}

//======================================================================================
// カギの情報を渡す
//======================================================================================
KeyItem* GetKeyItem(void)
{
	return &g_aKey[RESET_DATA];
}

//======================================================================================
// カギとの当たり判定
//======================================================================================
void CollisionKey(D3DXVECTOR3* pPos, D3DXVECTOR3* pPosOld, D3DXVECTOR3* pMove, D3DXVECTOR3 vtxMin, D3DXVECTOR3 vtxMax)
{
	Player* pPlayer = GetPlayer();

	for (int nCntKey = RESET_DATA; nCntKey < MAX_KEYITEM; nCntKey++)
	{
		if (pPlayer->motionType == MOTIONTYPE_ACTION || pPlayer->motionType == MOTIONTYPE_JUMP || pPlayer->motionType == MOTIONTYPE_LANDING ||
			pPlayer->motionTypeBlend == MOTIONTYPE_ACTION || pPlayer->motionTypeBlend == MOTIONTYPE_JUMP || pPlayer->motionTypeBlend == MOTIONTYPE_LANDING)
		{
			// 特定のモーション中はカギとの当たり判定を切る
		}
		else if (g_aKey[nCntKey].bUse == true && g_aKey[nCntKey].bUseOld == true)
		{
			// カギの高度の範囲に収まっているとき、XZ方向の当たり判定を行う
			if (pPos->y + vtxMax.y >= g_aKey[nCntKey].pos.y + g_aKeyModel[g_aKey[nCntKey].type].vtxMin.y &&	// カギの下端より大きい
				pPos->y + vtxMin.y <= g_aKey[nCntKey].pos.y + g_aKeyModel[g_aKey[nCntKey].type].vtxMax.y &&	// カギの上端より小さい
				pPos->x + vtxMax.x >= g_aKey[nCntKey].pos.x + g_aKeyModel[g_aKey[nCntKey].type].vtxMin.x &&	// ブロックの左端より右にいる
				pPos->x + vtxMin.x <= g_aKey[nCntKey].pos.x + g_aKeyModel[g_aKey[nCntKey].type].vtxMax.x &&	// ブロックの右端より左にいる
				pPos->z + vtxMax.z >= g_aKey[nCntKey].pos.z + g_aKeyModel[g_aKey[nCntKey].type].vtxMin.z &&	// ブロックの手前より奥にいる
				pPos->z + vtxMin.z <= g_aKey[nCntKey].pos.z + g_aKeyModel[g_aKey[nCntKey].type].vtxMax.z)	// ブロックの奥より手前にいる
			{
				PlaySound(SE_KEY);
				SetMotion(MOTIONTYPE_ACTION, true, 1);
				SetVibration(10000, 10000, 90);
				pPlayer->bMoveOld = false;
				g_aKey[nCntKey].bUseOld = false;
			}
		}
	}
}

#endif