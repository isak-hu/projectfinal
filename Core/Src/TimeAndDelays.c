/*
 * TimeAndDelays.c
 *
 *  Created on: 12 dec. 2025
 *      Author: Yichao Hu
 */
// file used for all the specied delays and max times etc

#include "TimeAndDelays.h"
#include "UARThandler.h"

 uint16_t greenDelay = 10000; // if there is no cars in any direction switch active lane svery green delay
 uint16_t redDelayMax = 10000;// the maxium time a car has to wait for green ligtht
// all in ms



 //pre task 5 variables

//const uint16_t toggleFreq = 500; // how fast the indcator light toggles
//const uint16_t pedestrianDelay = 10000; // how long the pederian have to wait before he can walk
//const uint16_t walkingDelay = 10000; // how long the pedstrian light stays green
//const uint16_t orangeDelay = 2000; // how long the light says orange
// uint16_t GreenToOrangeDelay = pedestrianDelay - 2 * orangeDelay; // will be Pedetrian in RTOS

// uint16_t Reddelay = redDelayMax - 2* orangeDelay; // beacue it has to wait for two orange daly before the light before it turns green
// uint16_t CarGreenToOrangeDelay = greenDelay - 2* orangeDelay; //Will be Green in RTOS
