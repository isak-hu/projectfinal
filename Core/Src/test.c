/*
 * test.c
 *
 *  Created on: Dec 9, 2025
 *      Author: Yichao Hu
 */

#include "test.h"



// to see if the test passed we used the debugger to look at the values of some key variabels or verified on the phyiscal board
// the key variable has the comment KEY beside them


// button testing for the pedetrian buttons

uint8_t PL1_passed = 0;  // KEY
uint8_t PL2_passed = 0;  // KEY


void Reset_Variables(void) {
    PL1_switch_var = 0;
    PL2_switch_var = 0;

}

void Test_PL1_Switch(void){

    Reset_Variables();
    HAL_GPIO_EXTI_Callback(PL1_Switch_Pin);

    if (PL1_switch_var == 1){
    	PL1_passed = 1;
    }
    else PL1_passed = 0;

}
void Test_PL2_Switch(void){

    Reset_Variables();
    HAL_GPIO_EXTI_Callback(PL2_Switch_Pin);

    if (PL2_switch_var == 1){
    	PL2_passed = 1;
    }
    else PL2_passed = 0;
}


// State led testing
// goes throug all the possoble states for task one
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

void test_led_state2(){

	while(1){
		SPIshow_state(Vgreen);
		HAL_Delay(1000);
		SPIshow_state(V2Horange1);
		HAL_Delay(1000);
		SPIshow_state(V2Horange2);
		HAL_Delay(1000);
		SPIshow_state(Hgreen);
		HAL_Delay(1000);
		SPIshow_state(H2Vorange1);
		HAL_Delay(1000);
		SPIshow_state(H2Vorange2);
		HAL_Delay(1000);

	}
}

void test_led_state3(){

	while(1){
		SPIshow_state(V_Green);
		HAL_Delay(2000);
		SPIshow_state(V_Orange_G2R);
		HAL_Delay(2000);
		SPIshow_state(H_Orange_R2G);
		HAL_Delay(2000);
		SPIshow_state(H_Green);
		HAL_Delay(2000);
		SPIshow_state(H_Orange_G2R);
		HAL_Delay(2000);

		HAL_Delay(2000);



	}
}

uint8_t OnLane = 0;
void test_CarOnLane(){
	 OnLane = ActiveCarOnLane(Vertical);// write what lane you want to test and KEY
}

// go to state

void GoToState(){}






