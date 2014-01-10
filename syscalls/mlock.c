/*
 * SYSCALL_DEFINE2(mlock, unsigned long, start, size_t, len)
 */
#include <stdlib.h>
#include "maps.h"
#include "shm.h"
#include "sanitise.h"

static void sanitise_mlock(int childno)
{
	(void) common_set_mmap_ptr_len(childno);
}

struct syscallentry syscall_mlock = {
	.name = "mlock",
	.num_args = 2,
	.arg1name = "addr",
	.arg1type = ARG_MMAP,
	.arg2name = "len",
	.group = GROUP_VM,
	.sanitise = sanitise_mlock,
};
