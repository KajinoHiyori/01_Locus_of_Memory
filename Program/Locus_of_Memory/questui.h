//========================================================
// 
// クエストのUI表示処理[questui.h]
// Author : KajinoHiyori
// 
//========================================================
#ifndef _QUESTUI_H_
#define	_QUESTUI_H_

// クエストの種類
typedef enum
{
	QUESTTYPE_TEMPLE = 0,	// 神殿
	QUESTTYPE_SHIRINE,		// 祠
	QUESTTYPE_DRAGON,		// ドラゴン
	QUESTTYPE_FLOWER,		// 花
	QUESTTYPE_BLACKSMITH,	// 鍛冶場
	QUESTTYPE_STREETLIGHT,	// 街灯
	QUESTTYPE_MAX
}QUESTTYPE;

// プロトタイプ宣言
void InitQuestUI(void);
void UninitQuestUI(void);
void UpdateQuestUI(void);
void DrawQuestUI(void);
void ClearQuest(QUESTTYPE questType);
void UpdateQuestPos(void);
void ExtendLine(int nIdx, QUESTTYPE questType);
void MoveSide(int nIdx, QUESTTYPE questType);
void MoveUp(int nIdx, QUESTTYPE questType);
void AddAlpha(int nIdx, QUESTTYPE questType);
void UpdateQuest(int nIdx, QUESTTYPE questType);
void UpdateLine(int nIdx, QUESTTYPE questType);

#endif