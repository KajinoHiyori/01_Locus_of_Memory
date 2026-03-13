//=============================================================================
//
//	リザルトUI処理 [resultui.cpp]
//	Author : Kajino Hiyori
// 
//=============================================================================
#ifndef _RESULTUI_H_
#define	_RESULTUI_H_

#include "title.h"

// リザルトUIの配置
typedef enum
{
	RESULTUITYPE_DIAGNOSIS = 0,	// 診断結果
	RESULTUITYPE_MOSTCOMMAND,	// 1番使ったコマンド
	RESULTUITYPE_COMMMANDTYPE,	// コマンドの種類
	RESULTUITYPE_CLEARTIME,		// 神殿到達時刻
	RESULTUITYPE_YOUARE,		// あなたは
	RESULTUITYPE_COMMANDRESULT,	// コマンド使用数に応じたリザルト
	RESULTUITYPE_CLEARRESULT,	// クリア時間に応じたリザルト
	RESULTUITYPE_EVENTRESULT,	// イベント発生回数に応じたリザルト
	RESULTUITYPE_PLAYER1,		// PLAYER01
	RESULTUITYPE_PLAYER2,		// PLAYER02
	RESULTUITYPE_MAX
}RESULTUITYPE;

// リザルトUIのテクスチャの種類
typedef enum
{
	RESULTUITEX_DIAGNOSIS = 0,	// 診断結果
	RESULTUITEX_MOSTCOMMAND,	// 1番使ったコマンド
	RESULTUITEX_R,				// 赤魔法
	RESULTUITEX_G,				// 赤魔法
	RESULTUITEX_B,				// 赤魔法
	RESULTUITEX_Y,				// 赤魔法
}RESULTUITEX;

// プロトタイプ宣言
void InitResultUI(void);
void UninitResultUI(void);
void UpdateResultUI(void);
void DrawResultUI(void);
void SetResultUI(RESULTUITYPE type, OPERATIONTYPE operationType);

void SetResultUIAppear(int nIdx);
void SetResultUIDisp(int nIdx);
#endif
