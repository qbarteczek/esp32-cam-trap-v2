# ESP32-CAM Advanced Camera Trap V2

W pełni funkcjonalna, energooszczędna fotopułapka oparta o moduł **AI-Thinker ESP32-CAM**.

## Funkcje projektu
- 🔋 **Energooszczędność (Deep Sleep):** Pomiędzy zdjęciami moduł pobiera znikome ilości prądu i jest wybudzany sprzętowo przez czujnik ruchu (PIR).
- 💾 **Zapis na kartę SD:** Każde zdjęcie jest zapisywane na lokalnej karcie pamięci (tryb 1-bit dla oszczędności pinów).
- ⏱ **Czas rzeczywisty (NTP):** Pliki na karcie SD otrzymują prawidłową datę i godzinę (np. `2026-07-19_00-15-22.jpg`).
- 📡 **Web Portal i Menedżer plików:** Możesz przeglądać zawartość karty SD przez Wi-Fi, łącząc się z IP kamery w trybie serwisowym.
- ⚙️ **Konfiguracja w przeglądarce:** Sieć Wi-Fi oraz Token Telegrama możesz wpisać w interfejsie Web – zostaną zapisane w pamięci Flash, bez konieczności re-kompilacji kodu.
- 🚀 **Aktualizacje bezprzewodowe (OTA):** Umożliwia wgranie nowego kodu prosto z Arduino IDE, gdy urządzenie znajduje się w sieci domowej.
- 💬 **Dwukierunkowy Telegram:** Kamera wysyła zrobione zdjęcia na Twój telefon, po czym czeka kilka sekund na komendy zwrotne (np. `/live` aby uruchomić Web Server).

## Tryby pracy
Przejście między trybami sterowane jest za pomocą pinu **GPIO 12**:
1. **Tryb Aktywny (Fotopułapka)** – brak połączenia pinu 12. Moduł po wybudzeniu przez czujnik robi zdjęcie i ponownie usypia.
2. **Tryb Serwisowy (Web Server / OTA)** – pin 12 zwarty z masą (GND) podczas włączania zasilania. Kamera uruchamia serwer na porcie 80, udostępniając pliki i panel konfiguracyjny.

## Uruchomienie (Kompilacja)
Aby wgrać oprogramowanie, otwórz plik `src/esp32_cam_trap_v2/esp32_cam_trap_v2.ino` w Arduino IDE (lub użyj `arduino-cli`). Upewnij się, że masz doinstalowane biblioteki:
- `UniversalTelegramBot`
- `ArduinoJson`

Jeśli urządzenie jest puste (brak wpisanych sieci), utworzy własną sieć **`ESP32-CAM-Config`** z hasłem `12345678`. Połącz się z nią i wejdź na `http://192.168.4.1`, aby skonfigurować swój dostęp.

## Schemat podłączenia (wkrótce w docs/)
Szczegóły podłączenia konwertera UART i czujnika ruchu PIR (na GPIO 13) zostaną dodane do katalogu `/docs`.
