/*
 * statemachine.h
 *
 *  Created on: Dec 9, 2025
 *      Author: isak2
 */

#ifndef INC_STATEMACHINE_H_
#define INC_STATEMACHINE_H_

#include <stdint.h>
#include "main.h"
#include "SPIhandler.h"
#include "GPIOinputs.h"



//funtion prototye BEGIN

void Taffic_StateMachine(void);

//funtion prototype END

// variable declaration BEGIN
extern uint8_t lights_init[3];

extern uint8_t pending_1[3];
extern uint8_t pending_0[3];

extern uint8_t G2R_1[3];
extern uint8_t G2R_0[3];

extern uint8_t Ped_Walk[3];

extern uint8_t R2G[3];


// variable declaration END

#endif /* INC_STATEMACHINE_H_ */
