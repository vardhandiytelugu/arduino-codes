#include <Wire.h>
#include <LiquidCrystal_I2C.h>
#include <SPI.h>
#include <nRF24L01.h>
#include <RF24.h>

RF24 radio(D3, D4); // Set the CE and CSN pins

LiquidCrystal_I2C lcd(0x27, 16, 2); // I2C address 0x27, 16 column and 2 rows

// Create a data structure to hold sensor data
struct SensorData {
  float temperature;
  float humidity;
};

void setup() {
  Serial.begin(9600);
  lcd.begin(16, 2);
  lcd.init();
  lcd.backlight();
  radio.begin();
  radio.openReadingPipe(1, 0xF0F0F0F0E1LL);
  radio.startListening();
}

void loop() {
  if (radio.available()) {
    SensorData data;
    radio.read(&data, sizeof(data));

    // Display data on the LCD
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("Temp: ");
    lcd.print(data.temperature);
    lcd.print(" C");

    lcd.setCursor(0, 1);
    lcd.print("Humidity: ");
    lcd.print(data.humidity);
    lcd.print(" %");

    Serial.print("Received data - Temperature: ");
    Serial.print(data.temperature);
    Serial.print(" °C, Humidity: ");
    Serial.print(data.humidity);
    Serial.println("%");

    delay(1000); // Wait for 10 seconds before reading the next data
  }
}
