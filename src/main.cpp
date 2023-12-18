#include <Arduino.h>
#include <ESP8266WiFi.h>
#include <PubSubClient.h>
#include "helper/Credentials.h"
#include <string.h>


#define TOPIC_LED "led"
#define TOPIC_LOG "log"


// put function declarations here:
void led_toggling(uint16_t delay_ms);
void led_blinking_shortly(uint8_t max_count, uint16_t delay_ms_on, uint16_t delay_ms_off);
int setup_wifi(void);
int setup_mqtt(void);
void cb_message_rx(char *, uint8_t *, unsigned int);
void reconnect(void);


WiFiClient clientESP;
PubSubClient client(clientESP);
bool reseived_toggle = false;

void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200);

  /*pinMode(LED_BUILTIN, OUTPUT);
  delay(200);
  digitalWrite(LED_BUILTIN, HIGH);*/

  setup_wifi();

  client.setServer(IP_BROKER, PORT_BROKER);
  client.setCallback(cb_message_rx);
}

void loop() {
  // put your main code here, to run repeatedly:
  if (!client.connected()) {
    reconnect();
  }

  if (reseived_toggle) {
    led_toggling(1000);
  }
  //Serial.println("LoOp");
}

void cb_message_rx(char * topic, uint8_t *payload, unsigned int length) {
  Serial.print((char*) payload);
  client.publish(TOPIC_LOG, (char*) payload);
  client.publish(TOPIC_LOG, "Emfangen");
}

void led_blinking_shortly(uint8_t max_count, uint16_t delay_ms_on, uint16_t delay_ms_off) {
  for (int i = 0; i < max_count; i++) {
    digitalWrite(LED_BUILTIN, LOW);
    delay(delay_ms_on);
    digitalWrite(LED_BUILTIN, HIGH);
    delay(delay_ms_off);
  }
  digitalWrite(LED_BUILTIN, HIGH);
}

void led_toggling(uint16_t delay_ms){
  static bool status = false;

  digitalWrite(LED_BUILTIN, status);
  status = !status;
  delay(delay_ms);
}

void reconnect(void)
{
  while(!client.connected()) {
    // Create a random client ID
    String clientId = "ESP8266Client-";
    clientId += String(random(0xffff), HEX);

    if (client.connect(clientId.c_str())) {
      //led_blinking_shortly(5, 700, 300);
      String msg;
      msg = (clientId + " is connected!");
      client.publish(TOPIC_LOG, msg.c_str());
      //Serial.print("connected...");
      client.subscribe(TOPIC_LED);
    }
    else {
      led_blinking_shortly(10, 300, 300);
      delay(5000);
    }
  }
}

int setup_wifi(void) {
  Serial.begin(115200);
  delay(10);
  Serial.println("\n");

  WiFi.begin(SSID, PASSWORD);
  Serial.print("Connecting...");
  Serial.print(SSID);
  Serial.println("...");

  int i = 0;
  while(WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.print(i++);
    Serial.print(" ");
  }
  digitalWrite(LED_BUILTIN, HIGH);

  Serial.println("\n");
  Serial.println("Connection etablished!");
  Serial.print("IP address: ");
  Serial.println(WiFi.localIP());

  //led_blinking_shortly(5, 400, 400);

  return 0;
}
