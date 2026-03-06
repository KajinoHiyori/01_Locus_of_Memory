//========================================================
// 
// 魔法の位置に合わせたアイコン表示処理[questionmark.h]
// Author : KajinoHiyori
// 
//========================================================
#ifndef _QUESTIONMARK_H_
#define	_QUESTIONMARK_H_

#include "magic.h"

// プロトタイプ宣言
void InitQuestionMark(void);
void UninitQuestionMark(void);
void UpdateQuestionMark(void);
void DrawQuestionMark(void);
float DistanceMagicAndMark(int nIdx);
void UpdateMarkPos(int nIdx);
void SetMarkColor(int nIdx, COMMANDOREDER commandType, float fDistance);
#endif