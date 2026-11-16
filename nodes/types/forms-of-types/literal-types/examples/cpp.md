```cpp run id=integral-constant
#include <iostream>
#include <type_traits>

using Five = std::integral_constant<int, 5>;

template <int N>
int times(Five) {
    return N * Five::value; // Five has one value, known at compile time
}

enum class Align { Left, Right, Center };

int main() {
    std::cout << Five::value << " " << Five{}() << " " << times<3>(Five{}) << "\n";
    std::cout << std::is_same_v<Five, std::integral_constant<int, 6>> << " "
              << std::is_same_v<std::true_type, std::integral_constant<bool, true>> << "\n";
    std::cout << sizeof(Five) << "\n";

    Align a = Align::Center;
    // Align b = 2; // error: no conversion from int to Align
    Align c = static_cast<Align>(7); // an explicit cast still works
    std::cout << static_cast<int>(a) << " " << static_cast<int>(c) << "\n";
}
```

```output for=integral-constant
5 5 15
0 1
1
2 7
```

- Verdict: stdlib. `std::integral_constant<int, 5>` is a type with one value, 5, and `std::true_type` is the same template with `bool` and `true`. Each such type is an empty class. An `enum class` does not convert from `int` without a cast, but `static_cast` can still make a value with no name.
