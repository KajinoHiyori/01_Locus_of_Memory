//=============================================================================
//
//	モーションの管理 [motion.cpp]
//	Author : SHUMA AIZU
// 
//=============================================================================

#include "motion.h"
#include "loadscript.h"

//*****************************************************************************
// グローバル変数
//*****************************************************************************
MotionData g_aMotionData[MAX_MOTIONDATA];		// モーションデータ格納

//=============================================================================
//	モーションの初期化処理
//=============================================================================
void InitMotion(void)
{
	MotionData* pMotionData = &g_aMotionData[0];

	memset(pMotionData, NULL, sizeof(MotionData) * MAX_MOTIONDATA);
}

//=============================================================================
//	モーションの設定処理
//=============================================================================
void SetMotion(Motion* pMotion, ModelData* pModelData, MOTIONTYPE motiontype, bool bLoopMotion, bool bBlendMotion, int nFrameBlend)
{
	if (bBlendMotion == true)
	{
		pMotion->bBlendMotion = true;
		pMotion->nFrameBlend = nFrameBlend;
		pMotion->nCounterBlend = 0;

		pMotion->motionTypeBlend = motiontype;
		pMotion->bLoopMotionBlend = bLoopMotion;
		pMotion->nNumKeyBlend = pMotion->pMotionData->aMotionInfo[motiontype].nNumKey;
		pMotion->nKeyBlend = 0;
		pMotion->nCounterMotionBlend = 0;
	}
	else
	{
		pMotion->nNumKey = pMotion->pMotionData->aMotionInfo[motiontype].nNumKey;
		pMotion->motionType = motiontype;
		pMotion->bLoopMotion = bLoopMotion;
		pMotion->nKey = 0;
		pMotion->nCounterMotion = 0;

		Model* pModel = &pModelData->aModel[0];
		KEY_INFO* pKeyInfo = &pMotion->pMotionData->aMotionInfo[pMotion->motionType].aKeyInfo[0];

		for (int nCntModel = 0; nCntModel < pModelData->nNumModel; nCntModel++, pModel++, pKeyInfo++)
		{// 全パーツの初期化
			pModel->pos.x = pKeyInfo->aKey[0].fPosX;
			pModel->pos.y = pKeyInfo->aKey[0].fPosY;
			pModel->pos.z = pKeyInfo->aKey[0].fPosZ;

			pModel->rot.x = pKeyInfo->aKey[0].fRotX;
			pModel->rot.y = pKeyInfo->aKey[0].fRotY;
			pModel->rot.z = pKeyInfo->aKey[0].fRotZ;

			pModel->pos += pModel->posLocal;
			pModel->rot += pModel->rotLocal;
		}
	}
}

//=============================================================================
//	モーションの更新処理
//=============================================================================
void UpdateMotion(Motion* pMotion, ModelData* pModelData)
{
	float fPosX, fPosY, fPosZ, fRotX, fRotY, fRotZ;
	float fDiffKey;

	// 全モデル(パーツ)の更新
	for (int nCntModel = 0; nCntModel < pModelData->nNumModel; nCntModel++)
	{
		KEY DiffKey = {};
		KEY RateKey = {};

		// 現在のキー
		KEY* pKey = &pMotion->pMotionData->aMotionInfo[pMotion->motionType].aKeyInfo[pMotion->nKey].aKey[nCntModel];

		// 次のキー
		KEY* pKeyNext = &pMotion->pMotionData->aMotionInfo[pMotion->motionType].aKeyInfo[(pMotion->nKey + 1) % pMotion->nNumKey].aKey[nCntModel];

		// 対象モデル
		Model* pModel = &pModelData->aModel[nCntModel];

		// 最大フレームと現在のフレームの比率
		float fRateKey = (float)pMotion->nCounterMotion / (float)pMotion->pMotionData->aMotionInfo[pMotion->motionType].aKeyInfo[pMotion->nKey].nFrame;

		if (pMotion->bBlendMotion == true)
		{// ブレンドあり

			// 現在の値格納用
			float fPosXCurrent, fRotXCurrent,	// 位置・向きX
				fPosYCurrent, fRotYCurrent, 	// 位置・向きY
				fPosZCurrent, fRotZCurrent;		// 位置・向きZ

			// ブレンド用
			float fPosXBlend, fRotXBlend,
				fPosYBlend, fRotYBlend,
				fPosZBlend, fRotZBlend;

			float fDiffKeyBlend, fDiffBlend;	// 差分

			// ブレンドモーションの現在のキー
			KEY* pKeyBlend = &pMotion->pMotionData->aMotionInfo[pMotion->motionTypeBlend].aKeyInfo[pMotion->nKeyBlend].aKey[nCntModel];

			// ブレンドモーションの次のキー
			KEY* pKeyNextBlend = &pMotion->pMotionData->aMotionInfo[pMotion->motionTypeBlend].aKeyInfo[(pMotion->nKeyBlend + 1) % pMotion->nNumKeyBlend].aKey[nCntModel];

			// ブレンドモーションの最大フレームと現在のフレームの比率
			float fRateKeyBlend = (float)pMotion->nCounterMotionBlend / (float)pMotion->pMotionData->aMotionInfo[pMotion->motionTypeBlend].aKeyInfo[pMotion->nKeyBlend].nFrame;

			// モーションブレンドの最大フレームと現在のフレームの比率
			float fRateBlend = (float)pMotion->nCounterBlend / (float)pMotion->nFrameBlend;

			// 座標差分
			// X座標
			fDiffKey = pKeyNext->fPosX - pKey->fPosX;
			fPosXCurrent = pKey->fPosX + (fDiffKey * fRateKey);

			fDiffKeyBlend = pKeyNextBlend->fPosX - pKeyBlend->fPosX;
			fPosXBlend = pKeyBlend->fPosX + (fDiffKeyBlend * fRateKeyBlend);

			fDiffBlend = fPosXBlend - fPosXCurrent;

			fPosX = fPosXCurrent + (fDiffBlend * fRateBlend);

			// Y座標
			fDiffKey = pKeyNext->fPosY - pKey->fPosY;
			fPosYCurrent = pKey->fPosY + (fDiffKey * fRateKey);

			fDiffKeyBlend = pKeyNextBlend->fPosY - pKeyBlend->fPosY;
			fPosYBlend = pKeyBlend->fPosY + (fDiffKeyBlend * fRateKeyBlend);

			fDiffBlend = fPosYBlend - fPosYCurrent;

			fPosY = fPosYCurrent + (fDiffBlend * fRateBlend);

			// Z座標
			fDiffKey = pKeyNext->fPosZ - pKey->fPosZ;
			fPosZCurrent = pKey->fPosZ + (fDiffKey * fRateKey);

			fDiffKeyBlend = pKeyNextBlend->fPosZ - pKeyBlend->fPosZ;
			fPosZBlend = pKeyBlend->fPosZ + (fDiffKeyBlend * fRateKeyBlend);

			fDiffBlend = fPosZBlend - fPosZCurrent;

			fPosZ = fPosZCurrent + (fDiffBlend * fRateBlend);

			// 向き差分
			// X軸
			fDiffKey = pKeyNext->fRotX - pKey->fRotX;
			fRotXCurrent = pKey->fRotX + (fDiffKey * fRateKey);

			fDiffKeyBlend = pKeyNextBlend->fRotX - pKeyBlend->fRotX;
			fRotXBlend = pKeyBlend->fRotX + (fDiffKeyBlend * fRateKeyBlend);

			fDiffBlend = fRotXBlend - fRotXCurrent;

			fRotX = fRotXCurrent + (fDiffBlend * fRateBlend);

			fRotX = AngleNormalize(fRotX);

			// Y軸
			fDiffKey = pKeyNext->fRotY - pKey->fRotY;
			fRotYCurrent = pKey->fRotY + (fDiffKey * fRateKey);

			fDiffKeyBlend = pKeyNextBlend->fRotY - pKeyBlend->fRotY;
			fRotYBlend = pKeyBlend->fRotY + (fDiffKeyBlend * fRateKeyBlend);

			fDiffBlend = fRotYBlend - fRotYCurrent;

			fRotY = fRotYCurrent + (fDiffBlend * fRateBlend);

			fRotY = AngleNormalize(fRotY);

			// Z軸
			fDiffKey = pKeyNext->fRotZ - pKey->fRotZ;
			fRotZCurrent = pKey->fRotZ + (fDiffKey * fRateKey);

			fDiffKeyBlend = pKeyNextBlend->fRotZ - pKeyBlend->fRotZ;
			fRotZBlend = pKeyBlend->fRotZ + (fDiffKeyBlend * fRateKeyBlend);

			fDiffBlend = fRotZBlend - fRotZCurrent;

			fRotZ = fRotZCurrent + (fDiffBlend * fRateBlend);

			fRotY = AngleNormalize(fRotY);
		}
		else
		{
			// 座標差分
			fDiffKey = pKeyNext->fPosX - pKey->fPosX;
			fPosX = pKey->fPosX + (fDiffKey * fRateKey);

			fDiffKey = pKeyNext->fPosY - pKey->fPosY;
			fPosY = pKey->fPosY + (fDiffKey * fRateKey);

			fDiffKey = pKeyNext->fPosZ - pKey->fPosZ;
			fPosZ = pKey->fPosZ + (fDiffKey * fRateKey);

			// 向き差分
			fDiffKey = pKeyNext->fRotX - pKey->fRotX;
			fRotX = AngleNormalize(pKey->fRotX + (fDiffKey * fRateKey));

			fDiffKey = pKeyNext->fRotY - pKey->fRotY;
			fRotY = AngleNormalize(pKey->fRotY + (fDiffKey * fRateKey));

			fDiffKey = pKeyNext->fRotZ - pKey->fRotZ;
			fRotZ = AngleNormalize(pKey->fRotZ + (fDiffKey * fRateKey));
		}

		// パーツの向きと位置を設定
		pModel->pos = D3DXVECTOR3(fPosX, fPosY, fPosZ);
		pModel->rot = D3DXVECTOR3(fRotX, fRotY, fRotZ);

		// オフセット位置加算
		pModel->pos += pModel->posLocal;
		pModel->rot += pModel->rotLocal;

		// 補正
		pModel->rot.x = AngleNormalize(pModel->rot.x);
		pModel->rot.y = AngleNormalize(pModel->rot.y);
		pModel->rot.z = AngleNormalize(pModel->rot.z);
	}

	if (pMotion->bBlendMotion == true)
	{// ブレンドあり
		// ブレンドモーションのカウンターを更新
		pMotion->nCounterMotionBlend++;

		if (pMotion->nCounterMotionBlend >= pMotion->pMotionData->aMotionInfo[pMotion->motionTypeBlend].aKeyInfo[pMotion->nKeyBlend].nFrame)
		{
			// キーを進める
			pMotion->nCounterMotionBlend = 0;	// カウンターリセット
			if ((pMotion->nKeyBlend = (pMotion->nKeyBlend + 1) % pMotion->nNumKeyBlend) == 0 && pMotion->bLoopMotionBlend == false)
			{// キーが終着点まで来たかつループ状態じゃなければ
				//SetMotion(MOTIONTYPE_NEUTRAL, true, true, 10);
			}
		}

		// ブレンドカウンターを更新
		pMotion->nCounterBlend++;
		if (pMotion->nCounterBlend >= pMotion->nFrameBlend)
		{// カウンターが回り切ったら
			pMotion->nCounterBlend = 0;									// カウンターリセット
			pMotion->motionType = pMotion->motionTypeBlend;				// 次のモーションに切り替える
			pMotion->nCounterMotion = pMotion->nCounterMotionBlend;		// モーションカウンターをブレンドと同期
			pMotion->nNumKey = pMotion->nNumKeyBlend;					// キーの総数をブレンドと同期
			pMotion->nKey = pMotion->nKeyBlend;							// キー番号をブレンドと同期
			pMotion->bLoopMotion = pMotion->bLoopMotionBlend;			// ループ状態をブレンドと同期
			pMotion->bBlendMotion = false;								// ブレンド状態を終了
		}
	}
	else
	{// ブレンドなし
		pMotion->nCounterMotion++;

		if (pMotion->nCounterMotion >= pMotion->pMotionData->aMotionInfo[pMotion->motionType].aKeyInfo[pMotion->nKey].nFrame)
		{
			// キーを進める
			pMotion->nCounterMotion = 0;	// カウンターリセット
			if ((pMotion->nKey = (pMotion->nKey + 1) % pMotion->nNumKey) == 0 && pMotion->bLoopMotion == false)
			{// キーが終着点まで来たかつループ状態じゃなければ
				SetMotion(pMotion, pModelData, MOTIONTYPE_NEUTRAL, true, true, 10);
			}
		}
	}
}

//=============================================================================
//	モーション読み込み処理
//=============================================================================
void LoadMotion(bool bLoop, int nNumKey, KEY_INFO* pKeyInfo, int nMotion, int nNumMotion)
{
	MOTION_INFO* pMotionInfo = &g_aMotionData[nNumMotion].aMotionInfo[nMotion];

	pMotionInfo->bLoop = bLoop;
	pMotionInfo->nNumKey = nNumKey;

	for (int nCntKeyInfo = 0; nCntKeyInfo < nNumKey; nCntKeyInfo++, pKeyInfo++)
	{
		pMotionInfo->aKeyInfo[nCntKeyInfo] = *pKeyInfo;
	}

	g_aMotionData[nNumMotion].nNumMotion++;
}

//=============================================================================
//	モーションデータの設定処理
//=============================================================================
MotionData* SetMotionData(MOTIONDATATYPE type)
{
	return &g_aMotionData[type];
}