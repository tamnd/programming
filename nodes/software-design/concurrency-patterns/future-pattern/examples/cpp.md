```cpp run id=future-pattern
#include <chrono>
#include <condition_variable>
#include <future>
#include <iostream>
#include <map>
#include <mutex>
#include <string>
#include <vector>

// The lookups wait here with the caller, so the output does not depend on timing.
class Meeting {
public:
    explicit Meeting(int parties) : left_(parties) {}
    bool arrive_and_wait() {
        std::unique_lock<std::mutex> lock(m_);
        if (--left_ == 0) cv_.notify_all();
        return cv_.wait_for(lock, std::chrono::seconds(5), [&] { return left_ <= 0; });
    }

private:
    std::mutex m_;
    std::condition_variable cv_;
    int left_;
};

int main() {
    std::vector<std::string> items = {"apple", "bread", "cheese"};
    std::map<std::string, int> prices = {{"apple", 12}, {"bread", 7}, {"cheese", 30}};
    Meeting meeting(4);

    // Each call returns a future at once. The lookup runs on its own thread.
    std::vector<std::future<int>> futures;
    for (const auto& item : items) {
        futures.push_back(std::async(std::launch::async, [&, item] {
            meeting.arrive_and_wait();  // returns when all 3 lookups and the caller are here
            return prices.at(item);
        }));
    }
    std::cout << "lookups started: " << futures.size() << "\n";
    bool any_done = false;
    for (auto& f : futures) {
        if (f.wait_for(std::chrono::seconds(0)) == std::future_status::ready) any_done = true;
    }
    std::cout << "done right after the calls: " << (any_done ? "True" : "False") << "\n";
    bool met = meeting.arrive_and_wait();  // the caller is free to do other work here
    std::cout << "all three lookups ran at once: " << (met ? "True" : "False") << "\n";
    std::vector<int> results;
    for (auto& f : futures) results.push_back(f.get());  // wait for each result

    std::cout << "prices: [";
    int total = 0;
    for (size_t i = 0; i < results.size(); i++) {
        std::cout << (i ? ", " : "") << results[i];
        total += results[i];
    }
    std::cout << "]\n";
    std::cout << "total: " << total << "\n";
}
```

```output
lookups started: 3
done right after the calls: False
all three lookups ran at once: True
prices: [12, 7, 30]
total: 49
```

- Verdict: stdlib. `std::async` with `std::launch::async` returns a `std::future<int>` at once; `wait_for` with a time of zero checks it and `get()` waits for the value.
