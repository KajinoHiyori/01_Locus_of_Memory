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
bool SetMagicEvent(MAGICEVENT event, MAGICTYPE type);

//*****************************************************************************
// 魔法イベント関連の処理用関数
//*****************************************************************************
bool SetMagicEvent001(MAGICTYPE type);
bool SetMagicEvent002(MAGICTYPE type);
bool SetMagicEvent003(MAGICTYPE type);
bool SetMagicEvent004(MAGICTYPE type);
bool SetMagicEvent005(MAGICTYPE type);
bool SetMagicEvent006(MAGICTYPE type);
bool SetMagicEvent007(MAGICTYPE type);


#endif // _EVENT_H_