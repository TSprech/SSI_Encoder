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
  // Note: Offset is given in raw encoder value, to use radians/degrees, use either
  // GetRadiansRad(offset) or GetDegreesDeg(offset)
  Serial.println(Encoder.GetRadians(16634));  // This will offset all readings by π/2
  delay(500);                                 // Optional delay, to make serial monitor more managable
}