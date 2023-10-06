#include "Arduino.h"
#include "Led.h"
#include "Button.h"
#include "WiFiAP1.h"

// MyClass
#include "MyClass.h"
MyClass myClassObject;

#include <ArduinoJson.h>
#include <MQTTClient.h>
MQTTClient mqttClient;

#include <WiFi.h>

// const char* ssid     = "ACTFIBERNET";
// const char* password = "act12345";

const char* ssid = "Coconut-Porto3-EE9";
const char* password = "1234567890";

WiFiAP1 wifiAP1(ssid, password);

#define LED_PIN 2
#define BUTTON_PIN 0
#define BLINK_DELAY 250

Led led(LED_PIN);
Button button(BUTTON_PIN);

void setup()
{
  Serial.begin(115200);

  // Configure mqttClient with the necessary config with a json object:
  StaticJsonDocument<384> doc;
  JsonObject mqtt = doc.createNestedObject("mqtt");
  mqtt["enabled"] = true;
  mqtt["reconnect_mqtt"] = true;
  mqtt["reconnect_retries"] = 10;
  mqtt["reconnect_time_ms"] = 10000;
  mqtt["server"] = "broker.hivemq.com";
  mqtt["port"] = 1883;
  mqtt["id_name"] = "iot-button";
  mqtt["enable_user_and_pass"] = false;
  mqtt["user_name"] = "userName";
  mqtt["user_password"] = "userPassword";
  mqtt["enable_certificates"] = false;
  mqtt["ca_file"] = "/certs/ca.crt";
  mqtt["cert_file"] = "/certs/cert.der";
  mqtt["key_file"] = "/certs/private.der";
  mqtt["enable_websockets"] = false;
  mqtt["websockets_path"] = "/";
  mqtt["pub_topic"][0] = "/iot-button/feed";

  serializeJsonPretty(doc, Serial);
  mqttClient.setConfig(mqtt);
  // Set MyClass MQTT client and its observer callbacks:
  myClassObject.setMQTTClient(&mqttClient);


  led.init(LOW);
  button.init();
  wifiAP1.init();
  wifiAP1.connect();

  mqttClient.setup();

  Serial.println("###  Looping time\n");
}

void loop()
{
  // MyClass Loop:
  myClassObject.loop();
}
