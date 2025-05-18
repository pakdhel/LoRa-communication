#include "secrets.h"
#include <LoRa.h>
#include <SPI.h>
#include <Wire.h>
#include <WiFi.h>
#include <Firebase_ESP_Client.h>

// Provide the token generation process info.
#include <addons/TokenHelper.h>

// Provide the RTDB payload printing info and other helper functions.
#include <addons/RTDBHelper.h>

// Define Firebase Data object
FirebaseData fbdo;

FirebaseAuth auth;
FirebaseConfig config;
unsigned long sendDataPrevMillis = 0;
bool signupOK = false;

// pin LORA
#define SCK   5
#define MOSI  27
#define MISO  19
#define SS    18
#define RST   23
#define DIO0  26

void initWifi() {
  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
  Serial.print("Connecting to Wi-Fi");
  while (WiFi.status() != WL_CONNECTED)
  {
    Serial.print(".");
    delay(300);
  }
  Serial.println();
  Serial.print("Connected with IP: ");
  Serial.println(WiFi.localIP());
  Serial.println();
}

void initFirebase() {
  /* Assign the api key (required) */
  config.api_key = API_KEY;

  /* Assign the RTDB URL (required) */
  config.database_url = DATABASE_URL;

  /* Sign up */
  if (Firebase.signUp(&config, &auth, "", "")){
    Serial.println("ok");
    signupOK = true;
  }
  else{
    Serial.printf("%s\n", config.signer.signupError.message.c_str());
  }

  /* Assign the callback function for the long running token generation task */
  config.token_status_callback = tokenStatusCallback; //see addons/TokenHelper.h
  
  Firebase.begin(&config, &auth);
  Firebase.reconnectWiFi(true);
}

void initLora() {
  Serial.println("Lora sender example");
  // delay(2000);

  SPI.begin(SCK,MISO,MOSI,SS);
  LoRa.setPins(SS,RST,DIO0);

  while (!LoRa.begin(915E6)) {
    Serial.println("Starting LoRa failed!");
    delay(500);
    // while (1);
  }

  LoRa.setSyncWord(0xF3);
  Serial.println("Lora OK");
}

void setup() {
  Serial.begin(115200);
  while (!Serial);

  initWifi();
  initFirebase();
  initLora();
  
  delay(1000);
}

void receivedLoRaPacket() {
  int packetSize = LoRa.parsePacket();
  if (packetSize) {
    Serial.print("Received packet '");
    String LoRaData;
    // read packet
    while (LoRa.available()) {
      LoRaData += LoRa.readString();
    }
    Serial.print(LoRaData); 

    // print RSSI of packet
    Serial.print("' with RSSI ");
    Serial.println(LoRa.packetRssi());

    float temperature, humidity;
    if (parseLoRaData(LoRaData, temperature, humidity)) {
      sendDataToFirebase(temperature, humidity);
    } else {
      Serial.println("Failed to parse LoRa data.");
    }
  }
}

bool parseLoRaData(String data, float &temperature, float &humidity) {
  int tempIndex = data.indexOf("temp:");
  int humIndex = data.indexOf(",hum:");

  if (tempIndex != -1 && humIndex != -1) {
    temperature = data.substring(tempIndex + 5, humIndex).toFloat();
    humidity = data.substring(humIndex + 5).toFloat();
    return true;
  } else {
    return false;
  }
}

void sendDataToFirebase(float temperature, float humidity) {
  FirebaseJson json;
  json.set("temperature", temperature);
  json.set("humidity", humidity);

  if (Firebase.RTDB.setJSON(&fbdo, "lora/sensor_data", &json)) {
    Serial.println("Data sent to Firebase successfully.");
  } else {
    Serial.println("Failed to send data to Firebase.");
    Serial.println(fbdo.errorReason());
  }
}

void loop() {
  receivedLoRaPacket();
}