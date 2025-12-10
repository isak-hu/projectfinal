/*
 * test.c
 *
 *  Created on: Dec 9, 2025
 *      Author: isak2
 */

#include "test.h"



// button testing

uint8_t PL1_passed = 0;
uint8_t PL2_passed = 0;
uint8_t TL1_Car_passed = 0;
uint8_t TL2_Car_passed = 0;
uint8_t TL3_Car_passed = 0;
uint8_t TL4_Car_passed = 0;

void Reset_Variables(void) {
    PL1_switch_var = 0;
    PL2_switch_var = 0;
    TL1_Car_var = 0;
    TL2_Car_var = 0;
    TL3_Car_var = 0;
    TL4_Car_var = 0;
}

void Test_PL1_Switch(void){

    Reset_Variables();
    HAL_GPIO_EXTI_Callback(PL1_Switch_Pin);

    if (PL1_switch_var == 1){
    	PL1_passed = 1;
    }

}
void Test_PL2_Switch(void){

    Reset_Variables();
    HAL_GPIO_EXTI_Callback(PL2_Switch_Pin);

    if (PL2_switch_var == 1){
    	PL2_passed = 1;
    }
}




// State led testing
// goes throug all the possoble states
void test_led_state(){

	while(1){
		SPIshow_state(lights_init);
		HAL_Delay(1000);
		SPIshow_state(pending_1);
		HAL_Delay(1000);
		SPIshow_state(pending_0);
		HAL_Delay(1000);
		SPIshow_state(G2R_1);
		HAL_Delay(1000);
		SPIshow_state(G2R_0);
		HAL_Delay(1000);
		SPIshow_state(Ped_Walk);
		HAL_Delay(1000);
		SPIshow_state(R2G);
		HAL_Delay(1000);
	}
}

// go to state

void GoToState(){}






