```cpp run id=resize-file
#include <filesystem>
#include <fstream>
#include <iostream>
#include <sys/stat.h>

namespace fs = std::filesystem;

int main() {
    fs::path dir = fs::temp_directory_path() / "sparse-cpp-example";
    fs::create_directories(dir);
    fs::path file = dir / "sparse.bin";

    std::ofstream(file) << "head";
    // A larger size with no write: the new area reads as zeros.
    fs::resize_file(file, 128 * 1024 * 1024);

    std::cout << "file_size:       " << fs::file_size(file) << '\n';

    // std::filesystem has no allocated size, so the example asks POSIX stat.
    struct stat st {};
    stat(file.c_str(), &st);
    std::cout << "uses less space: " << std::boolalpha
              << (static_cast<long long>(st.st_blocks) * 512 < static_cast<long long>(st.st_size)) << '\n';

    std::ifstream in(file, std::ios::binary);
    in.seekg(64 * 1024 * 1024);
    std::cout << "byte at 64 MiB:  " << in.get() << '\n';

    in.close();
    fs::remove_all(dir);
}
```

```output for=resize-file
file_size:       134217728
uses less space: true
byte at 64 MiB:  0
```

- Verdict: stdlib. `std::filesystem::resize_file` makes a file longer, and the new area reads as zeros. The library has no call for the allocated size or for `SEEK_DATA` and `SEEK_HOLE`, so a program uses the POSIX calls.
