//==========================================
// 
// player処理[player.h]
// Author : Kajino Hiyori
// 
//==========================================
#ifndef _PLAYER_H_
#define	_PLAYER_H_

#include "motion.h"

// マクロ定義
#define MAX_PLAYERMAT		(16)	// マテリアルの最大数
#define MAX_PLAYER			(2)		// プレイヤーの最大数
#define MAX_PLAYERMODEL		(14)	// モデルの最大数
#define MAX_PLAYERTEXTURE	(16)	// テクスチャの最大数
#define MAX_KEY				(16)	// キーの最大数

//// モデルの構造体
//typedef struct
//{
//	LPDIRECT3DTEXTURE9	apTexture[MAX_PLAYERTEXTURE];	// テクスチャ
//	LPD3DXMESH			pMesh;
//	LPD3DXBUFFER		pBuffMat;
//	DWORD				dwNumMat;
//	int					nIdxModelParent;	// 親モデルのインデックス
//	D3DXVECTOR3			pos;				// モデルの位置(オフセット)
//	D3DXVECTOR3			rot;				// 向き
//	D3DXMATRIX			mtxWorld;			// ワールドマトリックス
//}PlayerModel;

// モーションの種類
typedef enum
{
	PLAYERMOTIONTYPE_NEUTRAL = 0,	// 待機モーション[0]
	PLAYERMOTIONTYPE_MOVE,			// 移動モーション[1]
	PLAYERMOTIONTYPE_ACTION,		// アクションモーション[2]
	PLAYERMOTIONTYPE_JUMP,			// ジャンプモーション[3]
	PLAYERMOTIONTYPE_LANDING,		// 着地モーション[4]
	PLAYERMOTIONTYPE_MAX
}PLAYERMOTIONTYPE;

//// キー要素の構造体
//typedef struct
//{
//	float fPosX;	// 位置X...親からのオフセット
//	float fPosY;	// 位置Y...親からのオフセット
//	float fPosZ;	// 位置Z...親からのオフセット
//	float fRotX;	// 向きX
//	float fRotY;	// 向きY
//	float fRotZ;	// 向きZ
//}KEY;
//
//// キー情報の構造体
//typedef struct
//{
//	int nFrame;					// 再生フレーム(何フレームかけてモーションが遷移していくのか)
//	KEY aKey[MAX_PLAYERMODEL];	// 各パーツごとのキー要素
//}PLAYERKEY_INFO;
//
//// モーション除法の構造体
//typedef struct
//{
//	bool bLoop;							// ループするかどうか[0...ループなし / 1...ループあり]
//	int nNumKey;						// キーの総数
//	PLAYERKEY_INFO aKeyInfo[MAX_KEY];	// キー情報
//}PLAYERMOTION_INFO;

//// プレイヤーのモーション情報
//typedef struct
//{
//	PlayerModel			aModel[MAX_PLAYERMODEL];			// モデル(パーツ)
//	D3DXVECTOR3			aOffSet[MAX_PLAYERMODEL];			// モデルのオフセット[位置]を保存
//	D3DXVECTOR3			aOffSetRot[MAX_PLAYERMODEL];		// モデルのオフセット[角度]を保存
//	PLAYERMOTION_INFO	aMotionInfo[PLAYERMOTIONTYPE_MAX];	// 現在のモーション情報
//	int					nNumMotion;							// 現在のモーションの総数
//	PLAYERMOTIONTYPE	motionType;							// 現在のモーションの種類
//	bool				bLoopMotion;						// 現在のモーションのループを管理
//	int					nNumKey;							// 現在のキーの総数
//	int					nKey;								// 現在実行されているキーの番号
//	int					nCounterMotion;						// 現在のモーションのカウンター
//	bool				bFinishMotion;						// 現在のモーションが終了しているか
//	bool				bBlendMotion;						// ブレンドモーションするかどうか
//	PLAYERMOTIONTYPE	motionTypeBlend;					// ブレンドモーションの種類
//	bool				bLoopMotionBlend;					// ブレンドモーションのループを管理
//	int					nNumKeyBlend;						// ブレンドモーションのキーの総数
//	int					nKeyBlend;							// ブレンドモーションの実行されているキーの番号
//	int					nCounterMotionBlend;				// ブレンドモーションのカウンター
//	int					nFrameBlend;						// ブレンドフレーム数(何フレームかけてブレンドをするか)
//	int					nCounterBlend;						// ブレンドカウンター(ブレンドフレームが何フレーム処理されているか)
//}PlayerMotion;

// モデルの構造体
typedef struct
{
	D3DXMATRIX		mtxWorld;	// ワールドマトリックス
	D3DXVECTOR3		pos;		// プレイヤーの位置
	D3DXVECTOR3		posOld;		// プレイヤーの過去の位置
	D3DXVECTOR3		move;		// プレイヤーの移動量
	D3DXVECTOR3		rot;		// プレイヤーの向き
	D3DXVECTOR3		rotDest;	// プレイヤーの目的の向き
	Motion			motion;		// プレイヤーのモーション情報
	ModelData*		pModelData;	// モデルの情報
	int				nIdxShadow;	// 影のインデックス
	float			fSpeed;		// 移動量
	bool			bJump;		// ジャンプ状態
	bool			bUse;		// 使用状態
}Player;

// プロトタイプ宣言
void InitPlayer(void);
void UninitPlayer(void);
void UpdatePlayer(void);
void DrawPlayer(void);
Player* GetPlayer(void);
void SetPlayer(int nIdx, D3DXVECTOR3 pos, D3DXVECTOR3 rot);
#endif
