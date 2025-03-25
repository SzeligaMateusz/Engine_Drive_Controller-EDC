#include "driver.h"
#include "config.h"
#include "motor_control.h"
#include "server.h"
#include <Arduino.h>

void driver_begin() {
    Serial.begin(115200);
    Serial.println("[DEBUG] Rozpoczynam setup()...");
  
    // Inicjalizacja silników (ustawienie pinów i włączenie PWM)
    setupMotors();
  
    // Ustawienie domyślnego kierunku (do przodu)
    setMotorDirection(false);
  
    // Konfiguracja LED_BUILTIN (opcjonalnie – użyj, jeśli w projekcie korzystasz z diody)
    pinMode(LED_BUILTIN, OUTPUT);
    digitalWrite(LED_BUILTIN, LOW);
  
    // Łączenie z WiFi (używamy danych z config.cpp)
    WiFi.begin(ssid, password);
    Serial.print("[DEBUG] Łączenie z WiFi");
    while (WiFi.status() != WL_CONNECTED) {
        delay(500);
        Serial.print(".");
    }
    Serial.println();
    Serial.println("[DEBUG] Połączono z WiFi!");
    Serial.print("[DEBUG] Adres IP: ");
    Serial.println(WiFi.localIP());
  
    // Rejestracja obsługi tras HTTP
    setupServer();
    Serial.println("[DEBUG] driver_begin zakończony.");
}
  
void driver_loop() {
    // Obsługa żądań HTTP
    handleClientRequests();
  
    // Ustawienie PWM dla obu kanałów silników
    analogWrite(ENA, pwmValue1);
    analogWrite(ENB, pwmValue2);
  
    // Debugowanie – wypisywanie stanu PWM co 1 sekundę
    static unsigned long lastPrint = 0;
    if (millis() - lastPrint > 1000) {
        Serial.print("[DEBUG] Aktualne PWM - ENA: ");
        Serial.print(pwmValue1);
        Serial.print(" | ENB: ");
        Serial.println(pwmValue2);
        lastPrint = millis();
    }
  
    // Odkomentuj poniższą linię, aby dodać obsługę dodatkowych modułów
    // processAdditionalModules();
}
