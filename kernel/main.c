/* ------------------------------------------------------------------------ *\
**
**  This file is part of the Chaos Kernel, and is made available under
**  the terms of the GNU General Public License version 2.
**
**  Copyright (C) 2017 - Benjamin Grange <benjamin.grange@epitech.eu>
**
\* ------------------------------------------------------------------------ */

#include <kconfig.h> /* TODO */
#include <arch/x86/smp.h> /* TODO */
#include <drivers/vga.h>
#include <stdio.h>
#include <string.h>

/*
** Kernel main entry point
*/
void
kmain(void)
{
	vga_set_color(VGA_BLACK, VGA_WHITE);
	vga_clear();
	printf("Hello Kernel World!\n");

#if KCONFIG_ENABLE_SMP
	mp_init();
	printf("Nb cpu(s): %u\n", ncpu);
#endif /* KCONFIG_ENABLE_SMP */

	/* Halt */
	while (42);
}
