#include <freertos/FreeRTOS.h>
#include <freertos/task.h>
#include <stdio.h>

#define TASK1_CORE_ID         0
#define TASK2_CORE_ID         1
#define TASK3_CORE_ID         1

void vTask1(void *pvParameters)
{
    for(;;)
    {
        printf("Task 1 running on core %d\n", xPortGetCoreID());
        vTaskDelay(pdMS_TO_TICKS(1000));
    }
}

void vTask2(void *pvParameters)
{
    for(;;)
    {
        printf("Task 2 running on core %d\n", xPortGetCoreID());
        vTaskDelay(pdMS_TO_TICKS(1000));
    }
}

void vTask3(void *pvParameters)
{
    for(;;)
    {
        printf("Task 3 running on core %d\n", xPortGetCoreID());
        vTaskDelay(pdMS_TO_TICKS(1000));
    }
}

void app_main()
{
    /* Crear tareas asignadas a núcleos específicos */
    xTaskCreatePinnedToCore(
        vTask1,           // Función de la tarea
        "Task1",          // Nombre
        2048,             // Stack size
        NULL,             // Parámetros
        1,                // Prioridad
        NULL,             // Handle
        TASK1_CORE_ID     // Núcleo asignado
    );

    xTaskCreatePinnedToCore(
        vTask2,
        "Task2",
        2048,
        NULL,
        1,
        NULL,
        TASK2_CORE_ID
    );

    xTaskCreatePinnedToCore(
        vTask3,
        "Task3",
        2048,
        NULL,
        1,
        NULL,
        TASK3_CORE_ID
    );

    printf("Tasks created and pinned to cores\n");
}