//=============================================================================
//
//	Locus of Memory [tutorial.h]
//	Author : HAYATO NAMBA
// 
//=============================================================================
#ifndef _TUTORIAL_H_        // このマクロ定義がされなかったら
#define _TUTORIAL_H_        // 2重インクルード防止のマクロを定義する

// マクロ定義
#define GATE_POS		(D3DXVECTOR3(-1000.0f, 120.0f, -3650.0f))	// ゲートの位置

// プロトタイプ宣言
void InitTutorial(void);
void UninitTutorial(void);
void UpdateTutorial(void);
void DrawTutorial(void);
void DrawTutorialUIs(void);
void GoGameGate(void);
bool GetGate(int nIdx);

#endif