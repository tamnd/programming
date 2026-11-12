```cpp run id=active
#include <condition_variable>
#include <functional>
#include <future>
#include <iostream>
#include <mutex>
#include <queue>
#include <thread>
#include <vector>

// The servant: only the scheduler thread calls it, so it has no lock.
struct AccountServant {
    int total = 0;
    int deposit(int amount) { return total += amount; }
    int balance() const { return total; }
};

// The proxy: each method makes a request and returns a future at once.
class AccountProxy {
public:
    AccountProxy() : scheduler_([this] { schedule(); }) {}

    std::future<int> deposit(int amount) {
        return send([amount](AccountServant& s) { return s.deposit(amount); });
    }
    std::future<int> balance() {
        return send([](AccountServant& s) { return s.balance(); });
    }
    void open_gate() {  // lets the demo hold the scheduler
        std::lock_guard<std::mutex> guard(mutex_);
        open_ = true;
        ready_.notify_one();
    }
    ~AccountProxy() {
        {
            std::lock_guard<std::mutex> guard(mutex_);
            stop_ = true;
            ready_.notify_one();
        }
        scheduler_.join();
    }

private:
    using Request = std::function<void(AccountServant&)>;

    std::future<int> send(std::function<int(AccountServant&)> call) {
        auto future = std::make_shared<std::promise<int>>();
        auto result = future->get_future();
        std::lock_guard<std::mutex> guard(mutex_);
        activation_queue_.push([call, future](AccountServant& s) { future->set_value(call(s)); });
        ready_.notify_one();
        return result;
    }

    void schedule() {
        for (;;) {
            std::unique_lock<std::mutex> lock(mutex_);
            ready_.wait(lock, [this] { return stop_ || (open_ && !activation_queue_.empty()); });
            if (activation_queue_.empty() || !open_) return;
            Request request = std::move(activation_queue_.front());
            activation_queue_.pop();
            lock.unlock();
            request(servant_);
        }
    }

    AccountServant servant_;
    std::queue<Request> activation_queue_;
    std::mutex mutex_;
    std::condition_variable ready_;
    bool open_ = false, stop_ = false;
    std::thread scheduler_;
};

int main() {
    AccountProxy account;
    std::vector<std::vector<std::future<int>>> per_client(4);
    std::vector<std::thread> clients;
    for (auto& mine : per_client)
        clients.emplace_back([&account, &mine] {
            for (int i = 0; i < 250; ++i) mine.push_back(account.deposit(1));  // returns at once
        });
    for (auto& t : clients) t.join();
    int queued = 0, done = 0;
    for (auto& mine : per_client)
        for (auto& f : mine) {
            ++queued;
            if (f.wait_for(std::chrono::seconds(0)) == std::future_status::ready) ++done;
        }
    std::cout << "deposits queued: " << queued << "\n";
    std::cout << "deposits done before the scheduler starts: " << done << "\n";
    account.open_gate();
    std::cout << "balance after 1000 deposits: " << account.balance().get() << "\n";
}
```

```output
deposits queued: 1000
deposits done before the scheduler starts: 0
balance after 1000 deposits: 1000
```

- Verdict: idiom. The standard library gives `std::promise` and `std::future`, and the example builds the activation queue from a mutex, a condition variable and `std::queue`.
