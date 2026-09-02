/*
 mcp9808.c A simple C library for the Microchip MCP9808 temperature sensor
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

#include "mbtwi.h"

#include "mcp9808.h"

/************************************************************************/
/* Converts the value read from the TA register on the MCP9808 to an int*/
/* representation that is 16x larger than the actual value.             */
/************************************************************************/
inline MCP9808_INT16_T mcp9808_get_temp_int(MCP9808_INT16_T read_value) {
	if (read_value & 0x1000) {
		// Negative temperature. Extend sign bit and shift right by 4
		return (MCP9808_INT16_T)(0xF000 | read_value);
	} else {
		// Positive temperature. Extend sign bit and shift right by 4
		return (MCP9808_INT16_T)(0x0FFF & read_value);
	}
}

/************************************************************************/
/* Converts the value read from the TA register on the MCP9808 to float */
/************************************************************************/
inline MCP9808_FLOAT_T mcp9808_get_temp_float(MCP9808_INT16_T read_value) {
	return ((MCP9808_FLOAT_T)mcp9808_get_temp_int(read_value)) / 16.0;
}

/************************************************************************/
/* Retrieves the information for the specified MCP9808 Device.          */
/************************************************************************/
MCP9808_DEVICE mcp9808_load_device(MCP9808_INT16_T address) {
	MCP9808_DEVICE device;
	
	device.Address = address;
	
	// Read the manufacturer id
	send_twi_byte(device.Address, MCP9808_REG_MANUFACTURER_ID, 0);
	device.ManufacturerId = read_twi_int16(device.Address);

	// Read the device id
	send_twi_byte(device.Address, MCP9808_REG_DEVICE_ID, 0);
	device.DeviceId = read_twi_int16(device.Address);

	// Read the configuration
	send_twi_byte(device.Address, MCP9808_REG_CONFIG, 0);
	device.Configuration = read_twi_int16(device.Address);
	
	// Read the configuration
	send_twi_byte(device.Address, MCP9808_REG_RESOLUTION, 0);
	device.Resolution = read_twi_int16(device.Address);

	return device;
}

void mcp9808_reset_interrupts(MCP9808_DEVICE *device) {
	// Read the config
	send_twi_byte(device->Address, MCP9808_REG_CONFIG, 0);
	read_twi_int16(device->Address);
}

MCP9808_INT8_T mcp9808_apply_configuration(MCP9808_DEVICE *device) {
	MCP9808_INT8_T result = MCP9808_OK;
	
	// Read the config
	send_twi_byte(device->Address, MCP9808_REG_CONFIG, 0);
	MCP9808_INT16_T config = read_twi_int16(device->Address);
	
	if (config & _BV(MCP9808_CONFIG_WIN_LOCK) || config & _BV(MCP9808_CONFIG_CRIT_LOCK)) {
		result |= 0b0001;
	}
	
	// Write the config
	if (mcp9808_write_and_verify_int16(device, MCP9808_REG_CONFIG, device->Configuration & MCP9808_CONF_REG_MASK)) {
		result |= 0b0010;
	}
	
	// Write the config
	if (mcp9808_write_and_verify_int8(device, MCP9808_REG_RESOLUTION, device->Resolution)) {
		result |= 0b0100;
	}
	
	return result;
}

void mcp9808_set_temperature(MCP9808_INT16_T *tempVar, MCP9808_INT8_T tempC) {
	if (tempC & 0x80) {
		*tempVar = (0x1000 | (tempC << 4)) & MCP9808_TEMP_LIM_REG_MASK;
	} else {
		*tempVar = (tempC << 4) & MCP9808_TEMP_LIM_REG_MASK;
	}
}

MCP9808_INT8_T mcp9808_apply_temperature_configuration(MCP9808_DEVICE *device) {
	MCP9808_INT8_T result = MCP9808_OK;
	
	// Write the config
	if (mcp9808_write_and_verify_int16(device, MCP9808_REG_TUPPER, (MCP9808_UINT16_T)device->TUpper & MCP9808_TEMP_REG_MASK)) {
		result |= 0b0001;
	}
	
	if (mcp9808_write_and_verify_int16(device, MCP9808_REG_TLOWER, (MCP9808_UINT16_T)device->TLower & MCP9808_TEMP_REG_MASK)) {
		result |= 0b0010;
	}
	
	if (mcp9808_write_and_verify_int16(device, MCP9808_REG_TCRIT, (MCP9808_UINT16_T)device->TCrit & MCP9808_TEMP_REG_MASK)) {
		result |= 0b0100;
	}
	
	return result;
}

MCP9808_INT8_T mcp9808_write_and_verify_int16(MCP9808_DEVICE *device, MCP9808_INT8_T reg, MCP9808_UINT16_T value) {
	MCP9808_INT8_T buff[3] = { reg, (value & 0x7F00) >> 8, value & 0x00FF };
	send_twi_bin_data(device->Address, 3, buff);
	
	// Verify config
	send_twi_byte(device->Address, reg, 0);
	MCP9808_INT16_T result = read_twi_int16(device->Address);
	if (value != result) {
		return 1;
	}
	
	return MCP9808_OK;
}

MCP9808_INT8_T mcp9808_write_and_verify_int8(MCP9808_DEVICE *device, MCP9808_INT8_T reg, MCP9808_UINT8_T value) {
	MCP9808_INT8_T buff[2] = { reg, value };
	send_twi_bin_data(device->Address, 2, buff);
	
	// Verify config
	send_twi_byte(device->Address, reg, 0);
	if (value != read_twi_int8(device->Address)) {
		return 1;
	}
	
	return MCP9808_OK;
}

MCP9808_INT16_T mcp9808_read_temperature(MCP9808_DEVICE *device) {
	send_twi_byte(device->Address, MCP9808_REG_TA, 0);
	device->Temperature = read_twi_int16(device->Address);
	return device->Temperature;
}

void mcp9808_set_is_shutdown(MCP9808_DEVICE *device, MCP9808_SHUTDOWN_STATE is_shutdown) {
	if (is_shutdown) {
		device->Configuration |= (1 << MCP9808_CONFIG_SHDN);
	} else {
		device->Configuration &= ~(1 << MCP9808_CONFIG_SHDN);
	}
}