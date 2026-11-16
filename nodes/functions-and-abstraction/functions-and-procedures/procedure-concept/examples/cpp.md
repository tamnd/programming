```cpp run id=procedure-cpp
#include <iostream>
#include <string>

void greet(const std::string& name) {
    std::cout << "Hello, " << name << "!\n";
}

int main() {
    greet("Ada");
    greet("Alan");
}
```

```output for=procedure-cpp
Hello, Ada!
Hello, Alan!
```

- Verdict: native. A function with the return type `void` is a procedure.
