/*
 * GPIOinputs.h
 *
 *  Created on: Dec 9, 2025
 *      Author: isak2
 */

#ifndef INC_GPIOINPUTS_H_
#define INC_GPIOINPUTS_H_

#include <stdint.h>
#include "main.h"


//funtion prototye BEGIN
void HAL_GPIO_EXTI_Callback(uint16_t GPIO_Pin);
void Car_Starting_positions(void);
//funtion prototype END

// variable declaration BEGIN
extern volatile uint8_t PL1_switch_var;
extern volatile uint8_t PL2_switch_var;
extern volatile uint8_t TL1_Car_var;
extern volatile uint8_t TL2_Car_var;
extern volatile uint8_t TL3_Car_var;
extern volatile uint8_t TL4_Car_var;
// variable declaration END

#endif /* INC_GPIOINPUTS_H_ */
