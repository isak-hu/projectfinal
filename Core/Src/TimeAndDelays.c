/*
 * TimeAndDelays.c
 *
 *  Created on: 12 dec. 2025
 *      Author: isak
 */
// file used for all the specied delays and max times etc

#include "TimeAndDelays.h"

// all in ms
const uint16_t toggleFreq = 500;
const uint16_t pedestrianDelay = 10000;
const uint16_t walkingDelay = 10000;
const uint16_t orangeDelay = 2000;
const uint16_t GreenToOrangeDelay = pedestrianDelay - orangeDelay; // because of R1.3
const uint16_t greenDelay = 10000;
const uint16_t redDelayMax = 10000;
