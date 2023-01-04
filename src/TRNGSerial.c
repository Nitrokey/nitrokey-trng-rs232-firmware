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

#include <avr/io.h>
#include <stdbool.h>
#include <avr/wdt.h>
#include <avr/power.h>
#include <avr/interrupt.h>
#include <util/delay.h>

// SW1EN -> 1 => might toggle COMP1
// SW2EN -> 1 => might toggle COMP2

// bit-masks
#define COMP1 0b0100
#define COMP2 0b0010
#define SW1EN 0b0001
#define SW2EN 0b1000

// determined empirically for maximum entropy and least misfires
#define DELAY 4

// uncomment for testing of "pure" entropy of board
// #define ENT_TEST

// macros
#define BV(bit) (1 << bit)
#define set_bit(sfr, bit) (_SFR_BYTE(sfr) |= BV(bit))	 // old sbi()
#define clear_bit(sfr, bit) (_SFR_BYTE(sfr) &= ~BV(bit)) // old cbi()
#define toggle_bit(sfr, bit) (_SFR_BYTE(sfr) ^= BV(bit))

#define SERIAL_UBBRVAL(Baud) ((((F_CPU / 16) + (Baud / 2)) / (Baud)) - 1)
#define SERIAL_2X_UBBRVAL(Baud) ((((F_CPU / 8) + (Baud / 2)) / (Baud)) - 1)

#define Serial_IsSendReady() (UCSR1A & (1 << UDRE1))
#define Serial_SendByte(DataByte) (UDR1 = DataByte)

static inline void Serial_Init(const uint32_t BaudRate,
							   const bool DoubleSpeed)
{
	UBRR1 = (DoubleSpeed ? SERIAL_2X_UBBRVAL(BaudRate) : SERIAL_UBBRVAL(BaudRate));

	UCSR1C = BV(UCSZ11) | BV(UCSZ10); // 8n1
	UCSR1A = (DoubleSpeed ? BV(U2X1) : 0);
	UCSR1B = BV(TXEN1); // we only need TX

	set_bit(DDRD, PD3);
	set_bit(PORTD, PD2);
}

static inline void GPIO_Init(void)
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

/** Configures the board hardware and chip peripherals */
static inline void SetupHardware(void)
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
	// Serial_Init(230400, true);
	GPIO_Init();
}

/** Main program entry point. This routine contains the overall program flow,
 *  including initial setup of all components and the main program loop.
 */
int main(void)
{
	// init
	SetupHardware();
	cli();

	uint8_t pos = 1;
	uint8_t random_byte = 0;

	// loop
	for (;;)
	{
		PORTB = SW1EN;
		_delay_loop_1(DELAY);
		if (PINB & COMP1)
		{
			random_byte ^= pos;
		}
#ifdef ENT_TEST
		pos <<= 1;
#endif
		PORTB = SW2EN;
		_delay_loop_1(DELAY);
		if (PINB & COMP2)
		{
			random_byte ^= pos;
		}
		if (pos == 1 << 7)
		{
			pos = 1;
#ifdef ENT_TEST
			if (Serial_IsSendReady())
			{
				Serial_SendByte(random_byte);
			}
			random_byte = 0;
#endif
		}
		else
		{
			pos <<= 1;
		}
#ifndef ENT_TEST
		if (Serial_IsSendReady())
		{
			Serial_SendByte(random_byte);
		}
#endif
	}
}
