#include "max31856.h"

// Config assumptions:
// start running continuously
// use the 50 Hz noise filter
// sensor connected is a K-type thermocouple
void max31856_init(max31856_t *dev, SPI_HandleTypeDef *hspi, GPIO_TypeDef *cs_port, uint16_t cs_pin)
{
    uint8_t data[2];

    // SPI + CS pin belonging to each thermocouple board
    dev->hspi = hspi;
    dev->cs_port = cs_port;
    dev->cs_pin = cs_pin;

    HAL_GPIO_WritePin(dev->cs_port, dev->cs_pin, GPIO_PIN_SET);

    // MAX31856 has two control / config registers:

    // CR0: auto conversion (continuous measurements) + 50Hz filter (noise rejection) (either 50 or 60Hz)
    data[0] = 0x80;
    data[1] = 0x81;
    HAL_GPIO_WritePin(dev->cs_port, dev->cs_pin, GPIO_PIN_RESET);
    HAL_SPI_Transmit(dev->hspi, data, 2, HAL_MAX_DELAY);
    HAL_GPIO_WritePin(dev->cs_port, dev->cs_pin, GPIO_PIN_SET);

    // CR1: K-type thermocouple
    data[0] = 0x81;
    data[1] = 0x03; // set to K type
    HAL_GPIO_WritePin(dev->cs_port, dev->cs_pin, GPIO_PIN_RESET);
    HAL_SPI_Transmit(dev->hspi, data, 2, HAL_MAX_DELAY);
    HAL_GPIO_WritePin(dev->cs_port, dev->cs_pin, GPIO_PIN_SET);
}

float max31856_read_temp(max31856_t *dev)
{
    uint8_t reg = 0x0C;
    uint8_t buf[3];
    int32_t value;

    HAL_GPIO_WritePin(dev->cs_port, dev->cs_pin, GPIO_PIN_RESET);
    HAL_SPI_Transmit(dev->hspi, &reg, 1, HAL_MAX_DELAY);
    HAL_SPI_Receive(dev->hspi, buf, 3, HAL_MAX_DELAY);
    HAL_GPIO_WritePin(dev->cs_port, dev->cs_pin, GPIO_PIN_SET);

    // reads 3 bytes of temp data, combines into signed value, convert to celsius
    value = ((int32_t)buf[0] << 16) | ((int32_t)buf[1] << 8) | buf[2];
    value = value >> 5;

    if (value & 0x40000) {
        value |= 0xFFF80000;
    }

    return value / 128.0f;
}