//======================================================================================
// 
// リザルトのUI処理[resultui.h]
// Author : Kajino Hiyori
//
//======================================================================================
#include "main.h"
#include "color.h"
#include "fade.h"
#include "resultui.h"
#include "result.h"
#include "title.h"
#include "input.h"
#include "fog.h"
#include "goal.h"
#include "debugproc.h"

// リザルトUIの構造体
typedef struct
{
	D3DXMATRIX		mtxWorld;	// ワールドマトリックス
	D3DXVECTOR3		pos;		// 位置
	D3DXVECTOR3		posOffset;	// 基本
	RESULTUITYPE	type;		// 種類
	RESULTUITEX		tex;		// テクスチャ
	float	fWidth;		// 幅
	float	fHeight;	// 高さ
	int		nKey;		// 現在の処理キー
	int		nNumKey;	// フレーム数
	bool	bDisp;		// 表示状態
}ResultUI;

// マクロ定義
#define UI_ROT					(D3DXVECTOR3(0.0f, 0.0f, 0.0f))	// 表示方向
#define NORMAL					(D3DXVECTOR3(0.0f, 1.0f, 0.0f))	// 法線ベクトル
#define NUM_RESULTUI			(RESULTUITYPE_MAX)	// リザルトUIの表示数
#define MAXRESULT_TEX			(RESULTUITEX_MAX)	// テクスチャの最大数
#define WIDTH					(32.5f)	// 横幅
#define HEIGHT					(5.0f)		// 縦幅
#define UI_KEY					(30)		// UIのキー数
#define DIAGNOSIS_W				(650.0f * 0.075f)		// 診断結果の幅
#define DIAGNOSIS_H				(100.0f * 0.075f)		// 診断結果の高さ
#define MOSTCOMMANDSOLO_W		(DIAGNOSIS_W * 0.7f)		// 1番使ったコマンド[solo]
#define MOSTCOMMANDSOLO_H		(DIAGNOSIS_H * 0.7f)		// 1番使ったコマンド[solo]
#define COMMANDTYPESOLO_W		(MOSTCOMMANDSOLO_H)	// コマンドの種類[0.8倍]
#define COMMANDTYPESOLO_H		(MOSTCOMMANDSOLO_H)	// コマンドの種類[0.8倍]
#define NUM_W					(MOSTCOMMANDSOLO_H)	// 数字のサイズ[0.8倍]
#define NUM_H					(MOSTCOMMANDSOLO_H)	// 数字のサイズ[0.8倍]
#define YOUARE_W				(MOSTCOMMANDSOLO_W)	// あなたはのサイズ[0.8倍]
#define YOUARE_H				(MOSTCOMMANDSOLO_H)	// あなたはのサイズ[0.8倍]
#define NUMPLAYER_W				(300.0f * 0.035f)	// PLAYER01 / PLAYER02の幅[0.65倍]
#define NUMPLAYER_H				(150.0f * 0.035f)	// PLAYER01 / PLAYER02の高さ[0.65倍]
#define COMMANDTYPEPAIR_W		(NUMPLAYER_H * 0.9f)		// コマンドの種類の幅[pair]
#define COMMANDTYPEPAIR_H		(NUMPLAYER_H * 0.9f)		// コマンドの種類の高さ[pair]

// 1人用のリザルトUIの配置
#define DIAGNOSIS_POS			(D3DXVECTOR3(-50.0f, 45.0f, 0.0f))
#define MOSTCOMMANDSOLO_POS		(D3DXVECTOR3(40.0f, 40.0f, 0.0f))
#define COMMANDTYPESOLO_POS		(D3DXVECTOR3(65.0f, 30.0f, 0.0f))
#define CLEARTIME_POS			(D3DXVECTOR3(50.0f, 20.0f, 0.0f))
#define HOUR0_POS				(D3DXVECTOR3(37.0f, 10.0f, 0.0f))
#define HOUR1_POS				(D3DXVECTOR3(HOUR0_POS.x + NUM_W + 5.0f, HOUR0_POS.y, HOUR0_POS.z))
#define MIN0_POS				(D3DXVECTOR3(HOUR1_POS.x + NUM_W + 5.0f, HOUR1_POS.y, HOUR1_POS.z))
#define MIN1_POS				(D3DXVECTOR3(MIN0_POS.x + NUM_W + 5.0f, MIN0_POS.y, MIN0_POS.z))
#define COLON_POS				(D3DXVECTOR3(HOUR1_POS.x + 5.125f, HOUR1_POS.y, HOUR1_POS.z))
#define YOUARE_POS				(D3DXVECTOR3(-40.0f, 25.0f, -10.0f))
#define COMMANDRESULT_POS		(D3DXVECTOR3(-40.0f, 10.0f, -10.0f))
#define CLEARRESULT_POS			(D3DXVECTOR3(-10.0f, -5.0f, -10.0f))
#define EVENTRESULT_POS			(D3DXVECTOR3(15.0f, -20.0f, -10.0f))

// 2人用のリザルトUIの配置
#define MOSTCOMMANDPAIR_POS		(D3DXVECTOR3(40.0f, 45.0f, 0.0f))
#define COMMAND1P_POS			(D3DXVECTOR3(20.0f, 37.0f, 0.0f))
#define COMMAND2P_POS			(D3DXVECTOR3(60.0f, 37.0f, 0.0f))
#define COMMANDTYPE1P_POS		(D3DXVECTOR3(COMMAND1P_POS.x, COMMAND1P_POS.y - 7.0f, COMMAND1P_POS.z))
#define COMMANDTYPE2P_POS		(D3DXVECTOR3(COMMAND2P_POS.x, COMMAND2P_POS.y - 7.0f, COMMAND2P_POS.z))
#define THEYARE_POS				(D3DXVECTOR3(-40.0f, 15.0f, -10.0f))
#define COMPATIBILITY_POS		(D3DXVECTOR3(-15.0f, -5.0f, -10.0f))
#define CLEARPAIR_POS			(D3DXVECTOR3(15.0f, -20.0f, -10.0f))

// テクスチャの読み込み
const char* c_apFilenameResultUI[MAXRESULT_TEX] =
{
	"data\\TEXTURE\\result\\result000.png",		// 診断結果
	"data\\TEXTURE\\result\\result001.png",		// 1番使ったコマンド
	"data\\TEXTURE\\SpellUI\\01_Red.png",		// 赤魔法
	"data\\TEXTURE\\SpellUI\\02_Green.png",		// 緑魔法
	"data\\TEXTURE\\SpellUI\\03_Blue.png",		// 青魔法
	"data\\TEXTURE\\SpellUI\\04_Yellow.png",	// 黄魔法
	"data\\TEXTURE\\result\\result002.png",		// 神殿到達時刻
	"data\\TEXTURE\\number.png",				// 数字
	"data\\TEXTURE\\result\\result003.png",		// あなたは
	"data\\TEXTURE\\result\\result004_00.png",	// コマンド使用数に応じたリザルト[R]
	"data\\TEXTURE\\result\\result004_01.png",	// コマンド使用数に応じたリザルト[G]
	"data\\TEXTURE\\result\\result004_02.png",	// コマンド使用数に応じたリザルト[B]
	"data\\TEXTURE\\result\\result004_03.png",	// コマンド使用数に応じたリザルト[Y]
	"data\\TEXTURE\\result\\result005_00.png",	// クリア時間に応じたリザルト[早い]
	"data\\TEXTURE\\result\\result005_01.png",	// クリア時間に応じたリザルト[普通]
	"data\\TEXTURE\\result\\result005_02.png",	// クリア時間に応じたリザルト[遅い]
	"data\\TEXTURE\\result\\result006_00.png",	// イベント発生回数に応じたリザルト[多い]
	"data\\TEXTURE\\result\\result006_01.png",	// イベント発生回数に応じたリザルト[普通]
	"data\\TEXTURE\\result\\result006_02.png",	// イベント発生回数に応じたリザルト[少ない]
	"data\\TEXTURE\\result\\result007.png",		// あなたたちは
	"data\\TEXTURE\\result\\result008_RR.png",	// コマンド相性[RR]
	"data\\TEXTURE\\result\\result008_RG.png",	// コマンド相性[RG]
	"data\\TEXTURE\\result\\result008_RB.png",	// コマンド相性[RB]
	"data\\TEXTURE\\result\\result008_RY.png",	// コマンド相性[RY]
	"data\\TEXTURE\\result\\result008_BB.png",	// コマンド相性[BB]
	"data\\TEXTURE\\result\\result008_BG.png",	// コマンド相性[BG]
	"data\\TEXTURE\\result\\result008_BY.png",	// コマンド相性[BY]
	"data\\TEXTURE\\result\\result008_GG.png",	// コマンド相性[GG]
	"data\\TEXTURE\\result\\result008_GY.png",	// コマンド相性[GY]
	"data\\TEXTURE\\result\\result008_YY.png",	// コマンド相性[YY]
	"data\\TEXTURE\\result\\result009_00.png",	// ペアリザルト[早い]
	"data\\TEXTURE\\result\\result009_01.png",	// ペアリザルト[遅い]
	"data\\TEXTURE\\result\\result010.png",		// :
	"data\\TEXTURE\\result\\result011.png",		// 1P
	"data\\TEXTURE\\result\\result012.png",		// 2P
};

// グローバル変数
LPDIRECT3DTEXTURE9 g_apTextureResultUI[MAXRESULT_TEX] = {};	// テクスチャへのポインタ
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffResultUI = NULL; // 頂点バッファへのポインタ
ResultUI g_aResultUI[NUM_RESULTUI];	// 構造体

//======================================================================================
// ResultUIの初期化処理
//======================================================================================
void InitResultUI(void)
{
	// デバイスの取得
	LPDIRECT3DDEVICE9 pDevice = GetDevice();
	// 操作人数の取得
	OPERATIONTYPE operationType = GetOperationType();

	// テクスチャの読み込み
	for (int nCntUI = 0; nCntUI < MAXRESULT_TEX; nCntUI++)
	{
		D3DXCreateTextureFromFile(pDevice, c_apFilenameResultUI[nCntUI], &g_apTextureResultUI[nCntUI]);
	}

	// 各種初期化
	for (int nCntUI = 0; nCntUI < NUM_RESULTUI; nCntUI++)
	{
		g_aResultUI[nCntUI].pos			= INIT_D3DXVEC3;			// 位置
		g_aResultUI[nCntUI].posOffset	= INIT_D3DXVEC3;			// 位置
		g_aResultUI[nCntUI].type		= RESULTUITYPE_DIAGNOSIS;	// 種類
		g_aResultUI[nCntUI].tex			= RESULTUITEX_DIAGNOSIS;	// テクスチャ
		g_aResultUI[nCntUI].fWidth		= 0.0f;		// 幅
		g_aResultUI[nCntUI].fHeight		= 0.0f;		// 高さ
		g_aResultUI[nCntUI].nKey		= 0;		// 現在の処理キー
		g_aResultUI[nCntUI].nNumKey		= 0;		// フレーム数
		g_aResultUI[nCntUI].bDisp		= false;	// 表示状態
	}

	// 頂点バッファの生成
	pDevice->CreateVertexBuffer(sizeof(VERTEX_3D) * 4 * NUM_RESULTUI, D3DUSAGE_WRITEONLY, FVF_VERTEX_3D, D3DPOOL_MANAGED, &g_pVtxBuffResultUI, NULL);

	switch (operationType)
	{
	case OPERATIONTYPE_2P:	// 2人操作
		SetResultUI2P();
		break;

	default:	// 1人操作
		SetResultUI1P();
		break;
	}

	VERTEX_3D* pVtx;
	// 頂点バッファをロックし、頂点情報へのポインタを取得
	g_pVtxBuffResultUI->Lock(0, 0, (void**)&pVtx, 0);

	for (int nCntUI = 0; nCntUI < NUM_RESULTUI; nCntUI++, pVtx += 4)
	{
		// 頂点座標の設定
		pVtx[0].pos = D3DXVECTOR3(-g_aResultUI[nCntUI].fWidth, g_aResultUI[nCntUI].fHeight, 0.0f);
		pVtx[1].pos = D3DXVECTOR3(g_aResultUI[nCntUI].fWidth, g_aResultUI[nCntUI].fHeight, 0.0f);
		pVtx[2].pos = D3DXVECTOR3(-g_aResultUI[nCntUI].fWidth, -g_aResultUI[nCntUI].fHeight, 0.0f);
		pVtx[3].pos = D3DXVECTOR3(g_aResultUI[nCntUI].fWidth, -g_aResultUI[nCntUI].fHeight, 0.0f);

		// rhwの設定
		pVtx[0].nor = NORMAL;
		pVtx[1].nor = NORMAL;
		pVtx[2].nor = NORMAL;
		pVtx[3].nor = NORMAL;

		// 頂点カラーの設定
		pVtx[0].col = COLOR_WHITE;
		pVtx[1].col = COLOR_WHITE;
		pVtx[2].col = COLOR_WHITE;
		pVtx[3].col = COLOR_WHITE;
	}

	// 頂点バッファをアンロック
	g_pVtxBuffResultUI->Unlock();
}

//======================================================================================
// ResultUIの終了処理
//======================================================================================
void UninitResultUI(void)
{
	// テクスチャの破棄
	for (int nCntUI = 0; nCntUI < MAXRESULT_TEX; nCntUI++)
	{
		if (g_apTextureResultUI[nCntUI] != NULL)
		{
			g_apTextureResultUI[nCntUI]->Release();
			g_apTextureResultUI[nCntUI] = NULL;
		}
	}

	// 頂点バッファの破棄
	if (g_pVtxBuffResultUI != NULL)
	{
		g_pVtxBuffResultUI->Release();
		g_pVtxBuffResultUI = NULL;
	}

}

//======================================================================================
// ResultUIの更新処理
//======================================================================================
void UpdateResultUI(void)
{
	// ゴールの位置に合わせる
	for (int nCntUI = 0; nCntUI < NUM_RESULTUI; nCntUI++)
	{
		g_aResultUI[nCntUI].pos = GOAL_POS;
		g_aResultUI[nCntUI].pos += g_aResultUI[nCntUI].posOffset;
	}
}

//======================================================================================
// ResultUIの描画処理
//======================================================================================
void DrawResultUI(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();	// デバイスの取得
	D3DXMATRIX UIMatrix, mtxTrans, mtxView;	// UIのマトリックス情報を取得
	
	// アルファテストを有効にする
	pDevice->SetRenderState(D3DRS_ALPHATESTENABLE, TRUE);	// アルファテストを有効にする
	pDevice->SetRenderState(D3DRS_ALPHAFUNC, D3DCMP_GREATER);	// 比較方法を設定(基準値より大きい場合描画)
	pDevice->SetRenderState(D3DRS_ALPHAREF, 0);	// アルファテストの参照値を設定(この場合、0より大きい場合は描画)
	
	// ライトをオフにする
	pDevice->SetRenderState(D3DRS_LIGHTING, FALSE);
	
	for (int nCntUI = 0; nCntUI < NUM_RESULTUI; nCntUI++)
	{
		if (g_aResultUI[nCntUI].bDisp == false)
		{
			continue;
		}
	
		// ポリゴンのワールドマトリックスを初期化
		D3DXMatrixIdentity(&g_aResultUI[nCntUI].mtxWorld);
	
		// ビューマトリックスを取得する
		pDevice->GetTransform(D3DTS_VIEW, &mtxView);
	
		// ポリゴンをカメラに対して正面に向ける
		D3DXMatrixInverse(&g_aResultUI[nCntUI].mtxWorld, NULL, &mtxView);	//逆行列を求める
	
		g_aResultUI[nCntUI].mtxWorld._41 = 0.0f;		//マトリックス(行列)の内容
		g_aResultUI[nCntUI].mtxWorld._42 = 0.0f;
		g_aResultUI[nCntUI].mtxWorld._43 = 0.0f;
		
		// パーツの位置を反映
		D3DXMatrixTranslation(&mtxTrans, g_aResultUI[nCntUI].pos.x, g_aResultUI[nCntUI].pos.y, g_aResultUI[nCntUI].pos.z);
		D3DXMatrixMultiply(&g_aResultUI[nCntUI].mtxWorld, &g_aResultUI[nCntUI].mtxWorld, &mtxTrans);
	
		// パーツのワールドマトリックスを設定
		pDevice->SetTransform(D3DTS_WORLD, &g_aResultUI[nCntUI].mtxWorld);
	
		// 頂点バッファをデータストリームに設定
		pDevice->SetStreamSource(0, g_pVtxBuffResultUI, 0, sizeof(VERTEX_3D));
	
		// 頂点フォーマットの設定
		pDevice->SetFVF(FVF_VERTEX_3D);
	
		// テクスチャの設定
		pDevice->SetTexture(0, g_apTextureResultUI[g_aResultUI[nCntUI].tex]);
	
		// UIの描画
		pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, nCntUI * 4, 2);
	
	}
	// ライトをオンにする
	pDevice->SetRenderState(D3DRS_LIGHTING, TRUE);
	
	// アルファテストを無効にする
	pDevice->SetRenderState(D3DRS_ALPHATESTENABLE, FALSE);	// アルファテストを無効にする
	pDevice->SetRenderState(D3DRS_ALPHAFUNC, D3DCMP_ALWAYS);	// 比較方法を設定(条件に関わらず描画)
	pDevice->SetRenderState(D3DRS_ALPHAREF, 0);	// アルファテストの参照値を設定(この場合、0より大きい場合は描画)
}

//======================================================================================
// 1PのUI表示処理
//======================================================================================
void SetResultUI1P(void)
{
	// 診断結果
	SetResultUI(RESULTUITYPE_DIAGNOSIS, RESULTUITEX_DIAGNOSIS, DIAGNOSIS_POS);
	// 1番使ったコマンド
	SetResultUI(RESULTUITYPE_MOSTCOMMAND, RESULTUITEX_MOSTCOMMAND, MOSTCOMMANDPAIR_POS);
	// コマンドの種類[solo]
	SetResultUI(RESULTUITYPE_COMMMANDTYPESOLO, RESULTUITEX_R, COMMANDTYPESOLO_POS);
	// 神殿到達時刻
	SetResultUI(RESULTUITYPE_CLEARTIME, RESULTUITEX_TEMPLE, CLEARTIME_POS);
	// 神殿到達時刻[時間]
	SetResultUI(RESULTUITYPE_HOUR0, RESULTUITEX_CLEARTIME, HOUR0_POS);
	SetResultUI(RESULTUITYPE_HOUR1, RESULTUITEX_CLEARTIME, HOUR1_POS);
	SetResultUI(RESULTUITYPE_COLON, RESULTUITEX_COLON, COLON_POS);
	SetResultUI(RESULTUITYPE_MIN0, RESULTUITEX_CLEARTIME, MIN0_POS);
	SetResultUI(RESULTUITYPE_MIN1, RESULTUITEX_CLEARTIME, MIN1_POS);
	// あなたは
	SetResultUI(RESULTUITYPE_YOUARE, RESULTUITEX_YOUARE, YOUARE_POS);
	// コマンド数に応じたリザルト
	SetResultUI(RESULTUITYPE_COMMANDRESULT, RESULTUITEX_COMMANDR, COMMANDRESULT_POS);
	// クリア時間に応じたリザルト
	SetResultUI(RESULTUITYPE_CLEARRESULT, RESULTUITEX_CLEAREARLY, CLEARRESULT_POS);
	// イベント発生回数に応じたリザルト
	SetResultUI(RESULTUITYPE_EVENTRESULT, RESULTUITEX_EVENTMANY, EVENTRESULT_POS);

}

//======================================================================================
// 2PのUI表示処理
//======================================================================================
void SetResultUI2P(void)
{
	// 診断結果
	SetResultUI(RESULTUITYPE_DIAGNOSIS, RESULTUITEX_DIAGNOSIS, DIAGNOSIS_POS);
	// 1番使ったコマンド
	SetResultUI(RESULTUITYPE_MOSTCOMMAND, RESULTUITEX_MOSTCOMMAND, MOSTCOMMANDPAIR_POS);
	SetResultUI(RESULTUITYPE_1P, RESULTUITEX_1P, COMMAND1P_POS);
	SetResultUI(RESULTUITYPE_COMMMANDTYPE1P, RESULTUITEX_R, COMMANDTYPE1P_POS);
	SetResultUI(RESULTUITYPE_2P, RESULTUITEX_2P, COMMAND2P_POS);
	SetResultUI(RESULTUITYPE_COMMMANDTYPE2P, RESULTUITEX_R, COMMANDTYPE2P_POS);
	// 神殿到達時刻
	SetResultUI(RESULTUITYPE_CLEARTIME, RESULTUITEX_TEMPLE, CLEARTIME_POS);
	// 神殿到達時刻[時間]
	SetResultUI(RESULTUITYPE_HOUR0, RESULTUITEX_CLEARTIME, HOUR0_POS);
	SetResultUI(RESULTUITYPE_HOUR1, RESULTUITEX_CLEARTIME, HOUR1_POS);
	SetResultUI(RESULTUITYPE_COLON, RESULTUITEX_COLON, COLON_POS);
	SetResultUI(RESULTUITYPE_MIN0, RESULTUITEX_CLEARTIME, MIN0_POS);
	SetResultUI(RESULTUITYPE_MIN1, RESULTUITEX_CLEARTIME, MIN1_POS);
	// あなたたちは
	SetResultUI(RESULTUITYPE_THEYARE, RESULTUITEX_THEYARE, THEYARE_POS);
	// コマンド相性[pair]
	SetResultUI(RESULTUITYPE_COMPATIBILITY, RESULTUITEX_COMMANDGG, COMPATIBILITY_POS);
	// ペアリザルト[pair]
	SetResultUI(RESULTUITYPE_PAIRCLEAR, RESULTUITEX_PAIREARLY, CLEARPAIR_POS);
}

//======================================================================================
// UI設置処理
//======================================================================================
void SetResultUI(RESULTUITYPE type, RESULTUITEX tex, D3DXVECTOR3 pos)
{
	VERTEX_3D* pVtx;
	// 頂点バッファをロックし、頂点情報へのポインタを取得
	g_pVtxBuffResultUI->Lock(0, 0, (void**)&pVtx, 0);

	for (int nCntUI = 0; nCntUI < NUM_RESULTUI; nCntUI++)
	{
		if (g_aResultUI[nCntUI].bDisp == true)
		{
			continue;
		}
		
		pVtx += nCntUI * 4;

		switch (type)
		{
		case RESULTUITYPE_DIAGNOSIS:	// 診断結果
			g_aResultUI[nCntUI].fHeight = DIAGNOSIS_H;
			g_aResultUI[nCntUI].fWidth	= DIAGNOSIS_W;
			break;

		case RESULTUITYPE_MOSTCOMMAND:	// 1番使ったコマンド
			g_aResultUI[nCntUI].fHeight = MOSTCOMMANDSOLO_H;
			g_aResultUI[nCntUI].fWidth = MOSTCOMMANDSOLO_W;
			break;

		case RESULTUITYPE_COMMMANDTYPESOLO:	// コマンドの種類[solo]
			g_aResultUI[nCntUI].fHeight = COMMANDTYPESOLO_W;
			g_aResultUI[nCntUI].fWidth = COMMANDTYPESOLO_W;
			break;

		case RESULTUITYPE_CLEARTIME:	// クリア時間
			g_aResultUI[nCntUI].fHeight = MOSTCOMMANDSOLO_H;
			g_aResultUI[nCntUI].fWidth = MOSTCOMMANDSOLO_W;
			break;

		case RESULTUITYPE_HOUR0:	// 時間
			g_aResultUI[nCntUI].fHeight = NUM_H;
			g_aResultUI[nCntUI].fWidth = NUM_W;
			break;

		case RESULTUITYPE_HOUR1:	// 時間
			g_aResultUI[nCntUI].fHeight = NUM_H;
			g_aResultUI[nCntUI].fWidth = NUM_W;
			break;

		case RESULTUITYPE_COLON:	// :
			g_aResultUI[nCntUI].fHeight = MOSTCOMMANDSOLO_H;
			g_aResultUI[nCntUI].fWidth = MOSTCOMMANDSOLO_W;
			break;

		case RESULTUITYPE_MIN0:	// 時間
			g_aResultUI[nCntUI].fHeight = NUM_H;
			g_aResultUI[nCntUI].fWidth = NUM_W;
			break;

		case RESULTUITYPE_MIN1:	// 時間
			g_aResultUI[nCntUI].fHeight = NUM_H;
			g_aResultUI[nCntUI].fWidth = NUM_W;
			break;

		case RESULTUITYPE_YOUARE:	// あなたは
			g_aResultUI[nCntUI].fHeight = YOUARE_H;
			g_aResultUI[nCntUI].fWidth = YOUARE_W;
			break;

		case RESULTUITYPE_1P:	// 1P
			g_aResultUI[nCntUI].fHeight = NUMPLAYER_H;
			g_aResultUI[nCntUI].fWidth = NUMPLAYER_W;
			break;

		case RESULTUITYPE_2P:	// 2P
			g_aResultUI[nCntUI].fHeight = NUMPLAYER_H;
			g_aResultUI[nCntUI].fWidth = NUMPLAYER_W;
			break;

		case RESULTUITYPE_COMMMANDTYPE1P:	// 1Pのコマンドの種類
			g_aResultUI[nCntUI].fHeight = COMMANDTYPEPAIR_W;
			g_aResultUI[nCntUI].fWidth = COMMANDTYPEPAIR_H;
			break;

		case RESULTUITYPE_COMMMANDTYPE2P:	// 2Pのコマンドの種類
			g_aResultUI[nCntUI].fHeight = COMMANDTYPEPAIR_W;
			g_aResultUI[nCntUI].fWidth = COMMANDTYPEPAIR_H;
			break;

		case RESULTUITYPE_THEYARE:	// あなたたちは
			g_aResultUI[nCntUI].fHeight = YOUARE_H;
			g_aResultUI[nCntUI].fWidth = YOUARE_W;
			break;

		case RESULTUITYPE_COMPATIBILITY:	// 相性
			g_aResultUI[nCntUI].fHeight = DIAGNOSIS_H;
			g_aResultUI[nCntUI].fWidth = DIAGNOSIS_W;
			break;

		case RESULTUITYPE_PAIRCLEAR:	// ペアリザルト
			g_aResultUI[nCntUI].fHeight = DIAGNOSIS_H;
			g_aResultUI[nCntUI].fWidth = DIAGNOSIS_W;
			break;

		default:
			g_aResultUI[nCntUI].fHeight = DIAGNOSIS_H;
			g_aResultUI[nCntUI].fWidth = DIAGNOSIS_W;
			break;
		}
		
		g_aResultUI[nCntUI].nKey = 0;
		g_aResultUI[nCntUI].nNumKey = UI_KEY;
		g_aResultUI[nCntUI].posOffset = pos;
		g_aResultUI[nCntUI].tex = tex;
		g_aResultUI[nCntUI].type = type;
		g_aResultUI[nCntUI].bDisp = true;

		if (type == RESULTUITYPE_HOUR0 || type == RESULTUITYPE_HOUR1 || type == RESULTUITYPE_MIN0 || type == RESULTUITYPE_MIN1)
		{
			pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
			pVtx[1].tex = D3DXVECTOR2(0.1f, 0.0f);
			pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
			pVtx[3].tex = D3DXVECTOR2(0.1f, 1.0f);
		}
		else
		{
			pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
			pVtx[1].tex = D3DXVECTOR2(1.0f, 0.0f);
			pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
			pVtx[3].tex = D3DXVECTOR2(1.0f, 1.0f);
		}

		break;
	}

	// 頂点バッファをアンロック
	g_pVtxBuffResultUI->Unlock();
}
