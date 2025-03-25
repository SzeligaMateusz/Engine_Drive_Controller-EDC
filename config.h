#ifndef CONFIG_H
#define CONFIG_H

#include <WiFi.h>
#include <WebServer.h>

// Definicje pinów silników
#define IN1  25
#define IN2  26
#define ENA  27  
#define IN3  33
#define IN4  32
#define ENB  14  

// Dane do WiFi – deklarowane jako extern
extern const char* ssid;
extern const char* password;

// Deklaracja instancji serwera HTTP
extern WebServer server;

#endif // CONFIG_H
