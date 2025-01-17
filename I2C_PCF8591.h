#ifndef INC_I2C_PCF8591_H_
#define INC_I2C_PCF8591_H_

#include <stdint.h>          // For uint8_t
#include "stm32f1xx_hal.h"   // For HAL_StatusTypeDef

// External function declarations

/**
 * @brief Initialize the PCF8591 ADC/DAC.
 *
 * @param enableAnalogOutput Set to 1 to enable analog output (DAC mode), 0 to disable.
 * @param inputMode Input mode selection (e.g., single-ended or differential inputs).
 * @param referenceVoltage The reference voltage used for conversions.
 * @return HAL_StatusTypeDef Returns HAL_OK on success, or an error status on failure.
 */
HAL_StatusTypeDef I2C_PCF8591_init(uint8_t enableAnalogOutput, uint8_t inputMode, float referenceVoltage);

/**
 * @brief Write a value to the PCF8591 DAC.
 *
 * @param setValue The 8-bit value to be set on the DAC output.
 * @return HAL_StatusTypeDef Returns HAL_OK on success, or an error status on failure.
 */
HAL_StatusTypeDef I2C_PCF8591_write_raw(uint8_t setValue);
/**
 * @brief Write a value to the PCF8591 DAC.
 *
 * @param setValue The voltage value to be set on the DAC output.
 * maximum is reference voltage
 * @return HAL_StatusTypeDef Returns HAL_OK on success, or an error status on failure.
 */
HAL_StatusTypeDef I2C_PCF8591_write(float setValue);

/**
 * @brief Read raw ADC value from a specific channel of the PCF8591.
 *
 * @param channel The ADC channel to read (0-3).
 * @return uint8_t* Pointer to the raw ADC value (8-bit), or NULL if an error occurs.
 */
uint8_t* I2C_PCF8591_read_ch_raw(uint8_t channel);

/**
 * @brief Read raw ADC values from all channels of the PCF8591.
 *
 * @return uint8_t* Pointer to an array containing raw ADC values, or NULL if an error occurs.
 */
uint8_t* I2C_PCF8591_read_raw(void);

/**
 * @brief Read a converted voltage value from a specific ADC channel.
 *
 * @param channel The ADC channel to read (0-3).
 * @return float* Pointer to the converted voltage value, or NULL if an error occurs.
 */
float* I2C_PCF8591_read_ch(uint8_t channel);

/**
 * @brief Read converted voltage values from all ADC channels.
 *
 * @return float* Pointer to an array containing converted voltage values, or NULL if an error occurs.
 */
float* I2C_PCF8591_read(void);

#endif /* INC_I2C_PCF8591_H_ */
