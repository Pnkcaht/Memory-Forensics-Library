#ifndef MF_DIFF_H
#define MF_DIFF_H

#include <stddef.h>
#include "mf_types.h"
#include "snapshot.h"

/* Opaque diff type */
typedef struct mf_diff mf_diff_t;

/* Create diff between two snapshots */
mf_error_t mf_snapshot_diff(
    const mf_snapshot_t *old_snap,
    const mf_snapshot_t *new_snap,
    mf_diff_t **out
);

/* Accessors */
size_t mf_diff_modified_region_count(const mf_diff_t *d);
size_t mf_diff_modified_bytes(const mf_diff_t *d);
size_t mf_diff_added_region_count(const mf_diff_t *d);
size_t mf_diff_removed_region_count(const mf_diff_t *d);

/* Destroy */
void mf_diff_destroy(mf_diff_t *d);

#endif /* MF_DIFF_H */
