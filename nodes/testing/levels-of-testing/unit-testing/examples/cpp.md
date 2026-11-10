```cpp norun reason=fragment id=cart file=cart_test.cpp
#include <gtest/gtest.h>

#include <stdexcept>
#include <string>
#include <vector>

struct Item {
    int price;
    int quantity;
};

int cart_total(const std::vector<Item>& items) {
    int total = 0;
    for (const auto& item : items) {
        if (item.quantity < 0) {
            throw std::invalid_argument("negative quantity: " + std::to_string(item.quantity));
        }
        total += item.price * item.quantity;
    }
    return total;
}

TEST(CartTotal, EmptyCartIsZero) {
    EXPECT_EQ(cart_total({}), 0);
}

TEST(CartTotal, SumsPriceTimesQuantity) {
    EXPECT_EQ(cart_total({{250, 2}, {100, 1}}), 600);
}

TEST(CartTotal, RejectsNegativeQuantity) {
    EXPECT_THROW(cart_total({{250, -1}}), std::invalid_argument);
}
```

Built with `-lgtest -lgtest_main` and run, the program prints:

```text
Running main() from /private/tmp/googletest-20260810-2279-a5yc4x/googletest-1.18.0/googletest/src/gtest_main.cc
[==========] Running 3 tests from 1 test suite.
[----------] Global test environment set-up.
[----------] 3 tests from CartTotal
[ RUN      ] CartTotal.EmptyCartIsZero
[       OK ] CartTotal.EmptyCartIsZero (0 ms)
[ RUN      ] CartTotal.SumsPriceTimesQuantity
[       OK ] CartTotal.SumsPriceTimesQuantity (0 ms)
[ RUN      ] CartTotal.RejectsNegativeQuantity
[       OK ] CartTotal.RejectsNegativeQuantity (0 ms)
[----------] 3 tests from CartTotal (0 ms total)

[----------] Global test environment tear-down
[==========] 3 tests from 1 test suite ran. (0 ms total)
[  PASSED  ] 3 tests.
```

- Verdict: library. The C++ standard library has no test framework. GoogleTest and Catch2 are the usual libraries.
- `TEST(Suite, Name)` defines one test. The library `gtest_main` supplies a `main` function that runs all tests, which is why the file has none.
- `EXPECT_EQ` records a failure and goes on. `ASSERT_EQ` records a failure and returns from the test function at once.
- `EXPECT_THROW` checks that the expression throws an exception of the given type.
- This block does not run on this page, because it needs the GoogleTest headers and libraries, and the runner of this site builds one file with the standard library only.
