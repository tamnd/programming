```c run id=c-stream
#define _POSIX_C_SOURCE 200809L
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

static int stream(const char *path, long *count, long long *total, long *largest) {
    FILE *f = fopen(path, "r");
    if (f == NULL) {
        return -1;
    }
    char *line = NULL; // getline makes the buffer larger when a line needs it
    size_t cap = 0;
    *count = 0, *total = 0, *largest = 0;
    while (getline(&line, &cap, f) != -1) {
        long n = strtol(line, NULL, 10);
        if (*count == 0 || n > *largest) {
            *largest = n;
        }
        ++*count;
        *total += n;
    }
    int failed = ferror(f);
    free(line);
    fclose(f);
    return failed ? -1 : 0;
}

int main(void) {
    char dir[] = "/tmp/streamXXXXXX";
    if (mkdtemp(dir) == NULL) {
        return 1;
    }
    char path[64];
    snprintf(path, sizeof path, "%s/numbers.txt", dir);
    FILE *f = fopen(path, "w");
    if (f == NULL) {
        return 1;
    }
    for (int i = 0; i < 1000000; i++) {
        fprintf(f, "%d\n", i % 1000);
    }
    if (fclose(f) != 0) {
        return 1;
    }

    long count, largest;
    long long total;
    int rc = stream(path, &count, &total, &largest);
    printf("lines: %ld sum: %lld max: %ld rc: %d\n", count, total, largest, rc);
    remove(path);
    rmdir(dir);
    return rc != 0;
}
```

```output
lines: 1000000 sum: 499500000 max: 999 rc: 0
```

`getline` reads one line into a buffer that it makes larger when a line needs it, and the program frees the buffer once, after the loop. `ferror` after the loop shows whether the loop stopped on an error and not at the end of the file.
