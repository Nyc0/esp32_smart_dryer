/*
* wifi_reset_button.c
*
* Created on: Oct 18, 2023
*       Author: Nicolas Grymonprez
*/

#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/semphr.h"
#include "driver/gpio.h"
#include "esp_log.h"
#include "esp_timer.h"

#include "tasks_common.h"
#include "wifi_app.h"
#include "wifi_reset_button.h"
#include "mqtt_app.h"

static const char TAG[] = "wifi_reset_button";

// Semaphore handle
SemaphoreHandle_t wifi_reset_semaphore = NULL;

// Variable to store previous execution and current
static int64_t esp_timer_last;
static int64_t esp_timer_now;

/**
 * ISR handler for the Wifi reset (BOOT) button
 * @param arg parameter which can be passed to the ISR handler
*/
void IRAM_ATTR wifi_reset_button_isr_handler(void *arg) {
    // Notify the button task
    xSemaphoreGiveFromISR(wifi_reset_semaphore, NULL);
}

/**
 * Wifi reset button task reacts to a boot button event by sending a message to the wifi app application to disconnect from the Wifi and clear the saved credentials
 * @param pvParam which can be passed to the task.
*/
void wifi_reset_button_task (void *pvParam) {

    for (;;) {
        if (xSemaphoreTake(wifi_reset_semaphore, portMAX_DELAY) == pdTRUE) {
            
            esp_timer_now = esp_timer_get_time();
            printf("Time elapsed: %lldμs - %lldμs = %lldμs (%llds)\n", esp_timer_now, esp_timer_last, (esp_timer_now - esp_timer_last), (esp_timer_now - esp_timer_last)/1000000);

            if ((esp_timer_now - esp_timer_last) > 60000000) { // Can't be triggered within the 60s of boots, and can't be retrieggered within 60s since last trigger
                ESP_LOGI(TAG,"WIFI RESET BUTTON INTERRUPT OCCURED");

                // Send message to disconnect Wifi and clear credentials
                //wifi_app_send_message(WIFI_APP_MSG_USER_REQUESTED_STA_DISCONNECT);

                mqtt_app_pub_dryerisdone();

                esp_timer_last = esp_timer_now;
            } else {
                ESP_LOGI(TAG,"WIFI RESET BUTTON INTERRUPT OCCURED HOWEVER IT WAS WITHIN 60s - Do nothing");
            }
            
            vTaskDelay(2000 / portTICK_PERIOD_MS);
        }
    }
}

void wifi_reset_button_config(void) {
    // Create the binary semaphore
    wifi_reset_semaphore = xSemaphoreCreateBinary();

    // Configure the button and set the direction
    esp_rom_gpio_pad_select_gpio(WIFI_RESET_BUTTON);
    gpio_set_direction(WIFI_RESET_BUTTON, GPIO_MODE_INPUT);

    // Enable interrup on the negative edge
    gpio_set_intr_type(WIFI_RESET_BUTTON, GPIO_INTR_NEGEDGE);

    // Create the wifi reset button task
    xTaskCreatePinnedToCore(&wifi_reset_button_task, "wifi_reset_button", WIFI_RESET_BUTTON_TASK_STACK_SIZE, NULL, WIFI_RESET_BUTTON_TASK_PRIOROTY, NULL, WIFI_RESET_BUTTON_TASK_CORE_ID);

    // Install gpio isr service
    gpio_install_isr_service(ESP_INTR_FLAG_DEFAULT);

    // Attached the interupt service routine
    gpio_isr_handler_add(WIFI_RESET_BUTTON, wifi_reset_button_isr_handler, NULL);

    esp_timer_last = esp_timer_get_time();
    esp_timer_now = esp_timer_get_time();

}