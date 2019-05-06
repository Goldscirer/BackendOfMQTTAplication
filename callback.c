#include <stdio.h>
#include <MQTTClient.h>
#include <json-c/json.h>
#include <pigpio.h>
#include "settings.h"
#include "callback.h"

void delay(int ms) {  // delay in miliseconds
    gpioDelay(1000*ms);
}

void delivered(void *context, MQTTClient_deliveryToken dt) {
    printf("Message with token value %d delivery confirmed\n", dt);
    deliveredtoken = dt;
}

int msgarrvd(void *context, char *topicName, int topicLen, MQTTClient_message *message) {

    char* payloadptr;
    json_object *jobj;
    json_object *tmp;

    int state;

    printf("Message arrived\n");
    printf("     topic: %s\n", topicName);
    printf("   message: ");

    payloadptr = message->payload;

    jobj = json_tokener_parse(payloadptr);

    printf("\n---\n%s\n---\n", json_object_to_json_string_ext(jobj, JSON_C_TO_STRING_SPACED | JSON_C_TO_STRING_PRETTY));

    if (json_object_object_get_ex(jobj, "state", &tmp)) {
        state = json_object_get_int(tmp);

        if(state == 1)
        {
            gpioServo(GATE, 500);
            delay(300);
        }
        else if(state == 0)
        {
            gpioServo(GATE, 1500);
            delay(300);
        }
    }

    json_object_put(tmp);
    json_object_put(jobj);
    MQTTClient_freeMessage(&message);
    MQTTClient_free(topicName);
    return 1;
}

void connlost(void *context, char *cause) {
    printf("\nConnection lost\n");
    printf("     cause: %s\n", cause);
}