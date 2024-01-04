#include "Mqtt.h"
#include "PubSubClient.h"
#include <Arduino.h>


Mqtt::Mqtt()
{

}

Mqtt::~Mqtt()
{

}

void Mqtt::setupClient(WiFiClient wifiClient)
{
    Serial.println("Setup wifiClient for MQTT..");
    this->client.setClient(wifiClient);
}

String Mqtt::getServerIP(void)
{
    return this->ip_server;
}

uint16_t Mqtt::getServerPort(void)
{
    Serial.print("Get server port: ");
    Serial.println(this->port_server);
    return this->port_server;
}

bool Mqtt::connectToBroker(String* ip_server, uint16_t port_server, String *topic_log)
{
    uint8_t cntConnection = 0;

    Serial.println("MQTT: Connect to broker...");

    this->ip_server = *ip_server;
    this->port_server = port_server;

    while(!this->client.connected()) {
        String clientId = "ESP8266Client-";
        clientId += String(random(0xffff), HEX);

        if (this->client.connect(clientId.c_str())) {
            Serial.println("MQTT: Client connected!");
            client.publish(topic_log->c_str(), "ESPClient connected!");
            return true;
        }
        else {
            cntConnection++;
            Serial.print(cntConnection);
            Serial.println("MQTT: Failed connecting to client!");
            if (cntConnection >= 10) {
                Serial.println("MQTT: Error connecting client!");
                return false;
            }
            delay(3000);
        }
    }

    return false;
}

void Mqtt::subscribe(String* topic) // $$Möglicher Fehler, da noch andere Klasse in Frage kommt!
{
    Serial.print("MQTT: Subscribe topic: ");
    Serial.println(topic->c_str());
    this->client.subscribe(topic->c_str());
}

void Mqtt::publish(String* topic, String* payload)
{

}

void Mqtt::cb_default(char* topic, byte* payload, unsigned int length)
{
    _NOP();
}

void Mqtt::setCallback(void*)
{
    _NOP();
}

bool Mqtt::connected(void)
{
    return this->client.connected();;
}
