//=============================================================================
//
//	スクリプト読み込み処理 [loadsclipt.h]
//	Author : SHUMA AIZU
// 
//=============================================================================

#ifndef _LOADSCRIPT_H_
#define _LOADSCRIPT_H_

#include "main.h"

//*****************************************************************************
// マクロ定義
//*****************************************************************************
#define TITLE_SCRIPT		"data/SCRIPTS/title.txt"	// タイトル管理スクリプト
#define GAME_SCRIPT			"data/SCRIPTS/game.txt"		// ゲーム管理スクリプト
#define RESULT_SCRIPT		"data/SCRIPTS/result.txt"	// リザルト管理スクリプト
#define STAGE_SCRIPT		"data/SCRIPTS/stage.txt"	// ステージ情報スクリプト
#define OBJECT_SCRIPT		"data/SCRIPTS/object.txt"	// オブジェクト情報スクリプト

#define MODEL_SCRIPT			"data/SCRIPTS/model.txt"		// モデル情報スクリプト
#define PARENTMODEL_1PSCRIPT	"data/SCRIPTS/1P_model.txt"		// モデル情報スクリプト
#define PARENTMODEL_2PSCRIPT	"data/SCRIPTS/2P_model.txt"		// モデル情報スクリプト
#define MOTION_PLAYERSCRIPT		"data/SCRIPTS/player_motion.txt"	// モーション情報スクリプト

//*****************************************************************************
// プロトタイプ宣言
//*****************************************************************************
HRESULT LoadScript(const char* pScriptFileName);
HRESULT LoadParentModel(const char* pParentModelFileName);
HRESULT LoadMotionInfo(const char* pMotionFileName);
HRESULT LoadModel(const char* pModelFileName);
HRESULT LoadObject(const char* pObjectFileName);
void LoadEnableString(char* aStrCpy, char* pStart);

#endif	_LOADSCRIPT_H_