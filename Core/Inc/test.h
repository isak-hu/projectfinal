/*
 * test.h
 *
 *  Created on: Dec 9, 2025
 *      Author: isak2
 */

#ifndef INC_TEST_H_
#define INC_TEST_H_

//includes BEGIN
#include "main.h"
#include <stdint.h>
#include <stdio.h>
#include "GPIOinputs.h"
#include "SPIhandler.h"
#include "statemachine.h"
#include "statemachine2.h"
//includes END

//funtion prototye BEGIN
void Reset_Variables(void); //resets the funtions
void Test_PL1_Switch(void);// testing PL1_Switch
void Test_PL2_Switch(void);
void test_led_state(void); // dispalyina all possible states
void test_led_state2(void);
void test_CarOnLane(void);
//funtion prototype END

// variable declaration BEGIN

// variable declaration END


#endif /* INC_TEST_H_ */



