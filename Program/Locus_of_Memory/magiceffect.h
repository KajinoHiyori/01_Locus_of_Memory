#ifndef _MAGICEFFECT_H_
#define _MAGICEFFECT_H_

typedef enum
{
	MAGICEF_TYPE_NONE = -1,
	//浮遊
	MAGICEF_TYPE_LEVITATION,
	//燃焼
	MAGICEF_TYPE_COMBUSTION,
	//洪水、氾濫
	MAGICEF_TYPE_FLOOD,
	//フラッシュ
	MAGICEF_TYPE_FLASH,
	//火球
	MAGICEF_TYPE_FIREBALL,
	//太陽の動きを遅延する
	MAGICEF_TYPE_SUNSETDELAY,
	//雨乞い
	MAGICEF_TYPE_RAINPRAY,
	//凍結
	MAGICEF_TYPE_FREEZE,
	//成長(植物など)
	MAGICEF_TYPE_GROWTH,
	//加速
	MAGICEF_TYPE_ACCELERATION,
	//時間の巻き戻し(回帰)
	MAGICEF_TYPE_TIMEREVERT,

	MAGICEF_TYPE_MAX
}MAGICEF_TYPE;

//プロトタイプ宣言
void InitMagicEffect(void);
void UninitMagicEffect(void);
void UpdateMagicEffect(void);
void DrawMagicEffect(void);
void SetMagicEffect(D3DXVECTOR3 pos, int nLife, MAGICEF_TYPE type);
#endif 

