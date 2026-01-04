#ifndef MF_SNAPSHOT_H
#define MF_SNAPSHOT_H

#include <stddef.h>
#include <stdint.h>
#include "mf_types.h"

typedef struct mf_snapshot mf_snapshot_t;

mf_error_t mf_snapshot_create(
    mf_process_t *process,
    mf_snapshot_t **out
);

void mf_snapshot_destroy(mf_snapshot_t *snapshot);

size_t mf_snapshot_region_count(const mf_snapshot_t *s);
const struct mf_mem_region *mf_snapshot_regions(const mf_snapshot_t *s);
const uint64_t *mf_snapshot_hashes(const mf_snapshot_t *s);

#endif /* MF_SNAPSHOT_H */
