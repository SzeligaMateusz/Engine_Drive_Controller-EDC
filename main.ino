#include "driver.h"

void setup() {
    // Inicjalizacja systemu (WiFi, serwer, silniki, LED_BUILTIN)
    driver_begin();
    
    // Odkomentuj poniższą linię, jeśli chcesz zainicjalizować dodatkowy moduł, np. przyciski
    // initializeButtons();
}

void loop() {
    // Główna pętla systemu
    driver_loop();
    
    // Odkomentuj poniższą linię, aby dodać dodatkową logikę, np. obsługę przycisków lub czujników
    // processAdditionalModules();
}
