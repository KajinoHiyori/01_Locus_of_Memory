//========================================================
// 
// object処理[object.h]
// Author : Kajino Hiyori
// 
//========================================================
#ifndef _OBJECT_H_
#define	_OBJECT_H_
#include "main.h"

#include "motion.h"
#include "event.h"

// マクロ定義
#define MAX_OBJECTMAT		(32)	// マテリアルの最大数
#define MAX_OBJECT			(1024)	// オブジェクトの最大数
#define MAX_PARENTOBJECT	(128)	// 階層構造オブジェクトの最大数
#define MAX_PARTS			(32)	// パーツの最大数
#define MAX_PARENTMODEL		(16)	// 階層構造をもったモデルの最大数

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
	OBJECTTYPE_BIGTREE = 0,		// [0]巨木
	OBJECTTYPE_HOUSE000,		// [1]家小
	OBJECTTYPE_SHOP000,			// [2]店小
	OBJECTTYPE_SHOP001,			// [3]店中
	OBJECTTYPE_TEMPLE,			// [4]神殿
	OBJECTTYPE_HOUSE002,		// [5]家中
	OBJECTTYPE_HOUSE003,		// [6]家大
	OBJECTTYPE_SHOP002,			// [7]店大
	OBJECTTYPE_HOUSE004,		// [8]家4
	OBJECTTYPE_LECTERN,			// [9]書見台
	OBJECTTYPE_CASTLE,			// [10]城
	OBJECTTYPE_TREES,			// [11]木(複数)
	OBJECTTYPE_TREE,			// [12]木
	OBJECTTYPE_STREETLIGHT000,	// [13]街灯
	OBJECTTYPE_GATE,			// [14]門
	OBJECTTYPE_DRAGON,			// [15]ドラゴン
	OBJECTTYPE_BENCH,			// [16]ベンチ
	OBJECTTYPE_BIRD,			// [17]鳥
	OBJECTTYPE_DEN,				// [18]鳥の巣
	OBJECTTYPE_TORCH,			// [19]松明
	OBJECTTYPE_ROCK000,			// [20]岩
	OBJECTTYPE_ROCK001,			// [21]岩
	OBJECTTYPE_RABBIT,			// [22]兎
	OBJECTTYPE_KEY000,			// [23]鍵1
	OBJECTTYPE_KEY001,			// [24]鍵2
	OBJECTTYPE_KEY002,			// [25]鍵3
	OBJECTTYPE_CHAIN,			// [26]鎖
	OBJECTTYPE_PADLOCK,			// [27]南京錠
	OBJECTTYPE_RIVER,			// [28] 川
	OBJECTTYPE_STREETLIGHT001,	// [29]街灯2
	OBJECTTYPE_BONFIRE000,		// [30]焚火1
	OBJECTTYPE_BONFIRE001,		// [31]焚火2
	OBJECTTYPE_NEST,			// [32]鳥の巣2
	OBJECTTYPE_BUCKET000,		// [33]バケツ1
	OBJECTTYPE_BUCKET001,		// [34]バケツ2
	OBJECTTYPE_CLOCK000,		// [35]時計1
	OBJECTTYPE_CLOCK001,		// [36]時計2
	OBJECTTYPE_WATERINGCAN000,	// [37]じょうろ1
	OBJECTTYPE_WATERINGCAN001,	// [38]じょうろ2
	OBJECTTYPE_FLOWERBED000, 	// [39]花壇1
	OBJECTTYPE_FLOWERBED001, 	// [40]花壇2
	OBJECTTYPE_SHRINE,			// [41]祠
	OBJECTTYPE_SHOVEL,			// [42]シャベル
	OBJECTTYPE_WINDMILL,		// [43]風車
	OBJECTTYPE_LIGHTHOUSE,		// [44]灯台
	OBJECTTYPE_CHIMNEY,			// [45]煙突
	OBJECTTYPE_TUTORIALWALL000,	// [46]チュートリアル分断壁
	OBJECTTYPE_TUTORIALWALL001,	// [47]チュートリアル分断壁
	OBJECTTYPE_FANCE,			// [48]川を囲うフェンス
	OBJECTTYPE_SLED000,			// [49]そり
	OBJECTTYPE_MAX
}OBJECTTYPE;

// 階層構造モデルデータの種類
typedef enum PARENTMODELTYPE
{
	PARENTMODELTYPE_PLAYER1P = 0,	// 1P
	PARENTMODELTYPE_PLAYER2P,		// 2P
	PARENTMODELTYPE_HOUSE,			// 家
	PARENTMODELTYPE_DRAGON,			// ドラゴン
	PARENTMODELTYPE_BOOK,			// 本
	PARENTMODELTYPE_WATERMILL,		// 水車小屋
	PARENTMODELTYPE_BRIDGE,			// 橋
	PARENTMODELTYPE_PLANT,			// 植物[成長]
	PARENTMODELTYPE_CLOCKTOWER,		// 時計塔
	PARENTMODELTYPE_MAX
}PARENTMODELTYPE;

// オブジェクトの構造体
typedef struct
{
	D3DXMATRIX		mtxWorld;		// ワールドマトリックス
	D3DXVECTOR3		pos;			// オブジェクトの位置
	D3DXVECTOR3		rot;			// オブジェクトの向き
	OBJECTTYPE		type;			// モデルの種類
	float			fAlpha;			// アルファ値
	int				nIdxShadow;		// 影のインデックス
	float			fSize;			// 大きさ
	EVENTTYPE		EventType;		// イベントの種類
	int				nCollisionIdx;	// 当たり判定のインデックス
	bool			bUse;			// 使用状態
}Object;

// モーション中のデータの構造体定義
typedef struct OffSetData
{
	D3DXVECTOR3 pos[MAX_PARTS];						// 位置
	D3DXVECTOR3 rot[MAX_PARTS];						// 向き
}OffSetData;

// 階層構造オブジェクトの構造体定義
typedef struct
{
	D3DXMATRIX		mtxWorld;		// ワールドマトリックス
	D3DXVECTOR3		pos;			// オブジェクトの位置
	D3DXVECTOR3		rot;			// オブジェクトの向き
	PARENTMODELTYPE	type;			// 階層構造オブジェクトの種類
	float			fAlpha;			// アルファ値
	Motion			motion;			// モーション情報
	ModelData*		pModelData;		// モデルの情報
	OffSetData		OffSetData;		// モーション中の階層構造情報
	EVENTTYPE		EventType;		// イベントの種類
	int				nEventIdx;		// イベントのインデックス
	int				nCollisionIdx;	// 当たり判定のインデックス
	bool			bUse;			// 使用状態
}ParentObject;

// モデルの構造体定義
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

// 階層構造モデルデータの構造体定義
typedef struct ModelData
{
	Model				aModel[MAX_PARTS];			// モデル (パーツ)
	int					nNumModel;					// モデルの総数
	int					nNumParts;					// パーツの総数
}ModelData;

// プロトタイプ宣言
void InitObject(void);
void UninitObject(void);
void UpdateObject(void);
void DrawObject(void);
bool CollisionObject(D3DXVECTOR3* pPos, D3DXVECTOR3* pPosOld, D3DXVECTOR3* pMove, float fRadius);
void SetObject(OBJECTTYPE type, D3DXVECTOR3 pos, D3DXVECTOR3 rot, bool isShadow, bool isCollision, bool isRandObj = false);
void SetParentObject(D3DXVECTOR3 pos, D3DXVECTOR3 rot, PARENTMODELTYPE parentmodeltype, bool isCollision);
//void SetObject(OBJECTTYPE type, D3DXVECTOR3 pos, D3DXVECTOR3 rot, bool isShadow, bool isCollision, ColliderInfo* pColliderInfo, int nNumCollider, bool isRandObj = false);
//void SetParentObject(D3DXVECTOR3 pos, D3DXVECTOR3 rot, PARENTMODELTYPE parentmodeltype, ColliderInfo* pColliderInfo, int nNumCollider, bool isCollision);
void LoadObjectModel(const char* pModelPath);
ModelData* SetModelData(PARENTMODELTYPE type);
void LoadParentModel(const char* pModelPath, int nNumParentModel);
void LoadParentModelOffSet(D3DXVECTOR3 pos, D3DXVECTOR3 rot, int nIdxModel, int nIdxModelParent, int nNumParentModel, int nCntParts);
Object* GetObjectInfo(int nIdx);
ParentObject* GetParentObjectInfo(int nIdx);
ObjectModel* GetObjectModel(void);
void UninitRandomObject(void);
void UpdateObjectEvent001(ParentObject* pParentObject);
#endif
