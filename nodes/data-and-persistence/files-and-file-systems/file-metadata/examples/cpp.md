```cpp run id=status-and-symlink-status
#include <filesystem>
#include <fstream>
#include <iostream>

namespace fs = std::filesystem;

static const char *kind(fs::file_type t) {
    switch (t) {
    case fs::file_type::regular: return "regular";
    case fs::file_type::directory: return "directory";
    case fs::file_type::symlink: return "symlink";
    case fs::file_type::not_found: return "not_found";
    default: return "other";
    }
}

int main() {
    std::ofstream("data.txt") << "hello";
    fs::permissions("data.txt", fs::perms::owner_read | fs::perms::owner_write |
                                    fs::perms::group_read);
    fs::create_hard_link("data.txt", "second-name.txt");
    fs::create_symlink("data.txt", "link");
    fs::create_symlink("missing.txt", "dangling");

    // status follows the link, symlink_status does not.
    fs::file_status st = fs::status("link");
    fs::file_status lst = fs::symlink_status("link");
    std::cout << "status:         " << kind(st.type()) << ", perms " << std::oct
              << static_cast<unsigned>(st.permissions()) << std::dec << "\n";
    std::cout << "symlink_status: " << kind(lst.type()) << "\n";

    // The other attributes come from separate calls.
    std::cout << "file_size " << fs::file_size("link") << ", hard_link_count "
              << fs::hard_link_count("data.txt") << "\n";
    std::cout << "equivalent: " << fs::equivalent("data.txt", "second-name.txt") << "\n";

    // A missing target is a status, not an exception.
    std::cout << "dangling: " << kind(fs::status("dangling").type()) << " / "
              << kind(fs::symlink_status("dangling").type()) << "\n";
}
```

```output for=status-and-symlink-status
status:         regular, perms 640
symlink_status: symlink
file_size 5, hard_link_count 2
equivalent: 1
dangling: not_found / symlink
```

- Verdict: stdlib. `std::filesystem::status` returns a `file_status`, which holds only the file type and the permissions, and `symlink_status` does the same without following a link. The size, the link count and the time of the last write come from the separate functions `file_size`, `hard_link_count` and `last_write_time`. A missing file gives the type `file_type::not_found`, not an exception. The standard has no owner, no inode number and no extended attributes.
