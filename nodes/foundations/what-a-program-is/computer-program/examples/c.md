```c run id=gcd-in-main
#include <stdio.h>
#include <stdlib.h>

static long gcd(long a, long b) {
    while (b != 0) {
        long r = a % b;
        a = b;
        b = r;
        printf("%ld %ld\n", a, b);
    }
    return a;
}

int main(void) {
    printf("1071 462\n");
    printf("gcd %ld\n", gcd(1071, 462));
    return EXIT_SUCCESS;
}
```

```output for=gcd-in-main
1071 462
462 147
147 21
21 0
gcd 21
```

```c wrong id=order-of-updates
#include <stdio.h>

static long gcd(long a, long b) {
    while (b != 0) {
        a = b;     /* the old value of a is lost here */
        b = a % b; /* so b becomes 0 at once */
        printf("%ld %ld\n", a, b);
    }
    return a;
}

int main(void) {
    printf("1071 462\n");
    printf("gcd %ld\n", gcd(1071, 462));
    return 0;
}
```

```output for=order-of-updates
1071 462
462 0
gcd 462
```

- Verdict: native. A C program is a set of translation units that the linker joins into one executable, and a hosted program starts at `main`. The value that `main` returns becomes the exit status, and `EXIT_SUCCESS` from `<stdlib.h>` names the value for success. C has no multiple assignment, so the loop keeps the remainder in a temporary.
- The wrong block overwrites `a` before it computes the remainder, as the JavaScript file does, and gets 462.
