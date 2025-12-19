/*
 * statemachine2.h
 *
 *  Created on: 12 dec. 2025
 *      Author: Yichao Hu
 */

#ifndef SRC_STATEMACHINE2_H_
#define SRC_STATEMACHINE2_H_

#include <stdint.h>
#include "main.h"
#include "SPIhandler.h"
#include "GPIOinputs.h"
#include "TimeAndDelays.h"

// variable declaration BEGIN


extern const uint8_t Vertical;
extern const uint8_t Horizontal;
// variable declaration END

//funtion prototye BEGIN

uint8_t ActiveCarOnLane (uint8_t);
void Car_StateMachine(void);
//funtion prototype END

#endif /* SRC_STATEMACHINE2_H_ */
