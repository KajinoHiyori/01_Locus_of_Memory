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
	D3DXVECTOR3 pos1, pos2;
	D3DXVECTOR3 move1, move2;
	int nCountParticle = 0;

	pos1 = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	pos2 = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	move1 = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	move2 = D3DXVECTOR3(0.0f, 0.0f, 0.0f);

	if (GetKeyboardTrigger(DIK_BACK) == true)
	{
		SetParticle(D3DXVECTOR3(0.0f, 50.0f, 0.0f), 100, PARTICLETYPE_COMBUSTION);
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
					pos1 = g_aParticle[nCntParticle].pos;
					move1.x = (sinf(float(rand() % MAX_ANGRE - MAX_ANGRE2 / MAX_ONEHUNDRED))) * (float(rand() % MAX_MOVE - MAX_MOVE2 / MAX_ONEHUNDRED));
					move1.y = (cosf(float(rand() % MAX_ANGRE - MAX_ANGRE2 / MAX_ONEHUNDRED))) * (float(rand() % MAX_MOVE - MAX_MOVE2 / MAX_ONEHUNDRED));
					move1.z = (tanf(float(rand() % MAX_ANGRE - MAX_ANGRE2 / MAX_ONEHUNDRED))) * (float(rand() % MAX_MOVE - MAX_MOVE2 / MAX_ONEHUNDRED));
					SetEffect(pos1, move1, COLOR_RED, g_aParticle[nCntParticle].nLife);

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
					pos1 = g_aParticle[nCntParticle].pos;
					move1.x = (sinf(float(rand() % MAX_ANGRE - MAX_ANGRE2 / MAX_ONEHUNDRED))) * (float(rand() % MAX_MOVE - MAX_MOVE2 / MAX_ONEHUNDRED));
					move1.y = (cosf(float(rand() % MAX_ANGRE - MAX_ANGRE2 / MAX_ONEHUNDRED))) * (float(rand() % MAX_MOVE - MAX_MOVE2 / MAX_ONEHUNDRED));
					move1.z = (tanf(float(rand() % MAX_ANGRE - MAX_ANGRE2 / MAX_ONEHUNDRED))) * (float(rand() % MAX_MOVE - MAX_MOVE2 / MAX_ONEHUNDRED));
					SetEffect(pos1, move1, COLOR_RED, g_aParticle[nCntParticle].nLife);

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
					//位置の設定
					pos1.x = g_aParticle[nCntAppear].pos.x + sinf((float)(rand() % 70));
					pos1.y = g_aParticle[nCntAppear].pos.y + 100;
					pos1.z = g_aParticle[nCntAppear].pos.z + sinf((float)(rand() % 70));

					pos2.x = g_aParticle[nCntAppear].pos.x + sinf((float)(rand() % 40));
					pos2.y = g_aParticle[nCntAppear].pos.y + 30;
					pos2.z = g_aParticle[nCntAppear].pos.z + sinf((float)(rand() % 40));

					//移動量
					move1.x = sinf((float)(rand() % 629 - 314)) * 2.0f;
					move1.y = cosf((float)(rand() % 629 - 314)) * 2.4f;
					move1.z = sinf((float)(rand() % 629 - 314)) * 2.0f;

					move2.x = sinf((float)(rand() % 629 - 314)) * 1.0f;
					move2.y = cosf((float)(rand() % 629 - 314)) * 1.0f;
					move2.z = sinf((float)(rand() % 629 - 314)) * 1.0f;

					SetEffect(pos1, move1, COLOR_RED, g_aParticle[nCntParticle].nLife);
					SetEffect(pos2, move2, COLOR_ORANGE, g_aParticle[nCntParticle].nLife);

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
					pos1 = g_aParticle[nCntParticle].pos;
					move1.x = (sinf(float(rand() % MAX_ANGRE - MAX_ANGRE2 / MAX_ONEHUNDRED))) * (float(rand() % MAX_MOVE - MAX_MOVE2 / MAX_ONEHUNDRED));
					move1.y = (cosf(float(rand() % MAX_ANGRE - MAX_ANGRE2 / MAX_ONEHUNDRED))) * (float(rand() % MAX_MOVE - MAX_MOVE2 / MAX_ONEHUNDRED));
					move1.z = (tanf(float(rand() % MAX_ANGRE - MAX_ANGRE2 / MAX_ONEHUNDRED))) * (float(rand() % MAX_MOVE - MAX_MOVE2 / MAX_ONEHUNDRED));
					SetEffect(pos1, move1, COLOR_RED, g_aParticle[nCntParticle].nLife);

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
					pos1 = g_aParticle[nCntParticle].pos;
					move1.x = (sinf(float(rand() % MAX_ANGRE - MAX_ANGRE2 / MAX_ONEHUNDRED))) * (float(rand() % MAX_MOVE - MAX_MOVE2 / MAX_ONEHUNDRED));
					move1.y = (cosf(float(rand() % MAX_ANGRE - MAX_ANGRE2 / MAX_ONEHUNDRED))) * (float(rand() % MAX_MOVE - MAX_MOVE2 / MAX_ONEHUNDRED));
					move1.z = (tanf(float(rand() % MAX_ANGRE - MAX_ANGRE2 / MAX_ONEHUNDRED))) * (float(rand() % MAX_MOVE - MAX_MOVE2 / MAX_ONEHUNDRED));
					SetEffect(pos1, move1, COLOR_RED, g_aParticle[nCntParticle].nLife);

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
					pos1 = g_aParticle[nCntParticle].pos;
					move1.x = (sinf(float(rand() % MAX_ANGRE - MAX_ANGRE2 / MAX_ONEHUNDRED))) * (float(rand() % MAX_MOVE - MAX_MOVE2 / MAX_ONEHUNDRED));
					move1.y = (cosf(float(rand() % MAX_ANGRE - MAX_ANGRE2 / MAX_ONEHUNDRED))) * (float(rand() % MAX_MOVE - MAX_MOVE2 / MAX_ONEHUNDRED));
					move1.z = (tanf(float(rand() % MAX_ANGRE - MAX_ANGRE2 / MAX_ONEHUNDRED))) * (float(rand() % MAX_MOVE - MAX_MOVE2 / MAX_ONEHUNDRED));
					SetEffect(pos1, move1, COLOR_RED, g_aParticle[nCntParticle].nLife);

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
					pos1 = g_aParticle[nCntParticle].pos;
					move1.x = (sinf(float(rand() % MAX_ANGRE - MAX_ANGRE2 / MAX_ONEHUNDRED))) * (float(rand() % MAX_MOVE - MAX_MOVE2 / MAX_ONEHUNDRED));
					move1.y = (cosf(float(rand() % MAX_ANGRE - MAX_ANGRE2 / MAX_ONEHUNDRED))) * (float(rand() % MAX_MOVE - MAX_MOVE2 / MAX_ONEHUNDRED));
					move1.z = (tanf(float(rand() % MAX_ANGRE - MAX_ANGRE2 / MAX_ONEHUNDRED))) * (float(rand() % MAX_MOVE - MAX_MOVE2 / MAX_ONEHUNDRED));
					SetEffect(pos1, move1, COLOR_RED, g_aParticle[nCntParticle].nLife);

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
					pos1 = g_aParticle[nCntParticle].pos;
					move1.x = (sinf(float(rand() % MAX_ANGRE - MAX_ANGRE2 / MAX_ONEHUNDRED))) * (float(rand() % MAX_MOVE - MAX_MOVE2 / MAX_ONEHUNDRED));
					move1.y = (cosf(float(rand() % MAX_ANGRE - MAX_ANGRE2 / MAX_ONEHUNDRED))) * (float(rand() % MAX_MOVE - MAX_MOVE2 / MAX_ONEHUNDRED));
					move1.z = (tanf(float(rand() % MAX_ANGRE - MAX_ANGRE2 / MAX_ONEHUNDRED))) * (float(rand() % MAX_MOVE - MAX_MOVE2 / MAX_ONEHUNDRED));
					SetEffect(pos1, move1, COLOR_RED, g_aParticle[nCntParticle].nLife);

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
					pos1 = g_aParticle[nCntParticle].pos;
					move1.x = (sinf(float(rand() % MAX_ANGRE - MAX_ANGRE2 / MAX_ONEHUNDRED))) * (float(rand() % MAX_MOVE - MAX_MOVE2 / MAX_ONEHUNDRED));
					move1.y = (cosf(float(rand() % MAX_ANGRE - MAX_ANGRE2 / MAX_ONEHUNDRED))) * (float(rand() % MAX_MOVE - MAX_MOVE2 / MAX_ONEHUNDRED));
					move1.z = (tanf(float(rand() % MAX_ANGRE - MAX_ANGRE2 / MAX_ONEHUNDRED))) * (float(rand() % MAX_MOVE - MAX_MOVE2 / MAX_ONEHUNDRED));
					SetEffect(pos1, move1, COLOR_RED, g_aParticle[nCntParticle].nLife);

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
					pos1 = g_aParticle[nCntParticle].pos;
					move1.x = (sinf(float(rand() % MAX_ANGRE - MAX_ANGRE2 / MAX_ONEHUNDRED))) * (float(rand() % MAX_MOVE - MAX_MOVE2 / MAX_ONEHUNDRED));
					move1.y = (cosf(float(rand() % MAX_ANGRE - MAX_ANGRE2 / MAX_ONEHUNDRED))) * (float(rand() % MAX_MOVE - MAX_MOVE2 / MAX_ONEHUNDRED));
					move1.z = (tanf(float(rand() % MAX_ANGRE - MAX_ANGRE2 / MAX_ONEHUNDRED))) * (float(rand() % MAX_MOVE - MAX_MOVE2 / MAX_ONEHUNDRED));
					SetEffect(pos1, move1, COLOR_RED, g_aParticle[nCntParticle].nLife);

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
					pos1 = g_aParticle[nCntParticle].pos;
					move1.x = (sinf(float(rand() % MAX_ANGRE - MAX_ANGRE2 / MAX_ONEHUNDRED))) * (float(rand() % MAX_MOVE - MAX_MOVE2 / MAX_ONEHUNDRED));
					move1.y = (cosf(float(rand() % MAX_ANGRE - MAX_ANGRE2 / MAX_ONEHUNDRED))) * (float(rand() % MAX_MOVE - MAX_MOVE2 / MAX_ONEHUNDRED));
					move1.z = (tanf(float(rand() % MAX_ANGRE - MAX_ANGRE2 / MAX_ONEHUNDRED))) * (float(rand() % MAX_MOVE - MAX_MOVE2 / MAX_ONEHUNDRED));
					SetEffect(pos1, move1, COLOR_RED, g_aParticle[nCntParticle].nLife);

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
					pos1 = g_aParticle[nCntParticle].pos;
					move1.x = (sinf(float(rand() % MAX_ANGRE - MAX_ANGRE2 / MAX_ONEHUNDRED))) * (float(rand() % MAX_MOVE - MAX_MOVE2 / MAX_ONEHUNDRED));
					move1.y = (cosf(float(rand() % MAX_ANGRE - MAX_ANGRE2 / MAX_ONEHUNDRED))) * (float(rand() % MAX_MOVE - MAX_MOVE2 / MAX_ONEHUNDRED));
					move1.z = (tanf(float(rand() % MAX_ANGRE - MAX_ANGRE2 / MAX_ONEHUNDRED))) * (float(rand() % MAX_MOVE - MAX_MOVE2 / MAX_ONEHUNDRED));
					SetEffect(pos1, move1, COLOR_RED, g_aParticle[nCntParticle].nLife);

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