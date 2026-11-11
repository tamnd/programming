```cpp run id=jobs
#include <iostream>
#include <stdexcept>
#include <vector>

struct Cleanup {
    int job;
    ~Cleanup() { std::cout << "cleanup job " << job << '\n'; }
};

int run_job(int job, const std::vector<int>& items) {
    Cleanup guard{job};
    return items.at(job) * 10;
}

int main() {
    std::vector<int> items{1, 2};
    for (int job : {0, 5, 1}) {
        try {
            int result = run_job(job, items);
            std::cout << "job " << job << " -> " << result << '\n';
        } catch (const std::out_of_range&) {
            std::cout << "job " << job << " failed: out_of_range\n";
        }
    }
}
```

```output
cleanup job 0
job 0 -> 10
cleanup job 5
job 5 failed: out_of_range
cleanup job 1
job 1 -> 20
```

- Verdict: idiom. C++ has no panic. `vector::at` checks the index and throws `std::out_of_range`, and the destructor of `guard` runs while the stack unwinds.
- `vector::operator[]` does not check the index. The standard gives `at` a Throws clause and gives `operator[]` none. A bad index there reads memory outside the vector, and no handler can catch that.
- `std::terminate` is the hard stop of the exception system. Its default handler calls `std::abort`.

An exception that leaves a `noexcept` function calls `std::terminate`. No handler outside the function gets a chance to run.

```cpp fails expect=runtime id=noexcept
#include <iostream>
#include <stdexcept>
#include <vector>

int run_job(int job, const std::vector<int>& items) noexcept {
    return items.at(job) * 10;
}

int main() {
    std::vector<int> items{1, 2};
    try {
        std::cout << run_job(5, items) << '\n';
    } catch (const std::exception&) {
        std::cout << "caught\n";
    }
}
```

With `operator[]`, the same bad index reads memory outside the vector. AddressSanitizer reports it.

```cpp fails expect=sanitizer:address id=unchecked
#include <iostream>
#include <vector>

int main() {
    std::vector<int> items{1, 2};
    std::cout << items[5] * 10 << '\n';
}
```
