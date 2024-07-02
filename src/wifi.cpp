#include "utils.hpp"
#include <WiFi.h>

bool bWiFiConnected = false;
bool bWiFiSetup = false;

String ssid, password;

void setup_wifi()
{
    Serial.println("[Setup WiFi] Setting up AP_STA mode");
    WiFi.mode(WIFI_AP_STA);
    WiFi.softAP("Cheerful_ESP32", nullptr);
    Serial.println("[Setup WiFi] Successfully set up AP_STA mode");
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
        if (bWiFiConnected)
        {
            Serial.println("[Poll WiFi] WiFi Disconnected!");
            bWiFiConnected = false;
            setup_wifi();
        }
        return;
    }

    if (bWiFiConnected == false)
    {
        bWiFiConnected = true;
        Serial.printf("[Poll WiFi] WiFi Connected!\nIP:%s\n", WiFi.localIP().toString().c_str());
    }
}

void set_wifi(const String &in_ssid, const String &in_password)
{
    Serial.println("[Set WiFi] Received Data...");
    ssid = String(in_ssid);         // clone
    password = String(in_password); // clone
    Serial.printf("SSID:%s\nPassword:%s\n", ssid.c_str(), password.c_str());
    WiFi.begin(ssid, password);
    Serial.println("[Set WiFi] AP_STA Mode Complete!");
};