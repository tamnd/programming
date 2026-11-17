```cpp run id=trace
#include <iostream>
#include <string>
#include <vector>

// The program keeps its own copy of the stack. A Frame pushes a name when it is
// made and pops it when it is destroyed, so every return pops it.
std::vector<std::string> names;

struct Frame {
    explicit Frame(std::string name) { names.push_back(std::move(name)); }
    ~Frame() { names.pop_back(); }
};

std::string where() {
    std::string out;
    for (auto it = names.rbegin(); it != names.rend(); ++it) {
        out += (out.empty() ? "" : " <- ") + *it;
    }
    return out;
}

void c() {
    Frame f("c");
    std::cout << "      stack: " << where() << '\n';
}

void b() {
    Frame f("b");
    std::cout << "    call c\n";
    c();
    std::cout << "    back in b\n";
}

void a() {
    Frame f("a");
    std::cout << "  call b\n";
    b();
    std::cout << "  back in a\n";
}

int main() {
    Frame f("main");
    std::cout << "call a\n";
    a();
    std::cout << "back in main\n";
}
```

```output for=trace
call a
  call b
    call c
      stack: c <- b <- a <- main
    back in b
  back in a
back in main
```

- Verdict: native. The standard library has `std::stacktrace` in the header `<stacktrace>`. This example uses a destructor instead: the destructor of `Frame` runs on every return, so the copy of the stack stays correct.
