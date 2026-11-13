```c run id=qsort-copy-and-strtok
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

static int by_value(const void *a, const void *b) {
    int x = *(const int *)a, y = *(const int *)b;
    return (x > y) - (x < y);
}

static void show(const char *label, const int *a, size_t n) {
    printf("%s", label);
    for (size_t i = 0; i < n; i++) printf(" %d", a[i]);
    printf("\n");
}

int main(void) {
    /* qsort sorts the array in place and returns nothing. */
    int nums[] = {3, 1, 2};
    qsort(nums, 3, sizeof nums[0], by_value);
    show("sorted:", nums, 3);

    /* To keep the original, copy it first, then sort the copy. */
    int src[] = {3, 1, 2};
    int copy[3];
    memcpy(copy, src, sizeof src);
    qsort(copy, 3, sizeof copy[0], by_value);
    show("src:", src, 3);
    show("copy:", copy, 3);

    /* strtok also works in place: it writes a null character into the text. */
    char text[] = "a,b,c";
    char *first = strtok(text, ",");
    printf("%s %s %zu\n", first, text, strlen(text));
    return 0;
}
```

```output
sorted: 1 2 3
src: 3 1 2
copy: 1 2 3
a a 1
```

- Verdict: native. `qsort` sorts the array in place and returns nothing, so the program copies the array with `memcpy` to keep the original. `strtok` also works in place: it writes a null character after the first token, so `text` reads as `a` and its length is 1.
