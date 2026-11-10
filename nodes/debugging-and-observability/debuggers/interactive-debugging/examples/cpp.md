```cpp run id=source-location
#include <print>
#include <source_location>
#include <string_view>

// A default argument of type std::source_location is evaluated at the call
// site, so trace() reports the line and the function of its caller.
void trace(std::string_view note, std::source_location here = std::source_location::current()) {
    std::println("  line {} in {}: {}", here.line(), here.function_name(), note);
}

int price(std::string_view item) {
    if (item == "bread") {
        trace("found bread");
        return 250;
    }
    return 30;
}

int main() {
    int sum = price("apple") + price("bread");
    trace("done");
    std::println("total: {}", sum);
}
```

```output for=source-location
  line 13 in int price(std::string_view): found bread
  line 21 in int main(): done
total: 280
```

- Verdict: library. As in C, the program runs under gdb or lldb. The standard library gives `std::source_location` to record a place in the source.
- The text of `function_name()` depends on the compiler. The example prints the signature, but another compiler may print only the name.
- `std::source_location::current()` in a default argument gives the place of the call, not the place of the declaration.
