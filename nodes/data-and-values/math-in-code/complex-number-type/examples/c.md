```c run id=double-complex
#include <complex.h>
#include <stdio.h>

int main(void) {
    double complex z = CMPLX(3.0, 4.0);
    double complex w = 1.0 - 2.0 * I;
    double complex p = z * w;
    printf("%g %g\n", creal(p), cimag(p));
    printf("%g %g\n", cabs(z), carg(I));

    // csqrt accepts a negative number.
    double complex s = csqrt(-4.0);
    printf("%g %g\n", creal(s), cimag(s));
    printf("%zu\n", sizeof z);
    return 0;
}
```

```output for=double-complex
11 -2
5 1.5708
0 2
16
```

- Verdict: native. `double complex` is a type of the language since C99, and `<complex.h>` gives the macro `I`, the macro `CMPLX` and the functions with the prefix `c`, such as `cabs`, `carg` and `csqrt`. `printf` has no conversion for complex numbers, so the program prints `creal` and `cimag`. A `double complex` takes 16 bytes. C11 made the support optional, so a compiler can define `__STDC_NO_COMPLEX__`.
