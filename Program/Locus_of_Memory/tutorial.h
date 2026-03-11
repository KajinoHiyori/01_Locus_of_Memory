//=============================================================================
//
//	Locus of Memory [tutorial.h]
//	Author : HAYATO NAMBA
// 
//=============================================================================
#ifndef _TUTORIAL_H_        // このマクロ定義がされなかったら
#define _TUTORIAL_H_        // 2重インクルード防止のマクロを定義する


// プロトタイプ宣言
void InitTutorial(void);
void UninitTutorial(void);
void UpdateTutorial(void);
void DrawTutorial(void);
void GoGameGate(void);
bool GetGate(int nIdx);

#endif