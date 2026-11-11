```cpp run id=modes
#include <iostream>
#include <string>
#include <vector>

void by_value(std::vector<int> v) { v.push_back(0); }
void by_reference(std::vector<int>& v) { v.push_back(4); }
std::size_t by_const_reference(const std::vector<int>& v) { return v.size(); }
std::string greet(const std::string& name, const std::string& greeting = "hello") {
    return greeting + ", " + name;
}

int main() {
    std::vector<int> numbers{1, 2, 3};
    by_value(numbers);
    by_reference(numbers);
    std::cout << by_const_reference(numbers) << "\n";
    std::cout << greet("ann") << "\n";
}
```

```output for=modes
4
hello, ann
```

- Verdict: native. C++ passes by value unless the parameter is a reference. `T&` lets the function change the variable of the caller, and `const T&` avoids a copy of a large object without a change.
- An rvalue reference `T&&` and `std::move` let a function take the resources of an argument that the caller no longer needs.
- C++ has default arguments, function overloading and variadic templates. It has no named arguments. Designated initializers of a struct are the usual replacement.

```cpp wrong id=copy
#include <iostream>
#include <vector>

void add(std::vector<int> v, int x) {
    v.push_back(x);
}

int main() {
    std::vector<int> numbers{1, 2, 3};
    add(numbers, 4);
    std::cout << numbers.size() << "\n";
}
```

```output for=copy
3
```

The parameter is a `std::vector<int>`, not a reference, so the function adds to a copy. Each call also copies all the elements. Declare the parameter as `std::vector<int>& v`.
