```c run id=pointer-arithmetic
#include <stdbool.h>
#include <stddef.h>
#include <stdio.h>

// C does not check indices. A checked access is the program's own code.
static bool get(const int *a, size_t n, size_t i, int *out) {
    if (i >= n) return false;
    *out = a[i];
    return true;
}

int main(void) {
    int a[] = {10, 20, 30, 40, 50};
    size_t n = sizeof a / sizeof a[0];

    // a[i] is defined as *(a + i), so i[a] is the same item.
    printf("%d %d %d %d\n", a[2], *(a + 2), 2[a], a[n - 1]);

    // Item i is i * sizeof(int) bytes after the start of the array.
    printf("%td %td %zu\n", &a[3] - &a[0], (char *)&a[3] - (char *)&a[0], sizeof a[0]);

    int v = 0;
    bool ok = get(a, n, 4, &v);
    printf("%d %d ", ok, v);
    ok = get(a, n, 5, &v);
    printf("%d\n", ok);

    // A pointer one past the last item is valid for comparison, but not for reading.
    int sum = 0;
    for (const int *p = a; p != a + n; p++) sum += *p;
    printf("%d\n", sum);
    return 0;
}
```

```output
30 30 30 50
3 12 4
1 50 0
150
```

- Verdict: native. a[i] is *(a + i), so 2[a] is the same item. Item 3 is 3 items and 12 bytes after item 0 when an int has 4 bytes. C does not check indices, so the program writes its own get with a bounds test. A pointer one past the end ends the loop.
