//=============================================================================
//
//	当たり判定の管理 [collision.cpp]
//	Author : SHUMA AIZU
// 
//=============================================================================

#include "collision.h"
#include "debugproc.h"

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
int g_aUseColliderIdx[MAX_COLLIDER] = {};		// 使用しているコライダーの識別番号
int g_aUseCollisionIdx[MAX_COLLISION] = {};		// 使用している当たり判定の識別番号
int g_nNumCollider;								// 使用しているコライダーの数
int g_nNumCollision;							// 使用している当たり判定の数

//*****************************************************************************
// 関数ポインタの定義
//*****************************************************************************

//=============================================================================
//	矩形の当たり判定
//=============================================================================
bool(*BoxCollision[COLLIDERTYPE_MAX])(CollisionInfo& _CollisionInfo, ColliderType MyCollider, ColliderType TargetCollider, bool isTrigger) =
{
	CollisionBoxToBox,
	CollisionBoxToCylinder,
	CollisionBoxToSphere,
	CollisionBoxToCapsule,
};

//=============================================================================
//	筒の当たり判定
//=============================================================================
bool(*CylinderCollision[COLLIDERTYPE_MAX])(CollisionInfo& _CollisionInfo, ColliderType MyCollider, ColliderType TargetCollider, bool isTrigger) =
{
	CollisionCylinderToBox,
	CollisionCylinderToCylinder,
	CollisionCylinderToSphere,
	CollisionCylinderToCapsule,
};

//=============================================================================
//	球の当たり判定
//=============================================================================
bool(*SphereCollision[COLLIDERTYPE_MAX])(CollisionInfo& _CollisionInfo, ColliderType MyCollider, ColliderType TargetCollider, bool isTrigger) =
{
	CollisionSphereToBox,
	CollisionSphereToCylinder,
	CollisionSphereToSphere,
	CollisionSphereToCapsule,
};

//=============================================================================
//	カプセルの当たり判定
//=============================================================================
#if 0	// 未実装
bool(*CapsuleCollision[COLLIDERTYPE_MAX])(CollisionInfo& _CollisionInfo, ColliderType MyCollider, ColliderType TargetCollider, bool isTrigger) =
{

};
#endif

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
	memset(&g_aUseColliderIdx[0], -1, sizeof(int) * MAX_COLLIDER);
	memset(&g_aUseCollisionIdx[0], -1, sizeof(int) * MAX_COLLISION);

	g_nNumCollider = 0;
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
CollisionInfo UpdateCollision(int nMyIdx, int nTargetIdx, bool isTrigger)
{
	Collision* pMyCollision = &g_aCollision[nMyIdx];			// 自分の当たり判定へのポインタ
	Collision* pTargetCollision = &g_aCollision[nTargetIdx];	// 相手の当たり判定へのポインタ
	CollisionInfo CollisionInfo = {};							// 衝突情報
	bool isCollision = false;									// 衝突したかどうか

	// 自分のコライダーから
	for (int nCntMyCollider = 0; nCntMyCollider < pMyCollision->nNumCollider; nCntMyCollider++)
	{
		// 相手のコライダー
		for (int nCntTargetCollider = 0; nCntTargetCollider < pTargetCollision->nNumCollider; nCntTargetCollider++)
		{
			// 対応する当たり判定を呼ぶ
			switch (g_aCollider[pMyCollision->nColliderIdx[nCntMyCollider]].type)
			{
				// 矩形の当たり判定
			case COLLIDERTYPE_BOX:
				isCollision = BoxCollision[g_aCollider[pTargetCollision->nColliderIdx[nCntTargetCollider]].type]
				(CollisionInfo, 
				g_aCollider[pMyCollision->nColliderIdx[nCntMyCollider]].Collidertype, 
				g_aCollider[pTargetCollision->nColliderIdx[nCntMyCollider]].Collidertype, 
				isTrigger);
				break;

				// 筒の当たり判定
			case COLLIDERTYPE_CYLINDER:
				isCollision = CylinderCollision[g_aCollider[pTargetCollision->nColliderIdx[nCntTargetCollider]].type]
				(CollisionInfo,
				g_aCollider[pMyCollision->nColliderIdx[nCntMyCollider]].Collidertype,
				g_aCollider[pTargetCollision->nColliderIdx[nCntMyCollider]].Collidertype,
				isTrigger);
				break;

				// 球の当たり判定
			case COLLIDERTYPE_SPHERE:
				isCollision = SphereCollision[g_aCollider[pTargetCollision->nColliderIdx[nCntTargetCollider]].type]
				(CollisionInfo, 
				g_aCollider[pMyCollision->nColliderIdx[nCntMyCollider]].Collidertype, 
				g_aCollider[pTargetCollision->nColliderIdx[nCntMyCollider]].Collidertype,
				isTrigger);
				break;

				// カプセルの当たり判定
			case COLLIDERTYPE_CAPSULE:
				//CapsuleCollision[g_aCollider[pTargetCollision->nColliderIdx[nCntTargetCollider]].type]
				//(CollisionInfo, 
				//g_aCollider[pMyCollision->nColliderIdx[nCntMyCollider]].Collidertype, 
				//g_aCollider[pMyCollision->nColliderIdx[nCntMyCollider]].Collidertype);
				break;
			}
		}
	}

	CollisionInfo.isCollision = isCollision;

	return CollisionInfo;
}

//=============================================================================
//	コライダーの更新処理 (仮)
//=============================================================================
void UpdateCollider(int nIdx, D3DXVECTOR3 pos)
{
	// 位置を合わせる
	g_aCollider[g_aCollision[nIdx].nColliderIdx[0]].Collidertype.posOld = g_aCollider[g_aCollision[nIdx].nColliderIdx[0]].Collidertype.pos;
	g_aCollider[g_aCollision[nIdx].nColliderIdx[0]].Collidertype.pos = pos;
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
int SetCollision(D3DXVECTOR3 pos, D3DXVECTOR3 rot)
{
	Collision* pCollision = &g_aCollision[0];
	int nCntCollision = 0;

	for (nCntCollision = 0; nCntCollision < MAX_COLLISION; nCntCollision++, pCollision++)
	{
		if (pCollision->bUse == true)
		{// 使ってたら弾く
			continue;
		}

		pCollision->pos = pos;
		pCollision->rot = rot;

		pCollision->bUse = true;	// 使用状態に
		g_nNumCollision++;			// 全体の数を増やす
		break;
	}

	return nCntCollision;			// 使用状態にした当たり判定の番号を返す

	//// 今使ってる最大数をインデックスとして指定
	//g_aCollision[g_nNumCollider].bUse = true;
	//g_nNumCollision++;
	//return g_nNumCollision - 1;
}

//=============================================================================
//	当たり判定の解除処理
//=============================================================================
void ResetCollision(int nIdx)
{
	for (int nCntCollider = 0; nCntCollider < g_aCollision[nIdx].nNumCollider; nCntCollider++)
	{
		memset(&g_aCollider[g_aCollision[nIdx].nColliderIdx[nCntCollider]], NULL, sizeof(Collider));
		g_nNumCollider--;
	}
	memset(&g_aCollision[nIdx], NULL, sizeof(Collision));
	g_nNumCollision--;
}

//=============================================================================
//	コライダーの設定処理
//=============================================================================
void SetCollider(int nIdx, ColliderInfo ColliderInfo)
{
	Collider* pCollider = &g_aCollider[0];	// コライダーへのポインタ
	int nCntCollider = 0;					// カウント用変数

	for (nCntCollider = 0; nCntCollider < MAX_COLLIDER; nCntCollider++, pCollider++)
	{
		if (pCollider->bUse == true)
		{// 使ってたら弾く
			continue;
		}

		// 指定された当たり判定にコライダーのインデックスを追加
		g_aCollision[nIdx].nColliderIdx[g_aCollision[nIdx].nNumCollider] = nCntCollider;

		// 各値代入
		pCollider->Collidertype = ColliderInfo.Collidertype;
		pCollider->type = ColliderInfo.type;
		pCollider->bUse = true;

		// コライダー使用数を当たり判定でも全体でも増やす
		g_aCollision[nIdx].nNumCollider++;
		g_nNumCollider++;

		break;
	}
}

//=============================================================================
//	矩形と矩形との当たり判定処理
//=============================================================================
bool CollisionBoxToBox(CollisionInfo& _CollisionInfo, ColliderType MyCollider, ColliderType TargetCollider, bool isTrigger)
{
	// 計算用マトリックス
	D3DXMATRIX MymtxRot, TargetmtxRot;

	// マトリックスの初期化
	D3DXMatrixIdentity(&MymtxRot);
	D3DXMatrixIdentity(&TargetmtxRot);

	// 向きを算出
	D3DXMatrixRotationYawPitchRoll(&MymtxRot, MyCollider.box.rot.y, MyCollider.box.rot.x, MyCollider.box.rot.z);
	D3DXMatrixRotationYawPitchRoll(&TargetmtxRot, TargetCollider.box.rot.y, TargetCollider.box.rot.x, TargetCollider.box.rot.z);

	// 向き行列から各方向ベクトルの確保 (正規化Nと長さ)
	// 自分の矩形のX方面ベクトル
	D3DXVECTOR3 MyAxisXNor = { MymtxRot._11, MymtxRot._12, MymtxRot._13 },
		MyAxisX = MyAxisXNor * MyCollider.box.fWidth;

	// 自分の矩形のY方面ベクトル
	D3DXVECTOR3 MyAxisYNor = { MymtxRot._21, MymtxRot._22, MymtxRot._23 },
		MyAxisY = MyAxisYNor * MyCollider.box.fHeight;

	// 自分の矩形のZ方面ベクトル
	D3DXVECTOR3 MyAxisZNor = { MymtxRot._31, MymtxRot._32, MymtxRot._33 },
		MyAxisZ = MyAxisZNor * MyCollider.box.fDepth;

	// 対象の矩形のX方面ベクトル
	D3DXVECTOR3 TargetAxisXNor = { TargetmtxRot._11, TargetmtxRot._12, TargetmtxRot._13 },
		TargetAxisX = TargetAxisXNor * TargetCollider.box.fWidth;

	// 対象の矩形のY方面ベクトル
	D3DXVECTOR3 TargetAxisYNor = { TargetmtxRot._21, TargetmtxRot._22, TargetmtxRot._23 },
		TargetAxisY = TargetAxisYNor * TargetCollider.box.fHeight;

	// 対象の矩形のZ方面ベクトル
	D3DXVECTOR3 TargetAxisZNor = { TargetmtxRot._31, TargetmtxRot._32, TargetmtxRot._33 },
		TargetAxisZ = TargetAxisZNor * TargetCollider.box.fDepth;

	// 自分と対象の中心点の距離
	D3DXVECTOR3 Distance = MyCollider.box.pos - TargetCollider.box.pos;

	D3DXVECTOR3 Sep = {};

	// 分離軸 : Ae1
	Sep = D3DXVECTOR3(fabsf(D3DXVec3Dot(&MyAxisXNor, &TargetAxisX)), fabsf(D3DXVec3Dot(&MyAxisXNor, &TargetAxisY)), fabsf(D3DXVec3Dot(&MyAxisXNor, &TargetAxisZ)));

	float rA = D3DXVec3Length(&MyAxisX);
	float rB = Sep.x + Sep.y + Sep.z;
	float L = fabs(D3DXVec3Dot(&Distance, &MyAxisXNor));
	if (L > rA + rB)
	{
		PrintDebugProc("AX %f\n", L);
		return false; // 衝突していない
	}

	// 分離軸 : Ae2
	Sep = D3DXVECTOR3(fabsf(D3DXVec3Dot(&MyAxisYNor, &TargetAxisX)), fabsf(D3DXVec3Dot(&MyAxisYNor, &TargetAxisY)), fabsf(D3DXVec3Dot(&MyAxisYNor, &TargetAxisZ)));

	rA = D3DXVec3Length(&MyAxisY);
	rB = Sep.x + Sep.y + Sep.z;
	L = fabs(D3DXVec3Dot(&Distance, &MyAxisYNor));
	if (L > rA + rB)
	{
		PrintDebugProc("AY %f\n", L);
		return false; // 衝突していない
	}

	// 分離軸 : Ae3
	Sep = D3DXVECTOR3(fabsf(D3DXVec3Dot(&MyAxisZNor, &TargetAxisX)), fabsf(D3DXVec3Dot(&MyAxisZNor, &TargetAxisY)), fabsf(D3DXVec3Dot(&MyAxisZNor, &TargetAxisZ)));

	rA = D3DXVec3Length(&MyAxisZ);
	rB = Sep.x + Sep.y + Sep.z;
	L = fabs(D3DXVec3Dot(&Distance, &MyAxisZNor));
	if (L > rA + rB)
	{
		PrintDebugProc("AZ %f\n", L);
		return false; // 衝突していない
	}

	// 分離軸 : Be1
	Sep = D3DXVECTOR3(fabsf(D3DXVec3Dot(&MyAxisX, &TargetAxisXNor)), fabsf(D3DXVec3Dot(&MyAxisY, &TargetAxisXNor)), fabsf(D3DXVec3Dot(&MyAxisZ, &TargetAxisXNor)));

	rA = Sep.x + Sep.y + Sep.z;
	rB = D3DXVec3Length(&TargetAxisX);
	L = fabs(D3DXVec3Dot(&Distance, &TargetAxisXNor));
	if (L > rA + rB)
	{
		PrintDebugProc("BX %f\n", L);
		return false; // 衝突していない
	}

	// 分離軸 : Be2
	Sep = D3DXVECTOR3(fabsf(D3DXVec3Dot(&MyAxisX, &TargetAxisYNor)), fabsf(D3DXVec3Dot(&MyAxisY, &TargetAxisYNor)), fabsf(D3DXVec3Dot(&MyAxisZ, &TargetAxisYNor)));

	rA = Sep.x + Sep.y + Sep.z;
	rB = D3DXVec3Length(&TargetAxisY);
	L = fabs(D3DXVec3Dot(&Distance, &TargetAxisYNor));
	if (L > rA + rB)
	{
		PrintDebugProc("BY %f\n", L);
		return false; // 衝突していない
	}

	// 分離軸 : Be3
	Sep = D3DXVECTOR3(fabsf(D3DXVec3Dot(&MyAxisX, &TargetAxisZNor)), fabsf(D3DXVec3Dot(&MyAxisY, &TargetAxisZNor)), fabsf(D3DXVec3Dot(&MyAxisZ, &TargetAxisZNor)));

	rA = Sep.x + Sep.y + Sep.z;
	rB = D3DXVec3Length(&TargetAxisZ);
	L = fabs(D3DXVec3Dot(&Distance, &TargetAxisZNor));
	if (L > rA + rB)
	{
		PrintDebugProc("BZ %f\n", L);
		return false; // 衝突していない
	}

	// 分離軸 : C11
	D3DXVECTOR3 Cross;
	D3DXVec3Cross(&Cross, &MyAxisXNor, &TargetAxisXNor);

	Sep = D3DXVECTOR3(fabsf(D3DXVec3Dot(&MyAxisY, &Cross)), fabsf(D3DXVec3Dot(&MyAxisZ, &Cross)), 0.0f);
	D3DXVECTOR3 Sep2 = D3DXVECTOR3(fabsf(D3DXVec3Dot(&TargetAxisY, &Cross)), fabsf(D3DXVec3Dot(&TargetAxisZ, &Cross)), 0.0f);

	rA = Sep.x + Sep.y;
	rB = Sep2.x + Sep2.y;
	L = fabs(D3DXVec3Dot(&Distance, &Cross));
	if (L > rA + rB)
	{
		return false;
	}

	// 分離軸 : C12
	D3DXVec3Cross(&Cross, &MyAxisXNor, &TargetAxisYNor);

	Sep = D3DXVECTOR3(fabsf(D3DXVec3Dot(&MyAxisY, &Cross)), fabsf(D3DXVec3Dot(&MyAxisZ, &Cross)), 0.0f);
	Sep2 = D3DXVECTOR3(fabsf(D3DXVec3Dot(&TargetAxisX, &Cross)), fabsf(D3DXVec3Dot(&TargetAxisZ, &Cross)), 0.0f);

	rA = Sep.x + Sep.y;
	rB = Sep2.x + Sep2.y;
	L = fabs(D3DXVec3Dot(&Distance, &Cross));
	if (L > rA + rB)
	{
		return false;
	}

	// 分離軸 : C13
	D3DXVec3Cross(&Cross, &MyAxisXNor, &TargetAxisZNor);

	Sep = D3DXVECTOR3(fabsf(D3DXVec3Dot(&MyAxisY, &Cross)), fabsf(D3DXVec3Dot(&MyAxisZ, &Cross)), 0.0f);
	Sep2 = D3DXVECTOR3(fabsf(D3DXVec3Dot(&TargetAxisX, &Cross)), fabsf(D3DXVec3Dot(&TargetAxisY, &Cross)), 0.0f);

	rA = Sep.x + Sep.y;
	rB = Sep2.x + Sep2.y;
	L = fabs(D3DXVec3Dot(&Distance, &Cross));
	if (L > rA + rB)
	{
		return false;
	}

	// 分離軸 : C21
	D3DXVec3Cross(&Cross, &MyAxisYNor, &TargetAxisXNor);

	Sep = D3DXVECTOR3(fabsf(D3DXVec3Dot(&MyAxisX, &Cross)), fabsf(D3DXVec3Dot(&MyAxisZ, &Cross)), 0.0f);
	Sep2 = D3DXVECTOR3(fabsf(D3DXVec3Dot(&TargetAxisY, &Cross)), fabsf(D3DXVec3Dot(&TargetAxisZ, &Cross)), 0.0f);

	rA = Sep.x + Sep.y;
	rB = Sep2.x + Sep2.y;
	L = fabs(D3DXVec3Dot(&Distance, &Cross));
	if (L > rA + rB)
	{
		return false;
	}

	// 分離軸 : C22
	D3DXVec3Cross(&Cross, &MyAxisYNor, &TargetAxisYNor);

	Sep = D3DXVECTOR3(fabsf(D3DXVec3Dot(&MyAxisX, &Cross)), fabsf(D3DXVec3Dot(&MyAxisZ, &Cross)), 0.0f);
	Sep2 = D3DXVECTOR3(fabsf(D3DXVec3Dot(&TargetAxisX, &Cross)), fabsf(D3DXVec3Dot(&TargetAxisZ, &Cross)), 0.0f);

	rA = Sep.x + Sep.y;
	rB = Sep2.x + Sep2.y;
	L = fabs(D3DXVec3Dot(&Distance, &Cross));
	if (L > rA + rB)
	{
		return false;
	}

	// 分離軸 : C23
	D3DXVec3Cross(&Cross, &MyAxisYNor, &TargetAxisZNor);

	Sep = D3DXVECTOR3(fabsf(D3DXVec3Dot(&MyAxisX, &Cross)), fabsf(D3DXVec3Dot(&MyAxisZ, &Cross)), 0.0f);
	Sep2 = D3DXVECTOR3(fabsf(D3DXVec3Dot(&TargetAxisX, &Cross)), fabsf(D3DXVec3Dot(&TargetAxisY, &Cross)), 0.0f);

	rA = Sep.x + Sep.y;
	rB = Sep2.x + Sep2.y;
	L = fabs(D3DXVec3Dot(&Distance, &Cross));
	if (L > rA + rB)
	{
		return false;
	}

	// 分離軸 : C31
	D3DXVec3Cross(&Cross, &MyAxisZNor, &TargetAxisXNor);

	Sep = D3DXVECTOR3(fabsf(D3DXVec3Dot(&MyAxisX, &Cross)), fabsf(D3DXVec3Dot(&MyAxisY, &Cross)), 0.0f);
	Sep2 = D3DXVECTOR3(fabsf(D3DXVec3Dot(&TargetAxisY, &Cross)), fabsf(D3DXVec3Dot(&TargetAxisZ, &Cross)), 0.0f);

	rA = Sep.x + Sep.y;
	rB = Sep2.x + Sep2.y;
	L = fabs(D3DXVec3Dot(&Distance, &Cross));
	if (L > rA + rB)
	{
		return false;
	}

	// 分離軸 : C32
	D3DXVec3Cross(&Cross, &MyAxisZNor, &TargetAxisYNor);

	Sep = D3DXVECTOR3(fabsf(D3DXVec3Dot(&MyAxisX, &Cross)), fabsf(D3DXVec3Dot(&MyAxisY, &Cross)), 0.0f);
	Sep2 = D3DXVECTOR3(fabsf(D3DXVec3Dot(&TargetAxisX, &Cross)), fabsf(D3DXVec3Dot(&TargetAxisZ, &Cross)), 0.0f);

	rA = Sep.x + Sep.y;
	rB = Sep2.x + Sep2.y;
	L = fabs(D3DXVec3Dot(&Distance, &Cross));
	if (L > rA + rB)
	{
		return false;
	}

	// 分離軸 : C33
	D3DXVec3Cross(&Cross, &MyAxisZNor, &TargetAxisZNor);

	Sep = D3DXVECTOR3(fabsf(D3DXVec3Dot(&MyAxisX, &Cross)), fabsf(D3DXVec3Dot(&MyAxisY, &Cross)), 0.0f);
	Sep2 = D3DXVECTOR3(fabsf(D3DXVec3Dot(&TargetAxisX, &Cross)), fabsf(D3DXVec3Dot(&TargetAxisY, &Cross)), 0.0f);

	rA = Sep.x + Sep.y;
	rB = Sep2.x + Sep2.y;
	L = fabs(D3DXVec3Dot(&Distance, &Cross));
	if (L > rA + rB)
	{
		return false;
	}

	PrintDebugProc("MyPos = {%.2f, %.2f, %.2f}\n", MyCollider.pos.x, MyCollider.pos.y, MyCollider.pos.z);;

	// 未実装
	return true;
}

//=============================================================================
//	矩形と筒との当たり判定処理
//=============================================================================
bool CollisionBoxToCylinder(CollisionInfo& _CollisionInfo, ColliderType MyCollider, ColliderType TargetCollider, bool isTrigger)
{
	// 未実装
	return false;
}

//=============================================================================
//	矩形と球との当たり判定処理
//=============================================================================
bool CollisionBoxToSphere(CollisionInfo& _CollisionInfo, ColliderType MyCollider, ColliderType TargetCollider, bool isTrigger)
{
	// 未実装
	return false;
}

//=============================================================================
//	矩形とカプセルとの当たり判定処理
//=============================================================================
bool CollisionBoxToCapsule(CollisionInfo& _CollisionInfo, ColliderType MyCollider, ColliderType TargetCollider, bool isTrigger)
{
	// 未実装
	return false;
}

//=============================================================================
//	筒と矩形との当たり判定処理
//=============================================================================
bool CollisionCylinderToBox(CollisionInfo& _CollisionInfo, ColliderType MyCollider, ColliderType TargetCollider, bool isTrigger)
{
	// 未実装
	return false;
}

//=============================================================================
//	筒と筒との当たり判定処理
//=============================================================================
bool CollisionCylinderToCylinder(CollisionInfo& _CollisionInfo, ColliderType MyCollider, ColliderType TargetCollider, bool isTrigger)
{
	// 各距離を二乗したものをすべて足す
	float fDiff = powf(MyCollider.cylinder.pos.x - TargetCollider.cylinder.pos.x, 2) + 
		powf(MyCollider.cylinder.pos.z - TargetCollider.cylinder.pos.z, 2);

	// 総合より自分と対象の半径を足して二乗したもののほうが大きければ当たっている
	if (fDiff <= powf(MyCollider.cylinder.fRadius + TargetCollider.cylinder.fRadius, 2))
	{// 当たっていたら
		if (isTrigger)
		{// トリガーだったら
			return true;
		}

		// 距離から弾く角度を出す
		float fAngle = atan2f(MyCollider.cylinder.pos.x - TargetCollider.cylinder.pos.x, 
			MyCollider.cylinder.pos.z - TargetCollider.cylinder.pos.z);

		// 対象の原点から弾く方向に自分と対象の半径の合計分移動
		_CollisionInfo.Intersection.x = TargetCollider.cylinder.pos.x + sinf(fAngle) * (MyCollider.cylinder.fRadius + TargetCollider.cylinder.fRadius);
		_CollisionInfo.Intersection.y = MyCollider.cylinder.pos.y;
		_CollisionInfo.Intersection.z = TargetCollider.cylinder.pos.z + cosf(fAngle) * (MyCollider.cylinder.fRadius + TargetCollider.cylinder.fRadius);

		return true;
	}

	return false;
}

//=============================================================================
//	筒と球との当たり判定処理
//=============================================================================
bool CollisionCylinderToSphere(CollisionInfo& _CollisionInfo, ColliderType MyCollider, ColliderType TargetCollider, bool isTrigger)
{
	// 各距離を二乗したものをすべて足す
	float fDiff = powf(MyCollider.cylinder.pos.x - TargetCollider.cylinder.pos.x, 2) +
		powf(MyCollider.cylinder.pos.z - TargetCollider.cylinder.pos.z, 2);

	// 総合より自分と対象の半径を足して二乗したもののほうが大きければ当たっている
	if (fDiff <= powf(MyCollider.cylinder.fRadius + TargetCollider.cylinder.fRadius, 2))
	{// 当たっていたら
		if (isTrigger)
		{// トリガーだったら
			return true;
		}

		// 距離から弾く角度を出す
		float fAngle = atan2f(MyCollider.cylinder.pos.x - TargetCollider.cylinder.pos.x,
			MyCollider.cylinder.pos.z - TargetCollider.cylinder.pos.z);

		// 対象の原点から弾く方向に自分と対象の半径の合計分移動
		_CollisionInfo.Intersection.x = TargetCollider.cylinder.pos.x + sinf(fAngle) * (MyCollider.cylinder.fRadius + TargetCollider.cylinder.fRadius);
		_CollisionInfo.Intersection.y = MyCollider.cylinder.pos.y;
		_CollisionInfo.Intersection.z = TargetCollider.cylinder.pos.z + cosf(fAngle) * (MyCollider.cylinder.fRadius + TargetCollider.cylinder.fRadius);

		return true;
	}

	return false;
}

//=============================================================================
//	筒とカプセルとの当たり判定処理
//=============================================================================
bool CollisionCylinderToCapsule(CollisionInfo& _CollisionInfo, ColliderType MyCollider, ColliderType TargetCollider, bool isTrigger)
{
	// 未実装
	return false;
}

//=============================================================================
//	球と矩形との当たり判定処理
//=============================================================================
bool CollisionSphereToBox(CollisionInfo& _CollisionInfo, ColliderType MyCollider, ColliderType TargetCollider, bool isTrigger)
{
	// 未実装
	return false;
}

//=============================================================================
//	球と筒との当たり判定処理
//=============================================================================
bool CollisionSphereToCylinder(CollisionInfo& _CollisionInfo, ColliderType MyCollider, ColliderType TargetCollider, bool isTrigger)
{
	// 各距離を二乗したものをすべて足す
	float fDiff = powf(MyCollider.sphere.pos.x - TargetCollider.cylinder.pos.x, 2) +
		powf(MyCollider.sphere.pos.z - TargetCollider.cylinder.pos.z, 2);

	// 総合より自分と対象の半径を足して二乗したもののほうが大きければ当たっている
	if (fDiff <= powf(MyCollider.sphere.fRadius + TargetCollider.cylinder.fRadius, 2))
	{// 当たっていたら
		if (isTrigger)
		{// トリガーだったら
			return true;
		}

		// 距離から弾く角度を出す
		float fAngle = atan2f(MyCollider.sphere.pos.x - TargetCollider.cylinder.pos.x,
			MyCollider.sphere.pos.z - TargetCollider.cylinder.pos.z);

		// 対象の原点から弾く方向に自分と対象の半径の合計分移動
		_CollisionInfo.Intersection.x = TargetCollider.cylinder.pos.x + sinf(fAngle) * (MyCollider.sphere.fRadius + TargetCollider.cylinder.fRadius);
		_CollisionInfo.Intersection.y = MyCollider.sphere.pos.y;
		_CollisionInfo.Intersection.z = TargetCollider.cylinder.pos.z + cosf(fAngle) * (MyCollider.sphere.fRadius + TargetCollider.cylinder.fRadius);

		return true;
	}

	return false;
}

//=============================================================================
//	球と球との当たり判定処理
//=============================================================================
bool CollisionSphereToSphere(CollisionInfo& _CollisionInfo, ColliderType MyCollider, ColliderType TargetCollider, bool isTrigger)
{
	// 各距離を二乗したものをすべて足す
	float fDiff = powf(MyCollider.sphere.pos.x - TargetCollider.sphere.pos.x, 2) + 
		powf(MyCollider.sphere.pos.y - TargetCollider.sphere.pos.y, 2) + 
		powf(MyCollider.sphere.pos.z - TargetCollider.sphere.pos.z, 2);

	// 総合より自分と対象の半径を足して二乗したもののほうが大きければ当たっている
	if (fDiff <= powf(MyCollider.sphere.fRadius + TargetCollider.sphere.fRadius, 2))
	{// 当たっていたら
		if (isTrigger)
		{// トリガーだったら
			return true;
		}

		// 弾く方向を決める (自分と対象の距離から出た向きを正規化)
		D3DXVECTOR3 VecDir = (MyCollider.sphere.pos - TargetCollider.sphere.pos);
		D3DXVec3Normalize(&VecDir, &VecDir);

		// 交点を算出 (対象の位置から弾く方向に向かって自分と対象の半径を足しただけ進める)
		_CollisionInfo.Intersection = (TargetCollider.sphere.pos +
			(VecDir * (MyCollider.sphere.fRadius + TargetCollider.sphere.fRadius)));

		return true;		// 衝突していたと返す
	}

	return false;			// 衝突していなかったと返す
}

//=============================================================================
//	球とカプセルとの当たり判定処理
//=============================================================================
bool CollisionSphereToCapsule(CollisionInfo& _CollisionInfo, ColliderType MyCollider, ColliderType TargetCollider, bool isTrigger)
{
	// 未実装
	return false;
}