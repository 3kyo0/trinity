#pragma once

extern unsigned int num_online_cpus;

#define UNLOCKED 0
#define LOCKED 1

#define __unused__ __attribute((unused))

extern char *progname;

void do_main_loop(void);

int check_tainted(void);

void init_watchdog(void);
unsigned int check_if_fd(unsigned int child);

void regenerate(void);
