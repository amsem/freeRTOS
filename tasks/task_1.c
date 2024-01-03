#include <stdio.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/semphr.h"

#define TASK1_PRIORITY 1
#define TASK2_PRIORITY 2

TaskHandle_t Task1_Handle, Task2_Handle;

void Task1(void *pvParameters)
{
    while (1)
    {
        printf("Bonjour depuis la tâche 1\n");
        vTaskDelay(pdMS_TO_TICKS(1000)); // Delay for 1000 milliseconds
    }
}

void Task2(void *pvParameters)
{
    while (1)
    {
        printf("Bonjour depuis la tâche 2\n");
        vTaskDelay(pdMS_TO_TICKS(1000)); // Delay for 1000 milliseconds
    }
}

void app_main()
{
    xTaskCreatePinnedToCore(Task1, "Task1", 4096, NULL, TASK1_PRIORITY, &Task1_Handle, 0);
    xTaskCreatePinnedToCore(Task2, "Task2", 4096, NULL, TASK2_PRIORITY, &Task2_Handle, 0);
}
