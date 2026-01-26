//=============================================================================
//
//	モーションの管理 [motion.h]
//	Author : SHUMA AIZU
// 
//=============================================================================

#ifndef _MOTION_H_
#define _MOTION_H_

#include "main.h"
#include "object.h"

//*****************************************************************************
// マクロ定義
//*****************************************************************************
#define MAX_KEY				(16)	// キーの最大数
#define MAX_MOTION			(16)	// モーションの最大数
#define MAX_MOTIONDATA		(8)		// モーションデータの最大数

//*****************************************************************************
// モーションの種類
//*****************************************************************************
typedef enum MOTIONTYPE
{
	MOTIONTYPE_NEUTRAL = 0,	// 待機モーション[0]
	MOTIONTYPE_MOVE,		// 移動モーション[1]
	MOTIONTYPE_ACTION,		// アクションモーション[2]
	MOTIONTYPE_JUMP,		// ジャンプモーション[3]
	MOTIONTYPE_LANDING,		// 着地モーション[4]
	MOTIONTYPE_MAX
}MOTIONTYPE;

//*****************************************************************************
// モーションデータの種類
//*****************************************************************************
typedef enum MOTIONDATATYPE
{
	MOTIONDATATYPE_PLAYER = 0,		// プレイヤーのモーションデータ
	MOTIONDATATYPE_MAX
}MOTIONDATATYPE;

//*****************************************************************************
// キーの要素の構造体定義
//*****************************************************************************
typedef struct KEY
{
	float fPosX;	// 位置X...親からのオフセット
	float fPosY;	// 位置Y...親からのオフセット
	float fPosZ;	// 位置Z...親からのオフセット
	float fRotX;	// 向きX
	float fRotY;	// 向きY
	float fRotZ;	// 向きZ
}KEY;

//*****************************************************************************
// キー情報の構造体定義
//*****************************************************************************
typedef struct KEY_INFO
{
	int nFrame;					// 再生フレーム(何フレームかけてモーションが遷移していくのか)
	KEY aKey[MAX_PARTS];		// 各パーツごとのキー要素
}KEY_INFO;

//*****************************************************************************
// モーション情報の構造体定義
//*****************************************************************************
typedef struct MOTION_INFO
{
	bool bLoop;							// ループするかどうか[0...ループなし / 1...ループあり]
	int nNumKey;						// キーの総数
	KEY_INFO aKeyInfo[MAX_KEY];			// キー情報
}MOTION_INFO;

//*****************************************************************************
// モーションデータの構造体定義
//*****************************************************************************
typedef struct MotionData
{
	MOTION_INFO aMotionInfo[MAX_MOTION];		// モーション情報
	int			nNumMotion;						// モーションの総数
}MotionData;

//*****************************************************************************
// モーションの構造体定義
//*****************************************************************************
typedef struct Motion
{
	MotionData*			pMotionData;						// モーションデータへのポインタ (現在のモーション)
	MOTIONTYPE			motionType;							// 現在のモーションの種類
	bool				bLoopMotion;						// 現在のモーションのループを管理
	int					nNumKey;							// 現在のキーの総数
	int					nKey;								// 現在実行されているキーの番号
	int					nCounterMotion;						// 現在のモーションのカウンター
	bool				bFinishMotion;						// 現在のモーションが終了しているか
	MOTIONTYPE			motionTypeBlend;					// ブレンドモーションの種類
	bool				bBlendMotion;						// ブレンドモーションするかどうか
	bool				bLoopMotionBlend;					// ブレンドモーションのループを管理
	int					nNumKeyBlend;						// ブレンドモーションのキーの総数
	int					nKeyBlend;							// ブレンドモーションの実行されているキーの番号
	int					nCounterMotionBlend;				// ブレンドモーションのカウンター
	int					nFrameBlend;						// ブレンドフレーム数(何フレームかけてブレンドをするか)
	int					nCounterBlend;						// ブレンドカウンター(ブレンドフレームが何フレーム処理されているか)
}Motion;

//*****************************************************************************
// プロトタイプ宣言
//*****************************************************************************
void InitMotion(void);
void SetMotion(Motion* pMotion, ModelData* pModelData, MOTIONTYPE motiontype, bool bLoopMotion, bool bBlendMotion, int nFrameBlend);
void UpdateMotion(Motion* motion, ModelData* model);
void LoadMotion(bool bLoop, int nNumKey, KEY_INFO* pKeyInfo, int nMotion, int nNumMotion);
MotionData* SetMotionData(MOTIONDATATYPE type);

#endif	_MOTION_H_