#include "GPIOinputs.h"

uint8_t PL1_switch_var = 0;
uint8_t PL2_switch_var = 0;
uint8_t TL1_Car_var = 0;
uint8_t TL2_Car_var = 0;
uint8_t TL3_Car_var = 0;
uint8_t TL4_Car_var = 0;


void HAL_GPIO_EXTI_Callback(uint16_t GPIO_Pin)
{

	if(GPIO_Pin == PL1_Switch_Pin) {
		PL1_switch_var = 1 ;
	}
	if(GPIO_Pin == PL2_Switch_Pin) {
		PL2_switch_var = 1 ;
	}
	if(GPIO_Pin == TL1_Car_Pin) {
		if (HAL_GPIO_ReadPin(TL1_Car_GPIO_Port, TL1_Car_Pin) == 0) TL1_Car_var = 1;
			else TL1_Car_var = 0;
	}
	if(GPIO_Pin == TL2_Car_Pin) {
		if (HAL_GPIO_ReadPin(TL2_Car_GPIO_Port, TL2_Car_Pin) == 0) TL2_Car_var = 1;
				else TL2_Car_var = 0;
	}
	if(GPIO_Pin == TL3_Car_Pin) {
		if (HAL_GPIO_ReadPin(TL3_Car_GPIO_Port, TL3_Car_Pin) == 0) TL3_Car_var = 1;
				else TL3_Car_var = 0;
	}
	if(GPIO_Pin == TL4_Car_Pin) {
		if (HAL_GPIO_ReadPin(TL4_Car_GPIO_Port, TL4_Car_Pin) == 0) TL4_Car_var = 1;
				else TL4_Car_var = 0;
	}
	else {
	      __NOP();
	  }
	}


