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
#endif // !_COLOR_H_
