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
int g_aUseColliderIdx[MAX_COLLIDER] = {};		// 使用しているコライダーの識別番号
int g_aUseCollisionIdx[MAX_COLLISION] = {};		// 使用している当たり判定の識別番号
int g_nNumCollider;								// 使用しているコライダーの数
int g_nNumCollision;							// 使用している当たり判定の数

//*****************************************************************************
// 関数ポインタの定義
//*****************************************************************************
bool(*BoxCollision[COLLIDERTYPE_MAX])(CollisionInfo& _CollisionInfo, ColliderType MyCollider, ColliderType TargetCollider) =
{
	CollisionBoxToBox,
	CollisionBoxToCylinder,
	CollisionBoxToSphere,
	CollisionBoxToCapsule,
};

bool(*CylinderCollision[COLLIDERTYPE_MAX])(CollisionInfo& _CollisionInfo, ColliderType MyCollider, ColliderType TargetCollider) =
{
	CollisionCylinderToBox,
	CollisionCylinderToCylinder,
	CollisionCylinderToSphere,
	CollisionCylinderToCapsule,
};

bool(*SphereCollision[COLLIDERTYPE_MAX])(CollisionInfo& _CollisionInfo, ColliderType MyCollider, ColliderType TargetCollider) =
{
	CollisionSphereToBox,
	CollisionSphereToCylinder,
	CollisionSphereToSphere,
	CollisionSphereToCapsule,
};

#if 0	// こんなものなかった
bool(*CapsuleCollision[COLLIDERTYPE_MAX])(CollisionInfo& _CollisionInfo, ColliderType MyCollider, ColliderType TargetCollider) =
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
CollisionInfo UpdateCollision(int nMyIdx, int nTargetIdx)
{
	Collision* pMyCollision = &g_aCollision[nMyIdx];			// 自分の当たり判定へのポインタ
	Collision* pTargetCollision = &g_aCollision[nTargetIdx];	// 相手の当たり判定へのポインタ
	CollisionInfo CollisionInfo = {};							// 衝突情報
	bool isCollision = false;									// 衝突したかどうか

	for (int nCntMyCollider = 0; nCntMyCollider < pMyCollision->nNumCollider; nCntMyCollider++)
	{
		for (int nCntTargetCollider = 0; nCntTargetCollider < pTargetCollision->nNumCollider; nCntTargetCollider++)
		{
			switch (g_aCollider[pMyCollision->nColliderIdx[nCntMyCollider]].type)
			{
				// 矩形の当たり判定
			case COLLIDERTYPE_BOX:
				isCollision = BoxCollision[g_aCollider[pTargetCollision->nColliderIdx[nCntTargetCollider]].type]
				(CollisionInfo, 
				g_aCollider[pMyCollision->nColliderIdx[nCntMyCollider]].Collidertype, 
				g_aCollider[pTargetCollision->nColliderIdx[nCntMyCollider]].Collidertype);
				break;

				// 筒の当たり判定
			case COLLIDERTYPE_CYLINDER:
				isCollision = CylinderCollision[g_aCollider[pTargetCollision->nColliderIdx[nCntTargetCollider]].type]
				(CollisionInfo,
				g_aCollider[pMyCollision->nColliderIdx[nCntMyCollider]].Collidertype,
				g_aCollider[pTargetCollision->nColliderIdx[nCntMyCollider]].Collidertype);
				break;

				// 球の当たり判定
			case COLLIDERTYPE_SPHERE:
				isCollision = SphereCollision[g_aCollider[pTargetCollision->nColliderIdx[nCntTargetCollider]].type]
				(CollisionInfo, 
				g_aCollider[pMyCollision->nColliderIdx[nCntMyCollider]].Collidertype, 
				g_aCollider[pTargetCollision->nColliderIdx[nCntMyCollider]].Collidertype);
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
int SetCollision(void)
{
	Collision* pCollision = &g_aCollision[0];
	int nCntCollision = 0;

	for (nCntCollision = 0; nCntCollision < MAX_COLLISION; nCntCollision++, pCollision++)
	{
		if (pCollision->bUse == true)
		{
			continue;
		}

		pCollision->bUse = true;
		g_nNumCollision++;
		break;
	}

	return nCntCollision;


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
//	矩形のコライダーの設定処理
//=============================================================================
void SetBoxCollider(int nIdx, BoxCollider BoxColliderInfo)
{
	Collider* pCollider = &g_aCollider[0];	// コライダーへのポインタ
	int nCntCollider = 0;					// カウント用変数

	for (nCntCollider = 0; nCntCollider < MAX_COLLIDER; nCntCollider++, pCollider++)
	{
		if (pCollider->bUse == true)
		{
			continue;
		}

		// 指定された当たり判定にコライダーのインデックスを追加
		g_aCollision[nIdx].nColliderIdx[g_aCollision[nIdx].nNumCollider] = nCntCollider;

		// 各値代入
		pCollider->Collidertype.box = BoxColliderInfo;
		pCollider->type = COLLIDERTYPE_BOX;
		pCollider->bUse = true;

		// コライダー使用数を当たり判定でも全体でも増やす
		g_aCollision[nIdx].nNumCollider++;
		g_nNumCollider++;

		break;
	}
}

//=============================================================================
//	筒のコライダーの設定処理
//=============================================================================
void SetCylinderCollider(int nIdx, CylinderCollider ClinderColliderInfo)
{
	Collider* pCollider = &g_aCollider[0];	// コライダーへのポインタ
	int nCntCollider = 0;					// カウント用変数

	for (nCntCollider = 0; nCntCollider < MAX_COLLIDER; nCntCollider++, pCollider++)
	{
		if (pCollider->bUse == true)
		{
			continue;
		}

		// 指定された当たり判定にコライダーのインデックスを追加
		g_aCollision[nIdx].nColliderIdx[g_aCollision[nIdx].nNumCollider] = nCntCollider;

		// 各値代入
		pCollider->Collidertype.cylinder = ClinderColliderInfo;
		pCollider->type = COLLIDERTYPE_CYLINDER;
		pCollider->bUse = true;

		// コライダー使用数を当たり判定でも全体でも増やす
		g_aCollision[nIdx].nNumCollider++;
		g_nNumCollider++;

		break;
	}
}

//=============================================================================
//	球のコライダーの設定処理
//=============================================================================
void SetSphereCollider(int nIdx, SphereCollider SphereColliderInfo)
{
	Collider* pCollider = &g_aCollider[0];	// コライダーへのポインタ
	int nCntCollider = 0;					// カウント用変数

	for (nCntCollider = 0; nCntCollider < MAX_COLLIDER; nCntCollider++, pCollider++)
	{
		if (pCollider->bUse == true)
		{
			continue;
		}

		// 指定された当たり判定にコライダーのインデックスを追加
		g_aCollision[nIdx].nColliderIdx[g_aCollision[nIdx].nNumCollider] = nCntCollider;

		// 各値代入
		pCollider->Collidertype.sphere = SphereColliderInfo;
		pCollider->type = COLLIDERTYPE_SPHERE;
		pCollider->bUse = true;

		// コライダー使用数を当たり判定でも全体でも増やす
		g_aCollision[nIdx].nNumCollider++;
		g_nNumCollider++;

		break;
	}
}

//=============================================================================
//	カプセルのコライダーの設定処理
//=============================================================================
void SetCapsuleCollider(int nIdx, CapsuleCollider CapsuleColliderInfo)
{
	Collider* pCollider = &g_aCollider[0];	// コライダーへのポインタ
	int nCntCollider = 0;					// カウント用変数

	for (nCntCollider = 0; nCntCollider < MAX_COLLIDER; nCntCollider++, pCollider++)
	{
		if (pCollider->bUse == true)
		{
			continue;
		}

		// 指定された当たり判定にコライダーを追加
		g_aCollision[nIdx].nColliderIdx[g_aCollision[nIdx].nNumCollider] = nCntCollider;

		// 各値代入
		pCollider->Collidertype.capsule = CapsuleColliderInfo;
		pCollider->type = COLLIDERTYPE_CAPSULE;
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
bool CollisionBoxToBox(CollisionInfo& _CollisionInfo, ColliderType MyCollider, ColliderType TargetCollider)
{
	// 分離軸判定ってなんですか？？？
	return false;
}

//=============================================================================
//	矩形と筒との当たり判定処理
//=============================================================================
bool CollisionBoxToCylinder(CollisionInfo& _CollisionInfo, ColliderType MyCollider, ColliderType TargetCollider)
{
	// から
	return false;
}

//=============================================================================
//	矩形と球との当たり判定処理
//=============================================================================
bool CollisionBoxToSphere(CollisionInfo& _CollisionInfo, ColliderType MyCollider, ColliderType TargetCollider)
{
	// 使いどころはありそう
	return false;
}

//=============================================================================
//	矩形とカプセルとの当たり判定処理
//=============================================================================
bool CollisionBoxToCapsule(CollisionInfo& _CollisionInfo, ColliderType MyCollider, ColliderType TargetCollider)
{
	// できたほうがいい
	return false;
}

//=============================================================================
//	筒と矩形との当たり判定処理
//=============================================================================
bool CollisionCylinderToBox(CollisionInfo& _CollisionInfo, ColliderType MyCollider, ColliderType TargetCollider)
{
	// 今はいらないもの
	return false;
}

//=============================================================================
//	筒と筒との当たり判定処理
//=============================================================================
bool CollisionCylinderToCylinder(CollisionInfo& _CollisionInfo, ColliderType MyCollider, ColliderType TargetCollider)
{
	// 各距離を二乗したものをすべて足す
	float fDiff = powf(MyCollider.cylinder.pos.x - TargetCollider.cylinder.pos.x, 2) + 
		powf(MyCollider.cylinder.pos.z - TargetCollider.cylinder.pos.z, 2);

	// 総合より自分と対象の半径を足して二乗したもののほうが大きければ当たっている
	if (fDiff <= powf(MyCollider.cylinder.fRadius + TargetCollider.cylinder.fRadius, 2))
	{// 当たっていたら
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
bool CollisionCylinderToSphere(CollisionInfo& _CollisionInfo, ColliderType MyCollider, ColliderType TargetCollider)
{
	// 誰も上に登ろうなんて思わないでほしい

	// 各距離を二乗したものをすべて足す
	float fDiff = powf(MyCollider.cylinder.pos.x - TargetCollider.cylinder.pos.x, 2) +
		powf(MyCollider.cylinder.pos.z - TargetCollider.cylinder.pos.z, 2);

	// 総合より自分と対象の半径を足して二乗したもののほうが大きければ当たっている
	if (fDiff <= powf(MyCollider.cylinder.fRadius + TargetCollider.cylinder.fRadius, 2))
	{// 当たっていたら
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
bool CollisionCylinderToCapsule(CollisionInfo& _CollisionInfo, ColliderType MyCollider, ColliderType TargetCollider)
{
	// カプセルってなんだろう
	return false;
}

//=============================================================================
//	球と矩形との当たり判定処理
//=============================================================================
bool CollisionSphereToBox(CollisionInfo& _CollisionInfo, ColliderType MyCollider, ColliderType TargetCollider)
{
	// できればやりたいね
	return false;
}

//=============================================================================
//	球と筒との当たり判定処理
//=============================================================================
bool CollisionSphereToCylinder(CollisionInfo& _CollisionInfo, ColliderType MyCollider, ColliderType TargetCollider)
{
	// 誰も上に登ろうなんて思わないでほしい

	// 各距離を二乗したものをすべて足す
	float fDiff = powf(MyCollider.sphere.pos.x - TargetCollider.cylinder.pos.x, 2) +
		powf(MyCollider.sphere.pos.z - TargetCollider.cylinder.pos.z, 2);

	// 総合より自分と対象の半径を足して二乗したもののほうが大きければ当たっている
	if (fDiff <= powf(MyCollider.sphere.fRadius + TargetCollider.cylinder.fRadius, 2))
	{// 当たっていたら
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
bool CollisionSphereToSphere(CollisionInfo& _CollisionInfo, ColliderType MyCollider, ColliderType TargetCollider)
{
	// 各距離を二乗したものをすべて足す
	float fDiff = powf(MyCollider.sphere.pos.x - TargetCollider.sphere.pos.x, 2) + 
		powf(MyCollider.sphere.pos.y - TargetCollider.sphere.pos.y, 2) + 
		powf(MyCollider.sphere.pos.z - TargetCollider.sphere.pos.z, 2);

	// 総合より自分と対象の半径を足して二乗したもののほうが大きければ当たっている
	if (fDiff <= powf(MyCollider.sphere.fRadius + TargetCollider.sphere.fRadius, 2))
	{// 当たっていたら
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
bool CollisionSphereToCapsule(CollisionInfo& _CollisionInfo, ColliderType MyCollider, ColliderType TargetCollider)
{
	// カプセルいらないのでは？
	return false;
}

void testCollison(D3DXVECTOR3* pPos, D3DXVECTOR3* pPosOld, D3DXVECTOR3* pMove, float fRadius)
{
	// ここ筒と筒らしい
#if 0
	D3DXVECTOR3 pos = INIT_D3DXVEC3;
	float ftestRadius = 1000.0f;

	float fDiff = powf(pPos->x - pos.x, 2) + powf(pPos->z - pos.z, 2);

	if (fDiff <= powf(ftestRadius + fRadius, 2))
	{
		float fAngle = atan2f(pPos->x - pos.x, pPos->z - pos.z);

		pPos->x = pos.x + sinf(fAngle) * (ftestRadius + fRadius);
		pPos->z = pos.z + cosf(fAngle) * (ftestRadius + fRadius);
	}
#endif

	// これ球と球ね
#if 0
	// 各距離を二乗したものをすべて足す
	float fDiff = powf(pos.x - pPos->x, 2) + powf(pos.y - pPos->y, 2) + powf(pos.z - pPos->z, 2);

	if (fDiff <= powf(fRadius + ftestRadius, 2))
	{// 当たっていたら
		D3DXVECTOR3 postest = (*pPos - pos);
		D3DXVECTOR3 postesst;
		D3DXVec3Normalize(&postesst, &postest);

		*pPos = (pos + (postesst * (ftestRadius + fRadius)));

	}
#endif
}