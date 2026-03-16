//========================================================
// 
// 魔法の位置に合わせたアイコン表示処理[questionmark.cpp]
// Author : KajinoHiyori
// 
//========================================================
#if 0
#include "main.h"
#include "questionmark.h"
#include "debugproc.h"
#include "player.h"
#include "game.h"
#include "input.h"
#include "title.h"
#include "fog.h"
#include "debugproc.h"
#include "color.h"
#include "magiceffect.h"

// マクロ定義
#define QUESTIONMARK_TYPE	(1)		// テクスチャの最大数
#define MARK_WIDTH		(15.0f)		// 吹き出しの幅
#define MARK_HEIGHT		(15.0f)		// 吹き出しの高さ
#define MARK_X			(30.0f)		// 吹き出しのX軸
#define MARK_Y			(95.0f)		// 吹き出しのY高度
#define DISTANCE		(30.0f)		// 処理を行うキー数
#define NONDISP			(500.0f)	// これ以上離れていたら表示しない
#define DISP			(100.0f)	// アルファ値1.0fで表示
#define MAGICEF_SIZE	(75.0f)		// マジックエフェクトの配置サイズ
#define NORMAL			(D3DXVECTOR3(0.0f, 1.0f, 0.0f))	// 法線ベクトル

// MAGICMARKの構造体
typedef struct
{
	D3DXMATRIX		mtxWorld;		// ワールドマトリックス
	D3DXVECTOR3		pos;			// 位置
	D3DXVECTOR3		rot;			// 位置
	D3DXVECTOR3		Offset;			// 位置
	D3DXCOLOR		col0;			// 色0
	D3DXCOLOR		col1;			// 色1
	D3DXCOLOR		col2;			// 色2
	D3DXCOLOR		col3;			// 色3
	float			fWidth;			// 幅
	float			fHeight;		// 高さ
	bool			bDisp;			// 表示状態
	int				nIdxMagic;		// 落ちている魔法のインデックス
	COMMANDOREDER	commandType;	// 落ちてる魔法の種類
}QuestionMark;

// グローバル変数
LPDIRECT3DTEXTURE9	g_apTextureQuestionMark[QUESTIONMARK_TYPE] = {};	// テクスチャへのポインタ
LPDIRECT3DVERTEXBUFFER9	g_pVtxBuffQuestionMark = NULL;			// 頂点バッファへのポインタ
QuestionMark g_aQuestionMark[MAX_PLAYER];		// MAGICMARKの全体管理

// テクスチャの読み込み
const char* c_apFilenameQuestionMark[QUESTIONMARK_TYPE] =
{
	"data\\TEXTURE\\questionmark.png",	// ?
};

//======================================================================================
// questionmarkの初期化処理
//======================================================================================
void InitQuestionMark(void)
{
	// デバイスの取得
	LPDIRECT3DDEVICE9 pDevice = GetDevice();
	// 操作人数の取得
	OPERATIONTYPE operationType = GetOperationType();
	// プレイヤーの情報を取得
	Player* pPlayer = GetPlayer();

	// テクスチャの読み込み
	for (int nCntUI = 0; nCntUI < QUESTIONMARK_TYPE; nCntUI++)
	{
		if (g_apTextureQuestionMark[nCntUI] == NULL)
		{
			D3DXCreateTextureFromFile(pDevice, c_apFilenameQuestionMark[nCntUI], &g_apTextureQuestionMark[nCntUI]);
		}
	}

	// 初期化
	for (int nCntPlayer = 0; nCntPlayer < MAX_PLAYER; nCntPlayer++)
	{
		g_aQuestionMark[nCntPlayer].pos = D3DXVECTOR3(0.0f, MARK_Y, 0.0f);	// 位置
		g_aQuestionMark[nCntPlayer].pos = INIT_D3DXVEC3;	// 位置
		g_aQuestionMark[nCntPlayer].rot = INIT_D3DXVEC3;	// 位置
		g_aQuestionMark[nCntPlayer].col0 = COLOR_WHITE;		// 種類
		g_aQuestionMark[nCntPlayer].col1 = COLOR_WHITE;		// 種類
		g_aQuestionMark[nCntPlayer].col2 = COLOR_WHITE;		// 種類
		g_aQuestionMark[nCntPlayer].col3 = COLOR_WHITE;		// 種類
		g_aQuestionMark[nCntPlayer].fWidth = MARK_WIDTH;	// 幅
		g_aQuestionMark[nCntPlayer].fHeight = MARK_HEIGHT;	// 高さ
		g_aQuestionMark[nCntPlayer].bDisp = false;			// 表示状態
		g_aQuestionMark[nCntPlayer].nIdxMagic = -1;			// 落ちている魔法のインデックス
		g_aQuestionMark[nCntPlayer].commandType = COMMANDOREDER_NONE;	// 落ちている魔法の種類
	}

	// 頂点バッファの生成
	pDevice->CreateVertexBuffer(sizeof(VERTEX_3D) * 4 * MAX_PLAYER, D3DUSAGE_WRITEONLY, FVF_VERTEX_3D, D3DPOOL_MANAGED, &g_pVtxBuffQuestionMark, NULL);

	VERTEX_3D* pVtx;
	// 頂点バッファをロックし、頂点情報へのポインタを取得
	g_pVtxBuffQuestionMark->Lock(0, 0, (void**)&pVtx, 0);

	for (int nCntPlayer = 0; nCntPlayer < MAX_PLAYER; nCntPlayer++, pVtx += 4)
	{
		// 頂点座標の設定
		pVtx[0].pos = D3DXVECTOR3(-g_aQuestionMark[nCntPlayer].fWidth, g_aQuestionMark[nCntPlayer].fHeight, 0.0f);
		pVtx[1].pos = D3DXVECTOR3(g_aQuestionMark[nCntPlayer].fWidth, g_aQuestionMark[nCntPlayer].fHeight, 0.0f);
		pVtx[2].pos = D3DXVECTOR3(-g_aQuestionMark[nCntPlayer].fWidth, -g_aQuestionMark[nCntPlayer].fHeight, 0.0f);
		pVtx[3].pos = D3DXVECTOR3(g_aQuestionMark[nCntPlayer].fWidth, -g_aQuestionMark[nCntPlayer].fHeight, 0.0f);

		// rhwの設定
		pVtx[0].nor = NORMAL;
		pVtx[1].nor = NORMAL;
		pVtx[2].nor = NORMAL;
		pVtx[3].nor = NORMAL;

		// 頂点カラーの設定
		pVtx[0].col = g_aQuestionMark[nCntPlayer].col0;
		pVtx[1].col = g_aQuestionMark[nCntPlayer].col1;
		pVtx[2].col = g_aQuestionMark[nCntPlayer].col2;
		pVtx[3].col = g_aQuestionMark[nCntPlayer].col3;

		// テクスチャ座標の設定
		pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
		pVtx[1].tex = D3DXVECTOR2(1.0f, 0.0f);
		pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
		pVtx[3].tex = D3DXVECTOR2(1.0f, 1.0f);

	}
	// 頂点バッファをアンロック
	g_pVtxBuffQuestionMark->Unlock();
}

//======================================================================================
// questionmarkの終了処理
//======================================================================================
void UninitQuestionMark(void)
{
	// テクスチャの破棄
	for (int nCntUI = 0; nCntUI < QUESTIONMARK_TYPE; nCntUI++)
	{
		if (g_apTextureQuestionMark[nCntUI] != NULL)
		{
			g_apTextureQuestionMark[nCntUI]->Release();
			g_apTextureQuestionMark[nCntUI] = NULL;
		}
	}

	// 頂点バッファの破棄
	if (g_pVtxBuffQuestionMark != NULL)
	{
		g_pVtxBuffQuestionMark->Release();
		g_pVtxBuffQuestionMark = NULL;
	}

}

//======================================================================================
// questionmarkの更新処理
//======================================================================================
void UpdateQuestionMark(void)
{
	Player* pPlayer = GetPlayer();
	DropMagic* pDropMagic = GetDropMagic();	// 落ちている魔法を取得

	for (int nCntPlayer = 0; nCntPlayer < MAX_PLAYER; nCntPlayer++, pPlayer++)
	{
		if (pPlayer->bUse == false)
		{
			g_aQuestionMark[nCntPlayer].bDisp = false;
			continue;
		}
		else
		{
			g_aQuestionMark[nCntPlayer].bDisp = true;
		}

		// 位置の更新
		g_aQuestionMark[nCntPlayer].Offset = pPlayer->pos;
		g_aQuestionMark[nCntPlayer].Offset.y += 50.0f;

		// 距離を取得
		float fDistance = DistanceMagicAndMark(nCntPlayer);

		if (fDistance < MAGICEF_SIZE)
		{
			SetMagicEffect(pDropMagic[g_aQuestionMark[nCntPlayer].nIdxMagic].pos, MAGICEF_TYPE_MAGIC, nCntPlayer);
		}
		else
		{
			StopMagicEffect(nCntPlayer);
		}

		// 目的の向きに合わせて表示位置を変更
		UpdateMarkPos(nCntPlayer);

		// 色の変更
		SetMarkColor(nCntPlayer, g_aQuestionMark[nCntPlayer].commandType, fDistance);

		g_aQuestionMark[0].pos.x = sinf(g_aQuestionMark[nCntPlayer].rot.y) * DISTANCE;
		g_aQuestionMark[0].pos.z = cosf(g_aQuestionMark[nCntPlayer].rot.y) * DISTANCE;

		PrintDebugProc("?の位置 : (%f, %f, %f)\n", g_aQuestionMark[nCntPlayer].pos.x, g_aQuestionMark[nCntPlayer].pos.y, g_aQuestionMark[nCntPlayer].pos.z);
		PrintDebugProc("?の向き : (%f, %f, %f)\n", g_aQuestionMark[nCntPlayer].rot.x, g_aQuestionMark[nCntPlayer].rot.y, g_aQuestionMark[nCntPlayer].rot.z);
	}

	VERTEX_3D* pVtx;
	// 頂点バッファをロックし、頂点情報へのポインタを取得
	g_pVtxBuffQuestionMark->Lock(0, 0, (void**)&pVtx, 0);
	for (int nCntPlayer = 0; nCntPlayer < MAX_PLAYER; nCntPlayer++, pVtx += 4)
	{
		// 頂点座標の設定
		pVtx[0].pos = D3DXVECTOR3(-g_aQuestionMark[nCntPlayer].fWidth, g_aQuestionMark[nCntPlayer].fHeight, 0.0f);
		pVtx[1].pos = D3DXVECTOR3( g_aQuestionMark[nCntPlayer].fWidth, g_aQuestionMark[nCntPlayer].fHeight, 0.0f);
		pVtx[2].pos = D3DXVECTOR3(-g_aQuestionMark[nCntPlayer].fWidth, -g_aQuestionMark[nCntPlayer].fHeight, 0.0f);
		pVtx[3].pos = D3DXVECTOR3( g_aQuestionMark[nCntPlayer].fWidth, -g_aQuestionMark[nCntPlayer].fHeight, 0.0f);

		// rhwの設定
		pVtx[0].nor = NORMAL;
		pVtx[1].nor = NORMAL;
		pVtx[2].nor = NORMAL;
		pVtx[3].nor = NORMAL;

		// 頂点カラーの設定
		pVtx[0].col = g_aQuestionMark[nCntPlayer].col0;
		pVtx[1].col = g_aQuestionMark[nCntPlayer].col1;
		pVtx[2].col = g_aQuestionMark[nCntPlayer].col2;
		pVtx[3].col = g_aQuestionMark[nCntPlayer].col3;

		// テクスチャ座標の設定
		pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
		pVtx[1].tex = D3DXVECTOR2(1.0f, 0.0f);
		pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
		pVtx[3].tex = D3DXVECTOR2(1.0f, 1.0f);

	}
	// 頂点バッファをアンロック
	g_pVtxBuffQuestionMark->Unlock();
}

//======================================================================================
// questionmarkの描画処理
//======================================================================================
void DrawQuestionMark(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();	// デバイスの取得
	// UIのマトリックス情報を取得
	D3DXMATRIX UIMatrix, mtxView, mtxParentTrans;
	D3DXMATRIX	mtxParent;					// 親のマトリックス

	Player* pPlayer = GetPlayer();

	// ワールドマトリックスの初期化(デフォルトの値にする)
	D3DXMatrixIdentity(&UIMatrix);

	// アルファテストを有効にする
	pDevice->SetRenderState(D3DRS_ALPHATESTENABLE, TRUE);	// アルファテストを有効にする
	pDevice->SetRenderState(D3DRS_ALPHAFUNC, D3DCMP_GREATER);	// 比較方法を設定(基準値より大きい場合描画)
	pDevice->SetRenderState(D3DRS_ALPHAREF, 0);	// アルファテストの参照値を設定(この場合、0より大きい場合は描画)

	// ライトをオフにする
	pDevice->SetRenderState(D3DRS_LIGHTING, FALSE);

	// カリングをオフにする
	pDevice->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);

	for (int nCntPlayer = 0; nCntPlayer < MAX_PLAYER; nCntPlayer++, pPlayer++)
	{
		if (g_aQuestionMark[nCntPlayer].bDisp == false)
		{
			continue;
		}

		// ポリゴンのワールドマトリックスを初期化
		D3DXMatrixIdentity(&UIMatrix);

		// パーツの位置を反映
		D3DXMatrixTranslation(&mtxParentTrans, g_aQuestionMark[nCntPlayer].Offset.x, g_aQuestionMark[nCntPlayer].Offset.y, g_aQuestionMark[nCntPlayer].Offset.z);
		D3DXMatrixMultiply(&UIMatrix, &UIMatrix, &mtxParentTrans);

		// ワールドマトリックスの設定
		pDevice->SetTransform(D3DTS_WORLD, &UIMatrix);

		// UIのマトリックス情報を取得
		mtxParent = UIMatrix;

		// ワールドマトリックスの設定
		pDevice->SetTransform(D3DTS_WORLD, &mtxParent);

		D3DXMATRIX	mtxRotModel, mtxTransModel;	// 計算用マトリックス

		// ポリゴンのワールドマトリックスを初期化
		D3DXMatrixIdentity(&g_aQuestionMark[nCntPlayer].mtxWorld);

		// ビューマトリックスを取得する
		pDevice->GetTransform(D3DTS_VIEW, &mtxView);
		// ポリゴンをカメラに対して正面に向ける
		D3DXMatrixInverse(&g_aQuestionMark[nCntPlayer].mtxWorld, NULL, &mtxView);	//逆行列を求める
		
		g_aQuestionMark[nCntPlayer].mtxWorld._41 = 0.0f;		//マトリックス(行列)の内容
		g_aQuestionMark[nCntPlayer].mtxWorld._42 = 0.0f;
		g_aQuestionMark[nCntPlayer].mtxWorld._43 = 0.0f;

		// パーツの位置を反映
		D3DXMatrixTranslation(&mtxTransModel, g_aQuestionMark[nCntPlayer].pos.x, g_aQuestionMark[nCntPlayer].pos.y, g_aQuestionMark[nCntPlayer].pos.z);
		D3DXMatrixMultiply(&g_aQuestionMark[nCntPlayer].mtxWorld, &g_aQuestionMark[nCntPlayer].mtxWorld, &mtxTransModel);

		// 算出したパーツのワールドマトリックスと親モデルのマトリックスを掛け合わせる
		D3DXMatrixMultiply(&g_aQuestionMark[nCntPlayer].mtxWorld, &g_aQuestionMark[nCntPlayer].mtxWorld, &mtxParent);

		// パーツのワールドマトリックスを設定
		pDevice->SetTransform(D3DTS_WORLD, &g_aQuestionMark[nCntPlayer].mtxWorld);

		// 頂点バッファをデータストリームに設定
		pDevice->SetStreamSource(0, g_pVtxBuffQuestionMark, 0, sizeof(VERTEX_3D));

		// 頂点フォーマットの設定
		pDevice->SetFVF(FVF_VERTEX_3D);

		// テクスチャの設定
		pDevice->SetTexture(0, g_apTextureQuestionMark[0]);

		// UIの描画
		pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, nCntPlayer * 4, 2);
	}

	// ライトをオンにする
	pDevice->SetRenderState(D3DRS_LIGHTING, TRUE);

	// カリングを元に戻す
	pDevice->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);

	// アルファテストを無効にする
	pDevice->SetRenderState(D3DRS_ALPHATESTENABLE, FALSE);	// アルファテストを無効にする
	pDevice->SetRenderState(D3DRS_ALPHAFUNC, D3DCMP_ALWAYS);	// 比較方法を設定(条件に関わらず描画)
	pDevice->SetRenderState(D3DRS_ALPHAREF, 0);	// アルファテストの参照値を設定(この場合、0より大きい場合は描画)
}

//======================================================================================
// 1番近くに落ちている魔法との距離を返す
//======================================================================================
float DistanceMagicAndMark(int nIdx)
{
	DropMagic* pDropMagic = GetDropMagic();	// 落ちている魔法を取得
	float fLength = 1000000.0f;	// 魔法との距離を保存
	float fCompare = 0.0f;	// 比較用の値を代入
	Player* pPlayer = GetPlayer();
	
	for (int nCntMagic = 0; nCntMagic < MAX_DROPMAGIC; nCntMagic++, pDropMagic++)
	{
		if (pDropMagic->bUse == false)
		{ // 使用されていない場合は処理を繰り返す
			continue;
		}
		bool bCommand = false;	// 同じ魔法を持っていた場合のフラグを管理

		for (int nCntCommand = 0; nCntCommand < MAX_OWNCOMMAND; nCntCommand++)
		{
			if (pPlayer[nIdx].magicbook.OwnCommand[nCntCommand] == pDropMagic->oType)
			{ // プレイヤーが同じ魔法を持っていた場合
				bCommand = true;
				break;
			}
		}

		if (bCommand == true)
		{ // 同じ魔法を持っている場合は処理を繰り返す
			continue;
		}

		// ?と落ちている魔法の距離を判定
		fCompare = sqrtf(((pPlayer[nIdx].pos.x - pDropMagic->pos.x) * (pPlayer[nIdx].pos.x - pDropMagic->pos.x)) + ((pPlayer[nIdx].pos.z - pDropMagic->pos.z) * (pPlayer[nIdx].pos.z - pDropMagic->pos.z))) * 0.5f;
		if (fCompare < fLength)
		{ // 今回の値が前回の値より小さかった場合、距離を代入する
			fLength = fCompare;
			g_aQuestionMark[nIdx].nIdxMagic = nCntMagic;
			g_aQuestionMark[nIdx].commandType = pDropMagic->oType;
		}
	}

	return fLength;
}

//======================================================================================
// 魔法の位置に合わせて?の位置を変更する
//======================================================================================
void UpdateMarkPos(int nIdx)
{
	DropMagic* pDropMagic = GetDropMagic();	// 落ちている魔法を取得

	float fRot = atan2f(pDropMagic[g_aQuestionMark[nIdx].nIdxMagic].pos.x - g_aQuestionMark[nIdx].Offset.x, pDropMagic[g_aQuestionMark[nIdx].nIdxMagic].pos.z - g_aQuestionMark[nIdx].Offset.z);
	g_aQuestionMark[nIdx].rot.y = AngleNormalize(fRot);

	// 表示位置の変更
	g_aQuestionMark[nIdx].pos.x = sinf(g_aQuestionMark[nIdx].rot.y) * DISTANCE;
	g_aQuestionMark[nIdx].pos.z = cosf(g_aQuestionMark[nIdx].rot.y) * DISTANCE;
}

//======================================================================================
// 魔法の位置に合わせて?の色を変更する
//======================================================================================
void SetMarkColor(int nIdx, COMMANDOREDER commandType, float fDistance)
{
	// メインの色を指定
	if (commandType == COMMANDOREDER_NONE)
	{
		g_aQuestionMark[nIdx].col0 = COLOR_WHITE;
		g_aQuestionMark[nIdx].col1 = COLOR_WHITE;
		g_aQuestionMark[nIdx].col2 = COLOR_WHITE;
		g_aQuestionMark[nIdx].col3 = COLOR_WHITE;
		PrintDebugProc("色指定エラー\n");
	}
	//３つ同じ並び
	else if (commandType == COMMANDOREDER_GGG)
	{
		g_aQuestionMark[nIdx].col0 = COLOR_GREEN;
		g_aQuestionMark[nIdx].col1 = COLOR_GREEN;
		g_aQuestionMark[nIdx].col2 = COLOR_GREEN;
		g_aQuestionMark[nIdx].col3 = COLOR_GREEN;
		PrintDebugProc("魔法の種類 : COMMANDOREDER_GGG\n");
	}
	else if (commandType == COMMANDOREDER_RRR)
	{
		g_aQuestionMark[nIdx].col0 = COLOR_RED;
		g_aQuestionMark[nIdx].col1 = COLOR_RED;
		g_aQuestionMark[nIdx].col2 = COLOR_RED;
		g_aQuestionMark[nIdx].col3 = COLOR_RED;
		PrintDebugProc("魔法の種類 : COMMANDOREDER_RRR\n");
	}
	else if (commandType == COMMANDOREDER_BBB)
	{
		g_aQuestionMark[nIdx].col0 = COLOR_BLUE;
		g_aQuestionMark[nIdx].col1 = COLOR_BLUE;
		g_aQuestionMark[nIdx].col2 = COLOR_BLUE;
		g_aQuestionMark[nIdx].col3 = COLOR_BLUE;
		PrintDebugProc("魔法の種類 : COMMANDOREDER_BBB\n");
	}
	else if (commandType == COMMANDOREDER_YYY)
	{
		g_aQuestionMark[nIdx].col0 = COLOR_YELLOW;
		g_aQuestionMark[nIdx].col1 = COLOR_YELLOW;
		g_aQuestionMark[nIdx].col2 = COLOR_YELLOW;
		g_aQuestionMark[nIdx].col3 = COLOR_YELLOW;
		PrintDebugProc("魔法の種類 : COMMANDOREDER_YYY\n");
	}
	//赤赤緑 | 赤緑赤 | 緑赤赤
	else if (commandType == COMMANDOREDER_RRG || commandType == COMMANDOREDER_RGR || commandType == COMMANDOREDER_GRR)
	{
		g_aQuestionMark[nIdx].col0 = COLOR_RED;
		g_aQuestionMark[nIdx].col1 = COLOR_RED;
		g_aQuestionMark[nIdx].col2 = COLOR_GREEN;
		g_aQuestionMark[nIdx].col3 = COLOR_GREEN;
		PrintDebugProc("魔法の種類 : COMMANDOREDER_RRG\n");
	}
	//赤黄黄 | 黄赤黄 | 黄黄赤
	else if (commandType == COMMANDOREDER_RYY || commandType == COMMANDOREDER_YRY || commandType == COMMANDOREDER_YYR)
	{
		g_aQuestionMark[nIdx].col0 = COLOR_YELLOW;
		g_aQuestionMark[nIdx].col1 = COLOR_YELLOW;
		g_aQuestionMark[nIdx].col2 = COLOR_RED;
		g_aQuestionMark[nIdx].col3 = COLOR_RED;
		PrintDebugProc("魔法の種類 : COMMANDOREDER_RYY\n");
	}
	//青青緑 | 青緑青 | 緑青青
	else if (commandType == COMMANDOREDER_BBG || commandType == COMMANDOREDER_BGB || commandType == COMMANDOREDER_GBB)
	{
		g_aQuestionMark[nIdx].col0 = COLOR_BLUE;
		g_aQuestionMark[nIdx].col1 = COLOR_BLUE;
		g_aQuestionMark[nIdx].col2 = COLOR_GREEN;
		g_aQuestionMark[nIdx].col3 = COLOR_GREEN;
		PrintDebugProc("魔法の種類 : COMMANDOREDER_BBG\n");
	}
	//青緑緑 | 緑青緑 | 緑緑青
	else if (commandType == COMMANDOREDER_BGG || commandType == COMMANDOREDER_GBG || commandType == COMMANDOREDER_GGB)
	{
		g_aQuestionMark[nIdx].col0 = COLOR_GREEN;
		g_aQuestionMark[nIdx].col1 = COLOR_GREEN;
		g_aQuestionMark[nIdx].col2 = COLOR_BLUE;
		g_aQuestionMark[nIdx].col3 = COLOR_BLUE;
		PrintDebugProc("魔法の種類 : COMMANDOREDER_BGG\n");
	}
	//青黄黄 | 黄青黄 | 黄黄青
	else if (commandType == COMMANDOREDER_BYY || commandType == COMMANDOREDER_YBY || commandType == COMMANDOREDER_YYB)
	{
		g_aQuestionMark[nIdx].col0 = COLOR_YELLOW;
		g_aQuestionMark[nIdx].col1 = COLOR_YELLOW;
		g_aQuestionMark[nIdx].col2 = COLOR_BLUE;
		g_aQuestionMark[nIdx].col3 = COLOR_BLUE;
		PrintDebugProc("魔法の種類 : COMMANDOREDER_BYY\n");
	}
	//緑緑黄 | 緑黄緑 | 黄緑緑
	else if (commandType == COMMANDOREDER_GGY || commandType == COMMANDOREDER_GYG || commandType == COMMANDOREDER_YGG)
	{
		g_aQuestionMark[nIdx].col0 = COLOR_GREEN;
		g_aQuestionMark[nIdx].col1 = COLOR_GREEN;
		g_aQuestionMark[nIdx].col2 = COLOR_YELLOW;
		g_aQuestionMark[nIdx].col3 = COLOR_YELLOW;
		PrintDebugProc("魔法の種類 : COMMANDOREDER_GGY\n");
	}
	//赤緑青
	else if (commandType == COMMANDOREDER_RGB)
	{
		g_aQuestionMark[nIdx].col0 = COLOR_RED;
		g_aQuestionMark[nIdx].col1 = COLOR_GREEN;
		g_aQuestionMark[nIdx].col2 = COLOR_BLUE;
		g_aQuestionMark[nIdx].col3 = COLOR_YELLOW;
		PrintDebugProc("魔法の種類 : COMMANDOREDER_RGB\n");
	}

	float fAlpha = 0.0f;
	// アルファ値を管理
	if (fDistance > NONDISP)
	{ // 一定以上離れていたらアルファ値を0.0fにする
		fAlpha = 0.0f;
	}
	else if (fDistance < DISP)
	{ // 一定以上近づいたらアルファ値を1.0fにする
		fAlpha = 1.0f;
	}
	else
	{ // 距離に応じてアルファ値を管理する
		fAlpha = 1.0f -(fDistance / NONDISP);
	}

	g_aQuestionMark[nIdx].col0.a = fAlpha;
	g_aQuestionMark[nIdx].col1.a = fAlpha;
	g_aQuestionMark[nIdx].col2.a = fAlpha;
	g_aQuestionMark[nIdx].col3.a = fAlpha;
}
#endif