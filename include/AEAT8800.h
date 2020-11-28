/* Copyright 2020 Trey Sprecher. All Rights Reserved.

Licensed under the MIT License, (the "License");
you may not use this file except in compliance with the License.

Software
distributed under the License is distributed on an "AS IS" BASIS,
WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied. */

#include <Arduino.h>
#ifndef AEAT8800_H
#define AEAT8800_H

//! This code is intended for the AEAT-8800-Q24 magnetic encoder IC from Broadcom
//* This code is based on the SSI protocol defined in the timing diagram on page 15 of the datasheet, pub-005892
//* This chip supports up to 10 Mhz SSI communication, if using a high clock speed mcu, this class would need to be edited

class AEAT8800 {
 private:
  uint8_t nsl_pin;     // SSI data input, yellow
  uint8_t sel_pin;     // SSI select pin, green
  uint8_t do_pin;      // SSI data output, white
  uint8_t scl_pin;     // SSI clock input, blue
  uint8_t bit_count;   // Encoder packet size / resolution
  bool input_bit = 0;  // Used to calculate parity

  int32_t raw_input = 0;  // Holds the raw input including errors and parity
  uint16_t absolute_rotation = 0;
  bool magnet_high_error = false;
  bool magnet_low_error = false;
  bool chip_ready = false;
  bool parity_check = false;

  bool valid_data = false;  // Set to true if the parity matches

 public:
  // Sets pin variables, still required to call Begin() in the setup loop
  AEAT8800(uint8_t nsl_pin, uint8_t scl_pin, uint8_t do_pin, uint8_t sel_pin, uint8_t bit_count);

  bool Begin();  // Declares all pinModes and sets initial pin state
  void Read();   // Read the current value, errors, and parity checks the encoder

  //! The get functions below will return -1 if there is an error
  int32_t GetRawReading();                // Returns the raw value from 0-65536
  int32_t GetRawReading(int32_t offset);  // Returns the raw value + offset from 0-65536
  float GetRadians();                     // Returns radian value, range 0-2π
  float GetRadians(int32_t offset);       // Returns radian value with offset added to raw, range 0-2π
  float GetRadiansRad(float offset);      // Returns radian value + offset, range 0-2π
  float GetDegrees();                     // Returns degree value, range 0-360°
  float GetDegrees(int32_t offset);       // Returns degree value with offset added to raw, range 0-360°
  float GetDegreesDeg(float offset);      // Returns degree value + offset, range 0-360°

  bool GetMHE();     // Returns true if the magnet strength is too strong, resolve with a weaker magnet or increasing distance
  bool GetMLE();     // Returns true if the magnet strength is too weak, resolve with a stronger magnet or decreasing distance
  bool GetReady();   // Returns true if the chip is ready
  bool GetParity();  // Parity is an even parity
};

#endif  //AEAT8800_H_