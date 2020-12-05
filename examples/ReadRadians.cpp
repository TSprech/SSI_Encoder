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
  Serial.println(Encoder.GetRadians());
  delay(500);  // Optional delay, to make serial monitor more managable
}