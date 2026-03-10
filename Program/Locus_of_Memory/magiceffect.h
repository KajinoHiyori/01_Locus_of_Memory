#ifndef _MAGICEFFECT_H_
#define _MAGICEFFECT_H_

typedef enum
{
	MAGICEF_TYPE_NONE = -1,
	//ギミック用
	MAGICEF_TYPE_GIMMICK,
	//落ちている魔法用
	MAGICEF_TYPE_MAGIC,

	MAGICEF_TYPE_MAX
}MAGICEF_TYPE;

//プロトタイプ宣言
void InitMagicEffect(void);
void UninitMagicEffect(void);
void UpdateMagicEffect(void);
void DrawMagicEffect(void);
void SetMagicEffect(D3DXVECTOR3 pos, MAGICEF_TYPE type, int nIdx);
void StopMagicEffect(int nIdx);
#endif 

