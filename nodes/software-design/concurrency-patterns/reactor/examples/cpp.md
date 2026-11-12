```cpp run id=reactor
#include <functional>
#include <iostream>
#include <map>
#include <string>
#include <utility>
#include <vector>

using Handler = std::function<void(const std::string&, const std::string&)>;
std::map<std::string, Handler> handlers;

void echo(const std::string& handle, const std::string& data) {
    if (!data.empty()) {
        std::cout << handle << ": " << data << "\n";
    } else {
        std::cout << "close " << handle << "\n";
        handlers.erase(handle);
    }
}

int main() {
    handlers["listener"] = [](const std::string&, const std::string& newHandle) {
        std::cout << "accept " << newHandle << "\n";
        handlers[newHandle] = echo;
    };
    // ready stands in for select: handle and data pairs in arrival order
    std::vector<std::pair<std::string, std::string>> ready{
        {"listener", "conn-1"}, {"listener", "conn-2"},
        {"conn-1", "hello"}, {"conn-2", "hi"},
        {"conn-1", ""}, {"conn-2", ""}};
    for (const auto& [handle, data] : ready) {
        Handler handler = handlers.at(handle);  // a copy, because echo can erase its entry
        handler(handle, data);
    }
}
```

```output
accept conn-1
accept conn-2
conn-1: hello
conn-2: hi
close conn-1
close conn-2
```

- Verdict: idiom. The handler table maps handles to `std::function` values. The dispatcher copies the handler before the call, because a handler that removes itself would otherwise destroy the function that is running.
