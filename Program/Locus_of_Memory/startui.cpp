//=============================================================================
//
//	ロゴ処理 [startui.cpp]
//	Author : Kajino Hiyori
// 
//=============================================================================
#include "main.h"
#include "startui.h"
#include "debugproc.h"
#include "color.h"
#include "input.h"

// マクロ定義
#define LOGO_SIZE	(20.0f)	// ロゴの大きさ
#define LOGO_HEIGHT	(50.0f)	// ロゴの高さ
#define SHADOW		(20.0f)	// 影の大きさ
#define POS			(D3DXVECTOR3(0.0f, 0.0f, 0.0f))		// ロゴの位置
#define DEFAULT		(D3DXVECTOR3(0.0f, 0.0f, 0.0f))		// xyzが0.0fの場合
#define NORMAL		(D3DXVECTOR3(0.0f, 1.0f, 0.0f))		// 基本の法線

// グローバル変数
Logo g_Logo;	// ロゴの情報を格納
int g_nCounterVibration;	// バイブレーション間隔のカウント

// キー情報
static LOGOKEY_INFO g_aLogoKeyAction[]	// ACTION[1]
{
	// KEY0
	{20,	// 再生フレーム
	{{0.00f,  0.00f,  0.00f,	0.00f, 0.00f, 0.00f}}},

	// KEY1
	{20,	// 再生フレーム
	{{0.00f,  0.00f,  0.00f,	0.00f, 1.57f, 0.00f}}},

	// KEY2
	{20,	// 再生フレーム
	{{0.00f,  0.00f,  0.00f,	0.00f, 3.14f, 0.00f}}},

	// KEY3
	{20,	// 再生フレーム
	{{0.00f,  0.00f,  0.00f,	0.00f,-1.57f, 0.00f}}},
};

// モデルの読み込み
const char* c_apFilenameLogoModel[LOGOMODEL_MAX] =
{
	"data\\MODEL\\teamlogo\\teamlogo.x",	// 本体[0]
};

//======================================================================================
// ロゴの初期化処理
//======================================================================================
void InitStartUI(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();	// デバイスの取得
	D3DXMATERIAL* pMat;
	int nNumVtx = 0;	// 頂点数
	DWORD dwSizeFVF;			// 頂点フォーマットのサイズ

	// モーションの初期化処理
	InitLogoMotion();

	// 初期化
	g_Logo.pos = POS;					// 位置
	g_Logo.posOld = DEFAULT;				// 前回の位置
	g_Logo.move = DEFAULT;				// 移動量
	g_Logo.rot = DEFAULT;				// 向き
	g_Logo.rotDest = DEFAULT;				// 目的の向き
	g_Logo.bDisp = false;				// 表示状態
	g_Logo.nNumMotion = LOGOMOTIONTYPE_MAX;								// モーションの総数を設定
	g_Logo.motionType = LOGOMOTIONTYPE_ACTION;							// 現在のモーション(待機)
	g_Logo.bLoopMotion = g_Logo.aMotionInfo[g_Logo.motionType].bLoop;		// モーションのループを管理
	g_Logo.nNumKey = g_Logo.aMotionInfo[g_Logo.motionType].nNumKey;	// キーの総数
	g_Logo.nKey = 0;										// 現在実行されているキー
	g_Logo.nCounterMotion = 0;										// キーモーションカウンター
	g_Logo.nNumModel = LOGOMODEL_MAX;									// 使用されているモデル数
	g_Logo.bFinishMotion = false;	// モーションが終了していない

	for (int nCntModel = 0; nCntModel < LOGOMODEL_MAX; nCntModel++)
	{
		// Xファイルの読み込み
		D3DXLoadMeshFromX(c_apFilenameLogoModel[nCntModel], D3DXMESH_SYSTEMMEM, pDevice, NULL, &g_Logo.aModel[nCntModel].pBuffMat, NULL, &g_Logo.aModel[nCntModel].dwNumMat, &g_Logo.aModel[nCntModel].pMesh);

		// 頂点数を取得
		nNumVtx = g_Logo.aModel[nCntModel].pMesh->GetNumVertices();
		// 頂点フォーマットのサイズを取得
		dwSizeFVF = D3DXGetFVFVertexSize(g_Logo.aModel[nCntModel].pMesh->GetFVF());

		// マテリアルデータへのポインタを取得
		pMat = (D3DXMATERIAL*)g_Logo.aModel[nCntModel].pBuffMat->GetBufferPointer();

		for (int nCntMat = 0; nCntMat < (int)g_Logo.aModel[nCntModel].dwNumMat; nCntMat++)
		{
			if (pMat[nCntMat].pTextureFilename != NULL)	// テクスチャファイルが存在する場合
			{
				// テクスチャの読み込み
				D3DXCreateTextureFromFile(pDevice, pMat[nCntMat].pTextureFilename, &g_Logo.aModel[nCntModel].apTexture[nCntMat]);
			}
		}

	}

	// 本体[0]の情報を設定
	g_Logo.aModel[0].nIdxModelParent = -1;
	g_Logo.aModel[0].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	g_Logo.aOffSet[0] = g_Logo.aModel[0].pos;
	g_Logo.aModel[0].rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	g_Logo.aOffSetRot[0] = g_Logo.aModel[0].rot;

	// ロゴの設置
	SetLogo(D3DXVECTOR3(10000.0f, 0.0f, -10300.0f));
}

//======================================================================================
// ロゴの終了処理
//======================================================================================
void UninitStartUI(void)
{
	for (int nCntModel = 0; nCntModel < g_Logo.nNumModel; nCntModel++)
	{
		// メッシュの破棄
		if (g_Logo.aModel[nCntModel].pMesh != NULL)
		{
			g_Logo.aModel[nCntModel].pMesh->Release();
			g_Logo.aModel[nCntModel].pMesh = NULL;
		}

		// テクスチャの破棄
		for (int nCntMat = 0; nCntMat < (int)g_Logo.aModel[nCntModel].dwNumMat; nCntMat++)
		{
			if (g_Logo.aModel[nCntModel].apTexture[nCntMat] != NULL)
			{
				g_Logo.aModel[nCntModel].apTexture[nCntMat]->Release();
				g_Logo.aModel[nCntModel].apTexture[nCntMat] = NULL;
			}
		}

		// マテリアルの破棄
		if (g_Logo.aModel[nCntModel].pBuffMat != NULL)
		{
			g_Logo.aModel[nCntModel].pBuffMat->Release();
			g_Logo.aModel[nCntModel].pBuffMat = NULL;
		}
	}
}

//======================================================================================
// ロゴの更新処理
//======================================================================================
void UpdateStartUI(void)
{
	// モーションの更新
	UpdateLogoMotion();

	if (GetKeyboardPress(DIK_D) == true)
	{
		g_Logo.pos.x++;
	}
	else if (GetKeyboardPress(DIK_A) == true)
	{
		g_Logo.pos.x--;
	}
	if (GetKeyboardPress(DIK_W) == true)
	{
		g_Logo.pos.z++;
	}
	else if (GetKeyboardPress(DIK_S) == true)
	{
		g_Logo.pos.z--;
	}
	if (GetKeyboardPress(DIK_Q) == true)
	{
		g_Logo.pos.y++;
	}
	else if (GetKeyboardPress(DIK_Z) == true)
	{
		g_Logo.pos.y--;
	}

	PrintDebugProc("%ロゴモデルの位置 : (%f, %f, %f)\n", g_Logo.pos.x, g_Logo.pos.y, g_Logo.pos.z);

}

//======================================================================================
// ロゴの描画処理
//======================================================================================
void DrawStartUI(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();	// デバイスの取得
	D3DXMATRIX mtxRot, mtxTrans;	// 計算用マトリックス
	D3DMATERIAL9 matDef;	// 現在のマテリアルを保存
	D3DXMATERIAL* pMat;		// マテリアルデータへのポインタ

	if (g_Logo.bDisp == true)
	{
		// ワールドマトリックスの初期化(デフォルトの値にする)
		D3DXMatrixIdentity(&g_Logo.mtxWorld);

		// 向きを反映
		D3DXMatrixRotationYawPitchRoll(&mtxRot, g_Logo.rot.y, g_Logo.rot.x, g_Logo.rot.z);
		D3DXMatrixMultiply(&g_Logo.mtxWorld, &g_Logo.mtxWorld, &mtxRot);

		// 位置を反映
		D3DXMatrixTranslation(&mtxTrans, g_Logo.pos.x, g_Logo.pos.y, g_Logo.pos.z);
		D3DXMatrixMultiply(&g_Logo.mtxWorld, &g_Logo.mtxWorld, &mtxTrans);

		// ワールドマトリックスの設定
		pDevice->SetTransform(D3DTS_WORLD, &g_Logo.mtxWorld);

		// 現在のマテリアルを取得
		pDevice->GetMaterial(&matDef);

		for (int nCntModel = 0; nCntModel < g_Logo.nNumModel; nCntModel++)
		{
			D3DXMATRIX	mtxRotModel, mtxTransModel;	// 計算用マトリックス
			D3DXMATRIX	mtxParent;					// 親のマトリックス

			// パーツのワールドマトリックスを初期化
			D3DXMatrixIdentity(&g_Logo.aModel[nCntModel].mtxWorld);

			// パーツの向きを反映
			D3DXMatrixRotationYawPitchRoll(&mtxRotModel, g_Logo.aModel[nCntModel].rot.y, g_Logo.aModel[nCntModel].rot.x, g_Logo.aModel[nCntModel].rot.z);
			D3DXMatrixMultiply(&g_Logo.aModel[nCntModel].mtxWorld, &g_Logo.aModel[nCntModel].mtxWorld, &mtxRotModel);

			// パーツの位置を反映
			D3DXMatrixTranslation(&mtxTransModel, g_Logo.aModel[nCntModel].pos.x, g_Logo.aModel[nCntModel].pos.y, g_Logo.aModel[nCntModel].pos.z);
			D3DXMatrixMultiply(&g_Logo.aModel[nCntModel].mtxWorld, &g_Logo.aModel[nCntModel].mtxWorld, &mtxTransModel);

			// パーツの親マトリックスを設定
			if (g_Logo.aModel[nCntModel].nIdxModelParent != -1)
			{ // 親モデルがある場合
				mtxParent = g_Logo.aModel[g_Logo.aModel[nCntModel].nIdxModelParent].mtxWorld;
			}
			else
			{ // 親モデルがない場合
				mtxParent = g_Logo.mtxWorld;	// プレイヤーのマトリックスを設定
			}

			// 算出したパーツのワールドマトリックスと親モデルのマトリックスを掛け合わせる
			D3DXMatrixMultiply(&g_Logo.aModel[nCntModel].mtxWorld, &g_Logo.aModel[nCntModel].mtxWorld, &mtxParent);

			// パーツのワールドマトリックスを設定
			pDevice->SetTransform(D3DTS_WORLD, &g_Logo.aModel[nCntModel].mtxWorld);

			// マテリアルデータへのポインタを取得
			pMat = (D3DXMATERIAL*)g_Logo.aModel[nCntModel].pBuffMat->GetBufferPointer();

			// マテリアルごとにプレイヤーを描画
			for (int nCntMat = 0; nCntMat < (int)g_Logo.aModel[nCntModel].dwNumMat; nCntMat++)
			{
				// マテリアルの設定
				pDevice->SetMaterial(&pMat[nCntMat].MatD3D);

				// テクスチャの設定
				pDevice->SetTexture(0, g_Logo.aModel[nCntModel].apTexture[nCntMat]);

				// プレイヤーパーツの描画
				g_Logo.aModel[nCntModel].pMesh->DrawSubset(nCntMat);
			}

		}
		// 保存していたマテリアルに戻す
		pDevice->SetMaterial(&matDef);
	}
}

//======================================================================================
// モーションの初期化処理
//======================================================================================
void InitLogoMotion(void)
{
	// ACTION[2]
	g_Logo.aMotionInfo[LOGOMOTIONTYPE_ACTION].aKeyInfo[0] = g_aLogoKeyAction[0];
	g_Logo.aMotionInfo[LOGOMOTIONTYPE_ACTION].aKeyInfo[1] = g_aLogoKeyAction[1];
	g_Logo.aMotionInfo[LOGOMOTIONTYPE_ACTION].aKeyInfo[2] = g_aLogoKeyAction[2];
	g_Logo.aMotionInfo[LOGOMOTIONTYPE_ACTION].aKeyInfo[3] = g_aLogoKeyAction[3];

	g_Logo.aMotionInfo[LOGOMOTIONTYPE_ACTION].bLoop = true;	// ループなし
	g_Logo.aMotionInfo[LOGOMOTIONTYPE_ACTION].nNumKey = 4;		// キーの総数
}

//======================================================================================
// モーションの更新処理
//======================================================================================
void UpdateLogoMotion(void)
{
	// 全モデル(パーツ)ごとに位置、向きを更新
	// fDiffKeyは現在のキーと次のキーとの差分を取得
	// fRateKeyは現在のキーが何フレーム処理されたかの割合を取得
	for (int nCntModel = 0; nCntModel < g_Logo.nNumModel; nCntModel++)
	{
		LOGOKEY Key = g_Logo.aMotionInfo[g_Logo.motionType].aKeyInfo[g_Logo.nKey].aKey[nCntModel];										// 現在のキー
		LOGOKEY KeyNext = g_Logo.aMotionInfo[g_Logo.motionType].aKeyInfo[(g_Logo.nKey + 1) % g_Logo.nNumKey].aKey[nCntModel];			// 次のフレームのキー
		float	fRateKey = (float)g_Logo.nCounterMotion / (float)g_Logo.aMotionInfo[g_Logo.motionType].aKeyInfo[g_Logo.nKey].nFrame;	// 現在のモーションカウンター / 現在の再生フレーム
		float	fDiffKey = 0.0f;	// 現在のモーションの次のキーと現在のキーの差分
		float	fDiffKeyBlend = 0.0f;	// ブレンドモーションの次のキーと現在のキーの差分
		float	fDiffBrend = 0.0f;	// 現在のモーションとブレンドモーションとの差分
		float	fPosXCurrent = 0.0f;	// 現在のX座標の位置を格納
		float	fPosYCurrent = 0.0f;	// 現在のY座標の位置を格納
		float	fPosZCurrent = 0.0f;	// 現在のZ座標の位置を格納
		float	fRotXCurrent = 0.0f;	// 現在のX座標の角度を格納
		float	fRotYCurrent = 0.0f;	// 現在のY座標の角度を格納
		float	fRotZCurrent = 0.0f;	// 現在のZ座標の角度を格納
		float	fPosXBlend = 0.0f;	// ブレンドモーションのX座標の位置を格納
		float	fPosYBlend = 0.0f;	// ブレンドモーションのY座標の位置を格納
		float	fPosZBlend = 0.0f;	// ブレンドモーションのZ座標の位置を格納
		float	fRotXBlend = 0.0f;	// ブレンドモーションのX座標の角度を格納
		float	fRotYBlend = 0.0f;	// ブレンドモーションのY座標の角度を格納
		float	fRotZBlend = 0.0f;	// ブレンドモーションのZ座標の角度を格納
		float	fPosX = 0.0f;	// 求める値[X座標の位置]
		float	fPosY = 0.0f; // 求める値[Y座標の位置]
		float	fPosZ = 0.0f; // 求める値[Z座標の位置]
		float	fRotX = 0.0f; // 求める値[X座標の角度]
		float	fRotY = 0.0f; // 求める値[Y座標の角度]
		float	fRotZ = 0.0f; // 求める値[Z座標の角度]

		// 位置の更新 [Integer division by zero]に対処すること
		fDiffKey = KeyNext.fPosX - Key.fPosX;
		fPosX = Key.fPosX + fDiffKey * fRateKey;

		// Y
		fDiffKey = KeyNext.fPosY - Key.fPosY;
		fPosY = Key.fPosY + fDiffKey * fRateKey;

		// Z
		fDiffKey = KeyNext.fPosZ - Key.fPosZ;
		fPosZ = Key.fPosZ + fDiffKey * fRateKey;

		// 角度の更新
		// X
		fDiffKey = KeyNext.fRotX - Key.fRotX;
		fDiffKey = AngleNormalize(fDiffKey);
		fRotX = Key.fRotX + fDiffKey * fRateKey;

		// Y
		fDiffKey = KeyNext.fRotY - Key.fRotY;
		fDiffKey = AngleNormalize(fDiffKey);
		fRotY = Key.fRotY + fDiffKey * fRateKey;

		// Z
		fDiffKey = KeyNext.fRotZ - Key.fRotZ;
		fDiffKey = AngleNormalize(fDiffKey);
		fRotZ = Key.fRotZ + fDiffKey * fRateKey;

		// 角度補正
		fRotX = AngleNormalize(fRotX);
		fRotY = AngleNormalize(fRotY);
		fRotZ = AngleNormalize(fRotZ);

		// モデルの位置を更新
		g_Logo.aModel[nCntModel].pos.x = fPosX + g_Logo.aOffSet[nCntModel].x;
		g_Logo.aModel[nCntModel].pos.y = fPosY + g_Logo.aOffSet[nCntModel].y;
		g_Logo.aModel[nCntModel].pos.z = fPosZ + g_Logo.aOffSet[nCntModel].z;

		// モデルの角度を更新
		g_Logo.aModel[nCntModel].rot.x = fRotX + g_Logo.aOffSetRot[nCntModel].x;
		g_Logo.aModel[nCntModel].rot.y = fRotY + g_Logo.aOffSetRot[nCntModel].y;
		g_Logo.aModel[nCntModel].rot.z = fRotZ + g_Logo.aOffSetRot[nCntModel].z;

		// 角度補正
		g_Logo.aModel[nCntModel].rot.x = AngleNormalize(g_Logo.aModel[nCntModel].rot.x);
		g_Logo.aModel[nCntModel].rot.y = AngleNormalize(g_Logo.aModel[nCntModel].rot.y);
		g_Logo.aModel[nCntModel].rot.z = AngleNormalize(g_Logo.aModel[nCntModel].rot.z);

		if (g_Logo.bDisp == true)
		{
		}
	}

	if (g_Logo.bDisp == true)
	{
		// ブレンド無
		g_Logo.nCounterMotion++;
		if (g_Logo.nCounterMotion >= g_Logo.aMotionInfo[g_Logo.motionType].aKeyInfo[g_Logo.nKey].nFrame)
		{
			g_Logo.nCounterMotion = 0;
			g_Logo.nKey++;
			if (g_Logo.bLoopMotion == false && g_Logo.nKey + 1 >= g_Logo.nNumKey)
			{
				// モーション終了
				// g_Logo.bDisp = false;
			}
			g_Logo.nKey = g_Logo.nKey % g_Logo.nNumKey;
		}
		
	}

	if (g_Logo.bDisp == true)
	{
		PrintDebugProc("モーションの種類 : %d\n",  g_Logo.motionType);
		PrintDebugProc("現在のキー : %d\n", g_Logo.nKey);
		PrintDebugProc("キーカウンター : %d\n", g_Logo.nCounterMotion);
		if (g_Logo.bLoopMotion == true)
		{
			PrintDebugProc("ループ状態 : true\n");
		}
		else
		{
			PrintDebugProc("ループ状態 : false\n");
		}
	}
}

//======================================================================================
// モーションの設定
//======================================================================================
void SetLogoMotion(LOGOMOTIONTYPE type)
{
	g_Logo.motionType = type;
	g_Logo.nNumKey = g_Logo.aMotionInfo[g_Logo.motionType].nNumKey;
	g_Logo.nKey = 0;
	g_Logo.nCounterMotion = 0;
	g_Logo.bLoopMotion = g_Logo.aMotionInfo[g_Logo.motionType].bLoop;

	// 本体[0]の情報を設定
	g_Logo.aModel[0].nIdxModelParent = -1;
	g_Logo.aModel[0].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	g_Logo.aOffSet[0] = g_Logo.aModel[0].pos;
	g_Logo.aModel[0].rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	g_Logo.aOffSetRot[0] = g_Logo.aModel[0].rot;
}

//======================================================================================
// モーションの設定
//======================================================================================
void SetLogo(D3DXVECTOR3 pos)
{
	if (g_Logo.bDisp == false)
	{
		g_Logo.pos = pos;
		g_Logo.bDisp = true;
		SetLogoMotion(LOGOMOTIONTYPE_ACTION);
	}
}
