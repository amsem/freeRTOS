#include <stdio.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/semphr.h"

// Task 1
void Task1(void *pvParameters)
{
    for (int i = 0; i < 300; ++i)
    {
        printf("Hello from Task 1\n");
        vTaskDelay(pdMS_TO_TICKS(10)); // A short delay between each print
    }
    vTaskDelete(NULL); // Task completes
}

// Task 2
void Task2(void *pvParameters)
{
    for (int i = 0; i < 10; ++i)
    {
        printf("Hello from Task 2\n");
        vTaskDelay(pdMS_TO_TICKS(1000)); // Every second
    }
    vTaskDelete(NULL); // Task completes
}

// Task 3
void Task3(void *pvParameters)
{
    vTaskDelay(pdMS_TO_TICKS(1000)); // Wait for 1 second before starting
    for (int i = 0; i < 10; ++i)
    {
        printf("Hello from Task 3\n");
        vTaskDelay(pdMS_TO_TICKS(100)); // A short delay between each print
    }
    vTaskDelete(NULL); // Task completes
}

void app_main()
{
    xTaskCreatePinnedToCore(Task1, "Task1", 4096, NULL, 1, NULL, 0); // Priority 1, Core 0
    xTaskCreatePinnedToCore(Task2, "Task2", 4096, NULL, 2, NULL, 0); // Priority 2, Core 0
    xTaskCreatePinnedToCore(Task3, "Task3", 4096, NULL, 3, NULL, 1); // Priority 3, Core 1
}
