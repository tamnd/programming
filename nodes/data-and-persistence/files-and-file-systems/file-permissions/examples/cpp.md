```cpp run id=perms-and-perm-options
#include <filesystem>
#include <fstream>
#include <iostream>
#include <string>

namespace fs = std::filesystem;

static std::string text(fs::perms p) {
    std::string s;
    for (auto [bit, c] : {std::pair{fs::perms::owner_read, 'r'}, {fs::perms::owner_write, 'w'},
                          {fs::perms::owner_exec, 'x'}, {fs::perms::group_read, 'r'},
                          {fs::perms::group_write, 'w'}, {fs::perms::group_exec, 'x'},
                          {fs::perms::others_read, 'r'}, {fs::perms::others_write, 'w'},
                          {fs::perms::others_exec, 'x'}})
        s += (p & bit) != fs::perms::none ? c : '-';
    return s;
}

int main() {
    std::ofstream("data.txt") << "x";
    using fs::perms;

    // replace sets all of the bits, as chmod 640 does.
    fs::permissions("data.txt", perms::owner_read | perms::owner_write | perms::group_read);
    std::cout << "replace: " << text(fs::status("data.txt").permissions()) << "\n";

    // add and remove change only the given bits, as chmod u+x and g-r do.
    fs::permissions("data.txt", perms::owner_exec, fs::perm_options::add);
    std::cout << "add:     " << text(fs::status("data.txt").permissions()) << "\n";
    fs::permissions("data.txt", perms::group_read, fs::perm_options::remove);
    std::cout << "remove:  " << text(fs::status("data.txt").permissions()) << "\n";

    // perms has the octal values of POSIX.
    std::cout << "octal:   " << std::oct << static_cast<unsigned>(perms::owner_all) << " "
              << static_cast<unsigned>(perms::set_uid) << "\n";
}
```

```output for=perms-and-perm-options
replace: rw-r-----
add:     rwxr-----
remove:  rwx------
octal:   700 4000
```

- Verdict: stdlib. `std::filesystem::perms` is a bitmask type whose values are the POSIX bits, such as `owner_read` with the value 0400 and `set_uid` with the value 04000. `permissions` takes a `perm_options` value: `replace` sets all of the bits, and `add` and `remove` change only the given bits, so the program does not read the mode first. The standard has no umask and no ACLs.
