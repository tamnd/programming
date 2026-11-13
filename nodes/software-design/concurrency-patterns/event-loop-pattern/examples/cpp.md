```cpp run id=event-loop-pattern
#include <deque>
#include <functional>
#include <iostream>
#include <queue>
#include <string>
#include <tuple>
#include <vector>

// One thread takes ready callbacks from a queue and runs each one to the end.
class EventLoop {
public:
    using Callback = std::function<void()>;

    void call_soon(Callback callback) { ready_.push_back(std::move(callback)); }

    void call_later(int delay, Callback callback) {
        timers_.push({now_ + delay, ++order_, std::move(callback)});
    }

    void run() {
        while (!ready_.empty() || !timers_.empty()) {
            if (ready_.empty()) now_ = timers_.top().due;  // a real loop sleeps or waits for I/O here
            while (!timers_.empty() && timers_.top().due <= now_) {
                ready_.push_back(timers_.top().callback);
                timers_.pop();
            }
            turns_++;
            std::size_t count = ready_.size();  // callbacks added now wait for the next turn
            for (std::size_t i = 0; i < count; i++) {
                Callback callback = std::move(ready_.front());
                ready_.pop_front();
                callback();
            }
        }
    }

    int now() const { return now_; }
    int turns() const { return turns_; }

private:
    struct Timer {
        int due, order;
        Callback callback;
        bool operator>(const Timer& o) const { return std::tie(due, order) > std::tie(o.due, o.order); }
    };
    std::deque<Callback> ready_;  // callbacks to run in this turn or the next
    std::priority_queue<Timer, std::vector<Timer>, std::greater<Timer>> timers_;
    int now_ = 0;  // a counter clock, so the output does not depend on the machine
    int order_ = 0;
    int turns_ = 0;
};

int main() {
    EventLoop loop;
    std::vector<std::string> log;
    auto say = [&](std::string text) -> EventLoop::Callback {
        return [&, text] { log.push_back("time " + std::to_string(loop.now()) + ": " + text); };
    };

    loop.call_later(20, say("timer of 20"));
    loop.call_later(10, say("timer of 10"));
    loop.call_soon([&] {
        say("first callback")();
        loop.call_soon(say("callback that the first callback added"));
    });
    loop.call_soon(say("second callback"));
    loop.run();

    std::cout << "order of the callbacks:\n";
    for (const auto& line : log) std::cout << "  " << line << "\n";
    std::cout << "turns of the loop: " << loop.turns() << "\n";
}
```

```output
order of the callbacks:
  time 0: first callback
  time 0: second callback
  time 0: callback that the first callback added
  time 10: timer of 10
  time 20: timer of 20
turns of the loop: 4
```

- Verdict: idiom. A small loop class with a `std::deque` of `std::function` callbacks and a `std::priority_queue` of timers.
