/*
* mqtt_app.c
*
* Created on: Oct 19, 2023
*       Author: Nicolas Grymonprez
*/

#include "esp_log.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"

#include "tasks_common.h"
#include "credentials.h"
#include "http_server.h"
#include "wifi_app.h"

#include "mqtt_client.h" //provides important functions to connect with MQTT
//#include "protocol_examples_common.h" //important for running different protocols in code
#include "esp_event.h" //managing events of mqtt
#include "nvs_flash.h" //storing mqtt and wifi configs and settings

static const char TAG[] = "mqtt_server";

static esp_mqtt_client_handle_t client = NULL;

static void log_error_if_nonzero(const char *message, int error_code)
{
    if (error_code != 0) {
        ESP_LOGE(TAG, "Last error %s: 0x%x", message, error_code);
    }
}

static void mqtt_event_handler(void *handler_args, esp_event_base_t base, int32_t event_id, void *event_data) { 
    ESP_LOGI(TAG, "Event dispatched from event loop base=%s, event_id=%" PRIu32 "\r\n", base, event_id);

    esp_mqtt_event_handle_t event = event_data;
    //esp_mqtt_client_handle_t client = event->client;
    //int msg_id;

    switch ((esp_mqtt_event_id_t)event_id) {
        case MQTT_EVENT_CONNECTED:
                ESP_LOGI(TAG, "MQTT_EVENT_CONNECTED");

                //msg_id = esp_mqtt_client_publish(client, "test/DryerDone", "data", 0, 1, 0);
                //ESP_LOGI(TAG, "sent publish successful, msg_id=%d", msg_id);

                esp_mqtt_client_subscribe(client,"test/DryerDone",0); // in mqtt we require a topic to subscribe and client is from event client and 0 is quality of service it can be 1 or 2
                ESP_LOGI(TAG, "sent subscribe successful" );
                
            break;
        
        case MQTT_EVENT_DISCONNECTED:
                ESP_LOGI(TAG, "MQTT_EVENT_DISCONNECTED"); //if disconnected
                
            break;
        
        case MQTT_EVENT_SUBSCRIBED:
                ESP_LOGI(TAG, "MQTT_EVENT_SUBSCRIBED"); // when subscribed
                
            break;
        
        case MQTT_EVENT_UNSUBSCRIBED:
                ESP_LOGI(TAG, "MQTT_EVENT_UNSUBSCRIBED"); // when unsubscribed
                
            break;

        case MQTT_EVENT_PUBLISHED:
            ESP_LOGI(TAG, "MQTT_EVENT_PUBLISHED, msg_id=%d", event->msg_id);

            break;
        
        case MQTT_EVENT_DATA:
            ESP_LOGI(TAG, "MQTT_EVENT_DATA");
            printf("TOPIC=%.*s\r\n", event->topic_len, event->topic);
            printf("DATA=%.*s\r\n", event->data_len, event->data);

            break;

        case MQTT_EVENT_ERROR:
            ESP_LOGI(TAG, "MQTT_EVENT_ERROR"); // when any error
            
            if (event->error_handle->error_type == MQTT_ERROR_TYPE_TCP_TRANSPORT) {
                log_error_if_nonzero("reported from esp-tls", event->error_handle->esp_tls_last_esp_err);
                log_error_if_nonzero("reported from tls stack", event->error_handle->esp_tls_stack_err);
                log_error_if_nonzero("captured as transport's socket errno",  event->error_handle->esp_transport_sock_errno);
                ESP_LOGI(TAG, "Last errno string (%s)", strerror(event->error_handle->esp_transport_sock_errno));
            }

            break;
        
        default:
            ESP_LOGI(TAG, "Other event id:%d", event->event_id);

            break;
    }
}

void mqtt_app_pub_dryerisdone(void) {
    int msg_id;
    msg_id = esp_mqtt_client_publish(client, "test/DryerDone", "data", 0, 1, 0);
    ESP_LOGI(TAG, "sent publish successful, msg_id=%d", msg_id);
}

void mqtt_app_start(void) {
    const esp_mqtt_client_config_t mqtt_cfg = {
        .broker.address.uri="mqtt://homeassistant.local:1883", 
        .credentials.username=MQTT_LOGIN, // username located in credentials.h
        .credentials.authentication.password=MQTT_PWD, // password located in credentials.h
    };

    #if CONFIG_BROKER_URL_FROM_STDIN

    if (strcmp(mqtt_cfg.broker.address.uri, "FROM_STDIN") != 0) {
        ESP_LOGE(TAG, "Configuration mismatch: wrong broker url");
        abort();
    }

    #endif /* CONFIG_BROKER_URL_FROM_STDIN */

    client = esp_mqtt_client_init(&mqtt_cfg); 

    esp_mqtt_client_register_event(client, ESP_EVENT_ANY_ID, mqtt_event_handler, NULL);
    esp_mqtt_client_start(client); //starting the process
}
