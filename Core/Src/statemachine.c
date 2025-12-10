/*
 * statemachine.c
 *
 *  Created on: Dec 9, 2025
 *      Author: isak2
 */
#include "statemachine.h"


// pirvate variabel def BEGIN

// all in ms
const uint16_t toggleFreq = 500;
const uint16_t pedestrianDelay = 10000;
const uint16_t walkingDelay = 10000;
const uint16_t orangeDelay = 2000;

const uint16_t GreenToOrangeDelay = pedestrianDelay - orangeDelay; // because of R1.3

// timekeepers for the delays and toggling
volatile uint32_t state_start_time = 0;		//when it when into current state
volatile uint32_t toggled_at = 0;			//when it toggeeld last
volatile uint8_t  indicator_state = 0;

// private variabel def END




// led arrys for the diffrent states to feed to SPI handler

//init state aka (TL GREEN INIT)
uint8_t lights_init[3] = {0x20,0x0c,0x00};

//pending state aka (PED_PENDING)
uint8_t pending_1[3] = {0x20,0x2c,0x00}; //indicator on
uint8_t pending_0[3] = {0x20,0x0c,0x00}; //indicator off

//Green to Red so when TL is orange aka (TL_ORANGE_G2R)
uint8_t G2R_1[3] = {0x10,0x2a,0x00}; //indicator on
uint8_t G2R_0[3] = {0x10,0x0a,0x00}; //indicator off

//Ped_walk aka (PED_WALK)
uint8_t Ped_Walk[3] = {0x08,0x11,0x00};

//Red to Green so when TL is orange aka (TL_ORANGE_R2G)
uint8_t R2G[3] = {0x10,0x0a,0x00};




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

void Taffic_StateMachine(void){

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
