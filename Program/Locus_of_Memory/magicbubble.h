//========================================================
// 
// 魔法取得時のUI表示処理[magicbubble.h]
// Author : KajinoHiyori
// 
//========================================================
#ifndef _MAGICBUBBLE_H_
#define	_MAGICBUBBLE_H_

// テクスチャの種類
typedef enum
{
	MAGICBUBBLETYPE_FAR = 0,	// 遠い状態での吹き出し
	MAGICBUBBLETYPE_NEAR,		// 近い状態での吹き出し
	MAGICBUBBLETYPE_GGG,		//緑緑緑
	MAGICBUBBLETYPE_RRR,		//赤赤赤
	MAGICBUBBLETYPE_BBB,		//青青青
	MAGICBUBBLETYPE_YYY,		//黄黄黄
	//赤赤緑 | 赤緑赤 | 緑赤赤
	MAGICBUBBLETYPE_RRG,
	MAGICBUBBLETYPE_RGR,
	MAGICBUBBLETYPE_GRR,
	//赤黄黄 | 黄赤黄 | 黄黄赤
	MAGICBUBBLETYPE_RYY,
	MAGICBUBBLETYPE_YRY,
	MAGICBUBBLETYPE_YYR,
	//青青緑 | 青緑青 | 緑青青
	MAGICBUBBLETYPE_BBG,
	MAGICBUBBLETYPE_BGB,
	MAGICBUBBLETYPE_GBB,
	//青緑緑 | 緑青緑 | 緑緑青
	MAGICBUBBLETYPE_BGG,
	MAGICBUBBLETYPE_GBG,
	MAGICBUBBLETYPE_GGB,
	//青黄黄 | 黄青黄 | 黄黄青
	MAGICBUBBLETYPE_BYY,
	MAGICBUBBLETYPE_YBY,
	MAGICBUBBLETYPE_YYB,
	//緑緑黄 | 緑黄緑 | 黄緑緑
	MAGICBUBBLETYPE_GGY,
	MAGICBUBBLETYPE_GYG,
	MAGICBUBBLETYPE_YGG,
	//赤緑青
	MAGICBUBBLETYPE_RGB,
	MAGICBUBBLETYPE_MAX
}MAGICBUBBLETYPE;

// プロトタイプ宣言
void InitMagicBubble(void);
void UninitMagicBubble(void);
void UpdateMagicBubble(void);
void DrawMagicBubble(void);
void SetMagicBubble(int nIdx, MAGICBUBBLETYPE type);

#endif