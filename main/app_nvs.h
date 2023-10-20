/*
* app_nvs.h
*
* Created on: Oct 17, 2023
*       Author: Nicolas Grymonprez
*/

#ifndef MAIN_APP_NVS_H_
#define MAIN_APP_NVS_H_

/**
 * Save stations mode Wifi credential to NVS
 * @return ESP_OK if successful
*/
esp_err_t app_nvs_save_sta_creds(void);

/**
 * Loads previously saved credentials from NVS.
 * @return true if previously saved credentials were found
*/
esp_err_t app_nvs_load_sta_creds(void);

/**
 * Clear stations mode Wifi credential from NVS
 * @return ESP_OK if successful
*/
esp_err_t app_nvs_clear_sta_creds(void);

#endif /* MAIN_APP_NVS_H_ */