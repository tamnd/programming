```cpp run id=change
#include <print>
#include <vector>

void set_copy(int n) {
    n = 42;
    std::println("inside set_copy: {}", n);
}

void set_pointer(int* p) { *p = 42; }

void set_reference(int& r) { r = 42; }

int main() {
    int x = 1;
    set_copy(x);
    std::println("after set_copy: {}", x);
    set_pointer(&x);
    std::println("after set_pointer: {}", x);
    x = 1;
    set_reference(x);
    std::println("after set_reference: {}", x);

    int a = 1;
    int b = 2;
    int& r = a;
    r = b;
    std::println("a: {}, &r == &a: {}", a, &r == &a);

    std::vector<int> numbers{1, 2, 3};
    std::vector<int> copy = numbers;
    std::vector<int>& alias = numbers;
    alias[0] = 100;
    std::println("numbers[0]: {}, copy[0]: {}", numbers[0], copy[0]);
}
```

```output for=change
inside set_copy: 42
after set_copy: 1
after set_pointer: 42
after set_reference: 42
a: 2, &r == &a: true
numbers[0]: 100, copy[0]: 1
```

- Verdict: native. C++ has pointers (`T*`), which come from C, and references (`T&`).
- A reference is an alias for an object. It must be initialized, and it cannot be changed to refer to another object. So `r = b` copies the value of `b` into `a`.
- A parameter of type `int&` is true pass by reference. The call `set_reference(x)` looks the same as a call by value.
- Class types have value semantics by default. `copy = numbers` copies all elements, and only `alias` shares the vector.
