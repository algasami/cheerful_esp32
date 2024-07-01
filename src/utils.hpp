#pragma once
#include <Arduino.h>
#include <string>

void setup_wifi();
void setup_web_for_wifi();
void cleanup_web_for_wifi();
void set_wifi(const String &ssid, const String &password);
void poll_wifi(unsigned long int);
void poll_web();
void setup_web();
void cleanup_web();

extern bool bWiFiConnected;
extern bool bWiFiSetup;