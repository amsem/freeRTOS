#include <stdio.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/portmacro.h"

// Compteur pour le nombre de fois que la tâche idle est exécutée
volatile uint32_t idleCounter = 0;

// Fonction pour la tâche idle
void vApplicationIdleHook(void)
{
    // Incrémenter le compteur à chaque exécution de la tâche idle
    idleCounter++;
}

// Fonction pour la tâche qui affiche le nombre d'exécutions de la tâche idle
void displayTask(void *pvParameters)
{
    while (1)
    {
        vTaskDelay(pdMS_TO_TICKS(1000)); // Attendre une seconde
        printf("Nombre d'exécutions de la tâche idle en une seconde : %u\n", idleCounter);
        idleCounter = 0; // Réinitialiser le compteur pour la prochaine seconde
    }
}

void app_main()
{
    // Créer la tâche d'affichage
    xTaskCreatePinnedToCore(displayTask, "DisplayTask", 4096, NULL, 1, NULL, 0);

    // Démarrer le planificateur FreeRTOS
    vTaskStartScheduler();
}
