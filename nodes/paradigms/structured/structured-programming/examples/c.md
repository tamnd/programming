```c run id=cleanup
#include <stdio.h>
#include <stdlib.h>

static int copy_numbers(const char *path, size_t count) {
    int status = -1;
    int *numbers = malloc(count * sizeof *numbers);
    if (numbers == NULL) {
        goto out;
    }
    FILE *file = fopen(path, "w");
    if (file == NULL) {
        goto free_numbers;
    }
    for (size_t i = 0; i < count; i++) {
        numbers[i] = (int)(i * i);
        if (fprintf(file, "%d\n", numbers[i]) < 0) {
            goto close_file;
        }
    }
    status = 0;
close_file:
    fclose(file);
free_numbers:
    free(numbers);
out:
    return status;
}

int main(void) {
    printf("%d\n", copy_numbers("squares.txt", 5));
    printf("%d\n", copy_numbers("no-such-dir/squares.txt", 5));
    return 0;
}
```

```output for=cleanup
0
-1
```

- Verdict: native. C has blocks, loops, `switch`, `break`, `continue` and `return`, and it also has `goto` within one function. `break` leaves only the innermost loop or `switch`.
- The main accepted use of `goto` is the single exit for cleanup. Each label releases one resource, in the reverse order of acquisition. The Linux kernel coding style recommends this pattern.
