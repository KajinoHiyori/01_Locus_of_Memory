//=============================================================================
//
//	画面上の粒の処理 [grain.h]
//	Author : SHUMA AIZU
// 
//=============================================================================

#ifndef _GRAIN_H_
#define _GRAIN_H_

//*****************************************************************************
// 種類
//*****************************************************************************
typedef enum
{
	GRAINTYPE_A = 0,
	GRAINTYPE_B,
	GRAINTYPE_C,
	GRAINTYPE_D,
	GRAINTYPE_E,
	GRAINTYPE_F,
	GRAINTYPE_G,
	GRAINTYPE_H,
	GRAINTYPE_I,
	GRAINTYPE_J,
	GRAINTYPE_K,
	GRAINTYPE_L,
	GRAINTYPE_N,
	GRAINTYPE_M,
	GRAINTYPE_O,
	GRAINTYPE_P,
	GRAINTYPE_Q,
	GRAINTYPE_R,
	GRAINTYPE_S,
	GRAINTYPE_T,
	GRAINTYPE_U,
	GRAINTYPE_V,
	GRAINTYPE_W,
	GRAINTYPE_X,
	GRAINTYPE_Y,
	GRAINTYPE_MAX
}GRAINTYPE;

//*****************************************************************************
// 構造体定義
//*****************************************************************************
typedef struct Grain
{
	D3DXVECTOR3 pos;			// 位置
	D3DXVECTOR3 move;			// 移動量
	D3DXCOLOR	col;			// 色
	D3DXMATRIX	mtxWorld;		// ワールドマトリックス
	GRAINTYPE	type;			// 種類
	float		fRadius;		// 半径
	float		fAngle;			// 角度
	float		fSpeed;			// 速度
	int			nLife;			// 寿命（色）
	bool		bUse;			// 使用しているか
}Grain;

//*****************************************************************************
// プロトタイプ宣言
//*****************************************************************************
void InitGrain(void);
void UninitGrain(void);
void UpdateGrain(void);
void DrawGrain(void);
void SetGrain(void);
void ReleaseGrain(int nIdx);

#endif	// _GRAIN_H_