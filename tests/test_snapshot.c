#include <stdio.h>
#include <unistd.h>

#include "mf_types.h"
#include "context.h"
#include "process.h"
#include "snapshot.h"
#include "diff.h"


int main(void)
{
    mf_context_t *ctx = NULL;
    mf_process_t *proc = NULL;
    mf_snapshot_t *snap = NULL;

    mf_error_t err;

    /* create context */
    err = mf_context_create(&ctx);
    if (err != MF_OK) {
        printf("context_create failed: %d\n", err);
        return 1;
    }

    /* create process (self) */
    err = mf_process_create(ctx, getpid(), &proc);
    if (err != MF_OK) {
        printf("process_create failed: %d\n", err);
        mf_context_destroy(ctx);
        return 1;
    }

    /* create snapshot */
    err = mf_snapshot_create(proc, &snap);
    if (err != MF_OK) {
        printf("snapshot_create failed: %d\n", err);
        mf_process_destroy(proc);
        mf_context_destroy(ctx);
        return 1;
    }

    size_t count = mf_snapshot_region_count(snap);
    const struct mf_mem_region *regions = mf_snapshot_regions(snap);
    const uint64_t *hashes = mf_snapshot_hashes(snap);

    printf("Snapshot created with %zu regions\n", count);

    for (size_t i = 0; i < count && i < 10; i++) {
        printf(
            "%016lx-%016lx perms=%u hash=%016lx\n",
            regions[i].start,
            regions[i].end,
            regions[i].permissions,
            hashes[i]
        );
    }

    /* cleanup */
    mf_snapshot_destroy(snap);
    mf_process_destroy(proc);
    mf_context_destroy(ctx);

    printf("Snapshot test OK\n");
    return 0;
}
