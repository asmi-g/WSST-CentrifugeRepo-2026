# MAX31856 Driver
This driver reads temperature from a MAX31856 thermocouple interface using SPI.

## Files
- `/app/drivers/max31856.h`
- `/app/drivers/max31856.c`

## Driver setup

Two MAX31856 thermocouple boards are used (one per thermocouple).

Each thermocouple is represented by a device struct defined in the driver:
```c
typedef struct {
    SPI_HandleTypeDef *hspi;
    GPIO_TypeDef *cs_port;
    uint16_t cs_pin;
} max31856_t;
```
So each thermocouple uses:
- one SPI handle (Each board is connected to a separate SPI bus.)
- one chip select pin
Allows multiple thermocouples to use this same driver.

## Usage

### Initialization
Initialize each thermocouple once at startup:
'''c
max31856_t tc1;
max31856_t tc2;

max31856_init(&tc1, &hspi1, TC1_CS_GPIO_Port, TC1_CS_Pin);
max31856_init(&tc2, &hspi2, TC2_CS_GPIO_Port, TC2_CS_Pin);
'''

The initialization function in driver:
- stores the SPI and CS configuration
- configures the MAX31856 registers
- enables automatic / continuous temperature conversion
- sets thermocouple type to K-type

### Reading Temperature
Temperature can be read with:
```c
float temp = max31856_read_temp(&tc1);
```
Returned value is temperature in Celsius.

## Example FreeRTOS task:
```c
void StartThermoTask(void *argument)
{
    max31856_t tc1;
    max31856_t tc2;

    max31856_init(&tc1, &hspi1, TC1_CS_GPIO_Port, TC1_CS_Pin);
    max31856_init(&tc2, &hspi2, TC2_CS_GPIO_Port, TC2_CS_Pin);

    for (;;)
    {
        float temp1 = max31856_read_temp(&tc1);
        float temp2 = max31856_read_temp(&tc2);

        osDelay(500);
    }
}
```

## Important Notes / TODOs:
- SPI configurations have not been added to the .ioc files / CubeMX settings
- None of this has been tested yet so might not work
