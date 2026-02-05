

#ifndef _VIBRATION_H        // このマクロ定義がされなかったら
#define _VIBRATION_H        // 2重インクルード防止のマクロを定義する

#define VIBRATION_CLEAR	(-1)	// 振動を止める値

//コマンドの種類
typedef enum
{
	VIBRATIONTYPE_NONE = -1,	//何も入力されていないとき
	VIBRATIONTYPE_CLOSE,		//近距離
	VIBRATIONTYPE_MEDIUM,		//中距離
	VIBRATIONTYPE_FAR,			//遠距離
	VIBRATIONTYPE_NOTHING,		//範囲外
	VIBRATIONTYPE_MAX
}VIBRATIONTYPE;

//コマンドの種類
typedef struct
{
	bool Vibration;

}VIBRATION;

// プロトタイプ宣言
void InitVibration(void);
void UninitVibration(void);
void UpdateVibration(void);
void VibrationType(VIBRATIONTYPE Type,int nDropMagic,int nPlayertype);
VIBRATION* GetVibration(void);

#endif