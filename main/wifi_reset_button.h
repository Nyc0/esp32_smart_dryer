/*
* wifi_reset_button.h
*
* Created on: Oct 18, 2023
*       Author: Nicolas Grymonprez
*/

#ifndef MAIN_WIFI_RESET_BUTTON_H_
#define MAIN_WIFI_RESET_BUTTON_H_

// Default interupt flag
#define ESP_INTR_FLAG_DEFAULT   0

// Wifi reset button is the BOOT button on the dev kit
#define WIFI_RESET_BUTTON       0

/**
 * Configure Wifi reset button and interrup configuration
*/
void wifi_reset_button_config(void);

#endif /* MAIN_WIFI_RESET_BUTTON_H_ */