```cpp run id=function-cpp
#include <iostream>
#include <string>

int rectangle_area(int width, int height) {
    return width * height;
}

std::string greeting(const std::string& name) {
    return "Hello, " + name + "!";
}

int main() {
    std::cout << rectangle_area(3, 4) << '\n';
    std::cout << greeting("Ada") << '\n';
}
```

```output for=function-cpp
12
Hello, Ada!
```

- Verdict: native. C++ has free functions as in C. `std::string` lets a function return a new string by value.
