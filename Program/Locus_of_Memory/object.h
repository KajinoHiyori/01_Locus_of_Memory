//========================================================
// 
// object処理[object.h]
// Author : Kajino Hiyori
// 
//========================================================
#ifndef _OBJECT_H_
#define	_OBJECT_H_
#include "main.h"

// マクロ定義
#define MAX_OBJECTMAT	(16)	// マテリアルの最大数
#define MAX_OBJECT		(1024)	// オブジェクトの最大数
#define MAX_PARTS		(14)	// パーツの最大数
#define MAX_PARENTMODEL	(8)		// 階層構造をもったモデルの最大数

// モデルの種類
typedef struct
{
	LPD3DXMESH			pMesh;						// メッシュ(頂点情報)へのポインタ
	LPD3DXBUFFER		pBuffMat;					// マテリアルへのポインタ
	DWORD				dwNumMat;					// マテリアルの数
	LPDIRECT3DTEXTURE9	apTexture[MAX_OBJECTMAT];	// テクスチャへの	ポインタ
	D3DXVECTOR3			vtxMin;						// ブロックの大きさ(最小)
	D3DXVECTOR3			vtxMax;						// ブロックの大きさ(最大)
}ObjectModel;

// モデルの数
typedef enum
{
	OBJECTTYPE_HOUSE000 = 0,	// 家[0]
	OBJECTTYPE_HOUSE001,		// 家[1]
	OBJECTTYPE_HOUSE002,		// 家[2]
	OBJECTTYPE_HOUSE003,		// 家[3]
	OBJECTTYPE_SHOP000,			// 店[4]
	OBJECTTYPE_SHOP001,			// 店[5]
	OBJECTTYPE_SHOP002,			// 店[6]
	OBJECTTYPE_SHOP003,			// 
	OBJECTTYPE_SHOP004,			// 
	OBJECTTYPE_SHOP005,			// 
	OBJECTTYPE_CASTLE000,		// 城[9]
	OBJECTTYPE_MAX
}OBJECTTYPE;

//*****************************************************************************
// 階層構造モデルデータの種類
//*****************************************************************************
typedef enum PARENTMODELTYPE
{
	PARENTMODELTYPE_PLAYER1P = 0,	// 1P
	PARENTMODELTYPE_PLAYER2P,		// 2P
	PARENTMODELTYPE_MAX
}PARENTMODELTYPE;

// モデルの構造体
typedef struct
{
	D3DXMATRIX		mtxWorld;	// ワールドマトリックス
	D3DXVECTOR3		pos;		// オブジェクトの位置
	D3DXVECTOR3		rot;		// オブジェクトの向き
	OBJECTTYPE		type;		// モデルの種類
	int				nIdxShadow;	// 影のインデックス
	float			fSize;		// 大きさ
	bool			bUse;		// 使用状態
}Object;

//*****************************************************************************
// モデルの構造体定義
//*****************************************************************************
typedef struct Model
{
	LPDIRECT3DTEXTURE9	apTexture[MAX_OBJECTMAT];	// テクスチャ
	LPD3DXMESH			pMesh;						// メッシュへのポインタ
	LPD3DXBUFFER		pBuffMat;					// マテリアルへのポインタ
	DWORD				dwNumMat;					// マテリアルの数
	int					nIdxModel;					// モデルのインデックス
	int					nIdxModelParent;			// 親モデルのインデックス
	D3DXVECTOR3			pos;						// モデルの位置 (オフセット)
	D3DXVECTOR3			posLocal;					// モデルの位置 (ローカル)
	D3DXVECTOR3			rot;						// 向き
	D3DXVECTOR3			rotLocal;					// 向き (ローカル)
	D3DXMATRIX			mtxWorld;					// ワールドマトリックス
}Model;

//*****************************************************************************
// 階層構造モデルデータの構造体定義
//*****************************************************************************
typedef struct ModelData
{
	Model				aModel[MAX_PARTS];					// モデル (パーツ)
	D3DXVECTOR3			aOffSet[MAX_PARTS];					// モデルのオフセット[位置]を保存
	D3DXVECTOR3			aOffSetRot[MAX_PARTS];				// モデルのオフセット[角度]を保存
	int					nNumModel;							// モデル (パーツ) の総数
}ModelData;

// プロトタイプ宣言
void InitObject(void);
void UninitObject(void);
void UpdateObject(void);
void DrawObject(void);
void SetObject(OBJECTTYPE type, D3DXVECTOR3 pos, D3DXVECTOR3 rot, bool isShadow, bool isCollision);
void LoadObjectModel(const char* pModelPath);
ModelData* SetModelData(PARENTMODELTYPE type);
void LoadParentModel(const char* pModelPath, int nNumParentModel);
void LoadParentModelOffSet(D3DXVECTOR3 pos, D3DXVECTOR3 rot, int nIdxModel, int nIdxModelParent, int nNumParentModel);
Object* GetObjectInfo(void);
ObjectModel* GetObjectModel(void);
#endif
