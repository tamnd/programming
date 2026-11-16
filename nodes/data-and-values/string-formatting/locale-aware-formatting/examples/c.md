```c run id=setlocale-localeconv
#include <limits.h>
#include <locale.h>
#include <stdio.h>

static void show(void) {
    struct lconv *lc = localeconv();
    printf("decimal_point='%s' thousands_sep='%s' grouping=", lc->decimal_point, lc->thousands_sep);
    for (const char *g = lc->grouping; *g != 0 && *g != CHAR_MAX; g++) {
        printf("%s%d", g == lc->grouping ? "" : ",", *g);
    }
    printf("\n%'d %'.2f\n", 1234567, 1234567.891);
}

int main(void) {
    printf("%s\n", setlocale(LC_NUMERIC, NULL));
    show();
    if (setlocale(LC_NUMERIC, "en_US.UTF-8") == NULL) {
        printf("the locale en_US.UTF-8 is not installed\n");
        return 1;
    }
    printf("%s\n", setlocale(LC_NUMERIC, NULL));
    show();
    setlocale(LC_NUMERIC, "C");
    printf("%.2f\n", 1234567.891);
    return 0;
}
```

```output for=setlocale-localeconv
C
decimal_point='.' thousands_sep='' grouping=
1234567 1234567.89
en_US.UTF-8
decimal_point='.' thousands_sep=',' grouping=3
1,234,567 1,234,567.89
1234567.89
```

- Verdict: stdlib. `setlocale` sets one global locale for the program, and `localeconv` returns its symbols and group sizes. The group sizes `3` mean groups of 3, and the last size repeats. The program starts in the locale `"C"`, so `printf` has no group separator until the program calls `setlocale`. The flag `'` in `%'d` comes from POSIX, not from ISO C, and `setlocale` fails if the system does not have the locale.
