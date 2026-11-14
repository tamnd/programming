```cpp run id=getline
#include <iostream>
#include <sstream>
#include <string>

// show prints a string with CR written as \r.
static std::string show(const std::string& s) {
    std::string out = "\"";
    for (char c : s) out += c == '\r' ? std::string("\\r") : std::string(1, c);
    return out + "\"";
}

int main() {
    std::istringstream in("one\ntwo\r\nthree\rfour");
    std::string line;
    // getline stops at '\n' and removes it. The CR of a CRLF stays.
    while (std::getline(in, line)) {
        std::cout << show(line);
        if (!line.empty() && line.back() == '\r') line.pop_back();
        std::cout << " -> " << show(line) << '\n';
    }
}
```

```output for=getline
"one" -> "one"
"two\r" -> "two"
"three\rfour" -> "three\rfour"
```

- Verdict: stdlib. `std::getline` stops at `'\n'` and does not store it, so the CR of a CRLF stays at the end of the string, and the program removes it. A CR alone does not end a line.
