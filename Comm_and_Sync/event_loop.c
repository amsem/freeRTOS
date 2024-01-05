#include <stdio.h>
#include <freertos/FreeRTOS.h>
#include <freertos/task.h>
#include <freertos/queue.h>
#include <freertos/event_groups.h>

// Event families
#define TEMP_EVENT 0x01
#define HUM_EVENT 0x02

// Event list for each sensor
#define TEMP_CAPTURE_EVENT (1 << 0)
#define HUM_CAPTURE_EVENT (1 << 1)

// Event Loop
static EventGroupHandle_t eventGroup;

// Response functions
void TempSensorCallback(int tempValue)
{
    printf("TempSensorCallback: Temperature captured: %d\n", tempValue);
}

void HumSensorCallback(int humValue)
{
    printf("HumSensorCallback: Humidity captured: %d\n", humValue);
}

// Task for temperature capture
void TempTask(void *pvParameters)
{
    while (1)
    {
        // Simulate temperature capture
        int tempValue = 20; // Set temperature value to 20 degrees

        // Trigger the corresponding event
        xEventGroupSetBits(eventGroup, TEMP_CAPTURE_EVENT);

        // Wait for 1 second before the next capture
        vTaskDelay(1000 / portTICK_PERIOD_MS);
    }
}

// Task for humidity capture
void HumTask(void *pvParameters)
{
    while (1)
    {
        // Simulate humidity capture
        int humValue = 100; // Set humidity value to 100

        // Trigger the corresponding event
        xEventGroupSetBits(eventGroup, HUM_CAPTURE_EVENT);

        // Wait for 2 seconds before the next capture
        vTaskDelay(2000 / portTICK_PERIOD_MS);
    }
}

// Event Loop Task
void EventLoopTask(void *pvParameters)
{
    while (1)
    {
        EventBits_t bits = xEventGroupWaitBits(eventGroup, TEMP_CAPTURE_EVENT | HUM_CAPTURE_EVENT, pdTRUE, pdFALSE, portMAX_DELAY);

        if (bits & TEMP_CAPTURE_EVENT)
        {
            TempSensorCallback(/* Captured temperature value */);
        }

        if (bits & HUM_CAPTURE_EVENT)
        {
            HumSensorCallback(/* Captured humidity value */);
        }
    }
}

void app_main()
{
    // Initialize the Event Group
    eventGroup = xEventGroupCreate();

    // Create and launch tasks for temperature and humidity capture
    xTaskCreate(TempTask, "TempTask", 2048, NULL, 5, NULL);
    xTaskCreate(HumTask, "HumTask", 2048, NULL, 5, NULL);

    // Create and launch the Event Loop Task
    xTaskCreate(EventLoopTask, "EventLoopTask", 2400, NULL, 7, NULL);
}
