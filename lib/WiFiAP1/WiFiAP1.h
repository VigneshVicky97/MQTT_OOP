#ifndef WIFIAP1_H
#define WIFIAP1_H

#include "Arduino.h"
#include "WiFi.h"

#define CONNECTION_TIMEOUT 10

class WiFiAP1
{
  private:
    const char* ssid;
    const char* password;
    int timeout_counter = 0;
  public:
    WiFiAP1() {}
    
    WiFiAP1(const char* ssid, const char* password);
    void init();
    void connect();
};

#endif