```cpp run id=method-cpp
#include <iostream>

struct Rectangle {
    int width;
    int height;

    int area() const {
        return width * height;
    }

    static Rectangle square(int side) {
        return Rectangle{side, side};
    }
};

int main() {
    Rectangle r{3, 4};
    std::cout << r.area() << "\n";
    std::cout << Rectangle::square(5).area() << "\n";
}
```

```output for=method-cpp
12
25
```

- Verdict: native. `area` is a non-static member function, marked `const` because it does not change the object. `square` is a static member function.
