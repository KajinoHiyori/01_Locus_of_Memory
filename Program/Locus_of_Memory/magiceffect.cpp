//========================================================================
// 
// パーティクル描画[magiceffct.cpp]
//
//========================================================================

#include "main.h"
#include "magiceffect.h"
#include "effect.h"
#include "input.h"
#include "debugproc.h"
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
	D3DXMATRIX g_mtxWorldMagicEffect;	//ワールドマトリックス
	float fRadius;			//半径
	int nLife;				//寿命（色）
	MAGICEF_TYPE Type;		//種類
	bool bUse;				//使用しているか
}PARTICLE;

//グローバル変数
LPDIRECT3DTEXTURE9 g_pTextureBuffMagicEffect = NULL;
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffMagicEffect = NULL;
PARTICLE g_aMagicEffect[MAX_PARTICLE];

//========================================================================
// 初期化
//========================================================================
void InitMagicEffect(void)
{
	for (int nCntMagicEffect = 0; nCntMagicEffect < MAX_PARTICLE; nCntMagicEffect++)
	{
		g_aMagicEffect[nCntMagicEffect].bUse = false;
		g_aMagicEffect[nCntMagicEffect].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		g_aMagicEffect[nCntMagicEffect].move = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		g_aMagicEffect[nCntMagicEffect].nLife = 0;
		g_aMagicEffect[nCntMagicEffect].fRadius = 0;
		g_aMagicEffect[nCntMagicEffect].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
		g_aMagicEffect[nCntMagicEffect].Type = MAGICEF_TYPE_NONE;
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
	if (GetKeyboardTrigger(DIK_BACK) == true)
	{//時間
		SetMagicEffect(D3DXVECTOR3(0.0f, 0.0f, 0.0f), 120, MAGICEF_TYPE_TIMEREVERT);
	}

	if (GetKeyboardTrigger(DIK_1) == true)
	{//燃焼
		SetMagicEffect(D3DXVECTOR3(0.0f, 0.0f, 0.0f), 200, MAGICEF_TYPE_COMBUSTION);
	}

	if (GetKeyboardTrigger(DIK_2) == true)
	{//洪水、氾濫
		SetMagicEffect(D3DXVECTOR3(0.0f, 0.0f, 0.0f), 120, MAGICEF_TYPE_FLOOD);
	}

	if (GetKeyboardTrigger(DIK_3) == true)
	{//フラッシュ
		SetMagicEffect(D3DXVECTOR3(0.0f, 0.0f, 0.0f), 120, MAGICEF_TYPE_FLASH);
	}

	if (GetKeyboardTrigger(DIK_4) == true)
	{//火球
		SetMagicEffect(D3DXVECTOR3(0.0f, 0.0f, 0.0f), 120, MAGICEF_TYPE_FIREBALL);
	}

	if (GetKeyboardTrigger(DIK_5) == true)
	{//太陽の動きを遅延する
		SetMagicEffect(D3DXVECTOR3(0.0f, 0.0f, 0.0f), 120, MAGICEF_TYPE_SUNSETDELAY);
	}

	if (GetKeyboardTrigger(DIK_6) == true)
	{//雨乞い
		SetMagicEffect(D3DXVECTOR3(0.0f, 0.0f, 0.0f), 240, MAGICEF_TYPE_RAINPRAY);
	}

	if (GetKeyboardTrigger(DIK_7) == true)
	{//凍結
		SetMagicEffect(D3DXVECTOR3(0.0f, 0.0f, 0.0f), 100, MAGICEF_TYPE_FREEZE);
	}

	if (GetKeyboardTrigger(DIK_8) == true)
	{//成長(植物など)
		SetMagicEffect(D3DXVECTOR3(0.0f, 0.0f, 0.0f), 100, MAGICEF_TYPE_GROWTH);
	}

	if (GetKeyboardTrigger(DIK_9) == true)
	{//加速
		SetMagicEffect(D3DXVECTOR3(0.0f, 0.0f, 0.0f), 100, MAGICEF_TYPE_ACCELERATION);
	}

	if (GetKeyboardTrigger(DIK_0) == true)
	{//浮遊
		SetMagicEffect(D3DXVECTOR3(0.0f, 0.0f, 0.0f), 120, MAGICEF_TYPE_LEVITATION);
	}
#endif

	for (int nCntMagicEffect = 0; nCntMagicEffect < MAX_PARTICLE; nCntMagicEffect++)
	{
		if (g_aMagicEffect[nCntMagicEffect].bUse == true)
		{
			switch (g_aMagicEffect[nCntMagicEffect].Type)
			{
				//浮遊=============================================================================
			case MAGICEF_TYPE_LEVITATION:
				for (int nCntAppear = 0; nCntAppear < 1; nCntAppear++)
				{
					//位置の設定
					pos[0].x = g_aMagicEffect[nCntMagicEffect].pos.x + sinf((float)(rand() % 100)) * 40;
					pos[0].y = g_aMagicEffect[nCntMagicEffect].pos.y;
					pos[0].z = g_aMagicEffect[nCntMagicEffect].pos.z + cosf((float)(rand() % 100)) * 40;

					fSpeed = (float)(rand() % 2 + 1);

					//移動量	
					rot.x = ((float)(rand() % 629 - 314) / 100);
					rot.z = ((float)(rand() % 629 - 314) / 100);

					move[0][MAGICEF_TYPE_LEVITATION].x = sinf(rot.x) * fSpeed;
					move[0][MAGICEF_TYPE_LEVITATION].z = cosf(rot.z) * fSpeed;
					move[0][MAGICEF_TYPE_LEVITATION].y = -1.5f;

					SetEffect(EFFECT_TYPE_NORMAL, EFFECT_TEX_WING000, pos[0], move[0][MAGICEF_TYPE_LEVITATION], COLOR_WHITE, 100, 15);
				}
				for (int nCntAppear = 0; nCntAppear < 1; nCntAppear++)
				{
					//位置の設定
					pos[0].x = g_aMagicEffect[nCntMagicEffect].pos.x + sinf((float)(rand() % 100)) * 40;
					pos[0].y = g_aMagicEffect[nCntMagicEffect].pos.y;
					pos[0].z = g_aMagicEffect[nCntMagicEffect].pos.z + cosf((float)(rand() % 100)) * 40;

					fSpeed = (float)(rand() % 2 + 1);

					//移動量	
					rot.x = ((float)(rand() % 629 - 314) / 100);
					rot.z = ((float)(rand() % 629 - 314) / 100);

					move[0][MAGICEF_TYPE_LEVITATION].x = sinf(rot.x) * fSpeed;
					move[0][MAGICEF_TYPE_LEVITATION].z = cosf(rot.z) * fSpeed;
					move[0][MAGICEF_TYPE_LEVITATION].y = -1.5f;

					SetEffect(EFFECT_TYPE_NORMAL, EFFECT_TEX_WING001, pos[0], move[0][MAGICEF_TYPE_LEVITATION], COLOR_LIMEGREEN, 100, 15);
				}
				for (int nCntAppear = 0; nCntAppear < 1; nCntAppear++)
				{
					//位置の設定
					pos[0].x = g_aMagicEffect[nCntMagicEffect].pos.x + sinf((float)(rand() % 100)) * 40;
					pos[0].y = g_aMagicEffect[nCntMagicEffect].pos.y;
					pos[0].z = g_aMagicEffect[nCntMagicEffect].pos.z + cosf((float)(rand() % 100)) * 40;

					fSpeed = (float)(rand() % 2 + 1);

					//移動量	
					rot.x = ((float)(rand() % 629 - 314) / 100);
					rot.z = ((float)(rand() % 629 - 314) / 100);

					move[0][MAGICEF_TYPE_LEVITATION].x = sinf(rot.x) * fSpeed;
					move[0][MAGICEF_TYPE_LEVITATION].z = cosf(rot.z) * fSpeed;
					move[0][MAGICEF_TYPE_LEVITATION].y = -1.5f;

					SetEffect(EFFECT_TYPE_NORMAL, EFFECT_TEX_WING002, pos[0], move[0][MAGICEF_TYPE_LEVITATION], COLOR_LIMEGREEN, 100, 15);
				}
				break;

				//燃焼=============================================================================
			case MAGICEF_TYPE_COMBUSTION:
				for (int nCntAppear = 0; nCntAppear < 3; nCntAppear++)
				{
					//位置の設定
					pos[0].x = g_aMagicEffect[nCntMagicEffect].pos.x + sinf((float)(rand() % 100)) * 50;
					pos[0].y = g_aMagicEffect[nCntMagicEffect].pos.y;
					pos[0].z = g_aMagicEffect[nCntMagicEffect].pos.z + cosf((float)(rand() % 100)) * 50;

					fSpeed = (float)(rand() % 3 + 1);

					//移動量						　　　
					rot.z = ((float)(rand() % 629 - 314) / 100);
					rot.y = ((float)(rand() % 629 - 314) / 500);

					move[0][MAGICEF_TYPE_COMBUSTION].x = sinf(rot.z) * fSpeed;
					move[0][MAGICEF_TYPE_COMBUSTION].z = cosf(rot.z) * fSpeed;
					move[0][MAGICEF_TYPE_COMBUSTION].y = (float)(rand() % 2 + 1) * 1.8f;
					//move[0][MAGICEF_TYPE_COMBUSTION].y = cosf(rot.y) * fSpeed;

					//半径の設定
					fRadius = g_aMagicEffect[nCntMagicEffect].fRadius;

					SetEffect(EFFECT_TYPE_NORMAL, EFFECT_TEX_DIAMOND, pos[0], move[0][MAGICEF_TYPE_COMBUSTION], COLOR_RED, 100, 20);
				}
				for (int nCntAppear = 0; nCntAppear < 2; nCntAppear++)
				{
					//位置の設定
					pos[1].x = g_aMagicEffect[nCntMagicEffect].pos.x + sinf((float)(rand() % 100)) * 30;
					pos[1].z = g_aMagicEffect[nCntMagicEffect].pos.z + cosf((float)(rand() % 100)) * 30;

					fSpeed = (float)(rand() % 2 + 1);

					//移動量						　　　
					rot.z = ((float)(rand() % 629 - 314) / 100);

					move[1][MAGICEF_TYPE_COMBUSTION].x = sinf(rot.z) * fSpeed * 0.5f;
					move[1][MAGICEF_TYPE_COMBUSTION].z = cosf(rot.z) * fSpeed * 0.5f;
					move[1][MAGICEF_TYPE_COMBUSTION].y = (float)(rand() % 2 + 1) * 0.9f;

					//半径の設定
					fRadius = g_aMagicEffect[nCntMagicEffect].fRadius;

					SetEffect(EFFECT_TYPE_NORMAL, EFFECT_TEX_CIRCLE, pos[1], move[1][MAGICEF_TYPE_COMBUSTION], COLOR_ORANGE, 80, 10);
				}
				break;

				//洪水、氾濫=======================================================================
			case MAGICEF_TYPE_FLOOD:
				for (int nCntAppear = 0; nCntAppear < MAX_APPEAR; nCntAppear++)
				{
					pos[0] = g_aMagicEffect[nCntMagicEffect].pos;
					move[0][MAGICEF_TYPE_FLOOD].x = (sinf(float(rand() % MAX_ANGRE - MAX_ANGRE2 / MAX_ONEHUNDRED))) * (float(rand() % MAX_MOVE - MAX_MOVE2 / MAX_ONEHUNDRED));
					move[0][MAGICEF_TYPE_FLOOD].y = (cosf(float(rand() % MAX_ANGRE - MAX_ANGRE2 / MAX_ONEHUNDRED))) * (float(rand() % MAX_MOVE - MAX_MOVE2 / MAX_ONEHUNDRED));
					move[0][MAGICEF_TYPE_FLOOD].z = (tanf(float(rand() % MAX_ANGRE - MAX_ANGRE2 / MAX_ONEHUNDRED))) * (float(rand() % MAX_MOVE - MAX_MOVE2 / MAX_ONEHUNDRED));
					SetEffect(EFFECT_TYPE_NORMAL, EFFECT_TEX_CIRCLE, pos[0], move[0][MAGICEF_TYPE_FLOOD], COLOR_RED, 10, 25);
				}
				break;

				//フラッシュ=======================================================================
			case MAGICEF_TYPE_FLASH:
				for (int nCntAppear = 0; nCntAppear < MAX_APPEAR; nCntAppear++)
				{
					//位置の設定
					pos[0] = g_aMagicEffect[nCntAppear].pos;
					
					pos[1] = g_aMagicEffect[nCntAppear].pos;

					//移動量
					move[0][MAGICEF_TYPE_FLASH].x = (sinf((float)(rand() % 629 - 314) / 100.0f) * (float)(rand() % 10) / 2 + 1.0f) * 2.5f;	// 移動量X
					move[0][MAGICEF_TYPE_FLASH].y = (cosf((float)(rand() % 629 - 314) / 100.0f) * (float)(rand() % 10) / 2 + 1.0f) * 2.5f;	// 移動量Y
					move[0][MAGICEF_TYPE_FLASH].z = (cosf((float)(rand() % 629 - 314) / 100.0f) * (float)(rand() % 10) / 2 + 1.0f) * 2.5f;	// 移動量Z

					//移動量
					move[1][MAGICEF_TYPE_FLASH].x = (sinf((float)(rand() % 629 - 314) / 100.0f) * (float)(rand() % 10) / 2 + 1.0f) * 1.5f;	// 移動量X
					move[1][MAGICEF_TYPE_FLASH].y = (cosf((float)(rand() % 629 - 314) / 100.0f) * (float)(rand() % 10) / 2 + 1.0f) * 1.5f;	// 移動量Y
					move[1][MAGICEF_TYPE_FLASH].z = (cosf((float)(rand() % 629 - 314) / 100.0f) * (float)(rand() % 10) / 2 + 1.0f) * 1.5f;	// 移動量Z

					SetEffect(EFFECT_TYPE_NORMAL, EFFECT_TEX_DIAMOND, pos[0], move[0][MAGICEF_TYPE_FLASH], COLOR_YELLOW, 10, 15);
					SetEffect(EFFECT_TYPE_NORMAL, EFFECT_TEX_CIRCLE, pos[1], move[1][MAGICEF_TYPE_FLASH], COLOR_WHITE, 10, 10);
				}
				//move.x = (sinf((float)(rand() % ANGLE_DOUBLE - MAX_ANGLE) / ANGLE_ADJUST) * (float)(rand() % MOVE_PARTICLE) / MOVE_ADJUST + MOVE_MIN) * SPEED_EXPLOSION;	// 移動量X
				//move.y = (cosf((float)(rand() % ANGLE_DOUBLE - MAX_ANGLE) / ANGLE_ADJUST) * (float)(rand() % MOVE_PARTICLE) / MOVE_ADJUST + MOVE_MIN) * SPEED_EXPLOSION;	// 移動量Y
				//move.z = (cosf((float)(rand() % ANGLE_DOUBLE - MAX_ANGLE) / ANGLE_ADJUST) * (float)(rand() % MOVE_PARTICLE) / MOVE_ADJUST + MOVE_MIN) * SPEED_EXPLOSION;	// 移動量Z
				//col = g_aMagicEffect[nCntMagicEffect].col;
				//fRadius = SIZE_EXPLOSION;
				//nLife = ELIFE_EXPLOSION;
				//pos = g_aMagicEffect[nCntMagicEffect].pos;	// 発生位置
				//SetEffect(pos, move, col, EFFECTTYPE_EXPLOSION, 0.0f, nLife, fRadius);
				//ANGLE_DOUBLE→629;
				//MAX_ANGLE→314;
				//ANGLE_ADJUST→100.0f;
				//(float)(rand() % MOVE_PARTICLE) / MOVE_ADJUST + MOVE_MIN)* SPEED_EXPLOSIONは移動量を管理しています
				break;

				//火球=============================================================================
			case MAGICEF_TYPE_FIREBALL:
				for (int nCntAppear = 0; nCntAppear < MAX_APPEAR; nCntAppear++)
				{
					//位置の設定
					pos[0].x = g_aMagicEffect[nCntAppear].pos.x + sinf((float)(rand() % 70));
					pos[0].y = g_aMagicEffect[nCntAppear].pos.y + 30;
					pos[0].z = g_aMagicEffect[nCntAppear].pos.z + sinf((float)(rand() % 70));

					pos[1].x = g_aMagicEffect[nCntAppear].pos.x + sinf((float)(rand() % 40));
					pos[1].y = g_aMagicEffect[nCntAppear].pos.y + 30;
					pos[1].z = g_aMagicEffect[nCntAppear].pos.z + sinf((float)(rand() % 40));

					//移動量
					move[0][MAGICEF_TYPE_FIREBALL].x = sinf((float)(rand() % 629 - 314)) * 2.0f;
					move[0][MAGICEF_TYPE_FIREBALL].y = cosf((float)(rand() % 629 - 314)) * 2.4f;
					move[0][MAGICEF_TYPE_FIREBALL].z = sinf((float)(rand() % 629 - 314)) * 2.0f;

					move[1][MAGICEF_TYPE_FIREBALL].x = sinf((float)(rand() % 629 - 314)) * 1.0f;
					move[1][MAGICEF_TYPE_FIREBALL].y = cosf((float)(rand() % 629 - 314)) * 1.0f;
					move[1][MAGICEF_TYPE_FIREBALL].z = sinf((float)(rand() % 629 - 314)) * 1.0f;

					SetEffect(EFFECT_TYPE_NORMAL, EFFECT_TEX_DIAMOND, pos[0], move[0][MAGICEF_TYPE_FIREBALL], COLOR_RED, 10, 15);
					SetEffect(EFFECT_TYPE_NORMAL, EFFECT_TEX_CIRCLE, pos[1], move[1][MAGICEF_TYPE_FIREBALL], COLOR_ORANGE, 10, 10);
				}
				break;

				//太陽の動きを遅延する=============================================================
			case MAGICEF_TYPE_SUNSETDELAY:
				for (int nCntAppear = 0; nCntAppear < MAX_APPEAR; nCntAppear++)
				{
					pos[0] = g_aMagicEffect[nCntMagicEffect].pos;
					move[0][MAGICEF_TYPE_SUNSETDELAY].x = (sinf(float(rand() % MAX_ANGRE - MAX_ANGRE2 / MAX_ONEHUNDRED))) * (float(rand() % MAX_MOVE - MAX_MOVE2 / MAX_ONEHUNDRED));
					move[0][MAGICEF_TYPE_SUNSETDELAY].y = (cosf(float(rand() % MAX_ANGRE - MAX_ANGRE2 / MAX_ONEHUNDRED))) * (float(rand() % MAX_MOVE - MAX_MOVE2 / MAX_ONEHUNDRED));
					move[0][MAGICEF_TYPE_SUNSETDELAY].z = (cosf(float(rand() % MAX_ANGRE - MAX_ANGRE2 / MAX_ONEHUNDRED))) * (float(rand() % MAX_MOVE - MAX_MOVE2 / MAX_ONEHUNDRED));
					SetEffect(EFFECT_TYPE_NORMAL, EFFECT_TEX_CIRCLE, pos[0], move[0][MAGICEF_TYPE_SUNSETDELAY], COLOR_RED, 10, 25);
				}
				break;

				//雨乞い===========================================================================
			case MAGICEF_TYPE_RAINPRAY:
				for (int nCntAppear = 0; nCntAppear < 15; nCntAppear++)
				{
					//位置の設定
					pos[0].x = g_aMagicEffect[nCntMagicEffect].pos.x + sinf((float)(rand() % 500)) * 200;
					pos[0].y = g_aMagicEffect[nCntMagicEffect].pos.y + 300.0f;
					pos[0].z = g_aMagicEffect[nCntMagicEffect].pos.z + cosf((float)(rand() % 500)) * 200;

					fSpeed = (float)(rand() % 2 + 1);

					//移動量	
					rot.x = ((float)(rand() % 629 - 314) / 100);
					rot.z = ((float)(rand() % 629 - 314) / 100);

					move[0][MAGICEF_TYPE_RAINPRAY].y = -(float)(rand() % 10) - 2;

					SetEffect(EFFECT_TYPE_RAIN, EFFECT_TEX_CIRCLE, pos[0], move[0][MAGICEF_TYPE_RAINPRAY], COLOR_BLUE, (rand() % 200) + 100, (float)(rand() % 7));
					SetEffect(EFFECT_TYPE_RAIN, EFFECT_TEX_CIRCLE, pos[0], move[0][MAGICEF_TYPE_RAINPRAY], COLOR_HISUI, (rand() % 200) + 100, (float)(rand() % 4));
				}
				break;

				//凍結=============================================================================
			case MAGICEF_TYPE_FREEZE:
				for (int nCntAppear = 0; nCntAppear < 3; nCntAppear++)
				{
					//位置の設定
					pos[0].x = g_aMagicEffect[nCntMagicEffect].pos.x + (float)(rand() % 20 - 10);
					pos[0].y = g_aMagicEffect[nCntMagicEffect].pos.y + (float)(rand() % 20 - 10);

					fSpeed = (float)(rand() % 2 + 1);

					//移動量						　　　
					rot.z = ((float)(rand() % 629 - 314) / 100);
					move[0][MAGICEF_TYPE_FREEZE].x = sinf(rot.z) * fSpeed;
					move[0][MAGICEF_TYPE_FREEZE].z = cosf(rot.z) * fSpeed;

					//半径の設定
					fRadius = g_aMagicEffect[nCntMagicEffect].fRadius;

					SetEffect(EFFECT_TYPE_NORMAL, EFFECT_TEX_CIRCLE, pos[0], move[0][MAGICEF_TYPE_FREEZE], COLOR_CYAN, 100, 5);
				}
				for (int nCntAppear = 0; nCntAppear < 3; nCntAppear++)
				{
					//位置の設定
					pos[1].x = g_aMagicEffect[nCntMagicEffect].pos.x + (float)(rand() % 20 - 10);
					pos[1].y = g_aMagicEffect[nCntMagicEffect].pos.y + (float)(rand() % 20 - 10);

					fSpeed = (float)(rand() % 2 + 1);

					//移動量						　　　
					rot.z = ((float)(rand() % 629 - 314) / 100);

					move[1][MAGICEF_TYPE_FREEZE].x = sinf(rot.z) * fSpeed;
					move[1][MAGICEF_TYPE_FREEZE].z = cosf(rot.z) * fSpeed;

					//半径の設定
					fRadius = g_aMagicEffect[nCntMagicEffect].fRadius;

					SetEffect(EFFECT_TYPE_NORMAL, EFFECT_TEX_CIRCLE, pos[1], move[1][MAGICEF_TYPE_FREEZE], COLOR_WHITE, 80, 3);
				}
				break;

				//成長(植物など)===================================================================
			case MAGICEF_TYPE_GROWTH:
				for (int nCntAppear = 0; nCntAppear < 2; nCntAppear++)
				{
					//位置の設定
					pos[0].x = g_aMagicEffect[nCntMagicEffect].pos.x + sinf((float)(rand() % 100)) * 50;
					pos[0].y = g_aMagicEffect[nCntMagicEffect].pos.y;
					pos[0].z = g_aMagicEffect[nCntMagicEffect].pos.z + cosf((float)(rand() % 100)) * 50;

					fSpeed = (float)(rand() % 2 + 1);

					//移動量						　　　
					rot.z = ((float)(rand() % 629 - 314) / 100);
					rot.y = ((float)(rand() % 629 - 314) / 500);

					move[0][MAGICEF_TYPE_GROWTH].x = sinf(rot.z) * fSpeed;
					move[0][MAGICEF_TYPE_GROWTH].z = cosf(rot.z) * fSpeed;
					move[0][MAGICEF_TYPE_GROWTH].y = (float)(rand() % 2 + 1) * 1.2f;

					//半径の設定
					fRadius = g_aMagicEffect[nCntMagicEffect].fRadius;

					SetEffect(EFFECT_TYPE_GROWTH, EFFECT_TEX_DIAMOND, pos[0], move[0][MAGICEF_TYPE_GROWTH], COLOR_GREEN, 60, 30);
				}
				break;

				//加速=============================================================================
			case MAGICEF_TYPE_ACCELERATION:
					pos[0] = g_aMagicEffect[nCntMagicEffect].pos;
					
					move[0][MAGICEF_TYPE_ACCELERATION].y = (cosf(float(rand() % MAX_ANGRE - MAX_ANGRE2 / MAX_ONEHUNDRED))) * (float(rand() % MAX_MOVE3 - MAX_MOVE3 / MAX_ONEHUNDRED));
					
					move[1][MAGICEF_TYPE_ACCELERATION].y = (cosf(float(rand() % MAX_ANGRE - MAX_ANGRE2 / MAX_ONEHUNDRED))) * (float(rand() % MAX_MOVE3 - MAX_MOVE3 / MAX_ONEHUNDRED));
					move[1][MAGICEF_TYPE_ACCELERATION].x = (sinf(float(rand() % MAX_ANGRE - MAX_ANGRE2 / MAX_ONEHUNDRED))) * (float(rand() % MAX_MOVE3 - MAX_MOVE4 / MAX_ONEHUNDRED));
					move[1][MAGICEF_TYPE_ACCELERATION].z = (cosf(float(rand() % MAX_ANGRE - MAX_ANGRE2 / MAX_ONEHUNDRED))) * (float(rand() % MAX_MOVE3 - MAX_MOVE4 / MAX_ONEHUNDRED));

					SetEffect(EFFECT_TYPE_NORMAL, (EFFECT_TEX(rand() % 2 + 2)), D3DXVECTOR3(pos[0].x, pos[0].y + (rand() % 20), pos[0].z), move[0][MAGICEF_TYPE_ACCELERATION], COLOR_CYAN, 20, 10);
					SetEffect(EFFECT_TYPE_NORMAL, (EFFECT_TEX(rand() % 2 + 2)), D3DXVECTOR3(pos[0].x, pos[0].y + (rand() % 20), pos[0].z), move[1][MAGICEF_TYPE_ACCELERATION], COLOR_LIMEGREEN, 20, 5);
				break;

				//時間の巻き戻し(回帰)=============================================================
			case MAGICEF_TYPE_TIMEREVERT:
					//位置の設定
					pos[0].x = g_aMagicEffect[nCntMagicEffect].pos.x + sinf((float)(rand() % 100)) * 50;
					pos[0].y = g_aMagicEffect[nCntMagicEffect].pos.y;
					pos[0].z = g_aMagicEffect[nCntMagicEffect].pos.z + cosf((float)(rand() % 100)) * 50;

					fSpeed = (float)(rand() % 3 + 1);

					//移動量						　　　
					rot.z = ((float)(rand() % 629 - 314) / 100);
					
					move[0][MAGICEF_TYPE_TIMEREVERT].x = sinf(rot.z) * fSpeed;
					move[0][MAGICEF_TYPE_TIMEREVERT].z = cosf(rot.z) * fSpeed;
					move[0][MAGICEF_TYPE_TIMEREVERT].y = (float)(rand() % 2 + 1) * 1.0f;

					//半径の設定
					fRadius = g_aMagicEffect[nCntMagicEffect].fRadius;

					SetEffect(EFFECT_TYPE_NORMAL, EFFECT_TEX_CIRCLE, pos[0], move[0][MAGICEF_TYPE_TIMEREVERT], COLOR_WHITE, 100, 5);
				break;
			}

			g_aMagicEffect[nCntMagicEffect].nLife--;
			if (g_aMagicEffect[nCntMagicEffect].nLife < 0)
			{
				g_aMagicEffect[nCntMagicEffect].bUse = false;
			}
			nCountMagicEffect++;
		}
	}
	PrintDebugProc("使用しているパーティクル : %d\n", nCountMagicEffect++);
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
void SetMagicEffect(D3DXVECTOR3 pos, int nLife, MAGICEF_TYPE type)
{
	for (int nCntMagicEffect = 0; nCntMagicEffect < MAX_PARTICLE; nCntMagicEffect++)
	{
		//使われていなければ
		if (g_aMagicEffect[nCntMagicEffect].bUse == false)
		{
			g_aMagicEffect[nCntMagicEffect].pos = pos;
			g_aMagicEffect[nCntMagicEffect].nLife = nLife;
			g_aMagicEffect[nCntMagicEffect].Type = type;
			g_aMagicEffect[nCntMagicEffect].bUse = true;
			break;
		}
	}

}