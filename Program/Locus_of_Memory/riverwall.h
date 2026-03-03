//=============================================================================
//
//	川沿いの壁の処理 [riverwall.h]
//	Author : SHUMA AIZU
// 
//=============================================================================

#ifndef _RIVERWALL_H_
#define _RIVERWALL_H_

#include "main.h"
#include "custommesh.h"

//*****************************************************************************
// マクロ定義
//*****************************************************************************
#define MAX_VTX						(1000)				// 頂点数の最大
#define MAX_IDX						(512)				// インデックスの最大

//*****************************************************************************
// プロトタイプ宣言
//*****************************************************************************
void InitRiverWall(void);
void UninitRiverWall(void);
void UpdateRiverWall(void);
void DrawRiverWall(void);
void SetRiverWall(D3DXVECTOR3 pos, D3DXVECTOR3 rot, D3DXVECTOR3* pVtxPos, int nNumVtx, int* pIdxInfo, int nNumIdx);
void LoadRiverWall(const char* pFileName, D3DXVECTOR3 pos, D3DXVECTOR3 rot);

#endif // _RIVERWALL_H_