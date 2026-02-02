//========================================================
// 
// 時計処理[clock.h]
// Author : KajinoHiyori
// 
//========================================================
#ifndef _CLOCK_H_
#define _CLOCK_H_

// 時計の稼働状態
typedef enum
{
	CLOCKSTATE_NONE = -1,	// 何もしていない状態
	CLOCKSTATE_OPERATION,	// 稼働中
	CLOCKSTATE_STOP,		// 停止中
	CLOCKSTATE_MAX
}CLOCKSTATE;

// プロトタイプ宣言
void InitClock(void);
void UninitClock(void);
void UpdateClock(void);
void DrawClock(void);
void SetClock(int nIdx, D3DXVECTOR3 pos);
void SetClockState(CLOCKSTATE clockState);
CLOCKSTATE GetClockState(void);
#endif
