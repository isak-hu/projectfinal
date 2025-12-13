/*
 * statemachine.c
 *
 *  Created on: Dec 9, 2025
 *      Author: Yichao hu
 */
#include "statemachine.h"


// pirvate variabel def BEGIN

// timekeepers for the delays and toggling
volatile uint32_t state_start_time = 0;		//when it when into current state
volatile uint32_t toggled_at = 0;			//when it toggeeld last
volatile uint8_t  indicator_state = 0;

// private variabel def END


// defining the states
typedef enum {
    TL_GREEN_INIT,          // TL: Green   PL: Red    IND: of
    PED_PENDING,          	// TL: Green   PL: Red    IND: toogleing
    TL_ORANGE_G2R,      	// TL: Orange  PL: Red    IND: toogleing
    PED_WALK,             	// TL: Red     PL: Green  IND: of
    TL_ORANGE_R2G        	// TL: Orange  PL: Red    IND: of
} TrafficState;



// the state machine it self

TrafficState current_state = TL_GREEN_INIT;

void Pedestrian_StateMachine(void){

	switch(current_state){

		case TL_GREEN_INIT:

			SPIshow_state(lights_init);


			if (PL2_switch_var == 1){

				PL2_switch_var = 0;

				state_start_time = HAL_GetTick();
				toggled_at = HAL_GetTick();
				indicator_state = 0;
				current_state = PED_PENDING;

			}

			break;



		case PED_PENDING:

			// toggling the indicator light
			if (HAL_GetTick() - toggled_at >= toggleFreq){

				if (indicator_state == 0) {
					SPIshow_state(pending_1);
			        indicator_state = 1;
			    } else {

			        SPIshow_state(pending_0);
			        indicator_state = 0;

			    }

				toggled_at = HAL_GetTick();

				}
				// checking for trassition
			if (HAL_GetTick() - state_start_time >= GreenToOrangeDelay){
				state_start_time = HAL_GetTick();
				current_state = TL_ORANGE_G2R;
			}
			break;



		case TL_ORANGE_G2R:

			if (HAL_GetTick() - toggled_at >= toggleFreq){

				if (indicator_state == 0) {

					SPIshow_state(G2R_1);
					indicator_state = 1;
				} else {

					SPIshow_state(G2R_0);
					indicator_state = 0;

				}

				toggled_at = HAL_GetTick();

				}

			if (HAL_GetTick() - state_start_time >= orangeDelay){
				state_start_time = HAL_GetTick();
				current_state = PED_WALK;

			}
			break;

		case PED_WALK:

			SPIshow_state(Ped_Walk);

			if (HAL_GetTick()- state_start_time >= walkingDelay){
				state_start_time = HAL_GetTick();
				current_state = TL_ORANGE_R2G;

			}
			break;
		case TL_ORANGE_R2G:
			SPIshow_state(R2G);
			if (HAL_GetTick()- state_start_time >= orangeDelay){
				state_start_time = HAL_GetTick();
				current_state = TL_GREEN_INIT;
			}
			break;


	}

}
