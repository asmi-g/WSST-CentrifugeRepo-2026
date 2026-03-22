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
    uint8_t reg;
    uint8_t tc_buf[3];
    uint8_t cj_buf[2];
    int32_t tc_value;
    int16_t cj_value;
    float tc_temp;
    float cj_temp;

    // Read thermocouple temperature registers (0x0C, 0x0D, 0x0E)
    reg = 0x0C;
    HAL_GPIO_WritePin(dev->cs_port, dev->cs_pin, GPIO_PIN_RESET);
    HAL_SPI_Transmit(dev->hspi, &reg, 1, HAL_MAX_DELAY);
    HAL_SPI_Receive(dev->hspi, tc_buf, 3, HAL_MAX_DELAY);
    HAL_GPIO_WritePin(dev->cs_port, dev->cs_pin, GPIO_PIN_SET);

    // Combine thermocouple temp
    tc_value = ((int32_t)tc_buf[0] << 16) | ((int32_t)tc_buf[1] << 8) | tc_buf[2];
    tc_value >>= 5;

    if (tc_value & 0x40000) {
        tc_value |= 0xFFF80000;
    }

    tc_temp = tc_value / 128.0f;

    // Read cold-junction temperature registers (0x0A, 0x0B)
    reg = 0x0A;
    HAL_GPIO_WritePin(dev->cs_port, dev->cs_pin, GPIO_PIN_RESET);
    HAL_SPI_Transmit(dev->hspi, &reg, 1, HAL_MAX_DELAY);
    HAL_SPI_Receive(dev->hspi, cj_buf, 2, HAL_MAX_DELAY);
    HAL_GPIO_WritePin(dev->cs_port, dev->cs_pin, GPIO_PIN_SET);

    // Combine cold-junction temp
    cj_value = ((int16_t)cj_buf[0] << 8) | cj_buf[1];
    cj_value >>= 2;

    if (cj_value & 0x2000) {
        cj_value |= 0xC000;
    }

    cj_temp = cj_value / 64.0f;

    return tc_temp + cj_temp;
}