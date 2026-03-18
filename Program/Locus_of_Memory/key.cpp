//=============================================================================
//
//	鍵取得処理 [key.cpp]
//	Author : Kajino Hiyori
// 
//=============================================================================
#include "main.h"
#include "key.h"
#include "player.h"
#include "meshfield.h"
#include "particle.h"
#include "shadow.h"
#include "color.h"
#include "input.h"
#include "debugproc.h"

// マクロ定義
#define NUM_KEY			(KEYTYPE_MAX)	// モデルの数
#define SHADOW			(10.0f)			// 影の大きさ
#define BLANK			(0.001f)		// 空白
#define PARTICLE		(30)			// パーティクルの寿命
#define ANGLE_DOUBLE	(629)			// 角度の最大数(*2)
#define MAX_ANGLE		(314)			// 角度の最大数
#define ANGLE_ADJUST	(100.0f)		// 角度の値を指定
#define COLLISION		(50.0f)			// 鍵との当たり判定の大きさ
#define SCALE			(1.0f)			// 大きさの比率を管理
#define INIT_D3DXVEC3	(D3DXVECTOR3(0.0f, 0.0f, 0.0f))		// xyzが0.0fの場合
#define NORMAL			(D3DXVECTOR3(0.0f, 1.0f, 0.0f))		// 基本の法線
#define KEY001_POS		(D3DXVECTOR3(-50.0f, 0.0f, 10.0f))	// 鍵の位置
#define KEY002_POS		(D3DXVECTOR3(0.0f, 0.0f, 10.0f))	// 鍵の位置
#define KEY003_POS		(D3DXVECTOR3(50.0f, 0.0f, 10.0f))	// 鍵の位置

// モデルの読み込み
const char* c_apFilenameKey[NUM_KEY] =
{
	"data\\MODEL\\key\\key000.x",
	"data\\MODEL\\key\\key001.x",
	"data\\MODEL\\key\\key002.x",
};

// グローバル変数
KeyModel g_aKeyModel[NUM_KEY];		// モデルの種類を管理
KeyItem g_aKey[NUM_KEY];	// カギの情報を格納

//======================================================================================
// カギの初期化処理
//======================================================================================
void InitKey(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();	// デバイスの取得
	D3DXMATERIAL* pMat;
	int nNumVtx = 0;			// 頂点数
	DWORD dwSizeFVF;			// 頂点フォーマットのサイズ
	BYTE* pVtxBuff;				// 頂点バッファへのポインタ

	// モデル情報の初期化
	for (int nCntModel = 0; nCntModel < NUM_KEY; nCntModel++)
	{
		g_aKeyModel[nCntModel].pMesh = NULL;
		g_aKeyModel[nCntModel].pBuffMat = NULL;
		g_aKeyModel[nCntModel].dwNumMat = 0;
	}

	// Key情報の初期化
	for (int nCntKey = 0; nCntKey < MAX_KEYITEM; nCntKey++)
	{
		g_aKey[nCntKey].pos			= INIT_D3DXVEC3;	// カギの位置
		g_aKey[nCntKey].posOld		= INIT_D3DXVEC3;;	// カギの位置
		g_aKey[nCntKey].rot			= INIT_D3DXVEC3;	// カギの向き
		g_aKey[nCntKey].move		= INIT_D3DXVEC3;;	// カギの移動量
		g_aKey[nCntKey].type		= KEYTYPE_ONE;		// モデルの種類
		g_aKey[nCntKey].state		= KEYSTATE_NORMAL;	// 鍵の状態
		g_aKey[nCntKey].fCollision	= COLLISION;		// 当たり判定の半径
		g_aKey[nCntKey].fScale		= SCALE;			// 大きさ管理
		g_aKey[nCntKey].nIdxShadow	= -1;				// 影のインデックス
		g_aKey[nCntKey].bUse		= false;			// 使用状態
	}

	// Xファイルの読み込み
	for (int nCntModel = 0; nCntModel < NUM_KEY; nCntModel++)
	{
		D3DXLoadMeshFromX(c_apFilenameKey[nCntModel], D3DXMESH_SYSTEMMEM, pDevice, NULL, &g_aKeyModel[nCntModel].pBuffMat, NULL, &g_aKeyModel[nCntModel].dwNumMat, &g_aKeyModel[nCntModel].pMesh);

		// 頂点数を取得
		nNumVtx = g_aKeyModel[nCntModel].pMesh->GetNumVertices();

		// 頂点フォーマットのサイズを取得
		dwSizeFVF = D3DXGetFVFVertexSize(g_aKeyModel[nCntModel].pMesh->GetFVF());

		// 頂点バッファをロック
		g_aKeyModel[nCntModel].pMesh->LockVertexBuffer(D3DLOCK_READONLY, (void**)&pVtxBuff);

		// 頂点バッファをアンロック
		g_aKeyModel[nCntModel].pMesh->UnlockVertexBuffer();

		// マテリアルデータへのポインタを取得
		pMat = (D3DXMATERIAL*)g_aKeyModel[nCntModel].pBuffMat->GetBufferPointer();

		// テクスチャの読み込み
		for (int nCntMat = 0; nCntMat < (int)g_aKeyModel[nCntModel].dwNumMat; nCntMat++)
		{
			if (pMat[nCntMat].pTextureFilename != NULL)	// テクスチャファイルが存在する場合
			{
				D3DXCreateTextureFromFile(pDevice, pMat[nCntMat].pTextureFilename, &g_aKeyModel[nCntModel].apTexture[nCntMat]);
			}
		}
	}

	// 鍵の設置
	//SetKey(KEY001_POS, INIT_D3DXVEC3, KEYTYPE_ONE);
}

//======================================================================================
// カギの終了処理
//======================================================================================
void UninitKey(void)
{
	// メッシュの破棄
	for (int nCntModel = 0; nCntModel < NUM_KEY; nCntModel++)
	{
		if (g_aKeyModel[nCntModel].pMesh != NULL)
		{
			g_aKeyModel[nCntModel].pMesh->Release();
			g_aKeyModel[nCntModel].pMesh = NULL;
		}
	}

	// テクスチャの破棄
	for (int nCntModel = 0; nCntModel < NUM_KEY; nCntModel++)
	{
		for (int nCntMat = 0; nCntMat < (int)g_aKeyModel[nCntModel].dwNumMat; nCntMat++)
		{
			if (g_aKeyModel[nCntModel].apTexture[nCntMat] != NULL)
			{
				g_aKeyModel[nCntModel].apTexture[nCntMat]->Release();
				g_aKeyModel[nCntModel].apTexture[nCntMat] = NULL;
			}
		}
	}

	// マテリアルの破棄
	for (int nCntModel = 0; nCntModel < NUM_KEY; nCntModel++)
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
	Player* pPlayer = GetPlayer();

	for (int nCntPlayer = 0; nCntPlayer < MAX_PLAYER; nCntPlayer++, pPlayer++)
	{
		if (pPlayer->bUse == false)
		{
			continue;
		}

		// 鍵との当たり判定
		CollisionKey(nCntPlayer, &pPlayer[nCntPlayer].pos, &pPlayer[nCntPlayer].posOld, &pPlayer[nCntPlayer].move);

		for (int nCntKey = 0; nCntKey < MAX_KEYITEM; nCntKey++)
		{
			if (g_aKey[nCntKey].bUse == true)
			{
				// 位置の更新
				g_aKey[nCntKey].pos += g_aKey[nCntKey].move;

				// 鍵の位置
				//PrintDebugProc("鍵の位置[%d] : (%f, %f, %f)\n", nCntKey, g_aKey[nCntKey].pos.x, g_aKey[nCntKey].pos.y, g_aKey[nCntKey].pos.z);


				// 最低高度に到達したとき
				if (g_aKey[nCntKey].pos.y < 0.0f)
				{
					g_aKey[nCntKey].pos.y = 0.0f;
					g_aKey[nCntKey].move.y = 0.0f;
				}

				// 影の位置を更新
				SetPositionShadow(g_aKey[nCntKey].nIdxShadow, D3DXVECTOR3(g_aKey[nCntKey].pos.x, 0.0f, g_aKey[nCntKey].pos.z));

				// カギを自動回転
				g_aKey[nCntKey].rot.y += 0.05f;
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

	for (int nCntKey = 0; nCntKey < MAX_KEYITEM; nCntKey++)
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
			for (int nCntMat = 0; nCntMat < (int)g_aKeyModel[g_aKey[nCntKey].type].dwNumMat; nCntMat++)
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
	for (int nCntKey = 0; nCntKey < MAX_KEYITEM; nCntKey++)
	{
		if (g_aKey[nCntKey].bUse == false)
		{
			g_aKey[nCntKey].pos = pos;
			g_aKey[nCntKey].posOld = pos;
			g_aKey[nCntKey].rot = rot;
			g_aKey[nCntKey].rot.y = (float)((rand() % ANGLE_DOUBLE - MAX_ANGLE) / ANGLE_ADJUST);	// 回転の開始位置をランダムにする
			g_aKey[nCntKey].move = INIT_D3DXVEC3;
			g_aKey[nCntKey].type = type;
			g_aKey[nCntKey].bUse = true;
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
	return &g_aKey[0];
}

//======================================================================================
// カギとの当たり判定
//======================================================================================
void CollisionKey(int nIdx, D3DXVECTOR3* pPos, D3DXVECTOR3* pPosOld, D3DXVECTOR3* pMove)
{
	for (int nCntKey = 0; nCntKey < MAX_KEYITEM; nCntKey++)
	{
		if (g_aKey[nCntKey].bUse == false)
		{
			continue;
		}

		// カギの高度の範囲に収まっているとき、XZ方向の当たり判定を行う
		if (pPos->x >= g_aKey[nCntKey].pos.x - g_aKey[nCntKey].fCollision &&	// ブロックの左端より右にいる
			pPos->x <= g_aKey[nCntKey].pos.x + g_aKey[nCntKey].fCollision &&	// ブロックの右端より左にいる
			pPos->z >= g_aKey[nCntKey].pos.z - g_aKey[nCntKey].fCollision &&	// ブロックの手前より奥にいる
			pPos->z <= g_aKey[nCntKey].pos.z + g_aKey[nCntKey].fCollision)		// ブロックの奥より手前にいる
		{
			//PlaySound(SE_KEY);
			//SetMotion(MOTIONTYPE_ACTION, true, 1);
			//if ((GetKeyboardTrigger(DIK_RETURN) == true && nIdx == 0) || GetJoypadTrigger(JOYKEY_X, nIdx) == true)
			//{
			//	g_aKey[nCntKey].bUse = false;
			//}
			//PrintDebugProc("カギに当たっている\n");
		}
		else
		{
			//PrintDebugProc("カギに当たってない\n");
		}
	}
}