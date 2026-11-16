```c run id=money
#include <stdio.h>

int main(void) {
    double total = 0.0;
    float total_f = 0.0f;
    for (int i = 0; i < 1000; i++) {
        total += 0.1;
        total_f += 0.1f;
    }
    printf("0.1 exact: %.55f\n", 0.1);
    printf("double, 1000 additions: %.17g\n", total);
    printf("float, 1000 additions: %.9g\n", (double) total_f);

    double sum = 0.0, c = 0.0;
    for (int i = 0; i < 1000; i++) {
        double y = 0.1 - c;
        double t = sum + y;
        c = (t - sum) - y;
        sum = t;
    }
    printf("Kahan, 1000 additions: %.17g\n", sum);

    double big = 1e16;
    printf("1e16 + 1 - 1e16: %g\n", (big + 1.0) - big);
    return 0;
}
```

```output for=money
0.1 exact: 0.1000000000000000055511151231257827021181583404541015625
double, 1000 additions: 99.999999999998593
float, 1000 additions: 99.9990463
Kahan, 1000 additions: 100
1e16 + 1 - 1e16: 0
```

- Verdict: native. C `double` is binary64 and `float` is binary32 on common platforms.
- After 1000 additions the error of the `float` sum is much larger than the error of the `double` sum. Kahan summation gives 100.
