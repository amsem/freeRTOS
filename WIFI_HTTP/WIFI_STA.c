#include <stdio.h>
#include <string.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/event_groups.h"
#include "freertos/semphr.h"
#include "freertos/event_groups.h"
#include "esp_wifi.h"
#include "esp_event.h"
#include "esp_log.h"
#include "esp_system.h"
#include "nvs_flash.h"
#include "driver/gpio.h"

#define WIFI_SSID "Test"
#define WIFI_PASS "Essai2020"
#define LED_CONNECTED GPIO_NUM_2
#define LED_DISCONNECTED GPIO_NUM_4

static EventGroupHandle_t wifi_event_group;
const int CONNECTED_BIT = BIT0;

static void event_handler(void *arg, esp_event_base_t event_base, int32_t event_id, void *event_data)
{
    if (event_base == WIFI_EVENT && event_id == WIFI_EVENT_STA_START)
    {
        esp_wifi_connect();
    }
    else if (event_base == WIFI_EVENT && event_id == WIFI_EVENT_STA_DISCONNECTED)
    {
        xEventGroupClearBits(wifi_event_group, CONNECTED_BIT);
        gpio_set_level(LED_CONNECTED, 0);
        gpio_set_level(LED_DISCONNECTED, 1);
        esp_wifi_connect();
        ESP_LOGI("wifi", "Reconnecting to the WiFi network");
    }
    else if (event_base == IP_EVENT && event_id == IP_EVENT_STA_GOT_IP)
    {
        xEventGroupSetBits(wifi_event_group, CONNECTED_BIT);
        gpio_set_level(LED_CONNECTED, 1);
        gpio_set_level(LED_DISCONNECTED, 0);
    }
}

static void wifi_init_sta()
{
    wifi_event_group = xEventGroupCreate();

    tcpip_adapter_init();
    ESP_ERROR_CHECK(esp_event_loop_create_default());

    wifi_init_config_t cfg = WIFI_INIT_CONFIG_DEFAULT();
    ESP_ERROR_CHECK(esp_wifi_init(&cfg));

    ESP_ERROR_CHECK(esp_event_handler_instance_register(WIFI_EVENT, ESP_EVENT_ANY_ID, &event_handler, NULL, NULL));
    ESP_ERROR_CHECK(esp_event_handler_instance_register(IP_EVENT, IP_EVENT_STA_GOT_IP, &event_handler, NULL, NULL));

    wifi_config_t wifi_config = {
        .sta = {
            .ssid = WIFI_SSID,
            .password = WIFI_PASS,
        },
    };

    ESP_ERROR_CHECK(esp_wifi_set_mode(WIFI_MODE_STA));
    ESP_ERROR_CHECK(esp_wifi_set_config(ESP_IF_WIFI_STA, &wifi_config));
    ESP_ERROR_CHECK(esp_wifi_start());
}

static void network_task(void *pvParameter)
{
    while (1)
    {
        if (xEventGroupWaitBits(wifi_event_group, CONNECTED_BIT, false, true, portMAX_DELAY))
        {
            tcpip_adapter_ip_info_t ip_info;
            ESP_ERROR_CHECK(tcpip_adapter_get_ip_info(TCPIP_ADAPTER_IF_STA, &ip_info));
            ESP_LOGI("network_task", "IP Address: %s", ip4addr_ntoa(&ip_info.ip));
            ESP_LOGI("network_task", "Subnet Mask: %s", ip4addr_ntoa(&ip_info.netmask));
            ESP_LOGI("network_task", "Gateway Address: %s", ip4addr_ntoa(&ip_info.gw));
        }
    }
}

void app_main()
{
    nvs_flash_init();
    gpio_pad_select_gpio(LED_CONNECTED);
    gpio_set_direction(LED_CONNECTED, GPIO_MODE_OUTPUT);
    gpio_pad_select_gpio(LED_DISCONNECTED);
    gpio_set_direction(LED_DISCONNECTED, GPIO_MODE_OUTPUT);

    xTaskCreate(&network_task, "network_task", 4096, NULL, 5, NULL);
    wifi_init_sta();
}
