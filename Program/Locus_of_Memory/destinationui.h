//========================================================
// 
// 目的地の方向を示すUI表示処理[destinationui.h]
// Author : KajinoHiyori
// 
//========================================================
#ifndef _DESTINATIONUI_H_
#define	_DESTINATIONUI_H_

// プロトタイプ宣言
void InitDestinationUI(void);
void UninitDestinationUI(void);
void UpdateDestinationUI(void);
void DrawDestinationUI(void);
float DistanceDestination(int nIdx);
void UpdateArrowPos(int nIdx);

#endif