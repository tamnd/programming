```python run id=vm
from enum import IntEnum


class Op(IntEnum):
    PUSH = 0
    LOAD = 1
    STORE = 2
    ADD = 3
    SUB = 4
    MUL = 5
    LT = 6
    JUMP_IF_FALSE = 7
    JUMP = 8
    PRINT = 9
    HALT = 10


HAS_ARG = {Op.PUSH, Op.LOAD, Op.STORE, Op.JUMP_IF_FALSE, Op.JUMP}

# let n = 5; let acc = 1; while (0 < n) { let step = n; acc = acc * step; n = n - 1; } print acc;
# Slots: n = 0, acc = 1, step = 2.
code = bytes([
    0, 5, 2, 0, 0, 1, 2, 1,
    0, 0, 1, 0, 6, 7, 35,
    1, 0, 2, 2,
    1, 1, 1, 2, 5, 2, 1,
    1, 0, 0, 1, 4, 2, 0,
    8, 8,
    1, 1, 9, 10,
])

BINARY = {
    Op.ADD: lambda a, b: a + b,
    Op.SUB: lambda a, b: a - b,
    Op.MUL: lambda a, b: a * b,
    Op.LT: lambda a, b: 1 if a < b else 0,
}


def disassemble(code):
    pc = 0
    while pc < len(code):
        op = Op(code[pc])
        if op in HAS_ARG:
            print(f"{pc:4} {op.name:<13} {code[pc + 1]}")
            pc += 2
        else:
            print(f"{pc:4} {op.name}")
            pc += 1


def run(code, nslots):
    stack, slots, pc = [], [0] * nslots, 0
    executed = max_depth = 0
    while True:
        op = Op(code[pc])
        executed += 1
        arg = code[pc + 1] if op in HAS_ARG else None
        pc += 2 if op in HAS_ARG else 1
        if op is Op.PUSH:
            stack.append(arg)
        elif op is Op.LOAD:
            stack.append(slots[arg])
        elif op is Op.STORE:
            slots[arg] = stack.pop()
        elif op in BINARY:
            b = stack.pop()  # the right operand is on top
            a = stack.pop()
            stack.append(BINARY[op](a, b))
        elif op is Op.JUMP_IF_FALSE:
            if stack.pop() == 0:
                pc = arg
        elif op is Op.JUMP:
            pc = arg
        elif op is Op.PRINT:
            print(stack.pop())
        elif op is Op.HALT:
            return executed, max_depth
        max_depth = max(max_depth, len(stack))


disassemble(code)
executed, depth = run(code, 3)
print(f"bytes of code: {len(code)}")
print(f"instructions executed: {executed}, maximum stack depth: {depth}")
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

The second program shows the bytecode that CPython itself made for a function. The instruction names and offsets depend on the version of CPython that recorded the output.

```python run id=dis
import dis
import sys


def fact(n):
    acc = 1
    while 0 < n:
        acc = acc * n
        n = n - 1
    return acc


print(f"fact(5) = {fact(5)}")
print(f"bytes of code: {len(fact.__code__.co_code)}")
print(f"local variables: {fact.__code__.co_varnames}")
print(f"version: {sys.version_info.major}.{sys.version_info.minor}")
dis.dis(fact)
```

```output for=dis
fact(5) = 120
bytes of code: 62
local variables: ('n', 'acc')
version: 3.14
  5           RESUME                   0

  6           LOAD_SMALL_INT           1
              STORE_FAST               1 (acc)

  7   L1:     LOAD_SMALL_INT           0
              LOAD_FAST_BORROW         0 (n)
              COMPARE_OP              18 (bool(<))
              POP_JUMP_IF_FALSE       20 (to L2)
              NOT_TAKEN

  8           LOAD_FAST_BORROW_LOAD_FAST_BORROW 16 (acc, n)
              BINARY_OP                5 (*)
              STORE_FAST               1 (acc)

  9           LOAD_FAST_BORROW         0 (n)
              LOAD_SMALL_INT           1
              BINARY_OP               10 (-)
              STORE_FAST               0 (n)
              JUMP_BACKWARD           26 (to L1)

 10   L2:     LOAD_FAST_BORROW         1 (acc)
              RETURN_VALUE
```

- Verdict: stdlib. CPython compiles each function to bytecode in a code object, and the `dis` module in the standard library disassembles it.
- The `dis` output shows a stack machine with numbered local slots. In the recorded output, `LOAD_FAST_BORROW 0 (n)` pushes local slot 0, and `STORE_FAST 1 (acc)` pops into slot 1, as `LOAD` and `STORE` do in the toy machine.
- `fact.__code__.co_code` holds the raw bytes. The documentation calls the bytecode an implementation detail, so a program must not depend on the exact instructions.
- A `bytes` object holds the toy program, so each opcode and each operand is one byte, as in `co_code`.
