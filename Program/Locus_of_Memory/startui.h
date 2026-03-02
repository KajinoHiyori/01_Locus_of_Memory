//========================================================
// 
// ロゴ処理[startui.h]
// Author : Kajino Hiyori
// 
//========================================================
#ifndef _STARTUI_H_
#define	_STARTUI_H_

// マクロ定義
#define LOGO_MAT		(16)	// ロゴテクスチャの最大数
#define LOGOMODEL_MAX	(1)		// ロゴモデルのパーツ数
#define LOGOKEY_MAX		(32)	// ロゴのキー最大数

// モデル構造体
typedef struct
{
	LPDIRECT3DTEXTURE9	apTexture[LOGO_MAT];	// テクスチャ
	LPD3DXMESH			pMesh;
	LPD3DXBUFFER		pBuffMat;
	DWORD				dwNumMat;
	int					nIdxModelParent;	// 親モデルのインデックス
	D3DXVECTOR3			pos;				// モデルの位置(オフセット)
	D3DXVECTOR3			rot;				// 向き
	D3DXMATRIX			mtxWorld;			// ワールドマトリックス
}LogoModel;

// キー要素の構造体
typedef struct
{
	float fPosX;	// 位置X...親からのオフセット
	float fPosY;	// 位置Y...親からのオフセット
	float fPosZ;	// 位置Z...親からのオフセット
	float fRotX;	// 向きX
	float fRotY;	// 向きY
	float fRotZ;	// 向きZ
}LOGOKEY;

// キー情報の構造体
typedef struct
{
	int nFrame;				// 再生フレーム(何フレームかけてモーションが遷移していくのか)
	LOGOKEY aKey[LOGOMODEL_MAX];	// 各パーツごとのキー要素
}LOGOKEY_INFO;

// モーション除法の構造体
typedef struct
{
	bool bLoop;					// ループするかどうか[0...ループなし / 1...ループあり]
	int nNumKey;				// キーの総数
	LOGOKEY_INFO aKeyInfo[LOGOKEY_MAX];	// キー情報
}LOGOMOTION_INFO;

// ロゴのモーションの種類
typedef enum
{
	LOGOMOTIONTYPE_ACTION = 0,	// アクション
	LOGOMOTIONTYPE_NEUTRAL,	// 待機
	LOGOMOTIONTYPE_MAX
}LOGOMOTIONTYPE;

// ロゴ構造体
typedef struct
{
	D3DXMATRIX	mtxWorld;					// ワールドマトリックス
	D3DXVECTOR3	pos;						// ゴールの位置
	D3DXVECTOR3	posOld;						// ゴールの1フレーム前の位置
	D3DXVECTOR3	move;						// ゴールの移動量
	D3DXVECTOR3	rot;						// ゴールの向き
	D3DXVECTOR3	rotDest;					// ゴールの目的の向き
	LogoModel	aModel[LOGOMODEL_MAX];		// モデル(パーツ)
	D3DXVECTOR3 aOffSet[LOGOMODEL_MAX];		// モデルのオフセット[位置]を保存
	D3DXVECTOR3 aOffSetRot[LOGOMODEL_MAX];	// モデルのオフセット[角度]を保存
	int		nNumModel;						// モデル(パーツ)の総数
	bool	bDisp;							// 表示状態
	LOGOMOTION_INFO	aMotionInfo[LOGOMOTIONTYPE_MAX];	// 現在のモーション情報
	int				nNumMotion;							// 現在のモーションの総数
	LOGOMOTIONTYPE	motionType;							// 現在のモーションの種類
	bool			bLoopMotion;						// 現在のモーションのループを管理
	int				nNumKey;							// 現在のキーの総数
	int				nKey;								// 現在実行されているキーの番号
	int				nCounterMotion;						// 現在のモーションのカウンター
	bool			bFinishMotion;						// 現在のモーションが終了しているか
}Logo;

// プロトタイプ宣言
void InitStartUI(void);
void UninitStartUI(void);
void UpdateStartUI(void);
void DrawStartUI(void);
void InitLogoMotion(void);
void UpdateLogoMotion(void);
void SetLogoMotion(LOGOMOTIONTYPE type);
void SetLogo(D3DXVECTOR3 pos);

#endif
