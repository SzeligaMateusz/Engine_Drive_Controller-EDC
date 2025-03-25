#include "server.h"
#include "config.h"
#include "motor_control.h"
#include "html.h"
#include <Arduino.h>

void handleRoot() {
    Serial.println("[DEBUG] handleRoot() - żądanie HTML");
    server.send(200, "text/html", htmlContent);
    Serial.println("[DEBUG] handleRoot() - HTML wysłany");
}

void handleSetPWM() {
    if (server.hasArg("ch") && server.hasArg("pwm")) {
        int ch = server.arg("ch").toInt();
        int pwm = server.arg("pwm").toInt();
        if (ch == 1) {
            pwmValue1 = pwm;
            Serial.print("[DEBUG] handleSetPWM - Kanał 1, PWM: ");
            Serial.println(pwmValue1);
        } else if (ch == 2) {
            pwmValue2 = pwm;
            Serial.print("[DEBUG] handleSetPWM - Kanał 2, PWM: ");
            Serial.println(pwmValue2);
        }
    } else {
        Serial.println("[DEBUG] handleSetPWM - brak argumentów");
    }
    server.send(204); // No Content
}

void handleSetGear() {
    if (server.hasArg("gear")) {
        gearValue = server.arg("gear").toInt();
        Serial.print("[DEBUG] handleSetGear - Nowy bieg: ");
        Serial.println(gearValue);
        updateMotorsBasedOnGear();
    } else {
        Serial.println("[DEBUG] handleSetGear - brak argumentu 'gear'");
    }
    server.send(204);
}

// Nowy handler do przełączania wbudowanej diody LED
void handleToggleLED() {
    static bool ledState = false;
    ledState = !ledState;
    digitalWrite(LED_BUILTIN, ledState ? HIGH : LOW);
    Serial.print("[DEBUG] LED toggled to: ");
    Serial.println(ledState ? "ON" : "OFF");
    server.send(200, "text/plain", ledState ? "ON" : "OFF");
}

void setupServer() {
    server.on("/", HTTP_GET, handleRoot);
    server.on("/setPWM", HTTP_GET, handleSetPWM);
    server.on("/setGear", HTTP_GET, handleSetGear);
        // Rejestracja nowego endpointu /toggleLED
    server.on("/toggleLED", HTTP_GET, handleToggleLED);
    server.begin();
    Serial.println("[DEBUG] Serwer HTTP uruchomiony");
}

void handleClientRequests() {
    server.handleClient();
}
