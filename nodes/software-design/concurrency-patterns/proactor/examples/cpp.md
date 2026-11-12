```cpp run id=proactor
#include <condition_variable>
#include <functional>
#include <iostream>
#include <map>
#include <mutex>
#include <queue>
#include <string>
#include <thread>

using Handler = std::function<void(const std::string&, std::size_t)>;

template <typename T>
class Queue {
public:
    void put(T value) {
        { std::lock_guard<std::mutex> lock(m_); q_.push(std::move(value)); }
        cv_.notify_one();
    }
    T take() {
        std::unique_lock<std::mutex> lock(m_);
        cv_.wait(lock, [this] { return !q_.empty(); });
        T value = std::move(q_.front());
        q_.pop();
        return value;
    }
private:
    std::mutex m_;
    std::condition_variable cv_;
    std::queue<T> q_;
};

struct Completion { Handler handler; std::string name; std::size_t size; };

int main() {
    const std::map<std::string, std::string> files{{"a.txt", "hello"}, {"b.txt", "abc"}};
    Queue<std::pair<std::string, Handler>> operations;
    Queue<Completion> completions;
    std::thread processor([&] {
        for (int i = 0; i < 2; ++i) {
            auto [name, handler] = operations.take();
            completions.put({handler, name, files.at(name).size()});
        }
    });
    Handler onRead = [](const std::string& name, std::size_t size) {
        std::cout << name << " done: " << size << " bytes\n";
    };
    for (const std::string name : {"a.txt", "b.txt"}) {
        std::cout << "start read " << name << "\n";
        operations.put({name, onRead});
    }
    for (int i = 0; i < 2; ++i) {
        Completion c = completions.take();  // the Completion Dispatcher
        c.handler(c.name, c.size);
    }
    processor.join();
}
```

```output
start read a.txt
start read b.txt
a.txt done: 5 bytes
b.txt done: 3 bytes
```

- Verdict: idiom. The standard library has threads, a mutex and condition variables but no blocking queue, so the example writes a small one.
