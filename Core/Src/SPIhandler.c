/*
 * SPIhandler.c
 *
 *  Created on: Dec 9, 2025
 *      Author: isak2
 */
#include "main.h"
extern SPI_HandleTypeDef hspi3;

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
