```cpp run id=scheduler-pattern
#include <algorithm>
#include <atomic>
#include <chrono>
#include <condition_variable>
#include <iostream>
#include <mutex>
#include <string>
#include <thread>
#include <tuple>
#include <vector>

// Decides which waiting thread uses the printer next.
class Scheduler {
public:
    void enter(const std::string& name, int priority) {
        std::unique_lock<std::mutex> lock(m_);
        if (!busy_ && waiting_.empty()) {
            busy_ = true;
            return;
        }
        Entry entry{priority, ++arrivals_, name};
        waiting_.push_back(entry);
        cv_.notify_all();
        // Sleep until the scheduler chooses this thread.
        cv_.wait(lock, [&] { return chosen_ == entry.arrival; });
        chosen_ = 0;
    }

    void done() {
        std::lock_guard<std::mutex> lock(m_);
        if (waiting_.empty()) {
            busy_ = false;
            return;
        }
        // The policy: the lowest priority number first, then the first to arrive.
        auto it = std::min_element(waiting_.begin(), waiting_.end(), [](const Entry& a, const Entry& b) {
            return std::tie(a.priority, a.arrival) < std::tie(b.priority, b.arrival);
        });
        chosen_ = it->arrival;  // the printer stays busy and passes to that thread
        waiting_.erase(it);
        cv_.notify_all();
    }

    size_t wait_until_waiting(size_t n) {
        std::unique_lock<std::mutex> lock(m_);
        cv_.wait_for(lock, std::chrono::seconds(5), [&] { return waiting_.size() >= n; });
        return waiting_.size();
    }

private:
    struct Entry {
        int priority;
        int arrival;
        std::string name;
    };
    std::mutex m_;
    std::condition_variable cv_;
    bool busy_ = false;
    std::vector<Entry> waiting_;
    int arrivals_ = 0;
    int chosen_ = 0;  // the arrival number of the chosen entry
};

Scheduler scheduler;
std::mutex order_m;
std::vector<std::string> order;
std::atomic<int> on_printer{0};
int most_on_printer = 0;

void use_printer(const std::string& name) {
    int now = ++on_printer;
    {
        std::lock_guard<std::mutex> lock(order_m);
        most_on_printer = std::max(most_on_printer, now);
        order.push_back(name);
    }
    --on_printer;
}

int main() {
    scheduler.enter("first", 0);  // the first job takes the free printer
    use_printer("first");
    std::vector<std::pair<std::string, int>> jobs = {{"report", 2}, {"invoice", 1}, {"photo", 3}, {"label", 1}};
    std::vector<std::thread> threads;
    for (size_t i = 0; i < jobs.size(); i++) {
        auto job = jobs[i];
        threads.emplace_back([job] {
            scheduler.enter(job.first, job.second);
            use_printer(job.first);
            scheduler.done();
        });
        scheduler.wait_until_waiting(i + 1);  // start the jobs in a known order
    }
    std::cout << "jobs waiting when the first job ends: " << scheduler.wait_until_waiting(jobs.size()) << "\n";
    scheduler.done();
    for (auto& t : threads) t.join();

    std::cout << "print order: ";
    for (size_t i = 0; i < order.size(); i++) std::cout << (i ? ", " : "") << order[i];
    std::cout << "\n";
    std::cout << "jobs on the printer at once, at most: " << most_on_printer << "\n";
}
```

```output
jobs waiting when the first job ends: 4
print order: first, invoice, label, report, photo
jobs on the printer at once, at most: 1
```

- Verdict: idiom. A class with `std::mutex` and `std::condition_variable`; `enter` waits with a predicate until `done` chooses its arrival number.
