```cpp run id=vm
#include <algorithm>
#include <array>
#include <cstdint>
#include <cstdio>
#include <stdexcept>
#include <string_view>
#include <vector>

enum class Op : std::uint8_t { Push, Load, Store, Add, Sub, Mul, Lt, JumpIfFalse, Jump, Print, Halt };

constexpr std::array<std::string_view, 11> names{
    "PUSH", "LOAD", "STORE", "ADD", "SUB", "MUL", "LT", "JUMP_IF_FALSE", "JUMP", "PRINT", "HALT"};

constexpr bool has_arg(Op op) {
    return op == Op::Push || op == Op::Load || op == Op::Store || op == Op::JumpIfFalse || op == Op::Jump;
}

// let n = 5; let acc = 1; while (0 < n) { let step = n; acc = acc * step; n = n - 1; } print acc;
// Slots: n = 0, acc = 1, step = 2.
constexpr std::array<std::uint8_t, 39> code{
    0, 5, 2, 0, 0, 1, 2, 1,
    0, 0, 1, 0, 6, 7, 35,
    1, 0, 2, 2,
    1, 1, 1, 2, 5, 2, 1,
    1, 0, 0, 1, 4, 2, 0,
    8, 8,
    1, 1, 9, 10,
};

void disassemble(const auto& code) {
    for (std::size_t pc = 0; pc < code.size();) {
        auto op = static_cast<Op>(code[pc]);
        auto name = names.at(code[pc]);
        if (has_arg(op)) {
            std::printf("%4zu %-13.*s %d\n", pc, static_cast<int>(name.size()), name.data(), code[pc + 1]);
            pc += 2;
        } else {
            std::printf("%4zu %.*s\n", pc, static_cast<int>(name.size()), name.data());
            pc += 1;
        }
    }
}

struct Stats {
    int executed = 0;
    std::size_t max_depth = 0;
};

Stats run(const auto& code, std::size_t nslots) {
    std::vector<long> stack;
    std::vector<long> slots(nslots);
    auto pop = [&stack] {
        if (stack.empty()) throw std::runtime_error("stack underflow");
        long v = stack.back();
        stack.pop_back();
        return v;
    };
    Stats stats;
    std::size_t pc = 0;
    for (;;) {
        auto op = static_cast<Op>(code[pc]);
        ++stats.executed;
        std::size_t arg = has_arg(op) ? code[pc + 1] : 0;
        pc += has_arg(op) ? 2 : 1;
        switch (op) {
        case Op::Push: stack.push_back(static_cast<long>(arg)); break;
        case Op::Load: stack.push_back(slots[arg]); break;
        case Op::Store: slots[arg] = pop(); break;
        case Op::Add: case Op::Sub: case Op::Mul: case Op::Lt: {
            long b = pop(); // the right operand is on top
            long a = pop();
            stack.push_back(op == Op::Add ? a + b : op == Op::Sub ? a - b : op == Op::Mul ? a * b : a < b);
            break;
        }
        case Op::JumpIfFalse: if (pop() == 0) pc = arg; break;
        case Op::Jump: pc = arg; break;
        case Op::Print: std::printf("%ld\n", pop()); break;
        case Op::Halt: return stats;
        }
        stats.max_depth = std::max(stats.max_depth, stack.size());
    }
}

int main() {
    disassemble(code);
    Stats stats = run(code, 3);
    std::printf("bytes of code: %zu\n", code.size());
    std::printf("instructions executed: %d, maximum stack depth: %zu\n", stats.executed, stats.max_depth);
}
```

```output for=vm
   0 PUSH          5
   2 STORE         0
   4 PUSH          1
   6 STORE         1
   8 PUSH          0
  10 LOAD          0
  12 LT
  13 JUMP_IF_FALSE 35
  15 LOAD          0
  17 STORE         2
  19 LOAD          1
  21 LOAD          2
  23 MUL
  24 STORE         1
  26 LOAD          0
  28 PUSH          1
  30 SUB
  31 STORE         0
  33 JUMP          8
  35 LOAD          1
  37 PRINT
  38 HALT
120
bytes of code: 39
instructions executed: 86, maximum stack depth: 2
```

- Verdict: idiom. C++ compiles ahead of time to machine code, so the program builds its own bytecode.
- `enum class Op : std::uint8_t` fixes the size of an opcode at one byte. `static_cast<Op>` turns a byte into an opcode without a check, so `names.at` does the range check in the disassembler.
- `pop` checks for an empty stack and throws, where the C version reads out of bounds.
- `auto` parameters make `disassemble` and `run` templates, so they take the `std::array` without a fixed length in the signature.
