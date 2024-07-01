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

void setup_web_for_wifi()
{
    Serial.println("[Setup Web For WiFi] Initializing");
    dns_server = new DNSServer;
    web_server = new AsyncWebServer(80);
    web_server->on("/init", HTTP_POST, [](AsyncWebServerRequest *request) {
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
    web_server->addHandler(new CaptiveRequestHandler); // this would get cleaned up by websever
    web_server->begin();
    dns_server->start(53, "*", WiFi.softAPIP());
    Serial.println("[Setup Web For WiFi] Initialized");
}

void cleanup_web_for_wifi()
{
    Serial.println("[Setup Web For WiFi] Cleaning up");
    dns_server->stop();
    web_server->end();
    delete web_server;
    delete dns_server;
    Serial.println("[Setup Web For WiFi] Cleaned up");
}

void setup_web()
{
    Serial.println("[Setup Web] Initializing");
    web_server = new AsyncWebServer(80);
    web_server->on("/", HTTP_GET, [](AsyncWebServerRequest *req) { req->send(200, "text/plain", "Welcome!"); });
    Serial.println("[Setup Web] Initialized");
    web_server->begin();
}

void cleanup_web()
{
    web_server->end();
    delete web_server;
}

void poll_web()
{
    if (!bWiFiConnected)
    {
        dns_server->processNextRequest();
    }
}