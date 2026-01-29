//========================================================
// 
// 魔法発動状態のUI表示処理[spellui.h]
// Author : KajinoHiyori
// 
//========================================================
#ifndef _SPELLUI_H_
#define	_SPELLUI_H_


// UIのテクスチャの種類
typedef enum
{
	SPELLUI_TEX_MAGICNULL = 0,	// 空の魔法
	SPELLUI_TEX_RED,			// 赤魔法
	SPELLUI_TEX_GREEN,			// 緑魔法
	SPELLUI_TEX_BLUE,			// 青魔法
	SPELLUI_TEX_YELLOW,			// 黄魔法
	SPELLUI_TEX_1,				// キーボード操作[1]
	SPELLUI_TEX_2,				// キーボード操作[2]
	SPELLUI_TEX_3,				// キーボード操作[3]
	SPELLUI_TEX_4,				// キーボード操作[4]
	SPELLUI_TEX_A,				// ジョイパッド操作[A]
	SPELLUI_TEX_B,				// ジョイパッド操作[B]
	SPELLUI_TEX_X,				// ジョイパッド操作[X]
	SPELLUI_TEX_Y,				// ジョイパッド操作[Y]
	SPELLUI_TEX_NONE,			// 魔法詠唱失敗
	SPELLUI_TEX_LEVITATION,		//浮遊
	SPELLUI_TEX_COMBUSTION,		//燃焼	
	SPELLUI_TEX_FLOOD,			//洪水、氾濫	
	SPELLUI_TEX_FLASH,			//フラッシュ	
	SPELLUI_TEX_FIREBALL,		//火球	
	SPELLUI_TEX_SUNSETDELAY,	//太陽の動きを遅延する	
	SPELLUI_TEX_RAINPRAY,		//雨乞い	
	SPELLUI_TEX_FREEZE,			//凍結	
	SPELLUI_TEX_GROWTH,			//成長(植物など)	
	SPELLUI_TEX_ACCELERATION,	//加速	
	SPELLUI_TEX_TIMEREVERT,		//時間の巻き戻し(回帰)
	SPELLUI_TEX_SPELL,			// SPELLメニュー
	SPELLUI_TEX_PHONE,			// スマホ
	SPELLUI_TEX_MAX
}SPELLUI_TEX;

// UIの種類
typedef enum
{
	SPELLUI_TYPE_PHONE = 0,		// スマホ
	SPELLUI_TYPE_COMMAND0,		// コマンド1つ目
	SPELLUI_TYPE_COMMAND1,		// コマンド2つ目
	SPELLUI_TYPE_COMMAND2,		// コマンド3つ目
	SPELLUI_TYPE_MAGIC,			// 発動された魔法
	SPELLUI_TYPE_RED,			// 赤魔法
	SPELLUI_TYPE_GREEN,			// 緑魔法
	SPELLUI_TYPE_BLUE,			// 青魔法
	SPELLUI_TYPE_YELLOW,		// 黄魔法
	SPELLUI_TYPE_OP_R,			// 赤魔法発動ボタン
	SPELLUI_TYPE_OP_G,			// 緑魔法発動ボタン
	SPELLUI_TYPE_OP_B,			// 青魔法発動ボタン
	SPELLUI_TYPE_OP_Y,			// 黄魔法発動ボタン
	SPELLUI_TYPE_SPELL,			// SPELLメニュー 
	SPELLUI_TYPE_MAX
}SPELLUI_TYPE;

// UIの状態
typedef enum
{
	SPELL_UI_STATE_NONE,	// 何もしていない
	SPELL_UI_STATE_1PSPELL,	// 1PのSPELL
	SPELL_UI_STATE_2PSPELL,	// 2PのSPELL
}SPELLUI_STATE;

// プロトタイプ宣言
void InitSpellUI(void);
void UninitSpellUI(void);
void UpdateSpellUI(void);
void DrawSpellUI(void);
void SetSpellUI(int pauseMenu);

#endif
