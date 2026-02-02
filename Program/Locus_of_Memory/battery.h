//========================================================
// 
// バッテリー処理[buttery.h]
// Author : KajinoHiyori
// 
//========================================================
#ifndef _BATTERY_H_
#define _BATTERY_H_

// プロトタイプ宣言
void InitBattery(void);
void UninitBattery(void);
void UpdateBattery(void);
void DrawBattery(void);
void SetBattery(int nIdx, int nBattery, D3DXVECTOR3 pos);
void ChangeBattery(int nIdx, int nValue);

#endif