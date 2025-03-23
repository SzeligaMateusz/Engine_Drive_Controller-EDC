#ifndef RC_TANK_DRIVER_H
#define RC_TANK_DRIVER_H

#include <Arduino.h>
#include <WiFi.h>
#include <WebServer.h>

// Funkcja inicjująca sterownik (konfiguracja pinów, WiFi, serwera itp.)
void driver_begin(const char* ssid, const char* password);

// Funkcja wywoływana w pętli głównej (obsługa klienta serwera, PWM itd.)
void driver_loop();

#endif // RC_TANK_DRIVER_H
