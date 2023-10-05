#include "Arduino.h"
#include "Led.h"
#include "Button.h"
#include "WiFiAP1.h"
#include <PubSubClient.h>

// Your MQTT broker ID
const char *mqttBroker = "broker.hivemq.com";
int mqttPort = 1883;
// MQTT topics
const char *publishTopic = "halleffect";
const char *subscribeTopic = "led_oop";

unsigned long lastMsg = 0;
#define MSG_BUFFER_SIZE (5)
char msg[MSG_BUFFER_SIZE];

WiFiClient espClient;
PubSubClient client(espClient);


const char* ssid     = "Vicky";
const char* password = "vicky12345";

#define LED_PIN 2
#define BUTTON_PIN 0
#define BLINK_DELAY 250

Led led(LED_PIN);
Button button(BUTTON_PIN);
WiFiAP1 wifiAP1(ssid, password);

class MQTT1
{
  private:
    const char *mqttBroker;
    int mqttPort;
    const char *publishTopic;
    const char *subscribeTopic;

  public:
    MQTT1() {}

    void init(const char *mqttBroker, int mqttPort)
    {
      this->mqttBroker = mqttBroker;
      this->mqttPort = mqttPort;
      // setup the mqtt server and callback
      client.setServer(mqttBroker, mqttPort);
      // client.setCallback(callback);
    }

    // Callback function whenever an MQTT message is received
    void callback(char *topic, byte *payload, unsigned int length)
    {
      this->subscribeTopic = topic;

      Serial.print("Message arrived [");
      Serial.print(topic);
      Serial.print("] ");
      String message;
      for (int i = 0; i < length; i++)
      {
        Serial.print(message += (char)payload[i]);
      }
      Serial.println();

      // Switch on the LED if 'ON' was received
      if (message == "ON")
      {
        Serial.println("Turning ON Built In LED..");
        // led.on();
      }
      else
      {
        Serial.println("Turning OFF Built In LED..");
        // led.off();
      }
    }

    void reconnect()
    {
      // Loop until we're reconnected
      while (!client.connected())
      {
        Serial.print("Attempting MQTT connection...");

        // Create a random client ID
        String clientId = "ESP32Client-";
        clientId += String(random(0xffff), HEX);

        // Attempt to connect
        if (client.connect(clientId.c_str()))
        {
          Serial.println("connected");
          // Subscribe to topic
          client.subscribe(subscribeTopic);
        }
        else
        {
          Serial.print("failed, rc=");
          Serial.print(client.state());
          Serial.println(" try again in 5 seconds");
          // Wait 5 seconds before retrying
          delay(5000);
        }
      }

};

// Callback function whenever an MQTT message is received
void callback(char *topic, byte *payload, unsigned int length)
{
  Serial.print("Message arrived [");
  Serial.print(topic);
  Serial.print("] ");
  String message;
  for (int i = 0; i < length; i++)
  {
    Serial.print(message += (char)payload[i]);
  }
  Serial.println();

  // Switch on the LED if 'ON' was received
  if (message == "ON")
  {
    Serial.println("Turning ON Built In LED..");
    led.on();
  }
  else
  {
    Serial.println("Turning OFF Built In LED..");
    led.off();
  }
}

void reconnect()
{
  // Loop until we're reconnected
  while (!client.connected())
  {
    Serial.print("Attempting MQTT connection...");

    // Create a random client ID
    String clientId = "ESP32Client-";
    clientId += String(random(0xffff), HEX);

    // Attempt to connect
    if (client.connect(clientId.c_str()))
    {
      Serial.println("connected");
      // Subscribe to topic
      client.subscribe(subscribeTopic);
    }
    else
    {
      Serial.print("failed, rc=");
      Serial.print(client.state());
      Serial.println(" try again in 5 seconds");
      // Wait 5 seconds before retrying
      delay(5000);
    }
  }
}

void setup()
{
  Serial.begin(115200);

  led.init(LOW);
  button.init();
  wifiAP1.init();
  wifiAP1.connect();

  // setup the mqtt server and callback
  client.setServer(mqttBroker, mqttPort);
  client.setCallback(callback);
}

void loop()
{
  // if(button.isPressed())
  // {
  //   led.on();
  //   // Serial.println("Pressed");
  // }
  // else{
  //   led.off();
  // }

  // Listen for mqtt message and reconnect if disconnected
  if (!client.connected())
  {
    reconnect();
  }
  client.loop();

  // publish message after certain time.
  unsigned long now = millis();
  if (now - lastMsg > 10000)
  {
    lastMsg = now;
    // Read the Hall Effect sensor value
    int hallEffectValue = hallRead();

    snprintf(msg, MSG_BUFFER_SIZE, "%d", hallEffectValue);
    Serial.print("Publish message: ");
    Serial.println(msg);
    client.publish(publishTopic, msg);
  }
}
