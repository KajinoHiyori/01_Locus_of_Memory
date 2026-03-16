//========================================================================
// 
// パーティクル描画[magiceffct.h]
// Author : KAIRI MANABE
//
//========================================================================
#ifndef _MAGICEFFECT_H_
#define _MAGICEFFECT_H_

typedef enum
{
	MAGICEF_TYPE_NONE = -1,
	//ギミック用
	MAGICEF_TYPE_GIMMICK,
	//落ちている魔法用
	MAGICEF_TYPE_MAGIC,
	//鍛冶場のかまど
	MAGICEF_TYPE_FORGE,
	//焚火
	MAGICEF_TYPE_BONFIRE,
	//煙
	MAGICEF_TYPE_SMOKE,

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

