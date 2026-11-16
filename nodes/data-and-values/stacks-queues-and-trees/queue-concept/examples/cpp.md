```cpp run id=queue
#include <initializer_list>
#include <iostream>
#include <queue>
#include <string>

int main() {
    std::queue<std::string> queue;
    for (const char *item : {"a", "b", "c"}) {
        queue.push(item);
    }
    std::cout << "front " << queue.front() << " back " << queue.back()
              << " size " << queue.size() << '\n';

    // pop returns void, so read front first.
    std::string first = queue.front();
    queue.pop();
    std::cout << first << " next " << queue.front() << " size " << queue.size() << '\n';

    queue.pop();
    queue.pop();
    std::cout << "empty: " << (queue.empty() ? "yes" : "no") << '\n';
}
```

```output for=queue
front a back c size 3
a next b size 2
empty: yes
```

- Verdict: stdlib. `std::queue` is an adaptor over `std::deque` by default. `push` adds at the back, and `pop` removes the front.
- `pop` returns nothing, so the program reads `front()` first. It checks `empty()` before `front` or `pop`.
