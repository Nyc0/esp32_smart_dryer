/*
* tasks_common.h
*
* Created on: Oct 11, 2023
*       Author: Nicolas Grymonprez
*/

#ifndef MAIN_TASKS_COMMON_H_
#define MAIN_TASKS_COMMON_H_

// Wifi application task
#define WIFI_APP_TASK_STACK_SIZE                4096
#define WIFI_APP_TASK_PRIORITY                  5
#define WIFI_APP_TASK_CORE_ID                   0

//HTTP Server task
#define HTTP_SERVER_TASK_STACK_SIZE             4096
#define HTTP_SERVER_TASK_PRIORITY               4
#define HTTP_SERVER_TASK_CORE_ID                0

//HTTP Server Monitor task info
#define HTTP_SERVER_MONITOR_STACK_SIZE          4096
#define HTTP_SERVER_MONITOR_PRIORITY            3
#define HTTP_SERVER_MONITOR_CORE_ID             0

// Wifi Reset Button task
#define WIFI_RESET_BUTTON_TASK_STACK_SIZE       2048
#define WIFI_RESET_BUTTON_TASK_PRIOROTY         6
#define WIFI_RESET_BUTTON_TASK_CORE_ID          0

// SNTP Time Sync task
#define SNTP_TIME_SYNC_TASK_STACK_SIZE          4096
#define SNTP_TIME_SYNC_TASK_PRIOROTY            4
#define SNTP_TIME_SYNC_TASK_CORE_ID             1

// MQTT Task
#define MQTT_TASK_STACK_SIZE				    9216
#define MQTT_TASK_PRIORITY				        6
#define MQTT_TASK_CORE_ID				        1

#endif /* MAIN_TASKS_COMMON_H_ */