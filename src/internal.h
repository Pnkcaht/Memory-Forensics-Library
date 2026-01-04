#ifndef MF_INTERNAL_H
#define MF_INTERNAL_H

#include <stddef.h>
#include <stdint.h>

#include "mf_types.h"

/*
 * Internal structures — NOT public API
 */

/* ============================
 * OS Backend
 * ============================ */

struct mf_os_backend;

/* ============================
 * Context (real definition)
 * ============================ */

struct mf_context {
    struct mf_os_backend *os;
};

/* ============================
 * Process (real definition)
 * ============================ */

struct mf_process {
    int pid;
    void *os_handle;
    struct mf_context *ctx;
};

/* ============================
 * OS Backend Interface
 * ============================ */

struct mf_os_backend {
    const char *name;

    mf_error_t (*attach_process)(struct mf_process *process);
    void (*detach_process)(struct mf_process *process);

    mf_error_t (*enumerate_regions)(
        struct mf_process *process,
        struct mf_mem_region **regions,
        size_t *count
    );

    mf_error_t (*read_memory)(
        struct mf_process *process,
        uintptr_t address,
        void *buffer,
        size_t size
    );
};

/* ============================
 * Snapshot Diff (internal)
 * ============================ */

struct mf_diff {
    size_t modified_regions;
    size_t modified_bytes;
};


#endif /* MF_INTERNAL_H */
