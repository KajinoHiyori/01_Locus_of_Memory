//===================================================================
//
// magic処理[magic.cpp]
// Author : 眞鍋快陸
// 
//===================================================================
#include "magic.h"
#include "player.h"
#include "debugproc.h"
#include "input.h"
#include "shadow.h"
#include "particle.h"

//マクロ定義
#define MAX_MAGIC			(128)		//魔法の最大数
#define MAX_DROPMAGIC		(32)		//落ちてる魔法の最大数
#define MAX_COMMAND			(3)			//コマンドの最大数
#define DROPMAGIC_RADIUS	(15.0f)		//落ちてる魔法の半径

//グローバル変数宣言
Magic g_aMagic[MAX_PLAYER][MAX_MAGIC];					//魔法の情報
DropMagic g_aDropMagic[MAX_DROPMAGIC];					//落ちてる魔法の情報
COMMANDTYPE g_aCommand[MAX_PLAYER][MAX_COMMAND];		//コマンドの情報
int nCntCommand[MAX_PLAYER] = {};

//魔法の初期化処理=============================
void InitMagic(void)
{
	for (int nCntPlayerType = 0; nCntPlayerType < MAX_PLAYER; nCntPlayerType++)
	{
		for (int nCntMagic = 0; nCntMagic < MAX_MAGIC; nCntMagic++)
		{
			g_aMagic[nCntPlayerType][nCntMagic].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
			g_aMagic[nCntPlayerType][nCntMagic].rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
			g_aMagic[nCntPlayerType][nCntMagic].move = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
			g_aMagic[nCntPlayerType][nCntMagic].mType = MAGICTYPE_NONE;
			g_aMagic[nCntPlayerType][nCntMagic].bUse = false;
			g_aMagic[nCntPlayerType][nCntMagic].nLife = 0;
			g_aMagic[nCntPlayerType][nCntMagic].nIdxShadow = -1;
		}
	}

	for (int nCntPlayerType = 0; nCntPlayerType < MAX_PLAYER; nCntPlayerType++)
	{
		for (int nCntCommand = 0; nCntCommand < MAX_COMMAND; nCntCommand++)
		{
			g_aCommand[nCntPlayerType][nCntCommand] = COMMANDTYPE_NONE;
		}
	}

	for (int nCntMagic = 0; nCntMagic < MAX_DROPMAGIC; nCntMagic++)
	{
		g_aDropMagic[nCntMagic].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		g_aDropMagic[nCntMagic].rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		g_aDropMagic[nCntMagic].oType = COMMANDOREDER_NONE;
		g_aDropMagic[nCntMagic].fRadius = DROPMAGIC_RADIUS;
		g_aDropMagic[nCntMagic].bUse = false;
	}

	SetMagicPosition(COMMANDOREDER_BBB, D3DXVECTOR3(0.0f, 0.0f, 0.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f));
}

//魔法の終了処理==============================
void UninitMagic(void)
{

}

//魔法の更新処理==============================
void UpdateMagic(void)
{
	for (int nCntPlayerType = 0; nCntPlayerType < MAX_PLAYER; nCntPlayerType++)
	{
		for (int nCntMagic = 0; nCntMagic < MAX_MAGIC; nCntMagic++)
		{
			if (g_aMagic[nCntPlayerType][nCntMagic].bUse == true)
			{

			}
		}
	}
}

//魔法の描画処理==============================
void DrawMagic(void)
{

}

//コマンド入力情報=============================
COMMANDOREDER PressCommand(int nIdx)
{
	if (g_aCommand[nIdx][nCntCommand[nIdx]] == COMMANDTYPE_NONE)
	{//コマンドが何も入力されていないとき
		if (GetJoypadTrigger(JOYKEY_B, nIdx) == true)
		{//B(赤)が入力された
			g_aCommand[nIdx][nCntCommand[nIdx]] = COMMANDTYPE_R;
		}
		else if (GetJoypadTrigger(JOYKEY_A, nIdx) == true)
		{//A(緑)が入力された
			g_aCommand[nIdx][nCntCommand[nIdx]] = COMMANDTYPE_G;
		}
		else if (GetJoypadTrigger(JOYKEY_X, nIdx) == true)
		{//X(青)が入力された
			g_aCommand[nIdx][nCntCommand[nIdx]] = COMMANDTYPE_B;
		}
		else if (GetJoypadTrigger(JOYKEY_Y, nIdx) == true)
		{//Y(黄)が入力された
			g_aCommand[nIdx][nCntCommand[nIdx]] = COMMANDTYPE_Y;
		}
	}
	if (g_aCommand[nIdx][nCntCommand[nIdx]] != COMMANDTYPE_NONE)
	{//コマンドが何かしら入力されたとき
		nCntCommand[nIdx]++;
	}

	if (nCntCommand[nIdx] == MAX_COMMAND)
	{//コマンドが三つ入力されたとき
		//浮遊-------------------------------------------------------------------------------------------------
		//緑緑緑
		if (g_aCommand[nIdx][0] == COMMANDTYPE_G && g_aCommand[nIdx][1] == COMMANDTYPE_G && g_aCommand[nIdx][2] == COMMANDTYPE_G)
		{
			for (int nCntCommand = 0; nCntCommand < MAX_COMMAND; nCntCommand++)
			{
				g_aCommand[nIdx][nCntCommand] = COMMANDTYPE_NONE;
			}

			return COMMANDOREDER_GGG;
		}

		//燃焼-------------------------------------------------------------------------------------------------
		//赤赤赤
		else if (g_aCommand[nIdx][0] == COMMANDTYPE_R && g_aCommand[nIdx][1] == COMMANDTYPE_R && g_aCommand[nIdx][2] == COMMANDTYPE_R)
		{
			for (int nCntCommand = 0; nCntCommand < MAX_COMMAND; nCntCommand++)
			{
				g_aCommand[nIdx][nCntCommand] = COMMANDTYPE_NONE;
			}

			return COMMANDOREDER_RRR;
		}

		//洪水、氾濫-------------------------------------------------------------------------------------------
		//青青青
		else if (g_aCommand[nIdx][0] == COMMANDTYPE_B && g_aCommand[nIdx][1] == COMMANDTYPE_B && g_aCommand[nIdx][2] == COMMANDTYPE_B)
		{
			for (int nCntCommand = 0; nCntCommand < MAX_COMMAND; nCntCommand++)
			{
				g_aCommand[nIdx][nCntCommand] = COMMANDTYPE_NONE;
			}

			return COMMANDOREDER_BBB;
		}

		//フラッシュ-------------------------------------------------------------------------------------------
		//黄黄黄
		else if (g_aCommand[nIdx][0] == COMMANDTYPE_Y && g_aCommand[nIdx][1] == COMMANDTYPE_Y && g_aCommand[nIdx][2] == COMMANDTYPE_Y)
		{
			for (int nCntCommand = 0; nCntCommand < MAX_COMMAND; nCntCommand++)
			{
				g_aCommand[nIdx][nCntCommand] = COMMANDTYPE_NONE;
			}

			return COMMANDOREDER_YYY;
		}

		//火球-------------------------------------------------------------------------------------------------
		//赤赤緑
		else if (g_aCommand[nIdx][0] == COMMANDTYPE_R && g_aCommand[nIdx][1] == COMMANDTYPE_R && g_aCommand[nIdx][2] == COMMANDTYPE_G)
		{
			for (int nCntCommand = 0; nCntCommand < MAX_COMMAND; nCntCommand++)
			{
				g_aCommand[nIdx][nCntCommand] = COMMANDTYPE_NONE;
			}

			return COMMANDOREDER_RRG;
		}

		//赤緑赤
		else if (g_aCommand[nIdx][0] == COMMANDTYPE_R && g_aCommand[nIdx][1] == COMMANDTYPE_G && g_aCommand[nIdx][2] == COMMANDTYPE_R)
		{
			for (int nCntCommand = 0; nCntCommand < MAX_COMMAND; nCntCommand++)
			{
				g_aCommand[nIdx][nCntCommand] = COMMANDTYPE_NONE;
			}

			return COMMANDOREDER_RGR;
		}

		//緑赤赤
		else if (g_aCommand[nIdx][0] == COMMANDTYPE_G && g_aCommand[nIdx][1] == COMMANDTYPE_R && g_aCommand[nIdx][2] == COMMANDTYPE_R)
		{
			for (int nCntCommand = 0; nCntCommand < MAX_COMMAND; nCntCommand++)
			{
				g_aCommand[nIdx][nCntCommand] = COMMANDTYPE_NONE;
			}

			return COMMANDOREDER_GRR;
		}
		//-----------------------------------------------------------------------------------------------------

		//太陽の動きを遅延する---------------------------------------------------------------------------------
		//赤黄黄
		else if (g_aCommand[nIdx][0] == COMMANDTYPE_R && g_aCommand[nIdx][1] == COMMANDTYPE_Y && g_aCommand[nIdx][2] == COMMANDTYPE_Y)
		{
			for (int nCntCommand = 0; nCntCommand < MAX_COMMAND; nCntCommand++)
			{
				g_aCommand[nIdx][nCntCommand] = COMMANDTYPE_NONE;
			}

			return COMMANDOREDER_RYY;
		}

		//黄赤黄
		else if (g_aCommand[nIdx][0] == COMMANDTYPE_Y && g_aCommand[nIdx][1] == COMMANDTYPE_R && g_aCommand[nIdx][2] == COMMANDTYPE_Y)
		{
			for (int nCntCommand = 0; nCntCommand < MAX_COMMAND; nCntCommand++)
			{
				g_aCommand[nIdx][nCntCommand] = COMMANDTYPE_NONE;
			}

			return COMMANDOREDER_YRY;
		}

		//黄黄赤
		else if (g_aCommand[nIdx][0] == COMMANDTYPE_Y && g_aCommand[nIdx][1] == COMMANDTYPE_Y && g_aCommand[nIdx][2] == COMMANDTYPE_R)
		{
			for (int nCntCommand = 0; nCntCommand < MAX_COMMAND; nCntCommand++)
			{
				g_aCommand[nIdx][nCntCommand] = COMMANDTYPE_NONE;
			}

			return COMMANDOREDER_YYR;
		}
		//-----------------------------------------------------------------------------------------------------

		//雨乞い-----------------------------------------------------------------------------------------------
		//青青緑
		else if (g_aCommand[nIdx][0] == COMMANDTYPE_B && g_aCommand[nIdx][1] == COMMANDTYPE_B && g_aCommand[nIdx][2] == COMMANDTYPE_G)
		{
			for (int nCntCommand = 0; nCntCommand < MAX_COMMAND; nCntCommand++)
			{
				g_aCommand[nIdx][nCntCommand] = COMMANDTYPE_NONE;
			}

			return COMMANDOREDER_BBG;
		}

		//青緑青
		else if (g_aCommand[nIdx][0] == COMMANDTYPE_B && g_aCommand[nIdx][1] == COMMANDTYPE_G && g_aCommand[nIdx][2] == COMMANDTYPE_B)
		{
			for (int nCntCommand = 0; nCntCommand < MAX_COMMAND; nCntCommand++)
			{
				g_aCommand[nIdx][nCntCommand] = COMMANDTYPE_NONE;
			}

			return COMMANDOREDER_BGB;
		}

		//緑青青
		else if (g_aCommand[nIdx][0] == COMMANDTYPE_G && g_aCommand[nIdx][1] == COMMANDTYPE_B && g_aCommand[nIdx][2] == COMMANDTYPE_B)
		{
			for (int nCntCommand = 0; nCntCommand < MAX_COMMAND; nCntCommand++)
			{
				g_aCommand[nIdx][nCntCommand] = COMMANDTYPE_NONE;
			}

			return COMMANDOREDER_GBB;
		}
		//-----------------------------------------------------------------------------------------------------

		//凍結-------------------------------------------------------------------------------------------------
		//青緑緑
		else if (g_aCommand[nIdx][0] == COMMANDTYPE_B && g_aCommand[nIdx][1] == COMMANDTYPE_G && g_aCommand[nIdx][2] == COMMANDTYPE_G)
		{
			for (int nCntCommand = 0; nCntCommand < MAX_COMMAND; nCntCommand++)
			{
				g_aCommand[nIdx][nCntCommand] = COMMANDTYPE_NONE;
			}

			return COMMANDOREDER_BGG;
		}

		//緑青緑
		else if (g_aCommand[nIdx][0] == COMMANDTYPE_G && g_aCommand[nIdx][1] == COMMANDTYPE_B && g_aCommand[nIdx][2] == COMMANDTYPE_G)
		{
			for (int nCntCommand = 0; nCntCommand < MAX_COMMAND; nCntCommand++)
			{
				g_aCommand[nIdx][nCntCommand] = COMMANDTYPE_NONE;
			}

			return COMMANDOREDER_GBG;
		}

		//緑緑青
		else if (g_aCommand[nIdx][0] == COMMANDTYPE_G && g_aCommand[nIdx][1] == COMMANDTYPE_G && g_aCommand[nIdx][2] == COMMANDTYPE_B)
		{
			for (int nCntCommand = 0; nCntCommand < MAX_COMMAND; nCntCommand++)
			{
				g_aCommand[nIdx][nCntCommand] = COMMANDTYPE_NONE;
			}

			return COMMANDOREDER_GGB;
		}
		//-----------------------------------------------------------------------------------------------------

		//成長(植物など)---------------------------------------------------------------------------------------
		//青黄黄
		else if (g_aCommand[nIdx][0] == COMMANDTYPE_B && g_aCommand[nIdx][1] == COMMANDTYPE_Y && g_aCommand[nIdx][2] == COMMANDTYPE_Y)
		{
			for (int nCntCommand = 0; nCntCommand < MAX_COMMAND; nCntCommand++)
			{
				g_aCommand[nIdx][nCntCommand] = COMMANDTYPE_NONE;
			}

			return COMMANDOREDER_BYY;
		}

		//黄青黄
		else if (g_aCommand[nIdx][0] == COMMANDTYPE_Y && g_aCommand[nIdx][1] == COMMANDTYPE_B && g_aCommand[nIdx][2] == COMMANDTYPE_Y)
		{
			for (int nCntCommand = 0; nCntCommand < MAX_COMMAND; nCntCommand++)
			{
				g_aCommand[nIdx][nCntCommand] = COMMANDTYPE_NONE;
			}

			return COMMANDOREDER_YBY;
		}

		//黄黄青
		else if (g_aCommand[nIdx][0] == COMMANDTYPE_Y && g_aCommand[nIdx][1] == COMMANDTYPE_Y && g_aCommand[nIdx][2] == COMMANDTYPE_B)
		{
			for (int nCntCommand = 0; nCntCommand < MAX_COMMAND; nCntCommand++)
			{
				g_aCommand[nIdx][nCntCommand] = COMMANDTYPE_NONE;
			}

			return COMMANDOREDER_YYB;
		}
		//-----------------------------------------------------------------------------------------------------

		//加速-------------------------------------------------------------------------------------------------
		//緑緑黄
		else if (g_aCommand[nIdx][0] == COMMANDTYPE_G && g_aCommand[nIdx][1] == COMMANDTYPE_G && g_aCommand[nIdx][2] == COMMANDTYPE_Y)
		{
			for (int nCntCommand = 0; nCntCommand < MAX_COMMAND; nCntCommand++)
			{
				g_aCommand[nIdx][nCntCommand] = COMMANDTYPE_NONE;
			}

			return COMMANDOREDER_GGY;
		}

		//緑黄緑
		else if (g_aCommand[nIdx][0] == COMMANDTYPE_G && g_aCommand[nIdx][1] == COMMANDTYPE_Y && g_aCommand[nIdx][2] == COMMANDTYPE_G)
		{
			for (int nCntCommand = 0; nCntCommand < MAX_COMMAND; nCntCommand++)
			{
				g_aCommand[nIdx][nCntCommand] = COMMANDTYPE_NONE;
			}

			return COMMANDOREDER_GYG;
		}

		//黄緑緑
		else if (g_aCommand[nIdx][0] == COMMANDTYPE_Y && g_aCommand[nIdx][1] == COMMANDTYPE_G && g_aCommand[nIdx][2] == COMMANDTYPE_G)
		{
			for (int nCntCommand = 0; nCntCommand < MAX_COMMAND; nCntCommand++)
			{
				g_aCommand[nIdx][nCntCommand] = COMMANDTYPE_NONE;
			}

			return COMMANDOREDER_YGG;
		}
		//-----------------------------------------------------------------------------------------------------

		//時間の巻き戻し(回帰)---------------------------------------------------------------------------------
		//赤緑青
		else if (g_aCommand[nIdx][0] == COMMANDTYPE_R && g_aCommand[nIdx][1] == COMMANDTYPE_G && g_aCommand[nIdx][2] == COMMANDTYPE_B)
		{
			for (int nCntCommand = 0; nCntCommand < MAX_COMMAND; nCntCommand++)
			{
				g_aCommand[nIdx][nCntCommand] = COMMANDTYPE_NONE;
			}

			return COMMANDOREDER_RGB;
		}
		else if (nCntCommand[nIdx] >= MAX_COMMAND)
		{
			for (int nCntCommand = 0; nCntCommand < MAX_COMMAND; nCntCommand++)
			{
				g_aCommand[nIdx][nCntCommand] = COMMANDTYPE_NONE;
			}

			return COMMANDOREDER_NONE;
		}
	}
	else if (nCntCommand[nIdx] < MAX_COMMAND)
	{
		return COMMANDOREDER_NONE;
	}
}

//魔法の変換=============================
MAGICTYPE ChangeMagic(COMMANDOREDER commandorder)
{
	//浮遊---------------------------------------
	if (commandorder == COMMANDOREDER_GGG)
	{
		return MAGICTYPE_LEVITATION;
	}

	//燃焼---------------------------------------
	if (commandorder == COMMANDOREDER_RRR)
	{
		return MAGICTYPE_COMBUSTION;
	}

	//洪水、氾濫---------------------------------
	if (commandorder == COMMANDOREDER_BBB)
	{
		return MAGICTYPE_FLOOD;
	}

	//フラッシュ---------------------------------
	if (commandorder == COMMANDOREDER_YYY)
	{
		return MAGICTYPE_FLASH;
	}

	//火球---------------------------------------
	if (commandorder == COMMANDOREDER_RRG || commandorder == COMMANDOREDER_RGR || commandorder == COMMANDOREDER_GRR)
	{
		return MAGICTYPE_FIREBALL;
	}

	//太陽の動きを遅延する-----------------------
	if (commandorder == COMMANDOREDER_RYY || commandorder == COMMANDOREDER_YRY || commandorder == COMMANDOREDER_YYR)
	{
		return MAGICTYPE_SUNSETDELAY;
	}

	//雨乞い-------------------------------------
	if (commandorder == COMMANDOREDER_BBG || commandorder == COMMANDOREDER_BGB || commandorder == COMMANDOREDER_GBB)
	{
		return MAGICTYPE_RAINPRAY;
	}

	//凍結---------------------------------------
	if (commandorder == COMMANDOREDER_BGG || commandorder == COMMANDOREDER_GBG || commandorder == COMMANDOREDER_GGB)
	{
		return MAGICTYPE_FREEZE;
	}

	//成長(植物など)-----------------------------
	if (commandorder == COMMANDOREDER_BYY || commandorder == COMMANDOREDER_YBY || commandorder == COMMANDOREDER_YYB)
	{
		return MAGICTYPE_GROWTH;
	}

	//加速---------------------------------------
	if (commandorder == COMMANDOREDER_GGY || commandorder == COMMANDOREDER_GYG || commandorder == COMMANDOREDER_YGG)
	{
		return MAGICTYPE_ACCELERATION;
	}

	//時間の巻き戻し(回帰)-----------------------
	if (commandorder == COMMANDOREDER_RGB)
	{
		return MAGICTYPE_TIMEREVERT;
	}
}

//魔法の設定処理==============================
void SetMagic(MAGICTYPE type, D3DXVECTOR3 pos, D3DXVECTOR3 rot, D3DXVECTOR3 move)
{
	for (int nCntPlayerType = 0; nCntPlayerType < MAX_PLAYER; nCntPlayerType++)
	{
		for (int nCntMagic = 0; nCntMagic < MAX_MAGIC; nCntMagic++)
		{
			if (g_aMagic[nCntPlayerType][nCntMagic].bUse == false)
			{
				g_aMagic[nCntPlayerType][nCntMagic].mType = type;
				g_aMagic[nCntPlayerType][nCntMagic].pos = pos;
				g_aMagic[nCntPlayerType][nCntMagic].rot = rot;
				g_aMagic[nCntPlayerType][nCntMagic].move = move;
				g_aMagic[nCntPlayerType][nCntMagic].bUse = true;

				switch (type)
				{
					//浮遊
				case MAGICTYPE_LEVITATION:

					break;

					//燃焼
				case MAGICTYPE_COMBUSTION:

					break;

					//洪水、氾濫
				case MAGICTYPE_FLOOD:

					break;

					//フラッシュ
				case MAGICTYPE_FLASH:

					break;

					//火球
				case MAGICTYPE_FIREBALL:

					break;

					//太陽の動きを遅延する
				case MAGICTYPE_SUNSETDELAY:

					break;

					//雨乞い
				case MAGICTYPE_RAINPRAY:

					break;

					//凍結
				case MAGICTYPE_FREEZE:

					break;

					//成長(植物など)
				case MAGICTYPE_GROWTH:

					break;

					//加速
				case MAGICTYPE_ACCELERATION:

					break;

					//時間の巻き戻し(回帰)
				case MAGICTYPE_TIMEREVERT:

					break;
				}

				break;
			}
		}
	}
}

//魔法の設定処理==============================
void SetMagicPosition(COMMANDOREDER type, D3DXVECTOR3 pos, D3DXVECTOR3 rot)
{
	DropMagic* pDropMagic = &g_aDropMagic[0];		// 先頭アドレス	

	for (int nCntMagic = 0; nCntMagic < MAX_DROPMAGIC; nCntMagic++, pDropMagic++)
	{
		if (pDropMagic->bUse == true)
		{// もし使ってたら弾く
			continue;
		}

		pDropMagic->pos = pos;
		pDropMagic->rot = rot;
		pDropMagic->oType = type;
		pDropMagic->bUse = true;

		break;
	}
}

//魔法情報の取得==============================
Magic* GetMagic(void)
{
	return &g_aMagic[0][0];
}

//フィールドの魔法との当たり判定==============
COMMANDOREDER CollisionMagic(D3DXVECTOR3 pos, float fRadius)
{
	DropMagic* pDropMagic = &g_aDropMagic[0];	// 先頭アドレス

	float fDiff = 0.0f;		// 判定用変数

	for (int nCntMagic = 0; nCntMagic < MAX_DROPMAGIC; nCntMagic++, pDropMagic++)
	{
		if (pDropMagic->bUse == false)
		{// 使っていなかったら弾く
			continue;
		}

		// 各距離を二乗したものをすべて足す
		fDiff = powf(pDropMagic->pos.x - pos.x, 2) + powf(pDropMagic->pos.y - pos.y, 2) + powf(pDropMagic->pos.z - pos.z, 2);

		if (fDiff <= powf(fRadius + pDropMagic->fRadius, 2))
		{// 当たっていたら
			// ここで種類に応じた振動を呼び出す
			PrintDebugProc("[%d]落ちてる魔法とあたっている\n", nCntMagic);
			return pDropMagic->oType;
		}
	}

	return COMMANDOREDER_NONE;
}
//魔法情報の取得==============================
MAGICTYPE GetFieldMagic(D3DXVECTOR3 pos, float fRadius)
{
	return MAGICTYPE_NONE;
}