# Lista elementów

- **BMS**: [Link do produktu](https://allegro.pl/oferta/778-bms-ladowarka-2s-10a-8-4v-ogniw-li-ion-18650-13720710459)
- **Regulator napięcia**: [Link do produktu](https://allegro.pl/oferta/010-przetwornica-napiecia-dc-step-down-lm2596-adj-konwerter-13562221957)
- **Baterie**: [Link do produktu](https://botland.com.pl/akumulatory-li-ion/23832-ogniwo-18650-li-ion-samsung-inr18650-25r-2500mah-20a.html)
- **Kosz na baterie**: [Link do produktu](https://botland.com.pl/koszyki-na-baterie/16517-koszyk-na-2-akumulatory-typu-18650-bez-przewodow-5904422344603.html)
- **L298N**: [Link do produktu](https://allegro.pl/oferta/034-sterownik-silnikow-dc-lub-krokowego-l298n-arduino-15113205771)
- **ESP32**: [Link do produktu](https://allegro.pl/oferta/sterownik-mikrobot-esp-32-esp-wroom-32-wifi-bluetooth-14819515317)
- **Serwo wieża**: [Link do produktu](https://allegro.pl/oferta/servo-serwo-tower-pro-sg90-9g-arduino-avr-9236603807)
- **Silniki**: do ustalenia

---

## Specyfikacja komponentów

- **BMS**: Zabezpiecza akumulatory przed rozładowaniem, zwarciem i przeładowaniem (nie pełni funkcji ładowarki).
- **Regulator napięcia**: Obniża napięcie do 5V, maksymalnie 3A (możliwość regulacji potencjometrem) dla ESP32 i jego peryferiów.
- **Baterie**: 2 akumulatory w konfiguracji 2S (maksymalne napięcie naładowanego ogniwa 4,2V, napięcie nominalne 3,7V, maksymalne rozładowanie 2,5V; BMS odcina przy około 3V).
- **ESP32 Devkit V1**: Mikrokontroler sterujący pracą L298N oraz odpowiedzialny za komunikację. Pin "VIN" połączony z wbudowaną przetwornicą 3,3V umożliwia zasilanie 5V.
- **L298N**: Sterownik silników, maksymalnie 2A na kanał. Napięcie logiczne jest obniżone przez wbudowany stabilizator. Maksymalne napięcie 5V-46V (BMS odcina przy napięciu około 6V podczas rozładowania akumulatorów).

---

## Teoretyczne testy natężenia prądu (A)

- **Baterie**: Maksymalny prąd rozładowania: 20A.
- **BMS**: Natężenie wyjściowe 10A.
- **Regulator**: Maksymalny prąd: 2A lub 3A z radiatorem.
- **L298N**: 2A na kanał + logika 0,5A.
- **ESP32**: Wbudowany regulator napięcia 3,3V o prądzie 500mA - 1A. Maksymalne natężenie prądu przy pracy ESP32 jako Access Point (punkt dostępu WiFi z technologią AJAX): ~250-700mA.

> [!CAUTION]
> **Uwagi:**
> - Serwa zwykle pracują na 5V i pobierają ~200-500mA, dlatego najlepiej podłączyć je do przetwornicy (GND-ESP/5V-Przetwornica/Sterowanie-ESP).
> - Dane przedstawiają teoretyczne wartości i nie zostały potwierdzone testami fizycznymi.

---

## Teoretyczne testy napięcia (V)

- **Baterie (x2)**: Maksymalne naładowanie akumulatorów 8,4V, maksymalne rozładowanie około 6V, praca przy około 7,4V.
- **BMS**: Napięcie nominalne: 7,4V. Napięcie rozładowania: 2,30V – 3,00V (po przekroczeniu tego zakresu następuje rozłączenie układu).
- **Regulator**: Napięcie wejściowe: 4,5V - 35V. Napięcie wyjściowe (regulowane): 1,5V - 35V.
- **L298N**: Napięcie zasilania: 5V - 46V, napięcie sygnałów logicznych: 5V (odseparowane od napięcia zasilania).
- **ESP32**: Wbudowany regulator akceptuje napięcie 5V wzwyż.

> [!CAUTION]
> **Uwagi:**
> - Dane przedstawiają teoretyczne wartości i nie zostały potwierdzone testami fizycznymi.

---

## Rozładowanie akumulatorów

Po rozładowaniu akumulatorów do około 6V, BMS rozłącza napięcie, co powoduje, że napięcie na przetwornicy nie powinno spaść poniżej 6V. To samo dotyczy L298N.

---

## Software

### Proces inicjalizacji serwera WWW

1. Uruchomienie ESP32.
2. Połączenie z siecią ESP_ACP.
3. Wejście na stronę hostowaną lokalnie.
4. Załadowanie HTML/CSS/JS/AJAX.
5. Uruchomienie procesów systemowych.

### Procesy

- Kontrolowanie systemu soft-start.
- Zabezpieczenie przed zmianą kierunku przed wyhamowaniem silników.
- Odbieranie danych z urządzenia.

**Priorytet systemowy:**
1. Kontrolowanie systemu soft-start i zabezpieczenie przed zmianą kierunku przed wyhamowaniem silników.
2. Odbieranie danych z urządzenia.

---

## Komunikacja z pilotem

1. Wysłanie paczki ze stroną WWW do urządzenia pilota.
2. Załadowanie strony przez urządzenie.
3. Możliwość wysyłania danych do ESP32 poprzez stronę (zapytania AJAX).
4. Odbiór danych i wykonanie nagłówków.

---

## System Soft-start

- Wymuszenie początkowo niskich prędkości PWM, aby zapobiec zbyt gwałtownym skokom natężenia prądu dla układu L298N.
- Zabezpieczenie przed zmianą kierunku przed wyhamowaniem silników polega na wprowadzeniu odpowiedniego opóźnienia pomiędzy zmianą biegu "N" i "R".
