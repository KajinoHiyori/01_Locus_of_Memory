//=============================================================================
//
//	スクリプト読み込み処理 [loadsclipt.h]
//	Author : SHUMA AIZU
// 
//=============================================================================

#ifndef _LOADSCRIPT_H_
#define _LOADSCRIPT_H_

#include "main.h"
#include "collision.h"

//*****************************************************************************
// マクロ定義
//*****************************************************************************
#define INFORMATION_SCRIPT		"data/SCRIPTS/INFORMATION/information.txt"		// スクリプト情報スクリプト
#define MODEL_SCRIPT			"data/SCRIPTS/OBJECT/model.txt"					// モデル情報スクリプト
#define MAGICOBJECTINFO_SCRIPT	"data/SCRIPTS/INFORMATION/Magicobjectinfo.txt"	// 魔法オブジェクト情報スクリプト

//*****************************************************************************
// プロトタイプ宣言
//*****************************************************************************
HRESULT LoadScript(const char* pScriptFileName);
HRESULT LoadParentModel(const char* pParentModelFileName);
HRESULT LoadMotionInfo(const char* pMotionFileName);
HRESULT LoadModel(const char* pModelFileName);
HRESULT LoadMagicObject(const char* pMagicObjectFileName);
HRESULT LoadMagicObject(const char* pMagicObjectFileName);
ColliderInfo LoadCollider(FILE* pFile);
void LoadEnableString(char* aStrCpy, char* pStart);

#endif	_LOADSCRIPT_H_