// Definicje pinów dla silników
#define EN1 10   // PWM dla silnika 1
#define IN1 8    // Kierunek 1 (silnik 1)
#define IN2 7    // Kierunek 2 (silnik 1)
#define EN2 11   // PWM dla silnika 2
#define IN3 12   // Kierunek 3 (silnik 2)
#define IN4 13   // Kierunek 4 (silnik 2)

void setup() {
  
  //Esp32 set_up
  // Ustawienie pinów dla silnika 1
  pinMode(EN1, OUTPUT);
  pinMode(IN1, OUTPUT);
  pinMode(IN2, OUTPUT);

  // Ustawienie pinów dla silnika 2
  pinMode(EN2, OUTPUT);
  pinMode(IN3, OUTPUT);
  pinMode(IN4, OUTPUT);

  // Ustawienie prędkości PWM dla obu silników
  analogWrite(EN1, 120);  // PWM dla silnika 1 (50% prędkości)
  analogWrite(EN2, 220);  // PWM dla silnika 2 (50% prędkości)
}

void loop() {
  // Jazda do przodu
  digitalWrite(IN1, HIGH);  // Silnik 1 do przodu
  digitalWrite(IN2, LOW);   // Silnik 1 do przodu
  digitalWrite(IN3, HIGH);  // Silnik 2 do przodu
  digitalWrite(IN4, LOW);   // Silnik 2 do przodu
  delay(20000);  // Jazda do przodu przez 2 sekundy

  // Skręt w prawo (wyłącz silnik 1)
  digitalWrite(IN1, LOW);   // Silnik 1 wstecz
  digitalWrite(IN2, LOW);   // Silnik 1 wyłączony
  digitalWrite(IN3, HIGH);  // Silnik 2 do przodu
  digitalWrite(IN4, LOW);   // Silnik 2 do przodu
  delay(20000);  // Skręt w prawo przez 2 sekundy

  // Skręt w lewo (wyłącz silnik 2)
  digitalWrite(IN1, HIGH);  // Silnik 1 do przodu
  digitalWrite(IN2, LOW);   // Silnik 1 do przodu
  digitalWrite(IN3, LOW);   // Silnik 2 wyłączony
  digitalWrite(IN4, LOW);   // Silnik 2 wyłączony
  delay(20000);  // Skręt w lewo przez 2 sekundy

  // Jazda do tyłu (tylko silnik 1)
  digitalWrite(IN1, LOW);   // Silnik 1 do tyłu
  digitalWrite(IN2, HIGH);  // Silnik 1 do tyłu
  digitalWrite(IN3, LOW);   // Silnik 2 wyłączony
  digitalWrite(IN4, LOW);   // Silnik 2 wyłączony
  delay(20000);  // Jazda do tyłu przez 2 sekundy

  // Jazda do tyłu (tylko silnik 2)
  digitalWrite(IN1, LOW);   // Silnik 1 do tyłu
  digitalWrite(IN2, HIGH);  // Silnik 1 do tyłu
  digitalWrite(IN3, LOW);   // Silnik 2 do tyłu
  digitalWrite(IN4, HIGH);  // Silnik 2 do tyłu
  delay(20000);  // Jazda do tyłu przez 2 sekundy
}
