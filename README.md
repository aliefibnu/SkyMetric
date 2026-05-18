# SkyMetric ESP32 + Blynk

Project ini membaca DHT22 dan GUVA dari ESP32, mengontrol LED, lalu mengirim data ke Blynk (tanpa serial print telemetry).

## Struktur File

- `skymetric.ino`: entry point utama.
- `Config.h`: konstanta pin, ADC, dan interval update.
- `LedService.h/.cpp`: kontrol LED dan pembentukan JSON status LED.
- `SensorService.h/.cpp`: pembacaan DHT22 + GUVA.
- `BlynkService.h/.cpp`: koneksi Blynk dan pengiriman datastream.
- `secrets.h`: kredensial lokal (tidak boleh di-commit).
- `secrets.example.h`: template kredensial.

## Dependensi Arduino Library

Install library berikut di Arduino IDE:

1. `Blynk`
2. `DHT sensor library` (Adafruit)
3. `Adafruit Unified Sensor`

Pastikan board ESP32 sudah ter-install (`ESP32 by Espressif Systems`).

## Setup Kredensial (Environment Lokal)

1. Buka `secrets.example.h`.
2. Salin isi file itu ke `secrets.h`.
3. Isi nilai berikut di `secrets.h`:
   - `BLYNK_TEMPLATE_ID`
   - `BLYNK_TEMPLATE_NAME`
   - `BLYNK_AUTH_TOKEN`
   - `WIFI_SSID`
   - `WIFI_PASSWORD`

## Mapping Datastream Blynk

1. `V1` (String): JSON status LED
   - Format: `{"onboard":0|1,"green":0|1,"red":0|1}`
2. `V2` (Integer): suhu celcius
3. `V3` (Integer): kelembapan persen
4. `V4` (Integer): GUVA raw ADC (0-4095)
5. `V5` (Integer): GUVA centivolt (x100), contoh `330` = `3.30V`

## Cara Jalanin

1. Buka `skymetric.ino` di Arduino IDE.
2. Pilih board `ESP32 Dev Module` dan port yang benar.
3. Upload sketch.

## Catatan Runtime

- LED onboard, hijau, merah berkedip tiap 1 detik.
- Sensor dibaca tiap 2 detik agar sesuai karakteristik DHT22.
- Jika DHT gagal dibaca, nilai `V2` dan `V3` dikirim sebagai `-1`.
