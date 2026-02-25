//=============================================================================
//
//	イベント関連処理 [event.h]
//	Author : SHUMA AIZU
// 
//=============================================================================

#ifndef _EVENT_H_
#define _EVENT_H_

#include "main.h"
#include "magic.h"

//*****************************************************************************
// マクロ定義
//*****************************************************************************

//*****************************************************************************
// イベントの種類
//*****************************************************************************
typedef enum EVENTTYPE
{
	EVENTTYPE_NONE = -1,
	EVENTTYPE_001_0,
	EVENTTYPE_001_1,	// 家の燃焼
	EVENTTYPE_002_0,	// 橋の時戻し
	EVENTTYPE_MAX,
}EVENTTYPE;

//*****************************************************************************
// プロトタイプ宣言
//*****************************************************************************
bool SetMagicEvent(MAGICEVENT event, MAGICTYPE type, int nIdx);

//*****************************************************************************
// 魔法イベント関連の処理用関数
//*****************************************************************************
bool SetMagicEvent001(MAGICTYPE type, int nIdx);
bool SetMagicEvent002(MAGICTYPE type, int nIdx);
bool SetMagicEvent003(MAGICTYPE type, int nIdx);
bool SetMagicEvent004(MAGICTYPE type, int nIdx);
bool SetMagicEvent005(MAGICTYPE type, int nIdx);
bool SetMagicEvent006(MAGICTYPE type, int nIdx);
bool SetMagicEvent007(MAGICTYPE type, int nIdx);


#endif // _EVENT_H_