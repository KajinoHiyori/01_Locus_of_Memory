//=============================================================================
//
//	ゴール地点の処理 [goal.h]
//	Author : SHUMA AIZU
// 
//=============================================================================

#include "goal.h"
#include "game.h"
#include "player.h"
#include "readyui.h"
#include "input.h"
#include "title.h"

//*****************************************************************************
// マクロ定義
//*****************************************************************************
#define GOAL_SIZE	(120.0f)	// ゴール判定のサイズ

//*****************************************************************************
// グローバル変数
//*****************************************************************************
Book g_Book;					// 本の情報
bool g_abGoal[MAX_PLAYER];	// ゴール状態の保存
bool g_NextMode;

//=============================================================================
// ゴール地点の初期化処理
//=============================================================================
void InitGoal(void)
{
	g_abGoal[0] = false;
	g_abGoal[1] = false;
	g_NextMode = false;
	memset(&g_Book, NULL, sizeof(Book));
}

//=============================================================================
// ゴール地点の終了処理
//=============================================================================
void UninitGoal(void)
{

}

//=============================================================================
// ゴール地点の更新処理
//=============================================================================
void UpdateGoal(void)
{
	g_NextMode = false;
	// ゴールの当たり判定
	CollisionGoal();

	// モーション更新
	UpdateMotion(&g_Book.motion, g_Book.pModelData, &g_Book.OffSetData);
}

//==============================================================================
// ゴール地点の描画処理
//==============================================================================
void DrawGoal(void)
{
	// 階層構造モデル描画
	DrawParentModel(&g_Book.pos, &g_Book.rot, &g_Book.mtxWorld, g_Book.pModelData, &g_Book.OffSetData);
}

//==============================================================================
// ゴール地点の設定処理
//==============================================================================
void SetGoal(D3DXVECTOR3 pos, float fRadius)
{
	// 各値設定
	g_Book.pos = pos;
	g_Book.fRadius = fRadius;

	// モデルデータ設定
	g_Book.pModelData = SetModelData(PARENTMODELTYPE_BOOK);

	// モーションデータ設定
	g_Book.motion.pMotionData = SetMotionData(MOTIONDATATYPE_BOOK);

	// ニュートラルに
	SetMotion(&g_Book.motion, g_Book.pModelData, &g_Book.OffSetData, MOTIONTYPE_NEUTRAL, true, false, 10);
}

//==============================================================================
// ゴール地点の当たり判定処理
//==============================================================================
void CollisionGoal(void)
{
	Player* pPlayer = GetPlayer();
	OPERATIONTYPE operationType = GetOperationType();
	float afLength[MAX_PLAYER] = { 0.0f };

	for (int nCntPlayer = 0; nCntPlayer < MAX_PLAYER; nCntPlayer++, pPlayer++)
	{
		// ゲートとプレイヤーの距離を測定
		if (pPlayer->bUse == false)
		{
			ResetReadyUI(nCntPlayer);
			continue;
		}

		afLength[nCntPlayer] = sqrtf((pPlayer->pos.x - g_Book.pos.x) * (pPlayer->pos.x - g_Book.pos.x) + (pPlayer->pos.z - g_Book.pos.z) * (pPlayer->pos.z - g_Book.pos.z)) * 0.5f;
		if (afLength[nCntPlayer] < GOAL_SIZE)	// 遷移完了状態に移行できる範囲にいる
		{
			if ((GetKeyboardTrigger(DIK_RETURN) == true && nCntPlayer == 0) || GetJoypadTrigger(JOYKEY_X, nCntPlayer) == true)
			{
				g_abGoal[nCntPlayer] = true;
			}
		}
		else
		{
			g_abGoal[nCntPlayer] = false;
		}
	}

	// 準備完了状態のUIを管理
	switch (operationType)
	{
	case OPERATIONTYPE_1P:	// 1Pジョイパッド操作
		if (afLength[0] < GOAL_SIZE)
		{ // 遷移完了状態に移行できる範囲にいる
			if (g_abGoal[0] == true)
			{ // ゴールフラグが立っている
				SetReadyUI(0, READYUITYPE_GOAL);
				g_NextMode = true;
			}
			else
			{ // ゴールフラグが立っていない
				SetReadyUI(0, READYUITYPE_GAMEX);
			}
		}
		else
		{ // 遷移完了状態に移行できる範囲にいない
			ResetReadyUI(0);
		}
		break;

	case OPERATIONTYPE_2P:	// 2人操作
		if (afLength[0] < GOAL_SIZE && afLength[1] < GOAL_SIZE)
		{ // 1P2P共に遷移完了状態に移行できる範囲にいる
			for (int nCntPlayer = 0; nCntPlayer < MAX_PLAYER; nCntPlayer++)
			{
				if (g_abGoal[nCntPlayer] == true)
				{ // ゴールフラグが立っている
					SetReadyUI(nCntPlayer, READYUITYPE_GOAL);
				}
				else
				{ // ゴールフラグが立っていない
					SetReadyUI(nCntPlayer, READYUITYPE_GAMEX);
				}
			}
		}
		if (afLength[0] < GOAL_SIZE && afLength[1] > GOAL_SIZE)
		{ // 1Pが遷移完了状態に移行できる範囲にいて、2Pが遷移完了状態に移行できる範囲にいない
			SetReadyUI(0, READYUITYPE_WAITING);
			ResetReadyUI(1);
		}
		else if (afLength[0] > GOAL_SIZE && afLength[1] < GOAL_SIZE)
		{ // 1Pが遷移完了状態に移行できる範囲におらず、2Pが遷移完了状態に移行できる範囲にいる
			ResetReadyUI(0);
			SetReadyUI(1, READYUITYPE_WAITING);
		}

		if (g_abGoal[0] == true && g_abGoal[1] == true)
		{
			g_NextMode = true;
		}
		break;

	case OPERATIONTYPE_KEYBOARD:	// キーボード操作
		if (afLength[0] < GOAL_SIZE)
		{ // 遷移完了状態に移行できる範囲にいる
			if (g_abGoal[0] == true)
			{ // ゴールフラグが立っている
				SetReadyUI(0, READYUITYPE_GOAL);
				g_NextMode = true;
			}
			else
			{ // ゴールフラグが立っていない
				SetReadyUI(0, READYUITYPE_GAMEENTER);
			}
		}
		else
		{ // 遷移完了状態に移行できる範囲にいない
			ResetReadyUI(0);
		}
		break;
	}
}

//==============================================================================
// 遷移可能状態を返す
//==============================================================================
bool GetGoalState(void)
{
	return g_NextMode;
}
