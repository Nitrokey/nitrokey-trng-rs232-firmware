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

#include "TRNGSerial.h"
#include "macros.h"

uint8_t getRandomByte(void)
{
	// bit-masks
	const uint8_t COMP1 = 1 << 2;
	const uint8_t COMP2 = 1 << 1;
	const uint8_t SWEN1 = 1 << 0;
	const uint8_t SWEN2 = 1 << 3;

	const uint8_t delay = 9;

	uint8_t result = 0;
	uint8_t comp_state, bit, sw_new, comp_mask;
	bool odd;

	for (int i = 0; i < 8; ++i)
	{
		_delay_loop_1(delay);
		odd = i & 1;
		if (odd)
		{
			sw_new = SWEN1;
			comp_mask = COMP2;
		}
		else
		{
			sw_new = SWEN2;
			comp_mask = COMP1;
		}
		comp_state = PINB;
		PORTB = sw_new;
		bit = (comp_state & comp_mask) ? 1 : 0;
		result = (result << 1) | bit;
	}
	return result;
}

/** Main program entry point. This routine contains the overall program flow, including initial
 *  setup of all components and the main program loop.
 */
int main(void)
{
	// init
	SetupHardware();
	GlobalInterruptEnable();
	uint8_t send_byte;

	// loop
	for (;;)
	{
		send_byte = getRandomByte();
		// send as byte
		if (Serial_IsSendReady())
		{
			// Serial_SendByte('U');
			Serial_SendByte(send_byte);
		}
	}
}

/** Configures the board hardware and chip peripherals for the demo's functionality. */
void SetupHardware(void)
{
#if (ARCH == ARCH_AVR8)
	/* Disable watchdog if enabled by bootloader/fuses */
	MCUSR &= ~(1 << WDRF);
	wdt_disable();

	/* Disable clock division */
	clock_prescale_set(clock_div_1);
#endif

	/* Hardware Initialization */
	Serial_Init(115200, false);
	GPIO_Init();
}

void GPIO_Init(void)
{
	// output
	set_bit(DDRB, PB0); // set SW1EN as output
	set_bit(DDRB, PB3); // set SW2EN as output
	clear_bit(PORTB, PB3);
	clear_bit(PORTB, PB0);

	// input
	clear_bit(DDRB, PB1); // set COMP2 as input
	clear_bit(DDRB, PB2); // set COMP1 as input
	set_bit(PORTB, PB1);  // set COMP2 Pullup
	set_bit(PORTB, PB2);  // set COMP1 Pullup
}
