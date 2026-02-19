//=============================================================================
//
//	画面上の粒の処理 [sparkle.h]
//	Author : SHUMA AIZU
// 
//=============================================================================

#ifndef _SPARKLE_H_
#define _SPARKLE_H_

//*****************************************************************************
// 粒の構造体定義
//*****************************************************************************
typedef struct Sparkle
{
	D3DXVECTOR3 pos;			// 位置
	D3DXVECTOR3 move;			// 移動量
	D3DXCOLOR	col;			// 色
	D3DXMATRIX	mtxWorld;		// ワールドマトリックス
	float		fRadius;		// 半径
	float		fAngle;			// 角度
	float		fSpeed;			// 速度
	int			nLife;			// 寿命（色）
	bool		bUse;			// 使用しているか
}Sparkle;

//*****************************************************************************
// プロトタイプ宣言
//*****************************************************************************
void InitSparkle(void);
void UninitSparkle(void);
void UpdateSparkle(void);
void DrawSparkle(void);
void SetSparkle(void);
void ReleaseSparkle(int nIdx);

#endif	// _GRAIN_H_