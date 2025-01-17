/*
 * I2C_PCF8591.c
 *
 *  Created on: Jan 16, 2025
 *      Author: Lukas Svehla
 */

// Includes
#include <stdint.h>
#include <stdbool.h>
#include <string.h>

#include "stm32f1xx_hal.h"

// Definitions
#define TX_INPUT_MODE_MASK 0x70          // Mask for input mode bits
#define TX_CHANNEL_MASK 0x07             // Mask for channel selection bits
#define TX_AUTO_INCREMENT_FLAG 0x04      // Auto-increment flag
#define TX_CHANNEL_NUMBER_MASK 0x30      // Channel number mask
#define ADC_PCF_RESOLUTION 255           // ADC resolution
// Macros
#define ADC_CONVERT(voltage_value, ref_voltage) (((float)(voltage_value) / (float)(ref_voltage)) * (ADC_PCF_RESOLUTION))
#define DAC_CONVERT(adc_value, ref_voltage) (((float)(adc_value) / (float)(ADC_PCF_RESOLUTION)) * (ref_voltage))

// External I2C handler
extern I2C_HandleTypeDef hi2c1;

// Static variables
static uint8_t raw_data[4];
static float converted_data[4];
static const uint8_t PCF8591_address = 0x48; // I2C address for PCF8591
static uint8_t tx_data[2] = { 0x00, 0x00 };  // Transmit buffer
static const size_t size_1 = 1;             // Size for single-byte transmission
static float ref_voltage = 3.3;              // Reference voltage for ADC
static HAL_StatusTypeDef result = HAL_ERROR; // Status variable

// Initialize PCF8591 ADC/DAC
HAL_StatusTypeDef I2C_PCF8591_init(uint8_t enableAnalogOutput,
		uint8_t inputMode, float reference_voltage) {
	result = HAL_ERROR;
	ref_voltage = reference_voltage;

	// Check if the device is ready
	result = HAL_I2C_IsDeviceReady(&hi2c1, PCF8591_address << 1, 1,
	HAL_MAX_DELAY);
	if (result != HAL_OK) {
		return result;
	}

	// Configure input mode and analog output
	inputMode = (inputMode & 0x03) << 4;
	enableAnalogOutput = (enableAnalogOutput & 0x01) << 6;
	tx_data[0] = (tx_data[0] & ~TX_INPUT_MODE_MASK) | inputMode
			| enableAnalogOutput;

	// Transmit configuration
	result = HAL_I2C_Master_Transmit(&hi2c1, PCF8591_address << 1, tx_data,
			size_1, HAL_MAX_DELAY);
	return result;
}

// Write to the DAC raw Value
HAL_StatusTypeDef I2C_PCF8591_write_raw(uint8_t setValue) {
	tx_data[0] |= 0x40; // Enable analog output
	tx_data[1] = setValue;

	result = HAL_I2C_Master_Transmit(&hi2c1, PCF8591_address << 1, tx_data,
			sizeof(tx_data), HAL_MAX_DELAY);
	return result;
}

// Write to the DAC voltage
HAL_StatusTypeDef I2C_PCF8591_write(float setValue) {
	tx_data[0] |= 0x40; // Enable analog output
	setValue = (setValue > ref_voltage) ? ref_voltage : setValue;
	tx_data[1] = (uint8_t) ADC_CONVERT(setValue, ref_voltage);

	result = HAL_I2C_Master_Transmit(&hi2c1, PCF8591_address << 1, tx_data,
			sizeof(tx_data), HAL_MAX_DELAY);
	return result;
}

// Read raw ADC value for a specific channel
uint8_t* I2C_PCF8591_read_ch_raw(uint8_t channel) {
	uint8_t rx_data[] = { 0x00, 0x00 };

	if (channel > 3) {
		return NULL; // Invalid channel
	}

	tx_data[0] = (tx_data[0] & ~TX_CHANNEL_MASK) | channel;

	result = HAL_I2C_Master_Transmit(&hi2c1, PCF8591_address << 1, tx_data,
			size_1, HAL_MAX_DELAY);
	if (result != HAL_OK) {
		return NULL;
	}

	result = HAL_I2C_Master_Receive(&hi2c1, PCF8591_address << 1, rx_data,
			sizeof(rx_data), HAL_MAX_DELAY);
	if (result != HAL_OK) {
		return NULL;
	}

	raw_data[channel] = rx_data[1];
	converted_data[channel] = (float) DAC_CONVERT(raw_data[channel],
			ref_voltage);

	return &raw_data[channel];
}

// Read raw ADC values for all channels
uint8_t* I2C_PCF8591_read_raw(void) {
	uint8_t rx_data[] = { 0x00, 0x00, 0x00, 0x00, 0x00 };
	tx_data[0] &= ~TX_AUTO_INCREMENT_FLAG;

	result = HAL_I2C_Master_Transmit(&hi2c1, PCF8591_address << 1, tx_data,
			size_1, HAL_MAX_DELAY);
	if (result != HAL_OK) {
		return NULL;
	}

	result = HAL_I2C_Master_Receive(&hi2c1, PCF8591_address << 1, rx_data,
			sizeof(rx_data), HAL_MAX_DELAY);
	if (result != HAL_OK) {
		return NULL;
	}

	for (int i = 0; i < 4; i++) {
		raw_data[i] = rx_data[i + 1];
		converted_data[i] = (uint8_t) DAC_CONVERT(raw_data[i], ref_voltage);
	}

	return raw_data;
}

// Read converted ADC value for a specific channel
float* I2C_PCF8591_read_ch(uint8_t channel) {
	if (channel > 3) {
		return NULL; // Invalid channel
	}

	tx_data[0] = (tx_data[0] & ~TX_CHANNEL_MASK) | channel;

	result = HAL_I2C_Master_Transmit(&hi2c1, PCF8591_address << 1, tx_data,
			size_1, HAL_MAX_DELAY);
	if (result != HAL_OK) {
		return NULL;
	}

	uint8_t rx_data[] = { 0x00, 0x00 };
	result = HAL_I2C_Master_Receive(&hi2c1, PCF8591_address << 1, rx_data,
			sizeof(rx_data), HAL_MAX_DELAY);
	if (result != HAL_OK) {
		return NULL;
	}

	raw_data[channel] = rx_data[1];
	converted_data[channel] = (float) DAC_CONVERT(raw_data[channel],
			ref_voltage);
	return &converted_data[channel];
}

// Read converted ADC values for all channels
float* I2C_PCF8591_read(void) {
	uint8_t rx_data[] = { 0x00, 0x00, 0x00, 0x00, 0x00 };
	tx_data[0] &= ~TX_AUTO_INCREMENT_FLAG;

	result = HAL_I2C_Master_Transmit(&hi2c1, PCF8591_address << 1, tx_data,
			size_1, HAL_MAX_DELAY);
	if (result != HAL_OK) {
		return NULL;
	}

	result = HAL_I2C_Master_Receive(&hi2c1, PCF8591_address << 1, rx_data,
			sizeof(rx_data), HAL_MAX_DELAY);
	if (result != HAL_OK) {
		return NULL;
	}

	for (int i = 0; i < 4; i++) {
		raw_data[i] = rx_data[i + 1];
		converted_data[i] = (float) DAC_CONVERT(raw_data[i], ref_voltage);
	}

	return converted_data;
}
