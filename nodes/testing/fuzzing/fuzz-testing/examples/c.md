```c run id=seeds
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/* Counts the records of the form [tag][length][value]; returns -1 for a truncated input. */
static int parse(const uint8_t *data, size_t size, size_t *bad_offset) {
    int count = 0;
    size_t i = 0;
    while (i < size) {
        if (size - i < 2) {
            *bad_offset = i;
            return -1;
        }
        size_t length = data[i + 1];
        if (size - i - 2 < length) {
            *bad_offset = i;
            return -1;
        }
        count++;
        i += 2 + length;
    }
    return count;
}

/* The fuzz target, with the signature that libFuzzer calls. */
int LLVMFuzzerTestOneInput(const uint8_t *data, size_t size) {
    size_t bad = 0;
    int count = parse(data, size, &bad);
    if (count < 0) {
        printf("rejected at offset %zu\n", bad);
    } else {
        printf("records=%d\n", count);
    }
    return 0;
}

/* Our own main runs the seed corpus once; -fsanitize=fuzzer would supply a main that fuzzes. */
int main(void) {
    static const struct { const char *bytes; size_t size; } seeds[] = {
        {"", 0}, {"\x01\x02hi", 4}, {"\x01\x02hi\x02\x00", 6}, {"z", 1}, {"~\xe1", 2},
    };
    size_t n = sizeof seeds / sizeof seeds[0];
    for (size_t k = 0; k < n; k++) {
        /* A heap copy of the exact size lets AddressSanitizer see a read past the end. */
        uint8_t *copy = malloc(seeds[k].size > 0 ? seeds[k].size : 1);
        if (copy == NULL) {
            return 1;
        }
        memcpy(copy, seeds[k].bytes, seeds[k].size);
        printf("seed %zu: ", k);
        LLVMFuzzerTestOneInput(copy, seeds[k].size);
        free(copy);
    }
    printf("seed corpus: %zu inputs passed\n", n);
    return 0;
}
```

```output
seed 0: records=0
seed 1: records=1
seed 2: records=2
seed 3: rejected at offset 0
seed 4: rejected at offset 0
seed corpus: 5 inputs passed
```

The original parser reads the length byte with no check. For the input `z`, it reads one byte past the end of the heap copy. With AddressSanitizer, the run stops with a `heap-buffer-overflow` report.

```c fails expect=sanitizer:address timeout=30 id=no-length-check
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/* Counts the records of the form [tag][length][value]. */
static int parse(const uint8_t *data, size_t size) {
    int count = 0;
    size_t i = 0;
    while (i < size) {
        size_t length = data[i + 1]; /* no check that the length byte exists */
        count++;
        i += 2 + length;
    }
    return count;
}

int LLVMFuzzerTestOneInput(const uint8_t *data, size_t size) {
    printf("records=%d\n", parse(data, size));
    return 0;
}

int main(void) {
    static const struct { const char *bytes; size_t size; } seeds[] = {
        {"", 0}, {"\x01\x02hi", 4}, {"\x01\x02hi\x02\x00", 6}, {"z", 1}, {"~\xe1", 2},
    };
    for (size_t k = 0; k < sizeof seeds / sizeof seeds[0]; k++) {
        uint8_t *copy = malloc(seeds[k].size > 0 ? seeds[k].size : 1);
        if (copy == NULL) {
            return 1;
        }
        memcpy(copy, seeds[k].bytes, seeds[k].size);
        LLVMFuzzerTestOneInput(copy, seeds[k].size);
        free(copy);
    }
    return 0;
}
```

- Verdict: library. libFuzzer comes with Clang. `clang -fsanitize=fuzzer,address target.c` links a `main` that fuzzes `LLVMFuzzerTestOneInput`. The same target also builds for AFL.
- The example has its own `main`, which calls the target once for each seed and does no fuzzing. libFuzzer does the same when it gets a list of files instead of a directory.
- Without a sanitizer, the read past the end usually returns some byte, and the program goes on. The seed corpus passes and the bug stays. Build fuzz targets and their regression runs with AddressSanitizer.
- `size - i < 2` is the overflow-safe form of `i + 2 > size`: `i` is never more than `size`, so the subtraction cannot wrap.
