#include <stdio.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/portmacro.h"

void Task1(void *pvParameters)
{
    while (1)
    {
        printf("Bonjour depuis la tâche 1 - Core ID: %d\n", xPortGetCoreID());
        vTaskDelay(pdMS_TO_TICKS(1000));
    }
}

void Task2(void *pvParameters)
{
    while (1)
    {
        printf("Bonjour depuis la tâche 2 - Core ID: %d\n", xPortGetCoreID());
        vTaskDelay(pdMS_TO_TICKS(1000));
    }
}

void app_main()
{
    xTaskCreatePinnedToCore(Task1, "Task1", 4096, NULL, 1, NULL, 1);
    xTaskCreatePinnedToCore(Task2, "Task2", 4096, NULL, 1, NULL, 0);
}
