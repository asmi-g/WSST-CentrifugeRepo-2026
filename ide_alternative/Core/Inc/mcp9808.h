/*
 mcp9808.h A simple C library for the Microchip MCP9808 temperature sensor
 By Michael Blouin (michaelblouin.ca)
 Source available from https://github.com/Mobius5150/mcp9808
 
 =========================================================================
 Copyright 2016 Michael Blouin (michaelblouin.ca)

 Licensed under the Apache License, Version 2.0 (the "License");
 you may not use this file except in compliance with the License.
 You may obtain a copy of the License at

 http://www.apache.org/licenses/LICENSE-2.0

 Unless required by applicable law or agreed to in writing, software
 distributed under the License is distributed on an "AS IS" BASIS,
 WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 See the License for the specific language governing permissions and
 limitations under the License.
 */ 

#ifndef MCP9808_H_
#define MCP9808_H_

/************************************************************************/
/* You can edit the below, but the better way is to simply define the   */
/* constants in your own code and the library will use those.           */
/************************************************************************/
#ifndef MCP9808_INT8_T
#define MCP9808_INT8_T int8_t
#endif

#ifndef MCP9808_UINT8_T
#define MCP9808_UINT8_T uint8_t
#endif

#ifndef MCP9808_INT16_T
#define MCP9808_INT16_T int16_t
#endif

#ifndef MCP9808_UINT16_T
#define MCP9808_UINT16_T uint16_t
#endif

#ifndef MCP9808_FLOAT_T
#define MCP9808_FLOAT_T float
#endif

/************************************************************************/
/* Stop Editing! The below is MCP9808 code.                             */
/************************************************************************/
#define MCP9808_REG_CONFIG          0x01
#define MCP9808_REG_TUPPER          0x02
#define MCP9808_REG_TLOWER          0x03
#define MCP9808_REG_TCRIT           0x04
#define MCP9808_REG_TA              0x05
#define MCP9808_REG_MANUFACTURER_ID 0x06
#define MCP9808_REG_DEVICE_ID       0x07
#define MCP9808_REG_RESOLUTION      0x07

#define MCP9808_TEMP_REG_MASK       0x1FFF
#define MCP9808_TEMP_REG_MASK_UPPER (MCP9808_TEMP_REG_MASK & 0xFF00)
#define MCP9808_TEMP_REG_MASK_LOWER (MCP9808_TEMP_REG_MASK & 0x00FF)

#define MCP9808_TEMP_LIM_REG_MASK       0x1FFC
#define MCP9808_TEMP_LIM_REG_MASK_UPPER (MCP9808_TEMP_LIM_REG_MASK & 0xFF00)
#define MCP9808_TEMP_LIM_REG_MASK_LOWER (MCP9808_TEMP_LIM_REG_MASK & 0x00FF)

#define MCP9808_CONF_REG_MASK       0x01FF
#define MCP9808_CONF_REG_MASK_UPPER (MCP9808_CONF_REG_MASK & 0xFF00)
#define MCP9808_CONF_REG_MASK_LOWER (MCP9808_CONF_REG_MASK & 0x00FF)

#define MCP9808_MANUFACTURER_ID     0x0054
#define MCP9808_MIN_DEVICE_ID       0x0400

#define MCP9808_RESOLUTION_0P5      0b00
#define MCP9808_RESOLUTION_0P25     0b01
#define MCP9808_RESOLUTION_0P125    0b10
#define MCP9808_RESOLUTION_0P0625   0b11

#define MCP9808_CONFIG_ALERT_MODE      0
#define MCP9808_CONFIG_ALERT_POLARITY  1
#define MCP9808_CONFIG_ALERT_SELECT    2
#define MCP9808_CONFIG_ALERT_CONTROL   3
#define MCP9808_CONFIG_ALERT_STATUS    4
#define MCP9808_CONFIG_INT_CLR         5
#define MCP9808_CONFIG_WIN_LOCK        6
#define MCP9808_CONFIG_CRIT_LOCK       7
#define MCP9808_CONFIG_SHDN            8
#define MCP9808_CONFIG_THYST           9

#define MCP9808_ALERT_COMPARATOR 0
#define MCP9808_ALERT_INTERRUPT  1

#define MCP9808_ALERT_ALL_SOURCES 0
#define MCP9808_ALERT_TCRIT_ONLY  1

#define MCP9808_OUTPUT_ACTIVE_LOW  0
#define MCP9808_OUTPUT_ACTIVE_HIGH 1

#define MCP9808_ALERT_ENABLE  1
#define MCP9808_ALERT_DISABLE 0

#define MCP9808_ALERT_STATUS_NOT_ASSERTED 0
#define MCP9808_ALERT_STATUS_ASSERTED     1

#define MCP9808_OK 0

typedef enum {
	THYST_0C,
	THYST_1C5,
	THYST_3C,
	THYST_6C,
} MCP9808_THYST;

typedef enum {
	MCP9808_RUNNING = 0,
	MCP9808_SHUTDOWN = 1,
} MCP9808_SHUTDOWN_STATE;

typedef struct {
	MCP9808_INT8_T   Address;
	MCP9808_INT16_T  ManufacturerId;
	MCP9808_INT16_T  DeviceId;
	MCP9808_UINT16_T Configuration;
	MCP9808_INT16_T  TUpper;
	MCP9808_INT16_T  TLower;
	MCP9808_INT16_T  TCrit;
	MCP9808_UINT16_T Resolution;
	MCP9808_INT16_T  Temperature;
} MCP9808_DEVICE;

MCP9808_INT16_T mcp9808_get_temp_int(MCP9808_INT16_T read_value);
MCP9808_FLOAT_T   mcp9808_get_temp_float(MCP9808_INT16_T read_value);
MCP9808_DEVICE mcp9808_load_device(MCP9808_INT16_T address);
void mcp9808_reset_interrupts(MCP9808_DEVICE *device);
MCP9808_INT16_T mcp9808_read_temperature(MCP9808_DEVICE *device);
MCP9808_INT8_T mcp9808_write_and_verify_int16(MCP9808_DEVICE *device, MCP9808_INT8_T reg, MCP9808_UINT16_T value);
MCP9808_INT8_T mcp9808_write_and_verify_int8(MCP9808_DEVICE *device, MCP9808_INT8_T reg, MCP9808_UINT8_T value);
MCP9808_INT8_T mcp9808_apply_temperature_configuration(MCP9808_DEVICE *device);
void mcp9808_set_temperature(MCP9808_INT16_T *tempVar, MCP9808_INT8_T tempC);
MCP9808_INT8_T mcp9808_apply_configuration(MCP9808_DEVICE *device);
void mcp9808_set_is_shutdown(MCP9808_DEVICE *device, MCP9808_SHUTDOWN_STATE is_shutdown);

#endif /* MCP9808_H_ */