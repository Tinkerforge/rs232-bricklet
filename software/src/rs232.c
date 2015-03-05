/* rs232-bricklet
 * Copyright (C) 2015 Olaf Lüke <olaf@tinkerforge.com>
 *
 * rs232.c: Implementation of RS232 Bricklet messages
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

#include "rs232.h"

#include "brickletlib/bricklet_entry.h"
#include "bricklib/bricklet/bricklet_communication.h"
#include "bricklib/utility/util_definitions.h"
#include "config.h"

uint8_t sc16is740_get_address(void) {
	if(BS->address == I2C_EEPROM_ADDRESS_HIGH) {
		return I2C_ADDRESS_HIGH;
	} else {
		return I2C_ADDRESS_LOW;
	}
}

uint8_t sc16is740_read_register(const uint8_t address) {
	uint8_t value = 0;

	BA->TWID_Read(BA->twid,
	              sc16is740_get_address(),
	              address << 3,
	              1,
	              &value,
	              1,
	              NULL);

	return value;
}

void sc16is740_write_register(const uint8_t address, uint8_t value) {
	BA->TWID_Write(BA->twid,
	               sc16is740_get_address(),
	               address << 3,
	               1,
	               &value,
	               1,
	               NULL);
}

void sc16is740_reset(void) {
	BS->pin3_pwm.type = PIO_OUTPUT_0;
	BA->PIO_Configure(&BS->pin3_pwm, 1);

	SLEEP_MS(5);

	BS->pin3_pwm.type = PIO_OUTPUT_1;
	BA->PIO_Configure(&BS->pin3_pwm, 1);

	SLEEP_MS(100); // Wait for SC16IS740 initialization and clock stabilization
}

void sc16is740_init(void) {
	if(BA->mutex_take(*BA->mutex_twi_bricklet, 10)) {
		BA->bricklet_select(BS->port - 'a');

		// Configure UART
		uint8_t lcr = sc16is740_read_register(I2C_INTERNAL_ADDRESS_LCR);

		lcr |= (1 << 1) | (1 << 0); // Word length 8 bit: LCR[1], LCR[0] = 1, 1
		lcr &= ~(1 << 2); // 1 stop bit: LCR[2] = 0
		lcr &= ~(1 << 3); // No parity: LCR[3] = 0
		sc16is740_write_register(I2C_INTERNAL_ADDRESS_LCR, lcr);

		// Configure baudrate: ((14.7456 MHz * 1000000) / 1) /
		//                     (115200 baud * 16) = 8
		lcr |= 1 << 7; // Divisor latch enable: LCR[7] = 1
		sc16is740_write_register(I2C_INTERNAL_ADDRESS_LCR, lcr);

		sc16is740_write_register(I2C_INTERNAL_ADDRESS_DLL, 8);
		sc16is740_write_register(I2C_INTERNAL_ADDRESS_DLH, 0);

		lcr = sc16is740_read_register(I2C_INTERNAL_ADDRESS_LCR);
		lcr &= ~(1 << 7); // Divisor latch disable: LCR[7] = 0
		sc16is740_write_register(I2C_INTERNAL_ADDRESS_LCR, lcr);

		// Configure FIFOs
		uint8_t fcr = sc16is740_read_register(I2C_INTERNAL_ADDRESS_FCR);

		fcr |= (1 << 0); // Enable FIFOs: FCR[0] = 1
		sc16is740_write_register(I2C_INTERNAL_ADDRESS_FCR, fcr);

		BA->bricklet_deselect(BS->port - 'a');
		BA->mutex_give(*BA->mutex_twi_bricklet);
	}
}

void constructor(void) {
	_Static_assert(sizeof(BrickContext) <= BRICKLET_CONTEXT_MAX_SIZE,
	               "BrickContext too big");

	sc16is740_reset();
	sc16is740_init();
}

void destructor(void) {
}

void tick(const uint8_t tick_type) {
	if(tick_type & TICK_TASK_TYPE_CALCULATION) {

	}

	if(tick_type & TICK_TASK_TYPE_MESSAGE) {

	}
}

void invocation(const ComType com, const uint8_t *data) {
	switch(((StandardMessage*)data)->header.fid) {
		case FID_READ:                read(com, (Read*)data); break;
		case FID_WRITE:               write(com, (Write*)data); break;
		case FID_ENABLE_CALLBACK:     enable_callback(com, (EnableCallback*)data); break;
		case FID_IS_CALLBACK_ENABLED: is_callback_enabled(com, (IsCallbackEnabled*)data); break;
		case FID_SET_CONFIGURATION:   set_configuration(com, (SetConfiguration*)data); break;
		case FID_GET_CONFIGURATION:   get_configuration(com, (GetConfiguration*)data); break;
		default: BA->com_return_error(data, sizeof(MessageHeader), MESSAGE_ERROR_CODE_NOT_SUPPORTED, com); break;
	}
}

void read(const ComType com, const Read *data) {

}

void write(const ComType com, const Write *data) {

}

void enable_callback(const ComType com, const EnableCallback *data) {

}

void is_callback_enabled(const ComType com, const IsCallbackEnabled *data) {

}

void set_configuration(const ComType com, const SetConfiguration *data) {

}

void get_configuration(const ComType com, const GetConfiguration *data) {

}
