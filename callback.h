#ifndef MQTT_CALLBACK_H
#define MQTT_CALLBACK_H

#include "MQTTClient.h"

volatile MQTTClient_deliveryToken deliveredtoken;

void delivered(void *context, MQTTClient_deliveryToken dt);

int msgarrvd(void *context, char *topicName, int topicLen, MQTTClient_message *message);

void connlost(void *context, char *cause);

#endif //MQTT_CALLBACK_H
