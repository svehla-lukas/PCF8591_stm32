# I2C_PCF8591 Library

## Overview
This library provides a set of functions to interface with the PCF8591 ADC/DAC over I2C communication using the STM32 HAL library. It allows you to:
- Initialize the PCF8591 module.
- Read raw or converted ADC values from individual or all channels.
- Write values to the DAC output.

The library is designed for easy integration with STM32 projects using STM32CubeIDE and the HAL library.

## Features
1. ADC Initialization:
   - Configure input modes and enable/disable analog output.

2. Reading ADC Values:
   - Read raw ADC values as uint8_t from individual or all channels.
   - Read converted ADC values as float (using a specified reference voltage).

3. DAC Output:
   - Write an 8-bit value to the DAC for analog output.

## Files
- I2C_PCF8591.h: Contains function declarations and macros.
- I2C_PCF8591.c: Implements the library functions.

## API Functions

1. Initialization
HAL_StatusTypeDef I2C_PCF8591_init(uint8_t enableAnalogOutput, uint8_t inputMode, float referenceVoltage);
Description: Initializes the PCF8591 module.
Parameters:
- enableAnalogOutput: Set to 1 to enable DAC output, 0 to disable.
- inputMode: Configure input mode (e.g., single-ended or differential).
- referenceVoltage: The reference voltage for ADC conversions.
Returns: HAL_OK on success, otherwise an error status.

2. Writing to DAC
HAL_StatusTypeDef I2C_PCF8591_write_ain(uint8_t setValue);
Description: Writes an 8-bit value to the DAC output.
Parameters:
- setValue: The value to output via the DAC.
Returns: HAL_OK on success, otherwise an error status.

3. Reading Raw ADC Values
(a) Single Channel
uint8_t* I2C_PCF8591_read_raw_analog_ch(uint8_t channel);
Description: Reads the raw ADC value (8-bit) from a specific channel.
Parameters:
- channel: The ADC channel to read (0-3).
Returns: Pointer to the raw ADC value, or NULL on failure.

(b) All Channels
uint8_t* I2C_PCF8591_read_raw_analogs(void);
Description: Reads raw ADC values from all channels.
Returns: Pointer to an array containing the raw ADC values, or NULL on failure.

4. Reading Converted ADC Values
(a) Single Channel
float* I2C_PCF8591_read_analog_ch(uint8_t channel);
Description: Reads the converted ADC value (as voltage) from a specific channel.
Parameters:
- channel: The ADC channel to read (0-3).
Returns: Pointer to the converted value (in volts), or NULL on failure.

(b) All Channels
float* I2C_PCF8591_read_analogs(void);
Description: Reads converted ADC values (as voltages) from all channels.
Returns: Pointer to an array of converted values, or NULL on failure.

## Usage Example

### Initialization
#include "I2C_PCF8591.h"

HAL_StatusTypeDef result;
result = I2C_PCF8591_init(1, 0, 3.3);
if (result != HAL_OK) {
    // Handle initialization error
}


### Reading ADC Values
uint8_t* rawValue;
float* voltageValue;

// Read raw value from channel 0
rawValue = I2C_PCF8591_read_raw_analog_ch(0);
if (rawValue != NULL) {
    printf("Raw ADC Value (CH0): %d\n", *rawValue);
}

// Read converted value from channel 1
voltageValue = I2C_PCF8591_read_analog_ch(1);
if (voltageValue != NULL) {
    printf("Voltage (CH1): %.2f V\n", *voltageValue);
}

### Writing to DAC
result = I2C_PCF8591_write_ain(128); // Output halfway DAC voltage
if (result != HAL_OK) {
    // Handle DAC error
}

## Configuration

Dependencies
Ensure the following headers are included:
- <stdint.h>: For uint8_t.
- stm32f1xx_hal.h: For STM32 HAL functions and types.

Reference Voltage
The default reference voltage is 3.3V. This can be set during initialization.

Notes
1. ADC Resolution: The ADC has a resolution of 8 bits (0-255).
2. Auto-Increment Mode: This library uses auto-increment mode for multi-channel readings.
3. Error Handling: Always check the return values of functions for error statuses.

License
This library is provided under the MIT License. You are free to use, modify, and distribute it.
