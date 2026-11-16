```cpp run id=ring
#include <array>
#include <cstddef>
#include <initializer_list>
#include <iostream>
#include <optional>

template <typename T, std::size_t N>
class RingBuffer {
public:
    void push(const T& item) {
        items_[(start_ + count_) % N] = item;
        if (count_ < N) {
            ++count_;
        } else {
            start_ = (start_ + 1) % N;
        }
    }

    std::optional<T> pop() {
        if (count_ == 0) {
            return std::nullopt;
        }
        T item = items_[start_];
        start_ = (start_ + 1) % N;
        --count_;
        return item;
    }

    void print() const {
        for (std::size_t i = 0; i < count_; ++i) {
            std::cout << ' ' << items_[(start_ + i) % N];
        }
        std::cout << '\n';
    }

private:
    std::array<T, N> items_{};
    std::size_t start_ = 0;
    std::size_t count_ = 0;
};

int main() {
    RingBuffer<int, 3> ring;
    for (int reading : {10, 20, 30, 40, 50}) {
        ring.push(reading);
        std::cout << "after " << reading << ':';
        ring.print();
    }

    if (auto oldest = ring.pop()) {
        std::cout << "read " << *oldest << ", left:";
        ring.print();
    }
}
```

```output for=ring
after 10: 10
after 20: 10 20
after 30: 10 20 30
after 40: 20 30 40
after 50: 30 40 50
read 30, left: 40 50
```

- Verdict: idiom. The standard library has no ring buffer. This class template keeps a `std::array` of fixed size `N`.
- `pop` returns an empty `std::optional` when the buffer is empty.
