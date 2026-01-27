//==========================================
// 
// player処理[player.h]
// Author : Kajino Hiyori
// 
//==========================================
#ifndef _PLAYER_H_
#define	_PLAYER_H_

#include "motion.h"
#include "object.h"
#include "magic.h"

// マクロ定義
#define MAX_PLAYERMAT		(16)	// マテリアルの最大数
#define MAX_PLAYER			(2)		// プレイヤーの最大数
#define MAX_PLAYERMODEL		(14)	// モデルの最大数
#define MAX_PLAYERTEXTURE	(16)	// テクスチャの最大数
#define MAX_KEY				(16)	// キーの最大数

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

// 魔導書に記録されている魔法の種類
typedef struct
{
	COMMANDOREDER command0;
	COMMANDOREDER command1;
	COMMANDOREDER command2;
	COMMANDOREDER command3;
}MagicBook;

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
	float			fRadius;	// 半径
	float			fSpeed;		// 移動量
	bool			bJump;		// ジャンプ状態
	MagicBook		magicbook;	// 魔法の種類
	bool			bUse;		// 使用状態
}Player;

// プロトタイプ宣言
void InitPlayer(void);
void UninitPlayer(void);
void UpdatePlayer(void);
void DrawPlayer(void);
Player* GetPlayer(void);
void SetPlayer(int nIdx, D3DXVECTOR3 pos, D3DXVECTOR3 rot, PARENTMODELTYPE parentmodeltype);
#endif
