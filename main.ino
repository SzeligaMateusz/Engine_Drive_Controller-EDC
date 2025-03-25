#include "RC_Tank_Driver.h"

// Dane WiFi (SSID i hasło)
const char* ssid = "ssid";
const char* password = "password";

void setup() {
  driver_begin(ssid, password);
}

void loop() {
  driver_loop();
}
