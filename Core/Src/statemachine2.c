
/*
 * statemachine2.c
 *
 *  Created on: 12 dec. 2025
 *      Author: Yichao Hu
 */

#include "statemachine2.h"
#include "UARThandler.h"


// variavles used for the ActiveCarOnLane funtion
const uint8_t Veritical = 0;
const uint8_t Horizontal = 1;

//time keeper for the delays and timers
volatile uint32_t Lane_start_time = 0;
volatile uint32_t waiting_car_start_time = 0;

// defining the states
typedef enum {				//TL:	1	2	3	4
    CAR_V_GREEN,		 	//		R	G	R	G
    CAR_V2H_ORANGE,			// 		R	O	R	O
    CAR_H_GREEN,			//		G	R	G	R
    CAR_H2V_ORANGE,			//		O	R	O	R
} TrafficState2;

// staring state
TrafficState2 lane_state = CAR_V_GREEN;


void Car_StateMachine(){
	uint8_t V_ACTIVE = ActiveCarOnLane(Veritical);
	uint8_t H_ACTIVE = ActiveCarOnLane(Horizontal);

	switch(lane_state){

		case CAR_V_GREEN:
			SPIshow_state(Vgreen);



			if (H_ACTIVE == 1 && waiting_car_start_time == 0) { //if there is a car on the horizontal lane start a waiting counter
			        waiting_car_start_time = HAL_GetTick();
			    }

			else if (H_ACTIVE == 0) {							// if there is no car there reset the counter
			        waiting_car_start_time = 0;
			}

			if ((V_ACTIVE == 0)&&(H_ACTIVE == 1)){				// if current lane is emty an there is a car waiting immeadaty switch so that lane can drive
				Lane_start_time = HAL_GetTick();
				waiting_car_start_time = 0;
				lane_state = CAR_V2H_ORANGE;
			}

			else if((H_ACTIVE == 1)){							// if there are cars on the other lane and the reddelay max has been surpassed switch lane

				if (HAL_GetTick() - waiting_car_start_time >= (redDelayMax-orangeDelay)){
					Lane_start_time = HAL_GetTick();
					waiting_car_start_time = 0;
					lane_state = CAR_V2H_ORANGE;
				}

			}
			else if((V_ACTIVE == 1)&&(H_ACTIVE == 0)){			// if there is no car waiting hold the light green untill there is a car waiting
				Lane_start_time = HAL_GetTick();
			}

			else if((V_ACTIVE == 0)&&(H_ACTIVE == 0)){			// if there is no cars switch state after green delay
				if (HAL_GetTick() - Lane_start_time >= (greenDelay-orangeDelay)){
					Lane_start_time = HAL_GetTick();
					lane_state = CAR_V2H_ORANGE;
				}

			}
			break;



		case CAR_V2H_ORANGE:
			SPIshow_state(V2Horange);
			if (HAL_GetTick() - Lane_start_time >= orangeDelay){// BASIC ORANGE DELAY
				Lane_start_time = HAL_GetTick();
				lane_state = CAR_H_GREEN;
			}

			break;


			// same logic as CAR_V_GREEN but mirrord so i wont be commenting
		case CAR_H_GREEN:
			SPIshow_state(Hgreen);

			if (V_ACTIVE == 1 && waiting_car_start_time == 0) {
			        waiting_car_start_time = HAL_GetTick();
			    }

			else if (V_ACTIVE == 0) {
			        waiting_car_start_time = 0;
			}

			if ((H_ACTIVE == 0)&&(V_ACTIVE == 1)){
				Lane_start_time = HAL_GetTick();
				waiting_car_start_time = 0;
				lane_state = CAR_H2V_ORANGE;
			}

			else if((V_ACTIVE == 1)){

				if (HAL_GetTick() - waiting_car_start_time >= (redDelayMax-orangeDelay)){
					Lane_start_time = HAL_GetTick();
					waiting_car_start_time = 0;
					lane_state = CAR_H2V_ORANGE;
				}

			}
			else if((H_ACTIVE == 1)&&(V_ACTIVE == 0)){
				Lane_start_time = HAL_GetTick();
			}

			else if((H_ACTIVE == 0)&&(V_ACTIVE == 0)){
				if (HAL_GetTick() - Lane_start_time >= (greenDelay-orangeDelay)){
					Lane_start_time = HAL_GetTick();
					lane_state = CAR_H2V_ORANGE;
				}

			}
			break;

		case CAR_H2V_ORANGE:
			SPIshow_state(H2Vorange);
			if (HAL_GetTick() - Lane_start_time >= orangeDelay){
				Lane_start_time = HAL_GetTick();
				lane_state = CAR_V_GREEN;
			}

			break;


	}

}


// helper funtion to check if there are any active cars on the chosen lane
uint8_t ActiveCarOnLane (uint8_t Lane){
	if(Lane == Veritical){
		if((TL4_Car_var || TL2_Car_var) == 1 ){ // if car 2 OR 4 is on lane
			return 1;

		}
		else return 0;
	}

	if(Lane == Horizontal){
			if((TL1_Car_var || TL3_Car_var) == 1 ){ // if car 1 OR 3 is on lane
				return 1;

			}
			else return 0;
		}
	return 3; // if this happens something is wrong
}



