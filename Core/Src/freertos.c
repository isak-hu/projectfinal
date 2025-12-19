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
#include "UARThandler.h"


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


/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */
#define TOGGLE_EVT_1     (1 << 0)
#define TOGGLE_EVT_2     (1 << 1)
#define REDMAX_EVT    	 (1 << 2)
#define V_ORANGE_EVT     (1 << 3)
#define H_ORANGE_EVT     (1 << 7)
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
static uint32_t toggle_evt_value;/* USER CODE END Variables */

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
/* Definitions for V_Orange */
osTimerId_t V_OrangeHandle;
const osTimerAttr_t V_Orange_attributes = {
  .name = "V_Orange"
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
/* Definitions for H_Orange */
osTimerId_t H_OrangeHandle;
const osTimerAttr_t H_Orange_attributes = {
  .name = "H_Orange"
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
void V_OrangeDone(void *argument);
void RedMaxDone(void *argument);
void WalkingDone(void *argument);
void PedestrianDone(void *argument);
void ToggleTimerCallback(void *argument);
void H_OrangeDone(void *argument);

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
  ImGreenNowHandle = osSemaphoreNew(1, 0, &ImGreenNow_attributes);

  /* USER CODE BEGIN RTOS_SEMAPHORES */
  /* add semaphores, ... */
  /* USER CODE END RTOS_SEMAPHORES */

  /* Create the timer(s) */
  /* creation of Green */
  GreenHandle = osTimerNew(GreenDone, osTimerOnce, NULL, &Green_attributes);

  /* creation of V_Orange */
  V_OrangeHandle = osTimerNew(V_OrangeDone, osTimerOnce, NULL, &V_Orange_attributes);

  /* creation of RedMax */
  RedMaxHandle = osTimerNew(RedMaxDone, osTimerOnce, NULL, &RedMax_attributes);

  /* creation of Walking */
  WalkingHandle = osTimerNew(WalkingDone, osTimerOnce, NULL, &Walking_attributes);

  /* creation of Pedestrian */
  PedestrianHandle = osTimerNew(PedestrianDone, osTimerOnce, NULL, &Pedestrian_attributes);

  /* creation of ToggleTimer */
  ToggleTimerHandle = osTimerNew(ToggleTimerCallback, osTimerPeriodic, &toggle_evt_value, &ToggleTimer_attributes);


  /* creation of H_Orange */
  H_OrangeHandle = osTimerNew(H_OrangeDone, osTimerOnce, NULL, &H_Orange_attributes);

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
	LightState Vcurrent = STATE_GREEN;
	//take the semaphore at starup
	osSemaphoreRelease(ImGreenNowHandle);
	osSemaphoreAcquire(ImGreenNowHandle, osWaitForever);

 uint32_t events;


  /* Infinite loop */
  for(;;)
  {
	  uint8_t V_ACTIVE = ActiveCarOnLane(Vertical);
	  uint8_t H_ACTIVE = ActiveCarOnLane(Horizontal);

	  switch(Vcurrent){

	  case STATE_GREEN:
		  //show the lights that represent the current state on the board
	      memcpy(Shownstate, V_Green, 3);
	      xTaskNotify(ActuatorTaskHandle, SHOW_EVT, eSetBits);

	      // if horizotal car arrives start the redmax timer
	      if (H_ACTIVE == 1 && IsActive(RedMaxHandle) == 0) {
	          osTimerStart(RedMaxHandle, pdMS_TO_TICKS(redDelayMax - 2* orangeDelay));
	          osTimerStop(GreenHandle);
	      }
	      // if horizotal leaves arrives reset the redmax timer
	      else if (H_ACTIVE == 0) {
	          osTimerStop(RedMaxHandle);
	      }

		  // if current lane is emty and horiontal car is waiting immeadaty switch horizontal
	      if ((V_ACTIVE == 0) && (H_ACTIVE == 1)) {
	          osTimerStop(RedMaxHandle);
	          osTimerStop(GreenHandle);
	          Vcurrent = STATE_ORANGE_G2R;
	      }
	      // if no horizontal car is waiting reset the all timers to halt a transion from happening
	      else if ((V_ACTIVE == 1) && (H_ACTIVE == 0)) {
	          osTimerStop(GreenHandle);
	      }
	      // if there is no cars what so ever start greenDelay
	      else if ((V_ACTIVE == 0) && (H_ACTIVE == 0) && !IsActive(GreenHandle)) {
	          osTimerStart(GreenHandle, pdMS_TO_TICKS(greenDelay - 2* orangeDelay));
	      }

	      // Pedestrian button press immediately go to STATE_PENDING
	      if (PL2_switch_var == 1) {
	          PL2_switch_var = 0;
	          osTimerStop(RedMaxHandle);
	          osTimerStop(GreenHandle);
	          Vcurrent = STATE_PENDING;
	      }

	      // Check timer/event notifications without blocking
	      BaseType_t notified = xTaskNotifyWait(0, 0xFFFFFFFF, &events, pdMS_TO_TICKS(10));
	      if (notified == pdTRUE) {

	          if (events & GREEN_EVT) {
	              Vcurrent = STATE_ORANGE_G2R;
	          }
	          else if (events & REDMAX_EVT) {
	              Vcurrent = STATE_ORANGE_G2R;
	          }

	      }
	  break;



	  case STATE_ORANGE_G2R:
		  //show the lights that represent the current state on the board
	      memcpy(Shownstate, V_Orange_G2R, 3);
	      xTaskNotify(ActuatorTaskHandle, SHOW_EVT, eSetBits);

	      //start timer
	      if (!IsActive(V_OrangeHandle)) {
	          osTimerStart(V_OrangeHandle, pdMS_TO_TICKS(orangeDelay));
	      }
	      //wait for timer to finish
	      xTaskNotifyWait(0, 0xFFFFFFFF, &events, osWaitForever);
	      if (events & V_ORANGE_EVT) {
	    	  osTimerStop(V_OrangeHandle);
	          Vcurrent = STATE_RED;
	      }
	  break;




	  case STATE_RED:
		  //relase the semaphore so horizontal task can take over
		  osSemaphoreRelease(ImGreenNowHandle);
          Vcurrent = STATE_ORANGE_R2G;
		  osThreadYield();
		  // wait for horizontal task to finish to resume the statemachine
          osSemaphoreAcquire(ImGreenNowHandle, osWaitForever);

		  break;
	  //this is the state it returns to when coming from horizontal task
	  case STATE_ORANGE_R2G:

		  xTaskNotifyStateClear(NULL);
		  xTaskNotifyWait(0, 0xFFFFFFFF, &events, 0);

          memcpy(Shownstate, V_Orange_R2G, 3);
          xTaskNotify(ActuatorTaskHandle, SHOW_EVT, eSetBits);
	      if (!IsActive(V_OrangeHandle)) {
	          osTimerStart(V_OrangeHandle, pdMS_TO_TICKS(orangeDelay));
	      }

	      xTaskNotifyWait(0, 0xFFFFFFFF, &events, osWaitForever);

	      if (events & V_ORANGE_EVT) {
	    	  osTimerStop(V_OrangeHandle);
	          Vcurrent = STATE_GREEN;
	      }


		  break;


		  //this state is basically all the trassion states combined into one, It ignors all the other timers and focusees on the pedestrian timers
		  //this is due to that we pedestrian have the prioity
	  case STATE_PENDING:


		  toggle_evt_value =  TOGGLE_EVT_2;
		   if (!IsActive(ToggleTimerHandle)) {
		       osTimerStart(ToggleTimerHandle, pdMS_TO_TICKS(toggleFreq));
		    }

	      if (!IsActive(PedestrianHandle)) {
	          osTimerStart(PedestrianHandle, pdMS_TO_TICKS(pedestrianDelay - 2 * orangeDelay));
	      }
	      xTaskNotifyWait(0, 0xFFFFFFFF, &events, osWaitForever);

	      if (events & PED_EVT) {
	    	  memcpy(Shownstate, V_Orange_G2R, 3);
		      if (!IsActive(V_OrangeHandle)) {
		          osTimerStart(V_OrangeHandle, pdMS_TO_TICKS(orangeDelay));

		      }
	      }
		  xTaskNotifyWait(0, 0xFFFFFFFF, &events, osWaitForever);
	      if (events & V_ORANGE_EVT) {
	    	  memcpy(Shownstate, H_Orange_R2G, 3);
		      if (!IsActive(V_OrangeHandle)) {
		          osTimerStart(V_OrangeHandle, pdMS_TO_TICKS(orangeDelay));

		      }

		      }
		  xTaskNotifyWait(0, 0xFFFFFFFF, &events, osWaitForever);
	      if (events & V_ORANGE_EVT) {
	    	  osTimerStop(V_OrangeHandle);
	    	  Vcurrent = STATE_PED_WALK;

		      }


	      break;

	      //eqivalent to the horzontal STATE_GREEN but only stays there for walkingDelay ms
	  case STATE_PED_WALK:

		  osTimerStop(ToggleTimerHandle);
    	  memcpy(Shownstate, H_Green, 3);
    	  xTaskNotify(ActuatorTaskHandle,SHOW_EVT,eSetBits);


    	  if (!IsActive(WalkingHandle)) {
		       osTimerStart(WalkingHandle, pdMS_TO_TICKS(walkingDelay));
		  }
    	  xTaskNotifyWait(0, 0xFFFFFFFF, &events, osWaitForever);
    	  if (events & WALK_EVT) {
        	  memcpy(Shownstate, H_Orange_G2R, 3);
        	  xTaskNotify(ActuatorTaskHandle,SHOW_EVT,eSetBits);
		      if (!IsActive(V_OrangeHandle)) {
		          osTimerStart(V_OrangeHandle, pdMS_TO_TICKS(orangeDelay));

		      }
    	  }
		  xTaskNotifyWait(0, 0xFFFFFFFF, &events, osWaitForever);
	      if (events & V_ORANGE_EVT) {
	    	  osTimerStop(V_OrangeHandle);
	    	  Vcurrent = STATE_ORANGE_R2G;

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

	//first time aquireing semaphore will be after vertial state has reached red state that means we should pick up from STATE_ORANGE_R2G
	osSemaphoreAcquire(ImGreenNowHandle, osWaitForever);
    LightState Hcurrent = STATE_ORANGE_R2G;
	uint32_t events;// Start with its own R2G
  for(;;)
  {
   // the following state machine is jsut a mirrored version of the vertal statemachine so i wont be commenting
	  uint8_t V_ACTIVE = ActiveCarOnLane(Vertical);
	  uint8_t H_ACTIVE = ActiveCarOnLane(Horizontal);

	  switch(Hcurrent){

	  case STATE_GREEN:
	      memcpy(Shownstate, H_Green, 3);
	      xTaskNotify(ActuatorTaskHandle, SHOW_EVT, eSetBits);


	      if (V_ACTIVE == 1 && IsActive(RedMaxHandle) == 0) {
	          osTimerStart(RedMaxHandle, pdMS_TO_TICKS(redDelayMax - 2* orangeDelay));
	          osTimerStop(GreenHandle);
	      }
	      else if (V_ACTIVE == 0) {
	          osTimerStop(RedMaxHandle);
	      }


	      if ((H_ACTIVE == 0) && (V_ACTIVE == 1)) {
	          osTimerStop(RedMaxHandle);
	          osTimerStop(GreenHandle);
	          Hcurrent = STATE_ORANGE_G2R;
	      }
	      else if ((H_ACTIVE == 1) && (V_ACTIVE == 0)) {
	          osTimerStop(RedMaxHandle);
	          osTimerStop(GreenHandle);
	      }
	      else if ((H_ACTIVE == 0) && (V_ACTIVE == 0) && !IsActive(GreenHandle)) {
	          osTimerStart(GreenHandle, pdMS_TO_TICKS(greenDelay - 2* orangeDelay));
	      }


	      if (PL1_switch_var == 1) {
	          PL1_switch_var = 0;
	          osTimerStop(RedMaxHandle);
	          osTimerStop(GreenHandle);
	          Hcurrent = STATE_PENDING;
	      }


	      BaseType_t notified = xTaskNotifyWait(0, 0xFFFFFFFF, &events, pdMS_TO_TICKS(10));
	      if (notified == pdTRUE) {

	          if (events & GREEN_EVT) {
	              Hcurrent = STATE_ORANGE_G2R;
	          }
	          else if (events & REDMAX_EVT) {
	              Hcurrent = STATE_ORANGE_G2R;
	          }

	      }
	  break;



	  case STATE_ORANGE_G2R:


	      memcpy(Shownstate, H_Orange_G2R, 3);
	      xTaskNotify(ActuatorTaskHandle, SHOW_EVT, eSetBits);


	      if (!IsActive(H_OrangeHandle)) {
	          osTimerStart(H_OrangeHandle, pdMS_TO_TICKS(orangeDelay));
	      }

	      xTaskNotifyWait(0, 0xFFFFFFFF, &events, osWaitForever);

	      if (events & H_ORANGE_EVT) {
	    	  osTimerStop(H_OrangeHandle);
	          Hcurrent = STATE_RED;               // Transition to RED

	      }
	  break;




	  case STATE_RED:


		  osSemaphoreRelease(ImGreenNowHandle);

          Hcurrent = STATE_ORANGE_R2G;
		  osThreadYield();
          osSemaphoreAcquire(ImGreenNowHandle, osWaitForever);



		  break;

	  case STATE_ORANGE_R2G:

		  xTaskNotifyStateClear(NULL);
		  xTaskNotifyWait(0, 0xFFFFFFFF, &events, 0);

	      memcpy(Shownstate, H_Orange_R2G, 3);
	      xTaskNotify(ActuatorTaskHandle, SHOW_EVT, eSetBits);


	      if (!IsActive(H_OrangeHandle)) {
	          osTimerStart(H_OrangeHandle, pdMS_TO_TICKS(orangeDelay));
	      }

	      xTaskNotifyWait(0, 0xFFFFFFFF, &events, osWaitForever);

	      if (events & H_ORANGE_EVT) {
	    	  osTimerStop(H_OrangeHandle);
	          Hcurrent = STATE_GREEN;

	      }
		  break;

	  case STATE_PENDING:


		  toggle_evt_value =  TOGGLE_EVT_1;
		   if (!IsActive(ToggleTimerHandle)) {
		       osTimerStart(ToggleTimerHandle, pdMS_TO_TICKS(toggleFreq));
		    }

	      if (!IsActive(PedestrianHandle)) {
	          osTimerStart(PedestrianHandle, pdMS_TO_TICKS(pedestrianDelay - 2 * orangeDelay));
	      }
	      xTaskNotifyWait(0, 0xFFFFFFFF, &events, osWaitForever);

	      if (events & PED_EVT) {
	    	  memcpy(Shownstate, H_Orange_G2R, 3);
		      if (!IsActive(H_OrangeHandle)) {
		          osTimerStart(H_OrangeHandle, pdMS_TO_TICKS(orangeDelay));

		      }
	      }
		  xTaskNotifyWait(0, 0xFFFFFFFF, &events, osWaitForever);
	      if (events & H_ORANGE_EVT) {
	    	  memcpy(Shownstate, V_Orange_R2G, 3);
		      if (!IsActive(H_OrangeHandle)) {
		          osTimerStart(H_OrangeHandle, pdMS_TO_TICKS(orangeDelay));

		      }

		      }
		  xTaskNotifyWait(0, 0xFFFFFFFF, &events, osWaitForever);
	      if (events & H_ORANGE_EVT) {
	    	  osTimerStop(H_OrangeHandle);
	    	  Hcurrent = STATE_PED_WALK;

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
        	  memcpy(Shownstate, V_Orange_G2R, 3);
        	  xTaskNotify(ActuatorTaskHandle,SHOW_EVT,eSetBits);
		      if (!IsActive(H_OrangeHandle)) {
		          osTimerStart(H_OrangeHandle, pdMS_TO_TICKS(orangeDelay));

		      }
    	  }
		  xTaskNotifyWait(0, 0xFFFFFFFF, &events, osWaitForever);
	      if (events & H_ORANGE_EVT) {
	    	  osTimerStop(H_OrangeHandle);
	    	  Hcurrent = STATE_ORANGE_R2G;

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

// this task has sole possession over the SPI comunication


void StartActuatorTask(void *argument)
{
  /* USER CODE BEGIN StartActuatorTask */
	uint32_t events; // handles the "calls"
  /* Infinite loop */
  for(;;)
  {
	  xTaskNotifyWait(0,TOGGLE_EVT_1 | TOGGLE_EVT_2 | SHOW_EVT, &events, osWaitForever);
	  //responsible for the toggling of PL1
	  if (events & TOGGLE_EVT_1){
		  Shownstate[2] ^= indicatorbit;
	  }
	  //responsible for the toggling of PL2
	  if (events & TOGGLE_EVT_2){
		  Shownstate[1] ^= indicatorbit;
	  }
	  // just falls trough so
	  if (events & SHOW_EVT){

	  }

      SPIshow_state(Shownstate);

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

/* V_OrangeDone function */
void V_OrangeDone(void *argument)
{
  /* USER CODE BEGIN V_OrangeDone */
	xTaskNotify(VertialTaskHandle,V_ORANGE_EVT,eSetBits);
  /* USER CODE END V_OrangeDone */
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

/* H_OrangeDone function */
void H_OrangeDone(void *argument)
{
  /* USER CODE BEGIN H_OrangeDone */
	xTaskNotify(HorizontalTaskHandle,H_ORANGE_EVT,eSetBits);
  /* USER CODE END H_OrangeDone */
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

