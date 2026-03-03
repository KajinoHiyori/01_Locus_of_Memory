//=============================================================================
//
//	イベント関連処理 [event.h]
//	Author : SHUMA AIZU
// 
//=============================================================================
#include "event.h"
#include "object.h"
#include "particle.h"
#include "motion.h"
#include "magic.h"
#include "debugproc.h"

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
// 各魔法イベント処理[HOUSE_000のイベント]
//========================================================================
bool SetMagicEvent001(MAGICTYPE type, int nIdx)
{
	ParentObject* pParentObject = GetParentObjectInfo(nIdx);

	switch (type)
	{
	case MAGICTYPE_TIMEREVERT:

		return true;

		// 燃焼イベント
	case MAGICTYPE_COMBUSTION:
		pParentObject->EventType = EVENTTYPE_001_1;
		SetParticle(pParentObject->pos, 150, PARTICLETYPE_COMBUSTION);
		SetMotion(&pParentObject->motion, pParentObject->pModelData, &pParentObject->OffSetData, MOTIONTYPE_MOVE, false, false, 10);
		return true;

	default:
		return false;
	}
}

//========================================================================
// 各魔法イベント処理[BRIDGEのイベント]
//========================================================================
bool SetMagicEvent002(MAGICTYPE type, int nIdx)
{
	ParentObject* pParentObject = GetParentObjectInfo(nIdx);

	switch (type)
	{
	case MAGICTYPE_TIMEREVERT:
		pParentObject->EventType = EVENTTYPE_002_0;
		SetParticle(pParentObject->pos, 150, PARTICLETYPE_TIMEREVERT);
		SetMotion(&pParentObject->motion, pParentObject->pModelData, &pParentObject->OffSetData, MOTIONTYPE_MOVE, false, false, 10);
		return true;

	case MAGICTYPE_COMBUSTION:

		return true;

	case MAGICTYPE_LEVITATION:

		return true;

	default:
		return false;
	}
}

//========================================================================
// 各魔法イベント処理[PLANTのイベント][燃焼/成長]
//========================================================================
bool SetMagicEvent003(MAGICTYPE type, int nIdx)
{
	ParentObject* pParentObject = GetParentObjectInfo(nIdx);

	switch (type)
	{
#if 0
	case MAGICTYPE_TIMEREVERT:	// 成長前に戻る
		pParentObject->EventType = EVENTTYPE_003_0;
		SetParticle(pParentObject->pos, 150, PARTICLETYPE_TIMEREVERT);
		SetMotion(&pParentObject->motion, pParentObject->pModelData, &pParentObject->OffSetData, MOTIONTYPE_MOVE, false, true, 10);
		SetMagicLocus(MAGICEVENT_004, pParentObject->pos, 500.0f, pParentObject->nEventIdx);
		return true;
#endif

	case MAGICTYPE_GROWTH:	// 成長させる
		pParentObject->EventType = EVENTTYPE_003_1;
		SetParticle(pParentObject->pos, 150, PARTICLETYPE_GROWTH);
		SetMotion(&pParentObject->motion, pParentObject->pModelData, &pParentObject->OffSetData, MOTIONTYPE_MOVE, false, true, 10);
		SetMagicLocus(MAGICEVENT_004, pParentObject->pos, 500.0f, pParentObject->nEventIdx);
		return true;

	case MAGICTYPE_RAINPRAY:	// 成長させる
		pParentObject->EventType = EVENTTYPE_003_1;
		SetParticle(pParentObject->pos, 150, PARTICLETYPE_GROWTH);
		SetMotion(&pParentObject->motion, pParentObject->pModelData, &pParentObject->OffSetData, MOTIONTYPE_MOVE, false, true, 10);
		SetMagicLocus(MAGICEVENT_004, pParentObject->pos, 500.0f, pParentObject->nEventIdx);
		return true;

	case MAGICTYPE_COMBUSTION:	// 燃焼させる
		pParentObject->EventType = EVENTTYPE_003_2;
		SetParticle(pParentObject->pos, 150, PARTICLETYPE_TIMEREVERT);
		SetMotion(&pParentObject->motion, pParentObject->pModelData, &pParentObject->OffSetData, MOTIONTYPE_RUN, false, false, 10);
		return true;

	case MAGICTYPE_FIREBALL:	// 燃焼させる
		pParentObject->EventType = EVENTTYPE_003_2;
		SetParticle(pParentObject->pos, 150, PARTICLETYPE_TIMEREVERT);
		SetMotion(&pParentObject->motion, pParentObject->pModelData, &pParentObject->OffSetData, MOTIONTYPE_RUN, false, false, 10);
		return true;

	default:
		return false;
	}
}

//========================================================================
// 各魔法イベント処理[PLANTのイベント][時戻し]
//========================================================================
bool SetMagicEvent004(MAGICTYPE type, int nIdx)
{
	ParentObject* pParentObject = GetParentObjectInfo(nIdx);

	switch (type)
	{
	case MAGICTYPE_TIMEREVERT:	// 成長前に戻る
		pParentObject->EventType = EVENTTYPE_003_0;
		SetParticle(pParentObject->pos, 150, PARTICLETYPE_TIMEREVERT);
		SetMotion(&pParentObject->motion, pParentObject->pModelData, &pParentObject->OffSetData, MOTIONTYPE_ACTION, false, false, 10);
		SetMagicLocus(MAGICEVENT_003, pParentObject->pos, 500.0f, pParentObject->nEventIdx); 
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

//========================================================================
// イベントの設置
//========================================================================
void SetEvent(EVENTTYPE type, int nIdx)
{
	ParentObject* pParentObject = GetParentObjectInfo(nIdx);

	pParentObject->EventType = type;
}
