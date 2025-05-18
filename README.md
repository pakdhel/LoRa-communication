# 🌱 ESP32 LoRa Receiver + Firebase RTDB (TTGO T-Higrow)
Proyek ini menggunakan ESP32 LILYGO TTGO T-Higrow LoRa Shield untuk menerima data dari jaringan LoRa dan mengirimkan hasilnya ke Firebase Realtime Database. Data yang dikirim berupa suhu dan kelembapan.

## 📂 Struktur Folder
```LoRa/
├── LoRaReceiver/
│   ├── loraReceiver.ino
├── LoRaTransmitter/
│   └── loraTransmitter.ino
│   .gitignore              ← Untuk mengabaikan loRaReceiver/secrets.h
│   secrets.example.h       ← Contoh file credentials
```
## 🛠️ Fitur
- Menerima data via LoRa (modul LoRa internal TTGO T-Higrow)
- Koneksi WiFi
- Kirim data ke Firebase RTDB dalam bentuk JSON
- Token otomatis via Firebase_ESP_Client

## 🔧 Persiapan
1. Clone repository
2. Buat file secrets.h
3. Buat file LoRaReceiver/secrets.h berdasarkan secrets.example.h:

```#ifndef SECRETS_H
#define SECRETS_H

#define WIFI_SSID "your_wifi_ssid"
#define WIFI_PASSWORD "your_wifi_password"
#define API_KEY "your_firebase_api_key"
#define DATABASE_URL "your_firebase_database_url"

#endif
```

⚠️ Jangan upload secrets.h ke GitHub — file ini sudah masuk .gitignore.

## 📦 Library yang Dibutuhkan
Install melalui Library Manager di Arduino IDE:
- Firebase ESP Client
- LoRa by Sandeep Mistry

## 🖥️ Perangkat Keras
- ESP32
- LILYGO TTGO T-Higrow Lora Shield 


## ⚙️ Pinout LoRa (TTGO T-Higrow)
LoRa        ESP32 (TTGO T-Higrow)
SCK   →     GPIO5
MISO  →     GPIO19
MOSI  →     GPIO27
SS    →     GPIO18
RESET →     GPIO23
DIO0  →     GPIO26
📝 Pin ini sudah dikonfigurasi di sketch loraReceiver.ino.

## 📡 Format Data LoRa
Pastikan transmitter mengirim string dengan format seperti:

temp:25.5,hum:70.2

Receiver akan parsing dan menyimpan ke Firebase sebagai:

```{
  "temperature": 25.5,
  "humidity": 70.2
}
```
## ☁️ Konfigurasi Firebase
Aktifkan Realtime Database di Firebase Console

Atur rules sementara untuk pengujian:

{
  "rules": {
    ".read": true,
    ".write": true
  }
}

⚠️ Jangan gunakan aturan ini untuk produksi.
