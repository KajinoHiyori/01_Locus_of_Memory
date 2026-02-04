//========================================================
// 
// メニューの背景処理[menubg.h]
// Author : KajinoHiyori
// 
//========================================================
#ifndef _MENUBG_H_
#define	_MENUBG_H_

// マクロ定義
#define HEIGHT_POSUP	(254.0f)	// スマホの上端
#define HEIGHT_POSDOWN	(710.0f)	// スマホの下端

// 背景の種類
typedef enum
{
	MENUBG_TEX_CLOCK = 0,	// 時計の背景
	MENUBG_TEX_MAGICBOOK,	// 魔導書の背景
	MENUBG_TEX_MAX
}MENUBG_TEX;

// 背景の状態
typedef enum
{
	MENUBG_STATE_NONDISPLAY = 0,	// 非表示
	MENUBG_STATE_APPEAR,			// 出現
	MENUBG_STATE_DISPLAY,			// 表示
	MENUBG_STATE_DISAPPERA,			// 収縮
	MENUBG_STATE_MAX
}MENUBG_STATE;

// プロトタイプ宣言
void InitMenuBG(void);
void UninitMenuBG(void);
void UpdateMenuBG(void);
void DrawMenuBG(void);
void SetMenuBG(int nIdx, float fPosY, MENUBG_TEX tex);
void DisappearMenuBG(int nIdx);
#endif