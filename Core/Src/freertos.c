/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * File Name          : freertos.c
  * Description        : Code for freertos applications
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2025 STMicroelectronics.
  * All rights reserved.
  *
  * This software is licensed under terms that can be found in the LICENSE file
  * in the root directory of this software component.
  * If no LICENSE file comes with this software, it is provided AS-IS.
  *
  ******************************************************************************
  */
/* USER CODE END Header */

/* Includes ------------------------------------------------------------------*/
#include "FreeRTOS.h"
#include "task.h"
#include "main.h"
#include "cmsis_os.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include "GPIOinputs.h"
#include "SPIhandler.h"
#include "TimeAndDelays.h"
#include "statemachine2.h"
#include <string.h>
#include "timers.h"


/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */
typedef enum {
    STATE_RED,
    STATE_GREEN,
    STATE_PENDING,     // Button pressed, waiting for Green timer
    STATE_ORANGE_G2R,  // Green to Red transition
    STATE_ORANGE_R2G,  // Red to Green transition
    STATE_PED_WALK     // Pedestrian crossing active
} LightState;
LightState current = STATE_GREEN;

/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */
#define TOGGLE_EVT_1     (1 << 0)
#define TOGGLE_EVT_2     (1 << 1)
#define REDMAX_EVT    	 (1 << 2)
#define ORANGE_EVT    	 (1 << 3)
#define GREEN_EVT    	 (1 << 4)
#define PED_EVT     	 (1 << 5)
#define SHOW_EVT		 (1 << 6)
#define WALK_EVT		 (1 << 6)

/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/
/* USER CODE BEGIN Variables */
uint32_t toggle_bit;  // global variable used for timer argument





/* USER CODE END Variables */
/* Definitions for defaultTask */
osThreadId_t defaultTaskHandle;
const osThreadAttr_t defaultTask_attributes = {
  .name = "defaultTask",
  .stack_size = 128 * 4,
  .priority = (osPriority_t) osPriorityNormal,
};
/* Definitions for VertialTask */
osThreadId_t VertialTaskHandle;
const osThreadAttr_t VertialTask_attributes = {
  .name = "VertialTask",
  .stack_size = 128 * 4,
  .priority = (osPriority_t) osPriorityNormal,
};
/* Definitions for HorizontalTask */
osThreadId_t HorizontalTaskHandle;
const osThreadAttr_t HorizontalTask_attributes = {
  .name = "HorizontalTask",
  .stack_size = 128 * 4,
  .priority = (osPriority_t) osPriorityNormal,
};
/* Definitions for ActuatorTask */
osThreadId_t ActuatorTaskHandle;
const osThreadAttr_t ActuatorTask_attributes = {
  .name = "ActuatorTask",
  .stack_size = 128 * 4,
  .priority = (osPriority_t) osPriorityLow,
};
/* Definitions for MainQueue */
osMessageQueueId_t MainQueueHandle;
const osMessageQueueAttr_t MainQueue_attributes = {
  .name = "MainQueue"
};
/* Definitions for Green */
osTimerId_t GreenHandle;
const osTimerAttr_t Green_attributes = {
  .name = "Green"
};
/* Definitions for Orange */
osTimerId_t OrangeHandle;
const osTimerAttr_t Orange_attributes = {
  .name = "Orange"
};
/* Definitions for RedMax */
osTimerId_t RedMaxHandle;
const osTimerAttr_t RedMax_attributes = {
  .name = "RedMax"
};
/* Definitions for Walking */
osTimerId_t WalkingHandle;
const osTimerAttr_t Walking_attributes = {
  .name = "Walking"
};
/* Definitions for Pedestrian */
osTimerId_t PedestrianHandle;
const osTimerAttr_t Pedestrian_attributes = {
  .name = "Pedestrian"
};
/* Definitions for ToggleTimer */
osTimerId_t ToggleTimerHandle;
const osTimerAttr_t ToggleTimer_attributes = {
  .name = "ToggleTimer"
};
/* Definitions for UART_Mutex */
osMutexId_t UART_MutexHandle;
const osMutexAttr_t UART_Mutex_attributes = {
  .name = "UART_Mutex"
};
/* Definitions for ImGreenNow */
osSemaphoreId_t ImGreenNowHandle;
const osSemaphoreAttr_t ImGreenNow_attributes = {
  .name = "ImGreenNow"
};

/* Private function prototypes -----------------------------------------------*/
/* USER CODE BEGIN FunctionPrototypes */
uint8_t IsActive(osThreadId_t );
void Restbutton(void);
/* USER CODE END FunctionPrototypes */

void StartDefaultTask(void *argument);
void StartVertialTask(void *argument);
void StartHorizontalTask(void *argument);
void StartActuatorTask(void *argument);
void GreenDone(void *argument);
void OrangeDone(void *argument);
void RedMaxDone(void *argument);
void WalkingDone(void *argument);
void PedestrianDone(void *argument);
void ToggleTimerCallback(void *argument);

void MX_FREERTOS_Init(void); /* (MISRA C 2004 rule 8.1) */

/**
  * @brief  FreeRTOS initialization
  * @param  None
  * @retval None
  */
void MX_FREERTOS_Init(void) {
  /* USER CODE BEGIN Init */

  /* USER CODE END Init */
  /* Create the mutex(es) */
  /* creation of UART_Mutex */
  UART_MutexHandle = osMutexNew(&UART_Mutex_attributes);

  /* USER CODE BEGIN RTOS_MUTEX */
  /* add mutexes, ... */
  /* USER CODE END RTOS_MUTEX */

  /* Create the semaphores(s) */
  /* creation of ImGreenNow */
  ImGreenNowHandle = osSemaphoreNew(1, 1, &ImGreenNow_attributes);

  /* USER CODE BEGIN RTOS_SEMAPHORES */
  /* add semaphores, ... */
  /* USER CODE END RTOS_SEMAPHORES */

  /* Create the timer(s) */
  /* creation of Green */
  GreenHandle = osTimerNew(GreenDone, osTimerOnce, NULL, &Green_attributes);

  /* creation of Orange */
  OrangeHandle = osTimerNew(OrangeDone, osTimerOnce, NULL, &Orange_attributes);

  /* creation of RedMax */
  RedMaxHandle = osTimerNew(RedMaxDone, osTimerOnce, NULL, &RedMax_attributes);

  /* creation of Walking */
  WalkingHandle = osTimerNew(WalkingDone, osTimerOnce, NULL, &Walking_attributes);

  /* creation of Pedestrian */
  PedestrianHandle = osTimerNew(PedestrianDone, osTimerOnce, NULL, &Pedestrian_attributes);

  /* creation of ToggleTimer */
  ToggleTimerHandle = osTimerNew(ToggleTimerCallback, osTimerPeriodic, (void*) &toggle_bit, &ToggleTimer_attributes);

  /* USER CODE BEGIN RTOS_TIMERS */
  /* start timers, add new ones, ... */
  /* USER CODE END RTOS_TIMERS */

  /* Create the queue(s) */
  /* creation of MainQueue */
  MainQueueHandle = osMessageQueueNew (1, sizeof(uint16_t), &MainQueue_attributes);

  /* USER CODE BEGIN RTOS_QUEUES */
  /* add queues, ... */
  /* USER CODE END RTOS_QUEUES */

  /* Create the thread(s) */
  /* creation of defaultTask */
  defaultTaskHandle = osThreadNew(StartDefaultTask, NULL, &defaultTask_attributes);

  /* creation of VertialTask */
  VertialTaskHandle = osThreadNew(StartVertialTask, NULL, &VertialTask_attributes);

  /* creation of HorizontalTask */
  HorizontalTaskHandle = osThreadNew(StartHorizontalTask, NULL, &HorizontalTask_attributes);

  /* creation of ActuatorTask */
  ActuatorTaskHandle = osThreadNew(StartActuatorTask, NULL, &ActuatorTask_attributes);

  /* USER CODE BEGIN RTOS_THREADS */
  /* add threads, ... */
  /* USER CODE END RTOS_THREADS */

  /* USER CODE BEGIN RTOS_EVENTS */
  /* add events, ... */
  /* USER CODE END RTOS_EVENTS */

}

/* USER CODE BEGIN Header_StartDefaultTask */
/**
  * @brief  Function implementing the defaultTask thread.
  * @param  argument: Not used
  * @retval None
  */
/* USER CODE END Header_StartDefaultTask */
void StartDefaultTask(void *argument)
{
  /* USER CODE BEGIN StartDefaultTask */

  /* Infinite loop */
  for(;;)
  {


    osDelay(1);
  }
  /* USER CODE END StartDefaultTask */
}

/* USER CODE BEGIN Header_StartVertialTask */
/**
* @brief Function implementing the VertialTask thread.
* @param argument: Not used
* @retval None
*/
/* USER CODE END Header_StartVertialTask */
void StartVertialTask(void *argument)
{
  /* USER CODE BEGIN StartVertialTask */
 osSemaphoreAcquire(ImGreenNowHandle, osWaitForever);

 uint32_t events;


  /* Infinite loop */
  for(;;)
  {
	  uint8_t V_ACTIVE = ActiveCarOnLane(Veritical);
	  uint8_t H_ACTIVE = ActiveCarOnLane(Horizontal);

	  switch(current){

	  case STATE_GREEN:
	      memcpy(Shownstate, V_Green, 3);
	      xTaskNotify(ActuatorTaskHandle, SHOW_EVT, eSetBits);

	      // is there is a horizotal car star the redmax timer of there is any and if it leave reset the timer
	      if (H_ACTIVE == 1 && IsActive(RedMaxHandle) == 0) {
	          osTimerStart(RedMaxHandle, pdMS_TO_TICKS(redDelayMax));
	          osTimerStop(GreenHandle);
	      }
	      else if (H_ACTIVE == 0) {
	          osTimerStop(RedMaxHandle);
	      }

	      // diffrent possble lane combinations
	      if ((V_ACTIVE == 0) && (H_ACTIVE == 1)) {  // imeadiate state change stop all the timers
	          osTimerStop(RedMaxHandle);
	          osTimerStop(GreenHandle);
	          current = STATE_ORANGE_G2R;
	      }
	      else if ((V_ACTIVE == 1) && (H_ACTIVE == 0)) { // stay green forever so reset all the timers so it never triggers a state change
	          osTimerStop(RedMaxHandle);
	          osTimerStop(GreenHandle);
	      }
	      else if ((V_ACTIVE == 0) && (H_ACTIVE == 0) && !IsActive(GreenHandle)) {// all empty start the greentimer
	          osTimerStart(GreenHandle, pdMS_TO_TICKS(greenDelay));
	      }

	      // Pedestrian button press immediately goes to pending
	      if (PL2_switch_var == 1) {
	          PL2_switch_var = 0;
	          osTimerStop(RedMaxHandle);
	          osTimerStop(GreenHandle);
	          current = STATE_PENDING;
	      }

	      // Check timer/event notifications without blocking
	      BaseType_t notified = xTaskNotifyWait(0, 0xFFFFFFFF, &events, pdMS_TO_TICKS(10));
	      if (notified == pdTRUE) {

	          if (events & GREEN_EVT) {
	              current = STATE_ORANGE_G2R;
	          }
	          else if (events & REDMAX_EVT) {
	              current = STATE_ORANGE_G2R;
	          }

	      }
	  break;



	  case STATE_ORANGE_G2R:

	      memcpy(Shownstate, V_Orange_G2R, 3);
	      xTaskNotify(ActuatorTaskHandle, SHOW_EVT, eSetBits);


	      if (!IsActive(OrangeHandle)) {
	          osTimerStart(OrangeHandle, pdMS_TO_TICKS(orangeDelay));
	      }

	      xTaskNotifyWait(0, 0xFFFFFFFF, &events, osWaitForever);

	      if (events & ORANGE_EVT) {
	          current = STATE_RED;               // Transition to RED
	        // Give turn to other task
	      }
	  break;




	  case STATE_RED:
		  osSemaphoreRelease(ImGreenNowHandle);
          current = STATE_ORANGE_R2G;
		  osThreadYield();
          osSemaphoreAcquire(ImGreenNowHandle, osWaitForever);

		  break;

	  case STATE_ORANGE_R2G:
          memcpy(Shownstate, V_Orange_R2G, 3);
          xTaskNotify(ActuatorTaskHandle, SHOW_EVT, eSetBits);
	      if (!IsActive(OrangeHandle)) {
	          osTimerStart(OrangeHandle, pdMS_TO_TICKS(orangeDelay));
	      }

	      xTaskNotifyWait(0, 0xFFFFFFFF, &events, osWaitForever);

	      if (events & ORANGE_EVT) {
	          current = STATE_GREEN;
	      }


		  break;

	  case STATE_PENDING:

		  toggle_bit =  TOGGLE_EVT_2;
		   if (!IsActive(ToggleTimerHandle)) {
		       osTimerStart(ToggleTimerHandle, pdMS_TO_TICKS(toggleFreq));
		    }

	      if (!IsActive(PedestrianHandle)) {
	          osTimerStart(PedestrianHandle, pdMS_TO_TICKS(pedestrianDelay));
	      }
	      xTaskNotifyWait(0, 0xFFFFFFFF, &events, osWaitForever);

	      if (events & PED_EVT) {
	    	  memcpy(Shownstate, V_Orange_R2G, 3);
		      if (!IsActive(OrangeHandle)) {
		          osTimerStart(OrangeHandle, pdMS_TO_TICKS(orangeDelay));

		      }
		  xTaskNotifyWait(0, 0xFFFFFFFF, &events, osWaitForever);
	      if (events & ORANGE_EVT) {
	    	  current = STATE_PED_WALK;

		      }
	      }
	      break;
	  case STATE_PED_WALK:
		  osTimerStop(ToggleTimerHandle);
    	  memcpy(Shownstate, H_Green, 3);
    	  xTaskNotify(ActuatorTaskHandle,SHOW_EVT,eSetBits);


    	  if (!IsActive(WalkingHandle)) {
		       osTimerStart(WalkingHandle, pdMS_TO_TICKS(walkingDelay));
		  }
    	  xTaskNotifyWait(0, 0xFFFFFFFF, &events, osWaitForever);
    	  if (events & WALK_EVT) {
		    	current = STATE_ORANGE_R2G;

    	  }



		  break;
	  }

	  }



  }
  /* USER CODE END StartVertialTask */


/* USER CODE BEGIN Header_StartHorizontalTask */
/**
* @brief Function implementing the HorizontalTask thread.
* @param argument: Not used
* @retval None
*/
/* USER CODE END Header_StartHorizontalTask */
void StartHorizontalTask(void *argument)
{
  /* USER CODE BEGIN StartHorizontalTask */
  /* Infinite loop */
	osSemaphoreAcquire(ImGreenNowHandle, osWaitForever);
	current = STATE_ORANGE_R2G;
	uint32_t events;// Start with its own R2G
  for(;;)
  {

	  uint8_t V_ACTIVE = ActiveCarOnLane(Veritical);
	  uint8_t H_ACTIVE = ActiveCarOnLane(Horizontal);

	  switch(current){

	  case STATE_GREEN:
	      memcpy(Shownstate, H_Green, 3);
	      xTaskNotify(ActuatorTaskHandle, SHOW_EVT, eSetBits);

	      // is there is a horizotal car star the redmax timer of there is any and if it leave reset the timer
	      if (V_ACTIVE == 1 && IsActive(RedMaxHandle) == 0) {
	          osTimerStart(RedMaxHandle, pdMS_TO_TICKS(redDelayMax));
	          osTimerStop(GreenHandle);
	      }
	      else if (V_ACTIVE == 0) {
	          osTimerStop(RedMaxHandle);
	      }

	      // diffrent possble lane combinations
	      if ((H_ACTIVE == 0) && (V_ACTIVE == 1)) {  // imeadiate state change stop all the timers
	          osTimerStop(RedMaxHandle);
	          osTimerStop(GreenHandle);
	          current = STATE_ORANGE_G2R;
	      }
	      else if ((H_ACTIVE == 1) && (V_ACTIVE == 0)) { // stay green forever so reset all the timers so it never triggers a state change
	          osTimerStop(RedMaxHandle);
	          osTimerStop(GreenHandle);
	      }
	      else if ((H_ACTIVE == 0) && (V_ACTIVE == 0) && !IsActive(GreenHandle)) {// all empty start the greentimer
	          osTimerStart(GreenHandle, pdMS_TO_TICKS(greenDelay));
	      }

	      // Pedestrian button press immediately goes to pending
	      if (PL1_switch_var == 1) {
	          PL1_switch_var = 0;
	          osTimerStop(RedMaxHandle);
	          osTimerStop(GreenHandle);
	          current = STATE_PENDING;
	      }

	      // Check timer/event notifications without blocking
	      BaseType_t notified = xTaskNotifyWait(0, 0xFFFFFFFF, &events, pdMS_TO_TICKS(10));
	      if (notified == pdTRUE) {

	          if (events & GREEN_EVT) {
	              current = STATE_ORANGE_G2R;
	          }
	          else if (events & REDMAX_EVT) {
	              current = STATE_ORANGE_G2R;
	          }

	      }
	  break;



	  case STATE_ORANGE_G2R:


	      memcpy(Shownstate, H_Orange_G2R, 3);
	      xTaskNotify(ActuatorTaskHandle, SHOW_EVT, eSetBits);


	      if (!IsActive(OrangeHandle)) {
	          osTimerStart(OrangeHandle, pdMS_TO_TICKS(orangeDelay));
	      }

	      xTaskNotifyWait(0, 0xFFFFFFFF, &events, osWaitForever);

	      if (events & ORANGE_EVT) {
	          current = STATE_RED;               // Transition to RED

	      }
	  break;




	  case STATE_RED:
          // Wait until horizontal finishes its green cycle

		  osSemaphoreRelease(ImGreenNowHandle);
          current = STATE_ORANGE_R2G;
		  osThreadYield();
          osSemaphoreAcquire(ImGreenNowHandle, osWaitForever);
          // Now vertical can do its own R2G logic


		  break;

	  case STATE_ORANGE_R2G:

	      memcpy(Shownstate, H_Orange_R2G, 3);
	      xTaskNotify(ActuatorTaskHandle, SHOW_EVT, eSetBits);


	      if (!IsActive(OrangeHandle)) {
	          osTimerStart(OrangeHandle, pdMS_TO_TICKS(orangeDelay));
	      }

	      notified = xTaskNotifyWait(0, 0xFFFFFFFF, &events, pdMS_TO_TICKS(10));
	      if (notified == pdTRUE) {

	          if (events & ORANGE_EVT) {
	        	  current = STATE_GREEN;
	          }

	          }

		  break;

	  case STATE_PENDING:


		  toggle_bit =  TOGGLE_EVT_1;
		   if (!IsActive(ToggleTimerHandle)) {
		       osTimerStart(ToggleTimerHandle, pdMS_TO_TICKS(toggleFreq));
		    }

	      if (!IsActive(PedestrianHandle)) {
	          osTimerStart(PedestrianHandle, pdMS_TO_TICKS(pedestrianDelay));
	      }
	      xTaskNotifyWait(0, 0xFFFFFFFF, &events, osWaitForever);

	      if (events & PED_EVT) {
	    	  memcpy(Shownstate, H_Orange_R2G, 3);
		      if (!IsActive(OrangeHandle)) {
		          osTimerStart(OrangeHandle, pdMS_TO_TICKS(orangeDelay));

		      }
		  xTaskNotifyWait(0, 0xFFFFFFFF, &events, osWaitForever);
	      if (events & ORANGE_EVT) {
	    	  current = STATE_PED_WALK;

		      }
	      }
	      break;
	  case STATE_PED_WALK:

		  osTimerStop(ToggleTimerHandle);
    	  memcpy(Shownstate, V_Green, 3);
    	  xTaskNotify(ActuatorTaskHandle,SHOW_EVT,eSetBits);


    	  if (!IsActive(WalkingHandle)) {
		       osTimerStart(WalkingHandle, pdMS_TO_TICKS(walkingDelay));
		  }
    	  xTaskNotifyWait(0, 0xFFFFFFFF, &events, osWaitForever);
    	  if (events & WALK_EVT) {
		    	current = STATE_ORANGE_R2G;

    	  }



		  break;
	  }
  }
  /* USER CODE END StartHorizontalTask */
}

/* USER CODE BEGIN Header_StartActuatorTask */
/**
* @brief Function implementing the ActuatorTask thread.
* @param argument: Not used
* @retval None
*/
/* USER CODE END Header_StartActuatorTask */
void StartActuatorTask(void *argument)
{
  /* USER CODE BEGIN StartActuatorTask */
	uint32_t events; // handles the "calls"
  /* Infinite loop */
  for(;;)
  {
	  xTaskNotifyWait(0,TOGGLE_EVT_1 | TOGGLE_EVT_2 | SHOW_EVT, &events, osWaitForever);


	  if (events & TOGGLE_EVT_1){
		  Shownstate[2] ^= indicatorbit;

	  }
	  if (events & TOGGLE_EVT_2){
		  Shownstate[1] ^= indicatorbit;
	  }
	  if (events & SHOW_EVT){

	  }
      osMutexAcquire(UART_MutexHandle, osWaitForever);
      SPIshow_state(Shownstate);
      osMutexRelease(UART_MutexHandle);
  }
  /* USER CODE END StartActuatorTask */
}

/* GreenDone function */
void GreenDone(void *argument)
{
  /* USER CODE BEGIN GreenDone */
	xTaskNotify(VertialTaskHandle,GREEN_EVT,eSetBits);
	xTaskNotify(HorizontalTaskHandle,GREEN_EVT,eSetBits);
  /* USER CODE END GreenDone */
}

/* OrangeDone function */
void OrangeDone(void *argument)
{
  /* USER CODE BEGIN OrangeDone */
	xTaskNotify(VertialTaskHandle, ORANGE_EVT, eSetBits);
	xTaskNotify(HorizontalTaskHandle, ORANGE_EVT, eSetBits);
  /* USER CODE END OrangeDone */
}

/* RedMaxDone function */
void RedMaxDone(void *argument)
{
  /* USER CODE BEGIN RedMaxDone */
	xTaskNotify(VertialTaskHandle,REDMAX_EVT,eSetBits);
	xTaskNotify(HorizontalTaskHandle,REDMAX_EVT,eSetBits);

  /* USER CODE END RedMaxDone */
}

/* WalkingDone function */
void WalkingDone(void *argument)
{
  /* USER CODE BEGIN WalkingDone */
	xTaskNotify(VertialTaskHandle,WALK_EVT,eSetBits);
	xTaskNotify(HorizontalTaskHandle,WALK_EVT,eSetBits);
  /* USER CODE END WalkingDone */
}

/* PedestrianDone function */
void PedestrianDone(void *argument)
{
  /* USER CODE BEGIN PedestrianDone */
	xTaskNotify(VertialTaskHandle,PED_EVT,eSetBits);
	xTaskNotify(HorizontalTaskHandle,PED_EVT,eSetBits);

  /* USER CODE END PedestrianDone */
}

/* ToggleTimerCallback function */
void ToggleTimerCallback(void *argument)
{
  /* USER CODE BEGIN ToggleTimerCallback */
	uint32_t bit_to_toggle = *(uint32_t *)argument;
	xTaskNotify(ActuatorTaskHandle,bit_to_toggle,eSetBits);
  /* USER CODE END ToggleTimerCallback */
}

/* Private application code --------------------------------------------------*/
/* USER CODE BEGIN Application */

uint8_t IsActive(osThreadId_t timer){
	if(osTimerIsRunning(timer) == pdTRUE){
		return 1;
	}
	return 0;
}

void Restbutton(){
	PL1_switch_var = 0;
	PL2_switch_var = 0;

}

/* USER CODE END Application */

