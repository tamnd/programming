```cpp run id=references-and-structured-bindings
#include <iostream>
#include <map>
#include <string>

int main() {
    // A reference is bound once, when it is made. r = b assigns to a; it does not rebind r.
    int a = 1, b = 2;
    int& r = a;
    r = b;
    std::cout << a << " " << b << "\n";

    // A structured binding gives names to the parts of an object.
    std::map<std::string, int> ages{{"ann", 31}, {"bob", 27}};
    const char* sep = "";
    for (const auto& [name, age] : ages) {
        std::cout << sep << name << "=" << age;
        sep = " ";
    }
    std::cout << "\n";

    // A lambda captures by value with = and by reference with &.
    int k = 1;
    auto byValue = [=] { return k; };
    auto byRef = [&] { return k; };
    k = 5;
    std::cout << byValue() << " " << byRef() << "\n";

    // A const reference bound to a temporary keeps the temporary alive.
    const std::string& s = std::string("tem") + "porary";
    std::cout << s << "\n";
}
```

```output for=references-and-structured-bindings
2 2
ann=31 bob=27
1 5
temporary
```

- Verdict: native. A reference is bound to one object when it is made, so `r = b` assigns to `a`. A structured binding names the parts of an object. A lambda captures by value with `=` and by reference with `&`, so the two lambdas return `1` and `5`. A `const` reference bound to a temporary keeps the temporary alive.
