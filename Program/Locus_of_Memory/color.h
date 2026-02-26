//=============================================================================
//
//	色管理ヘッダーファイル [color.h]
//	Author : 
// 
//=============================================================================

#ifndef _COLOR_H_
#define _COLOR_H_

#define COL_MAX				(1000)		// 色の最大数
#define RAND_COLOR			((float)(rand() % COL_MAX) / COL_MAX)	// 配色のランダム

#define COLOR_RED			(D3DXCOLOR(1.0f,0.0f,0.0f,1.0f))
#define COLOR_GREEN			(D3DXCOLOR(0.0f,0.5f,0.0f,1.0f))
#define COLOR_BLUE			(D3DXCOLOR(0.0f,0.0f,1.0f,1.0f))
#define COLOR_YELLOW		(D3DXCOLOR(1.0f,1.0f,0.0f,1.0f))
#define COLOR_WHITE			(D3DXCOLOR(1.0f,1.0f,1.0f,1.0f))
#define COLOR_BLACK			(D3DXCOLOR(0.0f,0.0f,0.0f,0.0f))
#define COLOR_CYAN			(D3DXCOLOR(0.0f,1.0f,1.0f,1.0f))
#define COLOR_MAGENTA		(D3DXCOLOR(1.0f,0.0f,1.0f,1.0f))
#define COLOR_VIOLET		(D3DXCOLOR(0.933f,0.509f,0.933f,1.0f))
#define COLOR_ORANGE		(D3DXCOLOR(1.0f,0.647f,0.0f,1.0f))
#define COLOR_LIMEGREEN		(D3DXCOLOR(0.0f,1.0f,0.0f,1.0f))
#define COLOR_SKYBLUE		(D3DXCOLOR(0.529f,0.807f,0.921f,1.0f))
#define COLOR_HISUI			(D3DXCOLOR(0.219f,0.705f,0.545f,1.0f))
#define COLOR_RETROFILTER	(D3DXCOLOR(1.000f,	1.000f,	1.000f,	0.080f))
#define COLOR_DISALPHA		(D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.35f))
#define COLOR_GREENBATTERY	(D3DXCOLOR(0.027f, 1.000f, 0.525f, 1.0f))
#define COLOR_YELLOWBATTERY	(D3DXCOLOR(1.000f, 0.615f, 0.011f, 1.00f))
#define COLOR_REDBATTERY	(D3DXCOLOR(0.85f, 0.00f, 0.00f, 1.0f))
#define COLOR_UIBUBBLE		(D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.75f))
#define COLOR_RANDOM		(D3DXCOLOR(RAND_COLOR, RAND_COLOR, RAND_COLOR, 1.0f))
#define COLOR_SKY000		(D3DXCOLOR(0.262f, 0.909f, 1.000, 1.0f))
#define COLOR_SKY001		(D3DXCOLOR(0.035f, 0.678f, 1.000f, 1.0f))
#define COLOR_SKY002		(D3DXCOLOR(0.015f, 1.000f, 0.933f, 1.0f))
#define COLOR_SKY003		(D3DXCOLOR(1.000f, 0.827f, 0.027f, 1.0f))
#define COLOR_SKY004		(D3DXCOLOR(0.949f, 0.482f, 0.200f, 1.0f))
#define COLOR_SKY005		(D3DXCOLOR(0.623f, 0.333f, 0.678f, 1.0f))
#define COLOR_SKY006		(D3DXCOLOR(0.082f, 0.074f, 0.678f, 1.0f))
#define COLOR_SKY007		(D3DXCOLOR(0.062f, 0.054f, 0.509f, 1.0f))
#define COLOR_SKY_NOON		(COLOR_SKY001)
#define COLOR_SKY_NIGHT		(COLOR_SKY007)
#endif // !_COLOR_H_
