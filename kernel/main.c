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
#include <kernel/memory.h>
#include <kernel/thread.h>
#include <arch/linker.h>
#include <stdio.h>

#include <kernel/initrd.h> /* TODO FIXME For debugging purposes */
#include <kernel/exec.h>
#include <kernel/cpu.h>
#include <kernel/vaspace.h>

/*
** Kernel main entry point
*/
void
kmain(void)
{
	/* Put us in some sort of thread-context */
	thread_early_init();

	/* Trigger all init levels */
	trigger_init_levels(INIT_LEVEL_EARLIEST, INIT_LEVEL_LATEST);

	printf("Welcome to ChaOS\n\n");

	struct initrd_virt const *virt = initrd_get_virtual();
	struct thread *t = current_thread_acquire_write();
	rwlock_acquire_write(&t->vaspace->rwlock);

	/* exec will release the virtual address space and the current thread */
	assert_eq(exec(virt->start, virt->len), OK);

	panic("Leaving kmain()");
}

/* Mark the kernel as physically reserved */
NEW_PMM_RESERVED_AREA(kernel, NULL_FRAME, KERNEL_PHYSICAL_END);
