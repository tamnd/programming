```cpp run id=loop
#include <functional>
#include <iostream>
#include <map>
#include <queue>
#include <string>
#include <utility>
#include <vector>

class Loop {
public:
    using Handler = std::function<void(const std::string&)>;

    void on(const std::string& name, Handler h) { handlers_[name].push_back(std::move(h)); }
    void post(std::string name, std::string data) { queue_.emplace(std::move(name), std::move(data)); }

    void run() {
        while (!queue_.empty()) {
            auto [name, data] = queue_.front();
            queue_.pop();
            for (auto& h : handlers_[name]) h(data);
        }
    }

private:
    std::map<std::string, std::vector<Handler>> handlers_;
    std::queue<std::pair<std::string, std::string>> queue_;
};

int main() {
    Loop loop;
    loop.on("request", [&](const std::string& path) {
        std::cout << "request " << path << "\n";
        loop.post("response", path == "/" ? "200" : "404");
    });
    loop.on("response", [](const std::string& code) { std::cout << "response " << code << "\n"; });

    loop.post("request", "/");
    loop.post("request", "/missing");
    loop.run();
}
```

```output for=loop
request /
request /missing
response 200
response 404
```

- Verdict: idiom. The standard library has no event loop. A queue of events and a map from names to `std::function` handlers is the basic form. Boost.Asio and the user interface toolkits Qt and wxWidgets give complete event loops, and Qt connects events to handlers with signals and slots.
- The responses come after both requests, because a handler only posts a new event. It does not call the next handler directly.
