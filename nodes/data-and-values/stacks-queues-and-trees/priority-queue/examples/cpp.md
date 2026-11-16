```cpp run id=pq
#include <functional>
#include <iostream>
#include <queue>
#include <string>
#include <utility>
#include <vector>

int main() {
    using Task = std::pair<int, std::string>;
    std::priority_queue<Task, std::vector<Task>, std::greater<Task>> tasks;
    tasks.push({3, "update docs"});
    tasks.push({1, "fix outage"});
    tasks.push({2, "review patch"});
    tasks.push({4, "plan sprint"});

    std::cout << "peek: " << tasks.top().second << '\n';
    while (!tasks.empty()) {
        std::cout << tasks.top().first << ' ' << tasks.top().second << '\n';
        tasks.pop();
    }

    std::priority_queue<int> max_heap;
    for (int n : {3, 1, 2, 4}) {
        max_heap.push(n);
    }
    std::cout << "largest first:";
    while (!max_heap.empty()) {
        std::cout << ' ' << max_heap.top();
        max_heap.pop();
    }
    std::cout << '\n';
}
```

```output for=pq
peek: fix outage
1 fix outage
2 review patch
3 update docs
4 plan sprint
largest first: 4 3 2 1
```

- Verdict: stdlib. `std::priority_queue` gives the largest item first. With `std::greater`, it gives the smallest first.
- `top` reads the best item and `pop` removes it. `pop` returns nothing.
