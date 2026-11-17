```c run id=regions
#define _POSIX_C_SOURCE 200112L /* for readlink */
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

int counter = 42; /* a global with a value: the data segment */

/* Prints the permissions and the kind of the region that holds addr. */
static void where(const char *name, uintptr_t addr) {
    char exe[4096] = "";
    ssize_t len = readlink("/proc/self/exe", exe, sizeof exe - 1);
    if (len > 0) exe[len] = '\0';

    FILE *maps = fopen("/proc/self/maps", "r");
    char line[8192];
    while (maps && fgets(line, sizeof line, maps)) {
        unsigned long start, end;
        char perms[5], path[4096] = "";
        if (sscanf(line, "%lx-%lx %4s %*s %*s %*s %4095[^\n]", &start, &end, perms, path) < 3) continue;
        if (addr >= start && addr < end) {
            const char *kind = path;
            if (strcmp(path, exe) == 0) kind = "the program file";
            else if (path[0] == '\0' || strncmp(path, "[anon", 5) == 0) kind = "an anonymous mapping";
            printf("%-8s %s %s\n", name, perms, kind);
            break;
        }
    }
    if (maps) fclose(maps);
}

int main(void) {
    int here = 1;                      /* a local: the stack */
    int *small = malloc(sizeof *small); /* a small block: the heap */
    char *big = malloc(1 << 20);       /* 1 MiB, over MMAP_THRESHOLD */

    where("code", (uintptr_t)&where);
    where("global", (uintptr_t)&counter);
    where("local", (uintptr_t)&here);
    where("small", (uintptr_t)small);
    where("big", (uintptr_t)big);

    free(big);
    free(small);
    return 0;
}
```

```output for=regions
code     r-xp the program file
global   rw-p the program file
local    rw-p [stack]
small    rw-p [heap]
big      rw-p an anonymous mapping
```

- Verdict: native. A pointer is an address, so the program can find the region of each thing. The code is in a region of the program file that can be run, and `counter` is in a region of the file that can be written.
- The 1 MiB block is larger than `MMAP_THRESHOLD`, so `malloc` takes it from an anonymous mapping, not from `[heap]`.
