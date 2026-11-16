```cpp run id=enum-switch
#include <iostream>
#include <string_view>

enum class Light { Red, Yellow, Green };

std::string_view advice(Light light) {
    switch (light) {
    case Light::Red:
        return "stop";
    case Light::Yellow:
        return "slow down";
    case Light::Green:
        return "go";
    }
    return "broken light";
}

int main() {
    for (Light light : {Light::Red, Light::Yellow, Light::Green}) {
        std::cout << advice(light) << '\n';
    }

    // A case with a declaration needs its own block.
    int n = 2;
    switch (n) {
    case 1: {
        int doubled = n * 2;
        std::cout << doubled << '\n';
        break;
    }
    case 2:
        std::cout << "two\n";
        break;
    default:
        std::cout << "other\n";
        break;
    }
}
```

```output for=enum-switch
stop
slow down
go
two
```

- Verdict: native. A C++ `switch` can test an enumeration, here an `enum class`. The function has one case for each enumerator and no `default`, and the `return` after the `switch` handles a value outside the enumerators. In the second `switch`, the braces give `doubled` its own block, so the jump to `case 2` does not enter its scope.
