#include <WiFiNINA.h>
#include "secrets.h"
#include "ThingSpeak.h" 

char ssid[] = SECRET_SSID;   
char pass[] = SECRET_PASS;   
WiFiClient client;

unsigned long myChannelNumber = SECRET_CH_ID;
const char * myWriteAPIKey = SECRET_WRITE_APIKEY;

void setup() {
  Serial.begin(9600);
  while (!Serial);

  ThingSpeak.begin(client);  
}

void loop() {
  // Connect to WiFi if not connected
  if (WiFi.status() != WL_CONNECTED) {
    Serial.print("Connecting to WiFi: ");
    Serial.println(SECRET_SSID);
    while (WiFi.status() != WL_CONNECTED) {
      WiFi.begin(ssid, pass);
      Serial.print(".");
      delay(5000);
    }
    Serial.println("\nConnected.");
  }

  // Read light sensor value
  int lightValue = analogRead(A0);
  Serial.print("Analog reading: ");
  Serial.println(lightValue);

  // Set ThingSpeak field with the light sensor value
  ThingSpeak.setField(1, lightValue);

  // Send data to ThingSpeak
  int status = ThingSpeak.writeFields(myChannelNumber, myWriteAPIKey);
  if (status == 200) {
    Serial.println("Channel update successful.");
  } else {
    Serial.println("Error updating channel. HTTP code: " + String(status));
  }

  // Wait 30 seconds before next update
  delay(30000);
}
