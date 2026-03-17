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
#define COLOR_DARKGRAY		(D3DXCOLOR(0.439f,0.501f,0.564f,1.0f))
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

#define COLOR_SKY00			(D3DXCOLOR(0.000f,	0.000f,	0.180f,	1.0f))
#define COLOR_SKY01			(D3DXCOLOR(0.035f,	0.000f,	0.349f,	1.0f))
#define COLOR_SKY02			(D3DXCOLOR(0.082f,	0.066f,	0.435f,	1.0f))
#define COLOR_SKY03			(D3DXCOLOR(0.184f,	0.219f,	0.576f,	1.0f))
#define COLOR_SKY04			(D3DXCOLOR(0.313f,	0.349f,	0.650f,	1.0f))
#define COLOR_SKY05			(D3DXCOLOR(0.486f,	0.690f,	0.878f,	1.0f))
#define COLOR_SKY06			(D3DXCOLOR(0.725f,	0.890f,	0.976f,	1.0f))
#define COLOR_SKY07			(D3DXCOLOR(0.662f,	0.862f,	0.968f,	1.0f))
#define COLOR_SKY08			(D3DXCOLOR(0.576f,	0.835f,	0.968f,	1.0f))
#define COLOR_SKY09			(D3DXCOLOR(0.486f,	0.807f,	0.956f,	1.0f))
#define COLOR_SKY10			(D3DXCOLOR(0.384f,	0.780f,	0.952f,	1.0f))
#define COLOR_SKY11			(D3DXCOLOR(0.262f,	0.752f,	0.941f,	1.0f))
#define COLOR_SKY12			(D3DXCOLOR(0.117f,	0.725f,	0.937f,	1.0f))
#define COLOR_SKY13			(D3DXCOLOR(0.325f,	0.764f,	0.945f,	1.0f))
#define COLOR_SKY14			(D3DXCOLOR(0.572f,	0.823f,	0.862f,	1.0f))
#define COLOR_SKY15			(COLOR_SKY14)
//#define COLOR_SKY15			(D3DXCOLOR(0.749f,	0.807f,	0.576f,	1.0f))
#define COLOR_SKY16			(D3DXCOLOR(0.862f,	0.682f,	0.305f,	1.0f))
#define COLOR_SKY17			(D3DXCOLOR(0.929f,	0.427f,	0.000f,	1.0f))
#define COLOR_SKY18			(COLOR_SKY19)
//#define COLOR_SKY18			(D3DXCOLOR(0.705f,	0.313f,	0.305f,	1.0f))
#define COLOR_SKY19			(COLOR_SKY20)
//#define COLOR_SKY19			(D3DXCOLOR(0.443f,	0.215f,	0.443f,	1.0f))
#define COLOR_SKY20			(COLOR_SKY21)
//#define COLOR_SKY20			(D3DXCOLOR(0.113f,	0.125f,	0.537f,	1.0f))
#define COLOR_SKY21			(D3DXCOLOR(0.086f,	0.082f,	0.450f,	1.0f))
#define COLOR_SKY22			(D3DXCOLOR(0.086f,	0.082f,	0.450f,	1.0f))
#define COLOR_SKY23			(D3DXCOLOR(0.058f,	0.031f,	0.396f,	1.0f))
#define COLOR_SKY24			(D3DXCOLOR(0.203f,	0.176f,	0.400f,	1.0f))

#define COLOR_SKY_NOON		(COLOR_SKY12)
#define COLOR_SKY_NIGHT		(COLOR_SKY24)
#endif // !_COLOR_H_
