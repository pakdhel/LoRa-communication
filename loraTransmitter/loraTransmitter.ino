#include <LoRa.h>
#include <SPI.h>
#include <Wire.h>
#include <DHT.h>

// pin Lora SPI
#define SCK   5
#define MOSI  27
#define MISO  19
#define SS    18
#define RST   23
#define DIO0  26

// pin dht11
#define DHTPIN 15
#define DHTTYPE DHT11
DHT dht(DHTPIN, DHTTYPE);

// pin relay
#define PINRELAY4 32 // KIPAS
#define PINRELAY3 35 
#define PINRELAY2 34
#define PINRELAY1 33 // LAMPU



void setup() {
  pinMode(PINRELAY1, OUTPUT);
  pinMode(PINRELAY2, OUTPUT);
  pinMode(PINRELAY4, OUTPUT);
  pinMode(PINRELAY4, OUTPUT);

  digitalWrite(PINRELAY1, HIGH);
  digitalWrite(PINRELAY4, HIGH);

  Serial.begin(115200);
  while (!Serial);
  Serial.println("Lora sender example");

  delay(2000);

  SPI.begin(SCK,MISO,MOSI,SS);
  LoRa.setPins(SS,RST,DIO0);
  LoRa.setSpreadingFactor(10);
  dht.begin();

  while (!LoRa.begin(915E6)) {
    Serial.println("Starting LoRa failed!");
    delay(500);
    // while (1);
  }

  LoRa.setSyncWord(0xF3);
  Serial.println("Lora OK");
  delay(1000);
}

void loop() {
  float temperature = dht.readTemperature();
  float humidity = dht.readHumidity();

  // low untuk menutup saklar
  digitalWrite(PINRELAY1, LOW);
  // digitalWrite(PINRELAY4, HIGH);

  if (temperature > 38.0) {
    digitalWrite(PINRELAY4, LOW);
  } 
  
  if (temperature <= 38.0) {
    digitalWrite(PINRELAY4, HIGH);
  }

  String dataToSend = "temp:" + String(temperature, 1) + ",hum:" + String(humidity, 1);
  Serial.print("Sending packet: ");
  Serial.println(dataToSend);
  // Serial.println(counter);

  // send packet
  LoRa.beginPacket();
  // LoRa.print("hello ");
  LoRa.print(dataToSend);
  LoRa.endPacket();


  delay(2000);
}
