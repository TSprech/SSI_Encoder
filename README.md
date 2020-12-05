![Travis CI](https://img.shields.io/travis/com/TSprech/SSI_Encoder?logo=Travis-CI&logoColor=FFFFFF&style=flat&link=https://travis-ci.com/github/TSprech/SSI_Encoder) ![GitHub](https://img.shields.io/github/license/TSprech/SSI_Encoder?color=AE1F34&style=flat&link=https://choosealicense.com/licenses/mit/)
# SSI_Encoder
Encoder library for reading values from the AEAT-8800-Q24 magnetic encoder. This library is capable of reading:
- 16, 14, 12, and 10 bit resolution absolute encoder values
- The magnet high error and magnet low error
- The ready bit
- The parity bit and checking the data against it, if the parities do not match, it returns -1

*Note: This libarary is not intended to program the chip over SPI*

- [SSI_Encoder](#ssi_encoder)
  - [Usage](#usage)
  - [Functions](#functions)
  - [Other information](#other-information)
    - [License](#license)

## Usage
Once an instance of the AEAT8800 class is constructed, the only functions required to begin reading data is `Begin()`, `Read()`, and a function to get the absolute rotation in raw, radians, or degrees, such as `GetRadians()`. [ReadErrorAbsolute.cpp](examples/ReadErrorAbsolute.cpp) shows how to retrieve all values returned from the encoder.

## Functions
| Function                                                                                          | Description                                                                                                  |
| ------------------------------------------------------------------------------------------------- | ------------------------------------------------------------------------------------------------------ |
| `AEAT8800(uint8_t nsl_pin, uint8_t scl_pin, uint8_t do_pin, uint8_t sel_pin, uint8_t bit_count);` | Constructor, pass it the 4 pins and the rotation data resolution (16, 14, 12, 10)                      |
| `bool Begin();`                                                                                   | Declares all pinModes and sets initial pin state                                                       |
| `void Read();`                                                                                    | Read the current value, errors, and parity checks the encoder                                          |
| `int32_t GetRawReading();`                                                                        | Returns the raw value from 0-65536                                                                     |
| `int32_t GetRawReading(int32_t offset);`                                                          | Returns the raw value + offset from 0-65536                                                            |
| `float GetRadians();`                                                                             | Returns radian value, range 0-2π                                                                       |
| `float GetRadians(int32_t offset);`                                                               | Returns radian value with offset added to raw, range 0-2π                                              |
| `float GetRadiansRad(float offset); `                                                             | Returns radian value + offset, range 0-2π                                                              |
| `float GetDegrees();`                                                                             | Returns degree value, range 0-360°                                                                     |
| `float GetDegrees(int32_t offset);`                                                               | Returns degree value with offset added to raw range 0-360°                                             |
| `float GetDegreesDeg(float offset);`                                                              | Returns degree value + offset, range 0-360°                                                            |
| `bool GetMHE();`                                                                                  | Returns true if the magnet strength is too strong, resolve with a weaker magnet or increasing distance |
| `bool GetMLE();`                                                                                  | Returns true if the magnet strength is too weak, resolve with a stronger magnet or decreasing distance |
| `bool GetReady();`                                                                                | Returns true if the chip is ready                                                                      |
| `bool GetParity();`                                                                               | Parity is an even parity                                                                               |

## Other information
AEAT-8800-Q24 [Datasheet](https://docs.broadcom.com/doc/pub-005892)

### License
[MIT](https://choosealicense.com/licenses/mit/)

This software is distributed on an "AS IS" BASIS, WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.