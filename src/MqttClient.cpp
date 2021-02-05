/*
 * MqttClient.cpp
 *
 *  Created on: Jan 28, 2021
 *      Author: ht
 */

#include "MqttClient.h"
#include <cstring>
#include <iostream>

using namespace std;

MqttClient::MqttClient(const char *host, const int port = 1883) {
	this->host = host;
	this->port = port;
	this->username = NULL;
	this->password = NULL;
}

MqttClient::MqttClient(const char *host, const int port, const char *username,
		const char *password) {
	this->host = host;
	this->port = port;
	this->username = username;
	this->password = password;
}

void MqttClient::setClientId(const char *clientId) {
	if (this->clientId != NULL) {
		delete this->clientId;
	}
	this->clientId = clientId;
	this->mosq = mosquitto_new(this->clientId, true, 0);
}

MqttClient::~MqttClient() {
	this->close();
	delete this->host;
	delete this->username;
	delete this->password;
}

int MqttClient::connect() {
	if (this->clientId == NULL) {
		cout << "Set client ID first" << endl;
	}
	if (this->username != NULL) {
		mosquitto_username_pw_set(this->mosq, this->username, this->password);
	}
	int c = mosquitto_connect(this->mosq, this->host, this->port,
	MQTT_KEEP_ALIVE);
	if (c == MOSQ_ERR_SUCCESS) {
		cout << "Connected to broker" << endl;
	}
	return c;
}

int MqttClient::reconnect() {
	return mosquitto_reconnect(this->mosq);
}

void MqttClient::close() {
	if (this->mosq != NULL) {
		mosquitto_destroy(this->mosq);
	}
}

int MqttClient::publish(const char *topic, const char *payload) {
	return mosquitto_publish(this->mosq, NULL, topic, strlen(payload), payload,
			0, 0);
}

int MqttClient::subscribe(const char *topic) {
	mosquitto_subscribe(this->mosq, NULL, topic, 0);
	return 0;
}

int MqttClient::unsubscribe(const char *topic) {
	mosquitto_unsubscribe(this->mosq, NULL, topic);
	return 0;
}

void MqttClient::onMessage(
		void (*onMessage)(mosquitto *mosq, void *obj,
				const mosquitto_message *message)) {
	mosquitto_message_callback_set(this->mosq, onMessage);
	cout << "Set message callback" << endl;
}

mosquitto* MqttClient::getMosquitto() {
	return this->mosq;
}
