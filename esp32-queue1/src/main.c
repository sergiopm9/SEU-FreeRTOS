#include <freertos/FreeRTOS.h>
#include <freertos/task.h>
#include <freertos/queue.h>
#include "esp_log.h"
#include <stdlib.h>

#define TASK_STACK_SIZE                 2048
#define TASK_ACQ_PRIORITY               5
#define TASK_DISP_PRIORITY              3

#define TEMPERATURE_QUEUE_LENGTH        10
#define TEMPERATURE_ACQ_RATE_MS         1000

static const char* TAG = "TempQueue";

void TemperatureAcquisition(void * queue);
void TemperatureDisplay(void * queue);

void app_main()
{
    QueueHandle_t xTempQueue = NULL;
    TaskHandle_t xHandleAcq = NULL;
    TaskHandle_t xHandleDisp = NULL;    
    
    xTempQueue = xQueueCreate(TEMPERATURE_QUEUE_LENGTH, sizeof(int));
    if (xTempQueue == NULL)
    {
        ESP_LOGE(TAG, "Error creating queue. Restarting...");
        exit(EXIT_FAILURE);                
    }

    xTaskCreate(TemperatureAcquisition, "Task_Acq", TASK_STACK_SIZE, (void*)xTempQueue, TASK_ACQ_PRIORITY, &xHandleAcq);
    configASSERT( xHandleAcq );
    ESP_LOGI(TAG, "[app_main] Task_Acq created.");

    xTaskCreate(TemperatureDisplay, "Task_Disp", TASK_STACK_SIZE, (void*)xTempQueue, TASK_DISP_PRIORITY, &xHandleDisp);
    configASSERT( xHandleDisp );
    ESP_LOGI(TAG, "[app_main] Task_Disp created.");

    /* Enter suspend state forever */
    vTaskSuspend(NULL);

    if (xHandleAcq != NULL)
    {
        vTaskDelete(xHandleAcq);
    }

    if (xHandleDisp != NULL)
    {
        vTaskDelete(xHandleDisp);
    }
}

void TemperatureAcquisition(void * queue)
{
    TickType_t xLastWakeTime;

    QueueHandle_t xQueue = (QueueHandle_t)queue;

    for (;;)
    {
        xLastWakeTime = xTaskGetTickCount();

        /* Simulate temperature reading. Any vaule in the range [1-30] */
        int temp_reading = (rand() % 30) + 1;

        xQueueSendToBack(xQueue, &temp_reading, portMAX_DELAY);

        vTaskDelayUntil(&xLastWakeTime, TEMPERATURE_ACQ_RATE_MS / portTICK_PERIOD_MS);
    }
}

void TemperatureDisplay(void * queue)
{
    QueueHandle_t xQueue = (QueueHandle_t)queue;

    for (;;)
    {
        int received;

        BaseType_t xStatus = xQueueReceive(xQueue, &received, portMAX_DELAY);
        if (xStatus == pdPASS)
        {
            printf("Temperature measurement: %d°C\n", received);
        }
        else
        {
            ESP_LOGW(TAG, "Could not receive from the queue.");
        }
    }
}