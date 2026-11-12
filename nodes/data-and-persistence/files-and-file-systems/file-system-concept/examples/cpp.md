```cpp run id=std-filesystem
#include <filesystem>
#include <fstream>
#include <iostream>
#include <set>

namespace fs = std::filesystem;

int main() {
    fs::create_directories("tree/src");
    std::ofstream("tree/src/main.txt") << "main\n";
    std::ofstream("tree/readme.txt") << "read me\n";

    // recursive_directory_iterator walks the tree; the order is not fixed, so use a set.
    std::set<std::string> names;
    for (const auto &e : fs::recursive_directory_iterator("tree")) {
        names.insert(e.path().generic_string() + (e.is_directory() ? "/" : ""));
    }
    for (const auto &n : names) std::cout << n << '\n';

    // equivalent compares the files that two paths name.
    fs::create_hard_link("tree/src/main.txt", "tree/main-link.txt");
    std::cout << std::boolalpha;
    std::cout << "equivalent: " << fs::equivalent("tree/src/main.txt", "tree/main-link.txt") << '\n';
    std::cout << "links: " << fs::hard_link_count("tree/src/main.txt") << '\n';

    // space describes the file system that holds a path.
    fs::space_info s = fs::space("tree");
    std::cout << "available <= capacity: " << (s.available <= s.capacity) << '\n';

    // The overload with std::error_code reports an error without an exception.
    std::error_code ec;
    fs::file_size("tree/missing.txt", ec);
    std::cout << "missing: " << (ec == std::errc::no_such_file_or_directory) << '\n';
}
```

```output for=std-filesystem
tree/readme.txt
tree/src/
tree/src/main.txt
equivalent: true
links: 2
available <= capacity: true
missing: true
```

- Verdict: stdlib. `std::filesystem` has the operations of POSIX in a portable form. `recursive_directory_iterator` walks the tree in no fixed order, so the program puts the names in a `std::set`. `equivalent` compares the files that two paths name, `hard_link_count` counts the names, and `space` describes the file system. Each call has an overload with `std::error_code` that does not throw.
