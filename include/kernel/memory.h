/* ------------------------------------------------------------------------ *\
**
**  This file is part of the Chaos Kernel, and is made available under
**  the terms of the GNU General Public License version 2.
**
**  Copyright (C) 2017 - Benjamin Grange <benjamin.grange@epitech.eu>
**
\* ------------------------------------------------------------------------ */

#ifndef _KERNEL_MEMORY_H_
# define _KERNEL_MEMORY_H_

# include <chaosdef.h>

typedef uintptr		physaddr_t;
typedef void		*virtaddr_t;

# define P2V(phys) ((virtaddr_t)phys)

#endif /* !_KERNEL_MEMORY_H_ */
