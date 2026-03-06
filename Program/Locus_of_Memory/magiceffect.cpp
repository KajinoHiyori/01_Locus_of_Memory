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

#define MAX_MAGIC_PARTICLE	(128)		//パーティクルの最大数
#define MAX_MAGIC_APPEAR	(5)		//粒子の最大数
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
PARTICLE g_aMagicEffect[MAX_MAGIC_PARTICLE];

//========================================================================
// 初期化
//========================================================================
void InitMagicEffect(void)
{
	for (int nCntMagicEffect = 0; nCntMagicEffect < MAX_MAGIC_PARTICLE; nCntMagicEffect++)
	{
		g_aMagicEffect[nCntMagicEffect].bUse = false;
		g_aMagicEffect[nCntMagicEffect].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		g_aMagicEffect[nCntMagicEffect].move = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
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
	if (GetKeyboardTrigger(DIK_M) == true)
	{//時間
		SetMagicEffect(D3DXVECTOR3(0.0f, 0.0f, 0.0f), MAGICEF_TYPE_GIMMICK);
	}

	if (GetKeyboardTrigger(DIK_N) == true)
	{//燃焼
		SetMagicEffect(D3DXVECTOR3(0.0f, 0.0f, 0.0f), MAGICEF_TYPE_MAGIC);
	}
#endif

	for (int nCntMagicEffect = 0; nCntMagicEffect < MAX_MAGIC_PARTICLE; nCntMagicEffect++)
	{
		if (g_aMagicEffect[nCntMagicEffect].bUse == true)
		{
			switch (g_aMagicEffect[nCntMagicEffect].Type)
			{
				//ギミック=========================================================================
			case MAGICEF_TYPE_GIMMICK:
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

					move[0][MAGICEF_TYPE_GIMMICK].x = sinf(rot.x) * fSpeed;
					move[0][MAGICEF_TYPE_GIMMICK].z = cosf(rot.z) * fSpeed;
					move[0][MAGICEF_TYPE_GIMMICK].y = -1.5f;

					SetEffect(EFFECT_TYPE_NORMAL, EFFECT_TEX_WING000, pos[0], move[0][MAGICEF_TYPE_GIMMICK], COLOR_WHITE, 100, 15);
				}
				break;
				//落ちている魔法===================================================================
			case MAGICEF_TYPE_MAGIC:
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

					move[0][MAGICEF_TYPE_MAGIC].x = sinf(rot.x) * fSpeed;
					move[0][MAGICEF_TYPE_MAGIC].z = cosf(rot.z) * fSpeed;
					move[0][MAGICEF_TYPE_MAGIC].y = -1.5f;

					SetEffect(EFFECT_TYPE_NORMAL, EFFECT_TEX_WING000, pos[0], move[0][MAGICEF_TYPE_MAGIC], COLOR_WHITE, 100, 15);
				}
				break;
			}
			nCountMagicEffect++;
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
void SetMagicEffect(D3DXVECTOR3 pos, MAGICEF_TYPE type)
{
	for (int nCntMagicEffect = 0; nCntMagicEffect < MAX_MAGIC_PARTICLE; nCntMagicEffect++)
	{
		//使われていなければ
		if (g_aMagicEffect[nCntMagicEffect].bUse == false)
		{
			g_aMagicEffect[nCntMagicEffect].pos = pos;
			g_aMagicEffect[nCntMagicEffect].Type = type;
			g_aMagicEffect[nCntMagicEffect].bUse = true;
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

	}
}