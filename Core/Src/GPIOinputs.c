#include "GPIOinputs.h"

volatile uint8_t PL1_switch_var = 0;
volatile uint8_t PL2_switch_var = 0;
volatile uint8_t TL1_Car_var = 0;
volatile uint8_t TL2_Car_var = 0;
volatile uint8_t TL3_Car_var = 0;
volatile uint8_t TL4_Car_var = 0;

// for debouncing
volatile uint32_t PL1_Last_Interrupt = 0;
volatile uint32_t PL2_Last_Interrupt = 0;
#define DEBOUNCE_TIME 100 // in ms

void HAL_GPIO_EXTI_Callback(uint16_t GPIO_Pin)
{

	if(GPIO_Pin == PL1_Switch_Pin) {

		uint32_t current_time = HAL_GetTick();
		if ((current_time - PL1_Last_Interrupt) > DEBOUNCE_TIME){
				PL1_switch_var = 1;
				PL1_Last_Interrupt = current_time;
			}

	}
	if(GPIO_Pin == PL2_Switch_Pin) {

		uint32_t current_time = HAL_GetTick();
		if ((current_time - PL2_Last_Interrupt) > DEBOUNCE_TIME){
				PL2_switch_var = 1;
				PL2_Last_Interrupt = current_time;
			}
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

}
// a very cruisal  funtion used to read the intal carpossion when first running the code since the variables only change when a intreups happens
void Car_Starting_positions(){
    HAL_GPIO_EXTI_Callback(TL1_Car_Pin);
    HAL_GPIO_EXTI_Callback(TL2_Car_Pin);
    HAL_GPIO_EXTI_Callback(TL3_Car_Pin);
    HAL_GPIO_EXTI_Callback(TL4_Car_Pin);
}


