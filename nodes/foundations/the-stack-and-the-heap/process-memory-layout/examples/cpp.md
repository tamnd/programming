```cpp run id=regions
#include <cstdint>
#include <filesystem>
#include <fstream>
#include <iomanip>
#include <iostream>
#include <memory>
#include <sstream>
#include <string>
#include <vector>

int counter = 42; // a global with a value: the data segment

// Prints the permissions and the kind of the region that holds addr.
void where(const std::string& name, std::uintptr_t addr) {
    const std::string exe = std::filesystem::read_symlink("/proc/self/exe").string();
    std::ifstream maps("/proc/self/maps");
    for (std::string line; std::getline(maps, line);) {
        std::istringstream in(line);
        std::string range, perms, offset, dev, inode, path;
        in >> range >> perms >> offset >> dev >> inode;
        std::getline(in >> std::ws, path);
        const auto dash = range.find('-');
        const auto start = std::stoull(range.substr(0, dash), nullptr, 16);
        const auto end = std::stoull(range.substr(dash + 1), nullptr, 16);
        if (addr >= start && addr < end) {
            std::string kind = path;
            if (path == exe) kind = "the program file";
            else if (path.empty() || path.rfind("[anon", 0) == 0) kind = "an anonymous mapping";
            std::cout << std::left << std::setw(8) << name << " " << perms << " " << kind << "\n";
            return;
        }
    }
}

int main() {
    int here = 1;                             // a local: the stack
    auto small = std::make_unique<int>(7);    // a small block: the heap
    std::vector<char> big(1 << 20);           // 1 MiB, over MMAP_THRESHOLD

    where("code", reinterpret_cast<std::uintptr_t>(&where));
    where("global", reinterpret_cast<std::uintptr_t>(&counter));
    where("local", reinterpret_cast<std::uintptr_t>(&here));
    where("small", reinterpret_cast<std::uintptr_t>(small.get()));
    where("big", reinterpret_cast<std::uintptr_t>(big.data()));
}
```

```output for=regions
code     r-xp the program file
global   rw-p the program file
local    rw-p [stack]
small    rw-p [heap]
big      rw-p an anonymous mapping
```

- Verdict: native. As in C, a pointer is an address. `new` and `std::vector` get their memory from the C library, so the small block is in `[heap]` and the 1 MiB block is in an anonymous mapping.
