```cpp run id=light
#include <array>
#include <iostream>
#include <string_view>

enum class Light { Red, Amber, Green };

constexpr std::array<Light, 3> all{Light::Red, Light::Amber, Light::Green};

std::string_view name(Light l) {
    switch (l) {
    case Light::Red:
        return "red";
    case Light::Amber:
        return "amber";
    case Light::Green:
        return "green";
    }
    return "no name";
}

Light after(Light l) {
    switch (l) {
    case Light::Red:
        return Light::Green;
    case Light::Green:
        return Light::Amber;
    case Light::Amber:
        return Light::Red;
    }
    return Light::Red;
}

int main() {
    for (Light l : all) {
        std::cout << name(l) << ' ' << static_cast<int>(l) << " then " << name(after(l)) << '\n';
    }
    std::cout << "no name: " << name(static_cast<Light>(7)) << '\n';
}
```

```output for=light
red 0 then green
amber 1 then red
green 2 then amber
no name: no name
```

- Verdict: native. An `enum class` is a scoped enumeration. Its names need the prefix `Light::`, and it does not convert to `int` by itself.
- `static_cast` converts to and from the number, so a value with no name is possible.
- C++ cannot list the values of an enum, so the program keeps an `all` array.
