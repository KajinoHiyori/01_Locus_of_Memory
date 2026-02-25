//=============================================================================
//
//	2Dエフェクトの管理 [2deffect.h]
//	Author : Kajino Hiyori
// 
//=============================================================================
#ifndef _2DEFFECT_H_
#define	_2DEFFECT_H_
#include "main.h"

#if 0
// マクロ定義
#define MAX_EFFECT			(16384)	// エフェクトの最大数
#define EFFECT_RADIUS		(6.0f)	// エフェクトの半径
#define EFFECT_LIFE			(50)	// エフェクトの寿命
#define EFFECT_NOT_MOVE		D3DXVECTOR3(0.0f, 0.0f, 0.0f)		// エフェクトが移動しない場合

// エフェクトの種類
typedef enum
{
	EFFECTTYPE_NONE = 0,	// 何もしていない状態
	EFFECTTYPE_NORMAL,		// 通常状態
	EFFECTTYPE_ABSORPTION,	// 吸収状態
	EFFECTTYPE_MAX
}EFFECTTYPE;

// エフェクト構造体の定義
typedef struct
{
	EFFECTTYPE type;	// エフェクトの種類
	D3DXVECTOR3	pos;	// 位置
	D3DXVECTOR3 move;	// 移動量
	D3DXVECTOR3 dest;	// 目的地
	D3DXCOLOR	col;	// 色
	float fRadius;		// 半径(大きさ)
	int nLife;			// 寿命
	bool bUse;			// 使用しているかどうか
}Effect;

// プロトタイプ宣言
void InitEffect(void);
void UninitEffect(void);
void UpdateEffect(void);
void DrawEffect(void);
void SetEffect(EFFECTTYPE type, D3DXVECTOR3 pos, D3DXVECTOR3 move, D3DXVECTOR3 dest, D3DXCOLOR col, float fRadius, int nLife);
Effect* GetEffect(void);
#endif

#endif
