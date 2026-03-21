# MAX31856 Driver
This driver reads temperature from a MAX31856 thermocouple interface using SPI.

## Pin Configurations

Current SPI pin configurations for the F401RE and a `StartTestThermoTask` FreeRTOS task are defined in `F401RE.ioc`.

For each thermocouple, these pins on the amplifier boards are used: Vin, Gnd, SCK, SDO, SDI, and CS

So each thermocouple uses:
- one SPI handle (Each board is connected to a separate SPI bus, i.e. SCK, SDO, SDI pins)
- one chip select pin (i.e. CS pin)
Allows multiple thermocouples to use this same driver.

Current STM32F401RE to amplifier board pin mappings:
```
Thermocouple #1:
STM32 PB3 -> MAX31856 SCK
STM32 PB4 -> MAX31856 SDO
STM32 PB5 -> MAX31856 SDI
STM32 PB0 -> MAX31856 CS
STM32 3.3V -> MAX31856 Vin
STM32 GND -> MAX31856 GND

Thermocouple #1:
STM32 PB13 -> MAX31856 SCK
STM32 PB14 -> MAX31856 SDO
STM32 PB15 -> MAX31856 SDI
STM32 PB1 -> MAX31856 CS
STM32 3.3V -> MAX31856 Vin
STM32 GND -> MAX31856 GND
```

With two thermocouples connected to the STM32, share a common ground.

Connect the positive (hot) and negative (cold) junctions of the thermocouple to the positive and negative terminals of the amplifier board.

## Driver Usage

### Files
- `/app/drivers/max31856.h`
- `/app/drivers/max31856.c`

### Driver setup
Two MAX31856 amplifier boards are used to interface with the thermocouples (one per thermocouple).

Each thermocouple is represented by a device struct defined in the driver:
```c
typedef struct {
    SPI_HandleTypeDef *hspi;
    GPIO_TypeDef *cs_port;
    uint16_t cs_pin;
} max31856_t;
```

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
- enables automatic / continuous temperature conversion / readings
- sets thermocouple type to K-type

### Reading Temperature
Temperature <b>differences </b> can be read with:
```c
float temp = max31856_read_temp(&tc1);
```
Returned value is the temperature DIFFERENCE in Celsius (i.e. temp difference between temp when the task first starts and current temp).

On Mac: open up terminal, then `screen /dev/tty.usbmodem21103 115200`, replace `usbmodem ... ` with which ever port your stm32 is connected to (check with `ls /dev/tty.usb*`)

On Windows: Use Putty for serial output (add instructions)

## Example FreeRTOS task:

For testing purposes, this current task is defined in the `F401RE.ioc` and `app/boards/F401RE/Core/Src/main.c`. (modify later for use in DataAcquisitionTask etc.)

```c
void StartTestThermoTask(void *argument)
{
  max31856_t tc1;
  max31856_t tc2;
  float temp1, temp2;
  char msg[128];

  max31856_init(&tc1, &hspi1, TC1_CS_GPIO_Port, TC1_CS_Pin);
  max31856_init(&tc2, &hspi2, TC2_CS_GPIO_Port, TC2_CS_Pin);

  osDelay(200);

  for(;;)
  {
    temp1 = max31856_read_temp(&tc1);
    temp2 = max31856_read_temp(&tc2);

    snprintf(msg, sizeof(msg), "TC1: %.2f C, TC2: %.2f C\r\n", temp1, temp2);
    uart_tx(msg);

    osDelay(500);
  }
}
```

## Notes:
- Both SPI configs have been tested with a thermocouple + amplifier board
- Add Windows specific instructions under Reading Temperature section mentioned above
