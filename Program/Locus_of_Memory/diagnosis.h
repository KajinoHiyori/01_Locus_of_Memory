#if 0


#endif // 0
//========================================================
// 
// 診断画面のメイン処理[diagnosis.h]
// Author : KAJINO HIYORI
// 
//========================================================
#ifndef _DIAGNOSIS_H_
#define	_DIAGNOSIS_H_

// 操作方法の管理
typedef enum
{ 
	DIAGNOSISTYPE_1P = 0,	// 1PPLAY
	DIAGNOSISTYPE_2P,		// 2PPLAY
	DIAGNOSISTYPE_KEYBOARD,	// KEYBOARDPLAY
	DIAGNOSISTYPE_MAX
}DIAGNOSISTYPE;

// プロトタイプ宣言
void InitDiagnosis(void);
void UninitDiagnosis(void);
void UpdateDiagnosis(void);
void DrawDiagnosis(void);
void SetDiagnosisType(DIAGNOSISTYPE type);	// 操作方法の決定
DIAGNOSISTYPE GetDiagnosisType(void);		// 操作方法の取得

#endif
