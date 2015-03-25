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

#define NUM_BAUDRATES 13
#define NUM_PARITIES 5
#define NUM_STOPBITS 2
#define NUM_WORDLENGTH 4
#define NUM_FLOWCONTROL 2

const uint16_t baudrates[NUM_BAUDRATES] = {
	3072, //    300 baud
	1536, //    600 baud
	768,  //   1200 baud
	384,  //   2400 baud
	192,  //   4800 baud
	96,   //   9600 baud
	64,   //  14400 baud
	48,   //  19200 baud
	32,   //  28800 baud
	24,   //  38400 baud
	16,   //  57600 baud
	8,    // 115200 baud
	4     // 230400 baud
};

const uint8_t parities[NUM_PARITIES] = {
	0, // None
	1 << 3, // Odd
	(1 << 3) | (1 << 4), // Even
	(1 << 3) | (1 << 5), // Forced Parity 1
	(1 << 3) | (1 << 4) | (1 << 5), // Forced Parity 0
};

const uint8_t stopbits[NUM_STOPBITS] = {
	0, // 1 Stopbit
	1 << 2 // 2 Stopbits
};

const uint8_t wordlength[NUM_WORDLENGTH] = {
	0, // 5
	1 << 0, // 6
	1 << 1, // 7
	(1 << 0) | (1 << 1) // 8
};

const uint8_t hardware_flowcontrol[NUM_FLOWCONTROL] = {
	0, // Hardware Flow Control Disabled
	(1 << 6) | (1 << 7), // Hardware Flow Control Enabled
};

const uint8_t software_flowcontrol[NUM_FLOWCONTROL] = {
	0, // Software Flow Control Disabled
	(1 << 3) | (1 << 1), // Software Flow Control Enabled
};

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
	// Reset SC16IS740
	PIN_NRESET.type = PIO_OUTPUT_0;
	PIN_NRESET.attribute = PIO_DEFAULT;
	BA->PIO_Configure(&PIN_NRESET, 1);

	SLEEP_MS(5);

	PIN_NRESET.type = PIO_OUTPUT_1;
	BA->PIO_Configure(&PIN_NRESET, 1);

	SLEEP_MS(100); // Wait for SC16IS740 initialization and clock stabilization
}

void sc16is740_reconfigure(void) {
	if(BA->mutex_take(*BA->mutex_twi_bricklet, 10)) {
		BA->bricklet_select(BS->port - 'a');

		// Enable TCR and TLR
		sc16is740_write_register(I2C_INTERNAL_ADDRESS_MCR, 1 << 2);

		// Write 0xBF to LCR to be able to access EFR
		sc16is740_write_register(I2C_INTERNAL_ADDRESS_LCR, 0xBF);
		// Enable or Disable Hardware Flow Control
		sc16is740_write_register(I2C_INTERNAL_ADDRESS_EFR, hardware_flowcontrol[BC->hardware_flowcontrol] | (1 << 4) | 0b1010);

		// Configure UART
		uint8_t lcr = stopbits[BC->stopbits-1] | parities[BC->parity] | wordlength[BC->wordlength-5];
		sc16is740_write_register(I2C_INTERNAL_ADDRESS_LCR, lcr);

		// Configure baudrate: ((14.7456 MHz * 1000000) / 1) /
		//                     (X baud * 16) = baudrates[BC->baudrate]
		lcr |= 1 << 7; // Divisor latch enable: LCR[7] = 1
		sc16is740_write_register(I2C_INTERNAL_ADDRESS_LCR, lcr);

		sc16is740_write_register(I2C_INTERNAL_ADDRESS_DLL, baudrates[BC->baudrate]);
		sc16is740_write_register(I2C_INTERNAL_ADDRESS_DLH, 0);

		lcr = sc16is740_read_register(I2C_INTERNAL_ADDRESS_LCR);
		lcr &= ~(1 << 7); // Divisor latch disable: LCR[7] = 0
		sc16is740_write_register(I2C_INTERNAL_ADDRESS_LCR, lcr);

		BA->bricklet_deselect(BS->port - 'a');
		BA->mutex_give(*BA->mutex_twi_bricklet);
	}
}

void sc16is740_init(void) {
	if(BA->mutex_take(*BA->mutex_twi_bricklet, 10)) {
		BA->bricklet_select(BS->port - 'a');
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

	PIN_NIRQ.type = PIO_INPUT;
	PIN_NIRQ.attribute = PIO_DEFAULT;
	BA->PIO_Configure(&PIN_NIRQ, 1);

	BC->callback_enabled = false;
	BC->in_sync = true;

	BC->baudrate = 11;
	BC->parity = 0;
	BC->stopbits = 1;
	BC->wordlength = 8;
	BC->hardware_flowcontrol = 0;
	BC->software_flowcontrol = 0;

	read_configuration_from_eeprom();
	sc16is740_reset();
	sc16is740_init();
	sc16is740_reconfigure();
}

void destructor(void) {
}

uint8_t size_in(void) {
	if(BC->in_start == BC->in_end) {
		return 0;
	}

	if(BC->in_start < BC->in_end) {
		return BC->in_end - BC->in_start;
	}

	return (BUFFER_SIZE - BC->in_start) + BC->in_end;
}

uint8_t size_out(void) {
	if(BC->out_start == BC->out_end) {
		return 0;
	}

	if(BC->out_start < BC->out_end) {
		return BC->out_end - BC->out_start;
	}

	return (BUFFER_SIZE - BC->out_start) + BC->out_end;
}

bool new_char_out(char out) {
	BC->out[BC->out_end] = out;

	// If buffer is full, we ignore new char
	if((BC->out_end+1) % BUFFER_SIZE != BC->out_start) {
		BC->out_end = (BC->out_end + 1) % BUFFER_SIZE;
		return true;
	}

	return false;
}

bool get_char_in(char *c) {
	if(BC->in_start != BC->in_end) {
		*c = BC->in[BC->in_start];
		BC->in_start = (BC->in_start + 1) % BUFFER_SIZE;
		return true;
	}

	return false;
}

void tick(const uint8_t tick_type) {
	if(tick_type & TICK_TASK_TYPE_CALCULATION) {
		if(BA->mutex_take(*BA->mutex_twi_bricklet, 10)) {
			BA->bricklet_select(BS->port - 'a');

			uint8_t rxtxdone = 0;
			do {
				uint8_t lsr = sc16is740_read_register(I2C_INTERNAL_ADDRESS_LSR);

				if((lsr & SC16IS740_LSR_DATA_IN_RECEIVER) && (size_out() < (BUFFER_SIZE-1) )) {
					new_char_out(sc16is740_read_register(I2C_INTERNAL_ADDRESS_RHR));
				} else {
					rxtxdone |= (1 << 0);
				}

				if((lsr & SC16IS740_LSR_THR_IS_EMPTY) && (size_in() > 0)) {
					char c;
					if(get_char_in(&c)) {
						sc16is740_write_register(I2C_INTERNAL_ADDRESS_THR, c);
					}
				} else {
					rxtxdone |= (1 << 1);
				}
			} while(rxtxdone != ((1 << 0) | (1 << 1)));

			BA->bricklet_deselect(BS->port - 'a');
			BA->mutex_give(*BA->mutex_twi_bricklet);
		}
	}

	if(tick_type & TICK_TASK_TYPE_MESSAGE) {
		uint8_t length = size_out();
		if(BC->callback_enabled && length > 0) {
			ReadCallback rc;
			BA->com_make_default_header(&rc, BS->uid, sizeof(ReadCallback), FID_READ_CALLBACK);
			rc.length = length;

			for(uint8_t i = 0; i < rc.length; i++) {
				rc.message[i] = BC->out[BC->out_start];
				BC->out_start = (BC->out_start + 1) % BUFFER_SIZE;
			}

			for(uint8_t i = rc.length; i < MESSAGE_LENGTH; i++) {
				rc.message[i] = 0;
			}

			BA->send_blocking_with_timeout(&rc,
										   sizeof(ReadCallback),
										   *BA->com_current);
		}
	}
}

void invocation(const ComType com, const uint8_t *data) {
	switch(((StandardMessage*)data)->header.fid) {
		case FID_READ:                read(com, (Read*)data); break;
		case FID_WRITE:               write(com, (Write*)data); break;
		case FID_ENABLE_CALLBACK:     enable_callback(com, (EnableCallback*)data); break;
		case FID_DISABLE_CALLBACK:    disable_callback(com, (DisableCallback*)data); break;
		case FID_IS_CALLBACK_ENABLED: is_callback_enabled(com, (IsCallbackEnabled*)data); break;
		case FID_SET_CONFIGURATION:   set_configuration(com, (SetConfiguration*)data); break;
		case FID_GET_CONFIGURATION:   get_configuration(com, (GetConfiguration*)data); break;
		default: BA->com_return_error(data, sizeof(MessageHeader), MESSAGE_ERROR_CODE_NOT_SUPPORTED, com); break;
	}
}

void read(const ComType com, const Read *data) {
	ReadReturn rr;

	rr.header         = data->header;
	rr.header.length  = sizeof(ReadReturn);
	rr.length         = size_out();

	for(uint8_t i = 0; i < rr.length; i++) {
		rr.message[i] = BC->out[BC->out_start];
		BC->out_start = (BC->out_start + 1) % BUFFER_SIZE;
	}

	for(uint8_t i = rr.length; i < MESSAGE_LENGTH; i++) {
		rr.message[i] = 0;
	}

	BA->send_blocking_with_timeout(&rr, sizeof(ReadReturn), com);
}

void write(const ComType com, const Write *data) {
	if((data->length > MESSAGE_LENGTH) || (size_in() + data->length >= BUFFER_SIZE))  {
		BA->com_return_error(data, sizeof(MessageHeader), MESSAGE_ERROR_CODE_INVALID_PARAMETER, com);
		return;
	}

	for(uint8_t i = 0; i < data->length; i++) {
		BC->in[BC->in_end] = data->message[i];
		BC->in_end = (BC->in_end + 1) % BUFFER_SIZE;

		// Overflow handling, this should not be reachable!
		if(BC->in_end == BC->in_start) {
			BC->in_start = (BC->in_start + 1) % BUFFER_SIZE;
		}
	}
}

void enable_callback(const ComType com, const EnableCallback *data) {
	BC->callback_enabled = true;
}

void disable_callback(const ComType com, const DisableCallback *data) {
	BC->callback_enabled = false;
}

void is_callback_enabled(const ComType com, const IsCallbackEnabled *data) {
	IsCallbackEnabledReturn icer;

	icer.header         = data->header;
	icer.header.length  = sizeof(IsCallbackEnabledReturn);
	icer.enabled        = BC->callback_enabled;

	BA->send_blocking_with_timeout(&icer, sizeof(IsCallbackEnabledReturn), com);
}

void set_configuration(const ComType com, const SetConfiguration *data) {
	if((data->parity >= NUM_PARITIES) ||
	   (data->stopbits != 1 && data->stopbits != 2) ||
	   (data->wordlength < 5 || data->wordlength > 8) ||
	   (data->hardware_flowcontrol >= NUM_FLOWCONTROL) ||
	   (data->software_flowcontrol >= NUM_FLOWCONTROL) ||
	   (data->baudrate >= NUM_BAUDRATES)) {
		BA->com_return_error(data, sizeof(MessageHeader), MESSAGE_ERROR_CODE_INVALID_PARAMETER, com);
		return;
	}

	BC->parity               = data->parity;
	BC->stopbits             = data->stopbits;
	BC->baudrate             = data->baudrate;
	BC->wordlength           = data->wordlength;
	BC->hardware_flowcontrol = data->hardware_flowcontrol;
	BC->software_flowcontrol = data->software_flowcontrol;
	write_configuration_to_eeprom();
	sc16is740_reconfigure();
}

void get_configuration(const ComType com, const GetConfiguration *data) {
	GetConfigurationReturn gcr;

	gcr.header               = data->header;
	gcr.header.length        = sizeof(GetConfigurationReturn);
	gcr.parity               = BC->parity;
	gcr.stopbits             = BC->stopbits;
	gcr.baudrate             = BC->baudrate;
	gcr.wordlength           = BC->wordlength;
	gcr.hardware_flowcontrol = BC->hardware_flowcontrol;
	gcr.software_flowcontrol = BC->software_flowcontrol;

	BA->send_blocking_with_timeout(&gcr, sizeof(GetConfigurationReturn), com);
}

void write_configuration_to_eeprom(void) {
	uint8_t conf[10] = {0xDE, 0xAD, 0xBE, 0xEF, BC->parity, BC->stopbits, BC->baudrate, BC->wordlength, BC->hardware_flowcontrol, BC->software_flowcontrol};
	BA->bricklet_select(BS->port - 'a');
	BA->i2c_eeprom_master_write(BA->twid->pTwi,
	                            CONFIGURATION_EEPROM_POSITION,
	                            (const char*)conf,
	                            10);
	BA->bricklet_deselect(BS->port - 'a');
}

void read_configuration_from_eeprom(void) {
	uint8_t conf[10];
	BA->bricklet_select(BS->port - 'a');
	BA->i2c_eeprom_master_read(BA->twid->pTwi,
	                           CONFIGURATION_EEPROM_POSITION,
	                           (char*)conf,
	                           10);
	BA->bricklet_deselect(BS->port - 'a');

	if(conf[0] == 0xDE && conf[1] == 0xAD && conf[2] == 0xBE && conf[3] == 0xEF) {
		BC->parity = conf[4];
		BC->stopbits = conf[5];
		BC->baudrate = conf[6];
		BC->wordlength = conf[7];
		BC->hardware_flowcontrol = conf[8];
		BC->software_flowcontrol = conf[9];
	}
}
