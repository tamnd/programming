```cpp run id=kahan
#include <cstdio>
#include <limits>
#include <numeric>
#include <vector>

double kahan_sum(const std::vector<double>& xs) {
    double sum = 0.0;
    double c = 0.0;  // the low-order part that the last addition lost
    for (double x : xs) {
        double y = x - c;
        double t = sum + y;
        c = (t - sum) - y;
        sum = t;
    }
    return sum;
}

int main() {
    using limits = std::numeric_limits<double>;
    std::printf("digits = %d, epsilon = %g, is_iec559 = %d\n", limits::digits, limits::epsilon(), limits::is_iec559);

    std::vector<double> xs(10'000'000, 0.1);
    double naive = std::accumulate(xs.begin(), xs.end(), 0.0);
    std::printf("naive sum of 10^7 x 0.1 = %.17g\n", naive);
    std::printf("Kahan sum of 10^7 x 0.1 = %.17g\n", kahan_sum(xs));
}
```

```output
digits = 53, epsilon = 2.22045e-16, is_iec559 = 1
naive sum of 10^7 x 0.1 = 999999.99983897537
Kahan sum of 10^7 x 0.1 = 1000000
```

- Verdict: native. `std::numeric_limits<double>::is_iec559` is true when `double` follows IEEE 754.
- Kahan summation keeps the rounding error of each addition in `c` and puts it back into the next one. Its error does not grow with the number of terms the way that the error of a plain loop does.
- The compensation depends on exact IEEE 754 rules. A compiler option such as `-ffast-math` lets the compiler treat `(t - sum) - y` as zero and remove the correction.
