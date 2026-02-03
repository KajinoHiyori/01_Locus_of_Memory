//========================================================
// 
// 魔導書のUI表示処理[magicui.h]
// Author : KajinoHiyori
// 
//========================================================
#ifndef _MAGICUI_H_
#define	_MAGICUI_H_

// UIのテクスチャの種類
typedef enum
{
	MAGICUI_TEX_MAGICNULL = 0,	// 空の魔法
	MAGICUI_TEX_RED,			// 赤魔法
	MAGICUI_TEX_GREEN,			// 緑魔法
	MAGICUI_TEX_BLUE,			// 青魔法
	MAGICUI_TEX_YELLOW,			// 黄魔法
	MAGICUI_TEX_NONE,			// 魔法詠唱失敗
	MAGICUI_TEX_LEVITATION,		//浮遊
	MAGICUI_TEX_COMBUSTION,		//燃焼	
	MAGICUI_TEX_FLOOD,			//洪水、氾濫	
	MAGICUI_TEX_FLASH,			//フラッシュ	
	MAGICUI_TEX_FIREBALL,		//火球	
	MAGICUI_TEX_SUNSETDELAY,	//太陽の動きを遅延する	
	MAGICUI_TEX_RAINPRAY,		//雨乞い	
	MAGICUI_TEX_FREEZE,			//凍結	
	MAGICUI_TEX_GROWTH,			//成長(植物など)	
	MAGICUI_TEX_ACCELERATION,	//加速	
	MAGICUI_TEX_TIMEREVERT,		//時間の巻き戻し(回帰)
	MAGICUI_TEX_MAGICBOOK,		// 魔導書
	MAGICUI_TEX_BG,				// 背景
	MAGICUI_TEX_ADD,			// +
	MAGICUI_TEX_EQUAL,			// =
	MAGICUI_TEX_MAX
}MAGICUI_TEX;

// UIの種類
typedef enum
{
	MAGICUI_TYPE_BG = 0,		// 背景
	MAGICUI_TYPE_MAGICBOOK,		// 魔導書 
	MAGICUI_TYPE_0COMMAND0,		// 1つ目のコマンド1
	MAGICUI_TYPE_0_01ADD,		// +
	MAGICUI_TYPE_0COMMAND1,		// 1つ目のコマンド2
	MAGICUI_TYPE_0_12ADD,		// +
	MAGICUI_TYPE_0COMMAND2,		// 1つ目のコマンド3
	MAGICUI_TYPE_0_EQUAL,		// =
	MAGICUI_TYPE_0MAGIC,		// 1つ目の発動された魔法
	MAGICUI_TYPE_1COMMAND0,		// 2つ目のコマンド1
	MAGICUI_TYPE_1_01ADD,		// +
	MAGICUI_TYPE_1COMMAND1,		// 2つ目のコマンド2
	MAGICUI_TYPE_1_12ADD,		// +
	MAGICUI_TYPE_1COMMAND2,		// 2つ目のコマンド3
	MAGICUI_TYPE_1_EQUAL,		// =
	MAGICUI_TYPE_1MAGIC,		// 2つ目の発動された魔法
	MAGICUI_TYPE_2COMMAND0,		// 3つ目のコマンド1
	MAGICUI_TYPE_2_01ADD,		// +
	MAGICUI_TYPE_2COMMAND1,		// 3つ目のコマンド2
	MAGICUI_TYPE_2_12ADD,		// +
	MAGICUI_TYPE_2COMMAND2,		// 3つ目のコマンド3
	MAGICUI_TYPE_2_EQUAL,		// =
	MAGICUI_TYPE_2MAGIC,		// 3つ目の発動された魔法
	MAGICUI_TYPE_3COMMAND0,		// 4つ目のコマンド1
	MAGICUI_TYPE_3_01ADD,		// +
	MAGICUI_TYPE_3COMMAND1,		// 4つ目のコマンド2
	MAGICUI_TYPE_3_12ADD,		// +
	MAGICUI_TYPE_3COMMAND2,		// 4つ目のコマンド3
	MAGICUI_TYPE_3_EQUAL,		// =
	MAGICUI_TYPE_3MAGIC,		// 4つ目の発動された魔法
	MAGICUI_TYPE_MAX
}MAGICUI_TYPE;

// UIの状態
typedef enum
{
	MAGICUISTATE_NONDISPLAY = 0,	// 非表示
	MAGICUISTATE_APPEAR,			// 出現
	MAGICUISTATE_DISPLAY,			// 表示
	MAGICUISTATE_DISAPPERA,			// 収縮
	MAGICUISTATE_MAX
}MAGICUISTATE;

// プロトタイプ宣言
void InitMagicUI(void);
void UninitMagicUI(void);
void UpdateMagicUI(void);
void DrawMagicUI(void);
void ResetMagicUI(int nIdx);
void SetMagicUI(int nIdx);
void DisappearMagicUI(int nIdx);
void SetPlayerMagic(int nIdx);
#endif
