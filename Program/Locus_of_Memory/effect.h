//========================================================
// 
// エフェクト処理[effect.h]
// Author : MatuhasiEito
// Author : KajinoHiyori
// 
//========================================================
#ifndef _EFFECT_H_
#define _EFFECT_H_

// エフェクトの種類
typedef enum
{
	EFFECT_TYPE_NORMAL = 0,	// 通常のエフェクト
	EFFECT_TYPE_GROWTH,
	EFFECT_TYPE_RAIN,
	EFFECT_TYPE_FLASH,
	EFFECT_TYPE_MAGICEF,	// マジックエフェクト
	EFFECT_TYPE_FIRE,
	EFFECT_TYPE_MAX
}EFFECT_TYPE;

// エフェクトのテクスチャの種類
typedef enum
{
	EFFECT_TEX_CIRCLE = 0,	// 円形のエフェクト
	EFFECT_TEX_DIAMOND,		// ダイヤ型のエフェクト
	EFFECT_TEX_WING000,		// 羽のエフェクト[上向き]
	EFFECT_TEX_WING001,		// 羽のエフェクト[下向き]
	EFFECT_TEX_WING002,		// 羽のエフェクト[横向き]
	EFFECT_TEX_MAX
}EFFECT_TEX;

//プロトタイプ宣言
void InitEffect(void);
void UninitEffect(void);
void UpdateEffect(void);
void DrawEffect(void);
void SetEffect(EFFECT_TYPE type, EFFECT_TEX tex, D3DXVECTOR3 pos,D3DXVECTOR3 move, D3DXCOLOR col, int nLife, float fRadius);
void ReleaseEffect(int nIdx);

#endif