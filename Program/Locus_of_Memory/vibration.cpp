#include "vibration.h"
#include "magic.h"
#include "input.h"
#include "debugproc.h"

VIBRATIONTYPE g_VibrationType;
VIBRATION g_Vibration;
int nVibration = 0;
int nVibDel = 0;


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



	if (Type == VIBRATIONTYPE_NOTHING)
	{
		nVibForce = 10000;
	}

	if (nDropMagic == -1)
	{
		SetJoypadVibration(0, 0, 1, nPlayertype);
		g_Vibration.Vibration = true;
	}

	if (g_Vibration.Vibration == false)
	{
		if (nDropMagic == 1)
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
	}


}
VIBRATION* GetVibration(void)
{
	return &g_Vibration;
}