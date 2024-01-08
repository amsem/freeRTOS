#include <stdio.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/queue.h"
#include "esp_event.h"

ESP_EVENT_DECLARE_BASE(SENSOR_EVENTS);
ESP_EVENT_DEFINE_BASE(SENSOR_EVENTS);

enum
{
    TEMP_CAPTURE_EVENT,
    HUM_CAPTURE_EVENT
};

esp_event_loop_args_t event_loop_args = {
    .queue_size = 5,
    .task_name = "Sensor_Event_Loop_Task",
    .task_priority = 7,
    .task_stack_size = 2400,
    .task_core_id = 1};

esp_event_loop_handle_t sensor_event_loop;

void temp_handler(void *handler_args, esp_event_base_t base, int32_t id, void *event_data)
{
    printf("Temperature Captured: %f\n", *(float *)event_data);
}

void hum_handler(void *handler_args, esp_event_base_t base, int32_t id, void *event_data)
{
    printf("Humidity Captured: %f\n", *(float *)event_data);
}

void TempTask(void *pvParameter)
{
    while (1)
    {
        // Simuler la capture de la température
        float temp_value = (float)(rand() % 50); // Valeur aléatoire entre 0 et 50

        // Poster l'événement avec la valeur capturée
        esp_event_post_to(sensor_event_loop, SENSOR_EVENTS, TEMP_CAPTURE_EVENT, &temp_value, sizeof(float), portMAX_DELAY);

        // Attendre avant la prochaine capture
        vTaskDelay(pdMS_TO_TICKS(5000)); // par exemple, une capture toutes les 5 secondes
    }
}

void HumTask(void *pvParameter)
{
    while (1)
    {
        // Simuler la capture de l'humidité
        float hum_value = (float)(rand() % 100); // Valeur aléatoire entre 0 et 100

        // Poster l'événement avec la valeur capturée
        esp_event_post_to(sensor_event_loop, SENSOR_EVENTS, HUM_CAPTURE_EVENT, &hum_value, sizeof(float), portMAX_DELAY);

        // Attendre avant la prochaine capture
        vTaskDelay(pdMS_TO_TICKS(6000)); // par exemple, une capture toutes les 6 secondes
    }
}

void app_main()
{
    // Créer l'Event Loop
    esp_event_loop_create(&event_loop_args, &sensor_event_loop);

    // Enregistrement des fonctions de réponse
    esp_event_handler_register_with(sensor_event_loop, SENSOR_EVENTS, TEMP_CAPTURE_EVENT, temp_handler, NULL);
    esp_event_handler_register_with(sensor_event_loop, SENSOR_EVENTS, HUM_CAPTURE_EVENT, hum_handler, NULL);

    // Création des tâches de capture
    xTaskCreate(&TempTask, "TempTask", 2048, NULL, 5, NULL);
    xTaskCreate(&HumTask, "HumTask", 2048, NULL, 5, NULL);
}
