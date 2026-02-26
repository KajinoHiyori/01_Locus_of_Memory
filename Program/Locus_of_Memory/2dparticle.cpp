//========================================================================
// 
// 2Dアクション[particle.cpp]
// 
//========================================================================
#include "particle.h"
#include "effect.h"
#include "main.h"
#include "color.h"
#if 0
// マクロ定義
#define MOVE_PARTICLE	(10)		// パーティクル移動量のランダム数
#define MOVE_ADJUST		(1.25f)		// 移動量の値を指定
#define MOVE_MIN		(1.0f)		// 最低限の移動量
#define ANGLE_DOUBLE	(629)		// 角度の最大数(*2)
#define MAX_ANGLE		(314)		// 角度の最大数
#define ANGLE_ADJUST	(100.0f)	// 角度の値を指定

#define MAX_EAT			(4)		// 一度に発生するパーティクル数[PARTICLETYPE_EAT]
#define GRAIN_EAT		(15)	// 粒子の継続時間[PARTICLETYPE_EAT]
#define SIZE_EAT		(4.0f)	// 粒子の大きさ[PARTICLETYPE_EAT]

#define MAX_BLOCK		(7)		// 一度に発生するパーティクル数[PARTICLETYPE_BLOCK]
#define GRAIN_BLOCK		(20)	// 粒子の継続時間[PARTICLETYPE_BLOCK]
#define SIZE_BLOCK		(5.0f)	// 粒子の大きさ[PARTICLETYPE_BLOCK]

#define MAX_DEATH		(10)	// 一度に発生するパーティクル数[PARTICLETYPE_DEATH]
#define GRAIN_DEATH		(40)	// 粒子の継続時間[PARTICLETYPE_DEATH]
#define SIZE_DEATH		(5.0f)	// 粒子の大きさ[PARTICLETYPE_DEATH]

#define MAX_ITEM		(5)		// 一度に発生するパーティクル数[PARTICLETYPE_ITEM]
#define GRAIN_ITEM		(20)	// 粒子の継続時間[PARTICLETYPE_ITEM]
#define SIZE_ITEM		(3.0f)	// 粒子の大きさ[PARTICLETYPE_ITEM]

#define MAX_JUMP		(5)		// 一度に発生するパーティクル数[PARTICLETYPE_JUMP]
#define GRAIN_JUMP		(30)	// 粒子の継続時間[PARTICLETYPE_JUMP]
#define SIZE_JUMP		(2.0f)	// 粒子の大きさ[PARTICLETYPE_JUMP]
#define JUMP_ANGLE		(157)	// ジャンプ時の角度の制限

#define MAX_POPPER		(10)	// 一度に発生するパーティクル数[PARTICLETYPE_POPPER]
#define GRAIN_POPPER	(30)	// 粒子の継続時間[PARTICLETYPE_POPPER]
#define SIZE_POPPER		(2.0f)	// 粒子の大きさ[PARTICLETYPE_POPPER]
#define MAXANGLE_POP	(100)	// 最大角度
#define MINANGLE_POPL	(214)	// 最小角度
#define MINANGLE_POPR	(414)	// 最小角度


// グローバル変数
Particle g_aParticle[MAX_PARTICLE];	// パーティクルの情報

//========================================================================
// パーティクルの初期化処理
//========================================================================
void InitParticle(void)
{
	// 情報の初期化
	for (int nCntParticle = RESET_DATA; nCntParticle < MAX_PARTICLE; nCntParticle++)
	{
		g_aParticle[nCntParticle].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);	// 位置情報の初期化
		g_aParticle[nCntParticle].col = COLOR_WHITE;					// 色の初期化
		g_aParticle[nCntParticle].nLife = RESET_DATA;					// 発生時間の初期化
		g_aParticle[nCntParticle].bUse = false;							// 使用状態の初期化
	}
}

//========================================================================
// パーティクルの終了処理
//========================================================================
void UninitParticle(void)
{

}

//========================================================================
// パーティクルの更新処理
//========================================================================
void UpdateParticle(void)
{
	D3DXVECTOR3 pos;	// 粒子の位置情報
	D3DXVECTOR3 move;	// 粒子の移動量
	D3DXCOLOR col;		// 色の設定
	float fRadius;		// 半径の設定
	int nLife;			// 粒子の寿命

	for (int nCntParticle = RESET_DATA; nCntParticle < MAX_PARTICLE; nCntParticle++)
	{
		if (g_aParticle[nCntParticle].bUse == true)
		{ // パーティクルが使用されている場合
			switch (g_aParticle[nCntParticle].type)
			{
			case PARTICLETYPE_EAT:	// 食事パーティクル
				for (int nCntAppear = RESET_DATA; nCntAppear < MAX_EAT; nCntAppear++)
				{
					move.x	= sinf((float)(rand() % ANGLE_DOUBLE - MAX_ANGLE) / ANGLE_ADJUST) * (float)(rand() % MOVE_PARTICLE) / MOVE_ADJUST + MOVE_MIN;	// 移動量X
					move.y	= cosf((float)(rand() % ANGLE_DOUBLE - MAX_ANGLE) / ANGLE_ADJUST) * (float)(rand() % MOVE_PARTICLE) / MOVE_ADJUST + MOVE_MIN;	// 移動量Y
					move.z	= 0.0f;
					col		= g_aParticle[nCntParticle].col;
					fRadius = SIZE_EAT;
					nLife	= GRAIN_EAT;
					pos		= g_aParticle[nCntParticle].pos;	// 発生位置
					SetEffect(EFFECTTYPE_NORMAL, pos, move, EFFECT_NOT_MOVE, col, fRadius, nLife);
				}
				break;

			case PARTICLETYPE_BLOCK:	// ブロック消滅パーティクル
				for (int nCntAppear = RESET_DATA; nCntAppear < MAX_BLOCK; nCntAppear++)
				{
					move.x = sinf((float)(rand() % ANGLE_DOUBLE - MAX_ANGLE) / ANGLE_ADJUST) * (float)(rand() % MOVE_PARTICLE) / MOVE_ADJUST + MOVE_MIN;	// 移動量X
					move.y = cosf((float)(rand() % ANGLE_DOUBLE - MAX_ANGLE) / ANGLE_ADJUST) * (float)(rand() % MOVE_PARTICLE) / MOVE_ADJUST + MOVE_MIN;	// 移動量Y
					move.z = 0.0f;
					col = g_aParticle[nCntParticle].col;
					fRadius = SIZE_BLOCK;
					nLife = GRAIN_BLOCK;
					pos = g_aParticle[nCntParticle].pos;	// 発生位置
					SetEffect(EFFECTTYPE_NORMAL, pos, move, EFFECT_NOT_MOVE, col, fRadius, nLife);
				}
				break;

			case PARTICLETYPE_DEATH:	// プレイヤー死亡パーティクル
				for (int nCntAppear = RESET_DATA; nCntAppear < MAX_DEATH; nCntAppear++)
				{
					move.x = sinf((float)(rand() % ANGLE_DOUBLE - MAX_ANGLE) / ANGLE_ADJUST) * (float)(rand() % MOVE_PARTICLE) / MOVE_ADJUST + MOVE_MIN;	// 移動量X
					move.y = cosf((float)(rand() % ANGLE_DOUBLE - MAX_ANGLE) / ANGLE_ADJUST) * (float)(rand() % MOVE_PARTICLE) / MOVE_ADJUST + MOVE_MIN;	// 移動量Y
					move.z = 0.0f;
					col = g_aParticle[nCntParticle].col;
					fRadius = SIZE_DEATH;
					nLife = GRAIN_DEATH;
					pos = g_aParticle[nCntParticle].pos;	// 発生位置
					SetEffect(EFFECTTYPE_NORMAL, pos, move, EFFECT_NOT_MOVE, col, fRadius, nLife);
				}
				break;

			case PARTICLETYPE_ITEMAPPEAR:	// アイテム出現
				for (int nCntAppear = RESET_DATA; nCntAppear < MAX_ITEM; nCntAppear++)
				{
					move.x = sinf((float)(rand() % ANGLE_DOUBLE - MAX_ANGLE) / ANGLE_ADJUST) * (float)(rand() % MOVE_PARTICLE) / MOVE_ADJUST + MOVE_MIN;	// 移動量X
					move.y = cosf((float)(rand() % ANGLE_DOUBLE - MAX_ANGLE) / ANGLE_ADJUST) * (float)(rand() % MOVE_PARTICLE) / MOVE_ADJUST + MOVE_MIN;	// 移動量Y
					move.z = 0.0f;
					col = g_aParticle[nCntParticle].col;
					fRadius = SIZE_ITEM;
					nLife = GRAIN_ITEM;
					pos = g_aParticle[nCntParticle].pos;	// 発生位置
					SetEffect(EFFECTTYPE_NORMAL, pos, move, EFFECT_NOT_MOVE, col, fRadius, nLife);
				}
				break;

			case PARTICLETYPE_JUMP:	// ジャンプ
				for (int nCntAppear = RESET_DATA; nCntAppear < MAX_JUMP; nCntAppear++)
				{
					move.x = sinf((float)(rand() % MAX_ANGLE - JUMP_ANGLE) / ANGLE_ADJUST) * (float)(rand() % MOVE_PARTICLE) / MOVE_ADJUST + MOVE_MIN;	// 移動量X
					move.y = cosf((float)(rand() % MAX_ANGLE - JUMP_ANGLE) / ANGLE_ADJUST) * (float)(rand() % MOVE_PARTICLE) / MOVE_ADJUST + MOVE_MIN;	// 移動量Y
					move.z = 0.0f;
					col = g_aParticle[nCntParticle].col;
					fRadius = SIZE_JUMP;
					nLife = GRAIN_JUMP;
					pos = g_aParticle[nCntParticle].pos;	// 発生位置
					SetEffect(EFFECTTYPE_NORMAL, pos, move, EFFECT_NOT_MOVE, col, fRadius, nLife);
				}
				break;

			case 	PARTICLETYPE_POPPER_L:		// ゴールクリアクラッカー左
				for (int nCntAppear = RESET_DATA; nCntAppear < MAX_POPPER; nCntAppear++)
				{
					move.x = sinf((float)(rand() % MAXANGLE_POP + MINANGLE_POPL) / ANGLE_ADJUST) * (float)(rand() % MOVE_PARTICLE) / MOVE_ADJUST + MOVE_MIN;	// 移動量X
					move.y = cosf((float)(rand() % MAXANGLE_POP + MINANGLE_POPL) / ANGLE_ADJUST) * (float)(rand() % MOVE_PARTICLE) / MOVE_ADJUST + MOVE_MIN;	// 移動量Y
					move.z = 0.0f;
					col = g_aParticle[nCntParticle].col;
					fRadius = SIZE_POPPER;
					nLife = GRAIN_POPPER;
					pos = g_aParticle[nCntParticle].pos;	// 発生位置
					SetEffect(EFFECTTYPE_NORMAL, pos, move, EFFECT_NOT_MOVE, col, fRadius, nLife);
				}
				break;

			case 	PARTICLETYPE_POPPER_R:		// ゴールクリアクラッカー右
				for (int nCntAppear = RESET_DATA; nCntAppear < MAX_POPPER; nCntAppear++)
				{
					move.x = sinf((float)(rand() % MAXANGLE_POP - MINANGLE_POPR) / -ANGLE_ADJUST) * (float)(rand() % MOVE_PARTICLE) / MOVE_ADJUST + MOVE_MIN;	// 移動量X
					move.y = cosf((float)(rand() % MAXANGLE_POP - MINANGLE_POPR) / -ANGLE_ADJUST) * (float)(rand() % MOVE_PARTICLE) / MOVE_ADJUST + MOVE_MIN;	// 移動量Y
					move.z = 0.0f;
					col = g_aParticle[nCntParticle].col;
					fRadius = SIZE_POPPER;
					nLife = GRAIN_POPPER;
					pos = g_aParticle[nCntParticle].pos;	// 発生位置
					SetEffect(EFFECTTYPE_NORMAL, pos, move, EFFECT_NOT_MOVE, col, fRadius, nLife);
				}
				break;
			}
			
			g_aParticle[nCntParticle].nLife--;
		}
		if (g_aParticle[nCntParticle].nLife <= RESET_DATA)
		{
			g_aParticle[nCntParticle].bUse = false;
		}
	}
}

//========================================================================
// パーティクルの描画処理
//========================================================================
void DrawParticle(void)
{

}

//========================================================================
// パーティクルの設定
//========================================================================
void SetParticle(PARTICLETYPE type, D3DXVECTOR3 pos, D3DXCOLOR col, int nLife)
{
	for (int nCntParticle = RESET_DATA; nCntParticle < MAX_PARTICLE; nCntParticle++)
	{
		if (g_aParticle[nCntParticle].bUse == false)
		{
			g_aParticle[nCntParticle].type = type;
			g_aParticle[nCntParticle].pos = pos;
			g_aParticle[nCntParticle].col = col;
			g_aParticle[nCntParticle].nLife = nLife;
			g_aParticle[nCntParticle].bUse = true;
			break;
		}
	}
}

//========================================================================
// パーティクルの情報を取得
//========================================================================
Particle *GetParticle(void)
{
	return &g_aParticle[RESET_DATA];
}

#endif