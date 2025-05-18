# 🌱 ESP32 LoRa Firebase Monitoring (TTGO T-Higrow)

Proyek ini terdiri dari dua bagian utama:

- **loRaTransmitter**: Mengambil data sensor suhu & kelembapan menggunakan DHT11, lalu mengirimkannya melalui LoRa.
- **loRaReceiver**: Menerima data LoRa dan mengirimkannya ke **Firebase Realtime Database**.

Keduanya berjalan di board **LILYGO TTGO T-Higrow** berbasis ESP32 dan LoRa SX1276/78.

---

## 📂 Struktur Folder

```
├── loRaReceiver/
│   ├── loraReceiver.ino
├── loRaTransmitter/
│   └── loraTransmitter.ino
│   .gitignore              ← Untuk mengabaikan loRaReceiver/secrets.h
│   secrets.example.h       ← Contoh file credentials
```

---
## 🔧 loRaTransmitter

### Fungsi

- Membaca suhu dan kelembapan dari sensor **DHT11**
- Mengontrol 2 relay (lampu & kipas)
- Mengirimkan data LoRa setiap 2 detik

### 📡 Format Data LoRa
```
temp:25.3,hum:68.0
```

## 💡 Logika Relay
- Relay 1 (GPIO 33): Lampu — aktif saat LOW
- Relay 4 (GPIO 32): Kipas menyala jika suhu > 38°C

## 🧪 Sensor & Pinout
```
| Komponen        | Pin ESP32 (TTGO) |
| --------------- | ---------------- |
| DHT11           | GPIO 15          |
| Relay 1 (lampu) | GPIO 33          |
| Relay 4 (kipas) | GPIO 32          |
```

## 🔌 LoRa SPI Pinout
```
| LoRa Pin | ESP32 GPIO |
| -------- | ---------- |
| SCK      | 5          |
| MOSI     | 27         |
| MISO     | 19         |
| SS       | 18         |
| RST      | 23         |
| DIO0     | 26         |
```

## 🌐 loRaReceiver + Firebase
### Fungsi
- Koneksi ke WiFi
- Autentikasi dengan Firebase menggunakan API Key
- Parsing data LoRa
- Kirim data suhu & kelembapan ke Firebase Realtime Database sebagai JSON

```
{
  "temperature": 25.3,
  "humidity": 68.0
}
```

## 🔐 File secrets.h
Buat file secrets.h di loRaReceiver/ berdasarkan contoh ini:

```
#ifndef SECRETS_H
#define SECRETS_H

#define WIFI_SSID "your_wifi_ssid"
#define WIFI_PASSWORD "your_wifi_password"
#define API_KEY "your_firebase_api_key"
#define DATABASE_URL "https://your-project-id.firebaseio.com/"

#endif
```

## 📥 Instalasi Library
Install library berikut di Arduino IDE:
- Firebase ESP Client
- LoRa by Sandeep Mistry
- DHT sensor library

## 📋 Cara Kerja Sistem
1. loRaTransmitter membaca data dari sensor dan mengirimkan string melalui LoRa.
2. loRaReceiver menerima string, parsing suhu & kelembapan.
3. Data dikirim ke Firebase dalam bentuk JSON.
4. Firebase bisa diakses untuk monitoring real-time melalui dashboard atau aplikasi.

## 🛠️ Firebase Setup
1. Buat project di Firebase Console
2. Aktifkan Realtime Database
3. Atur rules sementara untuk testing:
```
{
  "rules": {
    ".read": true,
    ".write": true
  }
}
```
⚠️ Gantilah rules ini sebelum digunakan di lingkungan produksi.