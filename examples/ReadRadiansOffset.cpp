#include <Arduino.h>

#include "AEAT8800.h"

AEAT8800 Encoder(2, 3, 4, 5, 16);

void setup() {
  Serial.begin(9600);
  if (!Encoder.Begin()) {
    Serial.println("Error");
  }
}

void loop() {
  Encoder.Read();
  // Note: Offset is given in raw encoder value, to change this, go to the AEAT8800 library
  //files and uncomment indicated functions
  Serial.println(Encoder.GetDegrees(16634));  // This will offset all readings by π/2
  delay(500);                                 // Optional delay, to make serial monitor more managable
}