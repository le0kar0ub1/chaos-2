/* ------------------------------------------------------------------------ *\
**
**  This file is part of the Chaos Kernel, and is made available under
**  the terms of the GNU General Public License version 2.
**
**  Copyright (C) 2017 - Benjamin Grange <benjamin.grange@epitech.eu>
**
\* ------------------------------------------------------------------------ */

#include <kernel/init.h>
#include <kernel/interrupts.h>
#include <kernel/mutex.h>
#include <arch/x86/apic.h>
#include <arch/x86/asm.h>
#include <drivers/keyboard.h>
#include <stdio.h>

static char const fr_azerty_charset[128] =
{
	0,
	[16] = 'a',
	[17] = 'z',
	[18] = 'e',
	[19] = 'r',
	[20] = 't',
	[21] = 'y',
	[22] = 'u',
	[23] = 'i',
	[24] = 'o',
	[25] = 'p',
	[30] = 'q',
	[31] = 's',
	[32] = 'd',
	[33] = 'f',
	[34] = 'g',
	[35] = 'h',
	[36] = 'j',
	[37] = 'k',
	[38] = 'l',
	[39] = 'm',
	[44] = 'w',
	[45] = 'x',
	[46] = 'c',
	[47] = 'v',
	[48] = 'b',
	[49] = 'n',
	[28] = '\n',
	[14] = '\b',
	[57] = ' ',
};

static char input_buffer[PAGE_SIZE] = { 0 };
static size_t input_write_idx = 0;
static size_t input_read_idx = 0;
static struct mutex keyboard_lock = MUTEX_DEFAULT;

static void
keyboard_ihandler(void)
{
	unsigned char code;
	size_t check_overwrite;

	code = inb(KEYBOARD_IO_PORT);
	if (!(code & 0x80)) {
		code = fr_azerty_charset[(int)code];
		mutex_acquire(&keyboard_lock);
		check_overwrite = (input_write_idx + 1) % PAGE_SIZE;
		if (check_overwrite != input_read_idx && code != '\0')
		{
			printf("%c", code);
			input_buffer[input_write_idx] = code;
			input_write_idx = check_overwrite;
		}
		mutex_release(&keyboard_lock);
	}
	apic_eoi();
}

/*
** Sends the next char or loops until the user presses a key.
*/
char
keyboard_next_input(void)
{
	char c;

	while (1)
	{
		mutex_acquire(&keyboard_lock);
		if (input_read_idx == input_write_idx)
			mutex_release(&keyboard_lock);
		else
			break;
	}
	c = input_buffer[input_read_idx];
	input_read_idx = (input_read_idx + 1) % PAGE_SIZE;
	mutex_release(&keyboard_lock);
	return (c);
}

static void
keyboard_init(void)
{
	register_int_handler(INT_KEYBOARD, &keyboard_ihandler);
}

NEW_INIT_HOOK(keyboard_init, &keyboard_init, INIT_LEVEL_DRIVERS);
