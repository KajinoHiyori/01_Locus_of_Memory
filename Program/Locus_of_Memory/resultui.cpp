//======================================================================================
// 
// リザルトのUI処理[resultui.h]
// Author : Kajino Hiyori
//
//======================================================================================
#include "main.h"
#include "game.h"
#include "color.h"
#include "fade.h"
#include "resultui.h"
#include "result.h"
#include "title.h"
#include "input.h"
#include "fog.h"
#include "goal.h"
#include "debugproc.h"
#include "magic.h"

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

// 外部ファイル保存用の構造体[solo]
typedef struct
{
	int nRed;			// 使われたコマンド数[赤]
	int nGreen;			// 使われたコマンド数[緑]
	int nBlue;			// 使われたコマンド数[青]
	int nYellow;		// 使われたコマンド数[黄]
	int nLevitation;	// 使われた魔法数[浮遊]
	int nCombustion;	// 使われた魔法数[燃焼]
	int nFlood;			// 使われた魔法数[洪水]
	int nFlash;			// 使われた魔法数[フラッシュ]
	int nFireBall;		// 使われた魔法数[火球]
	int nSunsetDelay;	// 使われた魔法数[時間停止]
	int nRainPray;		// 使われた魔法数[雨乞い]
	int nFreeze;		// 使われた魔法数[凍結]
	int nGrowth;		// 使われた魔法数[成長]
	int nAcceleration;	// 使われた魔法数[加速]
	int nTimeRevert;	// 使われた魔法数[巻き戻し]
	int nEvent;			// 累計イベント発生回数
}SoloData;

// 外部ファイル保存用の構造体[pair]
typedef struct
{
	int nRed;			// 使われたコマンド数[赤]
	int nGreen;			// 使われたコマンド数[緑]
	int nBlue;			// 使われたコマンド数[青]
	int nYellow;		// 使われたコマンド数[黄]
	int nLevitation;	// 使われた魔法数[浮遊]
	int nCombustion;	// 使われた魔法数[燃焼]
	int nFlood;			// 使われた魔法数[洪水]
	int nFlash;			// 使われた魔法数[フラッシュ]
	int nFireBall;		// 使われた魔法数[火球]
	int nSunsetDelay;	// 使われた魔法数[時間停止]
	int nRainPray;		// 使われた魔法数[雨乞い]
	int nFreeze;		// 使われた魔法数[凍結]
	int nGrowth;		// 使われた魔法数[成長]
	int nAcceleration;	// 使われた魔法数[加速]
	int nTimeRevert;	// 使われた魔法数[巻き戻し]
	int nEvent;			// 累計イベント発生回数
	int nRR;			// コマンド相性[RR]
	int nRG;			// コマンド相性[RG]
	int nRB;			// コマンド相性[RB]
	int nRY;			// コマンド相性[RY]
	int nBB;			// コマンド相性[BB]
	int nBG;			// コマンド相性[BG]
	int nBY;			// コマンド相性[BY]
	int nGG;			// コマンド相性[GG]
	int nGY;			// コマンド相性[GY]
	int nYY;			// コマンド相性[YY]
}PairData;

// マクロ定義
#define UI_ROT			(D3DXVECTOR3(0.0f, 0.0f, 0.0f))	// 表示方向
#define NORMAL			(D3DXVECTOR3(0.0f, 1.0f, 0.0f))	// 法線ベクトル
#define NUM_RESULTUI	(RESULTUITYPE_MAX)	// リザルトUIの表示数
#define MAXRESULT_TEX	(RESULTUITEX_MAX)	// テクスチャの最大数
#define UI_KEY			(30)	// UIのキー数
#define EARLY_CLEAR		(1200)	// 早いクリア
#define SLOWLY_CLEAR	(1700)	// 遅いクリア
#define MANY_EVENT		(10)	// イベント回数が多い
#define LESS_EVENT		(3)		// イベント回数が少ない
#define PAIR_RESULT		(10)	// 2人で発生させたイベント数の分岐

// テクスチャのサイズ管理
#define WIDTH					(32.5f)	// 横幅
#define HEIGHT					(5.0f)	// 縦幅
#define DIAGNOSIS_W				(650.0f * 0.065f)		// 診断結果の幅
#define DIAGNOSIS_H				(100.0f * 0.065f)		// 診断結果の高さ
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
#define YOUARE_POS				(D3DXVECTOR3(-40.0f, 25.0f, -20.0f))
#define COMMANDRESULT_POS		(D3DXVECTOR3(-30.0f, 7.0f, -20.0f))
#define CLEARRESULT_POS			(D3DXVECTOR3(-10.0f, -5.0f, -20.0f))
#define EVENTRESULT_POS			(D3DXVECTOR3(15.0f, -17.0f, -20.0f))

// 2人用のリザルトUIの配置
#define MOSTCOMMANDPAIR_POS		(D3DXVECTOR3(40.0f, 45.0f, 0.0f))
#define COMMAND1P_POS			(D3DXVECTOR3(20.0f, 37.0f, 0.0f))
#define COMMAND2P_POS			(D3DXVECTOR3(60.0f, 37.0f, 0.0f))
#define COMMANDTYPE1P_POS		(D3DXVECTOR3(COMMAND1P_POS.x, COMMAND1P_POS.y - 7.0f, COMMAND1P_POS.z))
#define COMMANDTYPE2P_POS		(D3DXVECTOR3(COMMAND2P_POS.x, COMMAND2P_POS.y - 7.0f, COMMAND2P_POS.z))
#define THEYARE_POS				(D3DXVECTOR3(-40.0f, 15.0f, -20.0f))
#define COMPATIBILITY_POS		(D3DXVECTOR3(-15.0f, -5.0f, -20.0f))
#define CLEARPAIR_POS			(D3DXVECTOR3(15.0f, -15.0f, -20.0f))

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
	"data\\TEXTURE\\result\\result005_03.png",	// クリア時間に応じたリザルト[失敗]
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
int g_nClearTime = 0;	// クリア時間
SoloData g_SoloData;	// 外部ファイル保存[solo]
PairData g_PairData;	// 外部ファイル保存[pair]
TotalData g_TotalData;	// 外部ファイル保存[total]

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
		// データ保存
		SavaPairData();
		break;

	default:	// 1人操作
		SetResultUI1P();
		// データ保存
		SavaSoloData();
		break;
	}
	// データ保存
	SavaTotalData();

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
	GAMESTATE gameState = GetGameState();

	// 診断結果
	SetResultUI(RESULTUITYPE_DIAGNOSIS, RESULTUITEX_DIAGNOSIS, DIAGNOSIS_POS);
	// 1番使ったコマンド
	SetResultUI(RESULTUITYPE_MOSTCOMMAND, RESULTUITEX_MOSTCOMMAND, MOSTCOMMANDPAIR_POS);
	// コマンドの種類[solo]
	SetResultUI(RESULTUITYPE_COMMMANDTYPESOLO, MostCommand(0), COMMANDTYPESOLO_POS);
	if (gameState == GAMESTATE_CLEAR)
	{
		// 神殿到達時刻
		SetResultUI(RESULTUITYPE_CLEARTIME, RESULTUITEX_TEMPLE, CLEARTIME_POS);
		// 神殿到達時刻[時間]
		SetResultUI(RESULTUITYPE_HOUR0, RESULTUITEX_CLEARTIME, HOUR0_POS);
		SetResultUI(RESULTUITYPE_HOUR1, RESULTUITEX_CLEARTIME, HOUR1_POS);
		SetResultUI(RESULTUITYPE_COLON, RESULTUITEX_COLON, COLON_POS);
		SetResultUI(RESULTUITYPE_MIN0, RESULTUITEX_CLEARTIME, MIN0_POS);
		SetResultUI(RESULTUITYPE_MIN1, RESULTUITEX_CLEARTIME, MIN1_POS);
	}
	// あなたは
	SetResultUI(RESULTUITYPE_YOUARE, RESULTUITEX_YOUARE, YOUARE_POS);
	// コマンド数に応じたリザルト
	SetResultUI(RESULTUITYPE_COMMANDRESULT, CommandResult(0), COMMANDRESULT_POS);
	// クリア時間に応じたリザルト
	SetResultUI(RESULTUITYPE_CLEARRESULT, ClearResult(), CLEARRESULT_POS);
	// イベント発生回数に応じたリザルト
	SetResultUI(RESULTUITYPE_EVENTRESULT, EventResult(0), EVENTRESULT_POS);

}

//======================================================================================
// 2PのUI表示処理
//======================================================================================
void SetResultUI2P(void)
{
	GAMESTATE gameState = GetGameState();

	// 診断結果
	SetResultUI(RESULTUITYPE_DIAGNOSIS, RESULTUITEX_DIAGNOSIS, DIAGNOSIS_POS);
	// 1番使ったコマンド
	SetResultUI(RESULTUITYPE_MOSTCOMMAND, RESULTUITEX_MOSTCOMMAND, MOSTCOMMANDPAIR_POS);
	SetResultUI(RESULTUITYPE_1P, RESULTUITEX_1P, COMMAND1P_POS);
	SetResultUI(RESULTUITYPE_COMMMANDTYPE1P, MostCommand(0), COMMANDTYPE1P_POS);
	SetResultUI(RESULTUITYPE_2P, RESULTUITEX_2P, COMMAND2P_POS);
	SetResultUI(RESULTUITYPE_COMMMANDTYPE2P, MostCommand(1), COMMANDTYPE2P_POS);
	if (gameState == GAMESTATE_CLEAR)
	{
		// 神殿到達時刻
		SetResultUI(RESULTUITYPE_CLEARTIME, RESULTUITEX_TEMPLE, CLEARTIME_POS);
		// 神殿到達時刻[時間]
		SetResultUI(RESULTUITYPE_HOUR0, RESULTUITEX_CLEARTIME, HOUR0_POS);
		SetResultUI(RESULTUITYPE_HOUR1, RESULTUITEX_CLEARTIME, HOUR1_POS);
		SetResultUI(RESULTUITYPE_COLON, RESULTUITEX_COLON, COLON_POS);
		SetResultUI(RESULTUITYPE_MIN0, RESULTUITEX_CLEARTIME, MIN0_POS);
		SetResultUI(RESULTUITYPE_MIN1, RESULTUITEX_CLEARTIME, MIN1_POS);
	}
	// あなたたちは
	SetResultUI(RESULTUITYPE_THEYARE, RESULTUITEX_THEYARE, THEYARE_POS);
	// コマンド相性[pair]
	SetResultUI(RESULTUITYPE_COMPATIBILITY, PairCommand(), COMPATIBILITY_POS);
	// ペアリザルト[pair]
	SetResultUI(RESULTUITYPE_PAIRCLEAR, PairResult(), CLEARPAIR_POS);
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

		// テクスチャの読み込みを管理
		float TexU = 0;
		switch (type)
		{
		case RESULTUITYPE_HOUR0:	// X0:00
			TexU = g_nClearTime % 10000 / 1000;
			pVtx[0].tex = D3DXVECTOR2(TexU * 0.1f, 0.0f);
			pVtx[1].tex = D3DXVECTOR2(TexU * 0.1f + 0.1f, 0.0f);
			pVtx[2].tex = D3DXVECTOR2(TexU * 0.1f, 1.0f);
			pVtx[3].tex = D3DXVECTOR2(TexU * 0.1f + 0.1f, 1.0f);
			break;

		case RESULTUITYPE_HOUR1:	// 0X:00
			TexU = g_nClearTime % 1000 / 100;
			pVtx[0].tex = D3DXVECTOR2(TexU * 0.1f, 0.0f);
			pVtx[1].tex = D3DXVECTOR2(TexU * 0.1f + 0.1f, 0.0f);
			pVtx[2].tex = D3DXVECTOR2(TexU * 0.1f, 1.0f);
			pVtx[3].tex = D3DXVECTOR2(TexU * 0.1f + 0.1f, 1.0f);
			break;

		case RESULTUITYPE_MIN0:	// 00:X0
			TexU = g_nClearTime % 100 / 10;
			pVtx[0].tex = D3DXVECTOR2(TexU * 0.1f, 0.0f);
			pVtx[1].tex = D3DXVECTOR2(TexU * 0.1f + 0.1f, 0.0f);
			pVtx[2].tex = D3DXVECTOR2(TexU * 0.1f, 1.0f);
			pVtx[3].tex = D3DXVECTOR2(TexU * 0.1f + 0.1f, 1.0f);
			break;

		case RESULTUITYPE_MIN1:	// 00:0X
			TexU = g_nClearTime % 100 / 10;
			pVtx[0].tex = D3DXVECTOR2(TexU * 0.1f, 0.0f);
			pVtx[1].tex = D3DXVECTOR2(TexU * 0.1f + 0.1f, 0.0f);
			pVtx[2].tex = D3DXVECTOR2(TexU * 0.1f, 1.0f);
			pVtx[3].tex = D3DXVECTOR2(TexU * 0.1f + 0.1f, 1.0f);
			break;

		default:
			pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
			pVtx[1].tex = D3DXVECTOR2(1.0f, 0.0f);
			pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
			pVtx[3].tex = D3DXVECTOR2(1.0f, 1.0f);
			break;
		}
		break;
	}

	// 頂点バッファをアンロック
	g_pVtxBuffResultUI->Unlock();
}

//======================================================================================
// クリア時間の確認
//======================================================================================
void SetClearTime(int nTime)
{
	g_nClearTime = nTime;
}

//======================================================================================
// 1番使われたコマンドの判定
//======================================================================================
RESULTUITEX MostCommand(int nIdx)
{
	RESULTUITEX tex = RESULTUITEX_DIAGNOSIS;
	// コマンドの使用回数を取得
	MagicCounter* pMagicCounter = GetMagicCounter(nIdx);
	int nMax = -10;	// 1番多いコマンドの回数を保存
	COMMANDTYPE commandType = COMMANDTYPE_R;	// 1番多いコマンドの種類を保存

	// 1番使われたものを判定
	for (int nCntCommand = 0; nCntCommand < COMMANDTYPE_MAX; nCntCommand++)
	{
		if (nMax < pMagicCounter->nCommandCounter[nCntCommand])
		{
			nMax = pMagicCounter->nCommandCounter[nCntCommand];
			commandType = (COMMANDTYPE)nCntCommand;
		}
	}

	switch (commandType)
	{
	case COMMANDTYPE_R:	// R
		tex = RESULTUITEX_R;
		break;
	case COMMANDTYPE_G:	// G
		tex = RESULTUITEX_G;
		break;
	case COMMANDTYPE_B:	// B
		tex = RESULTUITEX_B;
		break;
	case COMMANDTYPE_Y:	// Y
		tex = RESULTUITEX_Y;
		break;
	default:
		tex = RESULTUITEX_R;
		break;
	}

	return tex;
}

//======================================================================================
// コマンドの種類に応じたリザルト
//======================================================================================
RESULTUITEX CommandResult(int nIdx)
{
	RESULTUITEX command = MostCommand(nIdx);	// 1番多いコマンドを取得
	RESULTUITEX tex = RESULTUITEX_DIAGNOSIS;

	// コマンドの種類に応じてテクスチャを変更
	switch (command)
	{
	case RESULTUITEX_R:	// R
		tex = RESULTUITEX_COMMANDR;
		break;

	case RESULTUITEX_G:	// G
		tex = RESULTUITEX_COMMANDG;
		break;

	case RESULTUITEX_B:	// B
		tex = RESULTUITEX_COMMANDB;
		break;

	case RESULTUITEX_Y:	// Y
		tex = RESULTUITEX_COMMANDY;
		break;

	default:
		tex = RESULTUITEX_COMMANDR;
		break;
	}

	return tex;
}

//======================================================================================
// クリア時間に応じたリザルト
//======================================================================================
RESULTUITEX ClearResult(void)
{
	RESULTUITEX tex = RESULTUITEX_DIAGNOSIS;
	GAMESTATE gameState = GetGameState();
	
	// クリア時間に応じてテクスチャを変化
	if (g_nClearTime < EARLY_CLEAR)	// 規定時間より早くクリア
	{
		tex = RESULTUITEX_CLEAREARLY;
	}
	else if (g_nClearTime >= EARLY_CLEAR && g_nClearTime <= SLOWLY_CLEAR)
	{
		tex = RESULTUITEX_CLEARNORMAL;
	}
	else if (g_nClearTime > SLOWLY_CLEAR)
	{ 
		tex = RESULTUITEX_CLEARSLOWLY;
	}
	
	if (gameState != GAMESTATE_CLEAR)
	{
		tex = RESULTUITEX_FAILED;
	}

	return tex;
}

//======================================================================================
// 発生させたイベント数に応じたリザルト
//======================================================================================
RESULTUITEX EventResult(int nIdx)
{
	RESULTUITEX tex = RESULTUITEX_DIAGNOSIS;
	int nNumEvent = nGetMagicEvent(nIdx);

	// クリア時間に応じてテクスチャを変化
	if (nNumEvent < LESS_EVENT)	// 規定時間より早くクリア
	{
		tex = RESULTUITEX_EVENTLESS;
	}
	else if (nNumEvent >= LESS_EVENT && nNumEvent <= MANY_EVENT)
	{
		tex = RESULTUITEX_EVENTNORMAL;
	}
	else if (nNumEvent > MANY_EVENT)
	{
		tex = RESULTUITEX_EVENTMANY;
	}

	return tex;
}

//======================================================================================
// コマンド相性に応じたリザルト
//======================================================================================
RESULTUITEX PairCommand(void)
{
	RESULTUITEX tex = RESULTUITEX_DIAGNOSIS;
	RESULTUITEX command0 = MostCommand(0);
	RESULTUITEX command1 = MostCommand(1);
	
	// 相性ごとによって判定=======================================
	// コマンド相性[RR]
	if (command0 == RESULTUITEX_R && command1 == RESULTUITEX_R)
	{
		tex = RESULTUITEX_COMMANDRR;
	}
	// コマンド相性[RG]
	else if ((command0 == RESULTUITEX_R && command1 == RESULTUITEX_G) || (command0 == RESULTUITEX_G && command1 == RESULTUITEX_R))
	{
		tex = RESULTUITEX_COMMANDRG;
	}
	// コマンド相性[RB]
	else if ((command0 == RESULTUITEX_R && command1 == RESULTUITEX_B) || (command0 == RESULTUITEX_B && command1 == RESULTUITEX_R))
	{
		tex = RESULTUITEX_COMMANDRB;
	}
	// コマンド相性[RY]
	else if ((command0 == RESULTUITEX_R && command1 == RESULTUITEX_Y) || (command0 == RESULTUITEX_Y && command1 == RESULTUITEX_R))
	{
		tex = RESULTUITEX_COMMANDRY;
	}
	// コマンド相性[BB]
	else if (command0 == RESULTUITEX_B && command1 == RESULTUITEX_B)
	{
		tex = RESULTUITEX_COMMANDBB;
	}
	// コマンド相性[BG]
	else if ((command0 == RESULTUITEX_B && command1 == RESULTUITEX_G) || (command0 == RESULTUITEX_G && command1 == RESULTUITEX_B))
	{
		tex = RESULTUITEX_COMMANDBG;
	}
	// コマンド相性[BY]
	else if ((command0 == RESULTUITEX_B && command1 == RESULTUITEX_Y) || (command0 == RESULTUITEX_Y && command1 == RESULTUITEX_B))
	{
		tex = RESULTUITEX_COMMANDBY;
	}
	// コマンド相性[GG]
	else if (command0 == RESULTUITEX_G && command1 == RESULTUITEX_G)
	{
		tex = RESULTUITEX_COMMANDGG;
	}
	// コマンド相性[GY]
	else if ((command0 == RESULTUITEX_G && command1 == RESULTUITEX_Y) || (command0 == RESULTUITEX_Y && command1 == RESULTUITEX_G))
	{
		tex = RESULTUITEX_COMMANDGY;
	}
	// コマンド相性[YY]
	else if (command0 == RESULTUITEX_Y && command1 == RESULTUITEX_Y)
	{
		tex = RESULTUITEX_COMMANDYY;
	}
	else
	{
		tex = RESULTUITEX_COMMANDRR;
	}
	return tex;
}

//======================================================================================
// 2人のイベント発生回数に応じたリザルト
//======================================================================================
RESULTUITEX PairResult(void)
{
	RESULTUITEX tex = RESULTUITEX_DIAGNOSIS;
	int nNumEvent = nGetMagicEvent(0) + nGetMagicEvent(1);

	// クリア時間に応じてテクスチャを変化
	if (nNumEvent < PAIR_RESULT)	// 規定時間より早くクリア
	{
		tex = RESULTUITEX_PAIREVENTLESS;
	}
	else
	{
		tex = RESULTUITEX_PAIREVENTMORE;
	}

	return tex;
}

//======================================================================================
// データを外部ファイルに記録する[1人プレイ]
//======================================================================================
void SavaSoloData(void)
{
	// データを外部ファイルから読み込む[1人プレイ]
	LoadSoloData();

	FILE* pFile;
	// ファイルを開く
	pFile = fopen(SCRIPTS_SOLORESULT, "w");

	if (pFile != NULL)
	{ // ファイルが開けた場合
		// 情報を格納する================================================================
		MagicCounter* pMagicCounter = GetMagicCounter(0);
		// コマンドの使用回数
		g_SoloData.nRed		= g_SoloData.nRed		+ pMagicCounter->nCommandCounter[COMMANDTYPE_R];
		g_SoloData.nGreen	= g_SoloData.nGreen		+ pMagicCounter->nCommandCounter[COMMANDTYPE_G];
		g_SoloData.nBlue	= g_SoloData.nBlue		+ pMagicCounter->nCommandCounter[COMMANDTYPE_B];
		g_SoloData.nYellow	= g_SoloData.nYellow	+ pMagicCounter->nCommandCounter[COMMANDTYPE_Y];
		// 魔法の使用回数
		g_SoloData.nLevitation		= g_SoloData.nLevitation	+ pMagicCounter->nMagicTypeCounter[MAGICTYPE_LEVITATION];	// 使われた魔法数[浮遊]
		g_SoloData.nCombustion		= g_SoloData.nCombustion	+ pMagicCounter->nMagicTypeCounter[MAGICTYPE_COMBUSTION];	// 使われた魔法数[燃焼]
		g_SoloData.nFlood			= g_SoloData.nFlood			+ pMagicCounter->nMagicTypeCounter[MAGICTYPE_FLOOD];		// 使われた魔法数[洪水]
		g_SoloData.nFlash			= g_SoloData.nFlash			+ pMagicCounter->nMagicTypeCounter[MAGICTYPE_FLASH];		// 使われた魔法数[フラッシュ]
		g_SoloData.nFireBall		= g_SoloData.nFireBall		+ pMagicCounter->nMagicTypeCounter[MAGICTYPE_FIREBALL];		// 使われた魔法数[火球]
		g_SoloData.nSunsetDelay		= g_SoloData.nSunsetDelay	+ pMagicCounter->nMagicTypeCounter[MAGICTYPE_SUNSETDELAY];	// 使われた魔法数[時間停止]
		g_SoloData.nRainPray		= g_SoloData.nRainPray		+ pMagicCounter->nMagicTypeCounter[MAGICTYPE_RAINPRAY];		// 使われた魔法数[雨乞い]
		g_SoloData.nFreeze			= g_SoloData.nFreeze		+ pMagicCounter->nMagicTypeCounter[MAGICTYPE_FREEZE];		// 使われた魔法数[凍結]
		g_SoloData.nGrowth			= g_SoloData.nGrowth		+ pMagicCounter->nMagicTypeCounter[MAGICTYPE_GROWTH];		// 使われた魔法数[成長]
		g_SoloData.nAcceleration	= g_SoloData.nAcceleration	+ pMagicCounter->nMagicTypeCounter[MAGICTYPE_ACCELERATION];	// 使われた魔法数[加速]
		g_SoloData.nTimeRevert		= g_SoloData.nTimeRevert	+ pMagicCounter->nMagicTypeCounter[MAGICTYPE_TIMEREVERT];	// 使われた魔法数[巻き戻し]
		// イベントの発生回数
		g_SoloData.nEvent	= g_SoloData.nEvent	+ nGetMagicEvent(0);

		// 外部ファイルに情報を書き出す
		fwrite(&g_SoloData, sizeof(SoloData), 1, pFile);
		fclose(pFile);
	}
}

//======================================================================================
// データを外部ファイルに記録する[2人プレイ]
//======================================================================================
void SavaPairData(void)
{
	// データを外部ファイルから読み込む[2人プレイ]
	LoadPairData();

	FILE* pFile;
	// ファイルを開く
	pFile = fopen(SCRIPTS_PAIRRESULT, "w");

	if (pFile != NULL)
	{ // ファイルが開けた場合
		// 情報を格納する================================================================
		MagicCounter* pMagicCounter0 = GetMagicCounter(0);
		MagicCounter* pMagicCounter1 = GetMagicCounter(1);
		// コマンドの使用回数を取得
		g_PairData.nRed		= g_PairData.nRed		+ pMagicCounter0->nCommandCounter[COMMANDTYPE_R]	+ pMagicCounter1->nCommandCounter[COMMANDTYPE_R];	// 使われたコマンド数[赤]
		g_PairData.nGreen	= g_PairData.nGreen		+ pMagicCounter0->nCommandCounter[COMMANDTYPE_G]	+ pMagicCounter1->nCommandCounter[COMMANDTYPE_G];	// 使われたコマンド数[緑]
		g_PairData.nBlue	= g_PairData.nBlue		+ pMagicCounter0->nCommandCounter[COMMANDTYPE_B]	+ pMagicCounter1->nCommandCounter[COMMANDTYPE_B];	// 使われたコマンド数[青]
		g_PairData.nYellow	= g_PairData.nYellow	+ pMagicCounter0->nCommandCounter[COMMANDTYPE_Y]	+ pMagicCounter1->nCommandCounter[COMMANDTYPE_Y];	// 使われたコマンド数[黄]
		// 魔法の使用回数
		g_PairData.nLevitation		= g_PairData.nLevitation	+ pMagicCounter0->nMagicTypeCounter[MAGICTYPE_LEVITATION]	+ pMagicCounter1->nMagicTypeCounter[MAGICTYPE_LEVITATION]	;	// 使われた魔法数[浮遊]
		g_PairData.nCombustion		= g_PairData.nCombustion	+ pMagicCounter0->nMagicTypeCounter[MAGICTYPE_COMBUSTION]	+ pMagicCounter1->nMagicTypeCounter[MAGICTYPE_COMBUSTION]	;	// 使われた魔法数[燃焼]
		g_PairData.nFlood			= g_PairData.nFlood			+ pMagicCounter0->nMagicTypeCounter[MAGICTYPE_FLOOD]		+ pMagicCounter1->nMagicTypeCounter[MAGICTYPE_FLOOD]		;	// 使われた魔法数[洪水]
		g_PairData.nFlash			= g_PairData.nFlash			+ pMagicCounter0->nMagicTypeCounter[MAGICTYPE_FLASH]		+ pMagicCounter1->nMagicTypeCounter[MAGICTYPE_FLASH]		;	// 使われた魔法数[フラッシュ]
		g_PairData.nFireBall		= g_PairData.nFireBall		+ pMagicCounter0->nMagicTypeCounter[MAGICTYPE_FIREBALL]		+ pMagicCounter1->nMagicTypeCounter[MAGICTYPE_FIREBALL]		;	// 使われた魔法数[火球]
		g_PairData.nSunsetDelay		= g_PairData.nSunsetDelay	+ pMagicCounter0->nMagicTypeCounter[MAGICTYPE_SUNSETDELAY]	+ pMagicCounter1->nMagicTypeCounter[MAGICTYPE_SUNSETDELAY]	;	// 使われた魔法数[時間停止]
		g_PairData.nRainPray		= g_PairData.nRainPray		+ pMagicCounter0->nMagicTypeCounter[MAGICTYPE_RAINPRAY]		+ pMagicCounter1->nMagicTypeCounter[MAGICTYPE_RAINPRAY]		;	// 使われた魔法数[雨乞い]
		g_PairData.nFreeze			= g_PairData.nFreeze		+ pMagicCounter0->nMagicTypeCounter[MAGICTYPE_FREEZE]		+ pMagicCounter1->nMagicTypeCounter[MAGICTYPE_FREEZE]		;	// 使われた魔法数[凍結]
		g_PairData.nGrowth			= g_PairData.nGrowth		+ pMagicCounter0->nMagicTypeCounter[MAGICTYPE_GROWTH]		+ pMagicCounter1->nMagicTypeCounter[MAGICTYPE_GROWTH]		;	// 使われた魔法数[成長]
		g_PairData.nAcceleration	= g_PairData.nAcceleration	+ pMagicCounter0->nMagicTypeCounter[MAGICTYPE_ACCELERATION]	+ pMagicCounter1->nMagicTypeCounter[MAGICTYPE_ACCELERATION]	;	// 使われた魔法数[加速]
		g_PairData.nTimeRevert		= g_PairData.nTimeRevert	+ pMagicCounter0->nMagicTypeCounter[MAGICTYPE_TIMEREVERT]	+ pMagicCounter1->nMagicTypeCounter[MAGICTYPE_TIMEREVERT]	;	// 使われた魔法数[巻き戻し]
		// コマンド相性
		switch (PairCommand())
		{
		case RESULTUITEX_COMMANDRR:	// コマンド相性[RR]
			g_PairData.nRR++;
			break;

		case RESULTUITEX_COMMANDRG:	// コマンド相性[RG]
			g_PairData.nRG++;
			break;

		case RESULTUITEX_COMMANDRB:	// コマンド相性[RB]
			g_PairData.nRB++;
			break;

		case RESULTUITEX_COMMANDRY:	// コマンド相性[RY]
			g_PairData.nRY++;
			break;

		case RESULTUITEX_COMMANDBB:	// コマンド相性[BB]
			g_PairData.nBB++;
			break;

		case RESULTUITEX_COMMANDBG:	// コマンド相性[BG]
			g_PairData.nBG++;
			break;

		case RESULTUITEX_COMMANDBY:	// コマンド相性[BY]
			g_PairData.nBY++;
			break;

		case RESULTUITEX_COMMANDGG:	// コマンド相性[GG]
			g_PairData.nGG++;
			break;

		case RESULTUITEX_COMMANDGY:	// コマンド相性[GY]
			g_PairData.nGY++;
			break;

		case RESULTUITEX_COMMANDYY:	// コマンド相性[YY]
			g_PairData.nYY++;
			break;
		}			
		// イベント発生回数
		g_PairData.nEvent	= g_PairData.nEvent + nGetMagicEvent(0) + nGetMagicEvent(1);

		// 外部ファイルに情報を書き出す
		fwrite(&g_PairData, sizeof(PairData), 1, pFile);
		fclose(pFile);
	}
}

//======================================================================================
// データを外部ファイルに記録する[累計]
//======================================================================================
void SavaTotalData(void)
{
	// データを外部ファイルから読み込む[1人プレイ]
	LoadSoloData();
	// データを外部ファイルから読み込む[2人プレイ]
	LoadPairData();
	// データを外部ファイルから読み込む[累計]
	g_TotalData = LoadTotalData();

	FILE* pFile;
	// ファイルを開く
	pFile = fopen(SCRIPTS_TOTALRESULT, "w");

	if (pFile != NULL)
	{ // ファイルが開けた場合
		// 情報を格納する================================================================
		// コマンドの使用回数を取得
		g_TotalData.nRed	= g_TotalData.nRed		+ g_SoloData.nRed		+ g_PairData.nRed;			// 使われたコマンド数[赤]
		g_TotalData.nGreen	= g_TotalData.nGreen	+ g_SoloData.nGreen		+ g_PairData.nGreen;		// 使われたコマンド数[緑]
		g_TotalData.nBlue	= g_TotalData.nBlue		+ g_SoloData.nBlue		+ g_PairData.nBlue;			// 使われたコマンド数[青]
		g_TotalData.nYellow	= g_TotalData.nYellow	+ g_SoloData.nYellow	+ g_PairData.nYellow;		// 使われたコマンド数[黄]
		// 魔法の使用回数
		g_TotalData.nLevitation		= g_TotalData.nLevitation	+ g_SoloData.nLevitation	+ g_PairData.nLevitation;	// 使われた魔法数[浮遊]
		g_TotalData.nCombustion		= g_TotalData.nCombustion	+ g_SoloData.nCombustion	+ g_PairData.nCombustion;	// 使われた魔法数[燃焼]
		g_TotalData.nFlood			= g_TotalData.nFlood		+ g_SoloData.nFlood			+ g_PairData.nFlood;		// 使われた魔法数[洪水]
		g_TotalData.nFlash			= g_TotalData.nFlash		+ g_SoloData.nFlash			+ g_PairData.nFlash;		// 使われた魔法数[フラッシュ]
		g_TotalData.nFireBall		= g_TotalData.nFireBall		+ g_SoloData.nFireBall		+ g_PairData.nFireBall;		// 使われた魔法数[火球]
		g_TotalData.nSunsetDelay	= g_TotalData.nSunsetDelay	+ g_SoloData.nSunsetDelay	+ g_PairData.nSunsetDelay;	// 使われた魔法数[時間停止]
		g_TotalData.nRainPray		= g_TotalData.nRainPray		+ g_SoloData.nRainPray		+ g_PairData.nRainPray;		// 使われた魔法数[雨乞い]
		g_TotalData.nFreeze			= g_TotalData.nFreeze		+ g_SoloData.nFreeze		+ g_PairData.nFreeze;		// 使われた魔法数[凍結]
		g_TotalData.nGrowth			= g_TotalData.nGrowth		+ g_SoloData.nGrowth		+ g_PairData.nGrowth;		// 使われた魔法数[成長]
		g_TotalData.nAcceleration	= g_TotalData.nAcceleration + g_SoloData.nAcceleration	+ g_PairData.nAcceleration;	// 使われた魔法数[加速]
		g_TotalData.nTimeRevert		= g_TotalData.nTimeRevert	+ g_SoloData.nTimeRevert	+ g_PairData.nTimeRevert;	// 使われた魔法数[巻き戻し]
		// イベントの発生回数
		g_TotalData.nEvent	= g_TotalData.nEvent + g_SoloData.nEvent + g_PairData.nEvent;			// 累計イベント発生回数

		// 外部ファイルに情報を書き出す
		fwrite(&g_TotalData, sizeof(TotalData), 1, pFile);
		fclose(pFile);
	}
}

//======================================================================================
// データを外部ファイルから読み込む[1人プレイ]
//======================================================================================
void LoadSoloData(void)
{
	FILE* pFile;
	// ファイルを開く
	pFile = fopen(SCRIPTS_SOLORESULT, "r");

	if (pFile != NULL)
	{
		fread(&g_SoloData, sizeof(SoloData), 1, pFile);
		fclose(pFile);
	}
	else
	{ // ファイルが開けなかった場合
		// コマンドの使用回数
		g_SoloData.nRed		= 0;
		g_SoloData.nGreen	= 0;
		g_SoloData.nBlue	= 0;
		g_SoloData.nYellow	= 0;
		// 魔法の使用回数
		g_SoloData.nLevitation		= 0;	// 使われた魔法数[浮遊]
		g_SoloData.nCombustion		= 0;	// 使われた魔法数[燃焼]
		g_SoloData.nFlood			= 0;	// 使われた魔法数[洪水]
		g_SoloData.nFlash			= 0;	// 使われた魔法数[フラッシュ]
		g_SoloData.nFireBall		= 0;	// 使われた魔法数[火球]
		g_SoloData.nSunsetDelay		= 0;	// 使われた魔法数[時間停止]
		g_SoloData.nRainPray		= 0;	// 使われた魔法数[雨乞い]
		g_SoloData.nFreeze			= 0;	// 使われた魔法数[凍結]
		g_SoloData.nGrowth			= 0;	// 使われた魔法数[成長]
		g_SoloData.nAcceleration	= 0;	// 使われた魔法数[加速]
		g_SoloData.nTimeRevert		= 0;	// 使われた魔法数[巻き戻し]
		// イベントの発生回数
		g_SoloData.nEvent = 0;
	}
}

//======================================================================================
// データを外部ファイルから読み込む[2人プレイ]
//======================================================================================
void LoadPairData(void)
{
	FILE* pFile;
	// ファイルを開く
	pFile = fopen(SCRIPTS_PAIRRESULT, "r");

	if (pFile != NULL)
	{
		fread(&g_PairData, sizeof(PairData), 1, pFile);
		fclose(pFile);
	}
	else
	{ // ファイルが開けなかった場合
		g_PairData.nRed		= 0;		// 使われたコマンド数[赤]
		g_PairData.nGreen	= 0;		// 使われたコマンド数[緑]
		g_PairData.nBlue	= 0;		// 使われたコマンド数[青]
		g_PairData.nYellow	= 0;		// 使われたコマンド数[黄]
		g_PairData.nLevitation		= 0;	// 使われた魔法数[浮遊]
		g_PairData.nCombustion		= 0;	// 使われた魔法数[燃焼]
		g_PairData.nFlood			= 0;	// 使われた魔法数[洪水]
		g_PairData.nFlash			= 0;	// 使われた魔法数[フラッシュ]
		g_PairData.nFireBall		= 0;	// 使われた魔法数[火球]
		g_PairData.nSunsetDelay		= 0;	// 使われた魔法数[時間停止]
		g_PairData.nRainPray		= 0;	// 使われた魔法数[雨乞い]
		g_PairData.nFreeze			= 0;	// 使われた魔法数[凍結]
		g_PairData.nGrowth			= 0;	// 使われた魔法数[成長]
		g_PairData.nAcceleration	= 0;	// 使われた魔法数[加速]
		g_PairData.nTimeRevert		= 0;	// 使われた魔法数[巻き戻し]
		g_PairData.nEvent	= 0;	// 累計イベント発生回数
		g_PairData.nRR	= 0;	// コマンド相性[RR]
		g_PairData.nRG	= 0;	// コマンド相性[RG]
		g_PairData.nRB	= 0;	// コマンド相性[RB]
		g_PairData.nRY	= 0;	// コマンド相性[RY]
		g_PairData.nBB	= 0;	// コマンド相性[BB]
		g_PairData.nBG	= 0;	// コマンド相性[BG]
		g_PairData.nBY	= 0;	// コマンド相性[BY]
		g_PairData.nGG	= 0;	// コマンド相性[GG]
		g_PairData.nGY	= 0;	// コマンド相性[GY]
		g_PairData.nYY	= 0;	// コマンド相性[YY]
	}
}

//======================================================================================
// データを外部ファイルから読み込む[累計]
//======================================================================================
TotalData LoadTotalData(void)
{
	FILE* pFile;
	// ファイルを開く
	pFile = fopen(SCRIPTS_TOTALRESULT, "r");

	TotalData totalData;

	if (pFile != NULL)
	{
		fread(&totalData, sizeof(TotalData), 1, pFile);
		fclose(pFile);
	}
	else
	{
		totalData.nRed = 0;
		totalData.nGreen = 0;
		totalData.nBlue = 0;
		totalData.nYellow = 0;
		totalData.nLevitation = 0;
		totalData.nCombustion = 0;
		totalData.nFlood = 0;
		totalData.nFlash = 0;
		totalData.nFireBall = 0;
		totalData.nSunsetDelay = 0;
		totalData.nRainPray = 0;
		totalData.nFreeze = 0;
		totalData.nGrowth = 0;
		totalData.nAcceleration = 0;
		totalData.nTimeRevert = 0;
		totalData.nEvent = 0;
	}
	return totalData;
}