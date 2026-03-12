//=============================================================================
//
//	イベント関連処理 [event.h]
//	Author : SHUMA AIZU
// 
//=============================================================================
#include "event.h"
#include "object.h"
#include "eventobject.h"
#include "particle.h"
#include "motion.h"
#include "magic.h"
#include "debugproc.h"
#include "sound.h"
#include "questui.h"

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
	SetMagicEvent008,
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
	EventObject* pEventObject = GetEventObject(nIdx);
	bool isSuccess = false;

	switch (type)
	{
		// 燃焼イベント
	case MAGICTYPE_COMBUSTION:
		if (nIdx == 0)	// 1P
		{
			PlaySound(SOUND_LABEL_COMBUSTION0);
		}
		else if (nIdx == 1)	// 2P
		{
			PlaySound(SOUND_LABEL_COMBUSTION1);
		}
		pEventObject->EventType = EVENTTYPE_001_1;
		SetParticle(pEventObject->pos, 150, PARTICLETYPE_COMBUSTION,nIdx);
		SetMotion(&pEventObject->ObjectInfo.ParentObject.motion, 
			pEventObject->ObjectInfo.ParentObject.pModelData, 
			&pEventObject->ObjectInfo.ParentObject.OffSetData, 
			MOTIONTYPE_MOVE, false, false, 10);
		isSuccess = true;
		break;
	}

	pEventObject->isEvent = isSuccess;

	return isSuccess;
}

//========================================================================
// 各魔法イベント処理[BRIDGEのイベント]
//========================================================================
bool SetMagicEvent002(MAGICTYPE type, int nIdx)
{
	EventObject* pEventObject = GetEventObject(nIdx);
	bool isSuccess = false;

	switch (type)
	{
	case MAGICTYPE_TIMEREVERT:
		if (nIdx == 0)	// 1P
		{
			PlaySound(SOUND_LABEL_TIMEREVERT0);
		}
		else if (nIdx == 1)	// 2P
		{
			PlaySound(SOUND_LABEL_TIMEREVERT1);
		}
		pEventObject->EventType = EVENTTYPE_002_0;
		SetParticle(pEventObject->pos, 150, PARTICLETYPE_TIMEREVERT, nIdx);
		SetMotion(&pEventObject->ObjectInfo.ParentObject.motion, 
			pEventObject->ObjectInfo.ParentObject.pModelData, 
			&pEventObject->ObjectInfo.ParentObject.OffSetData, 
			MOTIONTYPE_MOVE, false, false, 10);
		isSuccess = true;
		break;

	case MAGICTYPE_COMBUSTION:
	//	isSuccess = true;
		break;

	case MAGICTYPE_LEVITATION:
	//	isSuccess = true;
		break;
	}

	pEventObject->isEvent = isSuccess;

	return isSuccess;
}

//========================================================================
// 各魔法イベント処理[PLANTのイベント][燃焼/成長]
//========================================================================
bool SetMagicEvent003(MAGICTYPE type, int nIdx)
{
	EventObject* pEventObject = GetEventObject(nIdx);
	bool isSuccess = false;

	switch (type)
	{
#if 0
	case MAGICTYPE_TIMEREVERT:	// 成長前に戻る
		pParentObject->EventType = EVENTTYPE_003_0;
		SetParticle(pParentObject->pos, 150, PARTICLETYPE_TIMEREVERT);
		SetMotion(&pParentObject->motion, pParentObject->pModelData, &pParentObject->OffSetData, MOTIONTYPE_MOVE, false, true, 10);
		SetMagicLocus(MAGICEVENT_004, pParentObject->pos, 500.0f, pParentObject->nEventIdx);
		isSuccess = true;
		break;
#endif

	case MAGICTYPE_GROWTH:	// 成長させる
		if (nIdx == 0)	// 1P
		{
			PlaySound(SOUND_LABEL_GROWTH0);
		}
		else if (nIdx == 1)	// 2P
		{
			PlaySound(SOUND_LABEL_GROWTH1);
		}
		pEventObject->EventType = EVENTTYPE_003_1;
		SetParticle(pEventObject->pos, 150, PARTICLETYPE_GROWTH, nIdx);
		SetMotion(&pEventObject->ObjectInfo.ParentObject.motion, 
			pEventObject->ObjectInfo.ParentObject.pModelData, 
			&pEventObject->ObjectInfo.ParentObject.OffSetData, 
			MOTIONTYPE_MOVE, false, true, 10);
		SetMagicLocus(MAGICEVENT_004, pEventObject->pos, 500.0f, nIdx);
		isSuccess = true;
		break;

	case MAGICTYPE_RAINPRAY:	// 成長させる
		pEventObject->EventType = EVENTTYPE_003_1;
		SetParticle(pEventObject->pos, 150, PARTICLETYPE_RAINPRAY, nIdx);
		SetMotion(&pEventObject->ObjectInfo.ParentObject.motion, 
			pEventObject->ObjectInfo.ParentObject.pModelData, 
			&pEventObject->ObjectInfo.ParentObject.OffSetData, 
			MOTIONTYPE_MOVE, false, true, 10);
		SetMagicLocus(MAGICEVENT_004, pEventObject->pos, 500.0f, nIdx);
		isSuccess =  true;
		break;

	case MAGICTYPE_COMBUSTION:	// 燃焼させる
		if (nIdx == 0)	// 1P
		{
			PlaySound(SOUND_LABEL_COMBUSTION0);
		}
		else if (nIdx == 1)	// 2P
		{
			PlaySound(SOUND_LABEL_COMBUSTION1);
		}
		pEventObject->EventType = EVENTTYPE_003_2;
		SetParticle(pEventObject->pos, 150, PARTICLETYPE_COMBUSTION, nIdx);
		SetMotion(&pEventObject->ObjectInfo.ParentObject.motion, 
			pEventObject->ObjectInfo.ParentObject.pModelData, 
			&pEventObject->ObjectInfo.ParentObject.OffSetData, 
			MOTIONTYPE_RUN, false, false, 10);
		isSuccess = true;
		break;

	case MAGICTYPE_FIREBALL:	// 燃焼させる
		if (nIdx == 0)	// 1P
		{
			PlaySound(SOUND_LABEL_COMBUSTION0);
		}
		else if (nIdx == 1)	// 2P
		{
			PlaySound(SOUND_LABEL_COMBUSTION1);
		}
		pEventObject->EventType = EVENTTYPE_003_2;
		SetParticle(pEventObject->pos, 150, PARTICLETYPE_COMBUSTION, nIdx);
		SetMotion(&pEventObject->ObjectInfo.ParentObject.motion, 
			pEventObject->ObjectInfo.ParentObject.pModelData, 
			&pEventObject->ObjectInfo.ParentObject.OffSetData, 
			MOTIONTYPE_RUN, false, false, 10);
		isSuccess = true;
		break;
	}

	pEventObject->isEvent = isSuccess;

	return isSuccess;
}

//========================================================================
// 各魔法イベント処理[PLANTのイベント][時戻し]
//========================================================================
bool SetMagicEvent004(MAGICTYPE type, int nIdx)
{
	EventObject* pEventObject = GetEventObject(nIdx);
	bool isSuccess = false;

	switch (type)
	{
	case MAGICTYPE_TIMEREVERT:	// 成長前に戻る
		if (nIdx == 0)	// 1P
		{
			PlaySound(SOUND_LABEL_TIMEREVERT0);
		}
		else if (nIdx == 1)	// 2P
		{
			PlaySound(SOUND_LABEL_TIMEREVERT1);
		}
		pEventObject->EventType = EVENTTYPE_004_0;
		SetParticle(pEventObject->pos, 150, PARTICLETYPE_TIMEREVERT, nIdx);
		SetMotion(&pEventObject->ObjectInfo.ParentObject.motion, 
			pEventObject->ObjectInfo.ParentObject.pModelData, 
			&pEventObject->ObjectInfo.ParentObject.OffSetData, 
			MOTIONTYPE_ACTION, false, false, 10);
		SetMagicLocus(MAGICEVENT_003, pEventObject->pos, 500.0f, nIdx);
		isSuccess = true;
		break;
	}

	pEventObject->isEvent = isSuccess;

	return isSuccess;
}

//========================================================================
// 各魔法イベント処理[池の氷結]
//========================================================================
bool SetMagicEvent005(MAGICTYPE type, int nIdx)
{
	EventObject* pEventObject = GetEventObject(nIdx);
	bool isSuccess = false;

	switch (type)
	{
	case MAGICTYPE_FREEZE:	// 氷結魔法
		if (nIdx == 0)	// 1P
		{
			PlaySound(SOUND_LABEL_FREEZE0);
		}
		else if (nIdx == 1)	// 2P
		{
			PlaySound(SOUND_LABEL_FREEZE1);
		}
		pEventObject->EventType = EVENTTYPE_005_0;
		SetParticle(pEventObject->pos, 150, PARTICLETYPE_FREEZE, nIdx);
		SetMotion(&pEventObject->ObjectInfo.ParentObject.motion,
			pEventObject->ObjectInfo.ParentObject.pModelData,
			&pEventObject->ObjectInfo.ParentObject.OffSetData,
			MOTIONTYPE_MOVE, false, false, 10);
		SetMagicLocus(MAGICEVENT_006, pEventObject->pos, 500.0f, nIdx);
		isSuccess = true;
		break;
	}

	pEventObject->isEvent = isSuccess;

	return isSuccess;
}

//========================================================================
// 各魔法イベント処理[池の融解]
//========================================================================
bool SetMagicEvent006(MAGICTYPE type, int nIdx)
{
	EventObject* pEventObject = GetEventObject(nIdx);
	bool isSuccess = false;

	switch (type)
	{
	case MAGICTYPE_TIMEREVERT:	// 時戻し魔法
		if (nIdx == 0)	// 1P
		{
			PlaySound(SOUND_LABEL_TIMEREVERT0);
		}
		else if (nIdx == 1)	// 2P
		{
			PlaySound(SOUND_LABEL_TIMEREVERT1);
		}
		pEventObject->EventType = EVENTTYPE_006_0;
		SetParticle(pEventObject->pos, 150, PARTICLETYPE_TIMEREVERT, nIdx);
		SetMotion(&pEventObject->ObjectInfo.ParentObject.motion,
			pEventObject->ObjectInfo.ParentObject.pModelData,
			&pEventObject->ObjectInfo.ParentObject.OffSetData,
			MOTIONTYPE_RUN, false, false, 10);
		SetMagicLocus(MAGICEVENT_005, pEventObject->pos, 500.0f, nIdx);
		isSuccess = true;
		break;

	case MAGICTYPE_COMBUSTION:	// 溶かす
		if (nIdx == 0)	// 1P
		{
			PlaySound(SOUND_LABEL_COMBUSTION0);
		}
		else if (nIdx == 1)	// 2P
		{
			PlaySound(SOUND_LABEL_COMBUSTION1);
		}
		pEventObject->EventType = EVENTTYPE_006_1;
		SetParticle(pEventObject->pos, 150, PARTICLETYPE_COMBUSTION, nIdx);
		SetMotion(&pEventObject->ObjectInfo.ParentObject.motion,
			pEventObject->ObjectInfo.ParentObject.pModelData,
			&pEventObject->ObjectInfo.ParentObject.OffSetData,
			MOTIONTYPE_RUN, false, false, 10);
		SetMagicLocus(MAGICEVENT_005, pEventObject->pos, 500.0f, nIdx);
		isSuccess = true;
		break;

	case MAGICTYPE_FIREBALL:	// 溶かす
		if (nIdx == 0)	// 1P
		{
			PlaySound(SOUND_LABEL_COMBUSTION0);
		}
		else if (nIdx == 1)	// 2P
		{
			PlaySound(SOUND_LABEL_COMBUSTION1);
		}
		pEventObject->EventType = EVENTTYPE_006_1;
		SetParticle(pEventObject->pos, 150, PARTICLETYPE_COMBUSTION, nIdx);
		SetMotion(&pEventObject->ObjectInfo.ParentObject.motion,
			pEventObject->ObjectInfo.ParentObject.pModelData,
			&pEventObject->ObjectInfo.ParentObject.OffSetData,
			MOTIONTYPE_RUN, false, false, 10);
		SetMagicLocus(MAGICEVENT_005, pEventObject->pos, 500.0f, nIdx);
		isSuccess = true;
		break;
	}

	pEventObject->isEvent = isSuccess;

	return isSuccess;
}

//========================================================================
// 各魔法イベント処理[植物の開花]
//========================================================================
bool SetMagicEvent007(MAGICTYPE type, int nIdx)
{
	EventObject* pEventObject = GetEventObject(nIdx);
	bool isSuccess = false;
	switch (type)
	{
	case MAGICTYPE_RAINPRAY:	// 雨乞い
		pEventObject->EventType = EVENTTYPE_007_0;
		SetParticle(pEventObject->pos, 150, PARTICLETYPE_RAINPRAY, nIdx);
		SetMotion(&pEventObject->ObjectInfo.ParentObject.motion,
			pEventObject->ObjectInfo.ParentObject.pModelData,
			&pEventObject->ObjectInfo.ParentObject.OffSetData,
			MOTIONTYPE_MOVE, false, false, 10);
		SetMagicLocus(MAGICEVENT_008, pEventObject->pos, 500.0f, nIdx);
		isSuccess = true;
		ClearQuest(QUESTTYPE_FLOWER);
		break;

	case MAGICTYPE_GROWTH:	// 成長
		if (nIdx == 0)	// 1P
		{
			PlaySound(SOUND_LABEL_GROWTH0);
		}
		else if (nIdx == 1)	// 2P
		{
			PlaySound(SOUND_LABEL_GROWTH1);
		}
		pEventObject->EventType = EVENTTYPE_007_1;
		SetParticle(pEventObject->pos, 150, PARTICLETYPE_GROWTH, nIdx);
		SetMotion(&pEventObject->ObjectInfo.ParentObject.motion,
			pEventObject->ObjectInfo.ParentObject.pModelData,
			&pEventObject->ObjectInfo.ParentObject.OffSetData,
			MOTIONTYPE_MOVE, false, false, 10);
		SetMagicLocus(MAGICEVENT_008, pEventObject->pos, 500.0f, nIdx);
		isSuccess = true;
		ClearQuest(QUESTTYPE_FLOWER);
		break;
	}

	pEventObject->isEvent = isSuccess;

	return isSuccess;
}

//========================================================================
// 各魔法イベント処理[植物の萎む]
//========================================================================
bool SetMagicEvent008(MAGICTYPE type, int nIdx)
{
	EventObject* pEventObject = GetEventObject(nIdx);
	bool isSuccess = false;
	switch (type)
	{
	case MAGICTYPE_TIMEREVERT:	// 巻き戻し
		if (nIdx == 0)	// 1P
		{
			PlaySound(SOUND_LABEL_TIMEREVERT0);
		}
		else if (nIdx == 1)	// 2P
		{
			PlaySound(SOUND_LABEL_TIMEREVERT1);
		}
		pEventObject->EventType = EVENTTYPE_007_0;
		SetParticle(pEventObject->pos, 150, PARTICLETYPE_TIMEREVERT, nIdx);
		SetMotion(&pEventObject->ObjectInfo.ParentObject.motion,
			pEventObject->ObjectInfo.ParentObject.pModelData,
			&pEventObject->ObjectInfo.ParentObject.OffSetData,
			MOTIONTYPE_RUN, false, false, 10);
		SetMagicLocus(MAGICEVENT_007, pEventObject->pos, 500.0f, nIdx);
		isSuccess = true;
		break;
	}

	pEventObject->isEvent = isSuccess;

	return isSuccess;
}

//========================================================================
// イベントの設置
//========================================================================
void SetEvent(EVENTTYPE type, int nIdx)
{
	ParentObject* pParentObject = GetParentObjectInfo(nIdx);

	pParentObject->EventType = type;
}
