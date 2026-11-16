```cpp run id=ties
#include <cmath>
#include <iomanip>
#include <iostream>
#include <sstream>

int main() {
    std::cout << "    x round  rint floor  ceil trunc\n";
    for (double x : {0.5, 1.5, 2.5, -0.5, -1.5, -2.5}) {
        std::cout << std::setw(5) << x;
        for (double r : {std::round(x), std::rint(x), std::floor(x), std::ceil(x), std::trunc(x)}) {
            std::cout << std::setw(6) << r + 0.0;
        }
        std::cout << "\n";
    }

    std::ostringstream out;
    out << std::fixed << std::setprecision(0) << 0.5 << " " << 1.5 << " " << 2.5;
    out << std::setprecision(2) << " " << 0.125 << " " << 0.375 << " " << 2.675;
    std::cout << "printed: " << out.str() << "\n";
    std::cout << "lround(-2.5): " << std::lround(-2.5) << "\n";
}
```

```output for=ties
    x round  rint floor  ceil trunc
  0.5     1     0     0     1     0
  1.5     2     2     1     2     1
  2.5     3     2     2     3     2
 -0.5    -1     0    -1     0     0
 -1.5    -2    -2    -2    -1    -1
 -2.5    -3    -2    -3    -2    -2
printed: 0 2 2 0.12 0.38 2.67
lround(-2.5): -3
```

- Verdict: native. `std::round` rounds half away from zero, and `std::rint` uses the current IEEE mode, which is to nearest, ties to even, by default.
- The table adds `0.0` to each result so that negative zero prints as 0.
- Printing with `std::fixed` also rounds. In this run the ties 0.5, 2.5, 0.125 and 0.375 go to the even digit, and 2.675 prints as 2.67 because the double is below 2.675.
