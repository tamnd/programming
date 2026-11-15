```cpp run id=keep
#include <iostream>
#include <string>

template <class P>
concept HasXY = requires(P p) {
    p.x += 1;
    p.y;
};

template <HasXY P>
P shift(P p) {
    p.x += 1;
    return p;
}

struct Point {
    int x;
    int y;
};

struct Labelled {
    int x;
    int y;
    std::string label;
};

int main() {
    Point a = shift(Point{1, 2});
    Labelled b = shift(Labelled{1, 2, "home"});
    std::cout << "point: x=" << a.x << " y=" << a.y << '\n';
    std::cout << "labelled: x=" << b.x << " y=" << b.y << " label=" << b.label << '\n';
}
```

```output for=keep
point: x=2 y=2
labelled: x=2 y=2 label=home
```

- Verdict: idiom. A template accepts any type, and the concept `HasXY` requires only the members `x` and `y`. The compiler makes one copy of `shift` for each type, so the result keeps the full type.
- `Point` and `Labelled` do not name the concept. The compiler checks the members when it uses the template, which is close to a row type checked at compile time.
