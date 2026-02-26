//=============================================================================
//
//	当たり判定の管理 [collision.h]
//	Author : SHUMA AIZU
// 
//=============================================================================

#ifndef _COLLISION_H_
#define _COLLISION_H_

#include "main.h"

//*****************************************************************************
// マクロ定義
//*****************************************************************************
#define MAX_ONECOLLIDER			(5)		// ひとつのオブジェクトが持てるコライダーの最大

//*****************************************************************************
// タグの種類
//*****************************************************************************
typedef enum TAGTYPE
{
	TAGTYPE_PLAYER = 0,
	TAGTYPE_OBJECT,
	TAGTYPE_MAX
}TAGTYPE;

//*****************************************************************************
// コライダーの種類
//*****************************************************************************
typedef enum COLLIDERTYPE
{
	COLLIDERTYPE_BOX = 0,
	COLLIDERTYPE_SPHERE,
	COLLIDERTYPE_CAPSULE,
	COLLIDERTYPE_MAX
}COLLIDERTYPE;

//*****************************************************************************
// 矩形のコライダーの構造体定義
//*****************************************************************************
typedef struct BoxCollider
{
	float fWidth;		// 幅
	float fHeight;		// 高さ
	float fDepth;		// 奥行き
}BoxCollider;

//*****************************************************************************
// 球のコライダーの構造体定義
//*****************************************************************************
typedef struct SphereCollider
{
	float fRadius;		// 半径
}SphereCollider;

//*****************************************************************************
// カプセルのコライダーの構造体定義
//*****************************************************************************
typedef struct CapsuleCollider
{
	float fRadius;		// 半径
	float fHeight;		// 高さ
}CapsuleCollider;

union ColliderType
{// コライダーの種類
	BoxCollider box;			// 矩形
	SphereCollider sphere;		// 球
	CapsuleCollider capsule;	// カプセル
}ColliderType;

//*****************************************************************************
// コライダーの構造体定義
//*****************************************************************************
typedef struct Collider
{
	D3DXVECTOR3 pos;				// 原点
	D3DXVECTOR3 rot;				// 向き
	//COLLIDERTYPE type;				// 種類
	union ColliderType type;
	D3DXMATRIX mtxWorld;			// ワールドマトリックス
	D3DXMATRIX *mtxParent;			// 親マトリックス
}Collider;

//*****************************************************************************
// 当たり判定の構造体定義
//*****************************************************************************
typedef struct Collision
{
	int nIdx;								// 識別番号
	int nColliderIdx[MAX_ONECOLLIDER];		// 使用するコライダーの番号
	int nNumCollider;						// 使用しているコライダーの数
	bool bUse;								// 使用状態
	UINT tag;								// タグ
}Collision;

//*****************************************************************************
// プロトタイプ宣言
//*****************************************************************************
void InitCollision(void);
void UninitCollision(void);
void UpdateCollision(int nIdx, UINT TargetTag);
void DrawCollision(void);
int SetCollision(void);
void SetBoxCollider(int nIdx);

#endif // !_COLLISION_H_