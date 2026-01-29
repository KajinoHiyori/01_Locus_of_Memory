//===================================================================
//
// magic処理[magic.h]
// Author : 眞鍋快陸
// 
//===================================================================
#ifndef _MAGIC_H_
#define _MAGIC_H_

#include "main.h"

//魔法の種類
typedef enum
{
	MAGICTYPE_NONE = -1,	//何もない
	//浮遊
	MAGICTYPE_LEVITATION,	//緑緑緑	[0番]
	//燃焼
	MAGICTYPE_COMBUSTION,	//赤赤赤	[1番]
	//洪水、氾濫
	MAGICTYPE_FLOOD,		//青青青	[2番]
	//フラッシュ
	MAGICTYPE_FLASH,		//黄黄黄	[3番]
	//火球
	MAGICTYPE_FIREBALL,		//赤赤緑 | 赤緑赤 | 緑赤赤	[4番]
	//太陽の動きを遅延する
	MAGICTYPE_SUNSETDELAY,	//赤黄黄 | 黄赤黄 | 黄黄赤	[5番]
	//雨乞い
	MAGICTYPE_RAINPRAY,		//青青緑 | 青緑青 | 緑青青	[6番]
	//凍結
	MAGICTYPE_FREEZE,		//青緑緑 | 緑青緑 | 緑緑青	[7番]
	//成長(植物など)
	MAGICTYPE_GROWTH,		//青黄黄 | 黄青黄 | 黄黄青	[8番]
	//加速
	MAGICTYPE_ACCELERATION,	//緑緑黄 | 緑黄緑 | 黄緑緑	[9番]
	//時間の巻き戻し(回帰)
	MAGICTYPE_TIMEREVERT,	//赤緑青	[10番]

	MAGICTYPE_MAX
}MAGICTYPE;

//コマンドの並び順
typedef enum
{
	COMMANDOREDER_NONE = -1,
	//３つ同じ並び
	COMMANDOREDER_GGG,			//緑緑緑
	COMMANDOREDER_RRR,			//赤赤赤
	COMMANDOREDER_BBB,			//青青青
	COMMANDOREDER_YYY,			//黄黄黄
	//赤赤緑 | 赤緑赤 | 緑赤赤
	COMMANDOREDER_RRG,
	COMMANDOREDER_RGR,
	COMMANDOREDER_GRR,
	//赤黄黄 | 黄赤黄 | 黄黄赤
	COMMANDOREDER_RYY,
	COMMANDOREDER_YRY,
	COMMANDOREDER_YYR,
	//青青緑 | 青緑青 | 緑青青
	COMMANDOREDER_BBG,
	COMMANDOREDER_BGB,
	COMMANDOREDER_GBB,
	//青緑緑 | 緑青緑 | 緑緑青
	COMMANDOREDER_BGG,
	COMMANDOREDER_GBG,
	COMMANDOREDER_GGB,
	//青黄黄 | 黄青黄 | 黄黄青
	COMMANDOREDER_BYY,
	COMMANDOREDER_YBY,
	COMMANDOREDER_YYB,
	//緑緑黄 | 緑黄緑 | 黄緑緑
	COMMANDOREDER_GGY,
	COMMANDOREDER_GYG,
	COMMANDOREDER_YGG,
	//赤緑青
	COMMANDOREDER_RGB,

	COMMANDOREDER_MAX
}COMMANDOREDER;

//コマンドの種類
typedef enum
{
	COMMANDTYPE_NONE = -1,	//何も入力されていないとき
	COMMANDTYPE_R,			//赤のコマンド
	COMMANDTYPE_G,			//緑のコマンド
	COMMANDTYPE_B,			//青のコマンド
	COMMANDTYPE_Y,			//黄のコマンド
	COMMANDTYPE_MAX
}COMMANDTYPE;

typedef struct
{
	D3DXMATRIX mtxWorld;
	D3DXVECTOR3 pos;
	D3DXVECTOR3 move;
	D3DXVECTOR3 rot;
	MAGICTYPE mType;
	int nLife;
	int nIdxShadow;
	bool bUse;
}Magic;

//落ちてる魔法の構造体定義
typedef struct DropMagic
{
	D3DXVECTOR3 pos;		// 位置
	D3DXVECTOR3 rot;		// 向き
	COMMANDOREDER oType;	// 魔法の種類
	float fRadius;			// 半径
	bool bUse;				// 使用状態
}DropMagic;


//リザルトに必要なカウント類
typedef struct
{
	int nCommandCounter[COMMANDTYPE_MAX];		//コマンドの種類のカウント	
	int nMagicTypeCounter[MAGICTYPE_MAX];		//魔法の種類のカウント
}MagicCounter;

void InitMagic(void);
void UninitMagic(void);
void UpdateMagic(void);
void DrawMagic(void);
Magic* GetMagic(void);
MagicCounter* GetMagicCounter(int nIdx);

//入力されたコマンドを配列に格納(playerのMAGICBOOKから魔導書を持っているか確認してセット)
COMMANDOREDER PressCommand(int nIdx);

//マジックの変換
MAGICTYPE ChangeMagic(COMMANDOREDER commandorder);

//魔法の発動(コマンドの入力回数をカウント)
void SetMagic(MAGICTYPE type, D3DXVECTOR3 pos, D3DXVECTOR3 rot, D3DXVECTOR3 move, int nIdx);

//フィールド内に魔法を設置
void SetMagicPosition(COMMANDOREDER type, D3DXVECTOR3 pos, D3DXVECTOR3 rot);

// フィールド内の魔法との当たり判定
COMMANDOREDER CollisionMagic(D3DXVECTOR3 pos, float fRadius);

// フィールド内の魔法を取得する
MAGICTYPE GetFieldMagic(D3DXVECTOR3 pos, float fRadius);

#endif
