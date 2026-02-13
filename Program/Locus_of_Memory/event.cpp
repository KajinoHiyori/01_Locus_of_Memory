//=============================================================================
//
//	イベント関連処理 [event.h]
//	Author : SHUMA AIZU
// 
//=============================================================================

#include "event.h"
#include "object.h"
#include "particle.h"

//*****************************************************************************
// マクロ定義
//*****************************************************************************

//*****************************************************************************
// グローバル変数
//*****************************************************************************

bool(*SetMagicEventSelecter[MAGICEVENT_MAX])(MAGICTYPE type, int nIdx) =
{
	NULL,
	SetMagicEvent001,
	SetMagicEvent002,
	SetMagicEvent003,
	SetMagicEvent004,
	SetMagicEvent005,
	SetMagicEvent006,
	SetMagicEvent007,
};

//========================================================================
// 魔法イベントを発生させる
//========================================================================
bool SetMagicEvent(MAGICEVENT event, MAGICTYPE type, int nIdx)
{
	bool isEvent = false;

	if (SetMagicEventSelecter[event] != NULL)
	{
		isEvent = SetMagicEventSelecter[event](type, nIdx);
	}

	return isEvent;
}

//========================================================================
// 各魔法イベント処理
//========================================================================
bool SetMagicEvent001(MAGICTYPE type, int nIdx)
{
	switch (type)
	{
	case MAGICTYPE_TIMEREVERT:

		return true;

	case MAGICTYPE_COMBUSTION:
		SetParticle(GetObjectInfo(nIdx)->pos, 300, PARTICLETYPE_COMBUSTION);
		return true;

	default:
		return false;
	}
}

bool SetMagicEvent002(MAGICTYPE type, int nIdx)
{
	switch (type)
	{
	case MAGICTYPE_TIMEREVERT:

		return true;

	case MAGICTYPE_COMBUSTION:

		return true;

	case MAGICTYPE_LEVITATION:

		return true;

	default:
		return false;
	}
}

bool SetMagicEvent003(MAGICTYPE type, int nIdx)
{
	switch (type)
	{
	case MAGICTYPE_TIMEREVERT:

		return true;

	case MAGICTYPE_GROWTH:

		return true;

	case MAGICTYPE_COMBUSTION:
		
		return true;

	case MAGICTYPE_FIREBALL:

		return true;

	default:
		return false;
	}
}

bool SetMagicEvent004(MAGICTYPE type, int nIdx)
{
	switch (type)
	{
	case MAGICTYPE_FLASH:

		return true;

	case MAGICTYPE_COMBUSTION:

		return true;

	case MAGICTYPE_FIREBALL:

		return true;

	default:
		return false;
	}
}

bool SetMagicEvent005(MAGICTYPE type, int nIdx)
{
	switch (type)
	{
	case MAGICTYPE_FLASH:

		return true;

	case MAGICTYPE_RAINPRAY:

		return true;

	case MAGICTYPE_FLOOD:

		return true;

	default:
		return false;
	}
}

bool SetMagicEvent006(MAGICTYPE type, int nIdx)
{
	switch (type)
	{
	case MAGICTYPE_FREEZE:

		return true;

	case MAGICTYPE_LEVITATION:

		return true;

	default:
		return false;
	}
}

bool SetMagicEvent007(MAGICTYPE type, int nIdx)
{
	switch (type)
	{
	case MAGICTYPE_FLOOD:

		return true;

	case MAGICTYPE_LEVITATION:

		return true;

	default:
		return false;
	}
}