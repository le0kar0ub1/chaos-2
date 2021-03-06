/* ------------------------------------------------------------------------ *\
**
**  This file is part of the Chaos Kernel, and is made available under
**  the terms of the GNU General Public License version 2.
**
**  Copyright (C) 2017 - Benjamin Grange <benjamin.grange@epitech.eu>
**
\* ------------------------------------------------------------------------ */

#ifndef _KERNEL_VASPACE_H_
# define _KERNEL_VASPACE_H_

# include <chaoserr.h>
# include <kernel/memory.h>
# include <kernel/spin_rwlock.h>
# include <kernel/vseg.h>
# include <kernel/vmm.h>
# include <kernel/thread.h>
# include <kernel/vector.h>
# include <arch/vaspace.h>

struct vseg_vector
{
	NEW_VECTOR(struct vseg);
};

/*
** Represents the virtual address space shared by a couple of threads.
**
** The golden rule is that we must _NEVER_ have two virtual segments
** overlapping.
**
** Remember that vsegs are inclusives, which means that a segment that is
** one page long has the same 'start' and 'end' addresses.
*/
struct vaspace
{
	/*
	** All the memory segments composing this virtual address space.
	** This vector is sorted in ascending order of vseg->start.
	*/
	struct vseg_vector vsegs;

	struct arch_vaspace arch;

	/* RWlock to lock the virtual address space */
	struct spin_rwlock rwlock;

	/* Number of threads sharing this address space */
	uint count;
};

/*
** Returns the current virtual address space.
** This functions asserts a thread context.
**
** THIS WILL NOT TRY TO LOCK THE CURRENT THREAD NOR THE CURRENT VIRTUAL ADDRESS SPACE.
** BE CAREFUL WHEN USING IT. YOU HAVE BEEN WARNED.
*/
static inline struct vaspace *
current_vaspace(void)
{
	return (current_thread()->vaspace);
}

static inline struct vaspace *
current_vaspace_acquire_read(void)
{
	struct vaspace *v;

	v = current_vaspace();
	spin_rwlock_acquire_read(&v->rwlock);
	return (v);
}

static inline struct vaspace *
current_vaspace_acquire_write(void)
{
	struct vaspace *v;

	v = current_vaspace();
	spin_rwlock_acquire_write(&v->rwlock);
	return (v);
}

static inline void
current_vaspace_release_read(void)
{
	struct vaspace *v;

	v = current_vaspace();
	spin_rwlock_release_read(&v->rwlock);
}

static inline void
current_vaspace_release_write(void)
{
	struct vaspace *v;

	v = current_vaspace();
	spin_rwlock_release_write(&v->rwlock);
}

/*
** Arch-dependant function, reimplemented for each architecture.
*/
void		arch_vaspace_switch(struct vaspace *new);
struct vaspace	*arch_new_vaspace(void);
void		arch_vaspace_free(void);

status_t	vaspace_init(struct vaspace *vaspace);
void		vaspace_reset(void);
void		vaspace_free(void);
void		vaspace_cleanup(void);
status_t	vaspace_new_vseg(virtaddr_t start, size_t size, mmap_flags_t flags);
status_t	vaspace_add_vseg(struct vaspace *vaspace, virtaddr_t start, virtaddr_t end, vseg_flags_t);
virtaddr_t	vaspace_new_random_vseg(size_t size, mmap_flags_t flags);
status_t	vaspace_add_kernel_vseg(struct vaspace *vaspace);
void		vaspace_remove_vseg(virtaddr_t, munmap_flags_t flags);
void		vaspace_remove_vseg_by_idx(size_t idx, munmap_flags_t flags);
void		vaspace_dump(struct vaspace *vaspace);

#endif /* !_KERNEL_VASPACE_H_ */
