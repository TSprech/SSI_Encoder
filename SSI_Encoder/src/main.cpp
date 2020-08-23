#include <Arduino.h>
//! This code is intended for the AEAT-8800-Q24 magnetic encoder IC from Broadcom
//* This code is based on the SSI protocol defined in the timing diagram on page 15 of the datasheet, pub-005892
//* This chip supports up to 10 Mhz SSI communication
#define NSL_PIN 2     // SSI data input
#define SEL_PIN 3     // SSI select pin
#define DO_PIN 4      // SSI data output
#define SCL_PIN 5     // SSI clock input
#define BIT_COUNT 16  // Encoder packet size / resolution

void setup() {
  Serial.begin(9600);

  pinMode(DO_PIN, INPUT);
  pinMode(SCL_PIN, OUTPUT);
  pinMode(NSL_PIN, OUTPUT);
  pinMode(SEL_PIN, OUTPUT);

  // Set all pins in the state required before reading any data
  digitalWrite(SCL_PIN, HIGH);
  digitalWrite(SEL_PIN, LOW);
  digitalWrite(NSL_PIN, HIGH);
  delay(3);  // Holds NSL high for > 3 ms upon power up
}

void loop() {
  // Select must be held high for 1 μs to switch between SPI and SSI mode
  digitalWrite(SEL_PIN, HIGH);
  delayMicroseconds(1);
  digitalWrite(NSL_PIN, LOW);

  unsigned long data = 0;  //? Could be switched to an unsigned 16 bit integer

  //todo: switch from a delay model to using some sort of timer or single clock cycle delay based on F_CPU
  for (int i = 0; i < BIT_COUNT; i++) {
    data <<= 1;  // Bitshift the data right to prepare to read the next bit
    // Modulate the clock pin once to prepare to read in a new bit
    digitalWrite(SCL_PIN, LOW);
    delayMicroseconds(1);

    //! The data sheet recommends reading from the SSI falling edge
    data |= digitalRead(DO_PIN);  // Bitwise OR the new bit to add it to the end of the absolute reading
    
    digitalWrite(SCL_PIN, HIGH);
    delayMicroseconds(1);
  }
  Serial.println(data);

  delayMicroseconds(1);
  digitalWrite(NSL_PIN, HIGH);  // Return to a high state to prepare for next reading
  delay(5);                     // Optional delay, to make serial monitor more managable
}