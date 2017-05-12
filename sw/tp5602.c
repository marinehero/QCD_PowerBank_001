#include "tp5602.h"
#include "adc.h"
#include "main.h"


static u16 curBattVoltage;
static u8 curBattCapacity;
static u8 curInputVoltage;

static u8 second_count;
static bool tp5602_key_press_flag;
//==========================================================
//��ص�ѹ
u16 GetBattVoltage(void)
{
	return (curBattVoltage);
}

//��ص���
u8 GetBattCapacity(void)
{
	return (curBattCapacity);
}

//������ƿ��ѹ
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
	return (((PA_IDR & BIT2) > 0) ? true : false);
}
//==========================================================
//==========================================================
void Tp5602KeyPress(void)
{
	PA_ODR |= BIT1;
	tp5602_key_press_flag = false;
}
//==========================================================
//�����ص�ǰʣ�����
static void CalculationCurBattCapacity(void)
{
	
}
//==========================================================
//����ѭ����ÿ 10ms ����һ��
void Tp5602Funtion(void)
{
	if(PA_ODR & BIT1)
	{
		if(false == tp5602_key_press_flag)
		{
			tp5602_key_press_flag = true;
		}
		else
		{
			PA_ODR &= ~(BIT1);
		}
	}
	
	if((second_count ++) > 10)
	{
		second_count = 0;
		curBattVoltage = (u16)((((u32)GetAdc(ADC_CH_BATT)) * 495) >> 10);
		CalculationCurBattCapacity();
		
		curInputVoltage = (u8)((((u32)GetAdc(ADC_CH_INPUT)) * 99) >> 9) + 8;
	}
}
//==========================================================
void TP5602InitPoweron(void)
{
	PA_DDR |=  (BIT1);
	PA_CR1 |=  (BIT1);
	PA_CR2 &= ~(BIT1);
	PA_ODR &= ~(BIT1);
	
	PA_DDR &= ~(BIT2);
	PA_CR1 &= ~(BIT2);
	PA_CR2 &= ~(BIT2);
	
	curBattVoltage = 0;
	curBattCapacity = 0;
	curInputVoltage = 0;
	second_count = 0xfe;
}
//==========================================================
//end files
