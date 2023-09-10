#include <SoftwareSerial.h>


SoftwareSerial sim800lSerial(7, 8); // RX, TX


#define led1 11  //arduino pin nuber//
#define led2 12  //arduino pin nuber//

void setup() {
  Serial.begin(9600);      // Serial monitor for debugging
  sim800lSerial.begin(9600); // SIM800L module communication
  delay(1000);
  Serial.println("Initializing...");
  delay(1000);
  
  // Configure the SIM800L module
  sim800lSerial.println("AT"); // Check if the module is responding
  sim800lSerial.println("AT+CMGF=1"); // Set SMS text mode
  sim800lSerial.println("AT+CNMI=2,2,0,0,0"); // New SMS notification
  delay(1000);
  while (!sim800lSerial.available());
  Serial.println(sim800lSerial.readString());

  pinMode(led1,OUTPUT);
    pinMode(led2,OUTPUT);


  Serial.println("Ready to automatically answer calls!");
      sendSMS("Ready to automatically answer calls!");

}

void loop() {
  if (sim800lSerial.available()) {
    String response = sim800lSerial.readString();
    Serial.print("Received: ");
    Serial.println(response);
    
    // Check for incoming call
    if (response.indexOf("RING") != -1) {
      Serial.println("Incoming call!");
      delay(1000); // Wait for 1 second before answering
      sim800lSerial.println("ATA"); // Answer the call
      Serial.println("Call answered!");
      // You can add additional actions here, like sending an SMS or performing some other task.
    }
     if (response.indexOf("DTMF: 1") != -1) {
      Serial.println("1!");
     digitalWrite(11,HIGH);
         sendSMS("LED1");

  }
  else if (response.indexOf("DTMF: 2") != -1){
  Serial.println("2!");
     digitalWrite(12,HIGH);
         sendSMS("LED2");

}
if (response.indexOf("DTMF: 0") != -1){
     digitalWrite(11,LOW);
      digitalWrite(12,LOW);
            delay(1000);
  Serial.println("OFF");
    sendSMS("OFF");


}
  }
}
void sendSMS(String message) {
  sim800lSerial.println("AT+CMGS=\"+91xxxxxxxxxx\""); // Replace with the recipient's phone number
  delay(1000);
  sim800lSerial.print(message);
  delay(100);
  sim800lSerial.write(0x1A); // End the SMS with Ctrl+Z
  delay(1000);
}
