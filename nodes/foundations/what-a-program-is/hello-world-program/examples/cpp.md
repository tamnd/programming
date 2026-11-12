```cpp run id=iostream
#include <iostream>
#include <string>

int main(int argc, char *argv[]) {
    (void)argv;
    std::cout << "Hello, world!\n";
    std::string name = "world";
    std::cout << name << " has " << name.size() << " letters\n";
    std::cout << "arguments: " << argc - 1 << '\n';
}
```

```output for=iostream
Hello, world!
world has 5 letters
arguments: 0
```

- Verdict: native. A C++ program starts at `main`, as in C, and reaching the end of `main` returns 0. `std::cout` with `<<` writes values of any type that has an output operator. Newer standards add `std::print` and `std::println`, which format text in the style of Python and Rust.
