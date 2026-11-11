```cpp run id=valarray
#include <iostream>
#include <valarray>

void show(const char* name, const std::valarray<int>& v) {
    std::cout << name << ":";
    for (int x : v) std::cout << " " << x;
    std::cout << "\n";
}

int main() {
    std::valarray<int> a{3, 1, 4, 1, 5};
    std::valarray<int> b{2, 7, 1, 8, 2};

    show("a + b", a + b);                  // element-wise
    show("a * 10", a * 10);                // a scalar meets every element
    std::cout << "dot: " << (a * b).sum() << "\n";
    show("a[a > 2]", a[a > 2]);            // a boolean mask selects elements
    show("every 2nd", a[std::slice(0, 3, 2)]);
}
```

```output for=valarray
a + b: 5 8 5 9 7
a * 10: 30 10 40 10 50
dot: 35
a[a > 2]: 3 4 5
every 2nd: 3 4 5
```

- Verdict: stdlib. `std::valarray` has element-wise operators, operators between an array and a scalar, reductions such as `sum` and `max`, boolean masks and slices, which is a small array language in the standard library. Few programs use it. It has only one axis, so a matrix needs index arithmetic or `std::gslice`. Libraries such as Eigen and xtensor give matrices and n-dimensional arrays, and use expression templates to fuse operations into one loop.
