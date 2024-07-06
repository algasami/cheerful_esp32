#include "utils.hpp"
#include <ArduinoJson.hpp>
#include <DNSServer.h>
#include <ESPAsyncWebServer.h>
#include <LittleFS.h>

class CaptiveRequestHandler : public AsyncWebHandler
{
  public:
    CaptiveRequestHandler()
    {
    }
    virtual ~CaptiveRequestHandler()
    {
    }

    bool canHandle(AsyncWebServerRequest *request)
    {
        return true;
    }

    void handleRequest(AsyncWebServerRequest *request)
    {
        request->send(LittleFS, "/setup.html");
    }
};

AsyncWebServer web_server(80);
DNSServer dns_server;
CaptiveRequestHandler captive_handler;

void setup_web_for_wifi()
{
    Serial.println("[Setup Web For WiFi] Initializing");

    web_server.on("/", HTTP_GET, [](AsyncWebServerRequest *req) { req->send(LittleFS, "/index.html"); });

    web_server.on("/setup", HTTP_GET, [](AsyncWebServerRequest *req) { req->send(LittleFS, "/setup.html"); });

    web_server.on("/init", HTTP_POST, [](AsyncWebServerRequest *request) {
        if (!request->hasParam("SSID", true) || !request->hasParam("Password", true))
        {
            request->send(400, "text/plain", "Corrupted input...");
            return;
        }
        AsyncWebParameter *ssid = request->getParam("SSID", true);
        AsyncWebParameter *pswd = request->getParam("Password", true);
        request->send(200, "text/plain", "Sent...");
        set_wifi(ssid->value(), pswd->value());
    });

    web_server.on("/status", HTTP_GET, [](AsyncWebServerRequest *req) {
        ArduinoJson::JsonDocument status;
        status["wifi_connected"] = Flags::bWiFiConnected;
        char serialize_buffer[100];
        ArduinoJson::serializeJson(status, serialize_buffer);
        req->send(200, "application/json", serialize_buffer);
    });

    web_server.addHandler(&captive_handler);
    web_server.begin();
    dns_server.start(53, "*", WiFi.softAPIP());
    Serial.println("[Setup Web For WiFi] Initialized");
}

void poll_web()
{
    dns_server.processNextRequest();
}