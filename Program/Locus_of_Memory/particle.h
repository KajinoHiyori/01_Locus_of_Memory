#ifndef _PARTICLE_H_
#define _PARTICLE_H_

typedef enum
{
	PARTICLETYPE_001 = 0,
	PARTICLETYPE_MAX
}PARTICLETYPE;

//プロトタイプ宣言
void InitParticle(void);
void UninitParticle(void);
void UpdateParticle(void);
void DrawParticle(void);
void SetParticle(D3DXVECTOR3 pos, D3DXCOLOR col,int nLife, PARTICLETYPE type);
#endif 

