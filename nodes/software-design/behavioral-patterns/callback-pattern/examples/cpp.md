```cpp run id=callbacks
#include <cctype>
#include <deque>
#include <functional>
#include <iostream>
#include <string>
#include <vector>

void forEach(const std::vector<std::string>& items, const std::function<void(const std::string&)>& callback) {
    for (const auto& item : items) callback(item);  // a blocking callback
}

int main() {
    forEach({"hello", "world"}, [](const std::string& w) {
        std::string up = w;
        for (char& c : up) c = static_cast<char>(std::toupper(static_cast<unsigned char>(c)));
        std::cout << up << "\n";
    });

    std::deque<std::function<void()>> pending;
    pending.push_back([] { std::cout << "saved\n"; });  // a deferred callback
    std::cout << "request sent\n";
    while (!pending.empty()) {
        pending.front()();
        pending.pop_front();
    }
}
```

```output for=callbacks
HELLO
WORLD
request sent
saved
```

- Verdict: stdlib. `std::function` holds any callable object, such as a lambda.
