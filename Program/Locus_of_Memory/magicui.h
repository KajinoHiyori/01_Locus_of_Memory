//========================================================
// 
// 魔導書のUI表示処理[magicui.h]
// Author : KajinoHiyori
// 
//========================================================
#ifndef _MAGICUI_H_
#define	_MAGICUI_H_

// UIのテクスチャの種類
typedef enum
{
	MAGICUI_TEX_NULL = 0,	// NULL
	MAGICUI_TEX_GGG,		// GGG
	MAGICUI_TEX_RRR,		// RRR
	MAGICUI_TEX_BBB,		// BBB
	MAGICUI_TEX_YYY,		// YYY
	MAGICUI_TEX_RRG,		// RRG
	MAGICUI_TEX_RGR,		// RGR
	MAGICUI_TEX_GRR,		// GRR
	MAGICUI_TEX_RYY,		// RYY
	MAGICUI_TEX_YRY,		// YRY
	MAGICUI_TEX_YYR,		// YYR
	MAGICUI_TEX_BBG,		// BBG
	MAGICUI_TEX_BGB,		// BGB	
	MAGICUI_TEX_GBB,		// GBB
	MAGICUI_TEX_BGG,		// BGG
	MAGICUI_TEX_GBG,		// GBG
	MAGICUI_TEX_GGB,		// GGB
	MAGICUI_TEX_BYY,		// BYY
	MAGICUI_TEX_YBY,		// YBY
	MAGICUI_TEX_YYB,		// YYB
	MAGICUI_TEX_GGY,		// GGY
	MAGICUI_TEX_GYG,		// GYG
	MAGICUI_TEX_YGG,		// YGG
	MAGICUI_TEX_RGB,		// RGB
	MAGICUI_TEX_MAX
}MAGICUI_TEX;					
								
// UIの種類
typedef enum
{
	MAGICUI_TYPE_MAGIC0 = 0,	// 1つ目の発動された魔法
	MAGICUI_TYPE_MAGIC1,		// 2つ目の発動された魔法
	MAGICUI_TYPE_MAGIC2,		// 3つ目の発動された魔法
	MAGICUI_TYPE_MAGIC3,		// 4つ目の発動された魔法
	MAGICUI_TYPE_MAX
}MAGICUI_TYPE;

// プロトタイプ宣言
void InitMagicUI(void);
void UninitMagicUI(void);
void UpdateMagicUI(void);
void DrawMagicUI(void);
void SetMagicTexture(int nIdx);
void SetMagicUIAppear(int nIdx);
void SetMagicUIDisplay(int nIdx);
void SetMagicUIDisappear(int nIdx);
void SetMagicUINonDisplay(int nIdx);

#endif
