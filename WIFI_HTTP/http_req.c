#include <freertos/FreeRTOS.h>
#include <freertos/task.h>
#include <esp_log.h>
#include <esp_wifi.h>
#include <nvs_flash.h>
#include <esp_http_client.h>

static const char *TAG = "HTTP_POST_EXAMPLE";

static esp_err_t event_handler(esp_http_client_event_t *evt)
{
    switch (evt->event_id)
    {
    case HTTP_EVENT_ERROR:
        printf("HTTP_EVENT_ERROR\n");
        break;
    case HTTP_EVENT_ON_CONNECTED:
        printf("HTTP_EVENT_ON_CONNECTED\n");
        break;
    case HTTP_EVENT_HEADER_SENT:
        printf("HTTP_EVENT_HEADER_SENT\n");
        break;
    case HTTP_EVENT_ON_HEADER:
        printf("HTTP_EVENT_ON_HEADER, key=%s, value=%s\n", evt->header_key, evt->header_value);
        break;
    case HTTP_EVENT_ON_DATA:
        printf("HTTP_EVENT_ON_DATA, len=%d\n", evt->data_len);
        break;
    case HTTP_EVENT_ON_FINISH:
        printf("HTTP_EVENT_ON_FINISH\n");
        break;
    case HTTP_EVENT_DISCONNECTED:
        printf("HTTP_EVENT_DISCONNECTED\n");
        break;
    }
    return ESP_OK;
}

void http_post_task(void *pvParameters)
{
    esp_http_client_config_t config = {
        .url = "http://192.168.1.14/post",
        .event_handler = event_handler,
    };

    esp_http_client_handle_t client = esp_http_client_init(&config);

    esp_err_t err = esp_http_client_perform(client);
    if (err == ESP_OK)
    {
        printf("HTTP POST Status = %d, content_length = %d\n",
               esp_http_client_get_status_code(client),
               esp_http_client_get_content_length(client));
    }
    else
    {
        printf("HTTP POST request failed: %s\n", esp_err_to_name(err));
    }

    esp_http_client_cleanup(client);

    vTaskDelete(NULL);
}

void app_main()
{
    nvs_flash_init();
    tcpip_adapter_init();

    wifi_init_config_t cfg = WIFI_INIT_CONFIG_DEFAULT();
    esp_wifi_init(&cfg);
    esp_wifi_set_storage(WIFI_STORAGE_RAM);
    esp_wifi_set_mode(WIFI_MODE_STA);

    wifi_config_t wifi_config = {
        .sta = {
            .ssid = "FREE",
            .password = "123456789*",
        },
    };

    esp_wifi_set_config(WIFI_IF_STA, &wifi_config);
    esp_wifi_start();
    esp_wifi_connect();

    while (true)
    {
        if (esp_wifi_connect() == ESP_OK)
        {
            printf("WiFi connected\n");
            break;
        }
        printf("WiFi Connection Failed. Retrying...\n");
        vTaskDelay(1000 / portTICK_PERIOD_MS);
    }

    xTaskCreate(&http_post_task, "http_post_task", 4096, NULL, 5, NULL);
}
