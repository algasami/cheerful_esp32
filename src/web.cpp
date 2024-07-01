#include "utils.hpp"
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
        Serial.println("[Captive Handler] Responding...");
        request->send(LittleFS, "/setup.html");
        Serial.println("[Captive Handler] Sent!");
    }
};

AsyncWebServer *web_server;
DNSServer *dns_server;
CaptiveRequestHandler *captive_handler;

void setup_web_for_wifi()
{
    Serial.println("[Setup Web For WiFi] Initializing");
    dns_server = new DNSServer;
    captive_handler = new CaptiveRequestHandler;
    web_server = new AsyncWebServer(80);
    web_server->on("/init", HTTP_POST, [](AsyncWebServerRequest *request) {
        if (!request->hasParam("SSID", true) || !request->hasParam("Password", true))
        {
            request->send(400, "text/plain", "Corrupted input...");
            return;
        }
        AsyncWebParameter *ssid = request->getParam("SSID", true);
        AsyncWebParameter *pswd = request->getParam("Password", true);
        set_wifi(ssid->value(), pswd->value());
        request->send(200, "text/plain", "Sent...");
    });
    web_server->addHandler(captive_handler);
    web_server->begin();
    dns_server->start(53, "*", WiFi.softAPIP());
    Serial.println("[Setup Web For WiFi] Initialized");
}

bool b_cleanuped = false;
void cleanup_web_for_wifi()
{
    Serial.println("[Setup Web For WiFi] Cleaning up");
    dns_server->stop();
    web_server->end();
    delete web_server;
    delete dns_server;
    Serial.println("[Setup Web For WiFi] Cleaned up");
}

void poll_dns()
{
    if (!bWiFiConnected)
    {
        dns_server->processNextRequest();
    }
    else if (!b_cleanuped)
    {
        b_cleanuped = true;
        cleanup_web_for_wifi();
    }
}