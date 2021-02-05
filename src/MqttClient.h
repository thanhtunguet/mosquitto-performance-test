/*
 * MqttClient.h
 *
 *  Created on: Jan 28, 2021
 *      Author: ht
 */

#ifndef MQTTCLIENT_H_
#define MQTTCLIENT_H_

#include <iostream>
#include <mosquitto.h>

#define MQTT_KEEP_ALIVE	60

class MqttClient {
private:
    mosquitto *mosq;
    const char *host;
    int port;
    const char *username;
    const char *password;
    const char *clientId;
public:
    MqttClient(const char *host, int port);

    MqttClient(const char *host, int port, const char *username, const char *password);

    virtual ~MqttClient();

    int connect();

    int reconnect();

    void close();

    void setClientId(const char *clientId);

    int publish(const char *topic, const char *payload);

    int subscribe(const char *topic);

    int unsubscribe(const char *topic);

    void onMessage(void (*onMessage)(mosquitto *mosq, void *obj, const mosquitto_message *message));

    mosquitto* getMosquitto();
};

#endif /* MQTTCLIENT_H_ */
