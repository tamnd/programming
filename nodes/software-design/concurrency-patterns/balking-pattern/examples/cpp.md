```cpp run id=balking-pattern
#include <atomic>
#include <functional>
#include <future>
#include <iostream>
#include <mutex>
#include <thread>
#include <vector>

class Document {
public:
    explicit Document(std::function<void()> write) : write_(std::move(write)) {}

    void edit() {
        std::lock_guard<std::mutex> lock(m_);
        changed_ = true;
    }

    // Saves the document. Returns false at once if a save cannot start now.
    bool save() {
        {
            std::lock_guard<std::mutex> lock(m_);
            if (saving_ || !changed_) return false;  // balk: the object is not in the right state
            saving_ = true;
            changed_ = false;
        }
        write_();
        std::lock_guard<std::mutex> lock(m_);
        saving_ = false;
        saves_++;
        return true;
    }

    int saves() {
        std::lock_guard<std::mutex> lock(m_);
        return saves_;
    }

private:
    std::mutex m_;
    std::function<void()> write_;  // the slow part of a save
    bool changed_ = false, saving_ = false;
    int saves_ = 0;
};

int main() {
    std::promise<void> started_p, release_p;
    std::shared_future<void> release = release_p.get_future().share();
    std::atomic<bool> first_write{true};
    Document doc([&] {
        if (first_write.exchange(false)) started_p.set_value();
        release.wait();  // the first save stays in progress until the other calls end
    });
    doc.edit();
    std::thread first([&] { doc.save(); });
    started_p.get_future().wait();

    std::atomic<int> calls{0}, balked{0};
    std::vector<std::thread> callers;
    for (int i = 0; i < 7; i++) {
        callers.emplace_back([&] {
            calls++;
            if (!doc.save()) balked++;
        });
    }
    for (auto& t : callers) t.join();
    release_p.set_value();
    first.join();

    std::cout << "calls during a save: " << calls << "\n";
    std::cout << "calls that balked: " << balked << "\n";
    std::cout << "saves run: " << doc.saves() << "\n";
    std::cout << "save with no changes: " << (doc.save() ? "ran" : "balked") << "\n";
    doc.edit();
    std::cout << "save after an edit: " << (doc.save() ? "ran" : "balked") << "\n";
    std::cout << "saves run: " << doc.saves() << "\n";
}
```

```output
calls during a save: 7
calls that balked: 7
saves run: 1
save with no changes: balked
save after an edit: ran
saves run: 2
```

- Verdict: stdlib. A `std::lock_guard` on a `std::mutex` makes the test and the change of the `saving_` flag one step.
