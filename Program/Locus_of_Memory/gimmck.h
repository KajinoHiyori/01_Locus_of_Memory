//========================================================
// 
// ギミックが近いときのUI表示処理[gimmick.h]
// Author : KajinoHiyori
// 
//========================================================
#ifndef _GIMMICK_H_
#define	_GIMMICK_H_
#include "object.h"
#include "eventobject.h"

typedef enum
{
	GIMMICKTYPE_NONE = -1,
	GIMMICKTYPE_NORMAL,
	GIMMICKTYPE_PARENT,
	GIMMICKTYPE_MAX
}GIMMICKTYPE;

// プロトタイプ宣言
void InitMagicBubble(void);
void UninitMagicBubble(void);
void UpdateMagicBubble(void);
void DrawMagicBubble(void);
void SetMagicBubble(int nIdx, EVENTOBJECTTYPE eType, int nDistance);	// eType = 魔法の種類を取得してテクスチャを変更 / nDistance = 距離によってfar[1]/near[0]/不使用[-1]を切り替える
void ResetMagicBubble(int nIdx);

#endif