```cpp run id=forms
#include <iostream>
#include <string>
#include <vector>

int main() {
    int a{};
    int b = 7;
    std::string s;
    std::vector<int> v1(3);
    std::vector<int> v2{3};
    std::cout << a << ' ' << b << " [" << s << "] " << s.size() << '\n';
    std::cout << v1.size() << ' ' << v2.size() << ' ' << v2[0] << '\n';
}
```

```output for=forms
0 7 [] 0
3 1 3
```

- Verdict: native. `int a{};` gives 0, but `int a;` in a function leaves the value indeterminate. A class type such as `std::string` runs its default constructor, which makes an empty string. The two forms for a vector differ: `v1(3)` makes 3 elements of value 0, and `v2{3}` makes 1 element of value 3.
