//=============================================================================
//
//	イベント関連処理 [event.cpp]
//	Author : Aizu Shuma
// 
//=============================================================================
#include "event.h"
#include "object.h"
#include "eventobject.h"
#include "particle.h"
#include "magiceffect.h"
#include "effectfire.h"
#include "motion.h"
#include "magic.h"
#include "debugproc.h"
#include "sound.h"
#include "questui.h"

//=============================================================================
// マクロ定義
//=============================================================================
#define BLACKSMITH_POS_FIRE		(D3DXVECTOR3(-3200.0f, 40.0f, 950.0f))		// 鍛冶場の位置
#define BLACKSMITH_POS_SMOKE	(D3DXVECTOR3(-3200.0f, 500.0f, 980.0f))		// 鍛冶場の煙を設置
#define DRAGONFLASH_POS			(D3DXVECTOR3(2220.0f, 1000.0f, -4410.0f))	// 鍛冶場の煙を設置
#define FIRE_PARTICLE			(D3DXVECTOR3(BLACKSMITH_POS.x - 100.0f, BLACKSMITH_POS.y + 20.0f, BLACKSMITH_POS.z))	// 鍛冶場の炎の設置

//=============================================================================
// グローバル変数
//=============================================================================
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
	SetMagicEvent012,
	SetMagicEvent013,
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
// 各魔法イベント処理[家の燃焼]
//========================================================================
bool SetMagicEvent001(MAGICTYPE type, int nIdx)
{
	EventObject* pEventObject = GetEventObject(nIdx);
	bool isSuccess = false;

	switch (type)
	{
		// 燃焼イベント
	case MAGICTYPE_COMBUSTION:	// 燃焼
		pEventObject->EventType = EVENTTYPE_001_1;
		SetParticle(pEventObject->pos, 150, PARTICLETYPE_COMBUSTION,nIdx);
		SetMotion(&pEventObject->ObjectInfo.ParentObject.motion, 
			pEventObject->ObjectInfo.ParentObject.pModelData, 
			&pEventObject->ObjectInfo.ParentObject.OffSetData, 
			MOTIONTYPE_MOVE, false, false, 10);
		isSuccess = true;
		break;

	case MAGICTYPE_FIREBALL:	// 火球
		pEventObject->EventType = EVENTTYPE_001_1;
		SetParticle(pEventObject->pos, 150, PARTICLETYPE_COMBUSTION, nIdx);
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
// 各魔法イベント処理[橋の修復]
//========================================================================
bool SetMagicEvent002(MAGICTYPE type, int nIdx)
{
	EventObject* pEventObject = GetEventObject(nIdx);
	bool isSuccess = false;

	switch (type)
	{
	case MAGICTYPE_TIMEREVERT:	// 巻き戻し
		pEventObject->EventType = EVENTTYPE_002_0;
		SetParticle(pEventObject->pos, 150, PARTICLETYPE_TIMEREVERT, nIdx);
		SetMotion(&pEventObject->ObjectInfo.ParentObject.motion, 
			pEventObject->ObjectInfo.ParentObject.pModelData, 
			&pEventObject->ObjectInfo.ParentObject.OffSetData, 
			MOTIONTYPE_MOVE, false, false, 10);
		ResetCollision(pEventObject->nCollisionIdx);
		isSuccess = true;
		break;
	}
	pEventObject->isEvent = isSuccess;

	return isSuccess;
}

//========================================================================
// 各魔法イベント処理[蔦のイベント][燃焼/成長]
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
// 各魔法イベント処理[PLANTのイベント][巻き戻し]
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
		SetParticle(DRAGONFLASH_POS, 150, PARTICLETYPE_FLASH, nIdx);
		SetMotion(&pEventObject->ObjectInfo.ParentObject.motion,
			pEventObject->ObjectInfo.ParentObject.pModelData,
			&pEventObject->ObjectInfo.ParentObject.OffSetData,
			MOTIONTYPE_MOVE, false, false, 10);
		isSuccess = true;
		SetMagicLocus(MAGICEVENT_009, pEventObject->pos, 500.0f, nIdx); 
		ClearQuest(QUESTTYPE_DRAGON);
		PlaySound(SOUND_LABEL_DRAGON);
		break;

	case MAGICTYPE_RAINPRAY:	// 雨乞い
		pEventObject->EventType = EVENTTYPE_009_1;
		SetParticle(DRAGONFLASH_POS, 150, PARTICLETYPE_RAINPRAY, nIdx);
		SetMotion(&pEventObject->ObjectInfo.ParentObject.motion,
			pEventObject->ObjectInfo.ParentObject.pModelData,
			&pEventObject->ObjectInfo.ParentObject.OffSetData,
			MOTIONTYPE_MOVE, false, false, 10);
		SetMagicLocus(MAGICEVENT_009, pEventObject->pos, 500.0f, nIdx);
		isSuccess = true;
		ClearQuest(QUESTTYPE_DRAGON);
		PlaySound(SOUND_LABEL_DRAGON);
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
		SetParticle(D3DXVECTOR3(pEventObject->pos.x, pEventObject->pos.y + 200.0f, pEventObject->pos.z), 100, PARTICLETYPE_FLASH, nIdx);
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
	bool isSuccess = false;
	switch (type)
	{
	case MAGICTYPE_COMBUSTION:	// 燃焼
		SetEffectFire(BLACKSMITH_POS_FIRE, FIRE_TYPE_FORGE, nIdx);
		SetEffectFire(BLACKSMITH_POS_SMOKE, FIRE_TYPE_SMOKE, nIdx);
		ClearQuest(QUESTTYPE_BLACKSMITH);
		isSuccess = true;
		break;

	case MAGICTYPE_FIREBALL:	// 燃焼
		SetEffectFire(BLACKSMITH_POS_FIRE, FIRE_TYPE_FORGE, nIdx);
		SetEffectFire(BLACKSMITH_POS_SMOKE, FIRE_TYPE_SMOKE, nIdx);
		isSuccess = true;
		ClearQuest(QUESTTYPE_BLACKSMITH);
		break;
	}
	return isSuccess;
}

//========================================================================
// 各魔法イベント処理[焚火に火をつける]
//========================================================================
bool SetMagicEvent012(MAGICTYPE type, int nIdx)
{
	EventObject* pEventObject = GetEventObject(nIdx);
	bool isSuccess = false;
	switch (type)
	{
	case MAGICTYPE_COMBUSTION:	// 燃焼
		//pEventObject->EventType = EVENTTYPE_008_0;
		SetEffectFire(pEventObject->pos, FIRE_TYPE_BONFIRE, nIdx);
		isSuccess = true;
		break;

	case MAGICTYPE_FIREBALL:	// 燃焼
		//pEventObject->EventType = EVENTTYPE_008_0;
		SetEffectFire(pEventObject->pos, FIRE_TYPE_BONFIRE, nIdx);
		isSuccess = true;
		break;
	}
	return isSuccess;
}

//========================================================================
// 各魔法イベント処理[篝火に火をつける]
//========================================================================
bool SetMagicEvent013(MAGICTYPE type, int nIdx)
{
	EventObject* pEventObject = GetEventObject(nIdx);
	bool isSuccess = false;
	switch (type)
	{
	case MAGICTYPE_COMBUSTION:	// 燃焼
		pEventObject->EventType = EVENTTYPE_013_0;
		SetEffectFire(D3DXVECTOR3(pEventObject->pos.x, pEventObject->pos.y + 150.0f, pEventObject->pos.z), FIRE_TYPE_BONFIRE, nIdx);
		isSuccess = true;
		break;

	case MAGICTYPE_FIREBALL:	// 燃焼
		pEventObject->EventType = EVENTTYPE_013_0;
		SetEffectFire(D3DXVECTOR3(pEventObject->pos.x, pEventObject->pos.y + 150.0f, pEventObject->pos.z), FIRE_TYPE_BONFIRE, nIdx);
		isSuccess = true;
		break;
	}
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
