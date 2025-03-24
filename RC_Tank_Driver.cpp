#include "RC_Tank_Driver.h"

// Definicje pinów
#define IN1  25
#define IN2  26
#define ENA  27  // PWM kanału 1
#define IN3  33
#define IN4  32
#define ENB  14  // PWM kanału 2

// Zmienne sterujące PWM i biegiem
static int pwmValue1 = 0;
static int pwmValue2 = 0;
static int gearValue = 0;

// Utworzenie serwera HTTP na porcie 80
static WebServer server(80);

// Funkcja ustawiająca kierunek obrotu silników
void setMotorDirection(bool reverse) {
  if (reverse) {
    // Wsteczny: IN1 = LOW, IN2 = HIGH, IN3 = LOW, IN4 = HIGH
    digitalWrite(IN1, LOW);
    digitalWrite(IN2, HIGH);
    digitalWrite(IN3, LOW);
    digitalWrite(IN4, HIGH);
    Serial.println("[DEBUG] Kierunek ustawiony: WSTECZNY");
  } else {
    // Do przodu: IN1 = HIGH, IN2 = LOW, IN3 = HIGH, IN4 = LOW
    digitalWrite(IN1, HIGH);
    digitalWrite(IN2, LOW);
    digitalWrite(IN3, HIGH);
    digitalWrite(IN4, LOW);
    Serial.println("[DEBUG] Kierunek ustawiony: DO PRZODU");
  }
}

// Aktualizacja ustawień silników na podstawie biegu
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
      break;
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

// Funkcja obsługująca żądanie główne (HTML)
void handleRoot() {
  Serial.println("[DEBUG] handleRoot() - żądanie HTML");
  String html = R"rawliteral(
<!DOCTYPE html>
<html lang="pl">
<head>
  <meta charset="UTF-8">
  <meta name="viewport" content="width=device-width, initial-scale=1.0">
  <title>RC Tank Controller - Silniki</title>
  <style>
      body { margin: 0; font-family: 'Roboto', sans-serif; background: linear-gradient(135deg, #1e1e2f, #3b3b59); display: flex; justify-content: center; align-items: center; height: 100vh; }
      .controller { background: rgba(255, 255, 255, 0.1); border-radius: 20px; box-shadow: 0 4px 15px rgba(0, 0, 0, 0.2); backdrop-filter: blur(10px); padding: 20px; display: flex; flex-direction: column; align-items: center; color: #fff; width: 90%; max-width: 400px; }
      .slider-container { display: flex; flex-direction: column; align-items: center; margin-bottom: 20px; }
      .slider { width: 100%; height: 8px; background: #ddd; border-radius: 5px; outline: none; }
      .gearbox-container { display: flex; flex-direction: column; align-items: center; width: 100%; }
      .gearbox-container input[type="range"] { width: 100%; height: 8px; background: #ddd; border-radius: 5px; }
      .toggle-button { margin-top: 10px; padding: 10px; background: #4CAF50; color: white; border: none; border-radius: 5px; cursor: pointer; }
      .toggle-button.off { background: #d9534f; }
  </style>
</head>
<body>
  <div class="controller">
    <button id="syncButton" class="toggle-button">Sprzężenie: WŁĄCZONE</button>
    <div class="slider-container">
      <label for="motor1">Silnik 1</label>
      <input type="range" id="motor1" class="slider" min="0" max="220" value="0" step="1">
      <div class="output">Moc: <span id="motor1Output">0</span></div>
    </div>
    <div class="gearbox-container">
      <label for="gearbox">Skrzynia biegów</label>
      <input type="range" id="gearbox" min="-1" max="3" value="0" step="1">
      <div class="output">Bieg: <span id="gearboxOutput">Neutralny</span></div>
    </div>
    <div class="slider-container">
      <label for="motor2">Silnik 2</label>
      <input type="range" id="motor2" class="slider" min="0" max="220" value="0" step="1">
      <div class="output">Moc: <span id="motor2Output">0</span></div>
    </div>
  </div>
  <script>
    const motor1Slider = document.getElementById("motor1");
    const motor1Output = document.getElementById("motor1Output");
    const motor2Slider = document.getElementById("motor2");
    const motor2Output = document.getElementById("motor2Output");
    const gearboxSlider = document.getElementById("gearbox");
    const gearboxOutput = document.getElementById("gearboxOutput");
    const syncButton = document.getElementById("syncButton");
    let syncEnabled = true;
    
    function updateGearbox() {
        let gear = gearboxSlider.value;
        let gearText = "Neutralny";
        let maxPower = 0;
        switch (gear) {
            case "-1":
                gearText = "Wsteczny";
                maxPower = 73;
                break;
            case "0":
                gearText = "Neutralny";
                maxPower = 0;
                break;
            case "1":
                gearText = "Bieg 1";
                maxPower = 73;
                break;
            case "2":
                gearText = "Bieg 2";
                maxPower = 140;
                break;
            case "3":
                gearText = "Bieg 3";
                maxPower = 220;
                break;
        }
        gearboxOutput.textContent = gearText;
        motor1Slider.max = maxPower;
        motor2Slider.max = maxPower;
        if (parseInt(motor1Slider.value) > maxPower) {
            motor1Slider.value = maxPower;
        }
        if (parseInt(motor2Slider.value) > maxPower) {
            motor2Slider.value = maxPower;
        }
        fetch(`/setGear?gear=${gear}`)
          .then(response => console.log("[DEBUG] setGear response:", response.status))
          .catch(error => console.error("[DEBUG] setGear error:", error));
        updateMotors();
    }
    
    function updateMotors() {
        motor1Output.textContent = motor1Slider.value;
        motor2Output.textContent = motor2Slider.value;
        if (syncEnabled) {
            motor2Slider.value = motor1Slider.value;
            motor2Output.textContent = motor1Slider.value;
        }
        fetch(`/setPWM?ch=1&pwm=${motor1Slider.value}`)
          .then(response => console.log("[DEBUG] setPWM ch1 response:", response.status))
          .catch(error => console.error("[DEBUG] setPWM ch1 error:", error));
        fetch(`/setPWM?ch=2&pwm=${motor2Slider.value}`)
          .then(response => console.log("[DEBUG] setPWM ch2 response:", response.status))
          .catch(error => console.error("[DEBUG] setPWM ch2 error:", error));
    }
    
    function toggleSync() {
        syncEnabled = !syncEnabled;
        syncButton.textContent = syncEnabled ? "Sprzężenie: WŁĄCZONE" : "Sprzężenie: WYŁĄCZONE";
        syncButton.classList.toggle("off", !syncEnabled);
    }
    
    gearboxSlider.oninput = updateGearbox;
    motor1Slider.oninput = updateMotors;
    motor2Slider.oninput = updateMotors;
    syncButton.onclick = toggleSync;
    
    updateGearbox();
  </script>
</body>
</html>
  )rawliteral";
  
  server.send(200, "text/html", html);
  Serial.println("[DEBUG] handleRoot() - HTML wysłany");
}

// Funkcja obsługująca ustawienie PWM
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

// Funkcja obsługująca zmianę biegu
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

// Funkcja inicjująca sterownik
void driver_begin(const char* ssid, const char* password) {
  Serial.begin(115200);
  Serial.println("[DEBUG] Rozpoczynam setup()...");
  
  // Konfiguracja pinów jako wyjścia
  pinMode(IN1, OUTPUT);
  pinMode(IN2, OUTPUT);
  pinMode(ENA, OUTPUT);
  pinMode(IN3, OUTPUT);
  pinMode(IN4, OUTPUT);
  pinMode(ENB, OUTPUT);

  // Włączenie kanałów PWM
  digitalWrite(ENA, HIGH);
  digitalWrite(ENB, HIGH);

  // Łączenie z WiFi
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

  // Rejestracja obsługi żądań HTTP
  server.on("/", HTTP_GET, handleRoot);
  server.on("/setPWM", HTTP_GET, handleSetPWM);
  server.on("/setGear", HTTP_GET, handleSetGear);
  server.begin();
  Serial.println("[DEBUG] Serwer HTTP uruchomiony");

  // Ustawienie domyślnego kierunku (do przodu)
  setMotorDirection(false);
}

// Funkcja wywoływana w pętli głównej
void driver_loop() {
  server.handleClient();
  // Ustawienie PWM dla obu kanałów
  analogWrite(ENA, pwmValue1);
  analogWrite(ENB, pwmValue2);
  
  // Debugowanie - wypisywanie stanu PWM co 1 sekundę
  static unsigned long lastPrint = 0;
  if (millis() - lastPrint > 1000) {
    Serial.print("[DEBUG] Aktualne PWM - ENA: ");
    Serial.print(pwmValue1);
    Serial.print(" | ENB: ");
    Serial.println(pwmValue2);
    lastPrint = millis();
  }
}
