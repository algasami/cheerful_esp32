#include "utils.hpp"
#include <WiFi.h>

bool bWiFiConnected = false;
bool bWiFiSetup = false;

String ssid, password;

void setup_wifi()
{
    Serial.println("[Setup WiFi] Setting up AP mode");
    WiFi.mode(WIFI_AP);
    WiFi.softAP("Cheerful_ESP32", nullptr);
    Serial.println("[Setup WiFi] Successfully set up AP mode");
}

void poll_wifi(unsigned long int current_time)
{
    static unsigned long int last = millis();
    if (current_time - last < 1000)
    {
        return;
    }
    last = current_time;

    if (WiFi.status() != WL_CONNECTED)
    {
        bWiFiConnected = false;
        return;
    }
    if (bWiFiConnected == false)
    {
        bWiFiConnected = true;
        Serial.println("[Poll WiFi] WiFi Connected!");
    }
}

void set_wifi(const String &in_ssid, const String &in_password)
{
    Serial.println("[Set WiFi] Received Data...");
    WiFi.mode(WIFI_STA);
    ssid = String(in_ssid);         // clone
    password = String(in_password); // clone
    WiFi.begin(ssid, password);
    Serial.println("[Set WiFi] STA Mode Complete!");
};