```cpp run id=filesystem-links
#include <filesystem>
#include <fstream>
#include <iostream>

namespace fs = std::filesystem;

int main() {
    std::ofstream("data.txt") << "alpha\n";
    fs::create_hard_link("data.txt", "hard.txt");
    fs::create_symlink("data.txt", "soft.txt");

    std::cout << std::boolalpha;
    std::cout << "hard_link_count: " << fs::hard_link_count("data.txt") << '\n';
    std::cout << "equivalent:      " << fs::equivalent("hard.txt", "soft.txt") << '\n';
    std::cout << "is_symlink:      " << fs::is_symlink("soft.txt") << '\n';
    std::cout << "read_symlink:    " << fs::read_symlink("soft.txt") << '\n';

    fs::remove("data.txt");
    std::cout << "exists:          " << fs::exists("soft.txt") << '\n';
    std::cout << "link there:      " << fs::exists(fs::symlink_status("soft.txt")) << '\n';
}
```

```output for=filesystem-links
hard_link_count: 2
equivalent:      true
is_symlink:      true
read_symlink:    "data.txt"
exists:          false
link there:      true
```

- Verdict: stdlib. `std::filesystem` has `create_hard_link`, `create_symlink`, `create_directory_symlink`, `read_symlink`, `hard_link_count` and `equivalent`. `exists` and `status` follow a link, and `symlink_status` and `is_symlink` look at the link itself.
