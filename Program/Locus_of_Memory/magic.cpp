//===================================================================
//
// magic処理[magic.cpp]
// Author : 眞鍋快陸
// 
//===================================================================
#include "magic.h"
#include "spellui.h"
#include "player.h"
#include "debugproc.h"
#include "input.h"
#include "shadow.h"
#include "particle.h"
#include "vibration.h"
#include "event.h"
#include "magiccircle.h"

//マクロ定義
#define MAX_MAGIC				(128)		//魔法の最大数
#define MAX_DROPMAGIC			(32)		//落ちてる魔法の最大数
#define MAX_COMMAND				(3)			//コマンドの最大数
#define MAX_MAGICLOCUS			(64)		//魔法使用場所最大数
#define DROPMAGIC_RADIUS		(50.0f)		//落ちてる魔法の半径
#define DROPMAGIC_MEDIUMRADIUS	(100.0f)	//落ちてる魔法の半径
#define DROPMAGIC_FARRADIUS		(150.0f)	//落ちてる魔法の半径
#define DISP_MAGIC				(30)		// UIの発動魔法表示時間管理

//グローバル変数宣言
Magic g_aMagic[MAX_PLAYER][MAX_MAGIC];					//魔法の情報
DropMagic g_aDropMagic[MAX_DROPMAGIC];					//落ちてる魔法の情報
COMMANDTYPE g_aCommand[MAX_PLAYER][MAX_COMMAND];		//コマンドの情報
MagicCounter g_aCounter[MAX_PLAYER];					//リザルト用魔法回数カウント
MagicLocus g_aMagicLocus[MAX_MAGICLOCUS];				//魔法使用場所の情報
int g_aCntCommand[MAX_PLAYER] = {};						//

//魔法の初期化処理=============================
void InitMagic(void)
{
	MagicCounter* pCounter = &g_aCounter[0];
	MagicLocus* pMagicLocus = &g_aMagicLocus[0];

	memset(pCounter, NULL, sizeof(MagicCounter) * MAX_PLAYER);
	memset(&g_aCntCommand[0], NULL, sizeof(int) * MAX_PLAYER);
	//memset(pMagicLocus, NULL, sizeof(MagicLocus) * MAX_MAGICLOCUS);

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

	SetMagicPosition(COMMANDOREDER_BBB, D3DXVECTOR3(0.0f, 0.0f, 0.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f), DROPMAGIC_RADIUS);
	SetMagicPosition(COMMANDOREDER_RRR, D3DXVECTOR3(150.0f, 0.0f, 0.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f), DROPMAGIC_RADIUS);
	SetMagicPosition(COMMANDOREDER_GGG, D3DXVECTOR3(-150.0f, 0.0f, 0.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f), DROPMAGIC_RADIUS);

	SetMagicPosition(COMMANDOREDER_YYY, D3DXVECTOR3(0.0f, 0.0f, 300.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f), DROPMAGIC_RADIUS);
	SetMagicPosition(COMMANDOREDER_RGB, D3DXVECTOR3(0.0f, 0.0f, -300.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f), DROPMAGIC_RADIUS);

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
				PrintDebugProc("発動中魔法 : %d\n", g_aMagic[nCntPlayerType][nCntMagic].mType);
			}
		}
	}

	PrintDebugProc("入力コマンド数 : %d\n", g_aCntCommand[0]);
}

//魔法の描画処理==============================
void DrawMagic(void)
{

}

//コマンド入力情報=============================
COMMANDOREDER PressCommand(int nIdx)
{
	if (g_aCommand[nIdx][g_aCntCommand[nIdx]] == COMMANDTYPE_NONE)
	{//コマンドが何も入力されていないとき
		if (GetJoypadTrigger(JOYKEY_B, nIdx) == true || (GetKeyboardTrigger(DIK_3) == true && nIdx == 0))
		{//B(赤)が入力された
			g_aCommand[nIdx][g_aCntCommand[nIdx]] = COMMANDTYPE_R;
		}
		else if (GetJoypadTrigger(JOYKEY_A, nIdx) == true || (GetKeyboardTrigger(DIK_4) == true && nIdx == 0))
		{//A(緑)が入力された
			g_aCommand[nIdx][g_aCntCommand[nIdx]] = COMMANDTYPE_G;
		}
		else if (GetJoypadTrigger(JOYKEY_X, nIdx) == true || (GetKeyboardTrigger(DIK_2) == true && nIdx == 0))
		{//X(青)が入力された
			g_aCommand[nIdx][g_aCntCommand[nIdx]] = COMMANDTYPE_B;
		}
		else if (GetJoypadTrigger(JOYKEY_Y, nIdx) == true || (GetKeyboardTrigger(DIK_1) == true && nIdx == 0))
		{//Y(黄)が入力された
			g_aCommand[nIdx][g_aCntCommand[nIdx]] = COMMANDTYPE_Y;
		}
	}
	if (g_aCommand[nIdx][g_aCntCommand[nIdx]] != COMMANDTYPE_NONE)
	{//コマンドが何かしら入力されたとき
		g_aCntCommand[nIdx]++;
	}

	if (g_aCntCommand[nIdx] == MAX_COMMAND)
	{//コマンドが三つ入力されたとき
		//浮遊-------------------------------------------------------------------------------------------------
		//緑緑緑
		if (g_aCommand[nIdx][0] == COMMANDTYPE_G && g_aCommand[nIdx][1] == COMMANDTYPE_G && g_aCommand[nIdx][2] == COMMANDTYPE_G)
		{
			ResetCommand(nIdx);

			return COMMANDOREDER_GGG;
		}

		//燃焼-------------------------------------------------------------------------------------------------
		//赤赤赤
		else if (g_aCommand[nIdx][0] == COMMANDTYPE_R && g_aCommand[nIdx][1] == COMMANDTYPE_R && g_aCommand[nIdx][2] == COMMANDTYPE_R)
		{
			ResetCommand(nIdx);

			return COMMANDOREDER_RRR;
		}

		//洪水、氾濫-------------------------------------------------------------------------------------------
		//青青青
		else if (g_aCommand[nIdx][0] == COMMANDTYPE_B && g_aCommand[nIdx][1] == COMMANDTYPE_B && g_aCommand[nIdx][2] == COMMANDTYPE_B)
		{
			ResetCommand(nIdx);

			return COMMANDOREDER_BBB;
		}

		//フラッシュ-------------------------------------------------------------------------------------------
		//黄黄黄
		else if (g_aCommand[nIdx][0] == COMMANDTYPE_Y && g_aCommand[nIdx][1] == COMMANDTYPE_Y && g_aCommand[nIdx][2] == COMMANDTYPE_Y)
		{
			ResetCommand(nIdx);

			return COMMANDOREDER_YYY;
		}

		//火球-------------------------------------------------------------------------------------------------
		//赤赤緑
		else if (g_aCommand[nIdx][0] == COMMANDTYPE_R && g_aCommand[nIdx][1] == COMMANDTYPE_R && g_aCommand[nIdx][2] == COMMANDTYPE_G)
		{
			ResetCommand(nIdx);

			return COMMANDOREDER_RRG;
		}

		//赤緑赤
		else if (g_aCommand[nIdx][0] == COMMANDTYPE_R && g_aCommand[nIdx][1] == COMMANDTYPE_G && g_aCommand[nIdx][2] == COMMANDTYPE_R)
		{
			ResetCommand(nIdx);

			return COMMANDOREDER_RGR;
		}

		//緑赤赤
		else if (g_aCommand[nIdx][0] == COMMANDTYPE_G && g_aCommand[nIdx][1] == COMMANDTYPE_R && g_aCommand[nIdx][2] == COMMANDTYPE_R)
		{
			ResetCommand(nIdx);

			return COMMANDOREDER_GRR;
		}
		//-----------------------------------------------------------------------------------------------------

		//太陽の動きを遅延する---------------------------------------------------------------------------------
		//赤黄黄
		else if (g_aCommand[nIdx][0] == COMMANDTYPE_R && g_aCommand[nIdx][1] == COMMANDTYPE_Y && g_aCommand[nIdx][2] == COMMANDTYPE_Y)
		{
			ResetCommand(nIdx);

			return COMMANDOREDER_RYY;
		}

		//黄赤黄
		else if (g_aCommand[nIdx][0] == COMMANDTYPE_Y && g_aCommand[nIdx][1] == COMMANDTYPE_R && g_aCommand[nIdx][2] == COMMANDTYPE_Y)
		{
			ResetCommand(nIdx);

			return COMMANDOREDER_YRY;
		}

		//黄黄赤
		else if (g_aCommand[nIdx][0] == COMMANDTYPE_Y && g_aCommand[nIdx][1] == COMMANDTYPE_Y && g_aCommand[nIdx][2] == COMMANDTYPE_R)
		{
			ResetCommand(nIdx);

			return COMMANDOREDER_YYR;
		}
		//-----------------------------------------------------------------------------------------------------

		//雨乞い-----------------------------------------------------------------------------------------------
		//青青緑
		else if (g_aCommand[nIdx][0] == COMMANDTYPE_B && g_aCommand[nIdx][1] == COMMANDTYPE_B && g_aCommand[nIdx][2] == COMMANDTYPE_G)
		{
			ResetCommand(nIdx);

			return COMMANDOREDER_BBG;
		}

		//青緑青
		else if (g_aCommand[nIdx][0] == COMMANDTYPE_B && g_aCommand[nIdx][1] == COMMANDTYPE_G && g_aCommand[nIdx][2] == COMMANDTYPE_B)
		{
			ResetCommand(nIdx);

			return COMMANDOREDER_BGB;
		}

		//緑青青
		else if (g_aCommand[nIdx][0] == COMMANDTYPE_G && g_aCommand[nIdx][1] == COMMANDTYPE_B && g_aCommand[nIdx][2] == COMMANDTYPE_B)
		{
			ResetCommand(nIdx);

			return COMMANDOREDER_GBB;
		}
		//-----------------------------------------------------------------------------------------------------

		//凍結-------------------------------------------------------------------------------------------------
		//青緑緑
		else if (g_aCommand[nIdx][0] == COMMANDTYPE_B && g_aCommand[nIdx][1] == COMMANDTYPE_G && g_aCommand[nIdx][2] == COMMANDTYPE_G)
		{
			ResetCommand(nIdx);

			return COMMANDOREDER_BGG;
		}

		//緑青緑
		else if (g_aCommand[nIdx][0] == COMMANDTYPE_G && g_aCommand[nIdx][1] == COMMANDTYPE_B && g_aCommand[nIdx][2] == COMMANDTYPE_G)
		{
			ResetCommand(nIdx);

			return COMMANDOREDER_GBG;
		}

		//緑緑青
		else if (g_aCommand[nIdx][0] == COMMANDTYPE_G && g_aCommand[nIdx][1] == COMMANDTYPE_G && g_aCommand[nIdx][2] == COMMANDTYPE_B)
		{
			ResetCommand(nIdx);

			return COMMANDOREDER_GGB;
		}
		//-----------------------------------------------------------------------------------------------------

		//成長(植物など)---------------------------------------------------------------------------------------
		//青黄黄
		else if (g_aCommand[nIdx][0] == COMMANDTYPE_B && g_aCommand[nIdx][1] == COMMANDTYPE_Y && g_aCommand[nIdx][2] == COMMANDTYPE_Y)
		{
			ResetCommand(nIdx);

			return COMMANDOREDER_BYY;
		}

		//黄青黄
		else if (g_aCommand[nIdx][0] == COMMANDTYPE_Y && g_aCommand[nIdx][1] == COMMANDTYPE_B && g_aCommand[nIdx][2] == COMMANDTYPE_Y)
		{
			ResetCommand(nIdx);

			return COMMANDOREDER_YBY;
		}

		//黄黄青
		else if (g_aCommand[nIdx][0] == COMMANDTYPE_Y && g_aCommand[nIdx][1] == COMMANDTYPE_Y && g_aCommand[nIdx][2] == COMMANDTYPE_B)
		{
			ResetCommand(nIdx);

			return COMMANDOREDER_YYB;
		}
		//-----------------------------------------------------------------------------------------------------

		//加速-------------------------------------------------------------------------------------------------
		//緑緑黄
		else if (g_aCommand[nIdx][0] == COMMANDTYPE_G && g_aCommand[nIdx][1] == COMMANDTYPE_G && g_aCommand[nIdx][2] == COMMANDTYPE_Y)
		{
			ResetCommand(nIdx);
	
			return COMMANDOREDER_GGY;
		}

		//緑黄緑
		else if (g_aCommand[nIdx][0] == COMMANDTYPE_G && g_aCommand[nIdx][1] == COMMANDTYPE_Y && g_aCommand[nIdx][2] == COMMANDTYPE_G)
		{
			ResetCommand(nIdx);

			return COMMANDOREDER_GYG;
		}

		//黄緑緑
		else if (g_aCommand[nIdx][0] == COMMANDTYPE_Y && g_aCommand[nIdx][1] == COMMANDTYPE_G && g_aCommand[nIdx][2] == COMMANDTYPE_G)
		{
			ResetCommand(nIdx);

			return COMMANDOREDER_YGG;
		}
		//-----------------------------------------------------------------------------------------------------

		//時間の巻き戻し(回帰)---------------------------------------------------------------------------------
		//赤緑青
		else if (g_aCommand[nIdx][0] == COMMANDTYPE_R && g_aCommand[nIdx][1] == COMMANDTYPE_G && g_aCommand[nIdx][2] == COMMANDTYPE_B)
		{
			ResetCommand(nIdx);

			return COMMANDOREDER_RGB;
		}
		else
		{
			ResetCommand(nIdx);

			return COMMANDOREDER_NONE;
		}
	}
	else if (g_aCntCommand[nIdx] < MAX_COMMAND)
	{
		return COMMANDOREDER_NONE;
	}

	return COMMANDOREDER_NONE;

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
void SetMagic(MAGICTYPE type, D3DXVECTOR3 pos, D3DXVECTOR3 rot, D3DXVECTOR3 move, int nIdx)
{
	Player* pPlayer = GetPlayer();

	pPlayer += nIdx;

	for (int nCntMagic = 0; nCntMagic < MAX_MAGIC; nCntMagic++)
	{
		if (g_aMagic[nIdx][nCntMagic].bUse == false)
		{
			g_aMagic[nIdx][nCntMagic].mType = type;
			g_aMagic[nIdx][nCntMagic].pos = pos;
			g_aMagic[nIdx][nCntMagic].rot = rot;
			g_aMagic[nIdx][nCntMagic].move = move;
			g_aMagic[nIdx][nCntMagic].bUse = true;
			SetSpellUI(g_aMagic[nIdx][nCntMagic].mType, nIdx, DISP_MAGIC);

			CollisionMagicLocus(type, pos, 25.0f, nIdx);
			SetMagicCircle(type, &pPlayer->pos);

			switch (type)
			{
				//浮遊
			case MAGICTYPE_LEVITATION:
				g_aCounter[nIdx].nMagicTypeCounter[MAGICTYPE_LEVITATION]++;
				g_aCounter[nIdx].nCommandCounter[COMMANDTYPE_G] += 3;
				break;

				//燃焼
			case MAGICTYPE_COMBUSTION:
				g_aCounter[nIdx].nMagicTypeCounter[MAGICTYPE_COMBUSTION]++;
				g_aCounter[nIdx].nCommandCounter[COMMANDTYPE_R] += 3;
				break;

				//洪水、氾濫
			case MAGICTYPE_FLOOD:
				g_aCounter[nIdx].nMagicTypeCounter[MAGICTYPE_FLOOD]++;
				g_aCounter[nIdx].nCommandCounter[COMMANDTYPE_B] += 3;
				break;

				//フラッシュ
			case MAGICTYPE_FLASH:
				g_aCounter[nIdx].nMagicTypeCounter[MAGICTYPE_FLASH]++;
				g_aCounter[nIdx].nCommandCounter[COMMANDTYPE_Y] += 3;
				break;

				//火球
			case MAGICTYPE_FIREBALL:
				g_aCounter[nIdx].nMagicTypeCounter[MAGICTYPE_FIREBALL]++;
				g_aCounter[nIdx].nCommandCounter[COMMANDTYPE_R] += 2;
				g_aCounter[nIdx].nCommandCounter[COMMANDTYPE_G]++;
				break;

				//太陽の動きを遅延する
			case MAGICTYPE_SUNSETDELAY:
				g_aCounter[nIdx].nMagicTypeCounter[MAGICTYPE_SUNSETDELAY]++;
				g_aCounter[nIdx].nCommandCounter[COMMANDTYPE_Y] += 2;
				g_aCounter[nIdx].nCommandCounter[COMMANDTYPE_R]++;
				break;

				//雨乞い
			case MAGICTYPE_RAINPRAY:
				g_aCounter[nIdx].nMagicTypeCounter[MAGICTYPE_RAINPRAY]++;
				g_aCounter[nIdx].nCommandCounter[COMMANDTYPE_B] += 2;
				g_aCounter[nIdx].nCommandCounter[COMMANDTYPE_G]++;
				break;

				//凍結
			case MAGICTYPE_FREEZE:
				g_aCounter[nIdx].nMagicTypeCounter[MAGICTYPE_FREEZE]++;
				g_aCounter[nIdx].nCommandCounter[COMMANDTYPE_G] += 2;
				g_aCounter[nIdx].nCommandCounter[COMMANDTYPE_B]++;
				break;

				//成長(植物など)
			case MAGICTYPE_GROWTH:
				g_aCounter[nIdx].nMagicTypeCounter[MAGICTYPE_GROWTH]++;
				g_aCounter[nIdx].nCommandCounter[COMMANDTYPE_Y] += 2;
				g_aCounter[nIdx].nCommandCounter[COMMANDTYPE_B]++;
				break;

				//加速
			case MAGICTYPE_ACCELERATION:
				g_aCounter[nIdx].nMagicTypeCounter[MAGICTYPE_ACCELERATION]++;
				g_aCounter[nIdx].nCommandCounter[COMMANDTYPE_G] += 2;
				g_aCounter[nIdx].nCommandCounter[COMMANDTYPE_Y]++;
				break;

				//時間の巻き戻し(回帰)
			case MAGICTYPE_TIMEREVERT:
				g_aCounter[nIdx].nMagicTypeCounter[MAGICTYPE_TIMEREVERT]++;
				g_aCounter[nIdx].nCommandCounter[COMMANDTYPE_R]++;
				g_aCounter[nIdx].nCommandCounter[COMMANDTYPE_G]++;
				g_aCounter[nIdx].nCommandCounter[COMMANDTYPE_B]++;
				break;
			}

			break;
		}
	}
}

//魔法の設定処理==============================
void SetMagicPosition(COMMANDOREDER type, D3DXVECTOR3 pos, D3DXVECTOR3 rot, float fRadius)
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

//魔法使用場所設定処理==============================
void SetMagicLocus(MAGICEVENT event, D3DXVECTOR3 pos, float fRadius, int nIdx)
{
	MagicLocus* pMagicLocus = &g_aMagicLocus[0];		// 先頭アドレス

	for (int nCntMagicLocus = 0; nCntMagicLocus < MAX_MAGICLOCUS; nCntMagicLocus++, pMagicLocus++)
	{
		if (pMagicLocus->bUse == true)
		{// 使ってたら弾く
			continue;
		}

		// 各引数の値を代入
		pMagicLocus->MagicEvent = event;		// イベントの種類
		pMagicLocus->pos = pos;					// 原点
		pMagicLocus->fRadius = fRadius;			// 半径
		pMagicLocus->nIdxObject = nIdx;			// 対応するオブジェクトの番号
		pMagicLocus->bUse = true;				// 使用状態

		break;
	}
}


//魔法情報の取得==============================
Magic* GetMagic(void)
{
	return &g_aMagic[0][0];
}

//魔法使用可能場所との当たり判定==============
bool CollisionMagicLocus(MAGICTYPE type, D3DXVECTOR3 pos, float fRadius, int nIdx)
{
	MagicLocus* pMagicLocus = &g_aMagicLocus[0];	// 先頭アドレス
	float fDiff = 0.0f;								// 判定用変数

	for (int nCntMagicLocus = 0; nCntMagicLocus < MAX_MAGICLOCUS; nCntMagicLocus++, pMagicLocus++)
	{
		if (pMagicLocus->bUse == false)
		{// 使っていなければ弾く
			continue;
		}

		// 各距離を二乗したものをすべて足す
		fDiff = powf(pMagicLocus->pos.x - pos.x, 2) + powf(pMagicLocus->pos.y - pos.y, 2) + powf(pMagicLocus->pos.z - pos.z, 2);

		if (fDiff <= powf(fRadius + pMagicLocus->fRadius, 2))
		{// 当たっていたら
			if (SetMagicEvent(pMagicLocus->MagicEvent, type, pMagicLocus->nIdxObject) == true)
			{// 魔法と対応するイベントかどうかチェック
				return true;
			}
			else
			{// 対応していなければやり直し
				continue;
			}
		}
	}

	return false;
}

//フィールドの魔法との当たり判定==============
int CollisionMagic(D3DXVECTOR3 pos, float fRadius, int nIdx)
{
	DropMagic* pDropMagic = &g_aDropMagic[0];	// 先頭アドレス
	bool isSearch = false;

	//VIBRATIONTYPE* pVibration = GetVibration();

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
			VibrationType(VIBRATIONTYPE_CLOSE, MAGICTYPE_COMBUSTION, nIdx);
			PrintDebugProc("[%d]落ちている魔法とあたっている\n", nCntMagic);
			return nCntMagic;
		}
		else if (fDiff <= powf(fRadius + DROPMAGIC_MEDIUMRADIUS, 2))
		{// 落ちている魔法の周辺にいたら
			// ここで種類に応じた振動を呼び出す
			VibrationType(VIBRATIONTYPE_MEDIUM, MAGICTYPE_COMBUSTION, nIdx);
			PrintDebugProc("[%d]周辺に魔法が落ちている\n", nCntMagic);
			isSearch = true;
		}
		else if (fDiff <= powf(fRadius + DROPMAGIC_FARRADIUS, 2))
		{// 遠くに魔法が落ちていたら
			 // ここで種類に応じた振動を呼び出す
			VibrationType(VIBRATIONTYPE_FAR, MAGICTYPE_COMBUSTION, nIdx);

			PrintDebugProc("[%d]遠くに魔法が落ちている\n", nCntMagic);
			isSearch = true;
		}


	}

	if (isSearch == false)
	{// 範囲外に出た場合
		// ここで種類に応じた振動を呼び出す
		VibrationType(VIBRATIONTYPE_NOTHING, MAGICTYPE_COMBUSTION, nIdx);
	}

	return -1;
}

//魔法情報の取得==============================
COMMANDOREDER GetFieldMagic(int nIdx)
{
	g_aDropMagic[nIdx].bUse = false;
	return g_aDropMagic[nIdx].oType;
}

//魔法カウントの取得==========================
MagicCounter* GetMagicCounter(int nIdx)
{
	return &g_aCounter[nIdx];
}

// 入力された魔法の種類を取得==========================
COMMANDTYPE* GetCommandType(int nIdx)
{
	return &g_aCommand[nIdx][0];
}

// 入力情報リセット処理==========================
void ResetCommand(int nIdx)
{
	memset(&g_aCntCommand[nIdx], NULL, sizeof(int));
	for (int nCntCommand = 0; nCntCommand < MAX_COMMAND; nCntCommand++)
	{
		g_aCommand[nIdx][nCntCommand] = COMMANDTYPE_NONE;
	}
}