//============================================================================
// Name        : MQTT.cpp
// Author      : Pham Thanh Tung
// Version     :
// Copyright   : Your copyright notice
// Description : Hello World in C++, Ansi-style
//============================================================================

#include <iostream>
#include <cstring>
#include <mosquitto.h>
#include <unistd.h>
#include <cstdlib>
#include <cstdio>
#include <ctime>
#include "MqttClient.h"

#define HOST		"192.168.28.13"
#define PORT		1883
#define USER		"RD"
#define PASSWORD	"1"
#define TOPIC		"HC_TEST"
#define REQUESTER	"REQUESTER"
#define WORKER		"WORKER"

#define SIMPLE_MESSAGE "SIMPLE_MESSAGE"
#define COMPLEX_MESSAGE "COMPLEX_MESSAGE"

using namespace std;

int handled_messages = 0;

void handle_message(mosquitto *mosq, void *obj,
		const mosquitto_message *message) {
	char *msg = new char[255];
	strcpy(msg, (char*) message->payload);
	system("date");
	printf("%s\n", msg);
	if (strncmp(msg, SIMPLE_MESSAGE, strlen(SIMPLE_MESSAGE)) == 0) {
		printf("This is a simple message. HC will do nothing");
	}
	if (strncmp(msg, COMPLEX_MESSAGE, strlen(COMPLEX_MESSAGE)) == 0) {
		printf(
				"This is a complex message. HC will execute 10k operator before return the result");
		printf("\n--- Start calculation ---");
		long sum = 0;
		for (int i = 0; i < 10000; i++) {
			sum += rand();
		}
		long sleep_time = (rand() % 100) * 1000;
		cout << endl << "Sleep " << sleep_time / 1000 << " mili-seconds" << endl;
		usleep(sleep_time);
		printf("\nThe result is %i", sum);
		printf("\n--- Finish calculation ---");
	}
	printf("\nHandled queue: %i", handled_messages);
	handled_messages++;
	printf("\n----------------------\n");
	delete msg;
}

int main(const int argc, char **argv) {
	if (argc == 2) {
		if (strcmp(argv[1], "requester") == 0) {
			MqttClient *client = new MqttClient(HOST, PORT, USER, PASSWORD);
			client->setClientId(REQUESTER);
			client->connect();
			for (int i = 0; i < 100; i++) {
				int r = rand() % 2;
				char* message = new char[255];
				sprintf(message, "%s %i", (r == 0 ? SIMPLE_MESSAGE : COMPLEX_MESSAGE), i);
				int msgSent = client->publish(TOPIC, message);
				if (msgSent == MOSQ_ERR_SUCCESS) {
					cout << "Message published: " << message << endl;
					system("date");
				}
			}
			cout << time(0) << endl;
			return 0;
		}

		if (strcmp(argv[1], "worker") == 0) {
			MqttClient *client = new MqttClient(HOST, PORT, USER, PASSWORD);
			client->setClientId(WORKER);
			client->onMessage(handle_message);
			client->connect();
			client->subscribe(TOPIC);
			mosquitto_loop_start(client->getMosquitto());
			mosquitto_loop_forever(client->getMosquitto(), -1, 1);
			return 0;
		}
	}

	cout << "Invalid argument" << endl;
	return 1;
}
