#include <freertos/FreeRTOS.h>
#include <freertos/task.h>
#include <freertos/semphr.h>
#include <stdio.h>

// Mutex pour protéger l'accès à l'écran LCD
SemaphoreHandle_t lcdMutex;

// Fonction pour simuler l'écran LCD
void imprime(char *message)
{
    char c;
    uint8_t indice = 0;

    while ((c = message[indice++]) != '\0')
    {
        // Simuler l'envoi du caractère à l'écran LCD
        printf("%c", c);

        // Délai pour simuler l'affichage de chaque caractère
        for (int i = 0; i < 0xFFFF; i++)
            ;
    }
}

// Tâche 1
void task1(void *param)
{
    while (1)
    {
        xSemaphoreTake(lcdMutex, portMAX_DELAY); // Prend le Mutex pour accéder à l'écran LCD

        imprime("Message de la tâche 1\n");

        xSemaphoreGive(lcdMutex);      // Rend le Mutex
        vTaskDelay(pdMS_TO_TICKS(10)); // Attend 10 millisecondes
    }
}

// Tâche 2
void task2(void *param)
{
    while (1)
    {
        xSemaphoreTake(lcdMutex, portMAX_DELAY); // Prend le Mutex pour accéder à l'écran LCD

        imprime("Message de la tâche 2\n");

        xSemaphoreGive(lcdMutex);      // Rend le Mutex
        vTaskDelay(pdMS_TO_TICKS(10)); // Attend 10 millisecondes
    }
}

void app_main()
{
    // Crée le Mutex
    lcdMutex = xSemaphoreCreateMutex();

    // Crée les tâches
    xTaskCreate(&task1, "Task 1", 2048, NULL, 1, NULL);
    xTaskCreate(&task2, "Task 2", 2048, NULL, 1, NULL);

    // Démarrage du système d'exploitation FreeRTOS
    vTaskStartScheduler();
}
