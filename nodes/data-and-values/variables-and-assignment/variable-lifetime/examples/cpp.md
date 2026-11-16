```cpp run id=raii-trace
#include <iostream>
#include <string>
#include <utility>

struct Trace {
    std::string name;
    explicit Trace(std::string n) : name(std::move(n)) {
        std::cout << "make " << name << '\n';
    }
    ~Trace() {
        std::cout << "end " << name << '\n';
    }
};

int main() {
    Trace a("a");
    {
        Trace b("b");
    }
    Trace c("c");
}
```

```output for=raii-trace
make a
make b
end b
make c
end c
end a
```

- Verdict: native. An automatic object ends at the end of its block, and its destructor runs at that point. Objects in one block end in the reverse order of their construction, so `c` ends before `a`. This is the base of RAII: a destructor releases the resource that the object holds.
