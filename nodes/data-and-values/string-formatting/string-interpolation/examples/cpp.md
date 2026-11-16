```cpp run id=std-format
#include <format>
#include <iostream>
#include <string>
#include <vector>

int main() {
    std::string name = "Ada";
    double price = 4.5;
    std::vector<std::string> items{"tea", "milk"};

    std::string s = std::format("{} pays {:.2f} for {} items.", name, price * 2, items.size());
    std::cout << s << '\n';
    std::cout << std::format("[{0:>6}] [{0:<6}] [{0}]", name) << '\n';
    std::cout << std::format("{{name}} is text, {{{}}} is not.", name) << '\n';
}
```

```output for=std-format
Ada pays 9.00 for 2 items.
[   Ada] [Ada   ] [Ada]
{name} is text, {Ada} is not.
```

- Verdict: idiom. C++ has no string interpolation. `std::format` takes a format string with braces and the values after it, and the compiler checks the format string against the types of the values. Braces can hold an index and a format, and two braces give one brace.
