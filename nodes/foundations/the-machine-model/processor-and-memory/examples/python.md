```python run id=stored-program-machine
import struct
import sys

HALT, LOAD, ADD, SUB, STORE, JZ, JMP = range(7)


def run(mem):
    pc, acc, steps = 0, 0, 0
    while True:
        op, addr = mem[pc], mem[pc + 1]  # fetch
        pc += 2
        steps += 1
        match op:  # decode and execute; 0 is HALT, 1 is LOAD, and so on
            case 0:
                return steps
            case 1:
                acc = mem[addr]
            case 2:
                acc += mem[addr]
            case 3:
                acc -= mem[addr]
            case 4:
                mem[addr] = acc
            case 5:
                if acc == 0:
                    pc = addr
            case 6:
                pc = addr


mem = [LOAD, 20, JZ, 18, LOAD, 21, ADD, 20, STORE, 21,
       LOAD, 20, SUB, 22, STORE, 20, JMP, 0, HALT, 0,
       5, 0, 1]
steps = run(mem)
print(f"sum={mem[21]} steps={steps}")
print(struct.calcsize("P") * 8, "bit addresses;", sys.byteorder, "endian")
```

```output for=stored-program-machine
sum=15 steps=48
64 bit addresses; little endian
```

- Verdict: stdlib. Python hides the machine: a variable names an object on the heap, an `int` has no fixed width, and the interpreter decides where values live. The example builds the machine as a list and a loop, and `match` decodes each instruction. `struct.calcsize("P")` gives the size of a machine address, and `sys.byteorder` the byte order of the processor that runs the interpreter. The `ctypes`, `struct` and `memoryview` modules of the standard library reach raw memory and machine types when a program needs them.
