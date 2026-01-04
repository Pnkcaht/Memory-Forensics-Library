#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include "mf_types.h"
#include "context.h"
#include "process.h"
#include "snapshot.h"
#include "diff.h"

int main(void)
{
    mf_error_t err;

    mf_context_t *ctx = NULL;
    mf_process_t *proc = NULL;
    mf_snapshot_t *snap_a = NULL;
    mf_snapshot_t *snap_b = NULL;
    mf_diff_t *diff = NULL;

    /* buffer que vamos modificar de propósito */
    char *buf = NULL;

    /* create context */
    err = mf_context_create(&ctx);
    if (err != MF_OK) {
        printf("context create failed\n");
        return 1;
    }

    /* attach to current process */
    err = mf_process_create(ctx, getpid(), &proc);
    if (err != MF_OK) {
        printf("process create failed\n");
        goto cleanup;
    }

    /* snapshot A */
    err = mf_snapshot_create(proc, &snap_a);
    if (err != MF_OK) {
        printf("snapshot A create failed\n");
        goto cleanup;
    }

    /* === FORÇA MUDANÇA DE MEMÓRIA === */
    buf = malloc(4096);
    if (!buf) {
        printf("malloc failed\n");
        goto cleanup;
    }
    memset(buf, 0x41, 4096); /* escreve memória */

    /* snapshot B */
    err = mf_snapshot_create(proc, &snap_b);
    if (err != MF_OK) {
        printf("snapshot B create failed\n");
        goto cleanup;
    }

    /* diff inteligente */
    err = mf_snapshot_diff(snap_a, snap_b, &diff);
    if (err != MF_OK) {
        printf("snapshot diff failed\n");
        goto cleanup;
    }

    /* resultado */
    printf("Diff OK\n");
    printf("Modified regions: %zu\n",
           mf_diff_modified_region_count(diff));
    printf("Modified bytes:   %zu\n",
           mf_diff_modified_bytes(diff));
    printf("Added regions:    %zu\n",
           mf_diff_added_region_count(diff));
    printf("Removed regions:  %zu\n",
           mf_diff_removed_region_count(diff));

cleanup:
    if (diff)   mf_diff_destroy(diff);
    if (snap_b) mf_snapshot_destroy(snap_b);
    if (snap_a) mf_snapshot_destroy(snap_a);
    if (buf)    free(buf);
    if (proc)   mf_process_destroy(proc);
    if (ctx)    mf_context_destroy(ctx);

    return 0;
}
