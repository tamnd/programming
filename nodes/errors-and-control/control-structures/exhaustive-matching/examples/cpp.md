```cpp run id=exhaustive-matching-cpp
#include <iostream>
#include <string_view>

enum class Light { Red, Yellow, Green };

static std::string_view action(Light light) {
    // No default: -Wswitch (in -Wall) warns if an enumerator has no case.
    switch (light) {
    case Light::Red:
        return "stop";
    case Light::Yellow:
        return "slow down";
    case Light::Green:
        return "go";
    }
    return "unknown";
}

int main() {
    const char *names[] = {"red", "yellow", "green"};
    const Light lights[] = {Light::Red, Light::Yellow, Light::Green};
    for (int i = 0; i < 3; i++) {
        std::cout << names[i] << ": " << action(lights[i]) << "\n";
    }
}
```

```output for=exhaustive-matching-cpp
red: stop
yellow: slow down
green: go
```

- Verdict: idiom. As in C, the language does not require a switch to be exhaustive, and GCC warns with `-Wswitch` when an enumerator has no case and there is no `default`. The `return` after the switch handles a value that is not an enumerator.
