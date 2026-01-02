#include "process_maps.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAPS_LINE_MAX 512

static uint32_t parse_perms(const char *perms)
{
    uint32_t p = 0;

    if (perms[0] == 'r') p |= 0x1;
    if (perms[1] == 'w') p |= 0x2;
    if (perms[2] == 'x') p |= 0x4;

    return p;
}

mf_error_t mf_linux_enumerate_maps(
    struct mf_process *process,
    struct mf_mem_region **regions,
    size_t *count
)
{
    if (!process || !regions || !count) {
        return MF_ERR_INVALID_ARGUMENT;
    }

    char path[64];
    snprintf(path, sizeof(path), "/proc/%d/maps", process->pid);

    FILE *fp = fopen(path, "r");
    if (!fp) {
        return MF_ERR_PROCESS_ACCESS;
    }

    struct mf_mem_region *list = NULL;
    size_t used = 0;
    size_t capacity = 0;

    char line[MAPS_LINE_MAX];

    while (fgets(line, sizeof(line), fp)) {
        uintptr_t start, end;
        char perms[5];

        if (sscanf(line, "%lx-%lx %4s", &start, &end, perms) != 3) {
            continue;
        }

        if (used == capacity) {
            size_t new_cap = capacity == 0 ? 16 : capacity * 2;
            struct mf_mem_region *tmp =
                realloc(list, new_cap * sizeof(*list));

            if (!tmp) {
                free(list);
                fclose(fp);
                return MF_ERR_NO_MEMORY;
            }

            list = tmp;
            capacity = new_cap;
        }

        list[used].start = start;
        list[used].end = end;
        list[used].permissions = parse_perms(perms);

        used++;
    }

    fclose(fp);

    *regions = list;
    *count = used;

    return MF_OK;
}
