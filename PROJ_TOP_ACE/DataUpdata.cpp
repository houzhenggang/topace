﻿#include "DataUpdata.h"
#include "KeyInput.h"
#include "UnitMath.h"
#include "ExchangeThread.h"
float angleR=0.0f;
float Test3dsFrame=0.0f;
float maxFreme=100.0f;
bool Test3dsFrameSwitch=false;
extern tKeyInput KeyInput;
CUnitMath TestUnit,TestView;
CUnitMath TestUnitExchange;
int ReadingThreadNum=0;
float extern moveZ;
float extern moveY;
float extern moveX;
float extern PosOrgY;
float extern PosOrgZ;
CExchangeThread ThreadDataUpdata;
void DataUpdata()
{
	if(Test3dsFrameSwitch)
	{
		Test3dsFrame=Test3dsFrame+0.5f;
		if(Test3dsFrame>=maxFreme)
		{
			Test3dsFrame=0.0f;
			//Test3dsFrameSwitch=false;
		}
	}
	else
	{
		Test3dsFrame=Test3dsFrame-1.0f;
		if(Test3dsFrame<=0.0f)
		{
			Test3dsFrame=0.0f;
			Test3dsFrameSwitch=true;
		}
	}
	angleR=angleR+0.2f;

	TestView.Reset();
	TestView.MovExternal(_mm_set_ps(1.0f,-25-moveZ-PosOrgZ,moveY+PosOrgY,moveX));
	TestUnit.PosTo(TestView.UnitPos,1.25f);
	TestView.PosTo(_mm_set_ps(1.0f,0.0f,0.0f,0.0f));
	CUnitMath UnitMathDraw;
	UnitMathDraw.UnitPos=TestUnit.UnitPos;
	UnitMathDraw.UnitQuat=TestUnit.UnitQuat;
	UnitMathDraw.RotInternal(-90,1.0f,0.0f,0.0f);

	ThreadDataUpdata.DataCount=3;
	ThreadDataUpdata.DataList[1].UnitData_States=_UnitData_States_Use;
	ThreadDataUpdata.DataList[2].UnitData_States=_UnitData_States_Use;
	TestView.GetMatrix(ThreadDataUpdata.DataList[1].Matrix);
	UnitMathDraw.GetMatrix(ThreadDataUpdata.DataList[2].Matrix);

	ThreadUpdataToExchange(&ThreadDataUpdata);

}