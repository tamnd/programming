```cpp run id=around
#include <iostream>
#include <string>
#include <vector>

template <typename Block>
auto withConnection(const std::string& name, Block block) {
    struct Closer {
        const std::string& name;
        ~Closer() { std::cout << "close " << name << "\n"; }  // runs even if block throws
    } closer{name};
    std::cout << "open " << name << "\n";
    return block(name);  // the caller's work
}

int main() {
    int result = withConnection("db", [](const std::string&) {
        std::vector<std::string> rows = {"a", "b", "c"};
        std::cout << "query: " << rows.size() << " rows\n";
        return static_cast<int>(rows.size());
    });
    std::cout << "result: " << result << "\n";
}
```

```output for=around
open db
query: 3 rows
close db
result: 3
```

- Verdict: idiom. C++ has no `finally`, so a local object's destructor does the cleanup, as in RAII.
