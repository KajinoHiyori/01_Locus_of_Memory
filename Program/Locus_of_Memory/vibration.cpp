#include "vibration.h"
#include "magic.h"
#include "input.h"
#include "debugproc.h"

void InitVibration(void)
{

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