```cpp run id=fields
#include <bit>
#include <cstdint>
#include <cstdio>
#include <initializer_list>
#include <limits>

int main() {
    for (double x : {1.0, -2.5, 0.1}) {
        const auto b32 = std::bit_cast<std::uint32_t>(static_cast<float>(x));
        const auto b64 = std::bit_cast<std::uint64_t>(x);
        std::printf("%4.1f binary32 %08x binary64 %016llx\n", x, static_cast<unsigned>(b32),
                    static_cast<unsigned long long>(b64));
    }

    const auto yes = [](bool b) { return b ? "yes" : "no"; };
    std::printf("is_iec559: float %s, double %s\n", yes(std::numeric_limits<float>::is_iec559),
                yes(std::numeric_limits<double>::is_iec559));
    std::printf("digits: float %d, double %d\n", std::numeric_limits<float>::digits,
                std::numeric_limits<double>::digits);
    std::printf("max_exponent: float %d, double %d\n", std::numeric_limits<float>::max_exponent,
                std::numeric_limits<double>::max_exponent);
    std::printf("max_digits10: float %d, double %d\n", std::numeric_limits<float>::max_digits10,
                std::numeric_limits<double>::max_digits10);
}
```

```output for=fields
 1.0 binary32 3f800000 binary64 3ff0000000000000
-2.5 binary32 c0200000 binary64 c004000000000000
 0.1 binary32 3dcccccd binary64 3fb999999999999a
is_iec559: float yes, double yes
digits: float 24, double 53
max_exponent: float 128, double 1024
max_digits10: float 9, double 17
```

- Verdict: native. With GCC on x86-64, `float` and `double` are binary32 and binary64, and `is_iec559` says so.
- `std::bit_cast` gives the bits of a float as an unsigned integer of the same size. It is the safe form of a `reinterpret_cast` between pointers.
- `max_digits10` is the number of decimal digits that always gives back the same value: 9 for binary32 and 17 for binary64, the numbers of the standard.
- The header `<stdfloat>` names the formats directly as `std::float16_t`, `std::float32_t` and `std::float64_t`, if the implementation has them.
