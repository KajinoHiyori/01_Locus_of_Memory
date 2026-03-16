//========================================================================
// 
// パーティクル描画[magiceffct.cpp]
// Author : KAIRI MANABE
//
//========================================================================

#include "main.h"
#include "player.h"
#include "magiceffect.h"
#include "effect.h"
#include "input.h"
#include "debugproc.h"
#include "color.h"

#define MAX_MAGIC_PARTICLE	(1)		//パーティクルの最大数
#define MAX_MAGIC_APPEAR	(5)			//粒子の最大数
#define MAX_ANGRE			(629)
#define MAX_ANGRE2			(314)

//パーティクルの構造体
typedef struct
{
	D3DXVECTOR3 pos;					//位置
	D3DXVECTOR3 move;					//移動量
	D3DXCOLOR col;						//色
	D3DXMATRIX g_mtxWorldMagicEffect;	//ワールドマトリックス
	float fRadius;						//半径
	MAGICEF_TYPE Type;					//種類
	bool bUse;							//使用しているか
}PARTICLE;

//グローバル変数
LPDIRECT3DTEXTURE9 g_pTextureBuffMagicEffect = NULL;
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffMagicEffect = NULL;
PARTICLE g_aMagicEffect[MAX_PLAYER][MAX_MAGIC_PARTICLE];

//========================================================================
// 初期化
//========================================================================
void InitMagicEffect(void)
{
	for (int nCntPlayerType = 0; nCntPlayerType < MAX_PLAYER; nCntPlayerType++)
	{
		for (int nCntMagicEffect = 0; nCntMagicEffect < MAX_MAGIC_PARTICLE; nCntMagicEffect++)
		{
			g_aMagicEffect[nCntPlayerType][nCntMagicEffect].bUse = false;
			g_aMagicEffect[nCntPlayerType][nCntMagicEffect].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
			g_aMagicEffect[nCntPlayerType][nCntMagicEffect].move = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
			g_aMagicEffect[nCntPlayerType][nCntMagicEffect].fRadius = 0;
			g_aMagicEffect[nCntPlayerType][nCntMagicEffect].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
			g_aMagicEffect[nCntPlayerType][nCntMagicEffect].Type = MAGICEF_TYPE_NONE;
		}
	}
}

//========================================================================
// 終了
//========================================================================
void UninitMagicEffect(void)
{

}

//========================================================================
// 更新
//========================================================================
void UpdateMagicEffect(void)
{
	D3DXVECTOR3 pos[3];
	D3DXVECTOR3 move[3][MAGICEF_TYPE_MAX];		//パーティクルのmove
	D3DXVECTOR3 rot;							//向き
	float fSpeed;								//速度調整
	float fAngle = 0.0f;
	float fRadius = 0.0f;
	int nCountMagicEffect = 0;

	for (int nCntMovetype = 0; nCntMovetype < 3; nCntMovetype++)
	{
		pos[nCntMovetype] = D3DXVECTOR3(0.0f, 0.0f, 0.0f);

		for (int nCntMove = 0; nCntMove < MAGICEF_TYPE_MAX; nCntMove++)
		{
			move[nCntMovetype][nCntMove] = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		}
	}

	rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);

#ifdef _DEBUG
	if (GetKeyboardTrigger(DIK_M) == true)
	{
		SetMagicEffect(D3DXVECTOR3(0.0f, 0.0f, 0.0f), MAGICEF_TYPE_GIMMICK,1);
	}

	if (GetKeyboardTrigger(DIK_N) == true)
	{
		SetMagicEffect(D3DXVECTOR3(0.0f, 0.0f, 0.0f), MAGICEF_TYPE_MAGIC,1);
	}

	if (GetKeyboardTrigger(DIK_B) == true)
	{
		SetMagicEffect(D3DXVECTOR3(0.0f, 0.0f, 0.0f), MAGICEF_TYPE_GIMMICK, 2);
	}

	if (GetKeyboardTrigger(DIK_V) == true)
	{
		SetMagicEffect(D3DXVECTOR3(0.0f, 0.0f, 0.0f), MAGICEF_TYPE_MAGIC, 2);
	}
#endif

	for (int nCntPlayerType = 0; nCntPlayerType < MAX_PLAYER; nCntPlayerType++)
	{//プレイヤーの数
		for (int nCntMagicEffect = 0; nCntMagicEffect < MAX_MAGIC_PARTICLE; nCntMagicEffect++)
		{
			if (g_aMagicEffect[nCntPlayerType][nCntMagicEffect].bUse == true)
			{//使用されていたら
				switch (g_aMagicEffect[nCntPlayerType][nCntMagicEffect].Type)
				{
					//ギミック=========================================================================
				case MAGICEF_TYPE_GIMMICK:
					//位置の設定
					pos[0].x = g_aMagicEffect[nCntPlayerType][nCntMagicEffect].pos.x + (sinf((float)(rand() % 629 - 314 / 100.0f))) * (float)(rand() % 60 + 1);
					pos[0].y = g_aMagicEffect[nCntPlayerType][nCntMagicEffect].pos.y + (cosf((float)(rand() % 629 - 314 / 100.0f))) * (float)(rand() % 60 + 1) + 20;
					pos[0].z = g_aMagicEffect[nCntPlayerType][nCntMagicEffect].pos.z + (cosf((float)(rand() % 629 - 314 / 100.0f))) * (float)(rand() % 60 + 1);

					//位置の設定
					pos[1].x = g_aMagicEffect[nCntPlayerType][nCntMagicEffect].pos.x + (sinf((float)(rand() % 629 - 314 / 100.0f))) * (float)(rand() % 50 + 1);
					pos[1].y = g_aMagicEffect[nCntPlayerType][nCntMagicEffect].pos.y + (cosf((float)(rand() % 629 - 314 / 100.0f))) * (float)(rand() % 50 + 1) + 20;
					pos[1].z = g_aMagicEffect[nCntPlayerType][nCntMagicEffect].pos.z + (cosf((float)(rand() % 629 - 314 / 100.0f))) * (float)(rand() % 50 + 1);

					SetEffect(EFFECT_TYPE_MAGICEF, EFFECT_TEX_DIAMOND, pos[0], move[0][MAGICEF_TYPE_GIMMICK], COLOR_WHITE, 100, 8);
					SetEffect(EFFECT_TYPE_MAGICEF, EFFECT_TEX_DIAMOND, pos[1], move[0][MAGICEF_TYPE_GIMMICK], COLOR_YELLOW, 100, 5);
					break;
					//落ちている魔法===================================================================
				case MAGICEF_TYPE_MAGIC:
					//位置の設定
					pos[0].x = g_aMagicEffect[nCntPlayerType][nCntMagicEffect].pos.x + (sinf((float)(rand() % 629 - 314 / 100.0f))) * (float)(rand() % 30 + 1);
					pos[0].y = g_aMagicEffect[nCntPlayerType][nCntMagicEffect].pos.y + (cosf((float)(rand() % 629 - 314 / 100.0f))) * (float)(rand() % 30 + 1) + 20;
					pos[0].z = g_aMagicEffect[nCntPlayerType][nCntMagicEffect].pos.z + (cosf((float)(rand() % 629 - 314 / 100.0f))) * (float)(rand() % 30 + 1);

					//位置の設定
					pos[1].x = g_aMagicEffect[nCntPlayerType][nCntMagicEffect].pos.x + (sinf((float)(rand() % 629 - 314 / 100.0f))) * (float)(rand() % 25 + 1);
					pos[1].y = g_aMagicEffect[nCntPlayerType][nCntMagicEffect].pos.y + (cosf((float)(rand() % 629 - 314 / 100.0f))) * (float)(rand() % 25 + 1) + 20;
					pos[1].z = g_aMagicEffect[nCntPlayerType][nCntMagicEffect].pos.z + (cosf((float)(rand() % 629 - 314 / 100.0f))) * (float)(rand() % 25 + 1);

					SetEffect(EFFECT_TYPE_MAGICEF, EFFECT_TEX_DIAMOND, pos[0], move[0][MAGICEF_TYPE_MAGIC], COLOR_WHITE, 100, 8);
					SetEffect(EFFECT_TYPE_MAGICEF, EFFECT_TEX_DIAMOND, pos[1], move[0][MAGICEF_TYPE_MAGIC], COLOR_YELLOW, 100, 5);
					break;
				}
			}
		}
	}
}

//========================================================================
// 描画
//========================================================================
void DrawMagicEffect(void)
{

}

//========================================================================
// パーティクルの設定
//========================================================================
void SetMagicEffect(D3DXVECTOR3 pos, MAGICEF_TYPE type, int nIdx)
{
	for (int nCntMagicEffect = 0; nCntMagicEffect < MAX_MAGIC_PARTICLE; nCntMagicEffect++)
	{
		//使われていなければ
		if (g_aMagicEffect[nIdx][nCntMagicEffect].bUse == false)
		{
			g_aMagicEffect[nIdx][nCntMagicEffect].pos = pos;
			g_aMagicEffect[nIdx][nCntMagicEffect].Type = type;
			g_aMagicEffect[nIdx][nCntMagicEffect].bUse = true;
			break;
		}
	}
}

//========================================================================
// パーティクルの停止
//========================================================================
void StopMagicEffect(int nIdx)
{
	for (int nCntMagicEffect = 0; nCntMagicEffect < MAX_MAGIC_PARTICLE; nCntMagicEffect++)
	{
		//使われていれば
		if (g_aMagicEffect[nIdx][nCntMagicEffect].bUse == true)
		{
			g_aMagicEffect[nIdx][nCntMagicEffect].Type = MAGICEF_TYPE_NONE;
			g_aMagicEffect[nIdx][nCntMagicEffect].bUse = false;
			break;
		}
	}
}