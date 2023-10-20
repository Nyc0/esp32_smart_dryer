/**
 * Application entry point.
*/
#include "esp_log.h"
#include "nvs_flash.h"

#include "sntp_time_sync.h"
#include "freertos/FreeRTOS.h"
#include "wifi_app.h"
#include "wifi_reset_button.h"
#include "mqtt_app.h"

// #include <stdio.h>
// #include "freertos/FreeRTOS.h"
// #include "freertos/task.h"
// #include "esp_system.h"
// #include "driver/gpio.h"

// #include "rgb_led.h"

static const char TAG[] = "main";

void wifi_application_connected_events(void) {
    ESP_LOGI(TAG, "WiFi Application Connected!!");
    sntp_time_sync_task_start();
    mqtt_app_start(); // MQTT start app as shown above most important code for MQTT   
}

void app_main(void)
{
//     while (true){
//         rgb_led_wifi_app_started();
//         vTaskDelay(1000 / portTICK_PERIOD_MS);
//         rgb_led_http_server_started();
//         vTaskDelay(1000 / portTICK_PERIOD_MS);
//         rgb_led_wifi_connected();
//         vTaskDelay(1000 / portTICK_PERIOD_MS);
//     }

    // Initialize NVS
    esp_err_t ret = nvs_flash_init();
    if (ret == ESP_ERR_NVS_NO_FREE_PAGES || ret == ESP_ERR_NVS_NEW_VERSION_FOUND) {
        ESP_ERROR_CHECK(nvs_flash_erase());
        ret = nvs_flash_init();
    }
    ESP_ERROR_CHECK(ret);

    // Start Wifi
    wifi_app_start();

    // Configure Wifi reset button
    wifi_reset_button_config();

    // Set connected event callback
    wifi_app_set_callback(&wifi_application_connected_events);
}
