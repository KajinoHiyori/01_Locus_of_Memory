//=============================================================================
//
//	スクリプト読み込み処理 [loadsclipt.cpp]
//	Author : SHUMA AIZU
// 
//=============================================================================

#include "loadscript.h"
#include "object.h"
#include "player.h"
#include "motion.h"

//*****************************************************************************
// マクロ定義
//*****************************************************************************
#define MAX_STRING			(256)				// 読み込める文字列の最大
#define LOAD_START			"SCRIPT"			// 読み込み
#define LOAD_END			"END_SCRIPT"		// 終了
#define LOAD_OBJECT			"OBJECTSCRIPT"		// オブジェクトスクリプト読み込み
#define LOAD_PARENTMODEL	"PARENTSCRIPT"		// 階層構造モデルスクリプト読み込み
#define LOAD_PLAYER			"CHARACTERSET"		// プレイヤー情報読み込み
#define LOAD_ENDPLAYER		"END_CHARACTERSET"	// プレイヤー情報読み込み終了
#define LOAD_PARTS			"PARTSSET"			// パーツ情報読み込み
#define LOAD_ENDPARTS		"END_PARTSSET"		// パーツ情報読み込み終了
#define LOAD_MOTION			"MOTIONSCRIPT"		// モーションスクリプト読み込み
#define LOAD_MOTIONINFO		"MOTIONSET"			// モーション情報読み込み
#define LOAD_ENDMOTIONINFO	"END_MOTIONSET"		// モーション情報読み込み終了
#define LOAD_NUMMODEL		"NUM_MODEL"			// モデル数読み込み
#define LOAD_MODEL			"MODEL_FILENAME"	// モデルファイル名読み込み
#define LOAD_MESH			"MESH_FILENAME"		// メッシュファイル名読み込み
#define LOAD_ITEM			"ITEM_FILENAME"		// アイテムファイル名読み込み
#define LOAD_MOTIONLOOP		"LOOP"				// ループモーションか読み込み
#define LOAD_NUMKEY			"NUM_KEY"			// キー数読み込み
#define LOAD_KEYSET			"KEYSET"			// キー情報読み込み
#define LOAD_ENDKEYSET		"END_KEYSET"		// キー情報読み込み終了
#define LOAD_FRAME			"FRAME"				// フレーム数読み込み
#define LOAD_KEY			"KEY"				// キー読み込み
#define LOAD_ENDKEY			"END_KEY"			// キー読み込み終了
#define LOAD_MODE			"MODE"				// モード読み込み
#define LOAD_POS			"POS"				// 位置読み込み
#define LOAD_ROT			"ROT"				// 向き読み込み
#define LOAD_TYPE			"TYPE"				// 種類読み込み
#define LOAD_MESHTYPE		"MESHTYPE"			// メッシュの種類読み込み
#define LOAD_EVENT			"EVENT"				// イベント読み込み
#define LOAD_RADIUS			"RADIUS"			// 半径読み込み
#define LOAD_COLLISION		"COLLISION"			// 当たり判定有無読み込み
#define LOAD_INDEX			"INDEX"				// インデックス読み込み
#define LOAD_PARENT			"PARENT"			// 親インデックス読み込み
#define LOAD_MODELINFO		"MODELSET"			// モデル情報読み込み
#define LOAD_ENDMODELINFO	"END_MODELSET"		// モデル情報読み込み終了
#define LOAD_ITEMINFO		"ITEMSET"			// アイテム情報読み込み
#define LOAD_ENDITEMINFO	"END_ITEMSET"		// アイテム情報読み込み終了
#define LOAD_FRIENDS		"FRIENDSET"			// 仲間の情報読み込み
#define LOAD_ENDFRIENDS		"END_FRIENDSET"		// 仲間の情報読み込み終了
#define LOAD_RESULTINFO		"RESULTINFO"		// リザルトの仲間の情報読み込み
#define LOAD_ENDRESULTINFO	"END_RESULTINFO"	// リザルトの仲間の情報読み込み終了

//*****************************************************************************
// グローバル変数
//*****************************************************************************
int g_nNumModel;								// 読み込んだモデル数
int g_nNumMotion;								// 読み込んだモーション数
int g_nNumParentModel;							// 読み込んだ階層構造モデル数

//=============================================================================
//	スクリプトの読み込み処理
//=============================================================================
HRESULT LoadScript(const char* pScriptFileName)
{
	// 受け取ったパスからファイルを開く
	FILE* pScriptFile = fopen(pScriptFileName, "r");

	if (pScriptFile == NULL)
	{// スクリプト読み込み失敗
		return E_FAIL;
	}

	char aStr[MAX_STRING] = {};				// 文字列格納
	char aScriptPath[FILENAME_MAX] = {};	// スクリプトファイルパス
	char aTexPath[FILENAME_MAX] = {};		// テクスチャファイルパス
	char* pStart = NULL;					// 文字列の開始位置
	int type = NULL;						// 種類
	g_nNumModel = 0;						// モデル数初期化
	g_nNumMotion = 0;						// モーション数初期化
	g_nNumParentModel = 0;					// 階層構造モデル数初期化

	while (true)
	{
		memset(aStr, NULL, sizeof(aStr));			// 文字列初期化
		fgets(aStr, sizeof(aStr), pScriptFile);		// 一行読み取り

		if (strstr(aStr, LOAD_START) != NULL)
		{// 読み込み開始
			break;
		}

		if (feof(pScriptFile) != NULL)
		{// 読み込み失敗
			return E_FAIL;
		}
	}

	while (true)
	{
		memset(aStr, NULL, sizeof(aStr));					// 文字列初期化
		(void)fgets(aStr, sizeof(aStr), pScriptFile);		// 一行読み取り

		if (strstr(aStr, LOAD_MOTION) != NULL)
		{// MOTIONSCRIPTを読み込んだ
			pStart = strchr(aStr, '=');

			(void)sscanf(pStart + 1, "%s %d", &aScriptPath, &type);

			LoadMotionInfo(aScriptPath);
		}

		if (strstr(aStr, LOAD_OBJECT) != NULL)
		{// OBJECTSCRIPTを読み込んだ
			pStart = strchr(aStr, '=');

			(void)sscanf(pStart + 1, "%s %d", &aScriptPath, &type);

			LoadObject(aScriptPath);
		}

		if (strstr(aStr, LOAD_PARENTMODEL) != NULL)
		{// PARENTSCRIPTを読み込んだ
			pStart = strchr(aStr, '=');

			(void)sscanf(pStart + 1, "%s %d", &aScriptPath, &type);

			LoadParentModel(aScriptPath);
		}

		if (strstr(aStr, LOAD_END))
		{// END_SCRIPTを読み込んだ
			fclose(pScriptFile);

			break;
		}
	}

	return S_OK;
}

//=============================================================================
//	階層構造モデル読み込み処理
//=============================================================================
HRESULT LoadParentModel(const char* pParentModelFileName)
{
	FILE* pParentModelFile = fopen(pParentModelFileName, "r");

	if (pParentModelFile == NULL)
	{// 読み込み失敗
		return E_FAIL;
	}

	char aStr[MAX_STRING] = {};			   // 文字列読み込み
	char aStrCpy[MAX_STRING] = {};		   // 文字列複製(整理)
	char* pStart = NULL;				   // 文字列開始位置
	char aModelPath[FILENAME_MAX] = {};	   // モデルのファイル名読み込み
	int nIdx = 0;						   // モデルのインデックス読み込み
	int nParent = 0;					   // モデルの親インデックス読み込み
	D3DXVECTOR3 pos = {};				   // 位置読み込み
	D3DXVECTOR3 rot = {};				   // 向き読み込み
	int nNumModel = 0;					   // モデル数読み込み
	int nCntParts = 0;					   // パーツ数カウント

	while (true)
	{
		memset(aStr, NULL, sizeof(aStr));					// 文字列クリア
		(void)fgets(aStr, sizeof(aStr), pParentModelFile);	// 一列読み込み

		if (strstr(aStr, LOAD_START) != NULL)
		{// LOAD_STARTを読み込めば読み込み開始
			break;
		}

		if (feof(pParentModelFile) != NULL)
		{// 読み込み失敗
			return E_FAIL;
		}
	}

	while (true)
	{
		memset(aStr, NULL, sizeof(aStr));				// 文字列クリア
		memset(aStrCpy, NULL, sizeof(aStrCpy));			// コピーもクリア
		(void)fgets(aStr, sizeof(aStr), pParentModelFile);	// 一列読み込み
		LoadEnableString(&aStrCpy[0], &aStr[0]);		// 有効文字だけ抜き取って複製

		if (strstr(aStr, LOAD_NUMMODEL))
		{// NUM_MODELを読み込んだ
			if ((pStart = strchr(aStr, '=')) == NULL)
			{
				continue;
			}

			(void)sscanf(pStart + 1, "%d", &nNumModel);
		}

		if (strstr(aStr, LOAD_MODEL))
		{// MODEL_FILENAMEを読み込んだ
			if ((pStart = strchr(aStr, '=')) == NULL)
			{
				continue;
			}

			(void)sscanf(pStart + 1, "%s", &aModelPath);

			LoadParentModel(aModelPath, g_nNumParentModel);
		}

		if (strstr(aStr, LOAD_PLAYER))
		{// CHARACTERSETを読み込んだ
			while (true)
			{
				memset(aStr, NULL, sizeof(aStr));
				memset(aStrCpy, NULL, sizeof(aStrCpy));
				(void)fgets(aStr, sizeof(aStr), pParentModelFile);
				LoadEnableString(&aStrCpy[0], &aStr[0]);

				if (strcmp(aStrCpy, LOAD_PARTS) == 0)
				{// PARTSSETを読み込んだ
					while (true)
					{
						memset(aStr, NULL, sizeof(aStr));
						memset(aStrCpy, NULL, sizeof(aStrCpy));
						(void)fgets(aStr, sizeof(aStr), pParentModelFile);
						LoadEnableString(&aStrCpy[0], &aStr[0]);

						if (strstr(aStr, LOAD_INDEX))
						{// INDEXを読み込んだ
							if ((pStart = strchr(aStr, '=')) == NULL)
							{
								continue;
							}

							(void)sscanf(pStart + 1, "%d", &nIdx);

							continue;
						}

						if (strstr(aStr, LOAD_PARENT))
						{// PARENTを読み込んだ
							if ((pStart = strchr(aStr, '=')) == NULL)
							{
								continue;
							}

							(void)sscanf(pStart + 1, "%d", &nParent);

							continue;
						}

						if (strstr(aStr, LOAD_POS))
						{// POSを読み込んだ
							if ((pStart = strchr(aStr, '=')) == NULL)
							{
								continue;
							}

							(void)sscanf(pStart + 1, "%f %f %f", &pos.x, &pos.y, &pos.z);

							continue;
						}

						if (strstr(aStr, LOAD_ROT))
						{// ROTを読み込んだ
							if ((pStart = strchr(aStr, '=')) == NULL)
							{
								continue;
							}

							(void)sscanf(pStart + 1, "%f %f %f", &rot.x, &rot.y, &rot.z);

							continue;
						}

						if (strcmp(aStrCpy, LOAD_ENDPARTS) == 0)
						{// END_PARTSSETを読み込んだ

							LoadParentModelOffSet(pos, rot, nIdx, nParent, g_nNumParentModel);

							memset(&pos, NULL, sizeof(D3DXVECTOR3));
							memset(&rot, NULL, sizeof(D3DXVECTOR3));
							int nIdx = 0;
							int nParet = 0;
							break;
						}
					}
				}

				if (strcmp(aStrCpy, LOAD_ENDPLAYER) == 0)
				{// END_CHARACTERSETを読み込んだ
					break;
				}
			}
		}

		if (strcmp(aStrCpy, LOAD_END) == 0)
		{// END_SCRIPTを読み込んだ
			// 読み込み終了
			fclose(pParentModelFile);
			g_nNumParentModel++;
			break;
		}
	}
}

//=============================================================================
//	モーション情報読み込み処理
//=============================================================================
HRESULT LoadMotionInfo(const char* pMotionFileName)
{
	FILE* pMotionFile = fopen(pMotionFileName, "r");

	if (pMotionFile == NULL)
	{// 読み込み失敗
		return E_FAIL;
	}

	char aStr[MAX_STRING] = {};			   // 文字列読み込み
	char aStrCpy[MAX_STRING] = {};		   // 文字列複製(整理)
	char* pStart = NULL;				   // 文字列開始位置
	char aModelPath[FILENAME_MAX] = {};	   // モデルのファイル名読み込み
	int nIdx = 0;						   // モデルのインデックス読み込み
	int nParent = 0;					   // モデルの親インデックス読み込み
	D3DXVECTOR3 pos = {};				   // 位置読み込み
	D3DXVECTOR3 rot = {};				   // 向き読み込み
	int nNumModel = 0;					   // モデル数読み込み
	int nCntMotion = 0;					   // モーション数カウント
	int bLoop = false;					   // ループするかどうか読み込み
	int nNumKey = 0;					   // キー数読み込み
	KEY_INFO KeyInfo[MAX_KEY] = {};		   // キー情報読み込み
	int nCntKey = 0;					   // キー数カウント
	int nCntParts = 0;					   // パーツ数カウント

	while (true)
	{
		memset(aStr, NULL, sizeof(aStr));				// 文字列クリア
		(void)fgets(aStr, sizeof(aStr), pMotionFile);	// 一列読み込み

		if (strstr(aStr, LOAD_START) != NULL)
		{// LOAD_STARTを読み込めば読み込み開始
			break;
		}

		if (feof(pMotionFile) != NULL)
		{// 読み込み失敗
			return E_FAIL;
		}
	}

	while (true)
	{
		memset(aStr, NULL, sizeof(aStr));
		memset(aStrCpy, NULL, sizeof(aStrCpy));
		(void)fgets(aStr, sizeof(aStr), pMotionFile);
		LoadEnableString(&aStrCpy[0], &aStr[0]);

		if (strcmp(aStrCpy, LOAD_MOTIONINFO) == 0)
		{// MOTIONSETを読み込んだ
			while (true)
			{
				memset(aStr, NULL, sizeof(aStr));
				memset(aStrCpy, NULL, sizeof(aStrCpy));
				(void)fgets(aStr, sizeof(aStr), pMotionFile);
				LoadEnableString(&aStrCpy[0], &aStr[0]);

				if (strstr(aStr, LOAD_MOTIONLOOP))
				{// LOOPを読み込んだ
					if ((pStart = strchr(aStr, '=')) == NULL)
					{
						continue;
					}

					(void)sscanf(pStart + 1, "%d", &bLoop);

					continue;
				}

				if (strstr(aStr, LOAD_NUMKEY))
				{// NUM_KEYを読み込んだ
					if ((pStart = strchr(aStr, '=')) == NULL)
					{
						continue;
					}

					(void)sscanf(pStart + 1, "%d", &nNumKey);

					continue;
				}

				if (strcmp(aStrCpy, LOAD_KEYSET) == 0)
				{// KEYSETを読み込んだ
					while (true)
					{
						memset(aStr, NULL, sizeof(aStr));
						memset(aStrCpy, NULL, sizeof(aStrCpy));
						(void)fgets(aStr, sizeof(aStr), pMotionFile);
						LoadEnableString(&aStrCpy[0], &aStr[0]);

						if (strstr(aStr, LOAD_FRAME))
						{// FRAMEを読み込んだ
							if ((pStart = strchr(aStr, '=')) == NULL)
							{
								continue;
							}

							(void)sscanf(pStart + 1, "%d", &KeyInfo[nCntKey].nFrame);

							continue;
						}

						if (strcmp(aStrCpy, LOAD_KEY) == 0)
						{// KEYを読み込んだ
							while (true)
							{
								memset(aStr, NULL, sizeof(aStr));
								memset(aStrCpy, NULL, sizeof(aStrCpy));
								(void)fgets(aStr, sizeof(aStr), pMotionFile);
								LoadEnableString(&aStrCpy[0], &aStr[0]);

								if (strstr(aStr, LOAD_POS))
								{// POSを読み込んだ
									if ((pStart = strchr(aStr, '=')) == NULL)
									{
										continue;
									}

									(void)sscanf(pStart + 1, "%f %f %f", &KeyInfo[nCntKey].aKey[nCntParts].fPosX, &KeyInfo[nCntKey].aKey[nCntParts].fPosY, &KeyInfo[nCntKey].aKey[nCntParts].fPosZ);

									continue;
								}

								if (strstr(aStr, LOAD_ROT))
								{// ROTを読み込んだ
									if ((pStart = strchr(aStr, '=')) == NULL)
									{
										continue;
									}

									(void)sscanf(pStart + 1, "%f %f %f", &KeyInfo[nCntKey].aKey[nCntParts].fRotX, &KeyInfo[nCntKey].aKey[nCntParts].fRotY, &KeyInfo[nCntKey].aKey[nCntParts].fRotZ);

									continue;
								}

								if (strcmp(aStrCpy, LOAD_ENDKEY) == 0)
								{// END_KEYを読み込んだ
									nCntParts++;
									break;
								}
							}
						}

						if (strcmp(aStrCpy, LOAD_ENDKEYSET) == 0)
						{// END_KEYSET
							nCntKey++;
							nCntParts = 0;
							break;
						}
					}
				}

				if (strcmp(aStrCpy, LOAD_ENDMOTIONINFO) == 0)
				{// END_MOTIONSETを読み込んだ

					LoadMotion(bLoop, nNumKey, &KeyInfo[0], nCntMotion, g_nNumMotion);

					nCntMotion++;

					// 各種変数初期化
					memset(&KeyInfo, NULL, sizeof(KEY_INFO));
					bLoop = false;
					nNumKey = 0;
					nCntKey = 0;

					break;
				}

			}
		}

		if (strcmp(aStrCpy, LOAD_END) == 0)
		{// END_SCRIPTを読み込んだ
			// 読み込み終了
			fclose(pMotionFile);
			g_nNumMotion++;
			break;
		}
	}

	return S_OK;
}

//=============================================================================
//	ステージ情報読み込み処理
//=============================================================================
HRESULT LoadModel(const char* pModelFileName)
{
	FILE* pStageFile = fopen(pModelFileName, "r");

	if (pStageFile == NULL)
	{// 読み込み失敗
		return E_FAIL;
	}

	char aStr[MAX_STRING] = {};			   // 文字列読み込み
	char aStrCpy[MAX_STRING] = {};		   // 文字列複製(整理)
	char* pStart = NULL;				   // 文字列開始位置
	char aModelPath[FILENAME_MAX] = {};	   // モデルのファイル名読み込み
	char aMeshPath[FILENAME_MAX] = {};	   // メッシュのファイル名読み込み
	bool bSetMesh = false;				   // メッシュを作るかどうか
	int nIdx = 0;						   // モデルのインデックス読み込み
	int nParent = 0;					   // モデルの親インデックス読み込み
	D3DXVECTOR3 pos = {};				   // 位置読み込み
	D3DXVECTOR3 rot = {};				   // 向き読み込み
	int type = -1;						   // 種類読み込み
	int nShadow = 0;					   // 影をつけるか
	int  nCollision = true;				   // 当たり判定するか
	int nNumModel = 0;					   // モデル数読み込み

	while (true)
	{
		memset(aStr, NULL, sizeof(aStr));				// 文字列クリア
		(void)fgets(aStr, sizeof(aStr), pStageFile);	// 一列読み込み

		if (strstr(aStr, LOAD_START) != NULL)
		{// LOAD_STARTを読み込めば読み込み開始
			break;
		}

		if (feof(pStageFile) != NULL)
		{// 読み込み失敗
			return E_FAIL;
		}
	}

	while (true)
	{
		memset(aStr, NULL, sizeof(aStr));				// 文字列クリア
		memset(aStrCpy, NULL, sizeof(aStrCpy));			// コピーもクリア
		(void)fgets(aStr, sizeof(aStr), pStageFile);	// 一列読み込み
		LoadEnableString(&aStrCpy[0], &aStr[0]);		// 有効文字だけ抜き取って複製

		if (strstr(aStr, LOAD_NUMMODEL))
		{// NUM_MODELを読み込んだ
			if ((pStart = strchr(aStr, '=')) == NULL)
			{
				continue;
			}

			(void)sscanf(pStart + 1, "%d", &nNumModel);
		}

		if (strstr(aStr, LOAD_MODEL))
		{// MODEL_FILENAMEを読み込んだ
			if ((pStart = strchr(aStr, '=')) == NULL)
			{
				continue;
			}

			(void)sscanf(pStart + 1, "%s", &aModelPath);

			LoadObjectModel(aModelPath);
		}

		if (strcmp(aStrCpy, LOAD_MODELINFO) == 0)
		{
			while (true)
			{
				memset(aStr, NULL, sizeof(aStr));				// 文字列クリア
				memset(aStrCpy, NULL, sizeof(aStrCpy));			// コピーもクリア
				(void)fgets(aStr, sizeof(aStr), pStageFile);	// 一列読み込み
				LoadEnableString(&aStrCpy[0], &aStr[0]);		// 有効文字だけ抜き取って複製

				if (strstr(aStr, LOAD_POS))
				{// POSを読み込んだ
					if ((pStart = strchr(aStr, '=')) == NULL)
					{
						continue;
					}

					(void)sscanf(pStart + 1, "%f %f %f", &pos.x, &pos.y, &pos.z);

					continue;
				}

				if (strstr(aStr, LOAD_ROT))
				{// ROTを読み込んだ
					if ((pStart = strchr(aStr, '=')) == NULL)
					{
						continue;
					}

					(void)sscanf(pStart + 1, "%f %f %f", &rot.x, &rot.y, &rot.z);

					continue;
				}

				if (strncmp(aStrCpy, LOAD_TYPE, 4) == 0)
				{// TYPEを読み込んだ
					if ((pStart = strchr(aStr, '=')) == NULL)
					{
						continue;
					}

					(void)sscanf(pStart + 1, "%d", &type);

					continue;
				}

				if (strstr(aStr, LOAD_COLLISION))
				{// COLLISIONを読み込んだ
					if ((pStart = strchr(aStr, '=')) == NULL)
					{
						continue;
					}

					(void)sscanf(pStart + 1, "%d", &nCollision);

					continue;
				}

				if (strcmp(aStrCpy, LOAD_ENDMODELINFO) == 0)
				{// END_MODELSETを読み込んだ

					SetObject((OBJECTTYPE)type, pos, rot, (bool)nShadow, (bool)nCollision);
					
					memset(aMeshPath, NULL, sizeof(aMeshPath));				// 文字列クリア
					bSetMesh = false;
					break;
				}
			}
		}

		if (strcmp(aStrCpy, LOAD_END) == 0)
		{// END_SCRIPTを読み込んだ
			fclose(pStageFile);

			break;
		}
	}

	return S_OK;
}

//=============================================================================
//	オブジェクト情報読み込み処理
//=============================================================================
HRESULT LoadObject(const char* pObjectFileName)
{
	//FILE* pObjectFile = fopen(pObjectFileName, "r");

	//if (pObjectFile == NULL)
	//{// 読み込み失敗
	//	return E_FAIL;
	//}

	//char aStr[MAX_STRING] = {};			   // 文字列読み込み
	//char aStrCpy[MAX_STRING] = {};		   // 文字列複製(整理)
	//char* pStart = NULL;				   // 文字列開始位置
	//char aScriptPath[FILENAME_MAX] = {};   // スクリプトファイルパス
	//char aModelPath[FILENAME_MAX] = {};	   // モデルのファイル名読み込み
	//char aItemPath[FILENAME_MAX] = {};	   // アイテムのファイル名読み込み
	//int nIdx = 0;						   // モデルのインデックス読み込み
	//int nParent = 0;					   // モデルの親インデックス読み込み
	//D3DXVECTOR3 pos = {};				   // 位置読み込み
	//D3DXVECTOR3 rot = {};				   // 向き読み込み
	//int type = NULL;					   // 種類
	//int nCollision = NULL;				   // 当たり判定
	//int event = NULL;					   // イベント
	//int Objecttype = NULL;				   // オブジェクトの種類
	//int nNumModel = 0;					   // モデル数読み込み

	//while (true)
	//{
	//	memset(aStr, NULL, sizeof(aStr));				// 文字列クリア
	//	(void)fgets(aStr, sizeof(aStr), pObjectFile);	// 一列読み込み

	//	if (strstr(aStr, LOAD_START) != NULL)
	//	{// LOAD_STARTを読み込めば読み込み開始
	//		break;
	//	}

	//	if (feof(pObjectFile) != NULL)
	//	{// 読み込み失敗
	//		return E_FAIL;
	//	}
	//}

	//while (true)
	//{
	//	memset(aStr, NULL, sizeof(aStr));				// 文字列クリア
	//	memset(aStrCpy, NULL, sizeof(aStrCpy));			// コピーもクリア
	//	(void)fgets(aStr, sizeof(aStr), pObjectFile);	// 一列読み込み
	//	LoadEnableString(&aStrCpy[0], &aStr[0]);		// 有効文字だけ抜き取って複製

	//	if (strstr(aStr, LOAD_MODEL))
	//	{
	//		if ((pStart = strchr(aStr, '=')) == NULL)
	//		{
	//			continue;
	//		}

	//		(void)sscanf(pStart + 1, "%s", &aModelPath);

	//		LoadModelData(aModelPath);
	//	}

	//	if (strstr(aStr, LOAD_MOTION) != NULL)
	//	{
	//		pStart = strchr(aStr, '=');

	//		(void)sscanf(pStart + 1, "%s %d", &aScriptPath, &Objecttype);

	//		LoadMotionInfo(aScriptPath, (OBJECTTYPE)Objecttype);

	//		memset(aScriptPath, NULL, sizeof(aScriptPath));

	//		NumFriendsAdd();
	//	}

	//	if (strcmp(aStrCpy, LOAD_FRIENDS) == 0)
	//	{
	//		while (true)
	//		{
	//			memset(aStr, NULL, sizeof(aStr));				// 文字列クリア
	//			memset(aStrCpy, NULL, sizeof(aStrCpy));			// コピーもクリア
	//			(void)fgets(aStr, sizeof(aStr), pObjectFile);	// 一列読み込み
	//			LoadEnableString(&aStrCpy[0], &aStr[0]);		// 有効文字だけ抜き取って複製

	//			if (strstr(aStr, LOAD_POS))
	//			{
	//				if ((pStart = strchr(aStr, '=')) == NULL)
	//				{
	//					continue;
	//				}

	//				(void)sscanf(pStart + 1, "%f %f %f", &pos.x, &pos.y, &pos.z);

	//				continue;
	//			}

	//			if (strstr(aStr, LOAD_ROT))
	//			{
	//				if ((pStart = strchr(aStr, '=')) == NULL)
	//				{
	//					continue;
	//				}

	//				(void)sscanf(pStart + 1, "%f %f %f", &rot.x, &rot.y, &rot.z);

	//				continue;
	//			}

	//			if (strstr(aStr, LOAD_TYPE))
	//			{
	//				if ((pStart = strchr(aStr, '=')) == NULL)
	//				{
	//					continue;
	//				}

	//				(void)sscanf(pStart + 1, "%d", &type);

	//				continue;
	//			}

	//			if (strstr(aStr, LOAD_EVENT))
	//			{
	//				if ((pStart = strchr(aStr, '=')) == NULL)
	//				{
	//					continue;
	//				}

	//				(void)sscanf(pStart + 1, "%d", &event);

	//				continue;
	//			}

	//			if (strcmp(aStrCpy, LOAD_ENDFRIENDS) == 0)
	//			{
	//				SetFriends(pos, rot, (FRIENDSTYPE)type, (FRIENDEVENT)event);
	//				break;
	//			}
	//		}
	//	}

	//	if (strstr(aStr, LOAD_ITEM))
	//	{
	//		if ((pStart = strchr(aStr, '=')) == NULL)
	//		{
	//			continue;
	//		}

	//		(void)sscanf(pStart + 1, "%s", &aItemPath);

	//		LoadItemData(aItemPath);
	//	}

	//	if (strncmp(aStrCpy, LOAD_ITEMINFO, sizeof(LOAD_ITEMINFO) - 1) == 0)
	//	{
	//		while (true)
	//		{
	//			memset(aStr, NULL, sizeof(aStr));				// 文字列クリア
	//			memset(aStrCpy, NULL, sizeof(aStrCpy));			// コピーもクリア
	//			(void)fgets(aStr, sizeof(aStr), pObjectFile);	// 一列読み込み
	//			LoadEnableString(&aStrCpy[0], &aStr[0]);		// 有効文字だけ抜き取って複製

	//			if (strstr(aStr, LOAD_POS))
	//			{
	//				if ((pStart = strchr(aStr, '=')) == NULL)
	//				{
	//					continue;
	//				}

	//				(void)sscanf(pStart + 1, "%f %f %f", &pos.x, &pos.y, &pos.z);

	//				continue;
	//			}

	//			if (strstr(aStr, LOAD_ROT))
	//			{
	//				if ((pStart = strchr(aStr, '=')) == NULL)
	//				{
	//					continue;
	//				}

	//				(void)sscanf(pStart + 1, "%f %f %f", &rot.x, &rot.y, &rot.z);

	//				continue;
	//			}

	//			if (strstr(aStr, LOAD_TYPE))
	//			{
	//				if ((pStart = strchr(aStr, '=')) == NULL)
	//				{
	//					continue;
	//				}

	//				(void)sscanf(pStart + 1, "%d", &type);

	//				continue;
	//			}

	//			if (strstr(aStr, LOAD_COLLISION))
	//			{
	//				if ((pStart = strchr(aStr, '=')) == NULL)
	//				{
	//					continue;
	//				}

	//				(void)sscanf(pStart + 1, "%d", &nCollision);

	//				continue;
	//			}

	//			if (strcmp(aStrCpy, LOAD_ENDITEMINFO) == 0)
	//			{
	//				SetItem(pos, rot, (ITEMTYPE)type, (bool)nCollision);
	//				break;
	//			}
	//		}
	//	}

	//	if (strcmp(aStrCpy, LOAD_RESULTINFO) == 0)
	//	{
	//		while (true)
	//		{
	//			memset(aStr, NULL, sizeof(aStr));				// 文字列クリア
	//			memset(aStrCpy, NULL, sizeof(aStrCpy));			// コピーもクリア
	//			(void)fgets(aStr, sizeof(aStr), pObjectFile);	// 一列読み込み
	//			LoadEnableString(&aStrCpy[0], &aStr[0]);		// 有効文字だけ抜き取って複製

	//			if (strstr(aStr, LOAD_POS))
	//			{
	//				if ((pStart = strchr(aStr, '=')) == NULL)
	//				{
	//					continue;
	//				}

	//				(void)sscanf(pStart + 1, "%f %f %f", &pos.x, &pos.y, &pos.z);

	//				continue;
	//			}

	//			if (strstr(aStr, LOAD_ROT))
	//			{
	//				if ((pStart = strchr(aStr, '=')) == NULL)
	//				{
	//					continue;
	//				}

	//				(void)sscanf(pStart + 1, "%f %f %f", &rot.x, &rot.y, &rot.z);

	//				continue;
	//			}

	//			if (strcmp(aStrCpy, LOAD_ENDRESULTINFO) == 0)
	//			{
	//				SetResultFriendsInfo(pos, rot);
	//				break;
	//			}
	//		}
	//	}

	//	if (strcmp(aStrCpy, LOAD_END) == 0)
	//	{
	//		fclose(pObjectFile);

	//		break;
	//	}
	//}

	return S_OK;
}

//=============================================================================
//	有効文字抜き出し処理
//=============================================================================
void LoadEnableString(char* aStrCpy, char* pStart)
{
	while (true)
	{
		if (*pStart == '\n')
		{
			pStart++;
			continue;
		}
		else if (*pStart == '\t')
		{
			pStart++;
			continue;
		}
		else if (*pStart == 0x32)
		{
			pStart++;
			continue;
		}
		else if (*pStart == '#' || *pStart == '\0')
		{
			break;
		}
		else
		{
			strncat(aStrCpy, pStart, 1);
			pStart++;
		}
	}
}