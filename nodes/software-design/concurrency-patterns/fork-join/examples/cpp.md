```cpp run id=fork-join
#include <algorithm>
#include <atomic>
#include <future>
#include <iostream>

const long long THRESHOLD = 10000;  // a range this small is summed directly
std::atomic<int> leaves{0};
std::atomic<long long> largest{0};

// Returns the sum of lo, lo + 1, ..., hi - 1.
long long solve(long long lo, long long hi) {
    if (hi - lo <= THRESHOLD) {
        leaves++;
        long long seen = largest.load();
        while (seen < hi - lo && !largest.compare_exchange_weak(seen, hi - lo)) {
        }
        long long s = 0;
        for (long long i = lo; i < hi; i++) s += i;
        return s;
    }
    long long mid = (lo + hi) / 2;
    // Fork: a new thread solves the left half.
    std::future<long long> left = std::async(std::launch::async, solve, lo, mid);
    long long right = solve(mid, hi);  // this thread solves the right half
    return left.get() + right;         // join: wait for the left half
}

int main() {
    long long n = 1000000;
    long long total = solve(1, n + 1);
    std::cout << "sum: " << total << "\n";
    std::cout << "leaf tasks: " << leaves << "\n";
    std::cout << "largest leaf, at most 10000: " << (largest <= THRESHOLD ? "True" : "False") << "\n";
    std::cout << "matches n(n+1)/2: " << (total == n * (n + 1) / 2 ? "True" : "False") << "\n";
}
```

```output
sum: 500000500000
leaf tasks: 128
largest leaf, at most 10000: True
matches n(n+1)/2: True
```

- Verdict: stdlib. `std::async` with `std::launch::async` forks the left half, and `get` on its `std::future` joins it.
