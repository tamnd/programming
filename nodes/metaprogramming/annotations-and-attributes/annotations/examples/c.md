```c run id=standard
#include <stdio.h>

[[nodiscard("check the result")]] int save(const char *data) {
    return data[0] != '\0';
}

[[deprecated("use describe")]] const char *old_describe(int level);

static void describe(int level) {
    switch (level) {
        case 2:
            printf("high ");
            [[fallthrough]];
        case 1:
            printf("on\n");
            break;
        default:
            printf("off\n");
    }
}

int main(void) {
    [[maybe_unused]] int debug_level = 0;
    if (save("data")) {
        printf("saved\n");
    }
    describe(2);
    describe(0);
    return 0;
}
```

```output for=standard
saved
high on
off
```

C23 adopted the `[[...]]` syntax and the standard attributes `nodiscard`, `deprecated`, `fallthrough` and `maybe_unused`, with the same meaning as in C++.

```c fails expect=compile flags="-Werror" id=nodiscard
#include <stdio.h>

[[nodiscard("check the result")]] int save(const char *data) {
    return data[0] != '\0';
}

int main(void) {
    save("");
    printf("done\n");
    return 0;
}
```

The standard encourages a diagnostic when a program discards the result of a `nodiscard` call. The example compiles with `-Werror`, so the diagnostic stops the build.

- Verdict: native. C23 has the `[[...]]` attribute syntax, eight standard attributes and prefixed attributes for vendors, such as `gnu::`.
- A standard attribute does not change the meaning of a correct program. An implementation can parse it and ignore it.
- C has no annotations that a program can read at run time.
