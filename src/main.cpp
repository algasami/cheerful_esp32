#include "utils.hpp"
#include <LittleFS.h>

void setup()
{
    Serial.begin(9600);
    Serial.println("[LittleFS] Mounting...");
    if (!LittleFS.begin())
    {
        Serial.println("[LittleFS] Fatal Error!");
    }
    else
    {
        Serial.println("[LittleFS] Mounted!");
    }
    setup_wifi();
    setup_web_for_wifi();
}

void loop()
{
    unsigned long current_time = millis();
    poll_wifi(current_time);
    poll_dns();
}
