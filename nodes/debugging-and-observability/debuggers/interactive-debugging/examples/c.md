```c run id=trace
#include <stdio.h>
#include <string.h>

// C has no way to walk the stack in its standard library. A macro can record
// where it was used: the compiler fills in __func__ and __LINE__ at each use.
#define TRACE(fmt, ...) fprintf(stdout, "  %s line %d: " fmt "\n", __func__, __LINE__, __VA_ARGS__)

static int price(const char *item) {
    if (strcmp(item, "bread") == 0) {
        TRACE("item=%s", item);
        return 250;
    }
    return 30;
}

static int total(const char *cart[], int n) {
    int sum = 0;
    for (int i = 0; i < n; i++) {
        sum += price(cart[i]);
        TRACE("i=%d sum=%d", i, sum);
    }
    return sum;
}

int main(void) {
    const char *cart[] = {"apple", "bread"};
    printf("total: %d\n", total(cart, 2));
}
```

```output for=trace
  total line 20: i=0 sum=30
  price line 10: item=bread
  total line 20: i=1 sum=280
total: 280
```

- Verdict: library. The C standard library has no debugger and no way to walk the stack. A native debugger such as gdb or lldb runs the program, and the compiler writes the debug information with `-g`.
- `__func__` is the name of the enclosing function. `__LINE__` is the current line of the source. Both are standard.
- The trace of `price` comes before the second trace of `total`, because `price` runs inside the expression on line 19.
