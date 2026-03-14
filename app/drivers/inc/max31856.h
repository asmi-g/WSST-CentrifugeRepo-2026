#ifndef MAX31856_H
#define MAX31856_H

#include "main.h"

typedef struct {
    SPI_HandleTypeDef *hspi;
    GPIO_TypeDef *cs_port;
    uint16_t cs_pin;
} max31856_t;

void max31856_init(max31856_t *dev, SPI_HandleTypeDef *hspi, GPIO_TypeDef *cs_port, uint16_t cs_pin);
float max31856_read_temp(max31856_t *dev);

#endif