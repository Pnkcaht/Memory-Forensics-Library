#include "snapshot.h"

#include <stdlib.h>
#include <stdint.h>

#include "internal.h"
struct mf_snapshot {
    mf_process_t *process;
    struct mf_mem_region *regions;
    size_t region_count;
    uint64_t *hashes;
};

static uint64_t hash_buffer(const uint8_t *buf, size_t len)
{
    uint64_t h = 1469598103934665603ULL;

    for (size_t i = 0; i < len; i++) {
        h ^= buf[i];
        h *= 1099511628211ULL;
    }

    return h;
}

mf_error_t mf_snapshot_create(
    mf_process_t *process,
    mf_snapshot_t **out
)
{
    if (!process || !out) {
        return MF_ERR_INVALID_ARGUMENT;
    }

    struct mf_mem_region *regions = NULL;
    size_t count = 0;

    mf_error_t err =
        process->ctx->os->enumerate_regions(process, &regions, &count);

    if (err != MF_OK || count == 0) {
        return MF_ERR_SNAPSHOT_FAILED;
    }

    mf_snapshot_t *snap = calloc(1, sizeof(*snap));
    if (!snap) {
        free(regions);
        return MF_ERR_NO_MEMORY;
    }

    snap->process = process;
    snap->regions = regions;
    snap->region_count = count;
    snap->hashes = calloc(count, sizeof(uint64_t));

    if (!snap->hashes) {
        free(regions);
        free(snap);
        return MF_ERR_NO_MEMORY;
    }

    for (size_t i = 0; i < count; i++) {
        const struct mf_mem_region *r = &regions[i];

        /* skip unreadable regions */
        if (!(r->permissions & 0x1)) {
            snap->hashes[i] = 0;
            continue;
        }

        size_t size = r->end - r->start;
        uint8_t *buffer = malloc(size);
        if (!buffer) {
            mf_snapshot_destroy(snap);
            return MF_ERR_NO_MEMORY;
        }

        err = process->ctx->os->read_memory(
            process,
            r->start,
            buffer,
            size
        );

        if (err != MF_OK) {
            free(buffer);
            snap->hashes[i] = 0;
            continue;
        }

        snap->hashes[i] = hash_buffer(buffer, size);
        free(buffer);
    }

    *out = snap;
    return MF_OK;
}

void mf_snapshot_destroy(mf_snapshot_t *snapshot)
{
    if (!snapshot) {
        return;
    }

    free(snapshot->regions);
    free(snapshot->hashes);
    free(snapshot);
}

size_t mf_snapshot_region_count(const mf_snapshot_t *s)
{
    return s ? s->region_count : 0;
}

const struct mf_mem_region *
mf_snapshot_regions(const mf_snapshot_t *s)
{
    return s ? s->regions : NULL;
}

const uint64_t *
mf_snapshot_hashes(const mf_snapshot_t *s)
{
    return s ? s->hashes : NULL;
}
