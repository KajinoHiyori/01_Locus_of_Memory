//========================================================================
// 
// 3Dポリゴンの描画[particle.cpp]
//
//========================================================================

#include"main.h"
#include"particle.h"
#include"effect.h"
#include"input.h"
#include"debugproc.h"
#include "color.h"

#define MAX_PARTICLE	(128)	//パーティクルの最大数
#define MAX_APPEAR		(5)	//粒子の最大数
#define MAX_ANGRE		(629)
#define MAX_ANGRE2		(314)
#define MAX_ONEHUNDRED	(100)
#define MAX_MOVE		(20)
#define MAX_MOVE2		(30)

//パーティクルの構造体
typedef struct
{
	D3DXVECTOR3 pos;		//位置
	D3DXVECTOR3 move;		//移動量
	D3DXCOLOR col;			//色
	D3DXMATRIX g_mtxWorldParticle;	//ワールドマトリックス
	float fRadius;			//半径
	int nLife;				//寿命（色）
	PARTICLETYPE Type;				//種類
	bool bUse;				//使用しているか
}PARTICLE;

//グローバル変数
LPDIRECT3DTEXTURE9 g_pTextureBuffParticle = NULL;
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffParticle = NULL;
PARTICLE g_aParticle[MAX_PARTICLE];

void InitParticle(void)
{

}

void UninitParticle(void)
{

}

void UpdateParticle(void)
{
	D3DXVECTOR3 pos;
	D3DXVECTOR3 move;
	int nCountParticle = 0;

	if (GetKeyboardTrigger(DIK_BACK) == true)
	{
		SetParticle(D3DXVECTOR3(0.0f, 50.0f, 0.0f), 20, PARTICLETYPE_001);
	}
	for (int nCntParticle = 0; nCntParticle < MAX_PARTICLE; nCntParticle++)
	{
		if (g_aParticle[nCntParticle].bUse == true)
		{
			switch (g_aParticle[nCntParticle].Type)
			{
			case PARTICLETYPE_001:
				//パーティクルの生成
				for (int nCntAppear = 0; nCntAppear < MAX_APPEAR; nCntAppear++)
				{
					pos = g_aParticle[nCntParticle].pos;
					move.x = (sinf(float(rand() % MAX_ANGRE - MAX_ANGRE2 / MAX_ONEHUNDRED))) * (float(rand() % MAX_MOVE - MAX_MOVE2 / MAX_ONEHUNDRED));
					move.y = (cosf(float(rand() % MAX_ANGRE - MAX_ANGRE2 / MAX_ONEHUNDRED))) * (float(rand() % MAX_MOVE - MAX_MOVE2 / MAX_ONEHUNDRED));
					move.z = (tanf(float(rand() % MAX_ANGRE - MAX_ANGRE2 / MAX_ONEHUNDRED))) * (float(rand() % MAX_MOVE - MAX_MOVE2 / MAX_ONEHUNDRED));
					SetEffect(pos, move, COLOR_RED, g_aParticle[nCntParticle].nLife);

					g_aParticle[nCntParticle].nLife--;
					if (g_aParticle[nCntParticle].nLife < 0)
					{
						g_aParticle[nCntParticle].bUse = false;
					}
				}
				break;

				//浮遊
			case PARTICLETYPE_LEVITATION:
				for (int nCntAppear = 0; nCntAppear < MAX_APPEAR; nCntAppear++)
				{
					pos = g_aParticle[nCntParticle].pos;
					move.x = (sinf(float(rand() % MAX_ANGRE - MAX_ANGRE2 / MAX_ONEHUNDRED))) * (float(rand() % MAX_MOVE - MAX_MOVE2 / MAX_ONEHUNDRED));
					move.y = (cosf(float(rand() % MAX_ANGRE - MAX_ANGRE2 / MAX_ONEHUNDRED))) * (float(rand() % MAX_MOVE - MAX_MOVE2 / MAX_ONEHUNDRED));
					move.z = (tanf(float(rand() % MAX_ANGRE - MAX_ANGRE2 / MAX_ONEHUNDRED))) * (float(rand() % MAX_MOVE - MAX_MOVE2 / MAX_ONEHUNDRED));
					SetEffect(pos, move, COLOR_RED, g_aParticle[nCntParticle].nLife);

					g_aParticle[nCntParticle].nLife--;
					if (g_aParticle[nCntParticle].nLife < 0)
					{
						g_aParticle[nCntParticle].bUse = false;
					}
				}
				break;

				//燃焼
			case PARTICLETYPE_COMBUSTION:
				for (int nCntAppear = 0; nCntAppear < MAX_APPEAR; nCntAppear++)
				{
					pos = g_aParticle[nCntParticle].pos;
					move.x = (sinf(float(rand() % MAX_ANGRE - MAX_ANGRE2 / MAX_ONEHUNDRED))) * (float(rand() % MAX_MOVE - MAX_MOVE2 / MAX_ONEHUNDRED));
					move.y = (cosf(float(rand() % MAX_ANGRE - MAX_ANGRE2 / MAX_ONEHUNDRED))) * (float(rand() % MAX_MOVE - MAX_MOVE2 / MAX_ONEHUNDRED));
					move.z = (tanf(float(rand() % MAX_ANGRE - MAX_ANGRE2 / MAX_ONEHUNDRED))) * (float(rand() % MAX_MOVE - MAX_MOVE2 / MAX_ONEHUNDRED));
					SetEffect(pos, move, COLOR_RED, g_aParticle[nCntParticle].nLife);

					g_aParticle[nCntParticle].nLife--;
					if (g_aParticle[nCntParticle].nLife < 0)
					{
						g_aParticle[nCntParticle].bUse = false;
					}
				}
				break;

				//洪水、氾濫
			case PARTICLETYPE_FLOOD:
				for (int nCntAppear = 0; nCntAppear < MAX_APPEAR; nCntAppear++)
				{
					pos = g_aParticle[nCntParticle].pos;
					move.x = (sinf(float(rand() % MAX_ANGRE - MAX_ANGRE2 / MAX_ONEHUNDRED))) * (float(rand() % MAX_MOVE - MAX_MOVE2 / MAX_ONEHUNDRED));
					move.y = (cosf(float(rand() % MAX_ANGRE - MAX_ANGRE2 / MAX_ONEHUNDRED))) * (float(rand() % MAX_MOVE - MAX_MOVE2 / MAX_ONEHUNDRED));
					move.z = (tanf(float(rand() % MAX_ANGRE - MAX_ANGRE2 / MAX_ONEHUNDRED))) * (float(rand() % MAX_MOVE - MAX_MOVE2 / MAX_ONEHUNDRED));
					SetEffect(pos, move, COLOR_RED, g_aParticle[nCntParticle].nLife);

					g_aParticle[nCntParticle].nLife--;
					if (g_aParticle[nCntParticle].nLife < 0)
					{
						g_aParticle[nCntParticle].bUse = false;
					}
				}
				break;

				//フラッシュ
			case PARTICLETYPE_FLASH:
				for (int nCntAppear = 0; nCntAppear < MAX_APPEAR; nCntAppear++)
				{
					pos = g_aParticle[nCntParticle].pos;
					move.x = (sinf(float(rand() % MAX_ANGRE - MAX_ANGRE2 / MAX_ONEHUNDRED))) * (float(rand() % MAX_MOVE - MAX_MOVE2 / MAX_ONEHUNDRED));
					move.y = (cosf(float(rand() % MAX_ANGRE - MAX_ANGRE2 / MAX_ONEHUNDRED))) * (float(rand() % MAX_MOVE - MAX_MOVE2 / MAX_ONEHUNDRED));
					move.z = (tanf(float(rand() % MAX_ANGRE - MAX_ANGRE2 / MAX_ONEHUNDRED))) * (float(rand() % MAX_MOVE - MAX_MOVE2 / MAX_ONEHUNDRED));
					SetEffect(pos, move, COLOR_RED, g_aParticle[nCntParticle].nLife);

					g_aParticle[nCntParticle].nLife--;
					if (g_aParticle[nCntParticle].nLife < 0)
					{
						g_aParticle[nCntParticle].bUse = false;
					}
				}
				break;

				//火球
			case PARTICLETYPE_FIREBALL:
				for (int nCntAppear = 0; nCntAppear < MAX_APPEAR; nCntAppear++)
				{
					pos = g_aParticle[nCntParticle].pos;
					move.x = (sinf(float(rand() % MAX_ANGRE - MAX_ANGRE2 / MAX_ONEHUNDRED))) * (float(rand() % MAX_MOVE - MAX_MOVE2 / MAX_ONEHUNDRED));
					move.y = (cosf(float(rand() % MAX_ANGRE - MAX_ANGRE2 / MAX_ONEHUNDRED))) * (float(rand() % MAX_MOVE - MAX_MOVE2 / MAX_ONEHUNDRED));
					move.z = (tanf(float(rand() % MAX_ANGRE - MAX_ANGRE2 / MAX_ONEHUNDRED))) * (float(rand() % MAX_MOVE - MAX_MOVE2 / MAX_ONEHUNDRED));
					SetEffect(pos, move, COLOR_RED, g_aParticle[nCntParticle].nLife);

					g_aParticle[nCntParticle].nLife--;
					if (g_aParticle[nCntParticle].nLife < 0)
					{
						g_aParticle[nCntParticle].bUse = false;
					}
				}
				break;

				//太陽の動きを遅延する
			case PARTICLETYPE_SUNSETDELAY:
				for (int nCntAppear = 0; nCntAppear < MAX_APPEAR; nCntAppear++)
				{
					pos = g_aParticle[nCntParticle].pos;
					move.x = (sinf(float(rand() % MAX_ANGRE - MAX_ANGRE2 / MAX_ONEHUNDRED))) * (float(rand() % MAX_MOVE - MAX_MOVE2 / MAX_ONEHUNDRED));
					move.y = (cosf(float(rand() % MAX_ANGRE - MAX_ANGRE2 / MAX_ONEHUNDRED))) * (float(rand() % MAX_MOVE - MAX_MOVE2 / MAX_ONEHUNDRED));
					move.z = (tanf(float(rand() % MAX_ANGRE - MAX_ANGRE2 / MAX_ONEHUNDRED))) * (float(rand() % MAX_MOVE - MAX_MOVE2 / MAX_ONEHUNDRED));
					SetEffect(pos, move, COLOR_RED, g_aParticle[nCntParticle].nLife);

					g_aParticle[nCntParticle].nLife--;
					if (g_aParticle[nCntParticle].nLife < 0)
					{
						g_aParticle[nCntParticle].bUse = false;
					}
				}
				break;

				//雨乞い
			case PARTICLETYPE_RAINPRAY:
				for (int nCntAppear = 0; nCntAppear < MAX_APPEAR; nCntAppear++)
				{
					pos = g_aParticle[nCntParticle].pos;
					move.x = (sinf(float(rand() % MAX_ANGRE - MAX_ANGRE2 / MAX_ONEHUNDRED))) * (float(rand() % MAX_MOVE - MAX_MOVE2 / MAX_ONEHUNDRED));
					move.y = (cosf(float(rand() % MAX_ANGRE - MAX_ANGRE2 / MAX_ONEHUNDRED))) * (float(rand() % MAX_MOVE - MAX_MOVE2 / MAX_ONEHUNDRED));
					move.z = (tanf(float(rand() % MAX_ANGRE - MAX_ANGRE2 / MAX_ONEHUNDRED))) * (float(rand() % MAX_MOVE - MAX_MOVE2 / MAX_ONEHUNDRED));
					SetEffect(pos, move, COLOR_RED, g_aParticle[nCntParticle].nLife);

					g_aParticle[nCntParticle].nLife--;
					if (g_aParticle[nCntParticle].nLife < 0)
					{
						g_aParticle[nCntParticle].bUse = false;
					}
				}
				break;

				//凍結
			case PARTICLETYPE_FREEZE:
				for (int nCntAppear = 0; nCntAppear < MAX_APPEAR; nCntAppear++)
				{
					pos = g_aParticle[nCntParticle].pos;
					move.x = (sinf(float(rand() % MAX_ANGRE - MAX_ANGRE2 / MAX_ONEHUNDRED))) * (float(rand() % MAX_MOVE - MAX_MOVE2 / MAX_ONEHUNDRED));
					move.y = (cosf(float(rand() % MAX_ANGRE - MAX_ANGRE2 / MAX_ONEHUNDRED))) * (float(rand() % MAX_MOVE - MAX_MOVE2 / MAX_ONEHUNDRED));
					move.z = (tanf(float(rand() % MAX_ANGRE - MAX_ANGRE2 / MAX_ONEHUNDRED))) * (float(rand() % MAX_MOVE - MAX_MOVE2 / MAX_ONEHUNDRED));
					SetEffect(pos, move, COLOR_RED, g_aParticle[nCntParticle].nLife);

					g_aParticle[nCntParticle].nLife--;
					if (g_aParticle[nCntParticle].nLife < 0)
					{
						g_aParticle[nCntParticle].bUse = false;
					}
				}
				break;

				//成長(植物など)
			case PARTICLETYPE_GROWTH:
				for (int nCntAppear = 0; nCntAppear < MAX_APPEAR; nCntAppear++)
				{
					pos = g_aParticle[nCntParticle].pos;
					move.x = (sinf(float(rand() % MAX_ANGRE - MAX_ANGRE2 / MAX_ONEHUNDRED))) * (float(rand() % MAX_MOVE - MAX_MOVE2 / MAX_ONEHUNDRED));
					move.y = (cosf(float(rand() % MAX_ANGRE - MAX_ANGRE2 / MAX_ONEHUNDRED))) * (float(rand() % MAX_MOVE - MAX_MOVE2 / MAX_ONEHUNDRED));
					move.z = (tanf(float(rand() % MAX_ANGRE - MAX_ANGRE2 / MAX_ONEHUNDRED))) * (float(rand() % MAX_MOVE - MAX_MOVE2 / MAX_ONEHUNDRED));
					SetEffect(pos, move, COLOR_RED, g_aParticle[nCntParticle].nLife);

					g_aParticle[nCntParticle].nLife--;
					if (g_aParticle[nCntParticle].nLife < 0)
					{
						g_aParticle[nCntParticle].bUse = false;
					}
				}
				break;

				//加速
			case PARTICLETYPE_ACCELERATION:
				for (int nCntAppear = 0; nCntAppear < MAX_APPEAR; nCntAppear++)
				{
					pos = g_aParticle[nCntParticle].pos;
					move.x = (sinf(float(rand() % MAX_ANGRE - MAX_ANGRE2 / MAX_ONEHUNDRED))) * (float(rand() % MAX_MOVE - MAX_MOVE2 / MAX_ONEHUNDRED));
					move.y = (cosf(float(rand() % MAX_ANGRE - MAX_ANGRE2 / MAX_ONEHUNDRED))) * (float(rand() % MAX_MOVE - MAX_MOVE2 / MAX_ONEHUNDRED));
					move.z = (tanf(float(rand() % MAX_ANGRE - MAX_ANGRE2 / MAX_ONEHUNDRED))) * (float(rand() % MAX_MOVE - MAX_MOVE2 / MAX_ONEHUNDRED));
					SetEffect(pos, move, COLOR_RED, g_aParticle[nCntParticle].nLife);

					g_aParticle[nCntParticle].nLife--;
					if (g_aParticle[nCntParticle].nLife < 0)
					{
						g_aParticle[nCntParticle].bUse = false;
					}
				}
				break;

				//時間の巻き戻し(回帰)
			case PARTICLETYPE_TIMEREVERT:
				for (int nCntAppear = 0; nCntAppear < MAX_APPEAR; nCntAppear++)
				{
					pos = g_aParticle[nCntParticle].pos;
					move.x = (sinf(float(rand() % MAX_ANGRE - MAX_ANGRE2 / MAX_ONEHUNDRED))) * (float(rand() % MAX_MOVE - MAX_MOVE2 / MAX_ONEHUNDRED));
					move.y = (cosf(float(rand() % MAX_ANGRE - MAX_ANGRE2 / MAX_ONEHUNDRED))) * (float(rand() % MAX_MOVE - MAX_MOVE2 / MAX_ONEHUNDRED));
					move.z = (tanf(float(rand() % MAX_ANGRE - MAX_ANGRE2 / MAX_ONEHUNDRED))) * (float(rand() % MAX_MOVE - MAX_MOVE2 / MAX_ONEHUNDRED));
					SetEffect(pos, move, COLOR_RED, g_aParticle[nCntParticle].nLife);

					g_aParticle[nCntParticle].nLife--;
					if (g_aParticle[nCntParticle].nLife < 0)
					{
						g_aParticle[nCntParticle].bUse = false;
					}
				}
				break;
			}
			nCountParticle++;
			
		}
	}
	PrintDebugProc("使用しているパーティクル : %d\n", nCountParticle++);
}
void DrawParticle(void)
{

}

void SetParticle(D3DXVECTOR3 pos, int nLife, PARTICLETYPE type)
{
	for (int nCntParticle = 0; nCntParticle < MAX_PARTICLE; nCntParticle++)
	{
		//使われていなければ
		if (g_aParticle[nCntParticle].bUse == false)
		{
			g_aParticle[nCntParticle].pos = pos;
			g_aParticle[nCntParticle].nLife = nLife;
			g_aParticle[nCntParticle].Type = type;
			g_aParticle[nCntParticle].bUse = true;
			break;
		}
	}

}