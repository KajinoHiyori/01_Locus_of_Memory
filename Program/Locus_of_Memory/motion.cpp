//=============================================================================
//
//	モーションの管理 [motion.cpp]
//	Author : SHUMA AIZU
// 
//=============================================================================

#include "motion.h"

//*****************************************************************************
// グローバル変数
//*****************************************************************************
MotionData g_aMotionData[MAX_MOTIONDATA];		// モーションデータ格納

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