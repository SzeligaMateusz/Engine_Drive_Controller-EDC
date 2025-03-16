Lista elementów:
BMS                 https://allegro.pl/oferta/778-bms-ladowarka-2s-10a-8-4v-ogniw-li-ion-18650-13720710459
Regulator napięcia  https://allegro.pl/oferta/010-przetwornica-napiecia-dc-step-down-lm2596-adj-konwerter-13562221957
Baterie             https://botland.com.pl/akumulatory-li-ion/23832-ogniwo-18650-li-ion-samsung-inr18650-25r-2500mah-20a.html
kosz na baterie     https://botland.com.pl/koszyki-na-baterie/16517-koszyk-na-2-akumulatory-typu-18650-bez-przewodow-5904422344603.html
L298N               https://allegro.pl/oferta/034-sterownik-silnikow-dc-lub-krokowego-l298n-arduino-15113205771
ESP32               https://allegro.pl/oferta/sterownik-mikrobot-esp-32-esp-wroom-32-wifi-bluetooth-14819515317
Serwo wieża         https://allegro.pl/oferta/servo-serwo-tower-pro-sg90-9g-arduino-avr-9236603807
Silniki             do ustalenia
------------------------------------------------------------------------------------------------------------------------------------------------
BMS - Zabezpieczenie akumulatorów przed rozładowaniem, zwarciem, przeładowaniem(nie stosujemy go jako ładowarki).
Regulator - obniża napięcie do 5V i MAX 3A (można go regulować potencjometrem) dla esp32 i jego peryferiom, 
Baterie - 2 Akumulatory w konfiguracji 2S (MAX napięcie naładowanego ogniwa pojedynczego 4,2V). Napięcie nominalne: 3,7 V. Rozładownie MAX 2,5V (BMS odcina przy ok 3V).
ESP32 Devkit V1 - Mikrokontroler sterujący pracą L298N, oraz zajmujący się komunikacją. Pin "VIN" połączony z wbudowaną przetwornicą 3,3V daje możliwość podpięcia 5V jako zasilanie.
L298N - Sterownik silników, Max 2A na kanał. Napięcie logiczne jest obniżone przez wbudowany stabilizator. Max napięcie od 5V-46V(BMS odcina napięcie przy ok 6V rozładowania akumulatorów)
------------------------------------------------------------------------------------------------------------------------------------------------
Teoretyczne testy natężenia prądu (A):
        Baterie - Maksymalny prąd rozładowania: 20 A;
	BMS - Natężenie wyjściowe 10A;
        Regulator - Maksymalny prąd: 2A lub 3A z radiatorem;
        L298N - 2A na kanał + logika 0,5A;
        ESP32 - ESP posiada wbudowany regulator napięcia 3,3V i 500mA - 1A. Maksymalne natężenie prądu podczas pracy esp32 jako Akcess Poin(punkt dostępu WIFI z technologią AJAX ~250-700mA);
	|!|Należy pamiętać, że serwa najczęściej pracują na 5V i ok~200-500mA wy wyniku czego najlepiej będzie je podpiąć do przetwornicy(GND-ESP/5V-Przetwornica/Sterownie-ESP)|!|
|!!|Dane przedstawiają teoretyczne wartości i nie są poparte w testach fizycznych|!!|
-------------------------------------------------------------------------------------------------------------------------------------------------
Teoretyczne testy napięcia (V):
        Baterie x2 - Maksymalne naładowanie akumulatorów 8,4V. Maksymalne rozładowanie ok~6V. Praca ok~7,4V;
	BMS - Napięcie nominalne: 7,4V. Napięcie rozładowania: 2,30 – 3,00V(po przekroczeniu następuje rozłączenie układu);
        Regulator - Napięcie wejściowe 4,5V - 35V. Napięcie wyjściowe (regulowane): 1,5V - 35V;
        L298N - Napięcie zasilania: 5V - 46V, Napięcie sygnałów logicznych: 5V (zostaje odseparowane z napięcia zasilania);
        ESP32 - Wbudowany regulator akceptuje napięcie 5V wzwyż;
|!!|Dane przedstawiają teoretyczne wartości i nie są poparte w testach fizycznych|!!|
------------------------------------------------------------------------------------------------------------------------------------------------
Połączenie modułów:
        2 Ogniwa -> BMS -> L298N oraz Przetwornica -> ESP32
------------------------------------------------------------------------------------------------------------------------------------------------
Rozładowanie akumulatorów:
        Po rozładowaniu akumulatorów do ok~6V bms rozłącza napięcie w wyniku czego napięcie na przetwornicy nie powinno spoaść poniżej 6V również tyczy się to L298N.
------------------------------------------------------------------------------------------------------------------------------------------------
Softwere:
       Proces inicjalizacji serwera WWW: Uruchomienie ESP -> Połączenie się z siecią ESP_ACP -> Wejście na specjalną stronę hostowaną lokalnie -> (Tajne operacje: Załadowanie Html/Css/JS/AJAX) -> (Uruchomienie procesów systemowych).
       Procesy: Kontrolowanie systemu soft-start + zabezpieczenie przed zmianą kierunku przed wyhamowaniem silników. Odbieranie danych z urządzenia.
       Priorytet systemowy: 1.Kontrolowanie systemu soft-start + zabezpieczenie przed zmianą kierunku przed wyhamowaniem silników; 2.Odbieranie danych z urządzenia;
------------------------------------------------------------------------------------------------------------------------------------------------
Komunikacja z pilotem:
       Sekwencja: Wysłanie paczki ze stroną WWW do urządzenia pilota -> Załadowanie strony przez urządzenie -> Możliwość wysyłania danych do esp poprzez stronę(zapytań AJAX) -> Odbiór danych i wykonanie nagłówków.
------------------------------------------------------------------------------------------------------------------------------------------------
System Soft-start:
       Wymuszenie początkowo niskich prędkości PWM aby zapobiec zbyt gwałtownym skokom natężenia prądu dla układu L298N.
       Zabezpieczenie przed zmianą kierunku przed wyhamowaniem silników polega na wprowadzenie odpowiedniego opóźnienia pomiędzy zmianą biegu "N" i "R".
------------------------------------------------------------------------------------------------------------------------------------------------

        
