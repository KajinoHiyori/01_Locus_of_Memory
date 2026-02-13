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