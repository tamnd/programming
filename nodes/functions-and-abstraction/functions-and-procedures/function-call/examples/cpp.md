```cpp run id=function-call-cpp
#include <iostream>

int rectangle_area(int width, int height) {
    std::cout << "computing " << width << " by " << height << "\n";
    return width * height;
}

int main() {
    int area = rectangle_area(3, 4);
    std::cout << "area " << area << "\n";
}
```

```output for=function-call-cpp
computing 3 by 4
area 12
```

- Verdict: native. A call is an expression with the function, then the arguments in parentheses, as in C.
