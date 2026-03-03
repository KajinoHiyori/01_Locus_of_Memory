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
	CLOCKSTATE_MAGIC,		// 魔法による停止中
	CLOCKSTATE_REVERSE,		// 時間が巻き戻る[タイトル用]
	CLOCKSTATE_MAX
}CLOCKSTATE;

// プロトタイプ宣言
void InitClock(void);
void UninitClock(void);
void UpdateClock(void);
void DrawClock(void);
void SetClock(int nIdx);
void DisappearClock(int nIdx);
CLOCKSTATE GetClockState(void);
void SetTime(int nHour, int nMinute);
void SetClockTowerIdx(int nIdx);
void SetClockState(CLOCKSTATE clockState);
void InitTimer(void);
int GetTime(void);
int GetMinute(void);
#endif
