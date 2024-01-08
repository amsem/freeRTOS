#include <freertos/FreeRTOS.h>
#include <freertos/task.h>
#include <freertos/semphr.h>
#include <freertos/timers.h>
#include <stdio.h>
#include <freertos/event_groups.h>

#define BTN1_PIN 22
#define BTN2_PIN 23

EventGroupHandle_t buttonEventGroup;

#define BTN1_BIT (1 << 0)
#define BTN2_BIT (1 << 1)

void BTN1_Task(void *param)
{
    while (1)
    {
        if (xEventGroupWaitBits(buttonEventGroup, BTN1_BIT, pdTRUE, pdFALSE, portMAX_DELAY))
        {
            printf("Appui sur BTN1 détecté\n");
        }
    }
}

void BTN2_Task(void *param)
{
    while (1)
    {
        if (xEventGroupWaitBits(buttonEventGroup, BTN2_BIT, pdTRUE, pdFALSE, portMAX_DELAY))
        {
            printf("Appui sur BTN2 détecté\n");
        }
    }
}

void BTN_SIM_Task(void *param)
{
    while (1)
    {
        if (xEventGroupWaitBits(buttonEventGroup, (BTN1_BIT | BTN2_BIT), pdTRUE, pdFALSE, portMAX_DELAY))
        {
            printf("Appui simultané sur BTN1 et BTN2 détecté\n");
        }
    }
}

void buttonTimerCallback(TimerHandle_t xTimer)
{
    // Simuler l'appui sur les boutons à intervalles réguliers
    xEventGroupSetBits(buttonEventGroup, BTN1_BIT);
    vTaskDelay(pdMS_TO_TICKS(50)); // Délai pour simuler un appui sur un autre bouton après 50ms
    xEventGroupSetBits(buttonEventGroup, BTN2_BIT);
}

void app_main()
{
    // Créer le groupe d'événements
    buttonEventGroup = xEventGroupCreate();

    // Créer les tâches
    xTaskCreate(&BTN1_Task, "BTN1_Task", 2048, NULL, 1, NULL);
    xTaskCreate(&BTN2_Task, "BTN2_Task", 2048, NULL, 1, NULL);
    xTaskCreate(&BTN_SIM_Task, "BTN_SIM_Task", 2048, NULL, 1, NULL);

    // Créer et démarrer la minuterie pour simuler l'appui sur les boutons
    TimerHandle_t buttonTimer = xTimerCreate("ButtonTimer", pdMS_TO_TICKS(1000), pdTRUE, 0, buttonTimerCallback);
    xTimerStart(buttonTimer, 0);
}
