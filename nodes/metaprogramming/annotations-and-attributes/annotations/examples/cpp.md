```cpp run id=standard
#include <iostream>
#include <string>

[[nodiscard]] bool save(const std::string& data) {
    return !data.empty();
}

[[deprecated("use describe")]] std::string old_describe(int level) {
    return std::to_string(level);
}

std::string describe(int level) {
    std::string out;
    switch (level) {
        case 2:
            out += "high ";
            [[fallthrough]];
        case 1:
            out += "on";
            break;
        default:
            out = "off";
    }
    return out;
}

int main() {
    [[maybe_unused]] int unused_in_release = 42;
    if (save("data")) {
        std::cout << "saved\n";
    }
    std::cout << describe(2) << " | " << describe(1) << " | " << describe(0) << "\n";
}
```

```output for=standard
saved
high on | on | off
```

`[[nodiscard]]` asks for a warning if a caller drops the result. `[[fallthrough]]` says that the missing `break` is deliberate. `[[maybe_unused]]` silences the warning for a variable that a build does not use. `[[deprecated]]` causes a warning at each use, and this program has none.

```cpp fails expect=compile flags="-Werror" id=nodiscard
#include <iostream>
#include <string>

[[nodiscard("the result says if the data was saved")]] bool save(const std::string& data) {
    return !data.empty();
}

int main() {
    save("");
    std::cout << "done\n";
}
```

The standard asks only for a warning when a program drops the result of a `[[nodiscard]]` call. This example compiles with `-Werror`, so the warning stops the build.

```cpp wrong id=typo
#include <iostream>
#include <string>

[[nodiscrd]] bool save(const std::string& data) {
    return !data.empty();
}

int main() {
    save("");
    std::cout << "done\n";
}
```

```output for=typo
done
```

`nodiscrd` is not a standard attribute. An implementation ignores an attribute that it does not recognize, so the program compiles, the failed save goes unnoticed, and the program prints `done`. Compilers can warn about unknown attributes, but a warning does not stop the build.

- Verdict: native. Attributes are written `[[...]]`. The standard attributes give hints and requests for diagnostics. Vendors add their own in a namespace, such as `gnu::` or `clang::`.
- The standard attributes have optional semantics. If a correct program drops one of them, the set of possible executions is a subset of the executions of the original.
- C++ has no annotation that a program can read at run time.
