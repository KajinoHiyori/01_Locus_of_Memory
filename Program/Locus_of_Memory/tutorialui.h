//=============================================================================
//
//	チュートリアルUI処理 [tutorialui.cpp]
//	Author : Kajino Hiyori
// 
//=============================================================================
#ifndef _TUTORIALUI_H_
#define	_TUTORIALUI_H_

// チュートリアルUIの種類
typedef enum
{
	TUTORIALUITYPE_PLAYPAD = 0,	// 操作方法[パッド]
	TUTORIALUITYPE_PLAYKEY,		// 操作方法[キーボード]
	TUTORIALUITYPE_MAGICPAD,	// 魔法の使い方[パッド]
	TUTORIALUITYPE_MAGICKEY,	// 魔法の使い方[キーボード]
	TUTORIALUITYPE_TIMELIMIT,	// 制限時間
	TUTORIALUITYPE_MAGICBOOK,	// 魔導書
	TUTORIALUITYPE_FIREHOUSE,	// 燃える家
	TUTORIALUITYPE_PLANTS,		// 植物
	TUTORIALUITYPE_BRIDGES,		// 橋
	TUTORIALUITYPE_MAGICCIRCLE,	// 魔法陣によるバリア
	TUTORIALUITYPE_CAMERA,		// カメラ
	TUTORIALUITYPE_MAX
}TUTORIALUITYPE;

// プロトタイプ宣言
void InitTutorialUI(void);
void UninitTutorialUI(void);
void UpdateTutorialUI(void);
void DrawTutorialUI(void);
void SetTutorialUI(TUTORIALUITYPE type, D3DXVECTOR3 pos, D3DXVECTOR3 rot);

void SetTutorialUIAppear(int nIdx);
void SetTutorialUIDisp(int nIdx);
void SetTutorialUIDisappear(int nIdx);
void SetTutorialUINonDisp(int nIdx);
#endif