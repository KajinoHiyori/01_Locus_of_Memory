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
	EVENTTYPE_003_0,	// 植物の時戻し[コメントアウト済]
	EVENTTYPE_003_1,	// 植物の成長
	EVENTTYPE_003_2,	// 植物の燃焼
	EVENTTYPE_004_0,	// 植物の時戻し
	EVENTTYPE_005_0,	// 池の氷結
	EVENTTYPE_006_0,	// 池の時戻し
	EVENTTYPE_006_1,	// 池の融解
	EVENTTYPE_007_0,	// 花の水やり
	EVENTTYPE_007_1,	// 花の成長
	EVENTTYPE_008_0,	// 花が萎む
	EVENTTYPE_009_0,	// ドラゴンが眩しがる
	EVENTTYPE_009_1,	// ドラゴンが眩しがる
	EVENTTYPE_010_0,	// 街灯に灯りをともす
	EVENTTYPE_011_0,	// 鍛冶場に火をつける
	EVENTTYPE_011_1,	// 鍛冶場に火をつける
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
bool SetMagicEvent008(MAGICTYPE type, int nIdx);
bool SetMagicEvent009(MAGICTYPE type, int nIdx);
bool SetMagicEvent010(MAGICTYPE type, int nIdx);
bool SetMagicEvent011(MAGICTYPE type, int nIdx);

//*****************************************************************************
// イベントの更新処理
//*****************************************************************************
void SetEvent(EVENTTYPE type, int nIdx);

#endif // _EVENT_H_