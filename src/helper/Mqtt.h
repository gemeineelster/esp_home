#ifndef MQTT_H
#define MQTT_H

#pragma once

#include <ESP8266WiFi.h>
#include <PubSubClient.h>

class Mqtt
{
public:
    Mqtt();
    ~Mqtt();

    void setupClient(WiFiClient wifiClient);
    bool connectToBroker(String* ip_server, uint16_t port_server, String* topic_log);
    String getServerIP(void);
    uint16_t getServerPort(void);
    void subscribe(String* topic);
    void publish(String* topic, String* payload);

    void cb_default(char* topic, byte* payload, unsigned int length);
    void setCallback(void*);
    bool connected(void);

private:
    PubSubClient client;
    String ip_server;
    uint16_t port_server;

};

#endif