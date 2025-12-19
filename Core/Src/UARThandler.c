/*
 * UARThandler.c
 *
 *  Created on: Dec 18, 2025
 *      Author: isak
 */
#include "UARThandler.h"
#include "stm32l4xx_hal.h"
#include "TimeAndDelays.h"
extern UART_HandleTypeDef huart2;


uint16_t toggleFreq = 500;
uint16_t pedestrianDelay = 10000;
uint16_t walkingDelay = 10000;
uint16_t orangeDelay = 2000;



uint8_t messageID;
uint16_t value;


static uint8_t cmdreciver [4];
static uint8_t ackindex [1];

void RuntimeConfig_init(){
	HAL_UART_Receive_IT(&huart2, cmdreciver, 4);
}





void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart)
{
	if (huart != &huart2) return;
  /* Prevent unused argument(s) compilation warning */
  UNUSED(huart);

  /* NOTE : This function should not be modified, when the callback is needed,
            the HAL_UART_RxCpltCallback can be implemented in the user file.
   */
  value = (cmdreciver[2] << 8) | cmdreciver[3];

  switch(cmdreciver[0]){

  case 0x01:

	  if(value >= 100 && value <= 2000){
		  toggleFreq = value;
		  ackindex[0] = 0x01;
	  }
	  else ackindex[0] = 0x0;
	  break;

  case 0x02:
	  if( value <= greenDelay){
		  pedestrianDelay = value;
		  ackindex[0] = 0x01;
	  }
	  else ackindex[0] = 0x0;
	  break;

  case 0x03:
	  if( value <= greenDelay){
		  walkingDelay = value;
		  ackindex[0] = 0x01;
	  }
	  else ackindex[0] = 0x0;
	  break;

  case 0x04:
	  if( 2*value < redDelayMax && 2*value < greenDelay && 2*value < pedestrianDelay ){
		  orangeDelay = value;
		  ackindex[0] = 0x01;
	  }
	  else ackindex[0] = 0x0;
	  break;
  default:
      ackindex[0] = 0x00;
      break;

  }
  HAL_UART_Transmit_IT(&huart2, ackindex, 1);
  HAL_UART_Receive_IT(&huart2, cmdreciver, 4);




}
