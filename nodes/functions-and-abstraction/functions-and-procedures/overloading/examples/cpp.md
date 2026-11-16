```cpp run id=overloading-cpp
#include <iostream>
#include <string>

std::string show(int x) {
    return "int " + std::to_string(x);
}

std::string show(const std::string& s) {
    return "string " + s;
}

std::string show(int x, int y) {
    return "two ints " + std::to_string(x) + " " + std::to_string(y);
}

int main() {
    std::cout << show(42) << "\n";
    std::cout << show("hi") << "\n";
    std::cout << show(3, 4) << "\n";
}
```

```output for=overloading-cpp
int 42
string hi
two ints 3 4
```

- Verdict: native. The three free functions have the same name. Overload resolution chooses one at each call.
