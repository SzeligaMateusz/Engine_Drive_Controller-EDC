#include "RC_Tank_Driver.h"

// Dane WiFi (SSID i hasło)
const char* ssid = "PLAY_Swiatlowod_69A5";
const char* password = "s5nZ7%2wEFEr";

void setup() {
  driver_begin(ssid, password);
}

void loop() {
  driver_loop();
}
