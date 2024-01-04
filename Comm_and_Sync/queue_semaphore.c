#include <stdio.h>
#include <freertos/FreeRTOS.h>
#include <freertos/task.h>
#include <freertos/queue.h>
#include <freertos/semphr.h>

// Declaration of the RNG device address
#define DR_REG_RNG_BASE 0x3ff75144

// Structure to represent a record
typedef struct
{
    char type; // 'T' for temperature, 'H' for humidity, 'Q' for air quality
    int value; // Sensor value
} SensorData;

// Queue to store records
QueueHandle_t dataQueue;

// Semaphore to control access to the queue
SemaphoreHandle_t queueSemaphore;

// Producer task
void producerTask(void *pvParameters)
{
    while (1)
    {
        // Generate a random data
        char sensorType;
        int sensorValue;

        switch (READ_PERI_REG(DR_REG_RNG_BASE) % 3)
        {
        case 0:
            sensorType = 'T';
            break;
        case 1:
            sensorType = 'H';
            break;
        case 2:
            sensorType = 'Q';
            break;
        default:
            break;
        }

        sensorValue = READ_PERI_REG(DR_REG_RNG_BASE);

        // Create a SensorData structure
        SensorData data = {sensorType, sensorValue};

        // Wait for the queue to be available
        xSemaphoreTake(queueSemaphore, portMAX_DELAY);

        // Place the data in the queue
        xQueueSend(dataQueue, &data, 0);

        // Release the semaphore
        xSemaphoreGive(queueSemaphore);

        // Wait for a random delay up to 1 second
        vTaskDelay((READ_PERI_REG(DR_REG_RNG_BASE) % 1000) / portTICK_RATE_MS);
    }
}

// Consumer task
void consumerTask(void *pvParameters)
{
    while (1)
    {
        // Wait for the semaphore
        if (xSemaphoreTake(queueSemaphore, (READ_PERI_REG(DR_REG_RNG_BASE) % 3000) / portTICK_RATE_MS) == pdTRUE)
        {
            // Extract data from the queue
            SensorData data;
            if (xQueueReceive(dataQueue, &data, 0) == pdTRUE)
            {
                // Display or process the data
                printf("Type: %c, Value: %d\n", data.type, data.value);
            }
            else
            {
                // No data available, display a message
                printf("No data available. Please check that your sensors are functioning correctly.\n");
            }

            // Release the semaphore
            xSemaphoreGive(queueSemaphore);
        }
    }
}

void app_main()
{
    // Initialize the queue
    dataQueue = xQueueCreate(5, sizeof(SensorData));

    // Create the semaphore
    queueSemaphore = xSemaphoreCreateMutex();

    // Create producer and consumer tasks
    xTaskCreate(producerTask, "Producer", 2048, NULL, 5, NULL);
    xTaskCreate(consumerTask, "Consumer", 2048, NULL, 5, NULL);
}
