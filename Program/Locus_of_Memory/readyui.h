//========================================================
// 
// 準備完了状態のUI表示処理[readyui.h]
// Author : KajinoHiyori
// 
//========================================================
#ifndef _READYUI_H_
#define	_READYUI_H_

// テクスチャの種類
typedef enum
{
	READYUITYPE_TUTORIALX = 0,	// チュートリアル遷移[X]
	READYUITYPE_TUTORIALENTER,	// チュートリアル遷移[ENTER]
	READYUITYPE_READY,			// チュートリアル遷移完了状態
	READYUITYPE_GAMEX,			// ゲーム遷移[X]
	READYUITYPE_GAMEENTER,		// ゲーム遷移[ENTER]	
	READYUITYPE_GOAL,			// ゲーム遷移完了状態
	READYUITYPE_WAITING,		// 他プレイヤーの待機状態
	READYUITYPE_MAX
}READYUITYPE;

// プロトタイプ宣言
void InitReadyUI(void);
void UninitReadyUI(void);
void UpdateReadyUI(void);
void DrawReadyUI(void);
void SetReadyUI(int nIdx, READYUITYPE type);
void ResetReadyUI(int nIdx);

#endif