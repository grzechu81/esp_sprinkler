#include <Arduino.h>
#include <ESP8266WiFi.h>
#include <PubSubClient.h>
#include <ArduinoJson.h>
#include "sprinkler.hpp"
#include "supervisor.hpp"


const char *ssid =	"xxxxxxxx";
const char *pass =	"yyyyyyyy";
long lastReconnectAttempt = 0;

#define BUFFER_SIZE 100
#define PIN1 1
#define PIN2 2
#define MAX_WORKING_TIME 40 //in minutes

IPAddress mqttIpAddr(172, 16, 0, 2);
WiFiClient wifiClient;
PubSubClient mqttClient(wifiClient);

Sprinkler area1Sprinkler(PIN1, MAX_WORKING_TIME);
Sprinkler area2Sprinkler(PIN2, MAX_WORKING_TIME);

Supervisor sprSupervisor;

void callback(char* topic, byte* payload, unsigned int length) 
{
  Serial.print("Message arrived [");
  Serial.print(topic);
  Serial.print("] ");
  for (unsigned int i=0; i<length; i++) 
  {
    Serial.print((char)payload[i]);
  }
  Serial.println();

  StaticJsonDocument<128> json;
  DeserializationError err = deserializeJson(json, payload, length);

  if(err)
  {
    Serial.print(F("deserializeJson() returned "));
    Serial.println(err.c_str());
    return;
  }

  uint8_t cmd = json["cmd"];

  if(cmd == 0)
  {
    sprSupervisor.stop();
  }
  else
  {
    uint8_t time0 = json["data"][0]["time"];
    uint8_t time1 = json["data"][1]["time"];

    sprSupervisor.start(0, time0);
    sprSupervisor.start(1, time1);
  }
  

}

boolean reconnect() {
  long now = millis();

  if (now - lastReconnectAttempt > 5000) 
  {
      lastReconnectAttempt = now;

      // Attempt to reconnect
      if (mqttClient.connect("arduinoClient")) 
      {
        // Once connected, publish an announcement...
        mqttClient.publish("outTopic","hello world");
        // ... and resubscribe
        mqttClient.subscribe("inTopic");
      }
  }
 
  return mqttClient.connected();
}

void setup() {
  Serial.begin(115200);
  mqttClient.setServer(mqttIpAddr, 1883);
  mqttClient.setCallback(callback);

  WiFi.begin("ssid", "passwd");
  delay(1500);
}

void loop() {
  if (!mqttClient.connected()) 
  {
    reconnect();
  } 
  else 
  {
    // Client connected
    mqttClient.loop();
    sprSupervisor.loop();
  }
}
