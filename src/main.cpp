#include <Arduino.h>
#include <ESP8266WiFi.h>
#include <PubSubClient.h>
#include <Ticker.h>
#include "helper/Credentials.h"
#include <string.h>
#include <EthernetClient.h>
//#include "helper/Mqtt.h"
#include "helper/Parser.h"
#include <WString.h>

#define TOPIC_LED (char*) "led"
#define TOPIC_LOG (char*) "log"

WiFiClient espClient;
PubSubClient client;
bool reseived_toggle = false;

bool boMsgReseived = false;
String strPayload;
Parser *parser = new Parser();


// put function declarations here:
void reconnect(void);
int setup_wifi(void);
void led_toggling(uint16_t delay_ms);
void led_blinking_shortly(uint8_t max_count, uint16_t delay_ms_on, uint16_t delay_ms_off);
void callback(char* topic, byte* payload, unsigned int length);

void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200);

  delay(100);
  Serial.println("\nINFO:\t\tStart ESP8266");
  
  setup_wifi(); 

  client.setClient(espClient);
  client.setServer(IP_BROKER, PORT_BROKER);
  client.setCallback(callback);
  delay(300);
  reconnect();
  
  delay(500);
}

void loop() {
  // put your main code here, to run repeatedly:
  client.loop();
  if (!client.connected()) {
    Serial.println("ERROR:\t\tConnection lost!");
    reconnect();
  }

  Serial.print(client.state());
  if (boMsgReseived) {
    Serial.println("Nachricht empfangen!!!!!");
    parser->parseMQTTMessage(strPayload);
    strPayload.clear();
    boMsgReseived = false;
  }
  delay(500);
}

void reconnect(void)
{
  while(!client.connected()) {
    Serial.println("INFO:\t\tAttempting MQTT connection...");

    String clientId = "ESP8266Client-";
    clientId += String(random(0xfffff), HEX);

    if (client.connect(clientId.c_str())) {
      Serial.println("SUCCESS:\tConnected to Broker");

      bool subOk = client.subscribe(TOPIC_LED, 1);
      if (subOk) {
        Serial.print("INFO:\t\tTopic ");
        Serial.print(TOPIC_LED);
        Serial.println(" subscribed!");
        client.publish(TOPIC_LOG, "Connected");
        client.publish(TOPIC_LED, "Hello from ESP8266");
      }
      else {
        Serial.print("FAILED:\t\tTopic ");
        Serial.print(TOPIC_LED);
        Serial.println(" not subscribed!");
      }
      
    }
    else {
      Serial.print("FAILED:\t\trc=");
      Serial.print(client.state());
      Serial.println(" try again in 5 Seconds");
      delay(5000);
    }
  }
}

int setup_wifi(void) {
  delay(10);
  Serial.print("\n");
  Serial.println("INFO:\t\tConnect to Wifi...!");
  
  WiFi.mode(WIFI_STA);
  WiFi.begin(SSID, PASSWORD);

  while(WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.print("\n");
  randomSeed(micros());

  digitalWrite(LED_BUILTIN, HIGH);

  delay(600);
  
  Serial.println("SUCCESS:\tWifi connected!");
  Serial.print("INFO:\t\tLocal IP: ");
  Serial.println(WiFi.localIP());

  return 0;
}

void callback(char* topic, byte* payload, unsigned int length) {
  strPayload.clear();
  strPayload = (char*) payload;

  client.publish(TOPIC_LOG, strPayload.c_str());

  boMsgReseived = true;
}
