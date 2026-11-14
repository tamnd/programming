```cpp run id=getline-fields
#include <iostream>
#include <sstream>
#include <string>
#include <vector>

int main() {
    // getline with ':' reads one field at a time.
    std::istringstream in("a::b:");
    std::vector<std::string> fields;
    std::string f;
    while (std::getline(in, f, ':')) {
        fields.push_back(f);
    }
    // The empty field after the last ':' is lost: getline fails at the end.
    std::cout << fields.size() << " fields:";
    for (const auto& s : fields) std::cout << " \"" << s << "\"";
    std::cout << "\n";
}
```

```output for=getline-fields
3 fields: "a" "" "b"
```

- Verdict: stdlib. `std::getline` with a delimiter reads one field at a time and keeps empty fields between delimiters. After the last `:` it extracts no characters and fails, so the loop loses the empty last field. Count the delimiters, or test whether the line ends with one, to keep it.
