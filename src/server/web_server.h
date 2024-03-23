#ifndef WEB_SERVER_H
#define WEB_SERVER_H

#include <WiFi.h>
#include <WebServer.h>

extern WebServer axsServer;

void handleRoot();

void setupWebServer();

#endif // WEB_SERVER_H