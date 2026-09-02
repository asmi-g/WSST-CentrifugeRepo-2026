#ifndef MBTWI_H
#define MBTWI_H

#include "stm32f4xx_hal.h"
#include "stm32f4xx_hal_i2c.h"

extern I2C_HandleTypeDef hi2c3;   // defined in main.c

#ifndef _BV
#define _BV(bit) (1 << (bit))
#endif


// MCP9808 uses 7-bit address, shift for HAL's 8-bit convention
static inline void send_twi_byte(uint8_t address, uint8_t reg, uint8_t value) {
    (void)value; // unused: this call is only ever used to point at a register before a read
    HAL_I2C_Master_Transmit(&hi2c3, (uint16_t)(address << 1), &reg, 1, HAL_MAX_DELAY);
}

static inline int16_t read_twi_int16(uint8_t address) {
    uint8_t buf[2] = {0};
    HAL_I2C_Master_Receive(&hi2c3, (uint16_t)((address << 1) | 1), buf, 2, HAL_MAX_DELAY);
    return (int16_t)((buf[0] << 8) | buf[1]);
}

static inline int8_t read_twi_int8(uint8_t address) {
    uint8_t buf = 0;
    HAL_I2C_Master_Receive(&hi2c3, (uint16_t)((address << 1) | 1), &buf, 1, HAL_MAX_DELAY);
    return (int8_t)buf;
}

static inline void send_twi_bin_data(uint8_t address, uint8_t len, uint8_t *data) {
    HAL_I2C_Master_Transmit(&hi2c3, (uint16_t)(address << 1), data, len, HAL_MAX_DELAY);
}

#endif