```cpp run id=counter-module
#include <iostream>

namespace counter {
namespace {  // unnamed: these names are private to this file
int private_counter = 0;
void change_by(int val) { private_counter += val; }
}  // namespace

void increment() { change_by(1); }
void decrement() { change_by(-1); }
int value() { return private_counter; }
}  // namespace counter

int main() {
    std::cout << "value: " << counter::value() << "\n";
    counter::increment();
    counter::increment();
    std::cout << "value: " << counter::value() << "\n";
    counter::decrement();
    std::cout << "value: " << counter::value() << "\n";
}
```

```output
value: 0
value: 2
value: 1
```

- Verdict: idiom. A named namespace gives the module its name, and an unnamed namespace hides the helpers from other translation units. C++20 modules add `export` for the same purpose.
