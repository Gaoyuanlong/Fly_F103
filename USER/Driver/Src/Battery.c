#include "Battery.h"

void Battery_Init(Battery_* Aim);
void Battery_Read(Battery_* Aim);

Battery_OPS_ Battery_OPS = 
{
	Battery_Init,
	Battery_Read
};


void Battery_Init(Battery_* Aim)
{

}

void Battery_Read(Battery_* Aim)
{
	ADC.Updata();
	switch (Aim->ADC_Ch)
	{
		case 1:
			Aim->Volt = ADC.Data.CH1;
			break;
		default:
			break;
	}
}

