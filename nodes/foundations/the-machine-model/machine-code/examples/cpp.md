```cpp run id=compile-time-assembler
#include <array>
#include <cstdint>
#include <cstdio>

// A compile-time assembler for three ARM64 instructions. Each one is a 32-bit word.
constexpr std::uint32_t movz(unsigned rd, unsigned imm16) { return 0xD2800000u | imm16 << 5 | rd; }
constexpr std::uint32_t add(unsigned rd, unsigned rn, unsigned rm) { return 0x8B000000u | rm << 16 | rn << 5 | rd; }
constexpr std::uint32_t ret() { return 0xD65F03C0u; }

// The compiler checks the encodings against the words that an assembler gives.
static_assert(movz(0, 42) == 0xD2800540u);
static_assert(add(0, 0, 1) == 0x8B010000u);

constexpr std::array<std::uint32_t, 4> program{movz(0, 40), movz(1, 2), add(0, 0, 1), ret()};
constexpr std::array<const char*, 4> text{"movz x0, #40", "movz x1, #2", "add x0, x0, x1", "ret"};

int main() {
    for (std::size_t k = 0; k < program.size(); ++k) {
        std::uint32_t w = program[k];
        // Store the word in little-endian order, as ARM64 processors read it.
        std::printf("%08x ->", static_cast<unsigned>(w));
        for (int i = 0; i < 4; ++i) std::printf(" %02x", static_cast<unsigned>(w >> (8 * i) & 0xFF));
        std::printf("  %s\n", text[k]);
    }
    std::printf("%zu instructions, %zu bytes\n", program.size(), program.size() * 4);
}
```

```output for=compile-time-assembler
d2800500 -> 00 05 80 d2  movz x0, #40
d2800041 -> 41 00 80 d2  movz x1, #2
8b010000 -> 00 00 01 8b  add x0, x0, x1
d65f03c0 -> c0 03 5f d6  ret
4 instructions, 16 bytes
```

- Verdict: native. C++ compiles to machine code and allows `asm` in GCC and Clang. The example writes a small ARM64 assembler with `constexpr` functions, so the compiler builds the program at compile time and `static_assert` checks two encodings against an assembler. Each instruction is 4 bytes, stored in little-endian order.
