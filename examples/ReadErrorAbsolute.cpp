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
  Serial.print("Encoder Raw Value: ");
  Serial.print(Encoder.GetRawReading());
  Serial.print("\tMagnet High Error: ");
  Serial.print(Encoder.GetMHE());
  Serial.print("\tMagnet Low Error: ");
  Serial.print(Encoder.GetMLE());
  Serial.print("\tReady: ");
  Serial.print(Encoder.GetReady());
  Serial.print("\tParity: ");
  Serial.println(Encoder.GetParity());
  delay(500);  // Optional delay, to make serial monitor more managable
}