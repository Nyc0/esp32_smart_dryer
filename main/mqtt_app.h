/*
* mqtt_app.h
*
* Created on: Oct 19, 2023
*       Author: Nicolas Grymonprez
*/

#ifndef MAIN_MQTT_APP_H_
#define MAIN_MQTT_APP_H_

/**
 * Publish to test/DryerDone
*/
void mqtt_app_pub_dryerisdone(void);

/**
 * Initialize MQTT server task
*/
void mqtt_app_start(void);

#endif /* MAIN_MQTT_APP_H_ */
