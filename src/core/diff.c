#include "diff.h"

#include <stdlib.h>
#include <stdint.h>

struct mf_diff {
    size_t modified_regions;
    size_t modified_bytes;
    size_t added_regions;
    size_t removed_regions;
};

static int same_region(
    const struct mf_mem_region *a,
    const struct mf_mem_region *b
)
{
    return a->start == b->start && a->end == b->end;
}

mf_error_t mf_snapshot_diff(
    const mf_snapshot_t *old_snap,
    const mf_snapshot_t *new_snap,
    mf_diff_t **out
)
{
    if (!old_snap || !new_snap || !out) {
        return MF_ERR_INVALID_ARGUMENT;
    }

    size_t old_count = mf_snapshot_region_count(old_snap);
    size_t new_count = mf_snapshot_region_count(new_snap);

    const struct mf_mem_region *old_regions =
        mf_snapshot_regions(old_snap);
    const struct mf_mem_region *new_regions =
        mf_snapshot_regions(new_snap);

    const uint64_t *old_hashes =
        mf_snapshot_hashes(old_snap);
    const uint64_t *new_hashes =
        mf_snapshot_hashes(new_snap);

    mf_diff_t *d = calloc(1, sizeof(*d));
    if (!d) {
        return MF_ERR_NO_MEMORY;
    }

    /* Track matched regions in new snapshot */
    uint8_t *matched = calloc(new_count, 1);
    if (!matched) {
        free(d);
        return MF_ERR_NO_MEMORY;
    }

    /* Check old → new */
    for (size_t i = 0; i < old_count; i++) {
        int found = 0;

        for (size_t j = 0; j < new_count; j++) {
            if (same_region(&old_regions[i], &new_regions[j])) {
                found = 1;
                matched[j] = 1;

                if (old_hashes[i] != new_hashes[j]) {
                    d->modified_regions++;
                    d->modified_bytes +=
                        old_regions[i].end - old_regions[i].start;
                }
                break;
            }
        }

        if (!found) {
            d->removed_regions++;
        }
    }

    /* Anything not matched is new */
    for (size_t j = 0; j < new_count; j++) {
        if (!matched[j]) {
            d->added_regions++;
        }
    }

    free(matched);
    *out = d;
    return MF_OK;
}

/* ============================
 * Accessors
 * ============================ */

size_t mf_diff_modified_region_count(const mf_diff_t *d)
{
    return d ? d->modified_regions : 0;
}

size_t mf_diff_modified_bytes(const mf_diff_t *d)
{
    return d ? d->modified_bytes : 0;
}

size_t mf_diff_added_region_count(const mf_diff_t *d)
{
    return d ? d->added_regions : 0;
}

size_t mf_diff_removed_region_count(const mf_diff_t *d)
{
    return d ? d->removed_regions : 0;
}

void mf_diff_destroy(mf_diff_t *d)
{
    free(d);
}
