```cpp run id=variant-actor
#include <condition_variable>
#include <future>
#include <iostream>
#include <mutex>
#include <queue>
#include <thread>
#include <variant>
#include <vector>

struct Deposit { int amount; };
struct Withdraw { int amount; };
struct Balance { std::promise<int> reply; };
struct Stop {};
using Msg = std::variant<Deposit, Withdraw, Balance, Stop>;

class Account {
public:
    Account() : thread_([this] { loop(); }) {}
    ~Account() { send(Stop{}); }

    void send(Msg m) {
        { std::lock_guard g(mu_); box_.push(std::move(m)); }
        cv_.notify_one();
    }

private:
    void loop() {
        for (;;) {
            Msg m;
            {
                std::unique_lock g(mu_);
                cv_.wait(g, [this] { return !box_.empty(); });
                m = std::move(box_.front());
                box_.pop();
            }
            if (std::holds_alternative<Stop>(m)) return;
            if (auto* d = std::get_if<Deposit>(&m)) balance_ += d->amount;
            if (auto* w = std::get_if<Withdraw>(&m); w && w->amount <= balance_) balance_ -= w->amount;
            if (auto* b = std::get_if<Balance>(&m)) b->reply.set_value(balance_);
        }
    }

    int balance_ = 0; // only the actor's thread uses this member
    std::mutex mu_;
    std::condition_variable cv_;
    std::queue<Msg> box_;
    std::jthread thread_; // declared last, so it starts after the other members exist
};

int main() {
    Account account;
    {
        std::vector<std::jthread> clients;
        for (int i = 0; i < 4; i++)
            clients.emplace_back([&] { for (int j = 0; j < 25; j++) account.send(Deposit{10}); });
    }
    account.send(Withdraw{1});
    std::promise<int> p;
    auto f = p.get_future();
    account.send(Balance{std::move(p)});
    std::cout << "balance " << f.get() << "\n";
}
```

```output for=variant-actor
balance 999
```

- Verdict: idiom. The standard library has threads, mutexes, condition variables, `std::variant` for messages and `std::promise` for replies, but no actor type. The C++ Actor Framework (CAF) is a full actor library.
- The order of the members matters. `thread_` starts `loop` as soon as it is built, so it must be the last member. The destructor sends `Stop`, and then the destructor of `std::jthread` joins the thread.
