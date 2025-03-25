#ifndef MOTOR_CONTROL_H
#define MOTOR_CONTROL_H

// Funkcje inicjalizacji i sterowania silnikami
void setupMotors();
void setMotorDirection(bool reverse);
void updateMotorsBasedOnGear();

// Zmienne globalne sterujące PWM i biegiem
extern int pwmValue1;
extern int pwmValue2;
extern int gearValue;

#endif // MOTOR_CONTROL_H
