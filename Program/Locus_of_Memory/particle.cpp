//========================================================================
// 
// パーティクル描画[particle.cpp]
// Author : KAIRI MANABE
//
//========================================================================

#include"main.h"
#include"particle.h"
#include "player.h"
#include"effect.h"
#include"input.h"
#include"debugproc.h"
#include "color.h"

#define MAX_PARTICLE	(256)	//パーティクルの最大数
#define MAX_APPEAR		(5)		//粒子の最大数
#define MAX_ANGRE		(629)
#define MAX_ANGRE2		(314)
#define MAX_ONEHUNDRED	(100)
#define MAX_MOVE		(20)
#define MAX_MOVE2		(30)
#define MAX_MOVE3		(10)
#define MAX_MOVE4		(20)

//パーティクルの構造体
typedef struct
{
	D3DXVECTOR3 pos;		//位置
	D3DXVECTOR3 move;		//移動量
	D3DXCOLOR col;			//色
	D3DXMATRIX g_mtxWorldParticle;	//ワールドマトリックス
	float fRadius;			//半径
	int nLife;				//寿命（色）
	PARTICLETYPE Type;		//種類
	bool bUse;				//使用しているか
}PARTICLE;

//グローバル変数
LPDIRECT3DTEXTURE9 g_pTextureBuffParticle = NULL;
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffParticle = NULL;
PARTICLE g_aParticle[MAX_PLAYER][MAX_PARTICLE];

//========================================================================
// 初期化
//========================================================================
void InitParticle(void)
{
	for (int nCntPlayerType = 0; nCntPlayerType < MAX_PLAYER; nCntPlayerType++)
	{
		for (int nCntParticle = 0; nCntParticle < MAX_PARTICLE; nCntParticle++)
		{
			g_aParticle[nCntPlayerType][nCntParticle].bUse = false;
			g_aParticle[nCntPlayerType][nCntParticle].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
			g_aParticle[nCntPlayerType][nCntParticle].move = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
			g_aParticle[nCntPlayerType][nCntParticle].nLife = 0;
			g_aParticle[nCntPlayerType][nCntParticle].fRadius = 0;
			g_aParticle[nCntPlayerType][nCntParticle].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
			g_aParticle[nCntPlayerType][nCntParticle].Type = PARTICLETYPE_NONE;
		}
	}
}

//========================================================================
// 終了
//========================================================================
void UninitParticle(void)
{

}

//========================================================================
// 更新
//========================================================================
void UpdateParticle(void)
{
	D3DXVECTOR3 pos[3];
	D3DXVECTOR3 move[4][PARTICLETYPE_MAX];		//パーティクルのmove
	D3DXVECTOR3 rot;							//向き
	float fSpeed;								//速度調整
	float fAngle = 0.0f;
	float fRadius = 0.0f;
	float theta = 0.0f;							//シータ
	float phi = 0.0f;							//ファイ
	int nCountParticle = 0;

	for (int nCntMovetype = 0; nCntMovetype < 3; nCntMovetype++)
	{
		pos[nCntMovetype] = D3DXVECTOR3(0.0f, 0.0f, 0.0f);

		for (int nCntMove = 0; nCntMove < PARTICLETYPE_MAX; nCntMove++)
		{
			move[nCntMovetype][nCntMove] = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		}
	}

	rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);

#ifdef _DEBUG
	if (GetKeyboardTrigger(DIK_BACK) == true)
	{//時間
		SetParticle(D3DXVECTOR3(0.0f, 0.0f, 0.0f), 120, PARTICLETYPE_TIMEREVERT,1);
	}

	if (GetKeyboardTrigger(DIK_1) == true)
	{//燃焼
		SetParticle(D3DXVECTOR3(0.0f, 0.0f, 0.0f), 200, PARTICLETYPE_COMBUSTION,1);
	}

	if (GetKeyboardTrigger(DIK_2) == true)
	{//洪水、氾濫
		SetParticle(D3DXVECTOR3(0.0f, 0.0f, 0.0f), 120, PARTICLETYPE_FLOOD,1);
	}

	if (GetKeyboardTrigger(DIK_3) == true)
	{//フラッシュ
		SetParticle(D3DXVECTOR3(0.0f, 0.0f, 0.0f), 120, PARTICLETYPE_FLASH,1);
	}

	if (GetKeyboardTrigger(DIK_4) == true)
	{//火球
		SetParticle(D3DXVECTOR3(0.0f, 0.0f, 0.0f), 120, PARTICLETYPE_FIREBALL,1);
	}

	if (GetKeyboardTrigger(DIK_5) == true)
	{//太陽の動きを遅延する
		SetParticle(D3DXVECTOR3(0.0f, 0.0f, 0.0f), 120, PARTICLETYPE_SUNSETDELAY,1);
	}

	if (GetKeyboardTrigger(DIK_6) == true)
	{//雨乞い
		SetParticle(D3DXVECTOR3(0.0f, 0.0f, 0.0f), 240, PARTICLETYPE_RAINPRAY,1);
	}

	if (GetKeyboardTrigger(DIK_7) == true)
	{//凍結
		SetParticle(D3DXVECTOR3(0.0f, 0.0f, 0.0f), 100, PARTICLETYPE_FREEZE,1);
	}

	if (GetKeyboardTrigger(DIK_8) == true)
	{//成長(植物など)
		SetParticle(D3DXVECTOR3(0.0f, 0.0f, 0.0f), 100, PARTICLETYPE_GROWTH,1);
	}

	if (GetKeyboardTrigger(DIK_9) == true)
	{//加速
		SetParticle(D3DXVECTOR3(0.0f, 0.0f, 0.0f), 100, PARTICLETYPE_ACCELERATION,1);
	}

	if (GetKeyboardTrigger(DIK_0) == true)
	{//浮遊
		SetParticle(D3DXVECTOR3(0.0f, 0.0f, 0.0f), 120, PARTICLETYPE_LEVITATION,1);
	}
#endif
	for (int nCntPlayerType = 0; nCntPlayerType < MAX_PLAYER; nCntPlayerType++)
	{
		for (int nCntParticle = 0; nCntParticle < MAX_PARTICLE; nCntParticle++)
		{
			if (g_aParticle[nCntPlayerType][nCntParticle].bUse == true)
			{
				switch (g_aParticle[nCntPlayerType][nCntParticle].Type)
				{
					//浮遊=============================================================================
				case PARTICLETYPE_LEVITATION:
					for (int nCntAppear = 0; nCntAppear < 1; nCntAppear++)
					{
						//位置の設定
						pos[0].x = g_aParticle[nCntPlayerType][nCntParticle].pos.x + sinf((float)(rand() % 100)) * 40;
						pos[0].y = g_aParticle[nCntPlayerType][nCntParticle].pos.y;
						pos[0].z = g_aParticle[nCntPlayerType][nCntParticle].pos.z + cosf((float)(rand() % 100)) * 40;

						fSpeed = (float)(rand() % 2 + 1);

						//移動量	
						rot.x = ((float)(rand() % 629 - 314) / 100);
						rot.z = ((float)(rand() % 629 - 314) / 100);

						move[0][PARTICLETYPE_LEVITATION].x = sinf(rot.x) * fSpeed;
						move[0][PARTICLETYPE_LEVITATION].z = cosf(rot.z) * fSpeed;
						move[0][PARTICLETYPE_LEVITATION].y = -1.5f;

						SetEffect(EFFECT_TYPE_NORMAL, EFFECT_TEX_WING000, pos[0], move[0][PARTICLETYPE_LEVITATION], COLOR_WHITE, 100, 15);
					}
					for (int nCntAppear = 0; nCntAppear < 1; nCntAppear++)
					{
						//位置の設定
						pos[0].x = g_aParticle[nCntPlayerType][nCntParticle].pos.x + sinf((float)(rand() % 100)) * 40;
						pos[0].y = g_aParticle[nCntPlayerType][nCntParticle].pos.y;
						pos[0].z = g_aParticle[nCntPlayerType][nCntParticle].pos.z + cosf((float)(rand() % 100)) * 40;

						fSpeed = (float)(rand() % 2 + 1);

						//移動量	
						rot.x = ((float)(rand() % 629 - 314) / 100);
						rot.z = ((float)(rand() % 629 - 314) / 100);

						move[0][PARTICLETYPE_LEVITATION].x = sinf(rot.x) * fSpeed;
						move[0][PARTICLETYPE_LEVITATION].z = cosf(rot.z) * fSpeed;
						move[0][PARTICLETYPE_LEVITATION].y = -1.5f;

						SetEffect(EFFECT_TYPE_NORMAL, EFFECT_TEX_WING001, pos[0], move[0][PARTICLETYPE_LEVITATION], COLOR_LIMEGREEN, 100, 15);
					}
					for (int nCntAppear = 0; nCntAppear < 1; nCntAppear++)
					{
						//位置の設定
						pos[0].x = g_aParticle[nCntPlayerType][nCntParticle].pos.x + sinf((float)(rand() % 100)) * 40;
						pos[0].y = g_aParticle[nCntPlayerType][nCntParticle].pos.y;
						pos[0].z = g_aParticle[nCntPlayerType][nCntParticle].pos.z + cosf((float)(rand() % 100)) * 40;

						fSpeed = (float)(rand() % 2 + 1);

						//移動量	
						rot.x = ((float)(rand() % 629 - 314) / 100);
						rot.z = ((float)(rand() % 629 - 314) / 100);

						move[0][PARTICLETYPE_LEVITATION].x = sinf(rot.x) * fSpeed;
						move[0][PARTICLETYPE_LEVITATION].z = cosf(rot.z) * fSpeed;
						move[0][PARTICLETYPE_LEVITATION].y = -1.5f;

						SetEffect(EFFECT_TYPE_NORMAL, EFFECT_TEX_WING002, pos[0], move[0][PARTICLETYPE_LEVITATION], COLOR_LIMEGREEN, 100, 15);
					}
					break;

					//燃焼=============================================================================
				case PARTICLETYPE_COMBUSTION:
					for (int nCntAppear = 0; nCntAppear < 3; nCntAppear++)
					{
						//位置の設定
						pos[0].x = g_aParticle[nCntPlayerType][nCntParticle].pos.x + sinf((float)(rand() % 100)) * 50;
						pos[0].y = g_aParticle[nCntPlayerType][nCntParticle].pos.y;
						pos[0].z = g_aParticle[nCntPlayerType][nCntParticle].pos.z + cosf((float)(rand() % 100)) * 50;

						fSpeed = (float)(rand() % 3 + 1);

						//移動量						　　　
						rot.z = ((float)(rand() % 629 - 314) / 100);
						rot.y = ((float)(rand() % 629 - 314) / 500);

						move[0][PARTICLETYPE_COMBUSTION].x = sinf(rot.z) * fSpeed;
						move[0][PARTICLETYPE_COMBUSTION].z = cosf(rot.z) * fSpeed;
						move[0][PARTICLETYPE_COMBUSTION].y = (float)(rand() % 2 + 1) * 1.8f;
						//move[0][PARTICLETYPE_COMBUSTION].y = cosf(rot.y) * fSpeed;

						//半径の設定
						fRadius = g_aParticle[nCntPlayerType][nCntParticle].fRadius;

						SetEffect(EFFECT_TYPE_NORMAL, EFFECT_TEX_DIAMOND, pos[0], move[0][PARTICLETYPE_COMBUSTION], COLOR_RED, 100, 20);
					}
					for (int nCntAppear = 0; nCntAppear < 2; nCntAppear++)
					{
						//位置の設定
						pos[1].x = g_aParticle[nCntPlayerType][nCntParticle].pos.x + sinf((float)(rand() % 100)) * 30;
						pos[1].z = g_aParticle[nCntPlayerType][nCntParticle].pos.z + cosf((float)(rand() % 100)) * 30;

						fSpeed = (float)(rand() % 2 + 1);

						//移動量						　　　
						rot.z = ((float)(rand() % 629 - 314) / 100);

						move[1][PARTICLETYPE_COMBUSTION].x = sinf(rot.z) * fSpeed * 0.5f;
						move[1][PARTICLETYPE_COMBUSTION].z = cosf(rot.z) * fSpeed * 0.5f;
						move[1][PARTICLETYPE_COMBUSTION].y = (float)(rand() % 2 + 1) * 0.9f;

						//半径の設定
						fRadius = g_aParticle[nCntPlayerType][nCntParticle].fRadius;

						SetEffect(EFFECT_TYPE_NORMAL, EFFECT_TEX_CIRCLE, pos[1], move[1][PARTICLETYPE_COMBUSTION], COLOR_ORANGE, 80, 10);
					}
					break;

					//洪水、氾濫=======================================================================
				case PARTICLETYPE_FLOOD:
					for (int nCntAppear = 0; nCntAppear < MAX_APPEAR; nCntAppear++)
					{
						pos[0] = g_aParticle[nCntPlayerType][nCntParticle].pos;
						move[0][PARTICLETYPE_FLOOD].x = (sinf(float(rand() % MAX_ANGRE - MAX_ANGRE2 / MAX_ONEHUNDRED))) * (float(rand() % MAX_MOVE - MAX_MOVE2 / MAX_ONEHUNDRED));
						move[0][PARTICLETYPE_FLOOD].y = (cosf(float(rand() % MAX_ANGRE - MAX_ANGRE2 / MAX_ONEHUNDRED))) * (float(rand() % MAX_MOVE - MAX_MOVE2 / MAX_ONEHUNDRED));
						move[0][PARTICLETYPE_FLOOD].z = (tanf(float(rand() % MAX_ANGRE - MAX_ANGRE2 / MAX_ONEHUNDRED))) * (float(rand() % MAX_MOVE - MAX_MOVE2 / MAX_ONEHUNDRED));
						SetEffect(EFFECT_TYPE_NORMAL, EFFECT_TEX_CIRCLE, pos[0], move[0][PARTICLETYPE_FLOOD], COLOR_RED, 10, 25);
					}
					break;

					//フラッシュ=======================================================================
				case PARTICLETYPE_FLASH:
					for (int nCntAppear = 0; nCntAppear < MAX_APPEAR; nCntAppear++)
					{
						theta = 2.0f * D3DX_PI * RandFloat(); 
						phi = acosf(2.0f * RandFloat() - 1.0f);

						//位置の設定
						pos[0].x = sinf(phi) * cosf(theta);
						pos[0].y = sinf(phi) * sinf(theta);
						pos[0].z = cosf(phi);

						fSpeed = 1.0f + RandFloat() * 2.0f;

						//pos[0].x = g_aParticle[nCntPlayerType][nCntParticle].pos.x + (sinf((float)(rand() % 629 - 314 / 100))) * (float)(rand() % 40 + 1);
						//pos[0].y = g_aParticle[nCntPlayerType][nCntParticle].pos.y + (cosf((float)(rand() % 629 - 314 / 100))) * (float)(rand() % 40 + 1);
						//pos[0].z = g_aParticle[nCntPlayerType][nCntParticle].pos.z + (sinf((float)(rand() % 629 - 314 / 100))) * (float)(rand() % 40 + 1);

						//位置の設定
						pos[1].x = g_aParticle[nCntPlayerType][nCntParticle].pos.x + (sinf((float)(rand() % 629 - 314 / 100))) * (float)(rand() % 45 + 1);
						pos[1].y = g_aParticle[nCntPlayerType][nCntParticle].pos.y + (cosf((float)(rand() % 629 - 314 / 100))) * (float)(rand() % 45 + 1);
						pos[1].z = g_aParticle[nCntPlayerType][nCntParticle].pos.z + (sinf((float)(rand() % 629 - 314 / 100))) * (float)(rand() % 45 + 1);

						/*rot.x = ((float)(rand() % 629 - 314) / pos[0].x);
						rot.y = ((float)(rand() % 629 - 314) / pos[0].y);
						rot.z = ((float)(rand() % 629 - 314) / pos[0].z);*/

						move[0][PARTICLETYPE_FLASH].x = (pos[0].x) * 3;
						move[0][PARTICLETYPE_FLASH].y = (pos[0].y) * 3;
						move[0][PARTICLETYPE_FLASH].z = (pos[0].z) * 3;

						move[1][PARTICLETYPE_FLASH].x = (sinf(pos[1].y)) * 3;
						move[1][PARTICLETYPE_FLASH].y = (cosf(pos[1].y)) * 3;

						move[2][PARTICLETYPE_FLASH].y = (cosf(pos[1].y)) * 3;
						move[2][PARTICLETYPE_FLASH].z = (sinf(pos[1].y)) * 3;

						SetEffect(EFFECT_TYPE_FLASH, EFFECT_TEX_CIRCLE, pos[0], move[0][PARTICLETYPE_FLASH], COLOR_WHITE, 100, 12);
						SetEffect(EFFECT_TYPE_FLASH, EFFECT_TEX_CIRCLE, pos[0], move[0][PARTICLETYPE_FLASH], COLOR_YELLOW, 100, 20);
						SetEffect(EFFECT_TYPE_FLASH, EFFECT_TEX_DIAMOND, pos[1], move[1][PARTICLETYPE_FLASH], COLOR_WHITE, 100, 8);
						SetEffect(EFFECT_TYPE_FLASH, EFFECT_TEX_DIAMOND, pos[1], move[1][PARTICLETYPE_FLASH], COLOR_YELLOW, 100, 12);
						SetEffect(EFFECT_TYPE_FLASH, EFFECT_TEX_DIAMOND, pos[1], move[2][PARTICLETYPE_FLASH], COLOR_WHITE, 100, 8);
						SetEffect(EFFECT_TYPE_FLASH, EFFECT_TEX_DIAMOND, pos[1], move[2][PARTICLETYPE_FLASH], COLOR_YELLOW, 100, 12);
					}
					//move.x = (sinf((float)(rand() % ANGLE_DOUBLE - MAX_ANGLE) / ANGLE_ADJUST) * (float)(rand() % MOVE_PARTICLE) / MOVE_ADJUST + MOVE_MIN) * SPEED_EXPLOSION;	// 移動量X
					//move.y = (cosf((float)(rand() % ANGLE_DOUBLE - MAX_ANGLE) / ANGLE_ADJUST) * (float)(rand() % MOVE_PARTICLE) / MOVE_ADJUST + MOVE_MIN) * SPEED_EXPLOSION;	// 移動量Y
					//move.z = (cosf((float)(rand() % ANGLE_DOUBLE - MAX_ANGLE) / ANGLE_ADJUST) * (float)(rand() % MOVE_PARTICLE) / MOVE_ADJUST + MOVE_MIN) * SPEED_EXPLOSION;	// 移動量Z
					//col = g_aParticle[nCntPlayerType][nCntParticle].col;
					//fRadius = SIZE_EXPLOSION;
					//nLife = ELIFE_EXPLOSION;
					//pos = g_aParticle[nCntPlayerType][nCntParticle].pos;	// 発生位置
					//SetEffect(pos, move, col, EFFECTTYPE_EXPLOSION, 0.0f, nLife, fRadius);
					//ANGLE_DOUBLE→629;
					//MAX_ANGLE→314;
					//ANGLE_ADJUST→100.0f;
					//(float)(rand() % MOVE_PARTICLE) / MOVE_ADJUST + MOVE_MIN)* SPEED_EXPLOSIONは移動量を管理しています
					break;

					//火球=============================================================================
				case PARTICLETYPE_FIREBALL:
					//for (int nCntAppear = 0; nCntAppear < MAX_APPEAR; nCntAppear++)
					//{
					//	//位置の設定
					//	pos[0].x = g_aParticle[nCntAppear].pos.x + sinf((float)(rand() % 70));
					//	pos[0].y = g_aParticle[nCntAppear].pos.y + 30;
					//	pos[0].z = g_aParticle[nCntAppear].pos.z + sinf((float)(rand() % 70));

					//	pos[1].x = g_aParticle[nCntAppear].pos.x + sinf((float)(rand() % 40));
					//	pos[1].y = g_aParticle[nCntAppear].pos.y + 30;
					//	pos[1].z = g_aParticle[nCntAppear].pos.z + sinf((float)(rand() % 40));

					//	//移動量
					//	move[0][PARTICLETYPE_FIREBALL].x = sinf((float)(rand() % 629 - 314)) * 2.0f;
					//	move[0][PARTICLETYPE_FIREBALL].y = cosf((float)(rand() % 629 - 314)) * 2.4f;
					//	move[0][PARTICLETYPE_FIREBALL].z = sinf((float)(rand() % 629 - 314)) * 2.0f;

					//	move[1][PARTICLETYPE_FIREBALL].x = sinf((float)(rand() % 629 - 314)) * 1.0f;
					//	move[1][PARTICLETYPE_FIREBALL].y = cosf((float)(rand() % 629 - 314)) * 1.0f;
					//	move[1][PARTICLETYPE_FIREBALL].z = sinf((float)(rand() % 629 - 314)) * 1.0f;

					//	SetEffect(EFFECT_TYPE_NORMAL, EFFECT_TEX_DIAMOND, pos[0], move[0][PARTICLETYPE_FIREBALL], COLOR_RED, 10, 15);
					//	SetEffect(EFFECT_TYPE_NORMAL, EFFECT_TEX_CIRCLE, pos[1], move[1][PARTICLETYPE_FIREBALL], COLOR_ORANGE, 10, 10);
					//}
					break;

					//太陽の動きを遅延する=============================================================
				case PARTICLETYPE_SUNSETDELAY:
					for (int nCntAppear = 0; nCntAppear < MAX_APPEAR; nCntAppear++)
					{
						pos[0] = g_aParticle[nCntPlayerType][nCntParticle].pos;
						move[0][PARTICLETYPE_SUNSETDELAY].x = (sinf(float(rand() % MAX_ANGRE - MAX_ANGRE2 / MAX_ONEHUNDRED))) * (float(rand() % MAX_MOVE - MAX_MOVE2 / MAX_ONEHUNDRED));
						move[0][PARTICLETYPE_SUNSETDELAY].y = (cosf(float(rand() % MAX_ANGRE - MAX_ANGRE2 / MAX_ONEHUNDRED))) * (float(rand() % MAX_MOVE - MAX_MOVE2 / MAX_ONEHUNDRED));
						move[0][PARTICLETYPE_SUNSETDELAY].z = (cosf(float(rand() % MAX_ANGRE - MAX_ANGRE2 / MAX_ONEHUNDRED))) * (float(rand() % MAX_MOVE - MAX_MOVE2 / MAX_ONEHUNDRED));
						SetEffect(EFFECT_TYPE_NORMAL, EFFECT_TEX_CIRCLE, pos[0], move[0][PARTICLETYPE_SUNSETDELAY], COLOR_RED, 10, 25);
					}
					break;

					//雨乞い===========================================================================
				case PARTICLETYPE_RAINPRAY:
					for (int nCntAppear = 0; nCntAppear < 15; nCntAppear++)
					{
						//位置の設定
						pos[0].x = g_aParticle[nCntPlayerType][nCntParticle].pos.x + sinf((float)(rand() % 500)) * 200;
						pos[0].y = g_aParticle[nCntPlayerType][nCntParticle].pos.y + 300.0f;
						pos[0].z = g_aParticle[nCntPlayerType][nCntParticle].pos.z + cosf((float)(rand() % 500)) * 200;

						fSpeed = (float)(rand() % 2 + 1);

						//移動量	
						rot.x = ((float)(rand() % 629 - 314) / 100);
						rot.z = ((float)(rand() % 629 - 314) / 100);

						move[0][PARTICLETYPE_RAINPRAY].y = -(float)(rand() % 10) - 2;

						SetEffect(EFFECT_TYPE_RAIN, EFFECT_TEX_CIRCLE, pos[0], move[0][PARTICLETYPE_RAINPRAY], COLOR_BLUE, (rand() % 200) + 100, (float)(rand() % 7));
						SetEffect(EFFECT_TYPE_RAIN, EFFECT_TEX_CIRCLE, pos[0], move[0][PARTICLETYPE_RAINPRAY], COLOR_HISUI, (rand() % 200) + 100, (float)(rand() % 4));
					}
					break;

					//凍結=============================================================================
				case PARTICLETYPE_FREEZE:
					for (int nCntAppear = 0; nCntAppear < 3; nCntAppear++)
					{
						//位置の設定
						pos[0].x = g_aParticle[nCntPlayerType][nCntParticle].pos.x + (float)(rand() % 20 - 10);
						pos[0].y = g_aParticle[nCntPlayerType][nCntParticle].pos.y + (float)(rand() % 20 - 10);
						pos[0].z = g_aParticle[nCntPlayerType][nCntParticle].pos.z + (float)(rand() % 20 - 10);

						fSpeed = (float)(rand() % 2 + 1);

						//移動量						　　　
						rot.z = ((float)(rand() % 629 - 314) / 100);
						move[0][PARTICLETYPE_FREEZE].x = sinf(rot.z) * fSpeed;
						move[0][PARTICLETYPE_FREEZE].z = cosf(rot.z) * fSpeed;

						//半径の設定
						fRadius = g_aParticle[nCntPlayerType][nCntParticle].fRadius;

						SetEffect(EFFECT_TYPE_NORMAL, EFFECT_TEX_CIRCLE, pos[0], move[0][PARTICLETYPE_FREEZE], COLOR_CYAN, 100, 5);
					}
					for (int nCntAppear = 0; nCntAppear < 3; nCntAppear++)
					{
						//位置の設定
						pos[1].x = g_aParticle[nCntPlayerType][nCntParticle].pos.x + (float)(rand() % 20 - 10);
						pos[1].y = g_aParticle[nCntPlayerType][nCntParticle].pos.y + (float)(rand() % 20 - 10);
						pos[1].z = g_aParticle[nCntPlayerType][nCntParticle].pos.z + (float)(rand() % 20 - 10);

						fSpeed = (float)(rand() % 2 + 1);

						//移動量						　　　
						rot.z = ((float)(rand() % 629 - 314) / 100);

						move[1][PARTICLETYPE_FREEZE].x = sinf(rot.z) * fSpeed;
						move[1][PARTICLETYPE_FREEZE].z = cosf(rot.z) * fSpeed;

						//半径の設定
						fRadius = g_aParticle[nCntPlayerType][nCntParticle].fRadius;

						SetEffect(EFFECT_TYPE_NORMAL, EFFECT_TEX_CIRCLE, pos[1], move[1][PARTICLETYPE_FREEZE], COLOR_WHITE, 80, 3);
					}
					break;

					//成長(植物など)===================================================================
				case PARTICLETYPE_GROWTH:
						//位置の設定
						pos[0].x = g_aParticle[nCntPlayerType][nCntParticle].pos.x + sinf((float)(rand() % 100)) * 50;
						pos[0].y = g_aParticle[nCntPlayerType][nCntParticle].pos.y;
						pos[0].z = g_aParticle[nCntPlayerType][nCntParticle].pos.z + cosf((float)(rand() % 100)) * 50;

						fSpeed = (float)(rand() % 2 + 1);

						//移動量						　　　
						rot.z = ((float)(rand() % 629 - 314) / 100);
						rot.y = ((float)(rand() % 629 - 314) / 500);

						move[0][PARTICLETYPE_GROWTH].x = sinf(rot.z) * fSpeed;
						move[0][PARTICLETYPE_GROWTH].z = cosf(rot.z) * fSpeed;
						move[0][PARTICLETYPE_GROWTH].y = (float)(rand() % 2 + 1) * 1.2f;

						//半径の設定
						fRadius = g_aParticle[nCntPlayerType][nCntParticle].fRadius;

						SetEffect(EFFECT_TYPE_GROWTH, EFFECT_TEX_DIAMOND, pos[0], move[0][PARTICLETYPE_GROWTH], COLOR_GREEN, 60, 20);
					break;

					//加速=============================================================================
				case PARTICLETYPE_ACCELERATION:
					pos[0] = g_aParticle[nCntPlayerType][nCntParticle].pos;

					move[0][PARTICLETYPE_ACCELERATION].y = (cosf(float(rand() % MAX_ANGRE - MAX_ANGRE2 / MAX_ONEHUNDRED))) * (float(rand() % MAX_MOVE3 - MAX_MOVE3 / MAX_ONEHUNDRED));

					move[1][PARTICLETYPE_ACCELERATION].y = (cosf(float(rand() % MAX_ANGRE - MAX_ANGRE2 / MAX_ONEHUNDRED))) * (float(rand() % MAX_MOVE3 - MAX_MOVE3 / MAX_ONEHUNDRED));
					move[1][PARTICLETYPE_ACCELERATION].x = (sinf(float(rand() % MAX_ANGRE - MAX_ANGRE2 / MAX_ONEHUNDRED))) * (float(rand() % MAX_MOVE3 - MAX_MOVE4 / MAX_ONEHUNDRED));
					move[1][PARTICLETYPE_ACCELERATION].z = (cosf(float(rand() % MAX_ANGRE - MAX_ANGRE2 / MAX_ONEHUNDRED))) * (float(rand() % MAX_MOVE3 - MAX_MOVE4 / MAX_ONEHUNDRED));

					SetEffect(EFFECT_TYPE_NORMAL, (EFFECT_TEX(rand() % 2 + 2)), D3DXVECTOR3(pos[0].x, pos[0].y + (rand() % 20), pos[0].z), move[0][PARTICLETYPE_ACCELERATION], COLOR_CYAN, 20, 10);
					SetEffect(EFFECT_TYPE_NORMAL, (EFFECT_TEX(rand() % 2 + 2)), D3DXVECTOR3(pos[0].x, pos[0].y + (rand() % 20), pos[0].z), move[1][PARTICLETYPE_ACCELERATION], COLOR_LIMEGREEN, 20, 5);
					break;

					//時間の巻き戻し(回帰)=============================================================
				case PARTICLETYPE_TIMEREVERT:
					//位置の設定
					pos[0].x = g_aParticle[nCntPlayerType][nCntParticle].pos.x + sinf((float)(rand() % 100)) * 50;
					pos[0].y = g_aParticle[nCntPlayerType][nCntParticle].pos.y;
					pos[0].z = g_aParticle[nCntPlayerType][nCntParticle].pos.z + cosf((float)(rand() % 100)) * 50;

					fSpeed = (float)(rand() % 3 + 1);

					//移動量						　　　
					rot.z = ((float)(rand() % 629 - 314) / 100);

					move[0][PARTICLETYPE_TIMEREVERT].x = sinf(rot.z) * fSpeed;
					move[0][PARTICLETYPE_TIMEREVERT].z = cosf(rot.z) * fSpeed;
					move[0][PARTICLETYPE_TIMEREVERT].y = (float)(rand() % 2 + 1) * 1.0f;

					//半径の設定
					fRadius = g_aParticle[nCntPlayerType][nCntParticle].fRadius;

					SetEffect(EFFECT_TYPE_NORMAL, EFFECT_TEX_CIRCLE, pos[0], move[0][PARTICLETYPE_TIMEREVERT], COLOR_WHITE, 100, 5);
					break;
				}

				g_aParticle[nCntPlayerType][nCntParticle].nLife--;
				if (g_aParticle[nCntPlayerType][nCntParticle].nLife < 0)
				{
					g_aParticle[nCntPlayerType][nCntParticle].bUse = false;
				}
				nCountParticle++;
			}
		}
	}
	PrintDebugProc("使用しているパーティクル : %d\n", nCountParticle++);
}

//========================================================================
// 描画
//========================================================================
void DrawParticle(void)
{

}

//========================================================================
// パーティクルの設定
//========================================================================
void SetParticle(D3DXVECTOR3 pos, int nLife, PARTICLETYPE type, int nIdx)
{
	for (int nCntPlayerType = 0; nCntPlayerType < MAX_PLAYER; nCntPlayerType++)
	{
		for (int nCntParticle = 0; nCntParticle < MAX_PARTICLE; nCntParticle++)
		{
			//使われていなければ
			if (g_aParticle[nCntPlayerType][nCntParticle].bUse == false)
			{
				g_aParticle[nCntPlayerType][nCntParticle].pos = pos;
				g_aParticle[nCntPlayerType][nCntParticle].nLife = nLife;
				g_aParticle[nCntPlayerType][nCntParticle].Type = type;
				g_aParticle[nCntPlayerType][nCntParticle].bUse = true;
				break;
			}
		}
	}
}

//========================================================================
//0.0～1.0の乱数
float RandFloat(void)
{
	return (float)rand() / (float)RAND_MAX;
}