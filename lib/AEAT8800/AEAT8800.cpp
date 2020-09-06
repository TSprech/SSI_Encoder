#include "AEAT8800.h"

#include <Arduino.h>

//* Data Format
// First data section size depends on settings set by SPI
// [16, 14, 12, or 10 bit absolute rotation] [1 bit mag high err] [1 bit mag low err] [1 bit ready] [1 bit parity]

AEAT8800::AEAT8800(uint8_t nsl_pin, uint8_t scl_pin, uint8_t do_pin, uint8_t sel_pin, uint8_t bit_count) {
  this->nsl_pin = nsl_pin;
  this->sel_pin = sel_pin;
  this->do_pin = do_pin;
  this->scl_pin = scl_pin;
  this->bit_count = bit_count;
}

bool AEAT8800::Begin() {
  pinMode(do_pin, INPUT);
  pinMode(scl_pin, OUTPUT);
  pinMode(nsl_pin, OUTPUT);
  pinMode(sel_pin, OUTPUT);

  // Initial pin state settings can be found in the datasheet
  digitalWrite(scl_pin, HIGH);
  digitalWrite(sel_pin, LOW);
  digitalWrite(nsl_pin, HIGH);
  delay(3);  // Holds NSL high for > 3 ms upon power up
  return true;
}

void AEAT8800::Read() {
  // Select must be held high for 1 μs to switch between SPI and SSI mode
  digitalWrite(sel_pin, HIGH);
  delayMicroseconds(1);
  digitalWrite(nsl_pin, LOW);

  raw_input = 0;
  parity_check = false;
  valid_data = false;

  for (uint16_t lcv = 0; lcv < bit_count + 4; lcv++) {
    raw_input <<= 1;  // Shift to prepare for the next incoming bit
    //* If high clock speed mcu, insert delays or NOPs in between the digital writes and reads to slow the clock rate
    digitalWrite(scl_pin, LOW);

    //! The data sheet recommends reading from the SSI falling edge
    input_bit = digitalRead(do_pin);
    
    // Skip on last iteration to avoid inverting on the parity bit
    // Everytime a 1 is read, parity_check inverts to indicate an odd or even parity
    if (input_bit && (lcv < bit_count + 3)) {
      parity_check = !parity_check;
    }

    digitalWrite(scl_pin, HIGH);

    raw_input |= input_bit;  // Bitwise OR the new bit to add it to the end of the absolute reading
  }

  digitalWrite(nsl_pin, HIGH);  // Return to a high state to prepare for next reading

  if (parity_check == (raw_input & 1)) {
    absolute_rotation = raw_input >> 4;
    magnet_high_error = raw_input & 1000;
    magnet_low_error = raw_input & 100;
    chip_ready = raw_input & 10;
    parity_check = raw_input & 1;

    valid_data = true;
  } else {
    // The encoder cannot return negative values so it is used as our error value
    raw_input = -1;
  }
}

int32_t AEAT8800::GetRawReading() {
  if (valid_data) {
    return (absolute_rotation);
  } else {
    return -1;
  }
}

int32_t AEAT8800::GetRawReading(int32_t offset) {
  if (valid_data) {
    return (absolute_rotation + offset);
  } else {
    return -1;
  }
}

float AEAT8800::GetRadians() {
  if (valid_data) {
    return (absolute_rotation * ((2 * PI) / 65536));
  } else {
    return -1;
  }
}

float AEAT8800::GetRadians(int32_t offset) {
  if (valid_data) {
    // The first part recasts the sum so that any value over 65536 overflows so we retain the 0-2π range
    // The second half finds the radian value, the range can be adjusted by changing 2*PI to the new max value
    return (((uint16_t)(absolute_rotation + offset)) * ((2 * PI) / 65536));
  } else {
    return -1;
  }
}

float AEAT8800::GetDegrees() {
  if (valid_data) {
    return (absolute_rotation * (360 / 65536));
  } else {
    return -1;
  }
}

float AEAT8800::GetDegrees(int32_t offset) {
  if (valid_data) {
    // The first part recasts the sum so that any value over 65536 overflows so we retain the 0-360° range
    // The second half finds the degree value, the range can be adjusted by changing 360 to the new max value
    return (((uint16_t)(absolute_rotation + offset)) * (360 / 65536));
  } else {
    return -1;
  }
}

//! Uncomment the code below if the offset being passed is given in radian instead of absolute encoder ticks
// float AEAT8800::GetRadians(float offset) {
//   if (valid_data) {
//     return ((absolute_rotation * ((2 * PI) / 65536)) + offset);
//   } else {
//     return -1;
//   }
// }

//! Uncomment the code below if the offset being passed is given in degrees instead of absolute encoder ticks
// float AEAT8800::GetDegrees(float offset) {
//   if (valid_data) {
//     return ((absolute_rotation * (360 / 65536)) + offset);
//   } else {
//     return -1;
//   }
// }

bool AEAT8800::GetMHE() {
  return magnet_high_error;
}

bool AEAT8800::GetMLE() {
  return magnet_low_error;
}

bool AEAT8800::GetReady() {
  return chip_ready;
}

bool AEAT8800::GetParity() {
  return parity_check;
}