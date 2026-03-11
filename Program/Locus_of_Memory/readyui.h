//========================================================
// 
// 魔法取得時のUI表示処理[magicbubble.h]
// Author : KajinoHiyori
// 
//========================================================
#if 0
#ifndef _MAGICBUBBLE_H_
#define	_MAGICBUBBLE_H_
#include "magic.h"

// テクスチャの種類
typedef enum
{
	MAGICBUBBLETYPE_FAR = 0,	// 遠い状態での吹き出し
	MAGICBUBBLETYPE_NEAR,		// 近い状態での吹き出し
	MAGICBUBBLETYPE_GGG,
	MAGICBUBBLETYPE_RRR,
	MAGICBUBBLETYPE_BBB,
	MAGICBUBBLETYPE_YYY,
	MAGICBUBBLETYPE_RRG,
	MAGICBUBBLETYPE_RGR,
	MAGICBUBBLETYPE_GRR,
	MAGICBUBBLETYPE_RYY,
	MAGICBUBBLETYPE_YRY,
	MAGICBUBBLETYPE_YYR,
	MAGICBUBBLETYPE_BBG,
	MAGICBUBBLETYPE_BGB,
	MAGICBUBBLETYPE_GBB,
	MAGICBUBBLETYPE_BGG,
	MAGICBUBBLETYPE_GBG,
	MAGICBUBBLETYPE_GGB,
	MAGICBUBBLETYPE_BYY,
	MAGICBUBBLETYPE_YBY,
	MAGICBUBBLETYPE_YYB,
	MAGICBUBBLETYPE_GGY,
	MAGICBUBBLETYPE_GYG,
	MAGICBUBBLETYPE_YGG,
	MAGICBUBBLETYPE_RGB,
	MAGICBUBBLETYPE_MAX
}MAGICBUBBLETYPE;

// プロトタイプ宣言
void InitMagicBubble(void);
void UninitMagicBubble(void);
void UpdateMagicBubble(void);
void DrawMagicBubble(void);
void SetMagicBubble(int nIdx, COMMANDOREDER command, int nDistance);	// command = 魔法の種類を取得してテクスチャを変更 / nDistance = 距離によってfar[1]/near[0]/不使用[-1]を切り替える
void ResetMagicBubble(int nIdx);

#endif
#endif