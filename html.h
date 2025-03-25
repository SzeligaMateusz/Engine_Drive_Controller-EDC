#ifndef HTML_H
#define HTML_H

const char htmlContent[] PROGMEM = R"rawliteral(
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
    <!-- Istniejące elementy sterujące -->
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
    <!-- Nowy przycisk do przełączania LED -->
    <button id="ledButton" class="toggle-button">Toggle LED</button>
  </div>
  <script>
    const motor1Slider = document.getElementById("motor1");
    const motor1Output = document.getElementById("motor1Output");
    const motor2Slider = document.getElementById("motor2");
    const motor2Output = document.getElementById("motor2Output");
    const gearboxSlider = document.getElementById("gearbox");
    const gearboxOutput = document.getElementById("gearboxOutput");
    const syncButton = document.getElementById("syncButton");
    const ledButton = document.getElementById("ledButton");
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
    
    // Obsługa przycisku LED – po kliknięciu wyślij żądanie do endpointu /toggleLED
    ledButton.onclick = function() {
        fetch('/toggleLED')
          .then(response => response.text())
          .then(state => console.log("[DEBUG] LED state:", state))
          .catch(error => console.error("[DEBUG] toggleLED error:", error));
    };
    
    gearboxSlider.oninput = updateGearbox;
    motor1Slider.oninput = updateMotors;
    motor2Slider.oninput = updateMotors;
    syncButton.onclick = toggleSync;
    
    updateGearbox();
  </script>
</body>
</html>
)rawliteral";

#endif // HTML_H
