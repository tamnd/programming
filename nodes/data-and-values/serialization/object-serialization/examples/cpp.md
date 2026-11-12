```cpp run id=quoted-text-format
#include <iomanip>
#include <iostream>
#include <sstream>
#include <string>
#include <vector>

struct Person {
    std::string name;
    int born = 0;
    std::vector<std::string> tags;
    bool operator==(const Person&) const = default;
};

std::ostream& operator<<(std::ostream& os, const Person& p) {
    os << std::quoted(p.name) << ' ' << p.born << ' ' << p.tags.size();
    for (const auto& t : p.tags) os << ' ' << std::quoted(t);
    return os;
}

std::istream& operator>>(std::istream& is, Person& p) {
    std::size_t n = 0;
    is >> std::quoted(p.name) >> p.born >> n;
    p.tags.assign(n, "");
    for (auto& t : p.tags) is >> std::quoted(t);
    return is;
}

int main() {
    Person grace{"Grace \"Amazing\" Hopper", 1906, {"navy", "COBOL compilers"}};
    std::stringstream text;
    text << grace;
    std::cout << text.str() << '\n';
    Person back;
    text >> back;
    std::cout << "round trip: " << std::boolalpha << (back == grace) << '\n';

    std::stringstream raw; // no quoting: a space ends the field
    raw << grace.name << ' ' << grace.born;
    std::string name;
    int born = 0;
    raw >> name >> born;
    std::cout << "without quoting: name " << std::quoted(name) << ", born " << born
              << ", failed " << raw.fail() << '\n';
}
```

```output for=quoted-text-format
"Grace \"Amazing\" Hopper" 1906 2 "navy" "COBOL compilers"
round trip: true
without quoting: name "Grace", born 0, failed true
```

- Verdict: idiom. The standard library has streams but no serialization. The idiom defines `operator<<` and `operator>>` for each type, and `std::quoted` writes strings with quotes and escapes so that spaces and quotes survive. Without quoting, a space ends the name and the next read fails. Larger programs use libraries such as Boost.Serialization, cereal or Protocol Buffers.
