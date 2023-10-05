#include "WiFiAP1.h"

WiFiAP1::WiFiAP1(const char* ssid, const char* password) 
{
    this->ssid = ssid;
    this->password = password;
}  

void WiFiAP1::init()
{
    WiFi.mode(WIFI_STA); //Optional
    WiFi.begin(ssid, password);
    Serial.println("\nConnecting");
    // connect();
}

void WiFiAP1::connect()
{
    while(WiFi.status() != WL_CONNECTED)
    {
        Serial.print(".");
        delay(200);
        timeout_counter++;
        if(timeout_counter >= CONNECTION_TIMEOUT*5)
        {
            ESP.restart();
        }
    }
    Serial.println("\nConnected to the WiFi network");
    Serial.print("Local ESP32 IP: ");
    Serial.println(WiFi.localIP());
}