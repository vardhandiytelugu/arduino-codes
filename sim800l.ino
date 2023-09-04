#include <SoftwareSerial.h>

SoftwareSerial sim800lSerial(7, 8); // RX, TX pins
const int ledPin = 12;

void setup() {
  Serial.begin(9600);
  sim800lSerial.begin(9600);
  pinMode(ledPin, OUTPUT);
  delay(2000);

  // Initialize SIM800L
  if (!sim800lInit()) {
    Serial.println("SIM800L initialization failed. Check wiring and module.");
    while (1);
  }

  Serial.println("SIM800L initialization successful.");
  // Send welcome message
  sendSMS("Welcome to LED Control! Send 'ON' or 'OFF' to control the LED.");
}

void loop() {
  if (sim800lSerial.available()) {
    String smsContent = sim800lSerial.readStringUntil('\n');
    smsContent.trim();
    Serial.println("Received SMS: " + smsContent);

    if (smsContent == "ON") {
      digitalWrite(ledPin, HIGH);
      sendSMS("LED is ON.");
    } else if (smsContent == "OFF") {
      digitalWrite(ledPin, LOW);
      sendSMS("LED is OFF.");
    }
  }
}

bool sim800lInit() {
  sim800lSerial.println("AT");
  delay(1000); // Wait for SIM800L to respond
  if (sim800lSerial.find("OK")) {
    return true;
  } else {
    return false;
  }
}

void sendSMS(const char *message) {
  sim800lSerial.println("AT+CMGF=1"); // Set SMS mode to text
  delay(100);
  sim800lSerial.print("AT+CMGS=\"+919701274708\""); // Replace with your destination phone number
  delay(100);
  sim800lSerial.write(0x0D);
  delay(100);
  sim800lSerial.println(message);
  delay(100);
  sim800lSerial.write(0x1A); // End of message character
  delay(1000); // Wait for the module to send the message
  Serial.println("Feedback SMS sent: " + String(message));
}
