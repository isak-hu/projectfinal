///*
// * UARThandler.c
// *
// *  Created on: Dec 18, 2025
// *      Author: isak
// */
//#include "UARThandler.h"
//#include "stm32l4xx_hal.h"
//extern UART_HandleTypeDef huart2;
//
//
////uint16_t toggleFreq = 500;
////uint16_t pedestrianDelay = 10000;
////uint16_t walkingDelay = 10000;
////uint16_t orangeDelay = 2000;
//
//
//uint8_t freq   = 0x01;
//uint8_t ped    = 0x02;
//uint8_t walk   = 0x03;
//uint8_t orange = 0x04;
//uint8_t messageID;
//uint16_t value;
//
//
//static uint8_t cmdreciver [4];
//static uint8_t ackindex [1];
//
//void RuntimeConfig_init(void){
//	HAL_UART_Receive_IT(&huart2, cmdreciver, 4);
//}
//
//
//
//
//
//void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart)
//{
//  /* Prevent unused argument(s) compilation warning */
//  UNUSED(huart);
//
//  /* NOTE : This function should not be modified, when the callback is needed,
//            the HAL_UART_RxCpltCallback can be implemented in the user file.
//   */
//  value = (cmdreciver[2] << 8) | cmdreciver[3];
//
//  switch(cmdreciver[0]){
//
//  case freq:
//
//	  if(value >= 100 && value <= 2000){
//		  toggleFreq = value;
//		  ackindex[0] = 0x01;
//	  }
//	  else ackindex[0] = 0x0;
//	  break;
//
//  case ped:
//	  break;
//
//  case walk:
//	  break;
//
//  case orange:
//	  break;
//
//
//
//  }
//
//
//}
