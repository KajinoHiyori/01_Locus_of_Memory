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
	RESULTUITYPE_DIAGNOSIS = 0,		// 診断結果
	RESULTUITYPE_MOSTCOMMANDSOLO,	// 1番使ったコマンド
	RESULTUITYPE_MOSTCOMMAND1P,		// 1番使ったコマンド
	RESULTUITYPE_MOSTCOMMAND2P,		// 1番使ったコマンド
	RESULTUITYPE_1P,				// 1P
	RESULTUITYPE_2P,				// 2P
	RESULTUITYPE_COMMMANDTYPESOLO,	// コマンドの種類[solo]
	RESULTUITYPE_COMMMANDTYPE1P,	// コマンドの種類[1P]
	RESULTUITYPE_COMMMANDTYPE2P,	// コマンドの種類[2P]
	RESULTUITYPE_CLEARTIME,			// 神殿到達時刻
	RESULTUITYPE_HOUR0,				// 神殿到達時刻[X0:00]
	RESULTUITYPE_HOUR1,				// 神殿到達時刻[0X:00]
	RESULTUITYPE_COLON,				// 神殿到達時刻[:]
	RESULTUITYPE_MIN0,				// 神殿到達時刻[00:X0]
	RESULTUITYPE_MIN1,				// 神殿到達時刻[00:0X]
	RESULTUITYPE_YOUARE,			// あなたは[solo]
	RESULTUITYPE_COMMANDRESULT,		// コマンド使用数に応じたリザルト[solo]
	RESULTUITYPE_CLEARRESULT,		// クリア時間に応じたリザルト[solo]
	RESULTUITYPE_EVENTRESULT,		// イベント発生回数に応じたリザルト[solo]
	RESULTUITYPE_THEYARE,			// あなたたちは[pair]
	RESULTUITYPE_COMPATIBILITY,		// コマンド相性[pair]
	RESULTUITYPE_PAIRCLEAR,			// ペアリザルト[pair]
	RESULTUITYPE_MAX
}RESULTUITYPE;

// リザルトUIのテクスチャの種類
typedef enum
{
	RESULTUITEX_DIAGNOSIS = 0,	// 診断結果
	RESULTUITEX_MOSTCOMMAND,	// 1番使ったコマンド
	RESULTUITEX_R,				// 赤魔法
	RESULTUITEX_G,				// 緑魔法
	RESULTUITEX_B,				// 青魔法
	RESULTUITEX_Y,				// 黄魔法
	RESULTUITEX_TEMPLE,			// 神殿到達時刻
	RESULTUITEX_CLEARTIME,		// 数字
	RESULTUITEX_YOUARE,			// あなたは
	RESULTUITEX_COMMANDR,		// コマンド使用数に応じたリザルト[R]
	RESULTUITEX_COMMANDG,		// コマンド使用数に応じたリザルト[G]
	RESULTUITEX_COMMANDB,		// コマンド使用数に応じたリザルト[B]
	RESULTUITEX_COMMANDY,		// コマンド使用数に応じたリザルト[Y]
	RESULTUITEX_CLEAREARLY,		// クリア時間に応じたリザルト[早い]
	RESULTUITEX_CLEARNORMAL,	// クリア時間に応じたリザルト[普通]
	RESULTUITEX_CLEARSLOWLY,	// クリア時間に応じたリザルト[遅い]
	RESULTUITEX_EVENTMANY,		// イベント発生回数に応じたリザルト[多い]
	RESULTUITEX_EVENTNORMAL,	// イベント発生回数に応じたリザルト[普通]
	RESULTUITEX_EVENTLESS,		// イベント発生回数に応じたリザルト[少ない]
	RESULTUITEX_THEYARE,		// あなたたちは
	RESULTUITEX_COMMANDRR,		// コマンド相性[RR]
	RESULTUITEX_COMMANDRG,		// コマンド相性[RG]
	RESULTUITEX_COMMANDRB,		// コマンド相性[RB]
	RESULTUITEX_COMMANDRY,		// コマンド相性[RY]
	RESULTUITEX_COMMANDBB,		// コマンド相性[BB]
	RESULTUITEX_COMMANDBG,		// コマンド相性[BG]
	RESULTUITEX_COMMANDBY,		// コマンド相性[BY]
	RESULTUITEX_COMMANDGG,		// コマンド相性[GG]
	RESULTUITEX_COMMANDGY,		// コマンド相性[GY]
	RESULTUITEX_COMMANDYY,		// コマンド相性[YY]
	RESULTUITEX_PAIREARLY,		// ペアリザルト[早い]
	RESULTUITEX_PAIRSLOWLY,		// ペアリザルト[遅い]
	RESULTUITEX_COLON,			// :
	RESULTUITEX_1P,				// 1P
	RESULTUITEX_2P,				// 2P
	RESULTUITEX_MAX
}RESULTUITEX;

// プロトタイプ宣言
void InitResultUI(void);
void UninitResultUI(void);
void UpdateResultUI(void);
void DrawResultUI(void);
void SetResultUI1P(void);
void SetResultUI2P(void);
void SetResultUI(RESULTUITYPE type, RESULTUITEX tex, D3DXVECTOR3 pos);
#endif
