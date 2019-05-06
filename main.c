#include <stdio.h>
#include <stdlib.h>
#include <MQTTClient.h>
#include <pigpio.h>
#include <signal.h>
#include "settings.h"
#include "callback.h"

static volatile int keepRunning = 1;

void intHandler(int signum) {
    if (signum == SIGINT) {
        keepRunning = 0;
    }
}

int main(int argc, char *argv[]) {
    MQTTClient client;
    MQTTClient_connectOptions conn_opts = MQTTClient_connectOptions_initializer;
    int rc;

    if (gpioInitialise() < 0)
        return 1;

    MQTTClient_create(&client, ADDRESS, CLIENTID, MQTTCLIENT_PERSISTENCE_NONE, NULL);

    MQTTClient_setCallbacks(client, NULL, connlost, msgarrvd, delivered);

    if ((rc = MQTTClient_connect(client, &conn_opts)) != MQTTCLIENT_SUCCESS) {
        printf("Failed to connect, return code %d\n", rc);
        exit(-1);
    }

    printf("Subscribing to topic %s\nfor client %s using QoS%d\n\n"
           "Press CTRL+C to quit\n\n", TOPIC, CLIENTID, QOS);

    MQTTClient_subscribe(client, TOPIC, QOS);

    signal(SIGINT, intHandler);

    while (keepRunning) {

    }

    MQTTClient_disconnect(client, TIMEOUT);
    MQTTClient_destroy(&client);

    gpioTerminate();

    return EXIT_SUCCESS;
}