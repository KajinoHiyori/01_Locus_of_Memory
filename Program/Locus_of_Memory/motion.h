//=============================================================================
//
//	モーションの管理 [motion.h]
//	Author : SHUMA AIZU
// 
//=============================================================================

#ifndef _MOTION_H_
#define _MOTION_H_

#include "main.h"

//*****************************************************************************
// マクロ定義
//*****************************************************************************
#define MAX_MAT				(16)	// マテリアルの最大数
#define MAX_MODEL			(14)	// モデルの最大数
#define MAX_TEXTURE			(16)	// テクスチャの最大数
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
// モデルの構造体定義
//*****************************************************************************
typedef struct Model
{
	LPDIRECT3DTEXTURE9	apTexture[MAX_TEXTURE];		// テクスチャ
	LPD3DXMESH			pMesh;						// メッシュへのポインタ
	LPD3DXBUFFER		pBuffMat;					// マテリアルへのポインタ
	DWORD				dwNumMat;					// マテリアルの数
	int					nIdxModelParent;			// 親モデルのインデックス
	D3DXVECTOR3			pos;						// モデルの位置(オフセット)
	D3DXVECTOR3			rot;						// 向き
	D3DXMATRIX			mtxWorld;					// ワールドマトリックス
}Model;

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
	KEY aKey[MAX_MODEL];		// 各パーツごとのキー要素
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
	Model				aModel[MAX_MODEL];					// モデル(パーツ)
	D3DXVECTOR3			aOffSet[MAX_MODEL];					// モデルのオフセット[位置]を保存
	D3DXVECTOR3			aOffSetRot[MAX_MODEL];				// モデルのオフセット[角度]を保存
	MotionData*			pMotionData;						// モーションデータへのポインタ (現在のモーション)
	int					nNumMotion;							// 現在のモーションの総数
	MOTIONTYPE			motionType;							// 現在のモーションの種類
	bool				bLoopMotion;						// 現在のモーションのループを管理
	int					nNumKey;							// 現在のキーの総数
	int					nKey;								// 現在実行されているキーの番号
	int					nCounterMotion;						// 現在のモーションのカウンター
	bool				bFinishMotion;						// 現在のモーションが終了しているか
	bool				bBlendMotion;						// ブレンドモーションするかどうか
	MOTIONTYPE			motionTypeBlend;					// ブレンドモーションの種類
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
void LoadMotion(bool bLoop, int nNumKey, KEY_INFO* pKeyInfo, int nMotion, int nNumMotion);

#endif	_MOTION_H_