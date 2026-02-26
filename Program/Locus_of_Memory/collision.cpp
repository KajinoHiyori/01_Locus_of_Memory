//=============================================================================
//
//	当たり判定の管理 [collision.cpp]
//	Author : SHUMA AIZU
// 
//=============================================================================

#include "collision.h"

//*****************************************************************************
// マクロ定義
//*****************************************************************************
#define MAX_COLLISION			(256)									// 当たり判定の最大数
#define MAX_COLLIDER			(MAX_COLLISION * MAX_ONECOLLIDER)		// コライダーの最大数

//*****************************************************************************
// グローバル変数
//*****************************************************************************
Collider g_aCollider[MAX_COLLIDER] = {};		// コライダーの情報
Collision g_aCollision[MAX_COLLISION] = {};		// 当たり判定の情報
int g_aUseCollisionIdx[MAX_COLLISION] = {};		// 使用している当たり判定の識別番号
int g_nNumCollision;							// 使用している当たり判定の数
int g_nNumCollider;								// 使用しているコライダーの数

//=============================================================================
//	当たり判定の初期化処理
//=============================================================================
void InitCollision(void)
{
	// 各種ポインタ
	Collider* pCollider = &g_aCollider[0];
	Collision* pCollision = &g_aCollision[0];

	// 初期化
	memset(pCollider, NULL, sizeof(Collider) * MAX_COLLIDER);
	memset(pCollision, NULL, sizeof(Collision) * MAX_COLLISION);

	g_nNumCollision = 0;
}

//=============================================================================
//	当たり判定の終了処理
//=============================================================================
void UninitCollision(void)
{

}

//=============================================================================
//	当たり判定の更新処理
//=============================================================================
void UpdateCollision(int nIdx, UINT TargetTag)
{
	Collision* pCollision = &g_aCollision[0];		// 当たり判定へのポインタ

	for (int nCntCollision = 0; nCntCollision < g_nNumCollision; nCntCollision++)
	{
		if (!pCollision->tag & TargetTag)
		{// ターゲットじゃなければ除外
			continue;
		}


	}
}

//=============================================================================
//	当たり判定の描画処理 (仮)
//=============================================================================
void DrawCollision(void)
{

}

//=============================================================================
//	当たり判定の設定処理
//=============================================================================
int SetCollision(void)
{
	g_aCollision[g_nNumCollider].bUse = true;
	g_nNumCollision++;
	return g_nNumCollision - 1;
}

//=============================================================================
//	矩形のコライダーの設定処理
//=============================================================================
void SetBoxCollider(int nIdx, BoxCollider BoxColliderInfo, D3DXVECTOR3 pos, D3DXVECTOR3 rot)
{
	// 指定された当たり判定にコライダーを追加
	g_aCollision[nIdx].nColliderIdx[g_aCollision[nIdx].nNumCollider] = g_nNumCollider;

	// 各値代入
	g_aCollider[g_nNumCollider].pos = pos;
	g_aCollider[g_nNumCollider].rot = rot;
	g_aCollider[g_nNumCollider].type.box = BoxColliderInfo;

	// コライダー使用数を当たり判定でも全体でも増やす
	g_aCollision[nIdx].nNumCollider++;
	g_nNumCollider++;
}