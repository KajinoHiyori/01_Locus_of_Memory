//========================================================================
// 
// 3Dポリゴンの描画[particle.cpp]
//
//========================================================================

#include"main.h"
#include"particle.h"

#define MAX_PARTICLE	(128)	//パーティクルの最大数
#define MAX_APPEAR		(10)	//粒子の最大数
#define MAX_ANGRE		(629)
#define MAX_ANGRE2		(314)
#define MAX_ONEHUNDRED	(100)
#define MAX_MOVE		(200)
#define MAX_MOVE2		(300)

//パーティクルの構造体
typedef struct
{
	D3DXVECTOR3 pos;		//位置
	D3DXVECTOR3 move;		//移動量
	D3DXCOLOR col;			//色
	D3DXMATRIX g_mtxWorldParticle;	//ワールドマトリックス
	float fRadius;			//半径
	int nLife;				//寿命（色）
	int nType;				//種類
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
	for (int nCntParticle = 0; nCntParticle < MAX_PARTICLE; nCntParticle++)
	{
		if (g_aParticle[nCntParticle].bUse == true)
		{
			//パーティクルの生成
			for (int nCntAppear = 0; nCntAppear < MAX_APPEAR; nCntAppear++)
			{
				pos = g_aParticle[nCntParticle].pos;
				move.x = (sinf(float(rand() % MAX_ANGRE - MAX_ANGRE2 / MAX_ONEHUNDRED))) * (float(rand() % MAX_MOVE - MAX_MOVE2 / MAX_ONEHUNDRED));
				move.y = (cosf(float(rand() % MAX_ANGRE - MAX_ANGRE2 / MAX_ONEHUNDRED))) * (float(rand() % MAX_MOVE - MAX_MOVE2 / MAX_ONEHUNDRED));
				move.z = (tanf(float(rand() % MAX_ANGRE - MAX_ANGRE2 / MAX_ONEHUNDRED))) * (float(rand() % MAX_MOVE - MAX_MOVE2 / MAX_ONEHUNDRED));
			}
		}
	}
}