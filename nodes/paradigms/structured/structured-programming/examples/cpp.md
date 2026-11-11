```cpp run id=raii
#include <fstream>
#include <iostream>
#include <optional>
#include <utility>
#include <vector>

std::optional<std::pair<int, int>> first_negative(const std::vector<std::vector<int>>& grid) {
    for (int r = 0; r < static_cast<int>(grid.size()); ++r) {
        for (int c = 0; c < static_cast<int>(grid[r].size()); ++c) {
            if (grid[r][c] < 0) {
                return std::pair{r, c};
            }
        }
    }
    return std::nullopt;
}

bool write_squares(const char* path) {
    std::ofstream file(path);
    if (!file) {
        return false;
    }
    std::vector<int> numbers;
    for (int i = 0; i < 5; ++i) {
        numbers.push_back(i * i);
        file << numbers.back() << "\n";
    }
    return static_cast<bool>(file);
}

int main() {
    auto found = first_negative({{3, 1}, {4, -1}, {-5, 9}});
    std::cout << found->first << "," << found->second << "\n";
    std::cout << std::boolalpha << write_squares("squares.txt") << " " << write_squares("no-such-dir/x") << "\n";
}
```

```output for=raii
1,1
true false
```

- Verdict: native. C++ has the C statements, including `goto`. Destructors run on every exit from a scope, so an early `return` releases resources with no cleanup labels.

```cpp fails expect=compile id=skip-init
#include <iostream>
#include <string>

int main(int argc, char**) {
    if (argc > 5) {
        goto done;
    }
    std::string name = "unused";
done:
    std::cout << "end\n";
}
```

The compiler rejects the jump, because it would skip the initialization of `name`, and the destructor of `name` would then run on an object that was never made.
