#include "vibration.h"
#include "magic.h"
#include "input.h"
#include "debugproc.h"

VIBRATIONTYPE g_VibrationType;
VIBRATION g_Vibration;
int nVibration = 0;
int nVibDel = 0;
int nCntVibration;
bool Left = true;
bool Right = false;
bool Up = true;
bool Down = false;

void InitVibration(void)
{

	g_Vibration.Vibration = false;
}

void UninitVibration(void)
{

}

void UpdateVibration(void)
{
	static WORD wLeftMotorSpeed, wRightMotorSpeed;
	static int nVibrationCounter = 0;

	PrintDebugProc("右モーター : %d\n左モーター : %d\n", wLeftMotorSpeed, wRightMotorSpeed);
	PrintDebugProc("持続フレーム数 : %d\n", nVibrationCounter);

	if (GetKeyboardTrigger(DIK_TAB) == true)
	{// TABキーを押した
		// 振動開始
		SetJoypadVibration(wLeftMotorSpeed, wRightMotorSpeed, nVibrationCounter, 0);
	}

	if (GetKeyboardRepeat(DIK_F1) == true && GetKeyboardPress(DIK_LCONTROL) == true)
	{// F1キーとLCTRLを押した
		// 左モータースピードを下げる
		wLeftMotorSpeed += -1000;
	}
	else if (GetKeyboardRepeat(DIK_F1) == true)
	{// F1キーだけ押した
		// 左モータースピードをあげる
		wLeftMotorSpeed += 1000;
	}

	if (GetKeyboardRepeat(DIK_F2) == true && GetKeyboardPress(DIK_LCONTROL) == true)
	{// F2キーとLCTRLを押した
		// 右モータースピードを下げる
		wRightMotorSpeed += -1000;
	}
	else if (GetKeyboardRepeat(DIK_F2) == true)
	{// F2キーだけ押した
		// 右モータースピードをあげる
		wRightMotorSpeed += 1000;
	}

	if (GetKeyboardRepeat(DIK_F3) == true && GetKeyboardPress(DIK_LCONTROL) == true)
	{// F3キーとLCTRLを押した
		// 振動持続フレームを下げる
		nVibrationCounter--;
	}
	else if (GetKeyboardRepeat(DIK_F3) == true)
	{// F3キーだけ押した
		// 振動持続フレームをあげる
		nVibrationCounter++;
	}

}
void VibrationType(VIBRATIONTYPE Type, int nDropMagic, int nPlayertype)
{
	int nVibForce = 12000;

	// 画面遷移時に振動を消す
	if (nDropMagic == VIBRATION_CLEAR)
	{
		SetJoypadVibration(0, 0, 1, nPlayertype);
		nVibration = 0;
		g_Vibration.Vibration = true;
	}

	if (g_Vibration.Vibration == false)
	{
		if (Type == VIBRATIONTYPE_NOTHING)
		{
			SetJoypadVibration(0, 0, 1, nPlayertype);
		}
		else
		{
			if (nDropMagic == 0)
			{ // 浮遊

				if (Type == VIBRATIONTYPE_FAR)
				{
					if (nVibration >= 6000 && Up == true)
					{
						Up = false;
						Down = true;
					}
					else if (nVibration <= 120 && Down == true)
					{
						Up = true;
						Down = false;
					}

					if (Down == true)
					{
						nVibration = nVibration - 300;

					}

					else if (Up == true)
					{
						nVibration = nVibration + 300;

					}


					SetJoypadVibration(300 + nVibration, 300 + nVibration, 60, nPlayertype);
				}
				else if (Type == VIBRATIONTYPE_MEDIUM)
				{
					if (nVibration >= 15000 && Up == true)
					{
						Up = false;
						Down = true;
					}
					else if (nVibration <= 180 && Down == true)
					{
						Up = true;
						Down = false;
					}

					if (Down == true)
					{
						nVibration = nVibration - 300;

					}

					else if (Up == true)
					{
						nVibration = nVibration + 300;

					}


					SetJoypadVibration(300 + nVibration, 300 + nVibration, 60, nPlayertype);
				}
				else if (Type == VIBRATIONTYPE_CLOSE)
				{
					if (nVibration >= 24000 && Up == true)
					{
						Up = false;
						Down = true;
					}
					else if (nVibration <= 270 && Down == true)
					{
						Up = true;
						Down = false;
					}

					if (Down == true)
					{
						nVibration = nVibration - 300;

					}
		
					else if (Up == true)
					{
						nVibration = nVibration + 300;

					}
		

					SetJoypadVibration(300 + nVibration, 300 + nVibration, 60, nPlayertype);
				}

			}
			else if (nDropMagic == 1)
			{ // 燃焼

				nVibration++;

				if (nVibration % 60 < 15)
				{
					SetJoypadVibration(30000 - (nVibForce * Type), 0, 60, nPlayertype);
				}
				else if (nVibration % 60 >= 15 && nVibration % 60 < 30 || nVibration % 60 >= 45 && nVibration % 60 < 60)
				{
					SetJoypadVibration(0, 0, 1, nPlayertype);
				}
				else if (nVibration % 60 >= 30)
				{
					SetJoypadVibration(0, 30000 - (nVibForce * Type), 60, nPlayertype);
				}
		

				if (nVibration >= 120)
				{
					nVibration = 0;
				}
			}
			else if (nDropMagic == 2)
			{ // 洪水

				nVibration = nVibration + 100;

				if (Type == VIBRATIONTYPE_FAR)
				{
					if (nVibration >= 10000)
					{
						nVibDel++;
					}

					if (nVibDel >= 100)
					{
						nVibration = 0;
						nVibDel = 0;
					}
					else if (nVibDel == 0)
					{
						SetJoypadVibration(0 + nVibration, 10000 - nVibration, 60, nPlayertype);
					}
				}
				else if (Type == VIBRATIONTYPE_MEDIUM)
				{
					if (nVibration >= 20000)
					{
						nVibDel++;
					}

					if (nVibDel >= 100)
					{
						nVibration = 0;
						nVibDel = 0;
					}
					else if (nVibDel == 0)
					{
						SetJoypadVibration(0 + nVibration, 20000 - nVibration, 60, nPlayertype);
					}
				}
				else if (Type == VIBRATIONTYPE_CLOSE)
				{
					if (nVibration >= 30000)
					{
						nVibDel++;
					}

					if (nVibDel >= 100)
					{
						nVibration = 0;
						nVibDel = 0;
					}
					else if (nVibDel == 0)
					{
						SetJoypadVibration(0 + nVibration, 30000 - nVibration, 60, nPlayertype);
					}
				}



			}
			else if (nDropMagic == 3)
			{ // フラッシュ

				nVibration++;

				if (nVibration % 60 < 30)
				{
					SetJoypadVibration(30000 - (nVibForce * Type), 30000 - (nVibForce * Type), 60, nPlayertype);
				}
				else if (nVibration % 60 >= 30)
				{
					SetJoypadVibration(0, 0, 1, nPlayertype);
				}

				if (nVibration >= 120)
				{
					nVibration = 0;
				}
			}
			else if (nDropMagic == 4)
			{ // 火球

				if (Type == VIBRATIONTYPE_FAR)
				{
					nVibration = nVibration + 350;

					if (nCntVibration == 0)
					{
						SetJoypadVibration(0, 0 + nVibration, 60, nPlayertype);

					}
					else if (nCntVibration == 1)
					{
						SetJoypadVibration(0 + nVibration, 10000 - nVibration, 60, nPlayertype);

					}
					else if (nCntVibration == 2)
					{
						SetJoypadVibration(10000 - nVibration, 0, 60, nPlayertype);

					}
					else if (nCntVibration == 3)
					{
						nCntVibration = 0;
					}

					if (nVibration >= 10000)
					{
						nCntVibration++;
						nVibration = 0;
					}
				}
				else if (Type == VIBRATIONTYPE_MEDIUM)
				{
					nVibration = nVibration + 650;

					if (nCntVibration == 0)
					{
						SetJoypadVibration(0, 0 + nVibration, 60, nPlayertype);

					}
					else if (nCntVibration == 1)
					{
						SetJoypadVibration(0 + nVibration, 20000 - nVibration, 60, nPlayertype);

					}
					else if (nCntVibration == 2)
					{
						SetJoypadVibration(20000 - nVibration, 0, 60, nPlayertype);

					}
					else if (nCntVibration == 3)
					{
						nCntVibration = 0;
					}

					if (nVibration >= 20000)
					{
						nCntVibration++;
						nVibration = 0;
					}
				}
				else if (Type == VIBRATIONTYPE_CLOSE)
				{
					nVibration = nVibration + 1000;

					if (nCntVibration == 0)
					{
						SetJoypadVibration(0, 0 + nVibration, 60, nPlayertype);

					}
					else if (nCntVibration == 1)
					{
						SetJoypadVibration(0 + nVibration, 30000 - nVibration, 60, nPlayertype);

					}
					else if (nCntVibration == 2)
					{
						SetJoypadVibration(30000 - nVibration, 0, 60, nPlayertype);

					}
					else if (nCntVibration == 3)
					{
						nCntVibration = 0;
					}

					if (nVibration >= 30000)
					{
						nCntVibration++;
						nVibration = 0;
					}
				}

			}
			else if (nDropMagic == 5)
			{ // 遅延

				nVibration++;

				if (nVibration % 60 < 15)
				{
					SetJoypadVibration(30000 - (nVibForce * Type), 30000 - (nVibForce * Type), 60, nPlayertype);
				}
				else if (nVibration % 60 >= 15)
				{
					SetJoypadVibration(0, 0, 1, nPlayertype);
				}

				if (nVibration >= 120)
				{
					nVibration = 0;
				}
			}
			else if (nDropMagic == 6)
			{ // 雨乞い
				
				if (Type == VIBRATIONTYPE_FAR)
				{
					if (nCntVibration == 0)
					{

						SetJoypadVibration(0 + nVibration, 0 + nVibration, 60, nPlayertype);

					}
					else if (nCntVibration == 1)
					{
						SetJoypadVibration(10000 - nVibration, 10000 - nVibration, 60, nPlayertype);

					}
					else if (nCntVibration == 2)
					{
						nVibDel = 100;
						nCntVibration = 0;
					}

					if (nVibDel <= 0)
					{
						nVibration = nVibration + 100;
					}
					else
					{
						nVibDel--;

					}

					if (nVibration >= 10000)
					{
						nCntVibration++;
						nVibration = 0;

					}
				}
				else if (Type == VIBRATIONTYPE_MEDIUM)
				{
					if (nCntVibration == 0)
					{

						SetJoypadVibration(0 + nVibration, 0 + nVibration, 60, nPlayertype);

					}
					else if (nCntVibration == 1)
					{
						SetJoypadVibration(20000 - nVibration, 20000 - nVibration, 60, nPlayertype);

					}
					else if (nCntVibration == 2)
					{
						nVibDel = 100;
						nCntVibration = 0;
					}

					if (nVibDel <= 0)
					{
						nVibration = nVibration + 200;
					}
					else
					{
						nVibDel--;

					}

					if (nVibration >= 20000)
					{
						nCntVibration++;
						nVibration = 0;

					}
				}
				else if (Type == VIBRATIONTYPE_CLOSE)
				{
					if (nCntVibration == 0)
					{

						SetJoypadVibration(0 + nVibration, 0 + nVibration, 60, nPlayertype);

					}
					else if (nCntVibration == 1)
					{
						SetJoypadVibration(30000 - nVibration, 30000 - nVibration, 60, nPlayertype);

					}
					else if (nCntVibration == 2)
					{
						nVibDel = 100;
						nCntVibration = 0;
					}

					if (nVibDel <= 0)
					{
						nVibration = nVibration + 300;
					}
					else
					{
						nVibDel--;

					}

					if (nVibration >= 30000)
					{
						nCntVibration++;
						nVibration = 0;

					}
				}


			}
			else if (nDropMagic == 7)
			{ // 凍結


				if (Type == VIBRATIONTYPE_FAR)
				{
					nVibration = nVibration + 35;

					if (nVibration >= 10000)
					{
						nVibDel++;
					}

					if (nVibDel >= 100)
					{
						nVibration = 0;
						nVibDel = 0;
					}
					else if (nVibDel == 0)
					{
						SetJoypadVibration(0 + nVibration, 10000, 60, nPlayertype);
					}
				}
				else if (Type == VIBRATIONTYPE_MEDIUM)
				{
					nVibration = nVibration + 65;

					if (nVibration >= 20000)
					{
						nVibDel++;
					}

					if (nVibDel >= 100)
					{
						nVibration = 0;
						nVibDel = 0;
					}
					else if (nVibDel == 0)
					{
						SetJoypadVibration(0 + nVibration, 20000, 60, nPlayertype);
					}
				}
				else if (Type == VIBRATIONTYPE_CLOSE)
				{
					nVibration = nVibration + 100;

					if (nVibration >= 30000)
					{
						nVibDel++;
					}

					if (nVibDel >= 100)
					{
						nVibration = 0;
						nVibDel = 0;
					}
					else if (nVibDel == 0)
					{
						SetJoypadVibration(0 + nVibration, 30000, 60, nPlayertype);
					}
				}


			}
			else if (nDropMagic == 8)
			{ // 成長

				if (Type == VIBRATIONTYPE_FAR)
				{
					nVibration = nVibration + 35;

					if (nVibration >= 10000)
					{
						nVibDel++;
					}

					if (nVibDel >= 100)
					{
						nVibration = 0;
						nVibDel = 0;
					}
					else if (nVibDel == 0)
					{
						SetJoypadVibration(0 + nVibration, 0 + nVibration, 60, nPlayertype);
					}
				}
				else if (Type == VIBRATIONTYPE_MEDIUM)
				{
					nVibration = nVibration + 65;

					if (nVibration >= 20000)
					{
						nVibDel++;
					}

					if (nVibDel >= 100)
					{
						nVibration = 0;
						nVibDel = 0;
					}
					else if (nVibDel == 0)
					{
						SetJoypadVibration(0 + nVibration, 0 + nVibration, 60, nPlayertype);
					}
				}
				else if (Type == VIBRATIONTYPE_CLOSE)
				{
					nVibration = nVibration + 100;

					if (nVibration >= 30000)
					{
						nVibDel++;
					}

					if (nVibDel >= 100)
					{
						nVibration = 0;
						nVibDel = 0;
					}
					else if (nVibDel == 0)
					{
						SetJoypadVibration(0 + nVibration, 0 + nVibration, 60, nPlayertype);
					}
				}


			}
			else if (nDropMagic == 9)
			{ // 加速

				SetJoypadVibration(30000 - (nVibForce * Type), 30000 - (nVibForce * Type), 60, nPlayertype);

			}
		}

	}


}
VIBRATION* GetVibration(void)
{
	return &g_Vibration;
}