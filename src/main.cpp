#ifndef UNIT_TEST

#include <Arduino.h>
#include <ESP8266WiFi.h>
#include <PubSubClient.h>
#include <ArduinoJson.h>
#include "sprinkler.hpp"
#include "sprinklermanager.hpp"

#define BUFFER_SIZE 100
#define PIN1 1
#define PIN2 2
#define MAX_WORKING_TIME 40 //in minutes
#define LED 2

long lastReconnectAttempt = 0;
const char *ssid = "AndroidAP";
const char *password = "haslo123";

IPAddress mqttIpAddr(172, 16, 0, 2);
WiFiClient wifiClient;
PubSubClient mqttClient(wifiClient);

Sprinkler area1Sprinkler(0, PIN1, MAX_WORKING_TIME);
Sprinkler area2Sprinkler(1, PIN2, MAX_WORKING_TIME);
SprinklerManager sManager;

WiFiEventHandler onConnected;
WiFiEventHandler onDisconnected;
WiFiEventHandler onGotIp;


// {
// 	"cmd" : 1,
// 	"data" : [
// 		{"time":15},
// 		{"time":20}
// 	]
// }

// {
// 	"cmd" : 0
// }

void onStationConnected(const WiFiEventStationModeConnected& evt) {
  Serial.println("Station connected");
}

void onStationDisconnected(const WiFiEventStationModeDisconnected& evt) {
  Serial.println("Station disconnected");
}

void onStationGotIp(const WiFiEventStationModeGotIP& evt) {
  Serial.println("Gor Ip addr");
}

void onMqttPayloadRcvd(char* topic, byte* payload, unsigned int length) 
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
    sManager.stop();
  }
  else
  {
    uint8_t time0 = json["data"][0]["time"];
    uint8_t time1 = json["data"][1]["time"];

    sManager.start(0, time0);
    sManager.start(1, time1);
  }
}

boolean mqttReconnect() {
  long now = millis();

  if (((now - lastReconnectAttempt) > 5000) && WiFi.isConnected()) 
  {
      Serial.println(F("Connecting to MQTT server..."));
      lastReconnectAttempt = now;

      // Attempt to reconnect
      if (mqttClient.connect("esp_sprinkler")) 
      {
        // Once connected, publish an announcement...
        mqttClient.publish("/home/sprinkler/status","online");
        // ... and resubscribe
        mqttClient.subscribe("/home/sprinkler/cmd");
      }
  }
 
  return mqttClient.connected();
}

void setup() {
  Serial.begin(115200);

  WiFi.mode(WIFI_STA);
  WiFi.setAutoReconnect(true);
  WiFi.setAutoConnect(true);
  onConnected = WiFi.onStationModeConnected(&onStationConnected);
  onDisconnected = WiFi.onStationModeDisconnected(&onStationDisconnected);
  onGotIp = WiFi.onStationModeGotIP(&onStationGotIp);
  WiFi.begin(ssid, password);

  Serial.println("");
  Serial.println("WiFi connected");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());

  //Led Pin
	pinMode ( LED, OUTPUT );
	digitalWrite ( LED, LOW );

  mqttClient.setServer(mqttIpAddr, 1883);
  mqttClient.setCallback(onMqttPayloadRcvd);

  Serial.println(F("Starting up..."));

  sManager.begin();
  sManager.addSprinkler(&area1Sprinkler);
  sManager.addSprinkler(&area2Sprinkler);
}

void loop() 
{
  
  if (mqttClient.connected()) 
  {
    mqttClient.loop();
    sManager.loop();
  }
  else
  {
    mqttReconnect();

    if(sManager.isPending())
    {
      sManager.stop();
    }
  } 
}

#endif