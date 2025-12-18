/*
 * SPIhandler.h
 *
 *  Created on: Dec 9, 2025
 *      Author: Yichao Hu
 */

#ifndef INC_SPIHANDLER_H_
#define INC_SPIHANDLER_H_
// variable declaration BEGIN

//for the task 1 leds
extern uint8_t lights_init[3];
extern uint8_t pending_1[3];
extern uint8_t pending_0[3];
extern uint8_t G2R_1[3];
extern uint8_t G2R_0[3];
extern uint8_t Ped_Walk[3];
extern uint8_t R2G[3];


//for the task 2 leds
extern uint8_t Vgreen[3];
extern uint8_t V2Horange[3];
extern uint8_t Hgreen[3];
extern uint8_t H2Vorange[3];

//for the task 3 leds
extern uint8_t indicatorbit;


extern uint8_t Shownstate[3];

extern const uint8_t V_Orange_R2G[3];
extern uint8_t V_Green[3];
extern uint8_t V_Orange_G2R[3];

extern uint8_t H_Orange_R2G[3];
extern uint8_t H_Green[3];
extern uint8_t H_Orange_G2R[3];


// variable declaration END
void SPIshow_state(uint8_t*);
void SPIinit(void);




#endif /* INC_SPIHANDLER_H_ */

