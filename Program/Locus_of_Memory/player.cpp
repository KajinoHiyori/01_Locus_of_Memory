//========================================================================
// 
// player処理[player.cpp]
// Author : Kajino Hiyori
//
//========================================================================
#include "main.h"
#include "debugproc.h"
#include "player.h"
#include "shadow.h"
#include "camera.h"
#include "title.h"
#include "meshfield.h"
#include "input.h"

// マクロ定義
#define MAX_MODEL		(1)					// モデルの最大数
#define MOVE			(0.25f)				// 移動量
#define ROTATE			(0.9f)				// 回転量
#define GRAVITY			(0.75f)				// 重力
#define JUMP			(17.0f)				// ジャンプ
#define SHADOｗ			(10.0f)				// 影の大きさ
#define CORRECTION_ROT	(0.075f)			// 回転の減衰係数
#define PI				(D3DX_PI)			// 円周率
#define RIGHT			(D3DX_PI / 2)		// 右を向く
#define LEFT			(-(D3DX_PI / 2))	// 左を向く
#define BACK			(D3DX_PI)			// 後ろを向く
#define FRONT			(0.0f)				// 正面を向く
#define LENGTH			(50.0f)				// フィールドの範囲
#define POS				(D3DXVECTOR3(0.0f, 50.0f, 0.0f))	// プレイヤーの位置
#define DEFALT			(D3DXVECTOR3(0.0f, 0.0f, 0.0f))		// xyzが0.0fの場合
#define NORMAL			(D3DXVECTOR3(0.0f, 1.0f, 0.0f))		// 基本の法線

// グローバル変数
LPD3DXMESH			g_pMeshPlayer[MAX_PLAYER] = {};				// メッシュ(頂点情報)へのポインタ
LPD3DXBUFFER		g_pBuffMatPlayer[MAX_PLAYER] = {};			// マテリアルへのポインタ
DWORD				g_dwNumMatPlayer[MAX_PLAYER] = {0, 0};		// マテリアルの数
LPDIRECT3DTEXTURE9	g_apTexturePlayer[MAX_PLAYER][MAX_PLAYERMAT];		// テクスチャへの	ポインタ
Player				g_aPlayer[MAX_PLAYER];						// プレイヤーの情報を格納

// 1Pのモデル
const char* c_apFilename1PModel[MAX_MODEL] =
{
	"data\\MODEL\\player\\player.x",	// 胴体[0]
	//"data\\MODEL\\player\\airplane000.x",	// 胴体[0]
};

// 2Pのモデル
const char* c_apFilename2PModel[MAX_MODEL] =
{
	"data\\MODEL\\player\\tank000.x",	// 胴体[0]
};

//========================================================================
// プレイヤーの初期化処理
//========================================================================
void InitPlayer(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();	// デバイスの取得
	D3DXMATERIAL* pMat;

	// 初期化
	for (int nCntPlayer = 0; nCntPlayer < MAX_PLAYER; nCntPlayer++)
	{
		g_aPlayer[nCntPlayer].pos = DEFALT;
		g_aPlayer[nCntPlayer].posOld = DEFALT;
		g_aPlayer[nCntPlayer].rot = DEFALT;
		g_aPlayer[nCntPlayer].rotDest = DEFALT;
		g_aPlayer[nCntPlayer].move = DEFALT;
		g_aPlayer[nCntPlayer].nIdxShadow = -1;
		g_aPlayer[nCntPlayer].fSpeed = MOVE;
		g_aPlayer[nCntPlayer].bUse = false;
		g_aPlayer[nCntPlayer].bJump = false;

#if 0

		for (int nCntModel = 0; nCntModel < MAX_MODEL; nCntModel++)
		{
			switch (nCntPlayer)
			{
			case 0:	// 1P
				// Xファイルの読み込み
				D3DXLoadMeshFromX(c_apFilename1PModel[nCntModel], D3DXMESH_SYSTEMMEM, pDevice, NULL, &g_pBuffMatPlayer[nCntPlayer], NULL, &g_dwNumMatPlayer[nCntPlayer], &g_pMeshPlayer[nCntPlayer]);
				break;

			case 1:	// 2P
				D3DXLoadMeshFromX(c_apFilename2PModel[nCntModel], D3DXMESH_SYSTEMMEM, pDevice, NULL, &g_pBuffMatPlayer[nCntPlayer], NULL, &g_dwNumMatPlayer[nCntPlayer], &g_pMeshPlayer[nCntPlayer]);
				break;
			}

			// マテリアルデータへのポインタを取得
			pMat = (D3DXMATERIAL*)g_pBuffMatPlayer[nCntPlayer]->GetBufferPointer();

			for (int nCntMat = 0; nCntMat < (int)g_dwNumMatPlayer[nCntPlayer]; nCntMat++)
			{
				if (pMat[nCntMat].pTextureFilename != NULL)	// テクスチャファイルが存在する場合
				{
					// テクスチャの読み込み
					D3DXCreateTextureFromFile(pDevice, pMat[nCntMat].pTextureFilename, &g_apTexturePlayer[nCntPlayer][nCntMat]);
				}
			}
		}

#endif

	}

	OPERATIONTYPE operationtyoe = GetOperationType();
	SetPlayer(0, POS, DEFALT);
	if (operationtyoe == OPERATIONTYPE_2P)
	{
		SetPlayer(1, DEFALT, DEFALT);
	}
}

//========================================================================
// プレイヤーの終了処理
//========================================================================
void UninitPlayer(void)
{
	// メッシュの破棄
	for (int nCntPlayer = 0; nCntPlayer < MAX_PLAYER; nCntPlayer++)
	{
		if (g_pMeshPlayer[nCntPlayer]!= NULL)
		{
			g_pMeshPlayer[nCntPlayer]->Release();
			g_pMeshPlayer[nCntPlayer] = NULL;
		}
	}
	// テクスチャの破棄
	for (int nCntPlayer = 0; nCntPlayer < MAX_PLAYER; nCntPlayer++)
	{
		for (int nCntMat = 0; nCntMat < (int)g_dwNumMatPlayer[nCntPlayer]; nCntMat++)
		{
			if (g_apTexturePlayer[nCntPlayer][nCntMat] != NULL)
			{
				g_apTexturePlayer[nCntPlayer][nCntMat]->Release();
				g_apTexturePlayer[nCntPlayer][nCntMat] = NULL;
			}
		}
	}

	// マテリアルの破棄
	for (int nCntPlayer = 0; nCntPlayer < MAX_PLAYER; nCntPlayer++)
	{
		if (g_pBuffMatPlayer[nCntPlayer] != NULL)
		{
			g_pBuffMatPlayer[nCntPlayer]->Release();
			g_pBuffMatPlayer[nCntPlayer] = NULL;
		}
	}
}

//========================================================================
// プレイヤーの更新処理
//========================================================================
void UpdatePlayer(void)
{
	Camera* pCamera = GetCamera();
	D3DXVECTOR3 moveDir = D3DXVECTOR3(0.0f, 0.0f, 0.0f);	// 移動方向の向きを決定
	float fMoveDir = 0.0f;	// 移動方向の代入
	float fRotMove = 0.0f;	// 移動する向きを決定
	float fRotDest = 0.0f;	// 目的の向きを設定
	float fRotDiffKey = 0.0f;	// 角度の差分
	float fAngle = 0.0f;		// 角度

	// 過去の位置を保存
	for (int nCntPlayer = 0; nCntPlayer < MAX_PLAYER; nCntPlayer++)
	{
		if (g_aPlayer[nCntPlayer].bUse == true)
		{
			g_aPlayer[nCntPlayer].posOld = g_aPlayer[nCntPlayer].pos;
		}
	}

#ifdef _DEBUG
	// キーボード操作[1P] / WASD
	if (g_aPlayer[0].bUse == true)
	{
	
	}

	// キーボード操作[2P] / IJKL
	if (g_aPlayer[1].bUse == true)
	{
		// プレイヤーの移動を管理
		if (GetKeyboardPress(DIK_J) == true)	// 右に移動
		{
			if (GetKeyboardPress(DIK_I) == true)	// 奥に移動
			{
				g_aPlayer[1].move.x -= sinf(pCamera[1].rot.y - D3DX_PI / 4) * MOVE;
				g_aPlayer[1].move.z -= cosf(pCamera[1].rot.y - D3DX_PI / 4) * MOVE;
			}
			else if (GetKeyboardPress(DIK_K) == true)	// 手前に移動
			{
				g_aPlayer[1].move.x += sinf(pCamera[1].rot.y + D3DX_PI / 4) * MOVE;
				g_aPlayer[1].move.z += cosf(pCamera[1].rot.y + D3DX_PI / 4) * MOVE;
			}
			else
			{
				g_aPlayer[1].move.x += cosf(pCamera[1].rot.y) * MOVE;
				g_aPlayer[1].move.z -= sinf(pCamera[1].rot.y) * MOVE;
			}
		}
		else if (GetKeyboardPress(DIK_L) == true)	// 左に移動
		{
			if (GetKeyboardPress(DIK_I) == true)	// 奥に移動
			{
				g_aPlayer[1].move.x -= cosf(pCamera[1].rot.y - D3DX_PI / 4) * MOVE;
				g_aPlayer[1].move.z += sinf(pCamera[1].rot.y - D3DX_PI / 4) * MOVE;
			}
			else if (GetKeyboardPress(DIK_K) == true)	// 手前に移動
			{
				g_aPlayer[1].move.x -= cosf(pCamera[1].rot.y + D3DX_PI / 4) * MOVE;
				g_aPlayer[1].move.z += sinf(pCamera[1].rot.y + D3DX_PI / 4) * MOVE;
			}
			else
			{
				g_aPlayer[1].move.x -= cosf(pCamera[1].rot.y) * MOVE;
				g_aPlayer[1].move.z += sinf(pCamera[1].rot.y) * MOVE;
			}
		}
		else if (GetKeyboardPress(DIK_I) == true)	// 奥に移動
		{
			g_aPlayer[1].move.x -= sinf(pCamera[1].rot.y) * MOVE;
			g_aPlayer[1].move.z -= cosf(pCamera[1].rot.y) * MOVE;
		}
		else if (GetKeyboardPress(DIK_K) == true)	// 手前に移動
		{
			g_aPlayer[1].move.x += sinf(pCamera[1].rot.y) * MOVE;
			g_aPlayer[1].move.z += cosf(pCamera[1].rot.y) * MOVE;
		}

		// プレイヤーの向きを設定
		if (GetKeyboardPress(DIK_J) == true)	// 右に移動
		{
			if (GetKeyboardPress(DIK_I) == true)	// 奥に移動
			{
				g_aPlayer[1].rotDest.y = pCamera[1].rot.y - D3DX_PI / 2 + D3DX_PI / 4;
			}
			else if (GetKeyboardPress(DIK_K) == true)	// 手前に移動
			{
				g_aPlayer[1].rotDest.y = pCamera[1].rot.y - D3DX_PI / 2 - D3DX_PI / 4;
			}
			else
			{
				g_aPlayer[1].rotDest.y = pCamera[1].rot.y - D3DX_PI / 2;
			}
		}
		else if (GetKeyboardPress(DIK_L) == true)	// 左に移動
		{
			if (GetKeyboardPress(DIK_I) == true)	// 奥に移動
			{
				g_aPlayer[1].rotDest.y = pCamera[1].rot.y + D3DX_PI / 2 - D3DX_PI / 4;
			}
			else if (GetKeyboardPress(DIK_K) == true)	// 手前に移動
			{
				g_aPlayer[1].rotDest.y = pCamera[1].rot.y + D3DX_PI / 2 + D3DX_PI / 4;
			}
			else
			{
				g_aPlayer[1].rotDest.y = pCamera[1].rot.y + D3DX_PI / 2;
			}
		}
		else if (GetKeyboardPress(DIK_I) == true)	// 奥に移動
		{
			g_aPlayer[1].rotDest.y = pCamera[1].rot.y;
		}
		else if (GetKeyboardPress(DIK_K) == true)	// 手前に移動
		{
			g_aPlayer[1].rotDest.y = pCamera[1].rot.y + D3DX_PI;
		}

		// ジャンプ処理
		if (GetKeyboardTrigger(DIK_RETURN) == true && g_aPlayer[1].bJump == false)
		{
			g_aPlayer[1].move.y = JUMP;
			g_aPlayer[1].bJump = true;
		}
	}
#endif	
	
	// 操作
	for (int nCntPlayer = 0; nCntPlayer < MAX_PLAYER; nCntPlayer++)
	{
		if (g_aPlayer[nCntPlayer].bUse == true)
		{
			// 過去の位置を保存
			g_aPlayer[nCntPlayer].posOld = g_aPlayer[nCntPlayer].pos;
			moveDir = D3DXVECTOR3(0.0f, 0.0f, 0.0f);

			// 移動方向を管理
			if (GetKeyboardPress(DIK_A) == true || GetJoypadPress(JOYKEY_LEFT, nCntPlayer) == true)	// 左に移動
			{
				moveDir.x -= 1.0f;
			}
			else if (GetKeyboardPress(DIK_D) == true || GetJoypadPress(JOYKEY_RIGHT, nCntPlayer) == true)	// 右に移動
			{
				moveDir.x += 1.0f;
			}
			if (GetKeyboardPress(DIK_W) == true || GetJoypadPress(JOYKEY_UP, nCntPlayer) == true)	// 奥に移動
			{
				moveDir.z += 1.0f;
			}
			else if (GetKeyboardPress(DIK_S) == true || GetJoypadPress(JOYKEY_DOWN, nCntPlayer) == true)	// 手前に移動
			{
				moveDir.z -= 1.0f;
			}

			// 移動方向の正規化
			D3DXVec3Normalize(&moveDir, &moveDir);

			// スティックの入力方向を利用
			GetJoypadStickLeft(&moveDir.x, &moveDir.z, nCntPlayer);

			// 移動状態を求める(fMoveDir == 0は移動していない)
			fMoveDir = SQRTF(moveDir.x, moveDir.z);

			if (fMoveDir != 0)
			{// 移動している場合
				fRotMove = g_aPlayer[nCntPlayer].rot.y;							// 今の向き
				fRotDest = atan2f(moveDir.x, moveDir.z) + pCamera[nCntPlayer].rot.y;		// 目的地への向き

				// 目的向きに移動方向を合わせる
				moveDir.x = sinf(fRotDest) * fMoveDir;
				moveDir.z = cosf(fRotDest) * fMoveDir;

				// 移動量の更新
				g_aPlayer[nCntPlayer].move.x += (g_aPlayer[nCntPlayer].fSpeed * -moveDir.x);
				g_aPlayer[nCntPlayer].move.z += (g_aPlayer[nCntPlayer].fSpeed * -moveDir.z);
			}
			
			// ジャンプ処理
			if ((GetKeyboardTrigger(DIK_SPACE) == true || GetJoypadTrigger(JOYKEY_A, nCntPlayer) == true) && g_aPlayer[nCntPlayer].bJump == false)
			{
				g_aPlayer[nCntPlayer].move.y = JUMP;
				g_aPlayer[nCntPlayer].bJump = true;
			}

			// 移動中の場合、目的の向きを設定
			if (GetJoypadStickLeft(&moveDir.x, &moveDir.z, nCntPlayer) == true)
			{
				// プレイヤーの方向を設定
				fAngle = atan2f(moveDir.x, -moveDir.z);
				g_aPlayer[nCntPlayer].rotDest.y = atan2f(-(float)(sinf(pCamera[nCntPlayer].rot.y - fAngle)), -(float)cosf(pCamera[nCntPlayer].rot.y - fAngle));
			}
			else if (GetKeyboardPress(DIK_A) == true || GetJoypadPress(JOYKEY_LEFT, nCntPlayer) == true ||	// 左に移動
					 GetKeyboardPress(DIK_D) == true || GetJoypadPress(JOYKEY_RIGHT, nCntPlayer) == true ||	// 右に移動
					 GetKeyboardPress(DIK_W) == true || GetJoypadPress(JOYKEY_UP, nCntPlayer) == true ||	// 奥に移動
					 GetKeyboardPress(DIK_S) == true || GetJoypadPress(JOYKEY_DOWN, nCntPlayer) == true)	// 手前に移動
			{
				fAngle = atan2f(-moveDir.x, moveDir.z);
				g_aPlayer[nCntPlayer].rotDest.y = atan2f(-(float)(sinf(pCamera[nCntPlayer].rot.y - fAngle)), -(float)cosf(pCamera[nCntPlayer].rot.y - fAngle));
			}

			// プレイヤーの方向を補正
			fRotDiffKey = g_aPlayer[nCntPlayer].rotDest.y - g_aPlayer[nCntPlayer].rot.y;	// 差分を計算
			fRotDiffKey = AngleNormalize(fRotDiffKey);
			g_aPlayer[nCntPlayer].rot.y += (fRotDiffKey)*CORRECTION_ROT;
			g_aPlayer[nCntPlayer].rot.y = AngleNormalize(g_aPlayer[nCntPlayer].rot.y);

			// 重力
			g_aPlayer[nCntPlayer].move.y -= GRAVITY;

			// 位置の更新
			g_aPlayer[nCntPlayer].pos += g_aPlayer[nCntPlayer].move;

			// メッシュフィールドとの当たり判定
			if (CollisionMeshField(&g_aPlayer[nCntPlayer].pos, &g_aPlayer[nCntPlayer].posOld, &g_aPlayer[nCntPlayer].move) == true || 
				g_aPlayer[nCntPlayer].pos.y <= 0.0f)
			{
				g_aPlayer[nCntPlayer].pos.y = 0.0f;
				g_aPlayer[nCntPlayer].bJump = false;
			}

			// 影の位置を更新
			SetPositionShadow(g_aPlayer[nCntPlayer].nIdxShadow, g_aPlayer[nCntPlayer].pos);

			// 移動量の更新
			g_aPlayer[nCntPlayer].move.x += (0.0f - g_aPlayer[nCntPlayer].move.x) * 0.1f;
			g_aPlayer[nCntPlayer].move.y += (0.0f - g_aPlayer[nCntPlayer].move.y) * 0.1f;
			g_aPlayer[nCntPlayer].move.z += (0.0f - g_aPlayer[nCntPlayer].move.z) * 0.1f;

			// モーションの更新処理
			UpdateMotion(&g_aPlayer[nCntPlayer].motion, g_aPlayer[nCntPlayer].pModelData);

#ifdef _DEBUG
			// テスト
			if (GetKeyboardTrigger(DIK_1) == true)
			{
				SetMotion(&g_aPlayer[nCntPlayer].motion, g_aPlayer[nCntPlayer].pModelData, (MOTIONTYPE)PLAYERMOTIONTYPE_ACTION, false, true, 10);
			}
			
			if (GetKeyboardTrigger(DIK_2) == true)
			{
				SetMotion(&g_aPlayer[nCntPlayer].motion, g_aPlayer[nCntPlayer].pModelData, (MOTIONTYPE)PLAYERMOTIONTYPE_NEUTRAL, true, false, 10);
			}
#endif	// _DEBUG

			// デバッグ表示
			PrintDebugProc("プレイヤー[%d]の位置 : (%.3f, %.3f, %.3f)\n", nCntPlayer, g_aPlayer[nCntPlayer].pos.x, g_aPlayer[nCntPlayer].pos.y, g_aPlayer[nCntPlayer].pos.z);
			PrintDebugProc("プレイヤー[%d]の移動量 : (%.3f, %.3f, %.3f)\n", nCntPlayer, g_aPlayer[nCntPlayer].move.x, g_aPlayer[nCntPlayer].move.y, g_aPlayer[nCntPlayer].move.z);
			PrintDebugProc("プレイヤー[%d]の向き : (%.3f, %.3f, %.3f)\n", nCntPlayer, g_aPlayer[nCntPlayer].rot.x, g_aPlayer[nCntPlayer].rot.y, g_aPlayer[nCntPlayer].rot.z);
			PrintDebugProc("プレイヤー[%d]の目的の向き : (%.3f, %.3f, %.3f)\n", nCntPlayer, g_aPlayer[nCntPlayer].rotDest.x, g_aPlayer[nCntPlayer].rotDest.y, g_aPlayer[nCntPlayer].rotDest.z);
		}
	}
}

//========================================================================
// プレイヤーの描画処理
//========================================================================
void DrawPlayer(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();	// デバイスの取得
	D3DXMATRIX mtxRot, mtxTrans;				// 計算用マトリックス
	D3DMATERIAL9 matDef;						// 現在のマテリアル保存用
	D3DXMATERIAL* pMat;							// マテリアルデータへのポインタ
	Player* pPlayer = &g_aPlayer[0];

	for (int nCntPlayer = 0; nCntPlayer < MAX_PLAYER; nCntPlayer++, pPlayer++)
	{
		if (pPlayer->bUse == false)
		{
			continue;
		}

		// ワールドマトリックスの初期化
		D3DXMatrixIdentity(&pPlayer->mtxWorld);

		// 向きを反映
		D3DXMatrixRotationYawPitchRoll(&mtxRot, pPlayer->rot.y, pPlayer->rot.x, pPlayer->rot.z);
		D3DXMatrixMultiply(&pPlayer->mtxWorld, &pPlayer->mtxWorld, &mtxRot);

		// 位置を反映
		D3DXMatrixTranslation(&mtxTrans, pPlayer->pos.x, pPlayer->pos.y, pPlayer->pos.z);
		D3DXMatrixMultiply(&pPlayer->mtxWorld, &pPlayer->mtxWorld, &mtxTrans);

		// ワールドマトリックスの設定
		pDevice->SetTransform(D3DTS_WORLD, &pPlayer->mtxWorld);

		// 現在のマテリアルを取得
		pDevice->GetMaterial(&matDef);

		// 全モデル(パーツ)の描画
		for (int nCntOffSetModel = 0; nCntOffSetModel < pPlayer->pModelData->nNumModel; nCntOffSetModel++)
		{
			D3DXMATRIX mtxRotOffSetModel, mtxTransOffSetModel;	// 計算用マトリックス
			D3DXMATRIX mtxParent;								// 親のマトリックス

			// パーツのワールドマトリックスを初期化
			D3DXMatrixIdentity(&pPlayer->pModelData->aModel[nCntOffSetModel].mtxWorld);

			// パーツの向きを反映
			D3DXMatrixRotationYawPitchRoll(&mtxRotOffSetModel, pPlayer->pModelData->aModel[nCntOffSetModel].rot.y, pPlayer->pModelData->aModel[nCntOffSetModel].rot.x, pPlayer->pModelData->aModel[nCntOffSetModel].rot.z);
			D3DXMatrixMultiply(&pPlayer->pModelData->aModel[nCntOffSetModel].mtxWorld, &pPlayer->pModelData->aModel[nCntOffSetModel].mtxWorld, &mtxRotOffSetModel);

			// パーツの位置を反映(オフセット)
			D3DXMatrixTranslation(&mtxTransOffSetModel, pPlayer->pModelData->aModel[nCntOffSetModel].pos.x, pPlayer->pModelData->aModel[nCntOffSetModel].pos.y, pPlayer->pModelData->aModel[nCntOffSetModel].pos.z);
			D3DXMatrixMultiply(&pPlayer->pModelData->aModel[nCntOffSetModel].mtxWorld, &pPlayer->pModelData->aModel[nCntOffSetModel].mtxWorld, &mtxTransOffSetModel);

			// パーツの「親のマトリックス」を設定
			if (pPlayer->pModelData->aModel[nCntOffSetModel].nIdxModelParent != -1)
			{// 親モデルがある場合
				mtxParent = pPlayer->pModelData->aModel[pPlayer->pModelData->aModel[nCntOffSetModel].nIdxModelParent].mtxWorld;
			}
			else
			{// 親モデルがない場合
				mtxParent = pPlayer->mtxWorld;
			}

			// 算出した「パーツのワールドマトリックス」と「親のマトリックス」を掛け合わせる
			D3DXMatrixMultiply(&pPlayer->pModelData->aModel[nCntOffSetModel].mtxWorld,
				&pPlayer->pModelData->aModel[nCntOffSetModel].mtxWorld,
				&mtxParent);

			// パーツのワールドマトリックスを設定
			pDevice->SetTransform(D3DTS_WORLD, &pPlayer->pModelData->aModel[nCntOffSetModel].mtxWorld);

			// マテリアルデータへのポインタを取得
			pMat = (D3DXMATERIAL*)pPlayer->pModelData->aModel[nCntOffSetModel].pBuffMat->GetBufferPointer();

			for (int nCntMat = 0; nCntMat < (int)pPlayer->pModelData->aModel[nCntOffSetModel].dwNumMat; nCntMat++)
			{
				// マテリアルの設定
				pDevice->SetMaterial(&pMat[nCntMat].MatD3D);

				// テクスチャの設定
				pDevice->SetTexture(0, pPlayer->pModelData->aModel[nCntOffSetModel].apTexture[nCntMat]);

				// プレイヤー(パーツ)の描画
				pPlayer->pModelData->aModel[nCntOffSetModel].pMesh->DrawSubset(nCntMat);
			}
		}

		// 保存していたマテリアルを戻す
		pDevice->SetMaterial(&matDef);
	}
}

//========================================================================
// プレイヤーの情報を渡す
//========================================================================
Player* GetPlayer(void)
{
	return &g_aPlayer[0];
}

//========================================================================
// プレイヤーを設置する
//========================================================================
void SetPlayer(int nIdx, D3DXVECTOR3 pos, D3DXVECTOR3 rot)
{
	g_aPlayer[nIdx].pModelData = SetModelData(PARENTMODELTYPE_PLAYER);
	g_aPlayer[nIdx].motion.pMotionData = SetMotionData(MOTIONDATATYPE_PLAYER);

	g_aPlayer[nIdx].bUse = true;
	g_aPlayer[nIdx].pos = pos;
	g_aPlayer[nIdx].posOld = pos;
	g_aPlayer[nIdx].rot = rot;
	g_aPlayer[nIdx].nIdxShadow = SetShadow(SHADOWTYPE_CIRCLE, SHADOｗ, SHADOｗ);

	SetMotion(&g_aPlayer[nIdx].motion, g_aPlayer[nIdx].pModelData, (MOTIONTYPE)PLAYERMOTIONTYPE_NEUTRAL, true, false, 10);
}
