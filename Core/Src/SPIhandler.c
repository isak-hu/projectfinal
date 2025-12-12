/*
 * SPIhandler.c
 *
 *  Created on: Dec 9, 2025
 *      Author: isak2
 */
#include "main.h"
extern SPI_HandleTypeDef hspi3;



// led arrys for the diffrent states in Task1

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




// led arrys for the diffrent states in Task2

//init state aka (Car_V_Green)
uint8_t Vgreen[3] = {0x21,0x04,0x01};
//init state aka (Car_V2H_Orange)
uint8_t V2Horange[3] = {0x11,0x02,0x01};
//init state aka (Car_H_Green)
uint8_t Hgreen[3] = {0x0C,0x01,0x04};
//init state aka (Car_H2V_Orange)
uint8_t H2Vorange[3] = {0x0A,0x01,0x02};







void SPIinit(){
	HAL_GPIO_WritePin(_595_Enable_GPIO_Port, _595_Enable_Pin, RESET);
	HAL_GPIO_WritePin(_595_Reset_GPIO_Port, _595_Reset_Pin, SET);
	HAL_GPIO_WritePin(_595_STCP_GPIO_Port, _595_STCP_Pin, RESET);

}

void SPIshow_state(uint8_t *state){
	HAL_SPI_Transmit(&hspi3,state,3, 50);
	HAL_GPIO_WritePin(_595_STCP_GPIO_Port, _595_STCP_Pin, SET);
	HAL_GPIO_WritePin(_595_STCP_GPIO_Port, _595_STCP_Pin, RESET);
}
