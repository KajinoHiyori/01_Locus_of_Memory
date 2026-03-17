//========================================================================
// 
// パーティクル描画[effectfire.h]
// Author : KAIRI MANABE
//
//========================================================================
#ifndef _EFFECTFIRE_H_
#define _EFFECTFIRE_H_

typedef enum
{
	FIRE_TYPE_NONE = -1,
	//鍛冶場のかまど
	FIRE_TYPE_FORGE,
	//焚火
	FIRE_TYPE_BONFIRE,
	//煙
	FIRE_TYPE_SMOKE,

	FIRE_TYPE_MAX
}FIRE_TYPE;

//プロトタイプ宣言
void InitEffectFire(void);
void UninitEffectFire(void);
void UpdateEffectFire(void);
void DrawEffectFire(void);
void SetEffectFire(D3DXVECTOR3 pos, FIRE_TYPE type, int nIdx);
void StopEffectFire(int nIdx);
#endif 

