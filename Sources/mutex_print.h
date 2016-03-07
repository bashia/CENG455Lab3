#include <stdio.h>
#include <mqx.h>
#include <bsp.h>
#include <mutex.h>
#include <stdarg.h>
#include "constants.h"

/* Task IDs */
#define MAIN_TASK     5
#define PRINT_TASK    6

extern void main_task(uint32_t initial_data);
extern void print_task(uint32_t initial_data);

MUTEX_STRUCT   print_mutex;

int init_mutex();

void mprintf(const char* format, ...);
