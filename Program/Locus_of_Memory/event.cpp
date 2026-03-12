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
#define BLACKSMITH_POS	(D3DXVECTOR3(-3200.0f, 0.0f, 950.0f))	// 鍛冶場の位置
#define FIRE_PARTICLE	(D3DXVECTOR3(BLACKSMITH_POS.x - 100.0f, BLACKSMITH_POS.y + 20.0f, BLACKSMITH_POS.z))

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
	SetMagicEvent009,
	SetMagicEvent010,
	SetMagicEvent011,
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
		pEventObject->EventType = EVENTTYPE_002_0;
		SetParticle(pEventObject->pos, 150, PARTICLETYPE_TIMEREVERT, nIdx);
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
// 各魔法イベント処理[PLANTのイベント][燃焼/成長]
//========================================================================
bool SetMagicEvent003(MAGICTYPE type, int nIdx)
{
	EventObject* pEventObject = GetEventObject(nIdx);
	bool isSuccess = false;

	switch (type)
	{
	case MAGICTYPE_GROWTH:	// 成長させる
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
		pEventObject->EventType = EVENTTYPE_003_2;
		SetParticle(pEventObject->pos, 150, PARTICLETYPE_COMBUSTION, nIdx);
		SetMotion(&pEventObject->ObjectInfo.ParentObject.motion, 
			pEventObject->ObjectInfo.ParentObject.pModelData, 
			&pEventObject->ObjectInfo.ParentObject.OffSetData, 
			MOTIONTYPE_RUN, false, false, 10);
		isSuccess = true;
		break;

	case MAGICTYPE_FIREBALL:	// 燃焼させる
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
		pEventObject->EventType = EVENTTYPE_008_0;
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
// 各魔法イベント処理[ドラゴンが驚く]
//========================================================================
bool SetMagicEvent009(MAGICTYPE type, int nIdx)
{
	EventObject* pEventObject = GetEventObject(nIdx);
	bool isSuccess = false;
	switch (type)
	{
	case MAGICTYPE_FLASH:	// フラッシュ
		pEventObject->EventType = EVENTTYPE_009_0;
		SetParticle(pEventObject->pos, 150, PARTICLETYPE_FLASH, nIdx);
		SetMotion(&pEventObject->ObjectInfo.ParentObject.motion,
			pEventObject->ObjectInfo.ParentObject.pModelData,
			&pEventObject->ObjectInfo.ParentObject.OffSetData,
			MOTIONTYPE_MOVE, true, false, 10);
		isSuccess = true;
		ClearQuest(QUESTTYPE_DRAGON);
		break;
	}

	pEventObject->isEvent = isSuccess;

	return isSuccess;
}

//========================================================================
// 各魔法イベント処理[街灯置き換え処理]
//========================================================================
bool SetMagicEvent010(MAGICTYPE type, int nIdx)
{
	EventObject* pEventObject = GetEventObject(nIdx);
	ObjectModel* pObjectModel = GetObjectModel(OBJECTTYPE_STREETLIGHT001);	// 
	bool isSuccess = false;
	switch (type)
	{
	case MAGICTYPE_FLASH:	// フラッシュ
		pEventObject->EventType = EVENTTYPE_010_0;
		SetParticle(pEventObject->pos, 150, PARTICLETYPE_FLASH, nIdx);
		pEventObject->ObjectInfo.NormalObject.type = OBJECTTYPE_STREETLIGHT001;
		pEventObject->ObjectInfo.NormalObject.pModelData = pObjectModel;
		isSuccess = true;
		ClearQuest(QUESTTYPE_STREETLIGHT);
		break;
	}

	pEventObject->isEvent = isSuccess;

	return isSuccess;
}

//========================================================================
// 各魔法イベント処理[鍛冶場に火をつける]
//========================================================================
bool SetMagicEvent011(MAGICTYPE type, int nIdx)
{
	// EventObject* pEventObject = GetEventObject(nIdx);
	bool isSuccess = false;
	switch (type)
	{
	case MAGICTYPE_COMBUSTION:	// 燃焼
		//pEventObject->EventType = EVENTTYPE_008_0;
		SetParticle(FIRE_PARTICLE, 150, PARTICLETYPE_COMBUSTION, nIdx);
		isSuccess = true;
		break;

	case MAGICTYPE_FIREBALL:	// 燃焼
	//pEventObject->EventType = EVENTTYPE_008_0;
		SetParticle(FIRE_PARTICLE, 150, PARTICLETYPE_COMBUSTION, nIdx);
		isSuccess = true;
		break;
	}
	ClearQuest(QUESTTYPE_BLACKSMITH);
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
