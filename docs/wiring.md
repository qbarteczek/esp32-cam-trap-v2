# Schemat i podłączenie ESP32-CAM (Fotopułapka V2)

Ze względu na to, że układ ESP32-CAM nie posiada wbudowanego portu USB, wszystkie programowania i podłączenia peryferiów wykonujemy używając fizycznych pinów.

---

## 1. Wgrywanie programu (Kompilacja/Flash)
Aby wgrać oprogramowanie, będziesz potrzebował konwertera USB-UART (np. FT232RL, CP2102) ustawionego na logikę **3.3V** (niektóre mogą działać na 5V, ESP32-CAM posiada pin wejściowy 5V, ale sygnały RX/TX muszą pracować na poziomie 3.3V).

**Połączenie Konwerter USB -> ESP32-CAM:**
*   `GND` -> `GND`
*   `5V` (lub VCC) -> `5V`
*   `TX` -> `U0R` (RX kamery)
*   `RX` -> `U0T` (TX kamery)
*   **Ważne (Tryb Flash):** Połącz pin `IO0` z `GND`. Zewrzyj je przed podłączeniem zasilania lub wciśnij przycisk RESET na płytce po podłączeniu konwertera.

Po pomyślnym wgraniu kodu (komunikat w Arduino IDE), **odłącz zworkę `IO0` - `GND`** i wciśnij przycisk RESET na płytce, aby uruchomić swój kod.

---

## 2. Podłączenie docelowe (W terenie / W domu)

Po wgraniu programu odłączamy konwerter (lub zostawiamy podłączone tylko zasilanie z przetwornicy / baterii PowerBank 5V).

### A. Czujnik Ruchu (PIR - np. HC-SR501 lub AM312)
Urządzenie domyślnie "śpi". Wybudzenie realizowane jest przez zmianę stanu pinu na `HIGH`.

*   **VCC czujnika** -> `5V` lub `3.3V` (zależnie od tego, jakiego czujnika użyjesz, AM312 działa świetnie z pinu 3.3V)
*   **GND czujnika** -> `GND` kamery
*   **OUT (Sygnał) czujnika** -> `IO13` kamery

*Uwaga: HC-SR501 posiada potencjometry – zaleca się skręcenie czasu opóźnienia na minimum.*

### B. Przełącznik trybu pracy (Web Server / Fotopułapka)
Dla wygody zastosowaliśmy odczyt z pinu. Pozwala to fizycznym guzikiem lub zworką decydować, jak uruchomi się kamera.

*   Jeśli pin `IO12` jest niepołączony -> Kamera włącza się w **Trybie Fotopułapki** (Deep Sleep, reaguje na ruch).
*   Jeśli podłączysz pin `IO12` z `GND` (np. za pomocą małego fizycznego przełącznika "pstryczka" typu hebelkowego na obudowie) przed zasileniem kamery -> Kamera wejdzie w **Tryb Serwisowy** (uruchomi Web Server, udostępni pliki).

### C. Zasilanie bateryjne
Fotopułapka powinna być zasilana ze sprawdzonych źródeł (np. koszyk z ogniwami 18650 i układem ładującym TP4056 + Step-up podający stabilne 5V na wejście 5V kamery). 
Kamery ESP32-CAM bywają wrażliwe na jakość zasilania, dlatego mocny i czysty prąd w momencie włączenia Wi-Fi jest kluczowy do uniknięcia restartów w postaci "Brownout".
