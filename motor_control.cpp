#include "motor_control.h"
#include "config.h"
#include <Arduino.h>

// Definicje zmiennych globalnych
int pwmValue1 = 0;
int pwmValue2 = 0;
int gearValue = 0;

void setupMotors() {
    pinMode(IN1, OUTPUT);
    pinMode(IN2, OUTPUT);
    pinMode(ENA, OUTPUT);
    pinMode(IN3, OUTPUT);
    pinMode(IN4, OUTPUT);
    pinMode(ENB, OUTPUT);

    // Włączenie kanałów PWM
    digitalWrite(ENA, HIGH);
    digitalWrite(ENB, HIGH);
}

void setMotorDirection(bool reverse) {
    if (reverse) {
        // Kierunek wsteczny
        digitalWrite(IN1, LOW);
        digitalWrite(IN2, HIGH);
        digitalWrite(IN3, LOW);
        digitalWrite(IN4, HIGH);
        Serial.println("[DEBUG] Kierunek ustawiony: WSTECZNY");
    } else {
        // Kierunek do przodu
        digitalWrite(IN1, HIGH);
        digitalWrite(IN2, LOW);
        digitalWrite(IN3, HIGH);
        digitalWrite(IN4, LOW);
        Serial.println("[DEBUG] Kierunek ustawiony: DO PRZODU");
    }
}

void updateMotorsBasedOnGear() {
    int maxPower = 0;
    bool reverse = false;
  
    switch (gearValue) {
        case -1: // Wsteczny
            maxPower = 73;
            reverse = true;
            break;
        case 0: // Neutralny
            maxPower = 0;
            reverse = false;
            break;
        case 1: // Bieg 1
            maxPower = 73;
            reverse = false;
            break;
        case 2: // Bieg 2
            maxPower = 140;
            reverse = false;
            break;
        case 3: // Bieg 3
            maxPower = 220;
            reverse = false;
            break;
        default:
            Serial.print("[DEBUG] Nieznany bieg: ");
            Serial.println(gearValue);
            return;
    }

    Serial.print("[DEBUG] Ustawiony bieg: ");
    Serial.println(gearValue);
    Serial.print("[DEBUG] Maksymalna moc: ");
    Serial.println(maxPower);
  
    // Ograniczenie wartości PWM do maksymalnej mocy dla danego biegu
    pwmValue1 = min(pwmValue1, maxPower);
    pwmValue2 = min(pwmValue2, maxPower);
  
    Serial.print("[DEBUG] PWM Value 1: ");
    Serial.println(pwmValue1);
    Serial.print("[DEBUG] PWM Value 2: ");
    Serial.println(pwmValue2);
  
    // Ustawienie kierunku na podstawie flagi reverse
    setMotorDirection(reverse);
}
