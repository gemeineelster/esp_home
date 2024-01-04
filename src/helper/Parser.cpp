#include "Parser.h"
#include <Arduino.h>

Parser::Parser()
{
    pinMode(LED_BUILTIN, OUTPUT);
    Serial.begin(115200);
}

Parser::~Parser()
{

}

void Parser::parseMQTTMessage(String msg)
{
    //digitalWrite(LED_BUILTIN, LOW);
    Serial.println("Parser called");
    Serial.println(msg.c_str());
    if (msg.startsWith("LED=ON")) {
        digitalWrite(LED_BUILTIN, LOW);
    }
    
    if (msg.startsWith("LED=OFF")) {
        digitalWrite(LED_BUILTIN, HIGH);
    }
    
    return;
}