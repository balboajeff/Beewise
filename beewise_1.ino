// This example shows how to connect to Cayenne using an ESP8266 and send/receive sample data.
// Make sure you install the ESP8266 Board Package via the Arduino IDE Board Manager and select the correct ESP8266 board before compiling. 

//#define CAYENNE_DEBUG
#define CAYENNE_PRINT Serial
#include <CayenneMQTTESP8266.h>
#include <Wire.h>
#include "MPL3115A2.h"

//Create an instance of the object
MPL3115A2 myPressure;
// WiFi network info.
char ssid[] = "Yoyodyne";
char wifiPassword[] = "12monkeys";

// Cayenne authentication info. This should be obtained from the Cayenne Dashboard.
char username[] = "eae4d0e0-25fc-11e7-b2af-55ba5adcb0d5";
char password[] = "d18727aafc2c3afb77eef8f47a00e1aec259166c";
char clientID[] = "08c73290-c992-11e7-ba6c-75d14f3b7ebe";

unsigned long lastMillis = 0;

void setup() {
 Serial.begin(9600);
 
  Cayenne.begin(username, password, clientID, ssid, wifiPassword);
 //MPL3115A2 definitions
 Wire.begin();
myPressure.begin(); // Get sensor online

  //Configure the sensor
  myPressure.setModeAltimeter(); // Measure altitude above sea level in meters
  //myPressure.setModeBarometer(); // Measure pressure in Pascals from 20 to 110 kPa

  myPressure.setOversampleRate(7); // Set Oversample to the recommended 128
  myPressure.enableEventFlags(); // Enable all three pressure and temp event flags 

}

void loop() {
  Cayenne.loop();
float altitude = myPressure.readAltitudeFt();
  Serial.print(" Altitude(ft):");
  Serial.print(altitude, 2);
    float temperature = myPressure.readTempF();
  Serial.print(" Temp(f):");
  Serial.print(temperature, 2);
  //Publish data every 10 seconds (10000 milliseconds). Change this value to publish at a different interval.
  if (millis() - lastMillis > 10000) {
    lastMillis = millis();
    //Write data to Cayenne here. This example just sends the current uptime in milliseconds.
    Cayenne.virtualWrite(0, temperature);
    Cayenne.virtualWrite(6, altitude);
    //Some examples of other functions you can use to send data.
    //Cayenne.celsiusWrite(1, 22.0);
    //Cayenne.luxWrite(2, 700);
    //Cayenne.virtualWrite(3, 50, TYPE_PROXIMITY, UNIT_CENTIMETER);
  }
}

//Default function for processing actuator commands from the Cayenne Dashboard.
//You can also use functions for specific channels, e.g CAYENNE_IN(1) for channel 1 commands.
CAYENNE_IN_DEFAULT()
{
  CAYENNE_LOG("CAYENNE_IN_DEFAULT(%u) - %s, %s", request.channel, getValue.getId(), getValue.asString());
  //Process message here. If there is an error set an error message using getValue.setError(), e.g getValue.setError("Error message");
}
