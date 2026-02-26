//==========================================
// 
// 2Dアクション[particle.h]
// 
//==========================================
#if 0
#ifndef _PARTICLE_H_
#define	_PARTICLE_H_
#include "main.h"

// マクロ定義
#define PARTICLE_TIME	(20)		// パーティクル継続時間
#define MAX_PARTICLE	(512)		// パーティクルの最大数
#define LIFE_EAT		(20)		// パーティクル継続時間 [PARTICLETYPE_EAT]
#define LIFE_BLOCK		(20)		// パーティクル継続時間 [PARTICLETYPE_BLOCK]

// パーティクルの種類
typedef enum
{
	PARTICLETYPE_NONE = 0,		// 何もしていない状態
	PARTICLETYPE_EAT,			// 食事パーティクル
	PARTICLETYPE_BLOCK,			// ブロック消滅パーティクル
	PARTICLETYPE_DEATH,			// プレイヤー死亡パーティクル
	PARTICLETYPE_ITEMAPPEAR,	// アイテムの出現
	PARTICLETYPE_JUMP,			// ジャンプパーティクル
	PARTICLETYPE_POPPER_L,		// ゴールクリアクラッカー左
	PARTICLETYPE_POPPER_R,		// ゴールクリアクラッカー右
	PARTICLETYPE_MAX
}PARTICLETYPE;

// パーティクル構造体
typedef struct
{
	PARTICLETYPE type;	// 種類
	D3DXVECTOR3	pos;	// 発生位置
	D3DXCOLOR col;		// 色の設定
	int nLife;			// 発生時間
	bool bUse;			// 使用されているかどうか
}Particle;

// プロトタイプ宣言
void InitParticle(void);
void UninitParticle(void);
void UpdateParticle(void);
void DrawParticle(void);
void SetParticle(PARTICLETYPE type, D3DXVECTOR3 pos, D3DXCOLOR col, int nLife);
Particle *GetParticle(void);

#endif
#endif

