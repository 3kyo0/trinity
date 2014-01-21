/*
 * SYSCALL_DEFINE2(munmap, unsigned long, addr, size_t, len)
 */
#include <stdlib.h>
#include "arch.h"
#include "maps.h"
#include "random.h"
#include "sanitise.h"
#include "shm.h"

#define WHOLE 1
static int action;

static void sanitise_munmap(int childno)
{
	struct map *map;
	unsigned long len;
	unsigned long nr_pages;
	unsigned long offset, offsetpagenr;

	map = common_set_mmap_ptr_len(childno);
	nr_pages = map->size / page_size;

	action = 0;

	switch (rand() % 20) {
	case 0:
		/* delete the whole mapping. */
		delete_mapping(childno, map);
		action = WHOLE;
		return;

	case 1 ... 10:
		/* unmap a range of the mapping. */
		offsetpagenr = (rand() % nr_pages);
		offset = offsetpagenr * page_size;
		shm->a1[childno] = (unsigned long) map->ptr + offset;

		len = (rand() % (nr_pages - offsetpagenr)) + 1;
		len *= page_size;
		shm->a2[childno] = len;
		return;

	case 11 ... 19:
		/* just unmap 1 page of the mapping. */
		shm->a1[childno] = (unsigned long) map->ptr;
		shm->a1[childno] += (rand() % map->size) & PAGE_MASK;
		shm->a2[childno] = page_size;
		return;

	default:
		break;
	}
}

static void post_munmap(int childno)
{
	struct map *map = (struct map *) shm->scratch[childno];

	if (shm->retval[childno] != 0)
		return;

	if (action == WHOLE)
		delete_mapping(childno, map);
}

struct syscallentry syscall_munmap = {
	.name = "munmap",
	.num_args = 2,
	.arg1name = "addr",
	.arg1type = ARG_MMAP,
	.arg2name = "len",
	.group = GROUP_VM,
	.sanitise = sanitise_munmap,
	.post = post_munmap,
};
