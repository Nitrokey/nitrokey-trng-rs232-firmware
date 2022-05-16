/*
  True Random Number Generator "NK TRNG-Serial"
    Copyright (C)  Nitrokey, 2022.
    info[at]nitrokey[dot]com
    Nitrokey.com

    This program is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program.  If not, see <https://www.gnu.org/licenses/>.
*/

#ifndef _VIRTUALSERIAL_H_
#define _VIRTUALSERIAL_H_

/* Includes: */
#include <avr/io.h>
#include <avr/wdt.h>
#include <avr/power.h>
#include <avr/interrupt.h>
#include <string.h>
#include <stdio.h>

#include "Descriptors.h"

// #include <LUFA/Drivers/USB/USB.h>
// #include <LUFA/Platform/Platform.h>
#include <LUFA/Drivers/Peripheral/Serial.h>

// TRNG Protocol Bit Positions (no pin definitions)
#define COMP1_FTDI 1
#define COMP2_FTDI 4
#define SWEN1_FTDI 2
#define SWEN2_FTDI 0

/* Function Prototypes: */
void SetupHardware(void);
void GPIO_Init(void);
void handleCommand(int16_t byte);
void disconnect_port(void);
void connect_port(uint8_t port);
void check_gpio(void);

uint8_t Convert_Bit_Order(uint8_t input_reg);

#endif
