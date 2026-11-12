```c run id=kinds-of-behavior
#include <limits.h>
#include <stdio.h>

int main(void) {
    /* Defined by the standard: division truncates toward zero. */
    printf("defined:                -7 / 2 = %d, -7 %% 2 = %d\n", -7 / 2, -7 % 2);
    /* Defined: unsigned arithmetic wraps modulo 2 to the width. */
    printf("defined:                UINT_MAX + 1u = %u\n", UINT_MAX + 1u);
    /* Implementation-defined: each compiler documents its choice. */
    printf("implementation-defined: sizeof(int) = %zu, CHAR_MIN = %d\n", sizeof(int), CHAR_MIN);
    printf("implementation-defined: -8 >> 1 = %d\n", -8 >> 1);
    /* Undefined: INT_MAX + 1 has no meaning, so test before the addition. */
    int a = INT_MAX, b = 1;
    printf("undefined, so tested:   INT_MAX + 1 overflows: %s\n", b > 0 && a > INT_MAX - b ? "yes" : "no");
    return 0;
}
```

```output for=kinds-of-behavior
defined:                -7 / 2 = -3, -7 % 2 = -1
defined:                UINT_MAX + 1u = 0
implementation-defined: sizeof(int) = 4, CHAR_MIN = -128
implementation-defined: -8 >> 1 = -4
undefined, so tested:   INT_MAX + 1 overflows: yes
```

- Verdict: absent. The C standard is written in English and defines programs by an abstract machine. It sorts behavior into four kinds: defined; implementation-defined, where each compiler documents its choice; unspecified, where the compiler can choose without saying; and undefined, where the standard puts no limit on what happens. The output shows defined and implementation-defined results, and tests for signed overflow before the addition, because the addition itself would be undefined. CompCert and the K framework give C formal semantics.
