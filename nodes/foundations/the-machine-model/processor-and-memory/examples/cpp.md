```cpp run id=machine-in-std-array
#include <array>
#include <cstdint>
#include <iostream>

enum Op : std::int64_t { HALT, LOAD, ADD, SUB, STORE, JZ, JMP };

template <std::size_t N>
int run(std::array<std::int64_t, N>& mem) {
    std::int64_t acc = 0;
    std::size_t pc = 0;
    int steps = 0;
    for (;;) {
        auto op = mem.at(pc);
        auto addr = static_cast<std::size_t>(mem.at(pc + 1));
        pc += 2;
        steps++;
        switch (op) {
        case HALT: return steps;
        case LOAD: acc = mem.at(addr); break;
        case ADD: acc += mem.at(addr); break;
        case SUB: acc -= mem.at(addr); break;
        case STORE: mem.at(addr) = acc; break;
        case JZ: if (acc == 0) pc = addr; break;
        case JMP: pc = addr; break;
        }
    }
}

int main() {
    std::array<std::int64_t, 23> mem{LOAD, 20, JZ, 18, LOAD, 21, ADD, 20, STORE, 21,
                                     LOAD, 20, SUB, 22, STORE, 20, JMP, 0, HALT, 0, 5, 0, 1};
    int steps = run(mem);
    std::cout << "sum=" << mem[21] << " steps=" << steps << '\n';
    std::cout << sizeof(void*) << ' ' << sizeof mem << ' ' << alignof(std::int64_t) << '\n';
}
```

```output for=machine-in-std-array
sum=15 steps=48
8 184 8
```

- Verdict: native. C++ has the memory model of C: a program's memory is a sequence of bytes, each with an address, and `sizeof` and `alignof` show the layout. A `std::array` of 23 64-bit words is exactly 184 bytes with no header. `at` checks each index and throws, while `[]` does not check.
