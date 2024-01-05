#include <stdio.h>
#include <freertos/FreeRTOS.h>
#include <freertos/task.h>
#include <freertos/semphr.h>
#include <freertos/event_groups.h>
#include <driver/gpio.h>

#define GPIO_BTN1 GPIO_NUM_22
#define GPIO_BTN2 GPIO_NUM_23

#define BIT_BTN1 (1 << 0)
#define BIT_BTN2 (1 << 1)

EventGroupHandle_t btnEventGroup;

void BTN1_Task(void *pvParameters)
{
    while (1)
    {
        EventBits_t bits = xEventGroupWaitBits(btnEventGroup, BIT_BTN1, pdTRUE, pdFALSE, portMAX_DELAY);

        if ((bits & BIT_BTN1) != 0)
        {
            printf("BTN1 pressed\n");
        }
    }
}

void BTN2_Task(void *pvParameters)
{
    while (1)
    {
        EventBits_t bits = xEventGroupWaitBits(btnEventGroup, BIT_BTN2, pdTRUE, pdFALSE, portMAX_DELAY);

        if ((bits & BIT_BTN2) != 0)
        {
            printf("BTN2 pressed\n");
        }
    }
}

void BTN_SIM_Task(void *pvParameters)
{
    while (1)
    {
        EventBits_t bits = xEventGroupWaitBits(btnEventGroup, (BIT_BTN1 | BIT_BTN2), pdTRUE, pdFALSE, portMAX_DELAY);

        if ((bits & (BIT_BTN1 | BIT_BTN2)) == (BIT_BTN1 | BIT_BTN2))
        {
            printf("Both BTN1 and BTN2 pressed simultaneously\n");
        }
    }
}

void buttonTask(void *pvParameters)
{
    while (1)
    {
        if (gpio_get_level(GPIO_BTN1) == 0)
        {
            // BTN1 pressed
            xEventGroupSetBits(btnEventGroup, BIT_BTN1);
        }

        if (gpio_get_level(GPIO_BTN2) == 0)
        {
            // BTN2 pressed
            xEventGroupSetBits(btnEventGroup, BIT_BTN2);
        }

        vTaskDelay(10 / portTICK_PERIOD_MS); // Check buttons every 10ms
    }
}

void app_main()
{
    // Initialize the Event Group
    btnEventGroup = xEventGroupCreate();

    // Configure GPIOs for buttons
    gpio_config_t btn1_conf = {
        .pin_bit_mask = (1ULL << GPIO_BTN1),
        .mode = GPIO_MODE_INPUT,
        .intr_type = GPIO_INTR_ANYEDGE,
        .pull_up_en = GPIO_PULLUP_ENABLE,
    };

    gpio_config_t btn2_conf = {
        .pin_bit_mask = (1ULL << GPIO_BTN2),
        .mode = GPIO_MODE_INPUT,
        .intr_type = GPIO_INTR_ANYEDGE,
        .pull_up_en = GPIO_PULLUP_ENABLE,
    };

    gpio_config(&btn1_conf);
    gpio_config(&btn2_conf);

    // Create tasks for button handling
    xTaskCreate(BTN1_Task, "BTN1_Task", 2048, NULL, 5, NULL);
    xTaskCreate(BTN2_Task, "BTN2_Task", 2048, NULL, 5, NULL);
    xTaskCreate(BTN_SIM_Task, "BTN_SIM_Task", 2048, NULL, 5, NULL);
    xTaskCreate(buttonTask, "ButtonTask", 2048, NULL, 5, NULL);
}
