#ifndef MF_LINUX_PROCESS_MAPS_H
#define MF_LINUX_PROCESS_MAPS_H

#include <stddef.h>

#include "internal.h"   /* struct mf_process, mf_mem_region */
#include "mf_types.h"   /* mf_error_t */

/*
 * Linux-specific helpers for process memory maps.
 */

/*
 * Enumerates memory regions of a process using /proc/<pid>/maps.
 *
 * On success:
 *  - regions will point to a heap-allocated array
 *  - count will contain the number of regions
 *
 * Caller is responsible for freeing regions.
 */
mf_error_t mf_linux_enumerate_maps(
    struct mf_process *process,
    struct mf_mem_region **regions,
    size_t *count
);

#endif /* MF_LINUX_PROCESS_MAPS_H */
