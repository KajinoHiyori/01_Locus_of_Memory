//=============================================================================
//
//	2Dエフェクトの管理 [2deffect.h]
//	Author : Kajino Hiyori
// 
//=============================================================================
#ifndef _2DEFFECT_H_
#define	_2DEFFECT_H_
#include "main.h"

// マクロ定義
#define MAX_EFFECT2D		(128)	// エフェクトの最大数
#define EFFECT2D_RADIUS		(6.0f)	// エフェクトの半径
#define EFFECT2D_LIFE		(50)	// エフェクトの寿命
#define EFFECT2D_NOT_MOVE	(D3DXVECTOR3(0.0f, 0.0f, 0.0f))	// エフェクトが移動しない場合

// エフェクトの種類
typedef enum
{
	EFFECTTYPE2D_NONE = 0,	// 何もしていない状態
	EFFECTTYPE2D_NORMAL,		// 通常状態
	EFFECTTYPE2D_MAX
}EFFECTTYPE2D;

// エフェクト構造体の定義
typedef struct
{
	EFFECTTYPE2D	type;		// エフェクトの種類
	D3DXVECTOR3		pos;		// 位置
	D3DXVECTOR3		move;		// 移動量
	D3DXVECTOR3		dest;		// 目的地
	D3DXCOLOR		col;		// 色
	float			fRadius;	// 半径(大きさ)
	int				nLife;		// 寿命
	bool			bUse;		// 使用しているかどうか
}Effect2D;

// プロトタイプ宣言
void Init2DEffect(void);
void Uninit2DEffect(void);
void Update2DEffect(void);
void Draw2DEffect(void);
void Set2DEffect(EFFECTTYPE2D type, D3DXVECTOR3 pos, D3DXVECTOR3 move, D3DXVECTOR3 dest, D3DXCOLOR col, float fRadius, int nLife);
#endif

