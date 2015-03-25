/* rs232-bricklet
 * Copyright (C) 2015 Olaf Lüke <olaf@tinkerforge.com>
 *
 * rs232.h: Implementation of RS232 Bricklet messages
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public
 * License as published by the Free Software Foundation; either
 * version 2 of the License, or (at your option) any later version.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU
 * Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public
 * License along with this library; if not, write to the
 * Free Software Foundation, Inc., 59 Temple Place - Suite 330,
 * Boston, MA 02111-1307, USA.
 */

#ifndef RS232_H
#define RS232_H

#include <stdint.h>

#include "bricklib/com/com_common.h"

#define CONFIGURATION_EEPROM_POSITION (BRICKLET_PLUGIN_MAX_SIZE + 96)

#define FID_WRITE               1
#define FID_READ                2
#define FID_ENABLE_CALLBACK     3
#define FID_DISABLE_CALLBACK    4
#define FID_IS_CALLBACK_ENABLED 5
#define FID_SET_CONFIGURATION   6
#define FID_GET_CONFIGURATION   7
#define FID_READ_CALLBACK       8

#define MESSAGE_LENGTH 60

typedef struct {
	MessageHeader header;
} __attribute__((__packed__)) StandardMessage;

typedef struct {
	MessageHeader header;
	char message[MESSAGE_LENGTH];
	uint8_t length;
} __attribute__((__packed__)) Write;

typedef struct {
	MessageHeader header;
} __attribute__((__packed__)) Read;

typedef struct {
	MessageHeader header;
	char message[MESSAGE_LENGTH];
	uint8_t length;
} __attribute__((__packed__)) ReadReturn;

typedef struct {
	MessageHeader header;
} __attribute__((__packed__)) EnableCallback;

typedef struct {
	MessageHeader header;
} __attribute__((__packed__)) DisableCallback;

typedef struct {
	MessageHeader header;
} __attribute__((__packed__)) IsCallbackEnabled;

typedef struct {
	MessageHeader header;
	bool enabled;
} __attribute__((__packed__)) IsCallbackEnabledReturn;

typedef struct {
	MessageHeader header;
	uint8_t baudrate;
	char parity;
	uint8_t stopbits;
	uint8_t wordlength;
	uint8_t hardware_flowcontrol;
	uint8_t software_flowcontrol;
} __attribute__((__packed__)) SetConfiguration;

typedef struct {
	MessageHeader header;
} __attribute__((__packed__)) GetConfiguration;

typedef struct {
	MessageHeader header;
	uint8_t baudrate;
	uint8_t parity;
	uint8_t stopbits;
	uint8_t wordlength;
	uint8_t hardware_flowcontrol;
	uint8_t software_flowcontrol;
} __attribute__((__packed__)) GetConfigurationReturn;

typedef struct {
	MessageHeader header;
	char message[MESSAGE_LENGTH];
	uint8_t length;
} __attribute__((__packed__)) ReadCallback;

void read(const ComType com, const Read *data);
void write(const ComType com, const Write *data);
void enable_callback(const ComType com, const EnableCallback *data);
void disable_callback(const ComType com, const DisableCallback *data);
void is_callback_enabled(const ComType com, const IsCallbackEnabled *data);
void set_configuration(const ComType com, const SetConfiguration *data);
void get_configuration(const ComType com, const GetConfiguration *data);


#define I2C_EEPROM_ADDRESS_HIGH 84

#define I2C_ADDRESS_HIGH 76 // 0b1001100
#define I2C_ADDRESS_LOW 77 // 0b1001101

#define I2C_INTERNAL_ADDRESS_RHR 0x00
#define I2C_INTERNAL_ADDRESS_THR 0x00
#define I2C_INTERNAL_ADDRESS_IER 0x01
#define I2C_INTERNAL_ADDRESS_FCR 0x02
#define I2C_INTERNAL_ADDRESS_IIR 0x02
#define I2C_INTERNAL_ADDRESS_LCR 0x03
#define I2C_INTERNAL_ADDRESS_LSR 0x05
#define I2C_INTERNAL_ADDRESS_RXLVL 0x09

#define I2C_INTERNAL_ADDRESS_DLL 0x00
#define I2C_INTERNAL_ADDRESS_DLH 0x01
#define I2C_INTERNAL_ADDRESS_EFR 0x02
#define I2C_INTERNAL_ADDRESS_MCR 0x04

#define SC16IS740_LSR_DATA_IN_RECEIVER (1 << 0)
#define SC16IS740_LSR_OVERRUN_ERROR    (1 << 1)
#define SC16IS740_LSR_THR_IS_EMPTY     (1 << 5)

void sc16is740_init(void);
uint8_t sc16is740_get_address(void);
uint8_t sc16is740_read_register(const uint8_t address);
void sc16is740_write_register(const uint8_t address, uint8_t value);
void sc16is740_reset(void);
void write_configuration_to_eeprom(void);
void read_configuration_from_eeprom(void);

void invocation(const ComType com, const uint8_t *data);
void constructor(void);
void destructor(void);
void tick(const uint8_t tick_type);

#endif
