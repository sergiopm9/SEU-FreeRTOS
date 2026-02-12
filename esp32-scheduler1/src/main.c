#include <freertos/FreeRTOS.h>
#include <freertos/task.h>
#include <esp_task.h>
#include <math.h>
#include <stdio.h>

#define TASK1_PRIORITY        4
#define TASK2_PRIORITY        3
#define TASK3_PRIORITY        3
#define APP_MAIN_PRIORITY	    5
#define TASK_STACK_SIZE       2048   /* Words task stack size */

#define TASK_RUNNING_TIME_MS  5000   /* Time for tasks to run */

#define TASK1_PERIOD_MS       200    /* Period of Task1 in ms */
#define TASK2_PERIOD_MS       100    /* Period of Task2 in ms */
#define TASK3_PERIOD_MS       100    /* Period of Task3 in ms */

#define LOOP_COUNT_TASK_1     1000   /* Loop iterations for Task1 */
#define LOOP_COUNT_TASK_2     2000   /* Loop iterations for Task2 */
#define LOOP_COUNT_TASK_3     2000   /* Loop iterations for Task3 */

void vTask1(void * parameter);
void vTask2(void * parameter);
void vTask3(void * parameter);

void app_main() 
{
  TaskHandle_t xHandle1 = NULL;
  TaskHandle_t xHandle2 = NULL;
  TaskHandle_t xHandle3 = NULL;
  
  /* Set high priority for app_main */
  vTaskPrioritySet(NULL, APP_MAIN_PRIORITY);  
  
  /* Create a new task and add it to the list of tasks that are ready to run */
  xTaskCreate(
      vTask1,           /* Task function */
      "Task1",          /* Name of task; for human use */
      TASK_STACK_SIZE,  /* Stack size of task */
      NULL,             /* Parameter of the task */
      TASK1_PRIORITY,   /* Priority of the task */
      &xHandle1);       /* Task handle to keep track of created task */
  configASSERT(xHandle1);

  /* Create a new task and add it to the list of tasks that are ready to run */
  xTaskCreate(vTask2, "Task2", TASK_STACK_SIZE, NULL, TASK2_PRIORITY, &xHandle2); 
  configASSERT(xHandle2);

    /* Create a new task and add it to the list of tasks that are ready to run */
  xTaskCreate(vTask3, "Task3", TASK_STACK_SIZE, NULL, TASK3_PRIORITY, &xHandle3);
  configASSERT(xHandle3);  

  /* Wait TASK_RUNNING_TIME_MS ms */
  vTaskDelay(TASK_RUNNING_TIME_MS / portTICK_PERIOD_MS);
 
  /* Display end message */
  printf("\n\n=== End of execution ===\n");
  printf("Tasks executed for %d ms\n", TASK_RUNNING_TIME_MS);
  printf("Task1 (Priority %d, Period %dms): # symbols\n", TASK1_PRIORITY, TASK1_PERIOD_MS);
  printf("Task2 (Priority %d, Period %dms): - symbols\n", TASK2_PRIORITY, TASK2_PERIOD_MS);
  printf("Task3 (Priority %d, Period %dms): . symbols\n", TASK3_PRIORITY, TASK3_PERIOD_MS);
  
  /* Use the handles to delete the tasks. */
  if (xHandle1 != NULL)
  {
    vTaskDelete(xHandle1);
  }  
  if (xHandle2 != NULL)
  {
    vTaskDelete(xHandle2);
  }
  if (xHandle3 != NULL)
  {
    vTaskDelete(xHandle3);
  }
  
  /* Keep app_main alive to prevent scheduler issues */
  while(1) {
    vTaskDelay(pdMS_TO_TICKS(1000));
  }
}

/* Task 1 function 
 *  Periodic task: t = 200 ms
 */
void vTask1(void * parameter)
{
  double aux = acos(-1.0);  /* aux = PI */
  TickType_t xLastWakeTime = xTaskGetTickCount();
 
  /* loop forever */
  for(;;)
  {
    for (int i = 0; i < LOOP_COUNT_TASK_1; i++)
    {
        aux = sqrt(aux) * acos(-1.0) + aux * 0.1;
        if (i % 100 == 0)
        {
          printf("#");
          fflush(stdout);   
        }
    }    
    aux = acos(-1.0);
    vTaskDelayUntil(&xLastWakeTime, pdMS_TO_TICKS(TASK1_PERIOD_MS));
  }
}

/* Task 2 function 
 * Periodic task: t = 100 ms
 */
void vTask2(void * parameter)
{
  double aux = acos(-1.0);  /* aux = PI */
  TickType_t xLastWakeTime = xTaskGetTickCount();
 
  /* loop forever */
  for(;;)
  {
    for (int i = 0; i < LOOP_COUNT_TASK_2; i++)
    {
        aux = sqrt(aux) * acos(-1.0) + aux * 0.1;
        if (i % 100 == 0)
        {
          printf("-");
          fflush(stdout);    
        }
    }
    aux = acos(-1.0);
    vTaskDelayUntil(&xLastWakeTime, pdMS_TO_TICKS(TASK2_PERIOD_MS));
  }
}

/* Task 3 function 
 * Periodic task: t = 100 ms
 */
void vTask3(void * parameter)
{
  double aux = acos(-1.0);  /* aux = PI */
  TickType_t xLastWakeTime = xTaskGetTickCount();
 
  /* loop forever */
  for(;;)
  {
    for (int i = 0; i < LOOP_COUNT_TASK_3; i++)
    {
        aux = sqrt(aux) * acos(-1.0) + aux * 0.1;
        if (i % 100 == 0)
        {
          printf(".");
          fflush(stdout);    
        }
    }
    aux = acos(-1.0);
    vTaskDelayUntil(&xLastWakeTime, pdMS_TO_TICKS(TASK3_PERIOD_MS));
  }
}