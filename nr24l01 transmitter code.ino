#include <SPI.h>
#include <nRF24L01.h>
#include <RF24.h>
#include <DHT.h>

#define DHTPIN 5
#define DHTTYPE DHT11

DHT dht(DHTPIN, DHTTYPE);

RF24 radio(3, 2);

void setup() {
  Serial.begin(9600);
  radio.begin();
  dht.begin();
}

void loop() {
  float temperature = dht.readTemperature();
  float humidity = dht.readHumidity();

  Serial.print("Temperature: ");
  Serial.print(temperature);
  Serial.print(" °C, Humidity: ");
  Serial.print(humidity);
  Serial.println("%");

  // Create a data structure to hold sensor data
  struct SensorData {
    float temperature;
    float humidity;
  };

  // Populate the data structure
  SensorData data;
  data.temperature = temperature;
  data.humidity = humidity;

  // Send the data
  radio.openWritingPipe(0xF0F0F0F0E1LL);
  radio.write(&data, sizeof(data));

  delay(1000); // Wait for 12 seconds before sending the next data
}
