#ifndef _PARTICLE_H_
#define _PARTICLE_H_

typedef enum
{
	PARTICLETYPE_001 = 0,
	//浮遊
	PARTICLETYPE_LEVITATION,
	//燃焼
	PARTICLETYPE_COMBUSTION,
	//洪水、氾濫
	PARTICLETYPE_FLOOD,
	//フラッシュ
	PARTICLETYPE_FLASH,
	//火球
	PARTICLETYPE_FIREBALL,
	//太陽の動きを遅延する
	PARTICLETYPE_SUNSETDELAY,
	//雨乞い
	PARTICLETYPE_RAINPRAY,
	//凍結
	PARTICLETYPE_FREEZE,
	//成長(植物など)
	PARTICLETYPE_GROWTH,
	//加速
	PARTICLETYPE_ACCELERATION,
	//時間の巻き戻し(回帰)
	PARTICLETYPE_TIMEREVERT,

	PARTICLETYPE_MAX
}PARTICLETYPE;

//プロトタイプ宣言
void InitParticle(void);
void UninitParticle(void);
void UpdateParticle(void);
void DrawParticle(void);
void SetParticle(D3DXVECTOR3 pos, D3DXCOLOR col,int nLife, PARTICLETYPE type);
#endif 

