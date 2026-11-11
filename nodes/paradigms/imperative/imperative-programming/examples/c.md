```c run id=pointers
#include <stdio.h>
#include <string.h>

static void reverse_in_place(int *xs, size_t n)
{
    if (n == 0)
        return;
    for (size_t i = 0, j = n - 1; i < j; i++, j--) {
        int t = xs[i];
        xs[i] = xs[j];
        xs[j] = t;
    }
}

static void add_to(int *total, int amount)
{
    *total += amount;
}

int main(void)
{
    int data[] = { 1, 2, 3, 4, 5 };
    reverse_in_place(data, 5);
    for (int i = 0; i < 5; i++)
        printf("%d ", data[i]);
    printf("\n");

    int total = 0;
    add_to(&total, 5);
    add_to(&total, 7);
    printf("%d\n", total);

    int count = 0;
    for (const char *p = "imperative"; *p; p++)
        if (strchr("aeiou", *p))
            count++;
    printf("%d\n", count);
    return 0;
}
```

```output for=pointers
5 4 3 2 1 
12
5
```

- Verdict: native. C is close to the machine: a variable is a named piece of memory, and a pointer is its address. cppreference describes statements as "fragments of the C program that are executed in sequence".
- The `n == 0` check matters. With `size_t`, `n - 1` for an empty array wraps to the largest value.
