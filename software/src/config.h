/* rs232-bricklet
 * Copyright (C) 2015 Olaf Lüke <olaf@tinkerforge.com>
 *
 * config.h: RS232 Bricklet specific configuration
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

#ifndef CONFIG_H
#define CONFIG_H

#include <stdint.h>
#include <stdbool.h>

#include "bricklib/drivers/board/sam3s/SAM3S.h"
#include "bricklib/drivers/pio/pio.h"

#include "rs232.h"

#define BOARD_MCK 64000000

#define BRICKLET_FIRMWARE_VERSION_MAJOR 2
#define BRICKLET_FIRMWARE_VERSION_MINOR 0
#define BRICKLET_FIRMWARE_VERSION_REVISION 0

#define BRICKLET_HARDWARE_VERSION_MAJOR 1
#define BRICKLET_HARDWARE_VERSION_MINOR 0
#define BRICKLET_HARDWARE_VERSION_REVISION 0

#define BRICKLET_DEVICE_IDENTIFIER 254

#define LOGGING_LEVEL LOGGING_DEBUG
#define DEBUG_BRICKLET 0

#define INVOCATION_IN_BRICKLET_CODE

typedef struct {
	char in[100];
	uint8_t in_start;
	uint8_t in_end;

	char out[100];
	uint8_t out_start;
	uint8_t out_end;
} BrickContext;

#endif
