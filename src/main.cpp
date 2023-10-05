#include "Arduino.h"
#include "Led.h"
#include "Button.h"
#include "WiFiAP1.h"

const char* ssid     = "Vicky";
const char* password = "vicky12345";

#define LED_PIN 2
#define BUTTON_PIN 0
#define BLINK_DELAY 250

Led led(LED_PIN);
Button button(BUTTON_PIN);
WiFiAP1 wifiAP1(ssid, password);

void setup()
{
  Serial.begin(115200);

  led.init(LOW);
  button.init();
  wifiAP1.init();
  wifiAP1.connect();
}

void loop()
{
  if(button.isPressed())
  {
    led.on();
    // Serial.println("Pressed");
  }
  else{
    led.off();
  }
}
