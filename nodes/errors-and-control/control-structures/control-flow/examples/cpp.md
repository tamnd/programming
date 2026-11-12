```cpp run id=unwinding-and-fallthrough
#include <print>
#include <stdexcept>
#include <string>

// A destructor runs when control leaves the scope, by any path.
struct Guard {
    std::string name;
    ~Guard() { std::println("leave {}", name); }
};

void step(int n) {
    Guard g{"step " + std::to_string(n)};
    if (n == 2) throw std::runtime_error("failed at 2");
    std::println("step {} done", n);
}

int main() {
    // An exception unwinds the stack and runs the destructors on the way.
    try {
        for (int n = 1; n <= 3; n++) step(n);
    } catch (const std::exception& e) {
        std::println("caught: {}", e.what());
    }

    // [[fallthrough]] states that a case continues into the next one on purpose.
    for (int n : {1, 2}) {
        switch (n) {
        case 1:
            std::print("one ");
            [[fallthrough]];
        case 2:
            std::println("one-or-two");
            break;
        }
    }

    // if with an initializer limits the variable to the statement.
    if (int r = 17 % 5; r != 0) std::println("remainder {}", r);
}
```

```output for=unwinding-and-fallthrough
step 1 done
leave step 1
leave step 2
caught: failed at 2
one one-or-two
one-or-two
remainder 2
```

- Verdict: native. An exception at step 2 unwinds the stack, and the destructor of each `Guard` runs as control leaves its scope. `[[fallthrough]]` states that a case continues into the next one on purpose. An `if` with an initializer limits the variable to the statement.
