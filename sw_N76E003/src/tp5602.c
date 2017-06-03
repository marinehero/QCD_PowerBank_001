#include "../inc/tp5602.h"
#include "../inc/adc.h"
#include "../inc/main.h"


static u16 curBattVoltage;
static u8 curBattCapacity;
static u8 curInputVoltage;
static u8 second_count;
static u8 mathBattCapacityDlySecond;  //计算电量延时计数器
static bool tp5602_key_press_flag;
static bool isConnectInputPower_Old;  //保存上一个检测周期的外部电源状态
//==========================================================
//电池电压
u16 GetBattVoltage(void)
{
	return (curBattVoltage);
}

//电池电量
u8 GetBattCapacity(void)
{
	return (curBattCapacity);
}

//车辆电瓶电压
u8 GetInputVoltage(void)
{
	return (curInputVoltage);
}

bool IsConnectedInputPower(void)
{
	return ((curInputVoltage > 90) ? true : false);
}

bool IsBoostOut(void)
{
	return ((P17) ? true : false);
}
//==========================================================
//==========================================================
void Tp5602KeyPress(void)
{
	if(curBattVoltage<300) return;
	P30=1;
	tp5602_key_press_flag = false;
}
//==========================================================
//计算电池当前剩余电量
code u16 voltageadd[]=
{
	420,//100
	410,//90
	400,//80
	390,//70
	380,//60
	370,//50
	360,//40 
	350,//30
	340,//20
	330,//10
	320 //0
};

code u16 voltagedec[]=
{
	400,//100
	390,//90	
	380,//80	
	370,//70	
	360,//60
	350,//50
	340,//40
	330,//30
	320,//20
	310,//10
	300//0
};
static void CalculationCurBattCapacity(void)
{ 
	u8 i, tmp;
	u16 voltage = curBattVoltage;
	
	if(mathBattCapacityDlySecond)
	{
		mathBattCapacityDlySecond --;
		return;
	}
	
	if(IsConnectedInputPower())
	{//充电
		for(i = 0; i < 10; i ++)
		{
			if(voltage > voltageadd[i])
			{
				
				break;
			}
		}
		tmp = 100 - ((i > 10) ? 10 : i) * 10;
		if(tmp > curBattCapacity) 
		{
			curBattCapacity = tmp;
		}
	}
	else
	{//放电
		for(i = 0; i < 10; i ++)
		{
			if(voltage > voltagedec[i])
			{
				break;
			}
		}
		tmp = 100 - ((i > 10) ? 10 : i) * 10;
		if(tmp < curBattCapacity) 
		{
			curBattCapacity = tmp;
		}
	}
}
//==========================================================
//在主循环中每 10ms 调用一次
void Tp5602Funtion(void)
{
	if(P30)
	{
		if(false == tp5602_key_press_flag)
		{
			tp5602_key_press_flag = true;
		}
		else
		{
			P30=0;
		}
	}
	
	curInputVoltage = (u16)(((u32)GetAdc(ADC_CH_INPUT)*72)/GetAdc(ADC_CH_REF))+8;
	if(isConnectInputPower_Old != IsConnectedInputPower())
	{
		isConnectInputPower_Old = IsConnectedInputPower();
		mathBattCapacityDlySecond = 30;   //充电放电状态切换的30S内,不更新电量状态
	}
		
	if(++ second_count > 100)
	{
		second_count = 0;
		curBattVoltage = (u16)(((u32)GetAdc(ADC_CH_BATT)*120)/GetAdc(ADC_CH_REF));
		CalculationCurBattCapacity();
	}
}
//==========================================================
void TP5602InitPoweron(void)
{
	clr_P3M1_0;set_P3M2_0;
	clr_P30;
	set_P1M1_7;clr_P1M2_7;

	
	curBattVoltage = 0;
	curBattCapacity = 0;
	curInputVoltage = 0;
	second_count = 0;
	mathBattCapacityDlySecond = 0;
	isConnectInputPower_Old = false;
}
//==========================================================
//end files
