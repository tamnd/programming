```cpp run id=getline-in-the-condition-and-a-separator
#include <print>
#include <sstream>
#include <string>
#include <vector>

int main() {
    // getline returns the stream, which converts to false at the end.
    std::istringstream in("north\nsouth\n\neast\n");
    std::string line;
    std::vector<std::string> seen;
    while (std::getline(in, line) && !line.empty())
        seen.push_back(line);

    // A separator that is empty for the first item.
    std::string out;
    const char* sep = "";
    for (const auto& s : seen) {
        out += sep;
        out += s;
        sep = " -> ";
    }
    std::println("{}", out);
    std::println("rest: {}", std::getline(in, line) ? line : "none");
}
```

```output for=getline-in-the-condition-and-a-separator
north -> south
rest: east
```

- Verdict: idiom. `std::getline` reads a line and returns the stream, and the stream converts to `false` at the end of the input, so the read and the test sit in one condition. The loop also stops at the blank line, and `east` stays in the stream. The separator starts empty and changes after the first item, so it appears between the items only.
