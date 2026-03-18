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
#define MAX_OWNCOMMAND		(4)		// 所有できるコマンドの最大数
#define BLENDFRAME			(10)	// モーションブレンドのフレーム数

// モーションの種類
typedef enum
{
	PLAYERMOTIONTYPE_NEUTRAL = 0,	// 待機モーション[0]
	PLAYERMOTIONTYPE_MOVE,			// 移動モーション[1]
	PLAYERMOTIONTYPE_RUNNING,		// 移動(速)モーション[2]
	PLAYERMOTIONTYPE_STOPACTION,	// 静止魔法モーション[3]
	PLAYERMOTIONTYPE_TOSKYACTION,	// 空に向けて魔法モーション[4]
	PLAYERMOTIONTYPE_CROUCHING,		// しゃがみ魔法モーション[5]
	PLAYERMOTIONTYPE_FLOATONG,		// 浮遊モーション[6]
	PLAYERMOTIONTYPE_COMMAND,		// コマンド入力モーション[7]
	PLAYERMOTIONTYPE_JUMP,			// ジャンプモーション[8]
	PLAYERMOTIONTYPE_LANDING,		// 着地モーション[9]
	PLAYERMOTIONTYPE_FAILD,			// 魔法失敗モーション[10]
	PLAYERMOTIONTYPE_GETMAGIC,		// 魔法取得モーション[11]
	PLAYERMOTIONTYPE_MAX
}PLAYERMOTIONTYPE;

// プレイヤーの状態
typedef enum 
{
	PLAYERSTATE_NORMAL = 0,	// フィールド上
	PLAYERSTATE_PAUSE,		// ポーズ中
	PLAYERSTATE_SPELL,		// 呪文入力中
	PLAYERSTATE_MAGIC,		// 魔法発動中
	PLAYERSTATE_MAX
}PLAYERSTATE;

// 魔導書に記録されている魔法の種類
typedef struct
{
	COMMANDOREDER OwnCommand[MAX_OWNCOMMAND];		// 所有コマンド
	int nCntOwn;									// 何個持っているか
}MagicBook;

// モデルの構造体
typedef struct
{
	D3DXMATRIX		mtxWorld;		// ワールドマトリックス
	D3DXVECTOR3		pos;			// プレイヤーの位置
	D3DXVECTOR3		posOld;			// プレイヤーの過去の位置
	D3DXVECTOR3		move;			// プレイヤーの移動量
	D3DXVECTOR3		rot;			// プレイヤーの向き
	D3DXVECTOR3		rotDest;		// プレイヤーの目的の向き
	Motion			motion;			// プレイヤーのモーション情報
	ModelData*		pModelData;		// モデルの情報
	OffSetData		OffSetData;		// モーション中の階層構造情報
	PLAYERSTATE		state;			// プレイヤーの状態
	MagicBook		magicbook;		// 魔法の種類
	int				nIdxShadow;		// 影のインデックス
	int				nIdxCollision;	// 当たり判定のインデックス
	float			fRadius;		// 半径
	float			fSpeed;			// 移動量
	bool			bJump;			// ジャンプ状態
	bool			bKey1;			// 鍵の取得状態
	bool			bKey2;			// 鍵の取得状態
	bool			bKey3;			// 鍵の取得状態
	bool			bUse;			// 使用状態
}Player;

// プロトタイプ宣言
void InitPlayer(void);
void UninitPlayer(void);
void UpdatePlayer(void);
void DrawPlayer(void);
Player* GetPlayer(void);
void SetPlayer(int nIdx, D3DXVECTOR3 pos, D3DXVECTOR3 rot, PARENTMODELTYPE parentmodeltype);
void OwnCommand(MagicBook* pMagicBook, int nDropMagicIdx);										// コマンドを取得する
void RangeRestriction(D3DXVECTOR3* pPos, float fRadius);
#endif
