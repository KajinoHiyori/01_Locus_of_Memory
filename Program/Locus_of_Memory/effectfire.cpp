//========================================================================
// 
// パーティクル描画[effectfire.cpp]
// Author : KAIRI MANABE
//
//========================================================================

#include "main.h"
#include "player.h"
#include "effectfire.h"
#include "effect.h"
#include "input.h"
#include "debugproc.h"
#include "color.h"

#define MAX_MAGIC_PARTICLE	(10)		//パーティクルの最大数
#define MAX_MAGIC_APPEAR	(5)			//粒子の最大数
#define MAX_ANGRE			(629)
#define MAX_ANGRE2			(314)

//パーティクルの構造体
typedef struct
{
	D3DXVECTOR3 pos;					//位置
	D3DXVECTOR3 move;					//移動量
	D3DXCOLOR col;						//色
	D3DXMATRIX g_mtxWorldEffectFire;	//ワールドマトリックス
	float fRadius;						//半径
	FIRE_TYPE Type;						//種類
	bool bUse;							//使用しているか
}PARTICLE;

//グローバル変数
LPDIRECT3DTEXTURE9 g_pTextureBuffEffectFire = NULL;
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffEffectFire = NULL;
PARTICLE g_aEffectFire[MAX_MAGIC_PARTICLE];

//========================================================================
// 初期化
//========================================================================
void InitEffectFire(void)
{
	for (int nCntEffectFire = 0; nCntEffectFire < MAX_MAGIC_PARTICLE; nCntEffectFire++)
	{
			g_aEffectFire[nCntEffectFire].bUse = false;
			g_aEffectFire[nCntEffectFire].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
			g_aEffectFire[nCntEffectFire].move = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
			g_aEffectFire[nCntEffectFire].fRadius = 0;
			g_aEffectFire[nCntEffectFire].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
			g_aEffectFire[nCntEffectFire].Type = FIRE_TYPE_NONE;
	}
}

//========================================================================
// 終了
//========================================================================
void UninitEffectFire(void)
{

}

//========================================================================
// 更新
//========================================================================
void UpdateEffectFire(void)
{
	D3DXVECTOR3 pos[3];
	D3DXVECTOR3 move[3][FIRE_TYPE_MAX];		//パーティクルのmove
	D3DXVECTOR3 rot;							//向き
	float fSpeed;								//速度調整
	float fAngle = 0.0f;
	float fRadius = 0.0f;
	int nCountEffectFire = 0;

	for (int nCntMovetype = 0; nCntMovetype < 3; nCntMovetype++)
	{
		pos[nCntMovetype] = D3DXVECTOR3(0.0f, 0.0f, 0.0f);

		for (int nCntMove = 0; nCntMove < FIRE_TYPE_MAX; nCntMove++)
		{
			move[nCntMovetype][nCntMove] = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		}
	}

	rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);

#ifdef _DEBUG
	if (GetKeyboardTrigger(DIK_V) == true)
	{
		SetEffectFire(D3DXVECTOR3(0.0f, 0.0f, 0.0f), FIRE_TYPE_FORGE, 1);
	}
#endif

	for (int nCntEffectFire = 0; nCntEffectFire < MAX_MAGIC_PARTICLE; nCntEffectFire++)
	{
		if (g_aEffectFire[nCntEffectFire].bUse == true)
		{//使用されていたら
			switch (g_aEffectFire[nCntEffectFire].Type)
			{
			case FIRE_TYPE_FORGE:
				//位置の設定
				pos[0].x = g_aEffectFire[nCntEffectFire].pos.x + (sinf((float)(rand() % 629 - 314 / 100.0f))) * (float)(rand() % 25 + 1);
				pos[0].y = g_aEffectFire[nCntEffectFire].pos.y + (cosf((float)(rand() % 629 - 314 / 100.0f))) * (float)(rand() % 17 + 1) + 30;
				pos[0].z = g_aEffectFire[nCntEffectFire].pos.z + (cosf((float)(rand() % 629 - 314 / 100.0f))) * (float)(rand() % 25 + 1);

				//位置の設定
				pos[1].x = g_aEffectFire[nCntEffectFire].pos.x + (sinf((float)(rand() % 629 - 314 / 100.0f))) * (float)(rand() % 15 + 1);
				pos[1].y = g_aEffectFire[nCntEffectFire].pos.y + (cosf((float)(rand() % 629 - 314 / 100.0f))) * (float)(rand() % 8 + 1) + 15;
				pos[1].z = g_aEffectFire[nCntEffectFire].pos.z + (cosf((float)(rand() % 629 - 314 / 100.0f))) * (float)(rand() % 15 + 1);

				SetEffect(EFFECT_TYPE_FIRE, EFFECT_TEX_DIAMOND, pos[0], move[0][FIRE_TYPE_FORGE], COLOR_RED, 100, 18);
				SetEffect(EFFECT_TYPE_FIRE, EFFECT_TEX_CIRCLE, pos[1], move[0][FIRE_TYPE_FORGE], COLOR_ORANGE, 100, 10);
				break;
			case FIRE_TYPE_BONFIRE:
				//位置の設定
				pos[0].x = g_aEffectFire[nCntEffectFire].pos.x + (sinf((float)(rand() % 629 - 314 / 100.0f))) * (float)(rand() % 30 + 1);
				pos[0].y = g_aEffectFire[nCntEffectFire].pos.y + (cosf((float)(rand() % 629 - 314 / 100.0f))) * (float)(rand() % 30 + 1) + 30;
				pos[0].z = g_aEffectFire[nCntEffectFire].pos.z + (cosf((float)(rand() % 629 - 314 / 100.0f))) * (float)(rand() % 30 + 1);

				//位置の設定
				pos[1].x = g_aEffectFire[nCntEffectFire].pos.x + (sinf((float)(rand() % 629 - 314 / 100.0f))) * (float)(rand() % 15 + 1);
				pos[1].y = g_aEffectFire[nCntEffectFire].pos.y + (cosf((float)(rand() % 629 - 314 / 100.0f))) * (float)(rand() % 15 + 1) + 15;
				pos[1].z = g_aEffectFire[nCntEffectFire].pos.z + (cosf((float)(rand() % 629 - 314 / 100.0f))) * (float)(rand() % 15 + 1);

				SetEffect(EFFECT_TYPE_FIRE, EFFECT_TEX_DIAMOND, pos[0], move[0][FIRE_TYPE_BONFIRE], COLOR_RED, 100, 18);
				SetEffect(EFFECT_TYPE_FIRE, EFFECT_TEX_CIRCLE, pos[1], move[0][FIRE_TYPE_BONFIRE], COLOR_ORANGE, 100, 10);
				break;
			case FIRE_TYPE_SMOKE:
				//位置の設定
				pos[0].x = g_aEffectFire[nCntEffectFire].pos.x + (sinf((float)(rand() % 629 - 314 / 100.0f))) * (float)(rand() % 20 + 1);
				pos[0].y = g_aEffectFire[nCntEffectFire].pos.y + (cosf((float)(rand() % 629 - 314 / 100.0f))) * (float)(rand() % 50 + 1) + 50;
				pos[0].z = g_aEffectFire[nCntEffectFire].pos.z + (cosf((float)(rand() % 629 - 314 / 100.0f))) * (float)(rand() % 20 + 1);

				SetEffect(EFFECT_TYPE_SMOKE, EFFECT_TEX_CIRCLE, pos[0], move[0][FIRE_TYPE_SMOKE], COLOR_DARKGRAY, 100, 15);
				break;
			}
		}
	}
}

//========================================================================
// 描画
//========================================================================
void DrawEffectFire(void)
{

}

//========================================================================
// パーティクルの設定
//========================================================================
void SetEffectFire(D3DXVECTOR3 pos, FIRE_TYPE type, int nIdx)
{
	for (int nCntEffectFire = 0; nCntEffectFire < MAX_MAGIC_PARTICLE; nCntEffectFire++)
	{
		//使われていなければ
		if (g_aEffectFire[nCntEffectFire].bUse == false)
		{
			g_aEffectFire[nCntEffectFire].pos = pos;
			g_aEffectFire[nCntEffectFire].Type = type;
			g_aEffectFire[nCntEffectFire].bUse = true;
			break;
		}
	}
}

//========================================================================
// パーティクルの停止
//========================================================================
void StopEffectFire(int nIdx)
{
	for (int nCntEffectFire = 0; nCntEffectFire < MAX_MAGIC_PARTICLE; nCntEffectFire++)
	{
		//使われていれば
		if (g_aEffectFire[nCntEffectFire].bUse == true)
		{
			g_aEffectFire[nCntEffectFire].Type = FIRE_TYPE_NONE;
			g_aEffectFire[nCntEffectFire].bUse = false;
			break;
		}
	}
}