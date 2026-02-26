//========================================================================
// 
// player処理[player.cpp]
// Author : Kajino Hiyori
//
//========================================================================
#include "main.h"
#include "game.h"
#include "debugproc.h"
#include "player.h"
#include "shadow.h"
#include "camera.h"
#include "title.h"
#include "meshfield.h"
#include "input.h"
#include "loadscript.h"
#include "magic.h"
#include "uimanager.h"
#include "particle.h"
#include "goal.h"

// マクロ定義
#define MAX_SHADOW		(1)					// 影テクスチャの最大数
#define MAX_MODEL		(1)					// モデルの最大数
#define MOVE			(0.5f)				// 移動量
#define ROTATE			(0.9f)				// 回転量
#define GRAVITY			(0.75f)				// 重力
#define JUMP			(17.0f)				// ジャンプ
#define SHADOｗ			(10.0f)				// 影の大きさ
#define CORRECTION_ROT	(0.075f)			// 回転の減衰係数
#define RIGHT			(D3DX_PI / 2)		// 右を向く
#define LEFT			(-(D3DX_PI / 2))	// 左を向く
#define BACK			(D3DX_PI)			// 後ろを向く
#define FRONT			(0.0f)				// 正面を向く
#define LENGTH			(50.0f)				// フィールドの範囲
#define BLENDFRAME		(10)				// モーションブレンドのフレーム数
#define POS				(D3DXVECTOR3(0.0f, 50.0f, 0.0f))	// プレイヤーの位置
#define NORMAL			(D3DXVECTOR3(0.0f, 1.0f, 0.0f))		// 基本の法線
#define INERTIA			(0.1f)								// 慣性
#define FLOATSPEED		(0.7625f)							// 浮遊速度
#define FLOATINERTIA	(0.005f)							// 浮遊中慣性
#define FLOATMOVE		(0.015f)							// 浮遊中移動量
#define ACCELEMOVE		(0.175f)							// 加速中移動量
#define ACCELEINERTIA	(0.025f)							// 加速中

// テクスチャの読み込み
const char* c_apFilenamePlayer[MAX_SHADOW] =
{
	"data\\TEXTURE\\shadow_col.png",
};

//グローバル変数
LPDIRECT3DTEXTURE9	g_apTextureBuffPlayer[MAX_SHADOW] = {};
LPD3DXMESH			g_pMeshPlayer[MAX_PLAYER] = {};				// メッシュ(頂点情報)へのポインタ
LPD3DXBUFFER		g_pBuffMatPlayer[MAX_PLAYER] = {};			// マテリアルへのポインタ
DWORD				g_dwNumMatPlayer[MAX_PLAYER] = {0, 0};		// マテリアルの数
LPDIRECT3DTEXTURE9	g_apTexturePlayer[MAX_PLAYER][MAX_PLAYERMAT];		// テクスチャへの	ポインタ
Player				g_aPlayer[MAX_PLAYER];						// プレイヤーの情報を格納

//========================================================================
// プレイヤーの初期化処理
//========================================================================
void InitPlayer(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();	// デバイスの取得
	MODE mode = GetMode();	// 現在のモードを取得

	//テクスチャ読み込み
	for (int nCntShadow = 0; nCntShadow < MAX_SHADOW; nCntShadow++)
	{
		D3DXCreateTextureFromFile(pDevice, c_apFilenamePlayer[nCntShadow], &g_apTextureBuffPlayer[nCntShadow]);
	}

	for (int nCntPlayer = 0; nCntPlayer < MAX_PLAYER; nCntPlayer++)
	{
		for (int nCntCommand = 0; nCntCommand < MAX_OWNCOMMAND; nCntCommand++)
		{
			g_aPlayer[nCntPlayer].magicbook.OwnCommand[nCntCommand] = COMMANDOREDER_NONE;
		}
		g_aPlayer[nCntPlayer].magicbook.nCntOwn = 0;
	}
	// 初期化
	for (int nCntPlayer = 0; nCntPlayer < MAX_PLAYER; nCntPlayer++)
	{
		g_aPlayer[nCntPlayer].pos			= INIT_D3DXVEC3;
		g_aPlayer[nCntPlayer].posOld		= INIT_D3DXVEC3;
		g_aPlayer[nCntPlayer].rot			= INIT_D3DXVEC3;
		g_aPlayer[nCntPlayer].rotDest		= INIT_D3DXVEC3;
		g_aPlayer[nCntPlayer].move			= INIT_D3DXVEC3;
		g_aPlayer[nCntPlayer].nIdxShadow	= -1;
		g_aPlayer[nCntPlayer].fSpeed		= MOVE;
		g_aPlayer[nCntPlayer].state			= PLAYERSTATE_NORMAL;
		g_aPlayer[nCntPlayer].bKey1			= false;
		g_aPlayer[nCntPlayer].bKey2			= false;
		g_aPlayer[nCntPlayer].bKey3			= false;
		g_aPlayer[nCntPlayer].bUse			= false;
		g_aPlayer[nCntPlayer].bJump			= false;
		g_aPlayer[nCntPlayer].motion.motionTypeBlend = (MOTIONTYPE)PLAYERMOTIONTYPE_NEUTRAL;
		g_aPlayer[nCntPlayer].motion.motionType = (MOTIONTYPE)PLAYERMOTIONTYPE_NEUTRAL;
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
	float fInertia = INERTIA;	// 慣性

	int nDropMagicIdx;									// 落ちてる魔法保管用
	COMMANDOREDER InputCommand = COMMANDOREDER_NONE;	// 入力したコマンド
	MAGICTYPE CurrentMagictype;							// 今使っている魔法

	bool bPause = false;	// ポーズ状態の確認

	// 過去の位置を保存
	for (int nCntPlayer = 0; nCntPlayer < MAX_PLAYER; nCntPlayer++)
	{
		if (g_aPlayer[nCntPlayer].bUse == false)
		{
			continue;
		}
		g_aPlayer[nCntPlayer].posOld = g_aPlayer[nCntPlayer].pos;
	}

	// 操作
	for (int nCntPlayer = 0; nCntPlayer < MAX_PLAYER; nCntPlayer++)
	{
		if (g_aPlayer[nCntPlayer].bUse == false)
		{
			continue;
		}
		// 過去の位置を保存
		g_aPlayer[nCntPlayer].posOld = g_aPlayer[nCntPlayer].pos;
		moveDir = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		// ポーズ状態の取得
		bPause = GetPause(nCntPlayer);

		// 落ちてる魔法との判定 (保管)
		nDropMagicIdx = CollisionMagic(g_aPlayer[nCntPlayer].pos, g_aPlayer[nCntPlayer].fRadius,nCntPlayer);

		switch (g_aPlayer[nCntPlayer].state)
		{
		case PLAYERSTATE_NORMAL:	// 通常時
			if (bPause == true)	// ポーズ状態の場合、各種処理を行わない[]
			{
			
			}
			else
			{
				// キー入力を受け付ける====================================
					// 移動方向を管理
				if ((GetKeyboardPress(DIK_A) == true && nCntPlayer == 0) || (GetKeyboardPress(DIK_J) == true && nCntPlayer == 1) || GetJoypadPress(JOYKEY_LEFT, nCntPlayer) == true)	// 左に移動
				{
					moveDir.x -= 1.0f;
				}
				else if ((GetKeyboardPress(DIK_D) == true && nCntPlayer == 0) || (GetKeyboardPress(DIK_L) == true && nCntPlayer == 1) || GetJoypadPress(JOYKEY_RIGHT, nCntPlayer) == true)	// 右に移動
				{
					moveDir.x += 1.0f;
				}
				if ((GetKeyboardPress(DIK_W) == true && nCntPlayer == 0) || (GetKeyboardPress(DIK_I) == true && nCntPlayer == 1) || GetJoypadPress(JOYKEY_UP, nCntPlayer) == true)	// 奥に移動
				{
					moveDir.z += 1.0f;
				}
				else if ((GetKeyboardPress(DIK_S) == true && nCntPlayer == 0) || (GetKeyboardPress(DIK_K) == true && nCntPlayer == 1) || GetJoypadPress(JOYKEY_DOWN, nCntPlayer) == true)	// 手前に移動
				{
					moveDir.z -= 1.0f;
				}

				// ジャンプ処理
				if ((GetKeyboardTrigger(DIK_SPACE) == true || GetJoypadTrigger(JOYKEY_A, nCntPlayer) == true) && g_aPlayer[nCntPlayer].bJump == false)
				{
					g_aPlayer[nCntPlayer].move.y = JUMP;
					g_aPlayer[nCntPlayer].bJump = true;
					SetMotion(&g_aPlayer[nCntPlayer].motion, g_aPlayer[nCntPlayer].pModelData, &g_aPlayer[nCntPlayer].OffSetData, (MOTIONTYPE)PLAYERMOTIONTYPE_JUMP, false, true, BLENDFRAME);
				}

				if ((GetJoypadTrigger(JOYKEY_X, nCntPlayer) == true || (GetKeyboardTrigger(DIK_RETURN) == true && nCntPlayer == 0)) && nDropMagicIdx != COMMANDOREDER_NONE)
				{// Xボタンを押したかつ何かしらのコマンドが近くにある
					OwnCommand(&g_aPlayer[nCntPlayer].magicbook, nDropMagicIdx);
				}

				// 移動方向の正規化
				D3DXVec3Normalize(&moveDir, &moveDir);

				// スティックの入力方向を利用
				GetJoypadStickLeft(&moveDir.x, &moveDir.z, nCntPlayer);

				// 移動状態を求める(fMoveDir == 0は移動していない)
				fMoveDir = SQRTF(moveDir.x, moveDir.z);

			}
			break;

		case PLAYERSTATE_PAUSE:

			break;

		case PLAYERSTATE_SPELL:
			if (g_aPlayer[nCntPlayer].bJump == false) // 設置中のみ受け付ける
			{
				InputCommand = PressCommand(nCntPlayer);	// 呪文の入力を受け付ける
			}
			break;

		case PLAYERSTATE_MAGIC:

			break;
		}

		PrintDebugProc("%dプレイヤーのステート : %d [0 NORMAL / 1 PAUSE / 2 SPELL / 3 MAGIC]\n", nCntPlayer,g_aPlayer[nCntPlayer].state);

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


			fAngle = atan2f(-moveDir.x, moveDir.z);
			fAngle = AngleNormalize(fAngle);
			g_aPlayer[nCntPlayer].rotDest.y = fRotDest;
			fRotDest = AngleNormalize(fRotDest);

			if (g_aPlayer[nCntPlayer].bJump == false && g_aPlayer[nCntPlayer].motion.motionTypeBlend != (MOTIONTYPE)PLAYERMOTIONTYPE_MOVE)
			{// ジャンプ状態じゃないかつ移動モーション中じゃない
				SetMotion(&g_aPlayer[nCntPlayer].motion, g_aPlayer[nCntPlayer].pModelData, &g_aPlayer[nCntPlayer].OffSetData, (MOTIONTYPE)PLAYERMOTIONTYPE_MOVE, true, true, BLENDFRAME);
			}
		}
		else if (g_aPlayer[nCntPlayer].motion.motionTypeBlend == (MOTIONTYPE)PLAYERMOTIONTYPE_MOVE)
		{// もし歩行中だったら
			SetMotion(&g_aPlayer[nCntPlayer].motion, g_aPlayer[nCntPlayer].pModelData, &g_aPlayer[nCntPlayer].OffSetData, (MOTIONTYPE)PLAYERMOTIONTYPE_NEUTRAL, true, true, BLENDFRAME);
		}

		// プレイヤーの方向を補正
		fRotDiffKey = g_aPlayer[nCntPlayer].rotDest.y - g_aPlayer[nCntPlayer].rot.y;	// 差分を計算
		fRotDiffKey = AngleNormalize(fRotDiffKey);
		g_aPlayer[nCntPlayer].rot.y += (fRotDiffKey) * CORRECTION_ROT;
		g_aPlayer[nCntPlayer].rot.y = AngleNormalize(g_aPlayer[nCntPlayer].rot.y);

		// 今使っている魔法を取得
		CurrentMagictype = GetCurrentMagicType(nCntPlayer);

		switch (CurrentMagictype)
		{
		case MAGICTYPE_NONE:
			g_aPlayer[nCntPlayer].fSpeed = MOVE;
			fInertia = INERTIA;
			break;

			// 浮遊魔法発動中
		case MAGICTYPE_LEVITATION:
			g_aPlayer[nCntPlayer].bJump = true;				// ジャンプ状態にする
			g_aPlayer[nCntPlayer].move.y += FLOATSPEED;		// 浮遊速度加算
			g_aPlayer[nCntPlayer].fSpeed = FLOATMOVE;		// 浮遊中の移動量に
			fInertia = FLOATINERTIA;						// 浮遊中の慣性に
			SetParticle(g_aPlayer[nCntPlayer].pos, 1, PARTICLETYPE_LEVITATION);
			break;

			// 加速魔法発動中
		case MAGICTYPE_ACCELERATION:
			g_aPlayer[nCntPlayer].fSpeed = ACCELEMOVE;		// 加速中の移動量に
			fInertia = ACCELEINERTIA;						// 加速中の慣性に
			break;
		}

		// 重力
		g_aPlayer[nCntPlayer].move.y -= GRAVITY;

		// 位置の更新
		g_aPlayer[nCntPlayer].pos += g_aPlayer[nCntPlayer].move;

		// メッシュフィールドとの当たり判定
		if (CollisionMeshField(&g_aPlayer[nCntPlayer].pos, &g_aPlayer[nCntPlayer].posOld, &g_aPlayer[nCntPlayer].move) == true || 
			g_aPlayer[nCntPlayer].pos.y <= 0.0f)
		{
			if (g_aPlayer[nCntPlayer].bJump == true)
			{// ジャンプしている状態で判定があったら
				// 着地モーション
				SetMotion(&g_aPlayer[nCntPlayer].motion, g_aPlayer[nCntPlayer].pModelData, &g_aPlayer[nCntPlayer].OffSetData, (MOTIONTYPE)PLAYERMOTIONTYPE_LANDING, false, true, BLENDFRAME);
				g_aPlayer[nCntPlayer].state = PLAYERSTATE_NORMAL;
			}
			g_aPlayer[nCntPlayer].pos.y = 0.0f;
			g_aPlayer[nCntPlayer].move.y = 0.0f;
			g_aPlayer[nCntPlayer].bJump = false;
		}

		// オブジェクトとの当たり判定
		if (CollisionObject(&g_aPlayer[nCntPlayer].pos, &g_aPlayer[nCntPlayer].posOld, &g_aPlayer[nCntPlayer].move, g_aPlayer[nCntPlayer].fRadius) && g_aPlayer[nCntPlayer].bJump == true)
		{
			// 着地モーション
			SetMotion(&g_aPlayer[nCntPlayer].motion, g_aPlayer[nCntPlayer].pModelData, &g_aPlayer[nCntPlayer].OffSetData, (MOTIONTYPE)PLAYERMOTIONTYPE_LANDING, false, true, BLENDFRAME);
			g_aPlayer[nCntPlayer].bJump = false;
		}

		

		// ゴールとの当たり判定
		if (CollisionGoal(g_aPlayer[nCntPlayer].pos, g_aPlayer[nCntPlayer].fRadius) && GetJoypadTrigger(JOYKEY_X, nCntPlayer) == true)
		{// 当たっているかつXボタンが押されたら
			// クリア状態にする
			SetGameState(GAMESTATE_CLEAR, 0);
		}

		// 使用したコマンドと所持コマンドを判定
		for (int nCntCommand = 0; nCntCommand < g_aPlayer[nCntPlayer].magicbook.nCntOwn; nCntCommand++)
		{
			if (g_aPlayer[nCntPlayer].magicbook.OwnCommand[nCntCommand] == InputCommand && g_aPlayer[nCntPlayer].magicbook.OwnCommand[nCntCommand] != MAGICTYPE_NONE)
			{// コマンドを所有していたら
				// 魔法を使用する (モーションセット, 魔法セット)
				g_aPlayer[nCntPlayer].state = PLAYERSTATE_MAGIC;
				SetMotion(&g_aPlayer[nCntPlayer].motion, g_aPlayer[nCntPlayer].pModelData, &g_aPlayer[nCntPlayer].OffSetData, (MOTIONTYPE)PLAYERMOTIONTYPE_ACTION, false, true, BLENDFRAME);
				SetMagic(ChangeMagic(InputCommand), g_aPlayer[nCntPlayer].pos, g_aPlayer[nCntPlayer].rot, INIT_D3DXVEC3, nCntPlayer);
				break;
			}
		}

		// 移動量の更新
		g_aPlayer[nCntPlayer].move.x += (0.0f - g_aPlayer[nCntPlayer].move.x) * fInertia;
		g_aPlayer[nCntPlayer].move.y += (0.0f - g_aPlayer[nCntPlayer].move.y) * fInertia;
		g_aPlayer[nCntPlayer].move.z += (0.0f - g_aPlayer[nCntPlayer].move.z) * fInertia;

		// ジャンプモーションから着地モーションへの切り替え
		if (g_aPlayer[nCntPlayer].motion.motionTypeBlend == (MOTIONTYPE)PLAYERMOTIONTYPE_LANDING && g_aPlayer[nCntPlayer].motion.nKey + 1 >= g_aPlayer[nCntPlayer].motion.nNumKey)
		{
			SetMotion(&g_aPlayer[nCntPlayer].motion, g_aPlayer[nCntPlayer].pModelData, &g_aPlayer[nCntPlayer].OffSetData, (MOTIONTYPE)PLAYERMOTIONTYPE_NEUTRAL, true, true, BLENDFRAME);
		}

		// モーションの更新処理
		UpdateMotion(&g_aPlayer[nCntPlayer].motion, g_aPlayer[nCntPlayer].pModelData, &g_aPlayer[nCntPlayer].OffSetData);

		// デバッグ表示
		PrintDebugProc("プレイヤー[%d]の位置 : (%.3f, %.3f, %.3f)\n", nCntPlayer, g_aPlayer[nCntPlayer].pos.x, g_aPlayer[nCntPlayer].pos.y, g_aPlayer[nCntPlayer].pos.z);
		PrintDebugProc("プレイヤー[%d]の移動量 : (%.3f, %.3f, %.3f)\n", nCntPlayer, g_aPlayer[nCntPlayer].move.x, g_aPlayer[nCntPlayer].move.y, g_aPlayer[nCntPlayer].move.z);
		PrintDebugProc("プレイヤー[%d]の向き : (%.3f, %.3f, %.3f)\n", nCntPlayer, g_aPlayer[nCntPlayer].rot.x, g_aPlayer[nCntPlayer].rot.y, g_aPlayer[nCntPlayer].rot.z);
		PrintDebugProc("プレイヤー[%d]の目的の向き : (%.3f, %.3f, %.3f)\n", nCntPlayer, g_aPlayer[nCntPlayer].rotDest.x, g_aPlayer[nCntPlayer].rotDest.y, g_aPlayer[nCntPlayer].rotDest.z);

		PrintDebugProc("入力コマンド : %d\n", InputCommand);

		PrintDebugProc("コマンドタイプ : %d\n", nDropMagicIdx);

		for (int nCntCommand = 0; nCntCommand < MAX_OWNCOMMAND; nCntCommand++)
		{
			PrintDebugProc("[%d]所有コマンド : %d\n", nCntCommand, g_aPlayer[nCntPlayer].magicbook.OwnCommand[nCntCommand]);
		}
		PrintDebugProc("コマンド数 : %d\n", g_aPlayer[nCntPlayer].magicbook.nCntOwn);
		PrintDebugProc("使用状態 : %d\n", g_aPlayer[nCntPlayer].bUse);
	
	}
}

//========================================================================
// プレイヤーの描画処理
//========================================================================
void DrawPlayer(void)
{
	LPDIRECT3DDEVICE9	pDevice = GetDevice();		// デバイスの取得
	D3DXMATRIX			mtxRot, mtxTrans;			// 計算用マトリックス
	D3DMATERIAL9		matDef;						// 現在のマテリアル保存用
	D3DXMATERIAL*		pMat;						// マテリアルデータへのポインタ
	Player*				pPlayer = &g_aPlayer[0];	// プレイヤーのポインタを取得

	// 影の描画用変数
	D3DXMATRIX mtxShadow;		// シャドウマトリックス
	D3DLIGHT9 light;			// ライトの情報
	D3DXVECTOR4 posLight;		// ライトの位置
	D3DXVECTOR3 pos, normal;	// 平面の点, 法線ベクトル
	D3DXPLANE plane;			// 平面の情報

	D3DXMATERIAL mat;			// マテリアル保持用

	// ライトの位置を設定
	pDevice->GetLight(0, &light);
	posLight = D3DXVECTOR4(-light.Direction.x, -light.Direction.y, -light.Direction.z, 0.0f);

	// 平面情報を生成
	pos = D3DXVECTOR3(0.0f, 0.05f, 0.0f);		// 高さが0ならなんでもいい
	normal = NORMAL_PLANE;
	D3DXPlaneFromPointNormal(&plane, &pos, &normal);

	for (int nCntPlayer = 0; nCntPlayer < MAX_PLAYER; nCntPlayer++, pPlayer++)
	{
		if (pPlayer->bUse == false)
		{
			continue;
		}

		// 現在のマテリアルを取得
		pDevice->GetMaterial(&matDef);

		// 全モデル(パーツ)の描画
		for (int nCntOffSetModel = 0; nCntOffSetModel < pPlayer->pModelData->nNumModel; nCntOffSetModel++)
		{
			D3DXMATRIX mtxRotOffSetModel, mtxTransOffSetModel;	// 計算用マトリックス
			D3DXMATRIX mtxParent;								// 親のマトリックス

			// ワールドマトリックスの初期化(デフォルトの値にする)
			D3DXMatrixIdentity(&mtxShadow);

			// シャドウマトリックスの生成
			D3DXMatrixShadow(&mtxShadow, &posLight, &plane);
			D3DXMatrixMultiply(&mtxShadow, &pPlayer->pModelData->aModel[nCntOffSetModel].mtxWorld, &mtxShadow);

			// パーツのワールドマトリックスを設定
			pDevice->SetTransform(D3DTS_WORLD, &mtxShadow);

			// マテリアルデータへのポインタを取得
			pMat = (D3DXMATERIAL*)pPlayer->pModelData->aModel[nCntOffSetModel].pBuffMat->GetBufferPointer();

			for (int nCntMat = 0; nCntMat < (int)pPlayer->pModelData->aModel[nCntOffSetModel].dwNumMat; nCntMat++)
			{
				mat = pMat[nCntMat];

				//mat.MatD3D.Diffuse.r = 0.0f;
				//mat.MatD3D.Diffuse.g = 0.0f;
				//mat.MatD3D.Diffuse.b = 0.0f;

				// マテリアルの設定
				pDevice->SetMaterial(&mat.MatD3D);

				// テクスチャの設定
				pDevice->SetTexture(0, g_apTextureBuffPlayer[0]);

				// プレイヤー(パーツ)の描画
				pPlayer->pModelData->aModel[nCntOffSetModel].pMesh->DrawSubset(nCntMat);
			}
		}

		// 保存していたマテリアルを戻す
		pDevice->SetMaterial(&matDef);
	}

	pPlayer = &g_aPlayer[0];

	for (int nCntPlayer = 0; nCntPlayer < MAX_PLAYER; nCntPlayer++, pPlayer++)
	{
		if (pPlayer->bUse == false)
		{
			continue;
		}

		// 描画関数
		DrawParentModel(&pPlayer->pos, &pPlayer->rot, &pPlayer->mtxWorld, pPlayer->pModelData, &pPlayer->OffSetData);
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
void SetPlayer(int nIdx, D3DXVECTOR3 pos, D3DXVECTOR3 rot, PARENTMODELTYPE parentmodeltype)
{
	g_aPlayer[nIdx].pModelData = SetModelData(parentmodeltype);					// モデルデータを設定
	g_aPlayer[nIdx].motion.pMotionData = SetMotionData(MOTIONDATATYPE_PLAYER);	// モーションデータを設定

	// 各種設定
	g_aPlayer[nIdx].bUse = true;
	g_aPlayer[nIdx].pos = pos;
	g_aPlayer[nIdx].posOld = pos;
	g_aPlayer[nIdx].rot = rot;
	g_aPlayer[nIdx].nIdxShadow = SetShadow(SHADOWTYPE_CIRCLE, SHADOｗ, SHADOｗ);

	// モーションを設定
	SetMotion(&g_aPlayer[nIdx].motion, g_aPlayer[nIdx].pModelData, &g_aPlayer[nIdx].OffSetData, (MOTIONTYPE)PLAYERMOTIONTYPE_NEUTRAL, true, false, 10);
}

//========================================================================
// コマンドを取得する
//========================================================================
void OwnCommand(MagicBook* pMagicBook, int nDropMagicIdx)
{
	// 過去の情報を格納
	COMMANDOREDER ownCommandOld[MAX_OWNCOMMAND];
	
	for (int nCntCommand = 0; nCntCommand < MAX_OWNCOMMAND; nCntCommand++)
	{
		ownCommandOld[nCntCommand] = pMagicBook->OwnCommand[nCntCommand];
	}

	if (pMagicBook->nCntOwn <= 0)	// 1つも魔法を持っていない場合
	{
		pMagicBook->OwnCommand[0] = GetFieldMagic(nDropMagicIdx);
		pMagicBook->nCntOwn++;
		return;	// 処理を終了
	}
	else
	{
		//エラーを吐く可能性アリ\\

		pMagicBook->nCntOwn++;
		// 所持数が4を越えないように管理
		if (pMagicBook->nCntOwn > MAX_OWNCOMMAND)
		{
			pMagicBook->nCntOwn = MAX_OWNCOMMAND;
		}
		// 1つ以上の魔法を所持している場合
		//pMagicBook->OwnCommand[1] = ownCommandOld[0];
		//pMagicBook->OwnCommand[2] = ownCommandOld[1];
		//pMagicBook->OwnCommand[3] = ownCommandOld[2];
		for (int nCntCommand = 0; nCntCommand < pMagicBook->nCntOwn -1; nCntCommand++)
		{
			pMagicBook->OwnCommand[nCntCommand + 1] = ownCommandOld[nCntCommand];
		}

		pMagicBook->OwnCommand[0] = GetFieldMagic(nDropMagicIdx);
	}

#if 0
	if (pMagicBook->nCntOwn <= 0)
	{
		pMagicBook->OwnCommand[0] = GetFieldMagic(nDropMagicIdx);
		pMagicBook->nCntOwn++;
		return;
	}

	for (int nCntCommand = 0; nCntCommand < pMagicBook->nCntOwn; nCntCommand++)
	{
		pMagicBook->OwnCommand[pMagicBook->nCntOwn - nCntCommand] = pMagicBook->OwnCommand[pMagicBook->nCntOwn - nCntCommand - 1];
	}

	pMagicBook->OwnCommand[0] = GetFieldMagic(nDropMagicIdx);
	pMagicBook->nCntOwn++;
#endif
}
