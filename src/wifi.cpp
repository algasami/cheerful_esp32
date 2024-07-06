#include "utils.hpp"
#include <WiFi.h>
#include <string.h>

bool Flags::bWiFiConnected = false;
bool Flags::bWiFiSetup = false;

// * .bss-allocated to prevent heap frag
char ssid_cache[20], password_cache[20];

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
        if (Flags::bWiFiConnected)
        {
            Serial.println("[Poll WiFi] WiFi Disconnected!");
            Flags::bWiFiConnected = false;
            setup_wifi();
        }
        return;
    }

    if (Flags::bWiFiConnected == false)
    {
        Flags::bWiFiConnected = true;
        Serial.printf("[Poll WiFi] WiFi Connected!\nIP:%s\n", WiFi.localIP().toString().c_str());
    }
}

void set_wifi(const String &in_ssid, const String &in_password)
{
    Serial.println("[Set WiFi] Received Data...");
    strcpy(ssid_cache, in_ssid.c_str());         // clone
    strcpy(password_cache, in_password.c_str()); // clone
    Serial.printf("SSID:%s\n", ssid_cache);
    WiFi.begin(ssid_cache, password_cache);
    Serial.println("[Set WiFi] AP_STA Mode Complete!");
}